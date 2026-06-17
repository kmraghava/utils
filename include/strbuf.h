/*****************************************************************************
 *
 * FILE NAME     : strbuf.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : May 26, 2026
 *
 *****************************************************************************
 *
 *  DESCRIPTION : String Buffer.
 *
 *****************************************************************************/

#ifndef __STRBUF_H
#define __STRBUF_H

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
typedef struct strbuf
{
    char  *s;
    long   slen;

} strbuf_t;


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
 *  NAME        : empty_strbuf
 *                true_strbuf
 *                false_strbuf
 *
 *  DESCRIPTION : Useful strings
 *
 *  PARAMS      : void
 *
 *  RETURNS     : Returns string.
 *
 *****************************************************************************/
extern strbuf_t* empty_strbuf (void);
extern strbuf_t* true_strbuf  (void);
extern strbuf_t* false_strbuf (void);

/*****************************************************************************
 *
 *  NAME        : strbuf_set
 *                strbuf_setn
 *                strbuf_setb
 *
 *  DESCRIPTION : Initialize the string buffer from character array starting at
 *                pos and n characters
 *
 *  PARAMS      : str_p - The string buffer
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
#define strbuf_set(str_p, s)  strbuf_setb(str_p, s, 0, -1)
#define strbuf_setn(str_p, s, n)  strbuf_setb(str_p, s, 0, n)
extern void strbuf_setb (strbuf_t *str_p, char *s, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : strbuf_get
 *
 *  DESCRIPTION : Get the character array from the string buffer
 *
 *  PARAMS      : str_p - The string buffer
 *
 *  RETURNS     : Pointer to the character array
 *                NULL if str_p was invalid
 *
 *  NOTES       : strbuf_t may be pointing to a character array that has more
 *                that strbuf_t.slen characters. So, use this function carefully.
 *                If you are unsure, best way is to use strbuf_copy to copy
 *                the string out to a buffer.
 *
 *****************************************************************************/
