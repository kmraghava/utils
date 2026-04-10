/*****************************************************************************
 *
 *  FILE NAME     : logger.c
 *  MODULE        : utils
 *  AUTHOR        : KM Raghava
 *  CREATION DATE : September 13, 2025
 *
 ******************************************************************************
 *
 *  DESCRIPTION : Logger
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*****************************************************************************
   External Declarations
*****************************************************************************/

/*****************************************************************************
   Local Constants
*****************************************************************************/
#define LOG_FILE_EXT_LEN                           2           // ".0" or ".1"
#define LOGGER_MIN_FILE_SIZE                       0x2800      // 10 KB or 10240 Bytes
#define LOGGER_MAX_FILE_SIZE                       0x100000    // 1 MB or 1024 x 1024 Bytes
#define LOGGER_MAX_LINE_LEN                        1024        // 1024 Bytes


/*****************************************************************************
   Local Types
*****************************************************************************/
struct logger_s
{
    loglevel_e     level;                                    // Log level. Logs that are at <= this log level are recorded.

    char           filename[LOG_FILENAME_MAX_LEN+1];         // Full path of the file to log messages to. Can be empty.
    FILE          *fptr;                                     // File handle
    unsigned int   file_no;                                  // Current file number.
    unsigned int   max_file_size;                            // Max size of the file.

    log_f          log_cb;                                   // Custom log function.
};


/*****************************************************************************
   Local Variables
*****************************************************************************/

/*****************************************************************************
   Global Variables
*****************************************************************************/

/*****************************************************************************
   Local Macros
******************************************************************************/

/*****************************************************************************
   Local Function Prototypes
*****************************************************************************/
static void logger_stdout (const char *line_p);

static const char* logger_fname (const char *path);


/*****************************************************************************
   Local Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : logger_stdout
 *
 *  DESCRIPTION : Prints the given line to stdout
 *
 *  PARAMS      : line_p - Line to be printed
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
static void logger_stdout (const char *line_p)
{
    printf("%s\n", line_p);
}

/*****************************************************************************
 *
 *  NAME        : logger_fname
 *
 *  DESCRIPTION : Given full path to a file, returns file name.
 *
 *  PARAMS      : path - Full path to a file
 *
 *  RETURNS     : file name
 *
 *****************************************************************************/
static const char* logger_fname (const char *path)
{
    if (path)
    {
        const char *slash = strrchr(path, '/');

        return   slash
               ? slash + 1
               : path;
    }
    else
        return "(unknown file)";
}


/*****************************************************************************
   Global Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : logger_create
 *
 *  DESCRIPTION : Creates a new logger module
 *
 *  PARAMS      : void
 *
 *  RETURNS     : logger_t
 *
 *****************************************************************************/
logger_t* logger_create (void)
{
    logger_t  *logger_p = calloc(1, sizeof(logger_t));

    if (logger_p)
    {
        logger_p->max_file_size = LOGGER_MAX_FILE_SIZE;
        logger_p->log_cb = logger_stdout;
    }

    return logger_p;
}

/*****************************************************************************
 *
 *  NAME        : logger_destroy
 *
 *  DESCRIPTION : Destroy the given logger module
 *
 *  PARAMS      : logger_p - Logger
 *
 *  RETURNS     : NULL
 *
 *****************************************************************************/
logger_t* logger_destroy (logger_t *logger_p)
{
    if (logger_p)
    {
        if (logger_p->fptr)
            fclose(logger_p->fptr);

        free(logger_p);
        logger_p = NULL;
    }

    return logger_p;
}

