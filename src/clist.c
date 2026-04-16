
#include "allocator.h"
#include "clist.h"
#include <stdlib.h>


typedef struct clist_node_s clist_node_t;

struct clist_iterator_s
{
    clist_node_t  *node_p;
};

struct clist_node_s
{
    void              *member_p;

    clist_node_t      *prev,
                      *next;

    clist_t           *ll_p;
    clist_iterator_t   itr;
};

struct clist_s
{
    allocator_t     allocator;

    clist_node_t    head,
                    tail;

    unsigned int    count;

    member_clone_f  mclone_fn;
    member_free_f   mfree_fn;
    member_cmp_f    mcmp_fn;

    size_t          member_size;
};


#define clist_malloc(ll_p, size)  ll_p->allocator.malloc(size)
#define clist_free(ll_p, ptr)                \
    do                                       \
    {                                        \
        free_f  ffn = ll_p->allocator.free;  \
        ffn(ptr);                            \
    }                                        \
    while (0)


static clist_node_t* clist_node_new (clist_t *ll_p, void *member_p);


static clist_node_t* clist_node_new (clist_t *ll_p, void *member_p)
{
    clist_node_t  *nn_p = clist_malloc(ll_p, sizeof(clist_node_t));

    if (nn_p)
    {
        nn_p->ll_p = ll_p;
        nn_p->next = NULL;
        nn_p->prev = NULL;
        nn_p->itr.node_p = nn_p;

        nn_p->member_p = member_p;
    }

    return nn_p;
}


clist_t* clist_new (const allocator_t  *allocator_p)
{
    clist_t   *ll_p = NULL;

    malloc_f   mfn;
    free_f     ffn;

    if (   allocator_p
        && allocator_p->malloc
        && allocator_p->free
       )
    {
        mfn = allocator_p->malloc;
        ffn = allocator_p->free;
    }
    else
    {
        mfn = malloc;
        ffn = free;
    }

    ll_p = mfn(sizeof(clist_t));

    if (ll_p)
    {
        ll_p->allocator.malloc = mfn;
        ll_p->allocator.free = ffn;
        ll_p->allocator.calloc = NULL;
        ll_p->allocator.realloc = NULL;

        ll_p->head.member_p = NULL;
        ll_p->head.prev = NULL;
        ll_p->head.next = &ll_p->tail;
        ll_p->head.ll_p = ll_p;
        ll_p->head.itr.node_p = &ll_p->head;

        ll_p->tail.member_p = NULL;
        ll_p->tail.prev = &ll_p->head;
        ll_p->tail.next = NULL;
        ll_p->tail.ll_p = ll_p;
        ll_p->tail.itr.node_p = &ll_p->tail;

        ll_p->count = 0;

        ll_p->mclone_fn = member_clone;
        ll_p->mfree_fn  = member_free;
        ll_p->mcmp_fn   = member_cmp;

        ll_p->member_size = 0;
    }

    return ll_p;
}

clist_t* clist_del (clist_t *ll_p)
{
    if (ll_p)
    {
        clist_clear(ll_p);

        clist_free(ll_p, ll_p);
        ll_p = NULL;
    }

    return ll_p;
}

void clist_set_member_funcs (clist_t         *ll_p,
                             member_clone_f   mclone_fn,
                             member_free_f    mfree_fn,
                             member_cmp_f     mcmp_fn)
{
    if (ll_p)
    {
        if (mclone_fn)  ll_p->mclone_fn = mclone_fn;
        if (mfree_fn)   ll_p->mfree_fn  = mfree_fn;
        if (mcmp_fn)    ll_p->mcmp_fn   = mcmp_fn;
    }
}

void clist_set_member_size (clist_t *ll_p, size_t member_size)
{
    if (ll_p)
        ll_p->member_size = member_size;
}

unsigned int clist_count (clist_t *ll_p)
{
    unsigned int  count = 0;

    if (ll_p)
        count = ll_p->count;

    return count;
}

int clist_empty (clist_t *ll_p)
{
    return   clist_count(ll_p) == 0
           ? 1
           : 0;
}

