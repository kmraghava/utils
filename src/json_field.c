/*****************************************************************************
 *
 * FILE NAME     : json_field.c
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : December 2, 2025
 *
 ******************************************************************************
 *
 *  DESCRIPTION : JSON Field Set / Get APIs
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include "json_field.h"
#include <stdlib.h>
#include <string.h>


/*****************************************************************************
   External Declarations
*****************************************************************************/

/*****************************************************************************
   Local Constants
*****************************************************************************/

/*****************************************************************************
   Local Types
*****************************************************************************/

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

/*****************************************************************************
   Local Functions
*****************************************************************************/

/*****************************************************************************
   Global Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : json_field_get
 *
 *  DESCRIPTION : Follow the given path and retrieve the JSON object
 *
 *  PARAMS      : jobj_p     - Root JSON object
 *                field_path - Path to required JSON object
 *
 *  RETURNS     : JSON object
 *
 *  NOTES       : field_path is in dotted decimal format.
 *
 *                For example -
 *                JSON:
 *                    deviceParams : {
 *                        rfParams : {
 *                            channels : [
 *                                {
 *                                    freq: 3550,
 *                                    power: 10
 *                                },
 *                                {
 *                                    freq: 3560,
 *                                    power: 12
 *                                }
 *                            ]
 *                        },
 *                        "antennaName" : "Parabolic Antenna",
 *                        "status" : true,
 *                        "latitude" : 90.289234,
 *                        "measCapability" : [
 *                            "RECEIVE_POWER_WITHOUT_GRANT",
 *                            "RECEIVE_POWER_WITH_GRANT"
 *                        ]
 *                    }
 *
 *                Path in decimal notation:
 *                    devicePatams.rfParams.channels[0].freq
 *                    devicePatams.rfParams.channels[0].power
 *                    devicePatams.rfParams.channels[1].freq
 *                    devicePatams.rfParams.channels[1].power
 *                    devicePatams.antennaName
 *                    devicePatams.status
 *                    devicePatams.rfParams.latitude
 *                    devicePatams.measCapability[0]
 *                    devicePatams.measCapability[1]
 *
 *****************************************************************************/
json_t* json_field_get (json_t *jobj_p, const char *field_path)
{
    char    *name_p,
            *rem_path,
            *arr_ind;
    int      arr_idx;

    if (   !field_path
        ||  field_path[0] == '\0'
       )
    {
        return NULL;
    }

    rem_path = strdup(field_path);
    if (!rem_path)
        return NULL;

    while (   jobj_p
           && (name_p = strsep(&rem_path, "."))
          )
    {
        arr_ind = strchr(name_p, '[');
        if (arr_ind)
        {
            *arr_ind = '\0';
            arr_idx = atoi(arr_ind + 1);

            if (name_p[0])
                jobj_p = json_object_get(jobj_p, name_p);

            jobj_p = json_array_get(jobj_p, arr_idx);
        }
        else
            jobj_p = json_object_get(jobj_p, name_p);
    }

    free(rem_path);

    return jobj_p;
}

/*****************************************************************************
 *
 *  NAME        : json_field_set_new
 *
 *  DESCRIPTION : At the given path set the given JSON object in the given
 *                root JSON object
 *
 *  PARAMS      : jobj_pp      - Root JSON object
 *                field_path   - Path to required JSON object
 *                field_jobj_p - JSON object to be inserted
 *
 *  RETURNS     : true if the field was set.
 *                false otherwise.
 *
 *****************************************************************************/
bool json_field_set_new (json_t **jobj_pp, const char *field_path, json_t *field_jobj_p)
{
    json_t  *jobj_p;
    char    *name_p,
            *rem_path = NULL,
            *arr_ind;
    int      arr_idx;

    if (!jobj_pp)
        goto FATAL;

    if (   !field_path
        ||  field_path[0] == '\0'
       )
    {
        goto FATAL;
    }

    rem_path = strdup(field_path);
    if (!rem_path)
        goto FATAL;

    if (NULL == *jobj_pp)
    {
        *jobj_pp =   field_path[0] == '['
                   ? json_array()
                   : json_object();

        if (NULL == *jobj_pp)
            goto FATAL;
    }

    jobj_p = *jobj_pp;

    while ( (name_p = strsep(&rem_path, ".")) )
    {
        arr_ind = strchr(name_p, '[');
        if (arr_ind)
        {
            *arr_ind = '\0';
            arr_idx = atoi(arr_ind + 1);

            if (name_p[0])
            {
                json_t  *vjobj_p;

                if (!json_is_object(jobj_p))
                    goto FATAL;

                vjobj_p = json_object_get(jobj_p, name_p);
                if (!vjobj_p)
                {
                    vjobj_p = json_array();
                
                    if (0 != json_object_set_new(jobj_p, name_p, vjobj_p))
                        goto FATAL;
                }

                jobj_p = vjobj_p;
            }

            if (!json_is_array(jobj_p))
                goto FATAL;

            while (json_array_size(jobj_p) < arr_idx)
            {
                if (0 != json_array_append(jobj_p, json_null()))
                    goto FATAL;
            }

            if (   !rem_path
                || !rem_path[0]
               )
            {
                if (arr_idx < json_array_size(jobj_p))
                {
                    if (0 != json_array_set_new(jobj_p, arr_idx, field_jobj_p))
                        goto FATAL;
                }
                else
                {
                    if (0 != json_array_append_new(jobj_p, field_jobj_p))
                        goto FATAL;
                }
            }
            else
            {
                json_t  *vjobj_p;

                if (arr_idx < json_array_size(jobj_p))
                {
                    vjobj_p = json_array_get(jobj_p, arr_idx);

                    if (json_is_null(vjobj_p))
                    {
                        vjobj_p = json_object();

                        if (0 != json_array_set_new(jobj_p, arr_idx, vjobj_p))
                            goto FATAL;
                    }
                }
                else
                {
                    vjobj_p = json_object();

                    if (0 != json_array_append_new(jobj_p, vjobj_p))
                        goto FATAL;
                }

                jobj_p = vjobj_p;
            }
        }
        else
        {
            if (!json_is_object(jobj_p))
                goto FATAL;

            if (   !rem_path
                || !rem_path[0]
               )
            {
                if (0 != json_object_set_new(jobj_p, name_p, field_jobj_p))
                    goto FATAL;
            }
            else
            {
                json_t  *vjobj_p = json_object_get(jobj_p, name_p);

                if (!vjobj_p)
                {
                    vjobj_p = json_object();

                    if (0 != json_object_set_new(jobj_p, name_p, vjobj_p))
                        goto FATAL;
                }

                jobj_p = vjobj_p;
            }
        }
    }

    free(rem_path);

    return true;

FATAL:
    json_decref(field_jobj_p);

    if (rem_path)
        free(rem_path);

    return false;
}


/*****************************************************************************
   Test Functions
*****************************************************************************/

