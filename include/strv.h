/*****************************************************************************
 *
 * FILE NAME     : stringv.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : May 26, 2026
 *
 *****************************************************************************
 *
 *  DESCRIPTION : String View.
 *                For operations where string size doesn't change and
 *                therefore doesn't need reallocs.
 *
 *                One could of course use the native APIs in string.h. But
 *                they lack NULL and size checks. Also String View carries
 *                string length along with the string; so when length is
 *                required for string operation, it doesn't need to
 *                calculate.
 *
 *****************************************************************************/

#ifndef __stringv_H
#define __stringv_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include <stdbool.h>


/*****************************************************************************
 * Global Defines
 *****************************************************************************/

/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct stringv
{
    char  *s;
    long   slen;

} stringv_t;


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
 
/*****************************************************************************
 * Inline functions
 *****************************************************************************/
 
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : empty_stringv
 *                true_stringv
 *                false_stringv
 *
 *  DESCRIPTION : Useful strings
 *
 *  PARAMS      : void
 *
 *  RETURNS     : Returns string.
 *
 *****************************************************************************/
extern stringv_t* empty_stringv (void);
extern stringv_t* true_stringv  (void);
extern stringv_t* false_stringv (void);

/*****************************************************************************
 *
 *  NAME        : stringv_set
 *                stringv_setn
 *                stringv_setb
 *
 *  DESCRIPTION : Initialize the string view from character array starting at
 *                pos and n characters
 *
 *  PARAMS      : str_p - The string view
 *                s     - The CString
 *                pos   - Start position in s
 *                n     - Length of CString from pos in s
 *
 *  RETURNS     : void
 *
 *  NOTES       : The function is a no-op if input parameters are invalid.
 *                str_p is initialized only if it is safe to do so.
 *
 *****************************************************************************/
