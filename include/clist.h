
#ifndef __CLIST_H
#define __CLIST_H
 
#ifdef __cplusplus
extern "C" {
#endif


#include "allocator.h"


#define clist_foreach(ll_p, itr_p) \
	for (itr_p = clist_begin(ll_p); itr_p != clist_end(ll_p); itr_p = clist_iterator_next(itr_p))


typedef struct clist_s clist_t;
typedef struct clist_iterator_s clist_iterator_t;

typedef void (*clist_callback_f) (void *member_p);


extern clist_t* clist_new (const allocator_t *allocator_p);
extern clist_t* clist_del (clist_t *ll_p);

extern void clist_set_member_funcs (clist_t         *ll_p,
                                    member_clone_f   mclone_fn,
                                    member_free_f    mfree_fn,
                                    member_cmp_f     mcmp_fn);
extern void clist_set_member_size (clist_t  *ll_p, size_t member_size);

extern unsigned int clist_count (clist_t *ll_p);
extern int clist_empty (clist_t *ll_p);

extern int clist_push_back_new  (clist_t *ll_p, void *member_p);
extern int clist_push_back      (clist_t *ll_p, void *member_p);
extern int clist_push_front_new (clist_t *ll_p, void *member_p);
extern int clist_push_front     (clist_t *ll_p, void *member_p);

extern int clist_insert_before_new (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_before     (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_after_new  (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_after      (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_at_new     (clist_t *ll_p, int               pos,   void *member_p);
extern int clist_insert_at         (clist_t *ll_p, int               pos,   void *member_p);

extern clist_iterator_t* clist_begin (clist_t *ll_p);
extern clist_iterator_t* clist_end   (clist_t *ll_p);

extern clist_iterator_t* clist_iterator_next (clist_iterator_t *itr_p);
extern clist_iterator_t* clist_iterator_prev (clist_iterator_t *itr_p);

extern void* clist_get (clist_iterator_t *itr_p);
extern void* clist_get_at (clist_t *ll_p, unsigned int pos);

extern void* clist_pop (clist_iterator_t *itr_p);
extern void* clist_pop_at (clist_t *ll_p, unsigned int pos);
extern void* clist_pop_member (clist_t *ll_p, void *member_p);

extern void clist_remove (clist_iterator_t *itr_p);
extern void clist_remove_at (clist_t *ll_p, unsigned int pos);
extern void clist_remove_member (clist_t *ll_p, void *member_p);

extern void clist_clear (clist_t *ll_p);


#if defined(__cplusplus)
}
#endif

 
#endif
