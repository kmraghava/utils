
#include "clist.h"
#include <stdlib.h>


struct clist_node_s
{
    void          *member_p;

    clist_node_t  *prev,
                  *next;
};

struct clist_s
{
    clist_node_t  head,
                  tail;

    int           count;
};


static void clist_init (clist_t *ll_p)
{
    ll_p->head.prev = NULL;
    ll_p->head.next = &ll_p->tail;
    ll_p->tail.prev = &ll_p->head;
    ll_p->tail.next = NULL;

    ll_p->count = 0;
}


clist_t* clist_new (void)
{
    clist_t  *ll_p = malloc(sizeof(*ll_p));

    if (ll_p)
        clist_init(ll_p);

    return ll_p;
}

clist_t* clist_del (clist_t *ll_p)
{
    if (ll_p)
    {
        clist_clear(ll_p);

        free(ll_p);
        ll_p = NULL;
    }

    return ll_p;
}

int clist_count (clist_t *ll_p)
{
    int  count = 0;

    if (ll_p)
        count = ll_p->count;

    return count;
}

clist_node_t* clist_insert_before (clist_t *ll_p, clist_node_t *node_p, void *member_p)
{
    clist_node_t  *nn_p = NULL;

    if (   ll_p
        && node_p
        && node_p != &ll_p->head
       )
    {
        nn_p = malloc(sizeof(*nn_p));

        if (nn_p)
        {
            nn_p->member_p = member_p;

            nn_p->prev = node_p->prev;
            nn_p->next = node_p;

            nn_p->prev->next = nn_p;
            nn_p->next->prev = nn_p;

            ll_p->count++;
        }
    }

    return nn_p;
}

clist_node_t* clist_head (clist_t *ll_p) { return ll_p ? &ll_p->head : NULL; }
clist_node_t* clist_tail (clist_t *ll_p) { return ll_p ? &ll_p->tail : NULL; }

clist_node_t* clist_first (clist_t *ll_p) { return ll_p ? ll_p->head.next : NULL; }
clist_node_t* clist_last  (clist_t *ll_p) { return ll_p ? ll_p->tail.prev : NULL; }

clist_node_t* clist_next (clist_node_t *node_p) { return node_p ? node_p->next : NULL; } 
clist_node_t* clist_prev (clist_node_t *node_p) { return node_p ? node_p->prev : NULL; }

clist_node_t* clist_node (clist_t *ll_p, int pos)
{
    clist_node_t  *node_p = NULL;

    if (   ll_p
        && pos >= 0
        && pos <= ll_p->count
       )
    {
        if (pos < ll_p->count / 2)
        {
            node_p = ll_p->head.next;

            while (pos-- > 0)
                node_p = node_p->next;
        }
        else
        {
            node_p = &ll_p->tail;

            while (pos++ < ll_p->count)
                node_p = node_p->prev;
        }
    }

    return node_p;
}

void* clist_member (clist_node_t *node_p) { return node_p ? node_p->member_p : NULL; }

void clist_remove (clist_t *ll_p, clist_node_t *node_p)
{
    if (   ll_p
        && node_p
        && node_p->prev
        && node_p->next
       )
    {
        node_p->prev->next = node_p->next;
        node_p->next->prev = node_p->prev;

        ll_p->count--;

        free(node_p);
    }
}

void clist_append (clist_t *ll_p, clist_t *other_p)
{
    if (   ll_p
        && other_p
        && other_p->count
       )
    {
        clist_node_t  *ll_last_p = clist_last(ll_p),
                      *ll_tail_p = clist_tail(ll_p),

                      *other_first_p = clist_first(other_p),
                      *other_last_p = clist_last(other_p);

        other_first_p->prev = ll_last_p;
        other_last_p->next  = ll_tail_p;

        ll_last_p->next = other_first_p;
        ll_tail_p->prev = other_last_p;

        ll_p->count += other_p->count;

        clist_init(other_p);
    }
}

void* clist_find (clist_t *ll_p, void *key_p, bool (*member_match_fn) (void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   ll_p
        && key_p
        && member_match_fn
       )
    {
        clist_node_t  *nd_p;
        void          *member_p;

        clist_foreach_member(ll_p, nd_p, member_p)
        {
            if (member_match_fn(member_p, key_p))
            {
                matching_member_p = member_p;
                break;
            }
        }
    }

    return matching_member_p;
}

clist_t* clist_find_all (clist_t *ll_p, void *key_p, bool (*member_match_fn) (void *member_p, void *key_p))
{
    clist_t  *mm_list_p = NULL;

    if (   ll_p
        && key_p
        && member_match_fn
       )
    {
        mm_list_p = clist_new();

        if (mm_list_p)
        {
            clist_node_t  *nd_p;
            void          *member_p;

            clist_foreach_member(ll_p, nd_p, member_p)
            {
                if (member_match_fn(member_p, key_p))
                {
                    if (!clist_push_back(mm_list_p, member_p))
                    {
                        clist_del(mm_list_p);
                        mm_list_p = NULL;

                        break;
                    }
                }
            }
        }
    }

    return mm_list_p;
}

