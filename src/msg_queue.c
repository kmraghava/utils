
#include "msg_queue.h"
#include <stdlib.h>
#include <time.h>


msg_queue_t *msg_queue_new (void) {
    msg_queue_t  *mqueue_p = malloc(sizeof(*mqueue_p));
    if (mqueue_p) {
        list_init(mqueue_p->msgs);
        sem_init(&mqueue_p->msgs_sem, 0, 0);
        pthread_mutex_init(&mqueue_p->msgs_mutex, NULL);
    }
    return mqueue_p;
}

void msg_queue_del (msg_queue_t *mqueue_p) {
    if (mqueue_p) {
        list_delete_all(mqueue_p->msgs, msg_data_t, llnode, free);
        sem_destroy(&mqueue_p->msgs_sem);
        pthread_mutex_destroy(&mqueue_p->msgs_mutex);
        free(mqueue_p);
    }
}

bool msg_queue_post (msg_queue_t *mqueue_p, int type, void *data) {
    bool  msg_posted = false;
    if (mqueue_p) {
        msg_data_t  *msg_data_p = malloc(sizeof(*msg_data_p));
        if (msg_data_p) {
            msg_data_p->type = type;
            msg_data_p->data = data;
            pthread_mutex_lock(&mqueue_p->msgs_mutex);
            {
                list_push_back(mqueue_p->msgs, msg_data_p->llnode);
            }
            pthread_mutex_unlock(&mqueue_p->msgs_mutex);
            sem_post(&mqueue_p->msgs_sem);
            msg_posted = true;
        }
    }
    return msg_posted;
}

msg_data_t* msg_queue_recv (msg_queue_t *mqueue_p, int wait_period) {
    msg_data_t  *msg_data_p = NULL;
    if (mqueue_p) {
        if (wait_period < 0) {
            sem_wait(&mqueue_p->msgs_sem);
            pthread_mutex_lock(&mqueue_p->msgs_mutex);
            {
                msg_data_p = list_first_member(mqueue_p->msgs, msg_data_t, llnode);
                list_remove(mqueue_p->msgs, msg_data_p->llnode);
            }
            pthread_mutex_unlock(&mqueue_p->msgs_mutex);
        }
        else if (wait_period == 0) {
            if (0 == sem_trywait(&mqueue_p->msgs_sem)) {
                pthread_mutex_lock(&mqueue_p->msgs_mutex);
                {
                    msg_data_p = list_first_member(mqueue_p->msgs, msg_data_t, llnode);
                    list_remove(mqueue_p->msgs, msg_data_p->llnode);
                }
                pthread_mutex_unlock(&mqueue_p->msgs_mutex);
            }
        }
        else {
            struct timespec  ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += wait_period;
            if (0 == sem_timedwait(&mqueue_p->msgs_sem, &ts)) {
                pthread_mutex_lock(&mqueue_p->msgs_mutex);
                {
                    msg_data_p = list_first_member(mqueue_p->msgs, msg_data_t, llnode);
                    list_remove(mqueue_p->msgs, msg_data_p->llnode);
                }
                pthread_mutex_unlock(&mqueue_p->msgs_mutex);
            }
        }
    }
    return msg_data_p;
}

