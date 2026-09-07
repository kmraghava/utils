
#ifndef __msg_queue_h__
#define __msg_queue_h__


#ifdef __cplusplus
extern "C" {
#endif


#include <kmrUtils/list.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>


typedef struct msg_data_s {
    int    type;
    void  *data;
   
    list_node_t   llnode;
} msg_data_t;

typedef struct msg_queue_s {
    list_t           msgs;
    pthread_mutex_t  msgs_mutex;
    sem_t            msgs_sem;
} msg_queue_t;


extern msg_queue_t *msg_queue_new (void);
extern void msg_queue_del (msg_queue_t *mqueue_p);

extern bool msg_queue_post (msg_queue_t *mqueue_p, int type, void *data);
extern msg_data_t* msg_queue_recv (msg_queue_t *mqueue_p, int wait_period);


#if defined(__cplusplus)
}
#endif

#endif /* __msg_queue_h__ */