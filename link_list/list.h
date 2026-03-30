/*****************************************************************************
 *
 * FILE NAME     : list.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : September 8, 2025
 *
 *****************************************************************************
 *
 *  DESCRIPTION : Kernel style Double link list
 *
 *****************************************************************************/

#ifndef __LIST_H
#define __LIST_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include <stddef.h>


/*****************************************************************************
 * Global Defines
 *****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : list_init
 *
 *  DESCRIPTION : Initialize the given list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_init(ll)               \
    do                              \
    {                               \
        ll.head.prev = NULL;        \
        ll.head.next = &ll.tail;    \
        ll.tail.prev = &ll.head;    \
        ll.tail.next = NULL;        \
                                    \
        ll.count = 0;               \
    }                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_empty
 *
 *  DESCRIPTION : Checks if the given list is empty
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_empty(ll)  ( ll.count == 0 )

/*****************************************************************************
 *
 *  NAME        : list_add
 *
 *  DESCRIPTION : Adds the given node to the end of given list
 *
 *  PARAMS      : ll   - list_t
 *                node - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_add(ll, node)  list_push_back(ll, node)

/*****************************************************************************
 *
 *  NAME        : list_remove
 *
 *  DESCRIPTION : Removes the given node from the given list
 *
 *  PARAMS      : ll   - list_t
 *                node - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : Note the deliberate avoidance of parenthesis around node in
 *                the macro expansion. Passing dereferenced object (eg: *node_p)
 *                will cause compilation errors. Although, passing, for example
 *                (*node_p) should be okay, but generally discouraged for
 *                consistency.
 *
 *                Additionally, passing expressions such as (*list_first())
 *                will result in undefined behavior because list_first() will
 *                get evaluated multiple times.
 *
 *                Safest and recommended usage is to pass obj_p->llnode
 *                or obj.llnode where llnode is of type list_node_t.
 *
 *****************************************************************************/
#define list_remove(ll, node)               \
    do                                      \
    {                                       \
        if (   node.prev                    \
            && node.next                    \
           )                                \
        {                                   \
            node.prev->next = node.next;    \
            node.next->prev = node.prev;    \
                                            \
            node.prev = NULL;               \
            node.next = NULL;               \
                                            \
            (ll).count--;                   \
        }                                   \
    }                                       \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_remove_first
 *
 *  DESCRIPTION : Removes the first node from the given list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_remove_first(ll)                       \
    do                                              \
    {                                               \
        if (!list_empty(ll))                        \
        {                                           \
            list_node_t  *__nptr = list_first(ll);  \
                                                    \
            __nptr->prev->next = __nptr->next;      \
            __nptr->next->prev = __nptr->prev;      \
                                                    \
            __nptr->prev = NULL;                    \
            __nptr->next = NULL;                    \
                                                    \
            (ll).count--;                           \
        }                                           \
    }                                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_remove_last
 *
 *  DESCRIPTION : Removes the last node from the given list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_remove_last(ll)                        \
    do                                              \
    {                                               \
        if (!list_empty(ll))                        \
        {                                           \
            list_node_t  *__nptr = list_last(ll);   \
                                                    \
            __nptr->prev->next = __nptr->next;      \
            __nptr->next->prev = __nptr->prev;      \
                                                    \
            __nptr->prev = NULL;                    \
            __nptr->next = NULL;                    \
                                                    \
            (ll).count--;                           \
        }                                           \
    }                                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_remove_all
 *
 *  DESCRIPTION : Removes all nodes, one by one from the given list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_remove_all(ll)         \
    do                              \
    {                               \
        while (!list_empty(ll))     \
            list_remove_first(ll);  \
    }                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_push_back
 *
 *  DESCRIPTION : Adds the given node to the end of the given list
 *
 *  PARAMS      : ll   - list_t
 *                node - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_push_back(ll, node)  list_insert_before(ll, list_tail(ll), node)