int clist_push_back_new (clist_t *ll_p, void *member_p)
{
    return clist_insert_before_new(ll_p, clist_end(ll_p), member_p);
}

int clist_push_back (clist_t *ll_p, void *member_p)
{
    return clist_insert_before(ll_p, clist_end(ll_p), member_p);
}

int clist_push_front_new (clist_t *ll_p, void *member_p)
{
    return clist_insert_before_new(ll_p, clist_begin(ll_p), member_p);
}

int clist_push_front (clist_t *ll_p, void *member_p)
{
    return clist_insert_before(ll_p, clist_begin(ll_p), member_p);
}

int clist_insert_before_new (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
{
    int  ret = 1;

    if (   ll_p
        && itr_p
        && itr_p->node_p->ll_p == ll_p
        && itr_p != &ll_p->head.itr
       )
    {
        clist_node_t  *nn_p = clist_node_new(ll_p, member_p);

        if (nn_p)
        {
            nn_p->prev = itr_p->node_p->prev;
            nn_p->next = itr_p->node_p;

            nn_p->prev->next = nn_p;
            nn_p->next->prev = nn_p;

            ll_p->count++;

            ret = 0;
        }
    }

    return ret;
}

int clist_insert_before (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
{
    int  ret = 1;

    if (ll_p)
    {
        void  *mclone_p = ll_p->mclone_fn(&ll_p->allocator, member_p, ll_p->member_size);

        if (   !member_p
            ||  mclone_p
           )
        {
            ret = clist_insert_before_new(ll_p, itr_p, mclone_p);
        }

        if (1 == ret)
        {
            if (mclone_p)
                ll_p->mfree_fn(&ll_p->allocator, mclone_p);
        }
    }

    return ret;
}

int clist_insert_after_new (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
{
    int  ret = 1;

    if (   ll_p
        && itr_p
        && itr_p->node_p->ll_p == ll_p
        && itr_p != clist_end(ll_p)
       )
    {
        clist_node_t  *nn_p = clist_node_new(ll_p, member_p);

        if (nn_p)
        {
            nn_p->prev = itr_p->node_p;
            nn_p->next = itr_p->node_p->next;

            nn_p->prev->next = nn_p;
            nn_p->next->prev = nn_p;

            ll_p->count++;

            ret = 0;
        }
    }

    return ret;
}

int clist_insert_after (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
{
    int  ret = 1;

    if (ll_p)
    {
        void  *mclone_p = ll_p->mclone_fn(&ll_p->allocator, member_p, ll_p->member_size);

        if (   !member_p
            ||  mclone_p
           )
        {
            ret = clist_insert_after_new(ll_p, itr_p, mclone_p);
        }

        if (1 == ret)
        {
            if (mclone_p)
                ll_p->mfree_fn(&ll_p->allocator, mclone_p);
        }
    }

    return ret;
}

int clist_insert_at_new (clist_t *ll_p, int pos, void *member_p)
{
    int  ret = 1;

    if (   ll_p
        && pos >= 0
        && pos <= ll_p->count
       )
    {
        clist_node_t  *nd_p;
        int            ii;

        if (pos < ll_p->count / 2)
        {
            nd_p = ll_p->head.next;

            for (ii = 0; ii < pos; ii++)
                nd_p = nd_p->next;

            ret = clist_insert_before(ll_p, &nd_p->itr, member_p);
        }
        else
        {
            nd_p = ll_p->tail.prev;

            for (ii = ll_p->count - 1; ii >= pos; ii--)
                nd_p = nd_p->prev;

            ret = clist_insert_after(ll_p, &nd_p->itr, member_p);
        }
    }

    return ret;
}

int clist_insert_at (clist_t *ll_p, int pos, void *member_p)
{
    int  ret = 1;

    if (ll_p)
    {
        void  *mclone_p = ll_p->mclone_fn(&ll_p->allocator, member_p, ll_p->member_size);

        if (   !member_p
            ||  mclone_p
           )
        {
            ret = clist_insert_at_new(ll_p, pos, mclone_p);
        }

        if (1 == ret)
        {
            if (mclone_p)
                ll_p->mfree_fn(&ll_p->allocator, mclone_p);
        }
    }

    return ret;
}

clist_iterator_t* clist_begin (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->head.next->itr;

    return itr_p;
}

clist_iterator_t* clist_end (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->tail.itr;

    return itr_p;
}

clist_iterator_t* clist_iterator_next (clist_iterator_t *itr_p)
{
    return   (   itr_p
              && itr_p->node_p->next
             )
           ? &itr_p->node_p->next->itr
           :  NULL;
}

clist_iterator_t* clist_iterator_prev (clist_iterator_t *itr_p)
{
    return   (   itr_p
              && itr_p->node_p->prev
             )
           ? &itr_p->node_p->prev->itr
           :  NULL;
}

void* clist_get (clist_iterator_t *itr_p)
{
    return   itr_p
           ? itr_p->node_p->member_p
           : NULL;
}

void* clist_get_at (clist_t *ll_p, unsigned int pos)
{
    void  *member_p = NULL;

    if (   ll_p
        && pos < ll_p->count
       )
    {
        clist_node_t  *nd_p;
        unsigned int   ii;

        if (pos < ll_p->count / 2)
        {
            nd_p = ll_p->head.next;

            for (ii = 0; ii < pos; ii++)
                nd_p = nd_p->next;
        }
        else
        {
            nd_p = ll_p->tail.prev;

            for (ii = ll_p->count - 1; ii > pos; ii--)
                nd_p = nd_p->prev;
        }

        member_p = nd_p->member_p;
    }

    return member_p;
}

void* clist_pop (clist_iterator_t *itr_p)
{
    void  *member_p = NULL;

    if (   itr_p
        && itr_p->node_p->prev
        && itr_p->node_p->next
       )
    {
        itr_p->node_p->prev->next = itr_p->node_p->next;
        itr_p->node_p->next->prev = itr_p->node_p->prev;

        itr_p->node_p->ll_p->count--;

        member_p = itr_p->node_p->member_p;

        clist_free(itr_p->node_p->ll_p, itr_p->node_p);
    }

    return member_p;
}

void* clist_pop_at (clist_t *ll_p, unsigned int pos)
{
    void  *member_p = NULL;

    if (   ll_p
        && pos < ll_p->count
       )
    {
        clist_node_t  *nd_p;
        unsigned int   ii;

        if (pos < ll_p->count / 2)
        {
            nd_p = ll_p->head.next;

            for (ii = 0; ii < pos; ii++)
                nd_p = nd_p->next;
        }
        else
        {
            nd_p = ll_p->tail.prev;

            for (ii = ll_p->count - 1; ii > pos; ii--)
                nd_p = nd_p->prev;
        }

        member_p = clist_pop(&nd_p->itr);
    }

    return member_p;
}

void* clist_pop_member (clist_t *ll_p, void *member_p)
{
    void  *lmember_p = NULL;

    if (ll_p)
    {
        clist_iterator_t  *itr_p;

        clist_foreach(ll_p, itr_p)
        {
            if (0 == ll_p->mcmp_fn(clist_get(itr_p), member_p, ll_p->member_size))
            {
                lmember_p = clist_pop(itr_p);
                break;
            }
        }
    }

    return lmember_p;
}

void clist_remove (clist_iterator_t *itr_p)
{
    clist_t  *ll_p     =   itr_p
                         ? itr_p->node_p->ll_p
                         : NULL;
    void     *member_p = clist_pop(itr_p);

    if (member_p)
        ll_p->mfree_fn(&ll_p->allocator, member_p);
}

void clist_remove_at (clist_t *ll_p, unsigned int pos)
{
    void  *member_p = clist_pop_at(ll_p, pos);

    if (member_p)
        ll_p->mfree_fn(&ll_p->allocator, member_p);
}

void clist_remove_member (clist_t *ll_p, void *member_p)
{
    void  *lmember_p = clist_pop_member(ll_p, member_p);

    if (lmember_p)
        ll_p->mfree_fn(&ll_p->allocator, lmember_p);
}

void clist_clear (clist_t *ll_p)
{
    if (ll_p)
    {
        while (ll_p->count > 0)
            clist_remove(clist_begin(ll_p));
    }
}

