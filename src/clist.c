/*****************************************************************************
 *
 *  FILE NAME     : clist.c
 *  MODULE        : utils
 *  AUTHOR        : KM Raghava
 *  CREATION DATE : November 21, 2025
 *
 ******************************************************************************
 *
 *  DESCRIPTION : Double link list (Container)
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include "clist.h"
#include <stdlib.h>


/*****************************************************************************
   External Declarations
*****************************************************************************/

/*****************************************************************************
   Local Constants
*****************************************************************************/

/*****************************************************************************
   Local Types
*****************************************************************************/
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
    clist_node_t   head,
                   tail;

    unsigned int   count;
};


/*****************************************************************************
   Local Variables
*****************************************************************************/

/*****************************************************************************
   Global Variables
*****************************************************************************/

/*****************************************************************************
   Local Macros
******************************************************************************/

/*****************************************************************************
   Local Function Prototypes
*****************************************************************************/
static clist_node_t* clist_node_new (clist_t *ll_p, void *member_p);


/*****************************************************************************
   Local Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : clist_node_new
 *
 *  DESCRIPTION : Creates a new container list node
 *
 *  PARAMS      : ll_p     - Container list
 *                member_p - Container list member
 *
 *  RETURNS     : Container list
 *
 *****************************************************************************/
static clist_node_t* clist_node_new (clist_t *ll_p, void *member_p)
{
    clist_node_t  *nn_p = malloc(sizeof(clist_node_t));

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


/*****************************************************************************
   Global Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : clist_new
 *
 *  DESCRIPTION : Creates a new container list
 *
 *  PARAMS      : void
 *
 *  RETURNS     : Container list
 *
 *****************************************************************************/
clist_t* clist_new (void)
{
    clist_t   *ll_p = NULL;

    ll_p = malloc(sizeof(clist_t));

    if (ll_p)
    {
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
    }

    return ll_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_del
 *
 *  DESCRIPTION : Deletes the container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : NULL
 *
 *****************************************************************************/
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

/*****************************************************************************
 *
 *  NAME        : clist_count
 *
 *  DESCRIPTION : Returns number of elements in the container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Number of elements in the container list
 *
 *****************************************************************************/
unsigned int clist_count (clist_t *ll_p)
{
    unsigned int  count = 0;

    if (ll_p)
        count = ll_p->count;

    return count;
}

/*****************************************************************************
 *
 *  NAME        : clist_empty
 *
 *  DESCRIPTION : Checks if the container list is empty
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : 1 if container list is empty
 *                0 otherwise
 *
 *****************************************************************************/
int clist_empty (clist_t *ll_p)
{
    return   clist_count(ll_p) == 0
           ? 1
           : 0;
}

/*****************************************************************************
 *
 *  NAME        : clist_push_back
 *                clist_push_front
 *
 *  DESCRIPTION : Adds an element at the back of the container list
 *
 *  PARAMS      : ll_p     - Container list
 *                member_p - member to be added
 *
 *  RETURNS     : 0 if the element was added
 *                1 otherwise
 *
 *****************************************************************************/
int clist_push_back (clist_t *ll_p, void *member_p)
{
    return clist_insert_before(ll_p, clist_end(ll_p), member_p);
}

/*****************************************************************************
 *
 *  NAME        : clist_push_front
 *
 *  DESCRIPTION : Adds an element at the front of the container list
 *
 *  PARAMS      : ll_p     - Container list
 *                member_p - member to be added
 *
 *  RETURNS     : 0 if the element was added
 *                1 otherwise
 *
 *****************************************************************************/
int clist_push_front (clist_t *ll_p, void *member_p)
{
    return clist_insert_before(ll_p, clist_begin(ll_p), member_p);
}

/*****************************************************************************
 *
 *  NAME        : clist_insert_before
 *
 *  DESCRIPTION : Inserts the given element before the given position in the
 *                container list
 *
 *  PARAMS      : ll_p     - Container list
 *                itr_p    - Container list iterator that provides the position
 *                member_p - member to be inserted
 *
 *  RETURNS     : 0 if the element was added
 *                1 otherwise
 *
 *****************************************************************************/
int clist_insert_before (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
{
    int  ret = 1;

    if (   ll_p
        && itr_p
        && itr_p->node_p->ll_p == ll_p
        && itr_p != clist_rend(ll_p)
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

/*****************************************************************************
 *
 *  NAME        : clist_insert_after
 *
 *  DESCRIPTION : Inserts the given element after the given position in the
 *                container list
 *
 *  PARAMS      : ll_p     - Container list
 *                itr_p    - Container list iterator that provides the position
 *                member_p - member to be inserted
 *
 *  RETURNS     : 0 if the element was added
 *                1 otherwise
 *
 *****************************************************************************/
int clist_insert_after (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p)
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

/*****************************************************************************
 *
 *  NAME        : clist_insert_at
 *
 *  DESCRIPTION : Inserts the given element at the given position in the
 *                container list
 *
 *  PARAMS      : ll_p     - Container list
 *                pos      - Index in the list.
 *                member_p - member to be inserted
 *
 *  RETURNS     : 0 if the element was added
 *                1 otherwise
 *
 *****************************************************************************/
int clist_insert_at (clist_t *ll_p, int pos, void *member_p)
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

/*****************************************************************************
 *
 *  NAME        : clist_begin
 *
 *  DESCRIPTION : Returns an iterator that points to the first node of the
 *                container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_begin (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->head.next->itr;

    return itr_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_end
 *
 *  DESCRIPTION : Returns an iterator that points to the tail node of the
 *                container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_end (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->tail.itr;

    return itr_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_rbegin
 *
 *  DESCRIPTION : Returns an iterator that points to the last node of the
 *                container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_rbegin (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->tail.prev->itr;

    return itr_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_rend
 *
 *  DESCRIPTION : Returns an iterator that points to the head node of the
 *                container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_rend (clist_t *ll_p)
{
    clist_iterator_t  *itr_p = NULL;

    if (ll_p)
        itr_p = &ll_p->head.itr;

    return itr_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_iterator_next
 *
 *  DESCRIPTION : Returns iterator to next node
 *
 *  PARAMS      : itr_p - Container list iterator
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_iterator_next (clist_iterator_t *itr_p)
{
    return   (   itr_p
              && itr_p->node_p->next
             )
           ? &itr_p->node_p->next->itr
           :  NULL;
}

/*****************************************************************************
 *
 *  NAME        : clist_iterator_prev
 *
 *  DESCRIPTION : Returns iterator to previous node
 *
 *  PARAMS      : itr_p - Container list iterator
 *
 *  RETURNS     : Iterator
 *
 *****************************************************************************/
clist_iterator_t* clist_iterator_prev (clist_iterator_t *itr_p)
{
    return   (   itr_p
              && itr_p->node_p->prev
             )
           ? &itr_p->node_p->prev->itr
           :  NULL;
}

/*****************************************************************************
 *
 *  NAME        : clist_get
 *
 *  DESCRIPTION : Returns the member pointed to by this iterator
 *
 *  PARAMS      : itr_p - Container list iterator
 *
 *  RETURNS     : Container list member
 *
 *****************************************************************************/
void* clist_get (clist_iterator_t *itr_p)
{
    return   itr_p
           ? itr_p->node_p->member_p
           : NULL;
}

/*****************************************************************************
 *
 *  NAME        : clist_get_at
 *
 *  DESCRIPTION : Returns the member at the given position in the container
 *                list
 *
 *  PARAMS      : ll_p - Container list
 *                pos  - Index in the list.
 *
 *  RETURNS     : Container list member being deleted
 *
 *****************************************************************************/
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

/*****************************************************************************
 *
 *  NAME        : clist_remove
 *
 *  DESCRIPTION : Removes the element pointed to by this iterator from the
 *                container list
 *
 *  PARAMS      : itr_p - Container list iterator
 *
 *  RETURNS     : Container list member being deleted
 *
 *****************************************************************************/
void* clist_remove (clist_iterator_t *itr_p)
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

        free(itr_p->node_p);
    }

    return member_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_remove_at
 *
 *  DESCRIPTION : Removes the element at the given position from the
 *                container list
 *
 *  PARAMS      : ll_p - Container list
 *                pos  - Index in the list.
 *
 *  RETURNS     : Container list member being deleted
 *
 *****************************************************************************/
void* clist_remove_at (clist_t *ll_p, unsigned int pos)
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

        member_p = clist_remove(&nd_p->itr);
    }

    return member_p;
}

/*****************************************************************************
 *
 *  NAME        : clist_remove_member
 *
 *  DESCRIPTION : Removes the given member node from the container list
 *
 *  PARAMS      : ll_p     - Container list
 *                member_p - clist member
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
void clist_remove_member (clist_t *ll_p, void *member_p)
{
    if (ll_p)
    {
        clist_iterator_t  *itr_p;

        clist_foreach(ll_p, itr_p)
        {
            if (clist_get(itr_p) == member_p)
            {
                clist_remove(itr_p);
                break;
            }
        }
    }
}

/*****************************************************************************
 *
 *  NAME        : clist_clear
 *
 *  DESCRIPTION : Removes all element in the given container list
 *
 *  PARAMS      : ll_p - Container list
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
void clist_clear (clist_t *ll_p)
{
    if (ll_p)
    {
        while (ll_p->count > 0)
            clist_remove(clist_begin(ll_p));
    }
}


/*****************************************************************************
   Test Functions
*****************************************************************************/
