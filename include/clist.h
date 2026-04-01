/*****************************************************************************
 *
 * FILE NAME     : clist.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : November 21, 2025
 *
 *****************************************************************************
 *
 *  DESCRIPTION : Double link list (Container)
 *
 *****************************************************************************/

#ifndef __CLIST_H
#define __CLIST_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/

/*****************************************************************************
 * Global Defines
 *****************************************************************************/
#define clist_foreach(ll_p, itr_p) \
	for (itr_p = clist_begin(ll_p); itr_p != clist_end(ll_p); itr_p = clist_iterator_next(itr_p))

#define clist_reverse_foreach(ll_p, itr_p) \
	for (itr_p = clist_rbegin(ll_p); itr_p != clist_rend(ll_p); itr_p = clist_iterator_prev(itr_p))


/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct clist_s clist_t;
typedef struct clist_iterator_s clist_iterator_t;

typedef void (*clist_callback_f) (void *member_p);


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 
/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
extern clist_t* clist_new ();
extern clist_t* clist_del (clist_t *ll_p);

extern unsigned int clist_count (clist_t *ll_p);
extern int clist_empty (clist_t *ll_p);

extern int clist_push_back  (clist_t *ll_p, void *member_p);
extern int clist_push_front (clist_t *ll_p, void *member_p);

extern int clist_insert_before (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_after  (clist_t *ll_p, clist_iterator_t *itr_p, void *member_p);
extern int clist_insert_at     (clist_t *ll_p, int               pos,   void *member_p);

extern clist_iterator_t* clist_begin (clist_t *ll_p);
extern clist_iterator_t* clist_end   (clist_t *ll_p);

extern clist_iterator_t* clist_rbegin (clist_t *ll_p);
extern clist_iterator_t* clist_rend   (clist_t *ll_p);

extern clist_iterator_t* clist_iterator_next (clist_iterator_t *itr_p);
extern clist_iterator_t* clist_iterator_prev (clist_iterator_t *itr_p);

extern void* clist_get (clist_iterator_t *itr_p);
extern void* clist_get_at (clist_t *ll_p, unsigned int pos);

extern void* clist_remove (clist_iterator_t *itr_p);
extern void* clist_remove_at (clist_t *ll_p, unsigned int pos);
extern void  clist_remove_member (clist_t *ll_p, void *member_p);

extern void clist_clear (clist_t *ll_p);


#if defined(__cplusplus)
}
#endif

 
#endif /* __CLIST_H */
