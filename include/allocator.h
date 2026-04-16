
#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H
 
#ifdef __cplusplus
extern "C" {
#endif
 

#include <string.h>


typedef void* (*malloc_f ) (size_t size);
typedef void* (*calloc_f ) (size_t num, size_t size);
typedef void* (*realloc_f) (void *ptr, size_t size);
typedef void  (*free_f   ) (void *ptr);

typedef struct allocator_s
{
    malloc_f   malloc;
    calloc_f   calloc;
    realloc_f  realloc;
    free_f     free;

} allocator_t;

typedef void* (*member_clone_f) (allocator_t  *allocator_p,
                                 void         *member_p,
                                 size_t        member_size);
typedef void (*member_free_f) (allocator_t  *allocator_p,
                               void         *member_p);
typedef int (*member_cmp_f) (void    *member1_p,
                             void    *member2_p,
                             size_t   member_size);


static inline void* member_clone (allocator_t  *allocator_p,
                                  void         *member_p,
                                  size_t        member_size)
{
    void  *clone_p = NULL;

    if (   allocator_p
        && member_p
        && member_size > 0
       )
    {
        clone_p = allocator_p->malloc(member_size);

        if (clone_p)
            memcpy(clone_p, member_p, member_size);
    }

    return clone_p;
}

static inline void member_free (allocator_t  *allocator_p,
                                void         *member_p)
{
    if (   allocator_p
        && member_p
       )
    {
        allocator_p->free(member_p);
    }
}

static inline int member_cmp (void    *member1_p,
                              void    *member2_p,
                              size_t   member_size)
{
    int  ret = 0;

    if (member1_p)
    {
        if (member2_p)  ret = memcmp(member1_p, member2_p, member_size);
        else            ret = +1;
    }
    else if (member2_p) ret = -1;

    return ret;
}


#if defined(__cplusplus)
}
#endif

#endif
