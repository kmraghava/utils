
#ifndef __CTREE_H
#define __CTREE_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "clist.h"
#include <stdbool.h>


typedef struct ctree_node_s ctree_node_t;
typedef struct ctree_s ctree_t;


extern ctree_t* ctree_new ();
extern ctree_t* ctree_del (ctree_t *tree_p);

extern int ctree_count (ctree_t *tree_p);
extern int ctree_node_count (ctree_node_t *tnode_p);
extern int ctree_node_level (ctree_node_t *tnode_p);

extern ctree_node_t* ctree_add (ctree_t *tree_p, ctree_node_t *pnode_p, void *member_p);

extern void ctree_rem_node (ctree_t *tree_p, ctree_node_t *tnode_p);

extern void* ctree_member (ctree_node_t *tnode_p);

extern void*    ctree_find     (ctree_t *tree_p, ctree_node_t *snode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));
extern clist_t* ctree_find_all (ctree_t *tree_p, ctree_node_t *snode_p, int search_levels, void *key_p, bool (*member_match_fn)(void *member_p, void *key_p));


#if defined(__cplusplus)
}
#endif

#endif
