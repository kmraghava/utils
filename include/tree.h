/*****************************************************************************
 *
 * FILE NAME     : tree.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : April 24, 2026
 *
 *****************************************************************************
 *
 *  DESCRIPTION : Kernel style Tree
 *
 *****************************************************************************/

#ifndef __TREE_H
#define __TREE_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include <stdbool.h>
#include "list.h"
#include <stddef.h>


/*****************************************************************************
 * Global Defines
 *****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : tree_init
 *
 *  DESCRIPTION : Initialize the given tree
 *
 *  PARAMS      : t   - tree_t
 *                tnp - tree_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_init(t, tnp)       \
    do                          \
    {                           \
        list_init((t).nodes);   \
        (t).parent_p = tnp;     \
    }                           \
    while (0)

/*****************************************************************************
 *
 *  NAME        : tree_node_init
 *
 *  DESCRIPTION : Initialize the given tree node
 *
 *  PARAMS      : tn - tree_node_t
 *                tp - tree_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_node_init(tn)                  \
    do                                      \
    {                                       \
        (tn).parent_p = NULL;               \
        tree_init((tn).sub_tree, &(tn));    \
        list_node_init((tn).llnode);        \
    }                                       \
    while (0)

/*****************************************************************************
 *
 *  NAME        : tree_node_is_leaf
 *
 *  DESCRIPTION : Checks if the given tree node is leaf node
 *                (ie: has no children)
 *
 *  PARAMS      : tn - tree_node_t
 *
 *  RETURNS     : true or false
 *
 *****************************************************************************/
#define tree_node_is_leaf(tn)  ( list_empty((tn).sub_tree.nodes) )

/*****************************************************************************
 *
 *  NAME        : tree_first
 *                tree_last
 *
 *  DESCRIPTION : Returns pointer to first / last node in t.nodes
 *
 *  PARAMS      : t - tree_t
 *
 *  RETURNS     : Pointers to first or last node of t.nodes
 *
 *  NOTES       : If the t.nodes is empty, they return NULL
 *
 *****************************************************************************/
#define tree_first(t)     list_first_member((t).nodes, tree_node_t, llnode)
#define tree_last(t)      list_last_member((t).nodes, tree_node_t, llnode)

/*****************************************************************************
 *
 *  NAME        : tree_next
 *                tree_prev
 *
 *  DESCRIPTION : Returns pointer to next / previous node in t.nodes
 *
 *  PARAMS      : t  - tree_t
 *                tn - tree_node_t
 *
 *  RETURNS     : Pointers to next / previous node in t.nodes
 *
 *  NOTES       : If the t.nodes is empty, they return NULL
 *                If tn = t.nodes.head, tree_prev returns NULL
 *                If tn = t.nodes.tail, tree_next returns NULL
 *
 *****************************************************************************/
#define tree_next(t, tn)  list_next_member((t).nodes, (tn).llnode, tree_node_t, llnode)
#define tree_prev(t, tn)  list_prev_member((t).nodes, (tn).llnode, tree_node_t, llnode)

/*****************************************************************************
 *
 *  NAME        : tree_first_member
 *                tree_last_member
 *
 *  DESCRIPTION : Returns pointer to first / last member in t.nodes
 *
 *  PARAMS      : t - tree_t
 *
 *  RETURNS     : Pointers to first or last member of t.nodes
 *
 *  NOTES       : If the t.nodes is empty, they return NULL
 *
 *****************************************************************************/
 #define tree_first_member(t, container_type, tnode)            \
    ({                                                          \
        tree_node_t  *__tnp = tree_first(t);                    \
        __tnp ? tree_get(__tnp, container_type, tnode) : NULL;  \
    })

#define tree_last_member(t, container_type, tnode)              \
    ({                                                          \
        tree_node_t  *__tnp = tree_last(t);                     \
        __tnp ? tree_get(__tnp, container_type, tnode) : NULL;  \
    })

/*****************************************************************************
 *
 *  NAME        : tree_next_member
 *                tree_prev_member
 *
 *  DESCRIPTION : Returns pointer to next / previous member in t.nodes
 *
 *  PARAMS      : t  - tree_t
 *                tn - tree_node_t
 *
 *  RETURNS     : Pointers to next / previous member in t.nodes
 *
 *  NOTES       : If the t.nodes is empty, they return NULL
 *                If tn = t.nodes.head, tree_prev returns NULL
 *                If tn = t.nodes.tail, tree_next returns NULL
 *
 *****************************************************************************/
#define tree_next_member(t, tn, container_type, tnode)                                          \
    ({                                                                                          \
        tree_node_t  *__tnp = list_next_member((t).nodes, (tn).llnode, tree_node_t, llnode);    \
        __tnp ? tree_get(__tnp, container_type, tnode) : NULL;                                  \
    })
#define tree_prev_member(t, tn, container_type, tnode)                                          \
    ({                                                                                          \
        tree_node_t  *__tnp = list_prev_member((t).nodes, (tn).llnode, tree_node_t, llnode);    \
        __tnp ? tree_get(__tnp, container_type, tnode) : NULL;                                  \
    })


