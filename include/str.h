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
#define string_replace_all(str_p, substr_p, replacement_str_p) \
    string_replace(str_p, substr_p, replacement_str_p, -1)


/*****************************************************************************
 * Global Constants
 *****************************************************************************/
 
/*****************************************************************************
 * Global Types
 *****************************************************************************/
typedef struct string_s
{
    long   capacity;
    long   length;

    char  *s;

} string_t;


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
 *  NAME        : string_new
 *                string_newn
 *                string_newb
 *
 *  DESCRIPTION : Create a new string
 *
 *  PARAMS      : s   - CString or pointer to character array
 *                pos - Start position in string s
 *                n   - Number of characters
 *
 *  RETURNS     : Returns new string.
 *                Return NULL if input was invalid or if malloc failed.
 *
 *****************************************************************************/
#define string_new(s)  string_newb(s, 0, -1)
#define string_newn(s, n)  string_newb(s, 0, n)
extern string_t* string_newb (const char *s, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : string_init
 *                string_initn
 *                string_initb
 *
 *  DESCRIPTION : Initialize a string_t structure
 *
 *  PARAMS      : str_p - String
 *                s     - CString or pointer to character array
 *                pos   - Start position in string s
 *                n     - Number of characters
 *
 *  RETURNS     : Nothing.
 *
 *****************************************************************************/
#define string_init(str_p, s)  string_initb(str_p, s, 0, -1)
#define string_initn(str_p, s, n)  string_initb(str_p, s, 0, n)
extern bool string_initb (string_t *str_p, const char *s, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : string_set
 *                string_setn
 *                string_setb
 *
 *  DESCRIPTION : Set the value of a string
 *
 *  PARAMS      : str_p - String
 *                s     - CString or pointer to character array
 *                pos   - Start position in string s
 *                n     - Number of characters
 *
 *  RETURNS     : Nothing.
 *
 *  NOTES       : Previously allocated memory if any are freed before
 *                allocating memory for new string.
 *
 *****************************************************************************/
#define string_set(str_p, s)  string_setb(str_p, s, 0, -1)
#define string_setn(str_p, s, n)  string_setb(str_p, s, 0, n)
extern bool string_setb (string_t *str_p, const char *s, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : string_delete
 *
 *  DESCRIPTION : Deletes the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns NULL.
 *
 *****************************************************************************/
extern string_t* string_delete (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_reset
 *
 *  DESCRIPTION : Resets the given string. Frees the resources allocated for
                  this string; but doesn't free the string itself.
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
extern void string_reset (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_clone
 *
 *  DESCRIPTION : Clones the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns new string.
 *                Return NULL if input was invalid or if malloc failed.
 *
 *****************************************************************************/
extern string_t* string_clone (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_swap
 *
 *  DESCRIPTION : Swaps the contents of two strings
 *
 *  PARAMS      : str1_p - First string
 *                str2_p - Second string
 *
 *  RETURNS     : Nothing.
 *
 *****************************************************************************/
extern void string_swap (string_t *str1_p, string_t *str2_p);

/*****************************************************************************
 *
 *  NAME        : string_reserve
 *
 *  DESCRIPTION : Reserves memory of given size
 *
 *  PARAMS      : size - Desired capacity
 *
 *  RETURNS     : true if memory was reserved
 *                false otherwise
 *
 *****************************************************************************/
extern bool string_reserve (string_t *str_p, long size);

/*****************************************************************************
 *
 *  NAME        : string_cstr
 *                string_data
 *
 *  DESCRIPTION : Returns the C string of the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns const C string.
 *
 *****************************************************************************/
extern const char* string_cstr (string_t *str_p);
extern       char* string_data (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_at
                  string_first
                  string_last
 *
 *  DESCRIPTION : Get the character at the given position in the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Position in the string
 *
 *  RETURNS     : Character at the given position
 *
 *****************************************************************************/
extern char string_at (string_t *str_p, long pos);
#define string_first(str_p)  string_at(str_p, 0)
#define string_last(str_p)  string_at(str_p, string_length(str_p) - 1)

/*****************************************************************************
 *
 *  NAME        : string_length
 *
 *  DESCRIPTION : Get the length of the given string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Length of the string
 *
 *****************************************************************************/
extern long string_length (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_capacity
 *
 *  DESCRIPTION : Get the capacity of the given string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Capacity of the string
 *
 *****************************************************************************/
extern long string_capacity (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_empty
 *                string_blank
 *
 *  DESCRIPTION : Check if the given string is empty / blank
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string is empty
 *                false otherwise
 *
 *****************************************************************************/
extern bool string_empty (string_t *str_p);
extern bool string_blank (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_clear
 *
 *  DESCRIPTION : Clears the string
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string was cleared
 *                false otherwise
 *
 *****************************************************************************/
#define string_clear(str_p) string_set(str_p, "")

/*****************************************************************************
 *
 *  NAME        : string_tolower
 *
 *  DESCRIPTION : Convert the given string to lowercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in lower case
 *
 *****************************************************************************/
extern void string_tolower (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_toupper
 *
 *  DESCRIPTION : Convert the given string to uppercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : string in upper case
 *
 *****************************************************************************/
extern void string_toupper (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_compare
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
 *                 -1  if     string_length(str1_p) < string_length(str1_p)
 *                 +1  if     string_length(str1_p) > string_length(str1_p)
 *                < 0  if     string_length(str1_p) = string_length(str1_p)
 *                        and ASCII(str1_p) < ASCII(str2_p)
 *                > 0  if     string_length(str1_p) = string_length(str1_p)
 *                        and ASCII(str1_p) > ASCII(str2_p)
 *                = 0  if     string_length(str1_p) = string_length(str1_p)
 *                        and ASCII(str1_p) = ASCII(str2_p)
 *
 * NOTES        : If n is non-zero, then only first n characters of the
 *                strings are compared.
 *                If n < zero, then all characters of the strings are compared.
 *                If icase_b is true, then case-insensitive comparison is done.
 *                If icase_b is false, then case-sensitive comparison is done.
 *
 *****************************************************************************/
extern int string_compare (string_t *str1_p, string_t *str2_p, long n, bool icase_b);

/*****************************************************************************
 *
 *  NAME        : string_starts_with
 *
 *  DESCRIPTION : Check if the given string starts the given prefix
 *
 *  PARAMS      : str_p    - The string
 *                prefix_p - The substring to search for
 *
 *  RETURNS     : true if str_p starts with prefix_p
 *                false otherwise
 *
 *****************************************************************************/
extern bool string_starts_with (string_t *str_p, const char *prefix_p);

/*****************************************************************************
 *
 *  NAME        : string_ends_with
 *
 *  DESCRIPTION : Check if the given string ends the given suffix
 *
 *  PARAMS      : str_p    - The string
 *                suffix_p - The substring to search for
 *
 *  RETURNS     : true if str_p ends with suffix_p
 *                false otherwise
 *
 *****************************************************************************/
extern bool string_ends_with (string_t *str_p, const char *suffix_p);

/*****************************************************************************
 *
 *  NAME        : string_contains
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
#define string_contains(str_p, substr_p)  (-1 != string_find(str_p, 0, substr_p))

/*****************************************************************************
 *
 *  NAME        : string_span
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
extern long string_span (string_t *str_p, const char *accept_p);

/*****************************************************************************
 *
 *  NAME        : string_find
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
extern long string_find (string_t *str_p, long pos, const char *substr_p);

/*****************************************************************************
 *
 *  NAME        : string_rfind
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
extern long string_rfind (string_t *str_p, long last_pos, const char *substr_p);

/*****************************************************************************
 *
 *  NAME        : string_substr
 *
 *  DESCRIPTION : Create a new substring from the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Starting position in the string
 *                n     - Number of characters to extract.
 *
 *  RETURNS     : substring
 *
 *****************************************************************************/
extern string_t* string_substr (string_t *str_p, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : string_append
                  string_appendc
 *                string_appendn
 *
 *  DESCRIPTION : Appends the given string or character to the end of the
                  given string
 *
 *  PARAMS      : str_p - The string to append to
 *                s     - The string to append
 *                n     - Number of characters to append from s
 *
 *  RETURNS     : true if append was successful
 *                false otherwise
 *
 *****************************************************************************/
#define string_append(str_p, s)  string_appendn(str_p, s, -1)
#define string_appendc(str_p, ch) string_appendn(str_p, &ch, 1)
extern bool string_appendn (string_t *str_p, const char *s, long n);

/*****************************************************************************
 *
 *  NAME        : string_insert
 *                string_insertc
 *                string_insertn
 *
 *  DESCRIPTION : Inserts the given string or character at the specified
 *                position in the given string
 *
 *  PARAMS      : str_p - The string to insert into
 *                pos   - Insert position
 *                s     - The string to insert
 *                n     - Number of characters to insert from s
 *
 *  RETURNS     : true if insert was successful
 *                false otherwise
 *
 *****************************************************************************/
#define string_insert(str_p, pos, s)  string_insertn(str_p, pos, s, -1)
#define string_insertc(str_p, pos, ch) string_insertn(str_p, pos, &ch, 1)
extern bool string_insertn (string_t *str_p, long pos, const char *s, long n);

/*****************************************************************************
 *
 *  NAME        : string_replace
 *
 *  DESCRIPTION : Replace the first n occurrences of the given substring in
 *                the given string with the given replacement.
 *
 *  PARAMS      : str_p - The string
 *                ss_p  - The substring to replace
 *                rs_p  - The replacement string
 *                n     - Number of occurrences to replace (-1 means all)
 *
 *  RETURNS     : true if replace was successful.
 *                false otherwise.
 *
 *****************************************************************************/
extern bool string_replace (string_t *str_p, const char *ss_p, const char *rs_p, long n);

/*****************************************************************************
 *
 *  NAME        : string_remove
 *
 *  DESCRIPTION : Removes n characters from the specified position in the
 *                given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Position from which to remove characters
 *                n     - Number of characters to remove
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
extern void string_remove (string_t *str_p, long pos, long n);

/*****************************************************************************
 *
 *  NAME        : string_remove_prefix
 *                string_remove_suffix
 *
 *  DESCRIPTION : Removes the first / last n characters from the given string
 *
 *  PARAMS      : str_p - The string
 *                n     - Number of characters to remove
 *
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
extern void string_remove_prefix (string_t *str_p, long n);
extern void string_remove_suffix (string_t *str_p, long n);

/*****************************************************************************
 *
 *  NAME        : string_trim
 *                string_trim_leading_ws
 *                string_trim_trailing_ws
 *
 *  DESCRIPTION : Trims the given string by removing leading / trailing
 *                whitespaces
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Number of characters removed
 *
 *****************************************************************************/
extern long string_trim             (string_t *str_p);
extern long string_trim_leading_ws  (string_t *str_p);
extern long string_trim_trailing_ws (string_t *str_p);

/*****************************************************************************
 *
 *  NAME        : string_join
 *
 *  DESCRIPTION : Joins the strings in the given array.
 *
 *  PARAMS      : sarray_p - Array of strings to join
 *                sep_p    - Separator string
 *
 *  RETURNS     : Joined string
 *                NULL if sarray_p is NULL or if there is an alloc error
 *
 *****************************************************************************/
extern string_t* string_join (string_t **sarray_p, const char *sep_p);

/*****************************************************************************
 *
 *  NAME        : string_split
 *
 *  DESCRIPTION : Split the given string into an array of string using the
 *                the given separator as delimiter string.
 *
 *  PARAMS      : str_p - The string
 *                sep_p - Delimiter string
 *
 *  RETURNS     : An array of strings.
 *
 *                If sep_p is "", then the given string is split into
 *                constituent characters.
 *
 *                sep_p is a string. Therefore separator is the entire string
 *                in sep_p. For example, if sep_p = ":-",
 *                then "a:b-c:-d" will be split into a:b-c, d.
 *
 *****************************************************************************/
extern string_t** string_split (string_t *str_p, const char *sep_p);

/*****************************************************************************
 *
 *  NAME        : string_ssplit
 *
 *  DESCRIPTION : Split the given string into an array of
 *                strings using the characters as delimiters.
 *
 *  PARAMS      : str_p - The string
 *                sep_p - Set of characters to be used as delimiter
 *
 *  RETURNS     : An array of strings.
 *
 *                If sep_p is "", then the given string is split into
 *                constituent characters and the resulting array is returned.
 *
 *                sep_p is a set of characters. Therefore delimiter is any
 *                of the characters in sep_p. For example, if sep_p = ":-",
 *                then "a:b-c" will be split into a, b, c.
 *
 *****************************************************************************/
extern string_t** string_ssplit (string_t *str_p, const char *sep_p);


/*****************************************************************************
 *
 *  NAME        : string_array_new
 *
 *  DESCRIPTION : Create a fixed size array of strings
 *
 *  PARAMS      : count - Size of array
 *
 *  RETURNS     : Returns array of strings
 *                Return NULL if input was invalid or if calloc failed.
 *
 *****************************************************************************/
extern string_t** string_array_new (long count);

/*****************************************************************************
 *
 *  NAME        : string_array_del
 *
 *  DESCRIPTION : Deletes the given array of strings
 *
 *  PARAMS      : sarray_p - Array of strings
 *
 *  RETURNS     : Returns NULL.
 *
 *****************************************************************************/
extern string_t** string_array_del (string_t **sarray_p);

/*****************************************************************************
 *
 *  NAME        : string_array_clone
 *
 *  DESCRIPTION : Clones the given array of strings
 *
 *  PARAMS      : sarray_p - Array of strings
 *
 *  RETURNS     : Returns new array of strings.
 *
 *****************************************************************************/
extern string_t** string_array_clone (string_t **sarray_p);


#if defined(__cplusplus)
}
#endif

#endif /*__STR_H */