#define stringv_set(str_p, s)  stringv_setb(str_p, s, 0, -1)
#define stringv_setn(str_p, s, n)  stringv_setb(str_p, s, 0, n)
extern void stringv_setb (stringv_t *str_p, char *s, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : stringv_get
 *
 *  DESCRIPTION : Get the character array from the string view
 *
 *  PARAMS      : str_p - The string view
 *
 *  RETURNS     : Pointer to the character array
 *                NULL if str_p was invalid
 *
 *  NOTES       : stringv_t may be pointing to a character array that has more
 *                that stringv_t.slen characters. So, use this function carefully.
 *                If you are unsure, best way is to use stringv_copy to copy
 *                the string out to a buffer.
 *
 *****************************************************************************/
extern char* stringv_get (stringv_t *str_p);

/*****************************************************************************
 *
 *  NAME        : stringv_copy
 *
 *  DESCRIPTION : Copies the string to supplied buffer
 *
 *  PARAMS      : str_p       - String
 *                buffer_p    - Pointer to character array buffer
 *                buffer_size - Size of buffer_p
 *
 *  RETURNS     : buffer_p
 *
 *****************************************************************************/
extern char* stringv_copy (stringv_t *str_p, char *buffer_p, long buffer_size);

/*****************************************************************************
 *
 *  NAME        : stringv_at
 *
 *  DESCRIPTION : Get the character at the given position in the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Position in the string
 *
 *  RETURNS     : Character at the given position
 *
 *****************************************************************************/
extern char stringv_at (stringv_t *str_p, long pos);
#define stringv_first(str_p)  stringv_at(str_p, 0)
#define stringv_last(str_p)  stringv_at(str_p, stringv_length(str_p) - 1)

/*****************************************************************************
 *
 *  NAME        : stringv_length
 *
 *  DESCRIPTION : Get length of the given string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Length of given string
 *
 *****************************************************************************/
extern long stringv_length (stringv_t *str_p);

/*****************************************************************************
 *
 *  NAME        : stringv_empty
 *                stringv_blank
 *
 *  DESCRIPTION : Check if the given string is empty / blank
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string is empty
 *                false otherwise
 *
 *****************************************************************************/
extern bool stringv_empty (stringv_t *str_p);
extern bool stringv_blank (stringv_t *str_p);

/*****************************************************************************
 *
 *  NAME        : stringv_clear
 *
 *  DESCRIPTION : Clears the string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string was cleared
 *                false otherwise
 *
 *****************************************************************************/
#define stringv_clear(str_p) stringv_set(str_p, "")

/*****************************************************************************
 *
 *  NAME        : stringv_tolower
 *
 *  DESCRIPTION : Convert the given string to lowercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in lower case
 *
 *****************************************************************************/
extern void stringv_tolower (stringv_t *str_p);

/*****************************************************************************
 *
 *  NAME        : stringv_toupper
 *
 *  DESCRIPTION : Convert the given string to uppercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in upper case
 *
 *****************************************************************************/
extern void stringv_toupper (stringv_t *str_p);

/*****************************************************************************
 *
 *  NAME        : stringv_compare
 *
 *  DESCRIPTION : Compares two strings
 *
 *  PARAMS      : str1_p  - The first string
 *                str2_p  - The second string
 *                n       - Number of characters to compare.
 *                icase_b - case-insensitive comparison
 *
 *  RETURNS     :  -1  if     str1_p  = NULL
 *                        and str2_p != NULL
 *                 +1  if     str1_p != NULL
 *                        and str2_p  = NULL
 *                  0  if     str1_p  = NULL
 *                        and str2_p  = NULL
 *                 +1  if     stringv_length(str1_p) > stringv_length(str2_p)
 *                 -1  if     stringv_length(str1_p) < stringv_length(str2_p)
 *                 +1  if     stringv_length(str1_p) > stringv_length(str2_p)
 *                < 0  if     stringv_length(str1_p) = stringv_length(str2_p)
 *                        and ASCII(str1_p) < ASCII(str2_p)
 *                > 0  if     stringv_length(str1_p) = stringv_length(str2_p)
 *                        and ASCII(str1_p) > ASCII(str2_p)
 *                = 0  if     stringv_length(str1_p) = stringv_length(str2_p)
 *                        and ASCII(str1_p) = ASCII(str2_p)
 *
 * NOTES        : Comparison is done first based on length and then based on
 *                ASCII values of characters.
 *                If n is non-zero, then only first n characters of the
 *                strings are compared.
 *                If n is zero, then all characters of the strings are compared.
 *                If icase_b is true, then case-insensitive comparison is done.
 *                If icase_b is false, then case-sensitive comparison is done.
 *
 *****************************************************************************/
extern int stringv_compare (stringv_t *str1_p, stringv_t *str2_p, long n, bool icase_b);

/*****************************************************************************
 *
 *  NAME        : stringv_starts_with
 *
 *  DESCRIPTION : Check if the given string starts the given prefix
 *
 *  PARAMS      : str_p    - The string
 *                substr_p - The substring to search for
 *
 *  RETURNS     : true if str_p starts with prefix_p
 *                false otherwise
 *
 *****************************************************************************/
extern bool stringv_starts_with (stringv_t *str_p, const char *prefix_p);

/*****************************************************************************
 *
 *  NAME        : stringv_ends_with
 *
 *  DESCRIPTION : Check if the given string ends the given suffix
 *
 *  PARAMS      : str_p    - The string
 *                substr_p - The substring to search for
 *
 *  RETURNS     : true if str_p ends with suffix_p
 *                false otherwise
 *
 *****************************************************************************/
extern bool stringv_ends_with (stringv_t *str_p, const char *prefix_p);

/*****************************************************************************
 *
 *  NAME        : stringv_contains
 *
 *  DESCRIPTION : Check if the given string contains the given substring
 *
 *  PARAMS      : str_p    - The string
 *                substr_p - The substring to search for
 *
 *  RETURNS     : true if str_p contains substr_p
 *                false otherwise
 *
 *****************************************************************************/
#define stringv_contains(str_p, substr_p)  (-1 != stringv_find(str_p, 0, substr_p))

/*****************************************************************************
 *
 *  NAME        : stringv_span
 *
 *  DESCRIPTION : Calculate the length of the initial segment of the given
 *                string str_p which consists entirely of characters in the
 *                given accept string
 *
 *  PARAMS      : str_p    - The string
 *                accept_p - The string containing acceptable characters
 *
 *  RETURNS     : Length of the initial segment consisting of acceptable
 *                characters
 *
 *****************************************************************************/
extern long stringv_span (stringv_t *str_p, const char *accept_p);

/*****************************************************************************
 *
 *  NAME        : stringv_find
 *                stringv_find_first
 *
 *  DESCRIPTION : Find the first occurence of the given substring in the
 *                given string
 *
 *  PARAMS      : str_p    - The string
 *                pos      - Starting position in the string to search from   
 *                substr_p - The substring to search for
 *
 *  RETURNS     : Position of the substring if found, -1 otherwise
 *
 *****************************************************************************/
extern long stringv_find (stringv_t *str_p, long pos, const char *substr_p);
#define stringv_find_first(str_p, substr_p)  stringv_find(str_p, 0, substr_p)

/*****************************************************************************
 *
 *  NAME        : stringv_rfind
 *                stringv_find_last
 *
 *  DESCRIPTION : Find the last occurence of the given substring in the
 *                given string (reverse scan)
 *
 *  PARAMS      : str_p    - The string
 *                last_pos - Position in the string to search backwards from
 *                substr_p - The substring to search for
 *
 *  RETURNS     : Position of the substring if found, -1 otherwise
 *
 *****************************************************************************/
extern long stringv_rfind (stringv_t *str_p, long last_pos, const char *substr_p);
#define stringv_find_last(str_p, substr_p)  stringv_rfind(str_p, stringv_length(str_p) - 1, substr_p)

/*****************************************************************************
 *
 *  NAME        : stringv_substr
 *
 *  DESCRIPTION : Extract substring from the given string
 *
 *  PARAMS      : in_p  - The input string view
 *                pos   - Starting position in the input string
 *                n     - Number of characters to extract.
 *                out_p - The output string view
 *
 *  RETURNS     : Nothing
 * 
 *  NOTES       : No-Op if arguments are invalid
 *                      if pos is out of bounds
 *                empty_str if n = 0
 *                if n < 0 or pos + n > stringv_length(in_p), all characters
 *                from pos in str_p will be contained in out_p.
 *                otherwise, out_p will start at in_p->s + pos and will have
 *                string length of n.
 *
 *****************************************************************************/
extern void stringv_substr (stringv_t *in_p, long pos, long n, stringv_t *out_p);

/*****************************************************************************
 *
 *  NAME        : stringv_remove_prefix
 *                stringv_remove_suffix
 *
 *  DESCRIPTION : Removes the first / last n characters from the given string
 *
 *  PARAMS      : str_p - The string
 *                n     - Number of characters to remove
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : No-OP if inputs are invalid or if n > stringv_length(str_p)
 *                If n >= stringv_length(str_p), stringv_remove_suffix will retain
 *                str_p->s at current position but set n to 0 whereas
 *                stringv_remove_prefix will move str_p->s to str_p->s + n and
 *                set n to 0.
 *
 *****************************************************************************/
extern void stringv_remove_prefix (stringv_t *str_p, long n);
extern void stringv_remove_suffix (stringv_t *str_p, long n);

/*****************************************************************************
 *
 *  NAME        : stringv_trim
 *                stringv_trim_leading_ws
 *                stringv_trim_trailing_ws
 *
 *  DESCRIPTION : Trims the given string by removing leading / trailing
 *                whitespaces
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
extern void stringv_trim (stringv_t *str_p);
extern void stringv_trim_leading_ws (stringv_t *str_p);
extern void stringv_trim_trailing_ws (stringv_t *str_p);


#if defined(__cplusplus)
}
#endif

#endif /*__stringv_H */