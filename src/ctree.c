
#include "list.h"
#include "ctree.h"
#include <stdlib.h>


struct ctree_node_s
{
    ctree_t       *parent_p;
    ctree_t       *subtree_p;

    void          *member_p;

    list_node_t    llnode;
};

struct ctree_s
{
    ctree_node_t  *parent_p;
    list_t         nodes;
};


ctree_t* ctree_new (ctree_node_t *parent_p)
{
    ctree_t  *tree_p = calloc(1, sizeof(*tree_p));

    if (tree_p)
    {
        list_init(tree_p->nodes);

        if (parent_p)
        {
            tree_p->parent_p = parent_p;
            parent_p->subtree_p = tree_p;
        }
    }

    return tree_p;
}

ctree_t* ctree_del (ctree_t *tree_p)
{
    if (tree_p)
    {
        while (!list_empty(tree_p->nodes))
            ctree_node_del(list_first_member(tree_p->nodes, ctree_node_t, llnode));

        if (tree_p->parent_p)
            tree_p->parent_p->subtree_p = NULL;

        free(tree_p);
    }

    return NULL;
}

ctree_node_t* ctree_node_new (ctree_t *parent_p, void *member_p)
{
    ctree_node_t  *tnode_p = calloc(1, sizeof(*tnode_p));

    if (tnode_p)
    {
        tnode_p->member_p = member_p;

        if (parent_p)
        {
            tnode_p->parent_p = parent_p;
            list_push_back(parent_p->nodes, tnode_p->llnode);
        }
    }

    return tnode_p;
}

ctree_node_t* ctree_node_del (ctree_node_t *tnode_p)
{
    if (tnode_p)
    {
        ctree_del(tnode_p->subtree_p);

        if (tnode_p->parent_p)
            list_remove(tnode_p->parent_p->nodes, tnode_p->llnode);

        free(tnode_p);
    }
    
    return NULL;
}

size_t ctree_count (ctree_t *tree_p)
{
    size_t  count = 0;

    if (tree_p)
    {
        ctree_node_t  *tnode_p;

        list_foreach_member(tree_p->nodes, ctree_node_t, llnode, tnode_p)
            count += ctree_node_count(tnode_p);
    }

    return count;
}

size_t ctree_node_count (ctree_node_t *tnode_p)
{
    size_t  count = 0;

    if (tnode_p)
        count = 1 + ctree_count(tnode_p->subtree_p);

    return count;
}

size_t ctree_level (ctree_t *tree_p)
{
    size_t  level = 0;

    if (   tree_p
        && tree_p->parent_p
       )
    {
        level = 1 + ctree_node_level(tree_p->parent_p);
    }

    return level;
}

size_t ctree_node_level (ctree_node_t *tnode_p)
{
    size_t  level = 0;

    if (tnode_p)
        level = ctree_level(tnode_p->parent_p);

    return level;
}

void* ctree_node_member (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->member_p : NULL; }

void ctree_iterate (ctree_t *tree_p, void (*callback_fn)(void *member_p, void *uptr_p), void *uptr_p)
{
    if (   tree_p
        && callback_fn
       )
    {
        ctree_node_t  *tnode_p;

        list_foreach_member(tree_p->nodes, ctree_node_t, llnode, tnode_p)
            ctree_node_iterate(tnode_p, callback_fn, uptr_p);
    }
}

void ctree_node_iterate (ctree_node_t *tnode_p, void (*callback_fn)(void *member_p, void *uptr_p), void *uptr_p)
{
    if (   tnode_p
        && callback_fn
       )
    {
        ctree_iterate(tnode_p->subtree_p, callback_fn, uptr_p);
        callback_fn(tnode_p->member_p, uptr_p);
    }
}

ctree_node_t* ctree_first_node (ctree_t *tree_p) { return tree_p ? list_first_member(tree_p->nodes, ctree_node_t, llnode) : NULL; }
ctree_node_t* ctree_last_node  (ctree_t *tree_p) { return tree_p ? list_last_member(tree_p->nodes, ctree_node_t, llnode) : NULL; }

