/*****************************************************************************
 *
 * FILE NAME     : str.h
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : April 29, 2026
 *
 *****************************************************************************
 *
 *  DESCRIPTION : String
 *
 *****************************************************************************/

#ifndef __STR_H
#define __STR_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files
 *****************************************************************************/
#include <stdbool.h>
#include <stddef.h>


/*****************************************************************************
 * Global Defines
 *****************************************************************************/

/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct str_s str_t;


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
 *  NAME        : str_new
 *
 *  DESCRIPTION : Create a new string
 *
 *  PARAMS      : s - CString
 *
 *  RETURNS     : Returns new string.
 *                Returns empty_string() if s is "".
 *                Return NULL if input was invalid or if malloc failed.
 *
 *****************************************************************************/
extern str_t* str_new (const char *s);

/*****************************************************************************
 *
 *  NAME        : str_del
 *
 *  DESCRIPTION : Deletes the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns NULL.
 *
 *****************************************************************************/
extern str_t* str_del (str_t *str_p);

/*****************************************************************************
 *
 *  NAME        : str_clone
 *
 *  DESCRIPTION : Clones the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns new string.
 *                Returns empty_string() if input was also empty_string().
 *                Return NULL if input was invalid or if malloc failed.
 *
 *****************************************************************************/
extern str_t* str_clone (str_t *str_p);

/*****************************************************************************
 *
 *  NAME        : str_array_new
 *
 *  DESCRIPTION : Create a fixed size array of strings
 *
 *  PARAMS      : count - Size of array
 *
 *  RETURNS     : Returns array of strings
 *                Return NULL if input was invalid or if calloc failed.
 *
 *****************************************************************************/
extern str_t** str_array_new (long count);

/*****************************************************************************
 *
 *  NAME        : str_array_del
 *
 *  DESCRIPTION : Deletes the given array of strings
 *
 *  PARAMS      : sarray_p - Array of strings
 *
 *  RETURNS     : Returns NULL.
 *
 *****************************************************************************/
extern str_t** str_array_del (str_t **sarray_p);

/*****************************************************************************
 *
 *  NAME        : str_array_clone
 *
 *  DESCRIPTION : Clones the given array of strings
 *
 *  PARAMS      : sarray_p - Array of strings
 *
 *  RETURNS     : Returns new array of strings.
 *
 *****************************************************************************/
extern str_t** str_array_clone (str_t **sarray_p);

/*****************************************************************************
 *
 *  NAME        : empty_string
 *
 *  DESCRIPTION : Returns string initialized to ""
 *
 *  PARAMS      : void
 *
 *  RETURNS     : Always valid and returns the same pointer.
 *
 *****************************************************************************/
extern str_t* empty_str (void);

/*****************************************************************************
 *
 *  NAME        : str_length
 *
 *  DESCRIPTION : Get length of the given string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Length of given string
 *
 *****************************************************************************/
extern long str_length (str_t *str_p);

/*****************************************************************************
 *
 *  NAME        : str_tolower
 *
 *  DESCRIPTION : Convert the given string to lowercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in lower case
 *
 *****************************************************************************/
extern str_t* str_tolower (str_t *str_p);

/*****************************************************************************
 *
 *  NAME        : str_toupper
 *
 *  DESCRIPTION : Convert the given string to uppercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in upper case
 *
 *****************************************************************************/
extern str_t* str_toupper (str_t *str_p);

/*****************************************************************************
 *
 *  NAME        : str_compare
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
 *                 +1  if     str_length(str1_p) > str_length(str1_p)
 *                 -1  if     str_length(str1_p) < str_length(str1_p)
 *                 +1  if     str_length(str1_p) > str_length(str1_p)
 *                < 0  if     str_length(str1_p) = str_length(str1_p)
 *                        and ASCII(str1_p) < ASCII(str2_p)
 *                > 0  if     str_length(str1_p) = str_length(str1_p)
 *                        and ASCII(str1_p) > ASCII(str2_p)
 *                = 0  if     str_length(str1_p) = str_length(str1_p)
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
extern int str_compare (str_t *str1_p, str_t *str2_p, long n, bool icase_b);

/*****************************************************************************
 *
 *  NAME        : str_contains
 *
 *  DESCRIPTION : Check if the given string contains the given substring
 *
 *  PARAMS      : str_p    - The string
 *                pos      - Starting position in the string to search from   
 *                substr_p - The substring to search for
 *
 *  RETURNS     : Position of the substring if found, -1 otherwise
 *
 *****************************************************************************/
extern long str_contains (str_t *str_p, long pos, const char *substr_p);

/*****************************************************************************
 *
 *  NAME        : str_substr
 *
 *  DESCRIPTION : Create a new substring from the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Starting position in the string
 *                n     - Number of characters to extract.
 *
 *  RETURNS     : substring
 *                NULL      if str_p is NULL or if there is a malloc failure or
 *                          if pos is out of bounds
 *                empty_str if n = 0
 *                if n < 0 or n > pos + str_length(str_p), all characters
 *                from pos in str_p will be contained in returned substring.
 *
 *****************************************************************************/
extern str_t* str_substr (str_t *str_p, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : str_join
 *
 *  DESCRIPTION : Joins two strings
 *
 *  PARAMS      : str1_p - The first string
 *                str2_p - The second string
 *
 *  RETURNS     : Joined string
 *
 *                NULL          - if str1_p and str2_p are NULL
 *                Other's clone - if one of str1_p and str2_p is NULL
 *
 *****************************************************************************/
extern str_t* str_join (str_t **sarray_p, const char *sep_p);

/*****************************************************************************
 *
 *  NAME        : str_split
 *
 *  DESCRIPTION : Split the given string into an array of string using the
 *                the given separator as delimiter string.
 *
 *  PARAMS      : str_p - The string
 *                sep_p - Delimiter string
 *
 *  RETURNS     : An array of strings.
 *
 *                NULL               - if str_p or sep_p is NULL or if
 *                                     there is an alloc error.
 *                empty array        - if str_p = empty_string()
 *                sarray_p[0] = NULL
 *
 *                If sep_p is "", then the given string is split into
 *                constituent characters.
 *
 *                sep_p is a string. Therefore separator is the entire string
 *                in sep_p. For example, if sep_p = ":-",
 *                then "a:b-c:-d" will be split into a:b-c, d.
 *
 *****************************************************************************/
extern str_t** str_split (str_t *str_p, const char *sep_p);

/*****************************************************************************
 *
 *  NAME        : str_ssplit
 *
 *  DESCRIPTION : Split the given string into an array of
 *                strings using the characters as delimiters.
 *
 *  PARAMS      : str_p - The string
 *                sep_p - Set of characters to be used as delimiter
 *
 *  RETURNS     : An array of strings.
 *
 *                NULL               - if str_p or sep_p is NULL or if
 *                                     there is an alloc error.
 *                empty array        - if str_p = empty_string()
 *                sarray_p[0] = NULL
 *
 *                If sep_set_p is "", then the given string is split into
 *                constituent characters.
 *
 *                sep_p is a set of characters. Therefore separator is any
 *                of the characters in sep_p. For example, if sep_p = ":-",
 *                then "a:b-c" will be split into a, b, c.
 *
 *****************************************************************************/
extern str_t** str_ssplit (str_t *str_p, const char *sep_p);


#if defined(__cplusplus)
}
#endif

#endif /* __STR_H */
