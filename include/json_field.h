/*****************************************************************************
 *
 * FILE NAME     : json_field.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : December 12, 2025
 *
 *****************************************************************************
 *
 *  DESCRIPTION : Json field.
 *                Set / Get JSON fields based on path.
 *
 *****************************************************************************/

#ifndef __JSON_FIELD_H
#define __JSON_FIELD_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include "jansson.h"
#include <stdbool.h>


/*****************************************************************************
 * Global Defines
 *****************************************************************************/
#define json_string_get(jobj_p, field_path) \
    json_string_value(json_field_get(jobj_p, field_path))

#define json_integer_get(jobj_p, field_path) \
    json_integer_value(json_field_get(jobj_p, field_path))

#define json_real_get(jobj_p, field_path) \
    json_real_value(json_field_get(jobj_p, field_path))

#define json_bool_get(jobj_p, field_path) \
    json_is_true(json_field_get(jobj_p, field_path)) ? true : false

#define json_string_set(jobj_pp, field_path, value) \
    json_field_set_new(jobj_pp, field_path, json_string(value))

#define json_integer_set(jobj_pp, field_path, value) \
    json_field_set_new(jobj_pp, field_path, json_integer(value))

#define json_real_set(jobj_pp, field_path, value) \
    json_field_set_new(jobj_pp, field_path, json_real(value))

#define json_bool_set(jobj_pp, field_path, value) \
    json_field_set_new(jobj_pp, field_path, (value)? json_true() : json_false())

#define json_field_set(jobj_pp, field_path, field_jobj_p) \
    json_field_set_new(jobj_pp, field_path, json_incrref(field_jobj_p))


/*****************************************************************************
 * Global Constants
 *****************************************************************************/

/*****************************************************************************
 * Global Types
 *****************************************************************************/

/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 
/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
extern json_t* json_field_get (json_t *jobj_p, const char *field_path);
extern bool json_field_set_new (json_t **jobj_pp, const char *field_path, json_t *field_jobj_p);


/*****************************************************************************
   Test Functions
*****************************************************************************/


#if defined(__cplusplus)
}
#endif

#endif /* __JSON_FIELD_H */