extern char* strbuf_get (strbuf_t *str_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_copy
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
extern char* strbuf_copy (strbuf_t *str_p, char *buffer_p, long buffer_size);

/*****************************************************************************
 *
 *  NAME        : strbuf_at
 *
 *  DESCRIPTION : Get the character at the given position in the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Position in the string
 *
 *  RETURNS     : Character at the given position
 *
 *****************************************************************************/
extern char strbuf_at (strbuf_t *str_p, long pos);
#define strbuf_first(str_p)  strbuf_at(str_p, 0)
#define strbuf_last(str_p)  strbuf_at(str_p, strbuf_length(str_p) - 1)

/*****************************************************************************
 *
 *  NAME        : strbuf_length
 *
 *  DESCRIPTION : Get length of the given string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Length of given string
 *
 *****************************************************************************/
extern long strbuf_length (strbuf_t *str_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_empty
 *                strbuf_blank
 *
 *  DESCRIPTION : Check if the given string is empty / blank
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string is empty
 *                false otherwise
 *
 *****************************************************************************/
extern bool strbuf_empty (strbuf_t *str_p);
extern bool strbuf_blank (strbuf_t *str_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_clear
 *
 *  DESCRIPTION : Clears the string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string was cleared
 *                false otherwise
 *
 *****************************************************************************/
#define strbuf_clear(str_p) strbuf_set(str_p, "")

/*****************************************************************************
 *
 *  NAME        : strbuf_tolower
 *
 *  DESCRIPTION : Convert the given string to lowercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in lower case
 *
 *****************************************************************************/
extern void strbuf_tolower (strbuf_t *str_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_toupper
 *
 *  DESCRIPTION : Convert the given string to uppercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in upper case
 *
 *****************************************************************************/
extern void strbuf_toupper (strbuf_t *str_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_compare
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
 *                 +1  if     strbuf_length(str1_p) > strbuf_length(str2_p)
 *                 -1  if     strbuf_length(str1_p) < strbuf_length(str2_p)
 *                 +1  if     strbuf_length(str1_p) > strbuf_length(str2_p)
 *                < 0  if     strbuf_length(str1_p) = strbuf_length(str2_p)
 *                        and ASCII(str1_p) < ASCII(str2_p)
 *                > 0  if     strbuf_length(str1_p) = strbuf_length(str2_p)
 *                        and ASCII(str1_p) > ASCII(str2_p)
 *                = 0  if     strbuf_length(str1_p) = strbuf_length(str2_p)
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
extern int strbuf_compare (strbuf_t *str1_p, strbuf_t *str2_p, long n, bool icase_b);

/*****************************************************************************
 *
 *  NAME        : strbuf_starts_with
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
extern bool strbuf_starts_with (strbuf_t *str_p, const char *prefix_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_ends_with
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
extern bool strbuf_ends_with (strbuf_t *str_p, const char *prefix_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_contains
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
#define strbuf_contains(str_p, substr_p)  (-1 != strbuf_find(str_p, 0, substr_p))

/*****************************************************************************
 *
 *  NAME        : strbuf_span
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
extern long strbuf_span (strbuf_t *str_p, const char *accept_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_cspan
 *
 *  DESCRIPTION : Calculate the length of the initial segment of the given
 *                string str_p which does not contain characters of the
 *                given reject string
 *
 *  PARAMS      : str_p    - The string
 *                reject_p - The string containing rejectable characters
 *
 *  RETURNS     : Length of the initial segment not containing reject
 *                characters
 *
 *****************************************************************************/
extern long strbuf_cspan (strbuf_t *str_p, const char *reject_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_find
 *                strbuf_find_first
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
extern long strbuf_find (strbuf_t *str_p, long pos, const char *substr_p);
#define strbuf_find_first(str_p, substr_p)  strbuf_find(str_p, 0, substr_p)

/*****************************************************************************
 *
 *  NAME        : strbuf_rfind
 *                strbuf_find_last
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
extern long strbuf_rfind (strbuf_t *str_p, long last_pos, const char *substr_p);
#define strbuf_find_last(str_p, substr_p)  strbuf_rfind(str_p, strbuf_length(str_p) - 1, substr_p)

/*****************************************************************************
 *
 *  NAME        : strbuf_substr
 *
 *  DESCRIPTION : Extract substring from the given string
 *
 *  PARAMS      : in_p  - The input string buffer
 *                pos   - Starting position in the input string
 *                n     - Number of characters to extract.
 *                out_p - The output string buffer
 *
 *  RETURNS     : Nothing
 * 
 *  NOTES       : No-Op if arguments are invalid
 *                      if pos is out of bounds
 *                empty_str if n = 0
 *                if n < 0 or pos + n > strbuf_length(in_p), all characters
 *                from pos in str_p will be contained in out_p.
 *                otherwise, out_p will start at in_p->s + pos and will have
 *                string length of n.
 *
 *****************************************************************************/
extern void strbuf_substr (strbuf_t *in_p, long pos, long n, strbuf_t *out_p);

/*****************************************************************************
 *
 *  NAME        : strbuf_remove_prefix
 *                strbuf_remove_suffix
 *
 *  DESCRIPTION : Removes the first / last n characters from the given string
 *
 *  PARAMS      : str_p - The string
 *                n     - Number of characters to remove
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : No-OP if inputs are invalid or if n > strbuf_length(str_p)
 *                If n >= strbuf_length(str_p), strbuf_remove_suffix will retain
 *                str_p->s at current position but set n to 0 whereas
 *                strbuf_remove_prefix will move str_p->s to str_p->s + n and
 *                set n to 0.
 *
 *****************************************************************************/
extern void strbuf_remove_prefix (strbuf_t *str_p, long n);
extern void strbuf_remove_suffix (strbuf_t *str_p, long n);

/*****************************************************************************
 *
 *  NAME        : strbuf_trim
 *                strbuf_trim_leading_ws
 *                strbuf_trim_trailing_ws
 *
 *  DESCRIPTION : Trims the given string by removing leading / trailing
 *                whitespaces
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Number of characters removed
 *
 *****************************************************************************/
extern long strbuf_trim (strbuf_t *str_p);
extern long strbuf_trim_leading_ws (strbuf_t *str_p);
extern long strbuf_trim_trailing_ws (strbuf_t *str_p);


#if defined(__cplusplus)
}
#endif

#endif /*__STRBUF_H */