/*****************************************************************************
 *
 * FILE NAME     : mac_addr.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : September 29, 2025
 *
 *****************************************************************************
 *
 *  DESCRIPTION : MAC Address
 *
 *****************************************************************************/

#ifndef __MAC_ADDR_H
#define __MAC_ADDR_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
/*****************************************************************************
 * Include Files
 *****************************************************************************/

/*****************************************************************************
 * Global Defines
 *****************************************************************************/
#define mac_addr_equals(a, b)  (   a[0] == b[0] \
                                && a[1] == b[1] \
                                && a[2] == b[2] \
                                && a[3] == b[3] \
                                && a[4] == b[4] \
                                && a[5] == b[5] \
                               )

#define mac_addr_copy(from, to) \
    do                          \
    {                           \
        to[0] = from[0];        \
        to[1] = from[1];        \
        to[2] = from[2];        \
        to[3] = from[3];        \
        to[4] = from[4];        \
        to[5] = from[5];        \
    }                           \
    while (0)


/*****************************************************************************
 * Global Constants
 *****************************************************************************/
#define MAC_ADDR_STRLEN       18


/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef enum
{
    MAC_ADDR_FMT_SEPARATOR_NONE,
    MAC_ADDR_FMT_SEPARATOR_COLON,
    MAC_ADDR_FMT_SEPARATOR_HYPHEN,

    MAC_ADDR_NUM_FMT_SEPARATORS

} mac_addr_fmt_separator_e;

typedef unsigned char mac_addr_t[6];


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
extern const mac_addr_t null_mac_addr;


/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
extern const char* mac_addr_str (mac_addr_t addr, char *buffer_p, mac_addr_fmt_separator_e separator);
extern int mac_addr_parse (const char *saddr_p, mac_addr_t addr);
extern int mac_addr_valid (const char *saddr_p);


#if defined(__cplusplus)
}
#endif

#endif /* __MAC_ADDR_H */
