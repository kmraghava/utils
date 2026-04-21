
#ifndef __CLIST_H
#define __CLIST_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


#define clist_foreach(ll_p, nd_p) \
	for (nd_p = clist_first(ll_p); nd_p != clist_tail(ll_p); nd_p = clist_next(nd_p))

#define clist_rforeach(ll_p, nd_p) \
	for (nd_p = clist_last(ll_p); nd_p != clist_head(ll_p); nd_p = clist_prev(nd_p))

#define clist_foreach_member(ll_p, nd_p, member_p) \
	for (nd_p = clist_first(ll_p); nd_p != clist_tail(ll_p) && (member_p = clist_member(nd_p)); nd_p = clist_next(nd_p))

#define clist_rforeach_member(ll_p, nd_p, member_p) \
	for (nd_p = clist_last(ll_p); nd_p != clist_head(ll_p) && (member_p = clist_member(nd_p)); nd_p = clist_prev(nd_p))


typedef struct clist_s clist_t;
typedef struct clist_node_s clist_node_t;


extern clist_t* clist_new (void);
extern clist_t* clist_del (clist_t *ll_p);

extern int clist_count (clist_t *ll_p);
#define clist_empty(ll_p) (clist_count(ll_p) == 0)

extern clist_node_t* clist_insert_before (clist_t *ll_p, clist_node_t *node_p, void *member_p);

#define clist_push_back(ll_p, member_p)             clist_insert_before(ll_p, clist_tail(ll_p),      member_p)
#define clist_push_front(ll_p, member_p)            clist_insert_before(ll_p, clist_first(ll_p),     member_p)
#define clist_insert_after(ll_p, node_p, member_p)  clist_insert_before(ll_p, clist_next(node_p),    member_p)
#define clist_insert_at(ll_p, pos, member_p)        clist_insert_before(ll_p, clist_node(ll_p, pos), member_p)

extern clist_node_t* clist_head (clist_t *ll_p);
extern clist_node_t* clist_tail (clist_t *ll_p);

extern clist_node_t* clist_first (clist_t *ll_p);
extern clist_node_t* clist_last  (clist_t *ll_p);

extern clist_node_t* clist_next (clist_node_t *node_p);
extern clist_node_t* clist_prev (clist_node_t *node_p);

extern clist_node_t* clist_node (clist_t *ll_p, int pos);

extern void* clist_member (clist_node_t *node_p);
#define clist_member_at(ll_p, pos)  clist_member(clist_node(ll_p, pos))

extern void clist_remove (clist_t *ll_p, clist_node_t *node_p);
#define clist_remove_at(ll_p, pos)  clist_remove(ll_p, clist_node(ll_p, pos))

#define clist_clear(ll_p)  while(clist_count(ll_p)) clist_remove(ll_p, clist_first(ll_p))

#define clist_extend(ll_p, other_p)                         \
    do                                                      \
    {                                                       \
        clist_node_t  *__nd_p;                              \
        void         *__member_p;                           \
                                                            \
        clist_foreach_member(other_p, __nd_p, __member_p)   \
            clist_push_back(ll_p, __member_p);              \
    }                                                       \
    while (0)

extern void clist_append (clist_t *ll_p, clist_t *other_p);

extern void*    clist_find     (clist_t *ll_p, void *key_p, bool (*member_match_fn) (void *member_p, void *key_p));
extern clist_t* clist_find_all (clist_t *ll_p, void *key_p, bool (*member_match_fn) (void *member_p, void *key_p));


#if defined(__cplusplus)
}
#endif

#endif