/*****************************************************************************
 *
 *  NAME        : list_push_front
 *
 *  DESCRIPTION : Adds the given node to the beginning of the given list
 *
 *  PARAMS      : ll   - list_t
 *                node - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_push_front(ll, node)  list_insert_after(ll, list_head(ll), node)

/*****************************************************************************
 *
 *  NAME        : list_insert_before
 *
 *  DESCRIPTION : Inserts the given node at the given position in the given list
 *
 *  PARAMS      : ll    - list_t
 *                pos_p - list_node_t*
 *                node  - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_insert_before(ll, pos_p, node) \
    do                                      \
    {                                       \
        list_node_t  *__pos_nptr = pos_p;   \
                                            \
        if (   __pos_nptr                   \
            && __pos_nptr->prev             \
           )                                \
        {                                   \
            node.prev = __pos_nptr->prev;   \
            node.next = __pos_nptr;         \
                                            \
            node.prev->next = &node;        \
            node.next->prev = &node;        \
                                            \
            node.mark = 0;                  \
                                            \
            ll.count++;                     \
        }                                   \
    }                                       \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_insert_after
 *
 *  DESCRIPTION : Inserts the given node after the given position in the given list
 *
 *  PARAMS      : ll    - list_t
 *                pos_p - list_node_t*
 *                node  - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_insert_after(ll, pos_p, node)  \
    do                                      \
    {                                       \
        list_node_t  *__pos_nptr = pos_p;   \
                                            \
        if (   __pos_nptr                   \
            && __pos_nptr->next             \
           )                                \
        {                                   \
            node.prev = __pos_nptr;         \
            node.next = __pos_nptr->next;   \
                                            \
            node.prev->next = &node;        \
            node.next->prev = &node;        \
                                            \
            node.mark = 0;                  \
                                            \
            ll.count++;                     \
        }                                   \
    }                                       \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_clear
 *
 *  DESCRIPTION : Clears or empties the given list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : Note that this does not remove the elements one by one.
 *                Therefore, nodes in the list will still be linked to each
 *                other and to the list, but the list will have nothing to do
 *                with the nodes.
 *
 *                Use with caution.
 *
 *****************************************************************************/
#define list_clear(ll)  list_init(ll)

/*****************************************************************************
 *
 *  NAME        : list_delete
 *
 *  DESCRIPTION : Deletes the given node from the given list
 *
 *  PARAMS      : ll             - list_t
 *                node_p         - list_node_t*.
 *                                 Points to the node that is required to be deleted
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                del_cb         - Callback function to call for deleting
 *                                 container object.
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_delete(ll, node_p, container_type, llnode, del_cb)                 \
    do                                                                          \
    {                                                                           \
        list_node_t  *__nptr = node_p;                                          \
                                                                                \
        if (   __nptr                                                           \
            && __nptr->prev                                                     \
            && __nptr->next                                                     \
           )                                                                    \
        {                                                                       \
            container_type  *__mptr = list_get(__nptr, container_type, llnode); \
                                                                                \
            __nptr->prev->next = __nptr->next;                                  \
            __nptr->next->prev = __nptr->prev;                                  \
                                                                                \
            __nptr->prev = NULL;                                                \
            __nptr->next = NULL;                                                \
                                                                                \
            (ll).count--;                                                       \
                                                                                \
            del_cb(__mptr);                                                     \
        }                                                                       \
    }                                                                           \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_delete_first
 *                list_delete_last
 *
 *  DESCRIPTION : Deletes the first / last node from the given list
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                del_cb         - Callback function to call for deleting
 *                                 container object.
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_delete_first(ll, container_type, llnode, del_cb)  list_delete(ll, list_first(ll), container_type, llnode, del_cb)
#define list_delete_last(ll, container_type, llnode, del_cb)  list_delete(ll, list_last(ll), container_type, llnode, del_cb)

/*****************************************************************************
 *
 *  NAME        : list_delete_all
 *
 *  DESCRIPTION : Deletes all node from the given list
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                del_cb         - Callback function to call for deleting
 *                                 container object.
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_delete_all(ll, container_type, llnode, del_cb)                     \
    do                                                                          \
    {                                                                           \
        while(!list_empty(ll))                                                  \
            list_delete(ll, list_first(ll), container_type, llnode, del_cb);    \
    }                                                                           \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_get
 *
 *  DESCRIPTION : Given a pointer to list_node_t, returns pointer to its
 *                container object
 *
 *  PARAMS      : node_p         - list_node_t*
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *
 *  RETURNS     : Pointer to container object
 *
 *****************************************************************************/
