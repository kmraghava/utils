/*****************************************************************************
 *
 * FILE NAME     : logger.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : September 12, 2025
 *
 *****************************************************************************
 *
 *  DESCRIPTION : Logger
 *
 *****************************************************************************/

#ifndef __LOGGER_H
#define __LOGGER_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include "jansson.h"
 

/*****************************************************************************
 * Global Defines
 *****************************************************************************/

/*****************************************************************************
 * Global Constants
 *****************************************************************************/
#define LOG_FILENAME_MAX_LEN            257

#define LOGGER_PARAMS_INIT  { LOG_LEVEL_HIGH, NULL, 0, NULL }
#define logger_set_params(logger_p, lparams_p)                 \
    do                                                         \
    {                                                          \
        logger_set_level    (logger_p, lparams_p->level     ); \
        logger_set_filename (logger_p, lparams_p->filename_p); \
        logger_set_max_size (logger_p, lparams_p->max_size  ); \
        logger_set_log_fn   (logger_p, lparams_p->log_fn)    ; \
    }                                                          \
    while(0)

#define __FILE_LINE_FUNC__  __FILE__, __LINE__, __func__


/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef enum
{
    LOG_LEVEL_HIGH,
    LOG_LEVEL_MEDIUM,
    LOG_LEVEL_LOW,
    LOG_LEVEL_VERY_LOW,
    LOG_LEVEL_VERY_VERY_LOW

} loglevel_e;

typedef struct logger_s logger_t;
typedef void (*log_f) (const char *line_p);

typedef struct logger_params_s
{
    loglevel_e     level;
    const char    *filename_p;
    unsigned int   max_size;
    log_f          log_fn;

} logger_params_t;


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 
/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
extern logger_t* logger_create  (void);
extern logger_t* logger_destroy (logger_t *logger_p);

extern void logger_set_level    (logger_t *logger_p, loglevel_e level);
extern void logger_set_filename (logger_t *logger_p, const char *filename_p);
extern void logger_set_max_size (logger_t *logger_p, unsigned int max_size);
extern void logger_set_log_fn   (logger_t *logger_p, log_f log_fn);

extern void logger_log_line (logger_t *logger_p,
                             const char *file, const int line, const char *func,
                             const char *utc_time_p,
                             loglevel_e level,
                             const char *module_p, const char *tag_p,
                             const char *fmt_p, ...);
extern void logger_log_json (logger_t *logger_p,
                             const char *file, const int line, const char *func,
                             const char *utc_time_p,
                             loglevel_e level,
                             const char *module_p, const char *tag_p,
                             json_t *jobj_p);

extern void logger_flush (logger_t *logger_p);


#if defined(__cplusplus)
}
#endif
 
#endif /* __LOGGER_H */

