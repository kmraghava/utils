
#ifndef __CTREE_H
#define __CTREE_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "clist.h"
#include <stdbool.h>


typedef struct ctree_node_s ctree_node_t;
typedef struct ctree_s ctree_t;


extern ctree_t* ctree_new (ctree_node_t *parent_p);
extern ctree_t* ctree_del (ctree_t *tree_p);

extern ctree_node_t* ctree_node_new (ctree_t *parent_p, void *member_p);
extern ctree_node_t* ctree_node_del (ctree_node_t *tnode_p);

extern int ctree_count (ctree_t *tree_p);
extern int ctree_level (ctree_t *tree_p);
extern int ctree_node_count (ctree_node_t *tnode_p);
extern int ctree_node_level (ctree_node_t *tnode_p);

#define ctree_node_is_leaf(tnode_p)  (ctree_node_count(tnode_p) == 1)

extern void* ctree_node_member (ctree_node_t *tnode_p);

extern ctree_node_t* ctree_first_node (ctree_t *tree_p);
extern ctree_node_t* ctree_last_node  (ctree_t *tree_p);

extern ctree_node_t* ctree_node_next (ctree_node_t *tnode_p);

extern void ctree_add_node (ctree_t *tree_p, ctree_node_t *tnode_p);

extern ctree_t* ctree_node_get_subtree (ctree_node_t *tnode_p);
extern void ctree_node_set_subtree (ctree_node_t *tnode_p, ctree_t *subtree_p);

extern void*    ctree_find     (ctree_t *tree_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));
extern clist_t* ctree_find_all (ctree_t *tree_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));

extern void*    ctree_node_find     (ctree_node_t *tnode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));
extern clist_t* ctree_node_find_all (ctree_node_t *tnode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));


#if defined(__cplusplus)
}
#endif

#endif
