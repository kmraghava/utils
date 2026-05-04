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
 *  NAME        : tree_find
 *                tree_node_find
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
tree_node_t* tree_find (tree_t *tree_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p))
{
    tree_node_t  *tnode_p,
                 *fnode_p = NULL;

    if (   tree_p
        && key_p
        && member_match_fn
        && depth > 0
       )
    {
        list_foreach_member(tree_p->nodes, tree_node_t, llnode, tnode_p)
        {
            fnode_p = tree_node_find(tnode_p, depth, key_p, member_match_fn);

            if (fnode_p)
                break;
        }
    }

    return fnode_p;
}
tree_node_t* tree_node_find (tree_node_t *tnode_p, size_t depth, void *key_p, bool (*member_match_fn)(tree_node_t *tnode_p, void *key_p))
{
    tree_node_t  *fnode_p = NULL;

    if (   tnode_p
        && key_p
        && member_match_fn
        && depth > 0
       )
    {
        if (member_match_fn(tnode_p, key_p))
            fnode_p = tnode_p;
        else
            fnode_p = tree_find(&tnode_p->sub_tree, depth - 1, key_p, member_match_fn);
    }

    return fnode_p;
}


/*****************************************************************************
   Test Functions
*****************************************************************************/