/*****************************************************************************
 *
 *  NAME        : tree_add_node
 *
 *  DESCRIPTION : Adds the given node to the given tree
 *
 *  PARAMS      : t  - tree_t
 *                tn - tree_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_add_node(t, tn)                    \
    do                                          \
    {                                           \
        list_push_back((t).nodes, (tn).llnode); \
        (tn).parent_p = &(t);                   \
    }                                           \
    while (0)

/*****************************************************************************
 *
 *  NAME        : tree_rem_node
 *
 *  DESCRIPTION : Removes the given node from the given tree
 *
 *  PARAMS      : t  - tree_t
 *                tn - tree_node_t
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_rem_node(t, tn)                    \
    do                                          \
    {                                           \
        list_remove((t).nodes, (tn).llnode);    \
        (tn).parent_p = NULL;                   \
    }                                           \
    while (0)

/*****************************************************************************
 *
 *  NAME        : tree_get
 *
 *  DESCRIPTION : Given a pointer to tree_node_t, returns pointer to its
 *                container object
 *
 *  PARAMS      : tnp            - tree_node_t*
 *                container_type - Data type of the tnode's container.
 *                tnode          - tree_node_t
 *                                 Member of container_type.
 *
 *  RETURNS     : Pointer to container object
 *
 *****************************************************************************/
#define tree_get(tnp, container_type, tnode)                                    \
    ({                                                                          \
        const __typeof__( ((container_type *)0)->tnode ) *__mptr = (tnp);       \
        (container_type *)( (char *)__mptr - offsetof(container_type, tnode) ); \
    })

/*****************************************************************************
 *
 *  NAME        : tree_foreach_node
 *
 *  DESCRIPTION : Loops over each node in the given tree
 *
 *  PARAMS      : t   - tree_t
 *                tnp - tree_node_t*
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_foreach_node(t, tnp) \
    list_foreach_member((t).nodes, tree_node_t, llnode, tnp)

/*****************************************************************************
 *
 *  NAME        : tree_foreach_member
 *
 *  DESCRIPTION : Loops over each member of the given tree
 *
 *  PARAMS      : t              - tree_t
 *                container_type - Data type of the llnode's container.
 *                tnode          - tree_node_t member name in the container_type
 *                member_p       - Pointer to an object of type container_type
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
#define tree_foreach_member(t, container_type, tnode, member_p)               \
    for (tree_node_t *__tnp = NULL, *__p = &__tree_run_once; __p; __p = NULL) \
         tree_foreach_node(t, __tnp)                                          \
            if ((member_p = tree_get(__tnp, container_type, tnode)))


/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct tree_node_s tree_node_t;
typedef struct tree_s tree_t;

struct tree_s
{
    tree_node_t  *parent_p;
    list_t        nodes;
};

struct tree_node_s
{
    tree_t       *parent_p;
    tree_t        sub_tree;

    list_node_t   llnode;
};


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 extern tree_node_t  __tree_run_once;


/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : tree_del
 *                tree_node_del
 *
 *  DESCRIPTION : Delete the tree nodes
 *
 *  PARAMS      : tree_p  - Tree
 *                tnode_p - Tree node
 *                del_fn  - Callback function
 *
 *  RETURNS     : void
 *
 *  NOTES       : del_fn is called for each node in the tree. It can be used
 *                to free the memory allocated for the node's member. The
 *                del_fn is called after the node is removed from the tree.
 *                So, it is safe to free the member's memory if allocated.
 *                Also, the del_fn should not access the node's parent; it will
 *                be NULL.
 *                The del_fn is called for leaf nodes first and then for
 *                parent nodes.
 *
 *****************************************************************************/
extern void tree_del (tree_t *tree_p, void (*del_fn)(tree_node_t *tnode_p));
extern void tree_node_del (tree_node_t *tnode_p, void (*del_fn)(tree_node_t *tnode_p));

/*****************************************************************************
 *
 *  NAME        : tree_iterate
 *                tree_node_iterate
 *
 *  DESCRIPTION : Iterate through the tree nodes
 *
 *  PARAMS      : tree_p      - Tree
 *                tnode_p     - Tree node
 *                callback_fn - Callback function
 *                uptr_p      - User pointer
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
extern void tree_iterate (tree_t *tree_p, void (*callback_fn)(tree_node_t *tnode_p, void *uptr_p), void *uptr_p);
extern void tree_node_iterate (tree_node_t *tnode_p, void (*callback_fn)(tree_node_t *tnode_p, void *uptr_p), void *uptr_p);

/*****************************************************************************
 *
 *  NAME        : tree_count
 *                tree_node_count
 *
 *  DESCRIPTION : Count the nodes in the tree
 *
 *  PARAMS      : tree_p  - Tree
 *                tnode_p - Tree node
 *
 *  RETURNS     : Number of nodes in the tree
 *
 *****************************************************************************/
extern size_t tree_count (tree_t *tree_p);
extern size_t tree_node_count (tree_node_t *tnode_p);

/*****************************************************************************
 *
 *  NAME        : tree_level
 *                tree_node_level
 *
 *  DESCRIPTION : Get the level of the tree or tree node
 *
 *  PARAMS      : tree_p  - Tree
 *                tnode_p - Tree node
 *
 *  RETURNS     : level (0 for root, 1 for its subtree and so on)
 *
 *****************************************************************************/
extern size_t tree_level (tree_t *tree_p);
extern size_t tree_node_level (tree_node_t *tnode_p);

/*****************************************************************************
 *
 *  NAME        : tree_find_node
 *                tree_node_find_node
 *
 *  DESCRIPTION : Find a node in the tree
 *
 *  PARAMS      : tree_p          - Tree
 *                depth           - Depth to search
 *                key_p           - Search key
 *                member_match_fn - Function to match the key with the
 *                                  node member
 *
 *  RETURNS     : Found node or NULL
 *
 *****************************************************************************/
extern tree_node_t* tree_find_node (tree_t *tree_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p));
extern tree_node_t* tree_node_find_node (tree_node_t *tnode_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p));


#if defined(__cplusplus)
}
#endif

 
#endif /* __TREE_H */