#define list_get(node_p, container_type, llnode)                                    \
    ({                                                                              \
        const __typeof__( ((container_type *)0)->llnode ) *__mptr = (node_p);       \
        (container_type *)( (char *)__mptr - offsetof(container_type, llnode) );    \
    })

/*****************************************************************************
 *
 *  NAME        : list_get_nth
 *
 *  DESCRIPTION : Returns the pointer to nth node's container object in the list
 *
 *  PARAMS      : ll             - list_t
 *                n              - Node index
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *
 *  RETURNS     : Pointer to container object
 *
 *****************************************************************************/
#define list_get_nth(ll, n, container_type, llnode)                     \
    ({                                                                  \
        container_type  *__mptr = NULL;                                 \
        list_node_t     *__iptr;                                        \
        unsigned int     __ii = 0;                                      \
                                                                        \
        if (n < ll.count)                                               \
        {                                                               \
            list_foreach(ll, __iptr)                                    \
            {                                                           \
                if (__ii == n)                                          \
                {                                                       \
                    __mptr = list_get(__iptr, container_type, llnode);  \
                    break;                                              \
                }                                                       \
                                                                        \
                __ii++;                                                 \
            }                                                           \
        }                                                               \
                                                                        \
        __mptr;                                                         \
    })

/*****************************************************************************
 *
 *  NAME        : list_head
 *                list_tail
 *
 *  DESCRIPTION : Returns pointer to head or tail of the list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Pointers to head or tail of the list
 *
 *****************************************************************************/
#define list_head(ll)  ( &ll.head )
#define list_tail(ll)  ( &ll.tail )

/*****************************************************************************
 *
 *  NAME        : list_first
 *                list_last
 *
 *  DESCRIPTION : Returns pointer to first or last node of the list
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Pointers to first or last node of the list
 *
 *  NOTES       : If the list is empty, they return NULL
 *
 *****************************************************************************/
#define list_first(ll)  ( list_empty(ll) ? NULL : ll.head.next )
#define list_last(ll)   ( list_empty(ll) ? NULL : ll.tail.prev )

/*****************************************************************************
 *
 *  NAME        : list_first_member
 *                list_last_member
 *
 *  DESCRIPTION : Returns pointer to first or last member of the list. It is
 *                same as calling list_first() to get the first node and then
 *                calling list_get to get a pointer to the container object.
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *
 *  RETURNS     : Pointers to first or last member of the list
 *
 *  NOTES       : If the list is empty, they return NULL
 *
 *****************************************************************************/
#define list_first_member(ll, container_type, llnode)  (list_empty(ll) ? NULL : list_get(list_first(ll), container_type, llnode) )
#define list_last_member(ll, container_type, llnode)   (list_empty(ll) ? NULL : list_get(list_last(ll), container_type, llnode) )

