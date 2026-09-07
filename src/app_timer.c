
#include <stdlib.h>
#include "app_timer.h"


#define TIMER_ADD            1
#define TIMER_DEL            2


#define app_timer_log(tt_p, level, fmt_p, ...)                                                                           \
    do                                                                                                                   \
    {                                                                                                                    \
        logger_log_line(tt_p->logger_p, __FILE__, __LINE__, __func__, NULL, level, "timer", NULL, fmt_p, ##__VA_ARGS__); \
        logger_flush(tt_p->logger_p);                                                                                    \
    }                                                                                                                    \
    while (0)


static void app_timer_add (app_timer_task_t  *tt_p, app_timer_t  *tp) {
    app_timer_t  *i_tp;
    list_foreach_member(tt_p->timers, app_timer_t, llnode, i_tp) {
        if (tp->expiry_time <= i_tp->expiry_time) {
            list_insert_before(tt_p->timers, &i_tp->llnode, tp->llnode);
            return;
        }
    }
    list_push_back(tt_p->timers, tp->llnode);
}

static void* app_timer_run (void *ctx) {
    app_timer_task_t  *tt_p = ctx;
    int                wait_period = -1;
    while (true) {
        msg_data_t   *md = msg_queue_recv(tt_p->mqueue_p, wait_period);
        app_timer_t  *tp;
        if (md) {
            switch(md->type) {
                case TIMER_ADD:
                    app_timer_add(tt_p, md->data); 
                    break;

                case TIMER_DEL:
                    tp = md->data;
                    list_remove(tt_p->timers, tp->llnode);
                    free(tp);
                    break;

                default:
                    break;
            }
            free(md);
        }
        else {
            time_t  now = app_timer_now_monotonic();
            while (true) {
                tp = list_first_member(tt_p->timers, app_timer_t, llnode);
                if (!tp || tp->expiry_time > now) {
                    break;
                }
                app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Timer %s(%d, %s) expired", tp->name_p, tp->type, tp->period > 0 ? "true" : "false");
                list_remove(tt_p->timers, tp->llnode);
                tp->cb(tt_p, tp->type, tp->data);
                if (tp->period > 0) {
                    tp->expiry_time = now + tp->period;
                    app_timer_add(tt_p, tp);
                }
                else {
                    free(tp);
                }
            }
            tp = list_first_member(tt_p->timers, app_timer_t, llnode);
            if (tp) {
                wait_period = tp->expiry_time - now;
            }
            else {
                wait_period = -1;
            }
        }
    }
    return NULL;
}

app_timer_task_t* app_timer_task_new (logger_t  *logger_p) {
    app_timer_task_t  *tt_p = malloc(sizeof(*tt_p));
    if (tt_p) {
        tt_p->logger_p = logger_p;
        list_init(tt_p->timers);
        tt_p->mqueue_p = msg_queue_new();
        if (!tt_p->mqueue_p) {
            app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Failed to create message queue for timer task");
            free(tt_p);
            tt_p = NULL;
        }
        else if (pthread_create(&tt_p->thr, NULL, app_timer_run, tt_p) != 0) {
            app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Failed to create pthread for timer task");
            msg_queue_del(tt_p->mqueue_p);
            free(tt_p);
            tt_p = NULL;
        }
        else {
            app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Timer task initialized");
        }
    }
    return tt_p;
}

void app_timer_task_del (app_timer_task_t  *tt_p) {
    if (tt_p) {
        pthread_cancel(tt_p->thr);
        pthread_join(tt_p->thr, NULL);
        msg_queue_del(tt_p->mqueue_p);
        list_delete_all(tt_p->timers, app_timer_t, llnode, free);
        free(tt_p);
    }
}

app_timer_t* app_timer_start (app_timer_task_t  *tt_p, time_t  interval, bool  periodic_b, const char  *name_p, int  type, void  *data, app_timer_callback_f  cb) {
    app_timer_t  *tp = NULL;
    if (tt_p && name_p && cb) {
        tp = malloc(sizeof(*tp));
        if (tp) {
            tp->name_p = name_p;
            tp->type = type;
            tp->data = data;
            tp->expiry_time = app_timer_now_monotonic() + interval;
            tp->period = periodic_b? interval : 0;
            tp->cb = cb;

            if (!msg_queue_post(tt_p->mqueue_p, TIMER_ADD, tp)) {
                app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Failed to post Timer %s(%d, %s) ADD message to timer task", name_p, type, tp->period > 0 ? "true" : "false");
                free(tp);
                tp = NULL;
            }
            else {
                app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Timer %s(%d, %s) started", tp->name_p, tp->type, tp->period > 0 ? "true" : "false");
            }
        }
    }
    return tp;
}

bool app_timer_stop (app_timer_task_t  *tt_p, app_timer_t  *tp) {
    bool  stopped_b = false;
    if (tt_p && tp) {
        if (!msg_queue_post(tt_p->mqueue_p, TIMER_DEL, tp)) {
            app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Failed to post Timer %s(%d, %s) DEL message to timer task", tp->name_p, tp->type, tp->period > 0 ? "true" : "false");
        }
        else {
            stopped_b = true;
            app_timer_log(tt_p, LOG_LEVEL_MEDIUM, "Timer %s(%d, %s) stopped", tp->name_p, tp->type, tp->period > 0? "true" : "false");
        }
    }
    return stopped_b;
}

time_t app_timer_now_monotonic (void) {
    struct timespec  ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec;
}

