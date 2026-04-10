/*****************************************************************************
 *
 * FILE NAME     : mac_addr.c
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : September 29, 2025
 *
 ******************************************************************************
 *
 *  DESCRIPTION : MAC address
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include <ctype.h>
#include "mac_addr.h"
#include <stdio.h>
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
static char  *L_mac_addr_fmt_separator[MAC_ADDR_NUM_FMT_SEPARATORS] =
{
    "",
    ":",
    "-"
};


/*****************************************************************************
   Global Variables
*****************************************************************************/
const mac_addr_t null_mac_addr = { 0, 0, 0, 0, 0, 0 };


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
 *  NAME        : mac_addr_str
 *
 *  DESCRIPTION : Formats the given mac address
 *
 *  PARAMS      : addr      - MAC address
 *                buffer_p  - Buffer into which formatted string is placed
 *                separator - Separator to be used between the 6 bytes
 *
 *  RETURNS     : formatted mac address; buffer_p if successful
 *                NULL otherwise
 *
 *  NOTES       : buffer_p must point to a buffer that is large enough.
 *                If separator is specified, atleast 18 bytes are required
 *                to hold formatted mac address and the terminating '\0' byte.
 *                If no separator is specified, atleast 13 bytes are required.
 *
 *****************************************************************************/
const char* mac_addr_str (mac_addr_t addr, char *buffer_p, mac_addr_fmt_separator_e separator)
{
    const char  *ret_p = NULL;

    if (   addr
        && buffer_p
        && separator < MAC_ADDR_NUM_FMT_SEPARATORS
       )
    {
        sprintf(buffer_p, "%02hhx%s%02hhx%s%02hhx%s%02hhx%s%02hhx%s%02hhx", addr[0], L_mac_addr_fmt_separator[separator],
                                                                            addr[1], L_mac_addr_fmt_separator[separator],
                                                                            addr[2], L_mac_addr_fmt_separator[separator],
                                                                            addr[3], L_mac_addr_fmt_separator[separator],
                                                                            addr[4], L_mac_addr_fmt_separator[separator],
                                                                            addr[5]);

        ret_p = buffer_p;
    }

    return ret_p;
}

/*****************************************************************************
 *
 *  NAME        : mac_addr_parse
 *
 *  DESCRIPTION : Parse mac address (with / without separator) into mac_addr_t
 *
 *  PARAMS      : saddr_p - CString; mac address
 *                addr    - MAC address
 *
 *  RETURNS     : 0 if successful
 *                1 otherwise
 *
 *  NOTES       : saddr_p MUST be '\0' terminated.
 *
 *****************************************************************************/
int mac_addr_parse (const char *saddr_p, mac_addr_t addr)
{
    int  ret = 1;

    if (   addr
        && 0 == mac_addr_valid(saddr_p)
       )
    {
        if (   6 == sscanf(saddr_p, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5])
            || 6 == sscanf(saddr_p, "%2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5])
            || 6 == sscanf(saddr_p, "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",      &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5])
           )
        {
            ret = 0;
        }
    }

    return ret;
}

/*****************************************************************************
 *
 *  NAME        : mac_addr_valid
 *
 *  DESCRIPTION : Checks if the given mac address is in valid format.
 *                Valid separators are hyphen or colon or none.
 *
 *  PARAMS      : saddr_p - CString; mac address
 *
 *  RETURNS     : 0 if valid
 *                1 otherwise
 *
 *  NOTES       : saddr_p MUST be '\0' terminated.
 *
 *****************************************************************************/
int mac_addr_valid (const char *saddr_p)
{
    int  ret = 1;

    if (saddr_p)
    {
        size_t  len = strlen(saddr_p),
                ii;

        if (len == 12)
        {
            for (ii = 0; ii < len; ii++)
            {
                if (!isxdigit(saddr_p[ii]))
                    break;
            }

            if (ii == len)
                ret = 0;
        }
        else if (len == 17)
        {
            char  separator = saddr_p[2];

            if (   separator == ':'
                || separator == '-'
               )
            {
                for (ii = 0; ii < len; ii++)
                {
                    if (ii % 3 == 2 && saddr_p[ii] == separator)
                        continue;

                    if (!isxdigit(saddr_p[ii]))
                        break;
                }

                if (ii == len)
                    ret = 0;
            }
        }
    }

    return ret;
}


/*****************************************************************************
   Test Functions
*****************************************************************************/

