
#include "list.h"
#include "ctree.h"
#include <stdlib.h>


struct ctree_node_s
{
    ctree_node_t  *parent_p;
    list_t         children;

    int            level;
    int            count;

    void          *member_p;

    list_node_t    llnode;
};

struct ctree_s
{
    list_t  nodes;

    int     count;
};


static void ctree_node_del (ctree_node_t *tnode_p);
static void ctree_node_count_incr (ctree_node_t *tnode_p, int count);
static void ctree_node_count_decr (ctree_node_t *tnode_p, int count);


static void ctree_node_del (ctree_node_t *tnode_p)
{
    list_node_t   *lnd_p;
    ctree_node_t  *tnd_p;

    list_foreach_member(tnode_p->children, lnd_p, ctree_node_t, llnode, tnd_p)
        ctree_node_del(tnd_p);

    free(tnode_p);
}

static void ctree_node_count_incr (ctree_node_t *tnode_p, int count)
{
    while (tnode_p)
    {
        tnode_p->count += count;

        tnode_p = tnode_p->parent_p;
    }
}

static void ctree_node_count_decr (ctree_node_t *tnode_p, int count)
{
    while (tnode_p)
    {
        tnode_p->count -= count;

        tnode_p = tnode_p->parent_p;
    }
}

ctree_t* ctree_new (void)
{
    ctree_t  *tree_p = calloc(1, sizeof(*tree_p));

    if (tree_p)
        list_init(tree_p->nodes);

    return tree_p;
}

ctree_t* ctree_del (ctree_t *tree_p)
{
    if (tree_p)
    {
        list_node_t   *lnd_p;
        ctree_node_t  *tnd_p;

        list_foreach_member(tree_p->nodes, lnd_p, ctree_node_t, llnode, tnd_p)
            ctree_node_del(tnd_p);

        free(tree_p);
        tree_p = NULL;
    }

    return tree_p;
}

int ctree_count (ctree_t *tree_p) { return tree_p ? tree_p->count : -1; }
int ctree_node_count (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->count : -1; }
int ctree_node_level (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->level : -1; }

ctree_node_t* ctree_add (ctree_t* tree_p, ctree_node_t *pnode_p, void *member_p)
{
    ctree_node_t  *tnode_p = NULL;

    if (tree_p)
    {
        tnode_p = calloc(1, sizeof(*tnode_p));

        if (tnode_p)
        {
            list_init(tnode_p->children);

            tnode_p->member_p = member_p;
            tnode_p->count = 1;

            if (pnode_p)
            {
                tnode_p->parent_p = pnode_p;
                tnode_p->level = pnode_p->level + 1;

                ctree_node_count_incr(pnode_p, 1);

                list_push_back(pnode_p->children, tnode_p->llnode);
            }
            else
            {
                list_push_back(tree_p->nodes, tnode_p->llnode);
            }

            tree_p->count++;
        }
    }

    return tnode_p;
}

void ctree_rem_node (ctree_t *tree_p, ctree_node_t *tnode_p)
{
    if (   tree_p
        && tnode_p
       )
    {
        ctree_node_t *pnode_p = tnode_p->parent_p;

        if (pnode_p)
        {
            ctree_node_count_decr(pnode_p, tnode_p->count);

            list_remove(pnode_p->children, tnode_p->llnode);
        }
        else
        {
            list_remove(tree_p->nodes, tnode_p->llnode);
        }

        tree_p->count -= tnode_p->count;

        ctree_node_del(tnode_p);
    }
}

void* ctree_member (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->member_p : NULL; }

#define ctree_node_list_find(tree_p, nlist, search_levels, key_p, member_match_fn, matching_member_p)   \
    do                                                                                                  \
    {                                                                                                   \
        list_node_t   *lnd_p;                                                                           \
        ctree_node_t  *tnd_p;                                                                           \
                                                                                                        \
        list_foreach_member(nlist, lnd_p, ctree_node_t, llnode, tnd_p)                                  \
        {                                                                                               \
            matching_member_p = ctree_find(tree_p, tnd_p, search_levels, key_p, member_match_fn);       \
            if (matching_member_p)                                                                      \
                break;                                                                                  \
        }                                                                                               \
    }                                                                                                   \
    while (0)

void* ctree_find (ctree_t *tree_p, ctree_node_t *snode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   tree_p
        && key_p
        && member_match_fn
       )
    {
        if (snode_p)
        {
            if (   search_levels == -1
                || snode_p->level < search_levels
               )
            {
                if (member_match_fn(snode_p->member_p, key_p))
                    matching_member_p = snode_p->member_p;
                else
                {
                    ctree_node_list_find(tree_p, snode_p->children, search_levels, key_p, member_match_fn, matching_member_p);
                }
            }
        }
        else
        {
            ctree_node_list_find(tree_p, tree_p->nodes, search_levels, key_p, member_match_fn, matching_member_p);
        }
    }

    return matching_member_p;
}

#define ctree_node_list_find_all(tree_p, nlist, search_levels, key_p, member_match_fn, mm_list_p)           \
    do                                                                                                      \
    {                                                                                                       \
        list_node_t   *lnd_p;                                                                               \
        ctree_node_t  *tnd_p;                                                                               \
                                                                                                            \
        list_foreach_member(nlist, lnd_p, ctree_node_t, llnode, tnd_p)                                      \
        {                                                                                                   \
            clist_t  *cmm_list_p = ctree_find_all(tree_p, tnd_p, search_levels, key_p, member_match_fn);    \
            if (cmm_list_p)                                                                                 \
            {                                                                                               \
                clist_append(mm_list_p, cmm_list_p);                                                        \
                clist_del(cmm_list_p);                                                                      \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                clist_del(mm_list_p);                                                                       \
                mm_list_p = NULL;                                                                           \
                                                                                                            \
                break;                                                                                      \
            }                                                                                               \
        }                                                                                                   \
    }                                                                                                       \
    while (0)

clist_t* ctree_find_all (ctree_t *tree_p, ctree_node_t *snode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
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
            if (snode_p)
            {
                if (   search_levels == -1
                    || snode_p->level < search_levels
                   )
                {
                    if (member_match_fn(snode_p->member_p, key_p))
                    {
                        if (!clist_push_back(mm_list_p, snode_p->member_p))
                        {
                            clist_del(mm_list_p);
                            mm_list_p = NULL;
                        }
                    }
                    else
                    {
                        ctree_node_list_find_all(tree_p, snode_p->children, search_levels, key_p, member_match_fn, mm_list_p);
                    }
                }
            }
            else
            {
                ctree_node_list_find_all(tree_p, tree_p->nodes, search_levels, key_p, member_match_fn, mm_list_p);
            }
        }
    }

    return mm_list_p;
}

