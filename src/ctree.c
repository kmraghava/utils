
#include "list.h"
#include "ctree.h"
#include <stdlib.h>


struct ctree_node_s
{
    ctree_t       *parent_p;
    ctree_t       *subtree_p;

    int            level;
    int            count;

    void          *member_p;

    list_node_t    llnode;
};

struct ctree_s
{
    ctree_node_t  *parent_p;
    list_t         nodes;

    int            level;
    int            count;
};


static void ctree_free (ctree_t *tree_p);
static void ctree_node_free (ctree_node_t *tnode_p);

static void ctree_count_incr (ctree_t *tree_p, ctree_node_t *tnode_p, int count);
static void ctree_count_decr (ctree_t *tree_p, ctree_node_t *tnode_p, int count);

static void ctree_set_level (ctree_t *tree_p);

static void ctree_free (ctree_t *tree_p)
{
    if (tree_p)
    {
        list_delete_all(tree_p->nodes, ctree_node_t, llnode, ctree_node_free);
        free(tree_p);
    }
}

static void ctree_node_free (ctree_node_t *tnode_p)
{
    if (tnode_p)
    {
        ctree_free(tnode_p->subtree_p);
        free(tnode_p);
    }
}

static void ctree_count_incr (ctree_t *tree_p, ctree_node_t *tnode_p, int count)
{
    while (tree_p || tnode_p)
    {
        if (tree_p)
        {
            tree_p->count += count;

            tnode_p = tree_p->parent_p;
            tree_p = NULL;
        }

        if (tnode_p)
        {
            tnode_p->count += count;

            tree_p = tnode_p->parent_p;
            tnode_p = NULL;
        }
    }
}

static void ctree_count_decr (ctree_t *tree_p, ctree_node_t *tnode_p, int count)
{
    while (tree_p || tnode_p)
    {
        if (tree_p)
        {
            tree_p->count -= count;

            tnode_p = tree_p->parent_p;
            tree_p = NULL;
        }

        if (tnode_p)
        {
            tnode_p->count -= count;

            tree_p = tnode_p->parent_p;
            tnode_p = NULL;
        }
    }
}

static void ctree_set_level (ctree_t *tree_p)
{
    if (tree_p)
    {
        list_node_t   *lnd_p;
        ctree_node_t  *tnd_p;

        if (tree_p->parent_p)
            tree_p->level = tree_p->parent_p->level + 1;

        list_foreach_member(tree_p->nodes, lnd_p, ctree_node_t, llnode, tnd_p)
        {
            tnd_p->level = tree_p->level;
            ctree_set_level(tnd_p->subtree_p);
        }
    }
}


ctree_t* ctree_new (ctree_node_t *parent_p)
{
    ctree_t  *tree_p = calloc(1, sizeof(*tree_p));

    if (tree_p)
    {
        list_init(tree_p->nodes);

        if (parent_p)
        {
            tree_p->parent_p = parent_p;
            tree_p->level = parent_p->level + 1;

            if (parent_p->subtree_p)
                ctree_del(parent_p->subtree_p);
            parent_p->subtree_p = tree_p;
        }
    }

    return tree_p;
}

ctree_t* ctree_del (ctree_t *tree_p)
{
    if (tree_p)
    {
        if (tree_p->parent_p)
        {
            ctree_count_decr(NULL, tree_p->parent_p, tree_p->count);
            tree_p->parent_p->subtree_p = NULL;
        }

        ctree_free(tree_p);
    }

    return NULL;
}

ctree_node_t* ctree_node_new (ctree_t *parent_p, void *member_p)
{
    ctree_node_t  *tnode_p = calloc(1, sizeof(*tnode_p));;

    if (tnode_p)
    {
        tnode_p->member_p = member_p;
        tnode_p->count = 1;

        if (parent_p)
        {
            tnode_p->parent_p = parent_p;
            list_push_back(parent_p->nodes, tnode_p->llnode);

            tnode_p->level = parent_p->level;
            ctree_count_incr(parent_p, NULL, 1);
        }
    }

    return tnode_p;
}

ctree_node_t* ctree_node_del (ctree_node_t *tnode_p)
{
    if (tnode_p)
    {
        if (tnode_p->parent_p)
        {
            list_remove(tnode_p->parent_p->nodes, tnode_p->llnode);
            ctree_count_decr(tnode_p->parent_p, NULL, tnode_p->count);
        }

        ctree_node_free(tnode_p);
    }
    
    return NULL;
}

int ctree_count (ctree_t *tree_p) { return tree_p ? tree_p->count : -1; }
int ctree_level (ctree_t *tree_p) { return tree_p ? tree_p->level : -1; }
int ctree_node_count (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->count : -1; }
int ctree_node_level (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->level : -1; }

void* ctree_node_member (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->member_p : NULL; }

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

        ctree_count_incr(tree_p, NULL, tnode_p->count);
        tnode_p->level = tree_p->level;
        ctree_set_level(tnode_p->subtree_p);
    }
}

ctree_t* ctree_node_get_subtree (ctree_node_t *tnode_p) { return tnode_p ? tnode_p->subtree_p : NULL; }
void ctree_node_set_subtree (ctree_node_t *tnode_p, ctree_t *subtree_p)
{
    if (   tnode_p
        && subtree_p
       )
    {
        if (tnode_p->subtree_p)
            ctree_del(tnode_p->subtree_p);

        tnode_p->subtree_p = subtree_p;
        subtree_p->parent_p = tnode_p;
    
        ctree_count_incr(NULL, tnode_p, subtree_p->count);
        ctree_set_level(subtree_p);
    }
}

void* ctree_find (ctree_t *tree_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   tree_p
        && key_p
        && member_match_fn

        && (   search_levels == -1
            || tree_p->level < search_levels
           )
       )
    {
        list_node_t   *lnd_p;
        ctree_node_t  *tnd_p;

        list_foreach_member(tree_p->nodes, lnd_p, ctree_node_t, llnode, tnd_p)
        {
            matching_member_p = ctree_node_find(tnd_p, search_levels, key_p, member_match_fn);
            if (matching_member_p)
                break;
        }
    }

    return matching_member_p;
}

void* ctree_node_find (ctree_node_t *tnode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
{
    void  *matching_member_p = NULL;

    if (   tnode_p
        && key_p
        && member_match_fn

        && (   search_levels == -1
            || tnode_p->level < search_levels
           )
       )
    {
        matching_member_p =   member_match_fn(tnode_p->member_p, key_p)
                            ? tnode_p->member_p
                            : ctree_find(tnode_p->subtree_p, search_levels, key_p, member_match_fn);
    }

    return matching_member_p;
}

clist_t* ctree_find_all (ctree_t *tree_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
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
            if (   search_levels == -1
                || tree_p->level < search_levels
               )
            {
                list_node_t   *lnd_p;
                ctree_node_t  *tnd_p;

                list_foreach_member(tree_p->nodes, lnd_p, ctree_node_t, llnode, tnd_p)
                {
                    clist_t  *stn_mm_list_p = ctree_node_find_all(tnd_p, search_levels, key_p, member_match_fn);
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
    }

    return mm_list_p;
}

clist_t* ctree_node_find_all (ctree_node_t *tnode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p))
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
            if (   search_levels == -1
                || tnode_p->level < search_levels
               )
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
                    st_mm_list_p = ctree_find_all(tnode_p->subtree_p, search_levels, key_p, member_match_fn);
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
    }

    return mm_list_p;
}