/*****************************************************************************
 *
 *  NAME        : list_mark_node
 *                list_unmark_node
 *                list_node_marked
 *                list_mark_all_nodes
 *                list_unmark_all_nodes
 *
 *  DESCRIPTION : list_mark_node marks the given node of a list.
 *                list_unmark_node un-marks the given node of a list.
 *                list_node_marked checks if a node is marked.
 *                list_mark_all_nodes marks all nodes of the given list.
 *                list_unmark_all_nodes un-marks all nodes of the given list.
 *                Actions can be taken on marked nodes. For example, one can
 *                delete all marked nodes.
 *
 *  PARAMS      : node - list_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_mark_node(node)    node.mark = 1
#define list_unmark_node(node)  node.mark = 0
#define list_node_marked(node)  ( node.mark == 1 )
#define list_mark_all_nodes(ll)  \
    do                           \
    {                            \
        list_node_t  *__nd_p;    \
                                 \
        list_foreach(ll, __nd_p) \
            __nd_p->mark = 1;    \
    }                            \
    while (0)
#define list_unmark_all_nodes(ll) \
    do                            \
    {                             \
        list_node_t  *__nd_p;     \
                                  \
        list_foreach(ll, __nd_p)  \
            __nd_p->mark = 0;     \
    }                             \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_foreach
 *
 *  DESCRIPTION : Loops over each node in the given list
 *
 *  PARAMS      : ll     - list_t
 *                node_p - list_node_t*
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_foreach(ll, node_p) \
    for (node_p = ll.head.next; node_p != list_tail(ll); node_p = node_p->next)

/*****************************************************************************
 *
 *  NAME        : list_foreach_marked
 *
 *  DESCRIPTION : Loops over each marked node in the given list
 *
 *  PARAMS      : ll     - list_t
 *                node_p - list_node_t*
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_foreach_marked(ll, node_p) \
    for (node_p = ll.head.next; node_p != list_tail(ll); node_p = node_p->next) \
        if (node_p->mark == 1)

/*****************************************************************************
 *
 *  NAME        : list_foreach_member
 *
 *  DESCRIPTION : Loops over each member of the given list
 *
 *  PARAMS      : ll             - list_t
 *                node_p         - list_node_t*
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                member_p       - Pointer to an object of type container_type
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_foreach_member(ll, node_p, container_type, llnode, member_p) \
    for (node_p = ll.head.next; node_p != list_tail(ll) && (member_p = list_get(node_p, container_type, llnode)); node_p = node_p->next)

/*****************************************************************************
 *
 *  NAME        : list_foreach_marked_member
 *
 *  DESCRIPTION : Loops over each marked member of the given list
 *
 *  PARAMS      : ll             - list_t
 *                node_p         - list_node_t*
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                member_p       - Pointer to an object of type container_type
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_foreach_marked_member(ll, node_p, container_type, llnode, member_p) \
    for (node_p = ll.head.next; node_p != list_tail(ll) && (member_p = list_get(node_p, container_type, llnode)); node_p = node_p->next) \
        if (node_p->mark == 1)

/*****************************************************************************
 *
 *  NAME        : list_iterate
 *
 *  DESCRIPTION : Iterates through the list nodes in the given list and calls
 *                the given callback for each node.
 *
 *  PARAMS      : ll - list_t
 *                cb - Callback to be called
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_iterate(ll, cb)        \
    do                              \
    {                               \
        list_node_t  *__iptr;       \
                                    \
        list_foreach(ll, __iptr)    \
            cb(__iptr);             \
    }                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_iterate_members
 *
 *  DESCRIPTION : Iterates through the members in the given list and calls
 *                the given callback for each member.
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                cb             - Callback to be called
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_iterate_members(ll, container_type, llnode, cb)            \
    do                                                                  \
    {                                                                   \
        list_node_t     *__iptr;                                        \
        container_type  *__mptr;                                        \
                                                                        \
        list_foreach_member(ll, __iptr, container_type, llnode, __mptr) \
            cb(__mptr);                                                 \
    }                                                                   \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_iterate
 *
 *  DESCRIPTION : Iterates through marked nodes in the given list and calls
 *                the given callback for each node such.
 *
 *  PARAMS      : ll - list_t
 *                cb - Callback to be called
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_iterate_marked(ll, cb) \
    do                              \
    {                               \
        list_node_t  *__iptr;       \
                                    \
        list_foreach(ll, __iptr)    \
            if (__iptr->mark == 1)  \
                cb(__iptr);         \
    }                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_iterate_members
 *
 *  DESCRIPTION : Iterates through the members whose nodes are marked in the
 *                given list and calls the given callback for each member.
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                cb             - Callback to be called
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_iterate_marked_members(ll, container_type, llnode, cb)     \
    do                                                                  \
    {                                                                   \
        list_node_t     *__iptr;                                        \
        container_type  *__mptr;                                        \
                                                                        \
        list_foreach_member(ll, __iptr, container_type, llnode, __mptr) \
            if (__iptr->mark == 1)                                      \
                cb(__mptr);                                             \
    }                                                                   \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_remove_marked
 *
 *  DESCRIPTION : Removes marked nodes from the given list.
 *
 *  PARAMS      : ll - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_remove_marked(ll)                      \
    do                                              \
    {                                               \
        list_node_t  *__iptr = ll.head.next,        \
                     *__prev_iptr,                  \
                     *__next_iptr;                  \
                                                    \
        while (__iptr != list_tail(ll))             \
        {                                           \
            __prev_iptr = __iptr->prev;             \
            __next_iptr = __iptr->next;             \
                                                    \
            if (__iptr->mark == 1)                  \
            {                                       \
                __prev_iptr->next = __next_iptr;    \
                __next_iptr->prev = __prev_iptr;    \
                                                    \
                __iptr->prev = NULL;                \
                __iptr->next = NULL;                \
                                                    \
                ll.count--;                         \
            }                                       \
                                                    \
            __iptr = __next_iptr;                   \
        }                                           \
    }                                               \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_delete_marked
 *
 *  DESCRIPTION : Deletes marked member nodes from the given list.
 *
 *  PARAMS      : ll             - list_t
 *                container_type - Data type of the llnode's container.
 *                llnode         - list_node_t
 *                                 Member of container_type.
 *                del_cb         - Callback to be called for deletion
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_delete_marked(ll, container_type, llnode, del_cb)              \
    do                                                                      \
    {                                                                       \
        list_node_t  *__iptr = ll.head.next,                                \
                     *__next_iptr;                                          \
                                                                            \
        while (__iptr != list_tail(ll))                                     \
        {                                                                   \
            __next_iptr = __iptr->next;                                     \
                                                                            \
            if (__iptr->mark == 1)                                          \
                list_delete(ll, __iptr, container_type, llnode, del_cb);    \
                                                                            \
            __iptr = __next_iptr;                                           \
        }                                                                   \
    }                                                                       \
    while (0)

/*****************************************************************************
 *
 *  NAME        : list_append
 *
 *  DESCRIPTION : Appends list "from" to list "to"
 *
 *  PARAMS      : from - list_t
 *                to   - list_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define list_append(from, to)                           \
    do                                                  \
    {                                                   \
        if (!list_empty(from))                          \
        {                                               \
            if (list_empty(to))                         \
            {                                           \
                list_first(from)->prev = list_head(to); \
                list_head(to)->next = list_first(from); \
                                                        \
                list_last(from)->next = list_tail(to);  \
                list_tail(to)->prev = list_last(from);  \
                                                        \
                (to).count = (from).count;              \
            }                                           \
            else                                        \
            {                                           \
                list_first(from)->prev = list_last(to); \
                list_last(to)->next = list_first(from); \
                                                        \
                list_last(from)->next = list_tail(to);  \
                list_tail(to)->prev = list_last(from);  \
                                                        \
                (to).count += (from).count;             \
            }                                           \
                                                        \
            list_clear(from);                           \
        }                                               \
    }                                                   \
    while (0)


/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct list_node_s list_node_t;
typedef struct list_s list_t;

struct list_node_s
{
    list_node_t  *prev,
                 *next;

    int           mark;
};

struct list_s
{
    list_node_t   head,
                  tail;

    unsigned int  count;
};


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 
/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/


#if defined(__cplusplus)
}
#endif

 
#endif /* __LIST_H */
