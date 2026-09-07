
#ifndef __app_timer_h__
#define __app_timer_h__

#ifdef __cplusplus
extern "C" {
#endif


#include <kmrUtils/list.h>
#include <kmrUtils/logger.h>
#include "msg_queue.h"
#include <pthread.h>
#include <stdbool.h>
#include <time.h>


typedef struct app_timer_task_s {
    pthread_t     thr;
    msg_queue_t  *mqueue_p;
    logger_t     *logger_p;
    list_t        timers;
} app_timer_task_t;

typedef void (*app_timer_callback_f) (app_timer_task_t  *tt_p, int  type, void  *data);

typedef struct app_timer_s {
    time_t                 expiry_time;
    time_t                 period;
    const char            *name_p;
    int                    type;
    void                  *data;
    app_timer_callback_f   cb;

    list_node_t   llnode;
} app_timer_t;

extern app_timer_task_t* app_timer_task_new (logger_t  *logger_p);

extern app_timer_t* app_timer_start (app_timer_task_t  *tt_p, time_t  interval, bool  periodic_b, const char  *name_p, int  type, void  *data, app_timer_callback_f  cb);
extern bool app_timer_stop (app_timer_task_t  *tt_p, app_timer_t  *tp);

extern time_t app_timer_now_monotonic (void);


#if defined(__cplusplus)
}
#endif

#endif /* __app_timer_h__ */

