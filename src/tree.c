/*****************************************************************************
 *
 * FILE NAME     : tree.c
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : May 1, 2026
 *
 ******************************************************************************
 *
 *  DESCRIPTION : Kernel style Tree
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include "tree.h"


/*****************************************************************************
   External Declarations
*****************************************************************************/

/*****************************************************************************
   Local Constants
*****************************************************************************/

/*****************************************************************************
   Local Types
*****************************************************************************/

/*****************************************************************************
   Local Variables
*****************************************************************************/

/*****************************************************************************
   Global Variables
*****************************************************************************/
tree_node_t  __tree_run_once;


/*****************************************************************************
   Local Macros
******************************************************************************/

/*****************************************************************************
   Local Function Prototypes
*****************************************************************************/

/*****************************************************************************
   Local Functions
*****************************************************************************/

/*****************************************************************************
   Global Functions
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
void tree_del (tree_t *tree_p, void (*del_fn)(tree_node_t *tnode_p))
{
    if (tree_p)
    {
        while (!list_empty(tree_p->nodes))
            tree_node_del(list_first_member(tree_p->nodes, tree_node_t, llnode), del_fn);
    }
}
void tree_node_del (tree_node_t *tnode_p, void (*del_fn)(tree_node_t *tnode_p))
{
    if (tnode_p)
    {
        tree_del(&tnode_p->sub_tree, del_fn);

        if (tnode_p->parent_p)
            list_remove(tnode_p->parent_p->nodes, tnode_p->llnode);

        if (del_fn)
            del_fn(tnode_p);
    }
}

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
void tree_iterate (tree_t *tree_p, void (*callback_fn)(tree_node_t *tnode_p, void *uptr_p), void *uptr_p)
{
    if (   tree_p
        && callback_fn
       )
    {
        tree_node_t  *tnode_p;

        list_foreach_member(tree_p->nodes, tree_node_t, llnode, tnode_p)
            tree_node_iterate(tnode_p, callback_fn, uptr_p);
    }
}
void tree_node_iterate (tree_node_t *tnode_p, void (*callback_fn)(tree_node_t *tnode_p, void *uptr_p), void *uptr_p)
{
    if (   tnode_p
        && callback_fn
       )
    {
        tree_iterate(&tnode_p->sub_tree, callback_fn, uptr_p);
        callback_fn(tnode_p, uptr_p);
    }
}

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
size_t tree_count (tree_t *tree_p)
{
    size_t  count = 0;

    if (tree_p)
    {
        tree_node_t  *tnode_p;

        list_foreach_member(tree_p->nodes, tree_node_t, llnode, tnode_p)
            count += tree_node_count(tnode_p);
    }

    return count;
}
size_t tree_node_count (tree_node_t *tnode_p)
{
    size_t  count = 0;

    if (tnode_p)
        count = 1 + tree_count(&tnode_p->sub_tree);

    return count;
}

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
size_t tree_level (tree_t *tree_p)
{
    size_t  level = 0;

    if (   tree_p
        && tree_p->parent_p
       )
    {
        level = 1 + tree_node_level(tree_p->parent_p);
    }

    return level;
}
size_t tree_node_level (tree_node_t *tnode_p)
{
    size_t  level = 0;

    if (tnode_p)
        level = tree_level(tnode_p->parent_p);

    return level;
}

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
tree_node_t* tree_find_node (tree_t *tree_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p))
{
    tree_node_t  *tnode_p,
                 *fnode_p = NULL;

    if (   tree_p
        && member_match_fn
        && depth > 0
       )
    {
        list_foreach_member(tree_p->nodes, tree_node_t, llnode, tnode_p)
        {
            fnode_p = tree_node_find_node(tnode_p, depth, key_p, member_match_fn);

            if (fnode_p)
                break;
        }
    }

    return fnode_p;
}
tree_node_t* tree_node_find_node (tree_node_t *tnode_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p))
{
    tree_node_t  *fnode_p = NULL;

    if (   tnode_p
        && member_match_fn
        && depth > 0
       )
    {
        if (member_match_fn(tnode_p, key_p))
            fnode_p = tnode_p;
        else
            fnode_p = tree_find_node(&tnode_p->sub_tree, depth - 1, key_p, member_match_fn);
    }

    return fnode_p;
}


/*****************************************************************************
   Test Functions
*****************************************************************************/