/*****************************************************************************
 *
 *  NAME        : logger_set_level
 *                logger_set_filename
 *                logger_set_max_size
 *                logger_set_log_fn
 *
 *  DESCRIPTION : Set logger module parameters
 *
 *  PARAMS      : logger_p   - Logger
 *                level      - Log level
 *                filename_p - Name of the file to write out the logs
 *                max_lines  - Maximum number of lines after which this logger
 *                             module will wrap.
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
void logger_set_level (logger_t *logger_p, loglevel_e level)
{
    if (logger_p)
        logger_p->level = level;
}
void logger_set_filename (logger_t *logger_p, const char *filename_p)
{
    if (   logger_p
        && filename_p
        && strlen(filename_p) < LOG_FILENAME_MAX_LEN
       )
    {
        char  fname[LOG_FILENAME_MAX_LEN + LOG_FILE_EXT_LEN + 1];

        strcpy(logger_p->filename, filename_p);
        logger_p->file_no = 0;

        if (logger_p->fptr)
            fclose(logger_p->fptr);

        sprintf(fname, "%s.0", logger_p->filename);
        logger_p->fptr = fopen(fname, "w");
    }
}
void logger_set_max_size (logger_t *logger_p, unsigned int max_size)
{
    if (   logger_p
        && max_size >= LOGGER_MIN_FILE_SIZE
       )
    {
        logger_p->max_file_size = max_size;
    }
}
void logger_set_log_fn (logger_t *logger_p, log_f log_fn)
{
    if (   logger_p
        && log_fn
       )
    {
        logger_p->log_cb = log_fn;
    }
}

/*****************************************************************************
 *
 *  NAME        : logger_log_line
 *
 *  DESCRIPTION : Log a line
 *
 *  PARAMS      : logger_p   - Logger
 *                file       - The file that called this function
 *                line       - Line number in the file from where this
 *                             function was called.
 *                func       - Function that called this function
 *                utc_time_p - UTC time in ISO 8601 format
 *                level      - Log level of the line
 *                module_p   - Name of the module that is generating this
 *                             log line
 *                tag_p      - Identifier that can be used to filter logs
 *                             later. For example device MAC address
 *                fmt_p, ... - printf style params
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
void logger_log_line (logger_t *logger_p,
                      const char *file, const int line, const char *func,
                      const char *utc_time_p,
                      loglevel_e level,
                      const char *module_p, const char *tag_p,
                      const char *fmt_p, ...)
{
    if (   logger_p
        && logger_p->level >= level
       )
    {
        char     ll[LOGGER_MAX_LINE_LEN+1];
        int      ll_size = 0;
        va_list  args;

        ll[LOGGER_MAX_LINE_LEN] = '\0';

        if (   utc_time_p
        	&& utc_time_p[0]
           )
        {
            ll_size += sprintf(ll + ll_size, "[ %s ]", utc_time_p);
        }
        if (   module_p
            && module_p[0]
           )
        {
            ll_size += sprintf(ll + ll_size, "[ %s ]", module_p);
        }
        if (   tag_p
            && tag_p[0]
           )
        {
            ll_size += sprintf(ll + ll_size, "[ %s ]", tag_p);
        }

        ll_size += sprintf(ll + ll_size, "[ %s ][ %d ][ %s ] ", logger_fname(file), line, func);

        va_start(args, fmt_p);
        ll_size += vsnprintf(ll + ll_size, LOGGER_MAX_LINE_LEN - ll_size, fmt_p, args);
        va_end(args);

        if (ll_size > LOGGER_MAX_LINE_LEN)
            ll_size = LOGGER_MAX_LINE_LEN;

        if (logger_p->log_cb)
            logger_p->log_cb(ll);

        if (   logger_p->fptr
            && ftell(logger_p->fptr) + ll_size > logger_p->max_file_size
           )
        {
            static const char  *sfno[2] = { ".0", ".1" };

            char  fname[LOG_FILENAME_MAX_LEN + LOG_FILE_EXT_LEN];

            fclose(logger_p->fptr);

            logger_p->file_no = (logger_p->file_no + 1) % 2;
            sprintf(fname, "%s.%s", logger_p->filename, sfno[logger_p->file_no]);
            logger_p->fptr = fopen(fname, "w");
        }

        if (logger_p->fptr)
            fprintf(logger_p->fptr, "%s\n", ll);
    }
}

/*****************************************************************************
 *
 *  NAME        : logger_log_json
 *
 *  DESCRIPTION : Log a JSON object
 *
 *  PARAMS      : logger_p   - Logger
 *                file       - The file that called this function
 *                line       - Line number in the file from where this
 *                             function was called.
 *                func       - Function that called this function
 *                utc_time_p - UTC time in ISO 8601 format
 *                level      - Log level of the line
 *                module_p   - Name of the module that is generating this
 *                             log line
 *                tag_p      - Identifier that can be used to filter logs
 *                             later. For example device MAC address
 *                jobj_p     - JSON object
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
void logger_log_json (logger_t *logger_p,
                      const char *file, const int line, const char *func,
                      const char *utc_time_p,
                      loglevel_e level,
                      const char *module_p, const char *tag_p,
                      json_t *jobj_p)
{
    if (   logger_p
        && logger_p->level >= level
       )
    {
        char  *jobj_str = json_dumps(jobj_p, JSON_INDENT(4));

        if (jobj_str)
        {
            char  *next_ll;
            char  *ll = strtok_r(jobj_str, "\n", &next_ll);

            while (ll != NULL)
            {
                logger_log_line(logger_p,
                                file, line, func,
                                utc_time_p,
                                level,
                                module_p, tag_p,
                                "%s", ll);

                ll = strtok_r(NULL, "\n", &next_ll);
            }
        }

        free(jobj_str);
    }
}

/*****************************************************************************
 *
 *  NAME        : logger_flush
 *
 *  DESCRIPTION : Flush output stream
 *
 *  PARAMS      : logger_p - Logger
 *
 *  RETURNS     : void
 *
 *****************************************************************************/
void logger_flush (logger_t *logger_p)
{
    if (logger_p)
    {
        if (logger_p->log_cb == logger_stdout)
            fflush(stdout);

        if (logger_p->fptr)
            fflush(logger_p->fptr);
    }
}


/*****************************************************************************
   Test Functions
*****************************************************************************/