ctree_node_t* ctree_node_next (ctree_node_t *tnode_p)
{
    return   (   tnode_p
              && tnode_p->parent_p
             )
           ? list_next_member(tnode_p->parent_p->nodes, tnode_p->llnode, ctree_node_t, llnode)
           : NULL;
}

void ctree_add_node (ctree_t *tree_p, ctree_node_t *tnode_p)
{
    if (   tree_p
        && tnode_p
       )
    {
        list_push_back(tree_p->nodes, tnode_p->llnode);
        tnode_p->parent_p = tree_p;
    }
}

void ctree_rem_node (ctree_t *tree_p, ctree_node_t *tnode_p)
{
    if (   tree_p
        && tnode_p
       )
    {
        list_remove(tree_p->nodes, tnode_p->llnode);
        tnode_p->parent_p = NULL;
    }
}

ctree_t* ctree_node_get_subtree (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->subtree_p : NULL; }
void ctree_node_set_subtree (ctree_node_t *tnode_p, ctree_t *subtree_p)
{
    if (   tnode_p
        && subtree_p
       )
    {
        tnode_p->subtree_p = subtree_p;
        subtree_p->parent_p = tnode_p;
    }
}

void* ctree_find (ctree_t *tree_p, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   tree_p
        && key_p
        && member_match_fn
       )
    {
        ctree_node_t  *tnode_p;

        list_foreach_member(tree_p->nodes, ctree_node_t, llnode, tnode_p)
        {
            matching_member_p = ctree_node_find(tnode_p, key_p, member_match_fn);
            if (matching_member_p)
                break;
        }
    }

    return matching_member_p;
}

clist_t* ctree_find_all (ctree_t *tree_p, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    clist_t  *mm_list_p = NULL;

    if (   tree_p
        && key_p
        && member_match_fn
       )
    {
        mm_list_p = clist_new();

        if (mm_list_p)
        {
            ctree_node_t  *tnode_p;

            list_foreach_member(tree_p->nodes, ctree_node_t, llnode, tnode_p)
            {
                clist_t  *stn_mm_list_p = ctree_node_find_all(tnode_p, key_p, member_match_fn);
                if (stn_mm_list_p)
                {
                    clist_append(mm_list_p, stn_mm_list_p);
                    clist_del(stn_mm_list_p);
                }
                else
                {
                    clist_del(mm_list_p);
                    mm_list_p = NULL;

                    break;
                }
            }
        }
    }

    return mm_list_p;
}

void* ctree_node_find (ctree_node_t *tnode_p, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   tnode_p
        && key_p
        && member_match_fn
       )
    {
        matching_member_p =   member_match_fn(tnode_p->member_p, key_p)
                            ? tnode_p->member_p
                            : ctree_find(tnode_p->subtree_p, key_p, member_match_fn);
    }

    return matching_member_p;
}

clist_t* ctree_node_find_all (ctree_node_t *tnode_p, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    clist_t  *mm_list_p = NULL;

    if (   tnode_p
        && key_p
        && member_match_fn
       )
    {
        mm_list_p = clist_new();

        if (mm_list_p)
        {
            clist_t  *st_mm_list_p;

            if (member_match_fn(tnode_p->member_p, key_p))
            {
                if (!clist_push_back(mm_list_p, tnode_p->member_p))
                {
                    clist_del(mm_list_p);
                    mm_list_p = NULL;
                }
            }

            if (   mm_list_p
                && tnode_p->subtree_p
                )
            {
                st_mm_list_p = ctree_find_all(tnode_p->subtree_p, key_p, member_match_fn);
                if (st_mm_list_p)
                {
                    clist_append(mm_list_p, st_mm_list_p);
                    clist_del(st_mm_list_p);
                }
                else
                {
                    clist_del(mm_list_p);
                    mm_list_p = NULL;
                }
            }
        }
    }

    return mm_list_p;
}
