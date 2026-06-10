/*****************************************************************************
 *
 * FILE NAME     : str.c
 * MODULE        : utils
 * AUTHOR        : KM Raghava
 * CREATION DATE : April 29, 2026
 *
 *****************************************************************************
 *
 *  DESCRIPTION : String
 *
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include <stdlib.h>
#include "str.h"
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
static bool string_alloc_set_s (string_t *str_p, const char *s, long n);
static void string_set_s (string_t *str_p, const char *s, long n);

static char char_tolower (char ch);
static char char_toupper (char ch);


/*****************************************************************************
   Local Functions
*****************************************************************************/
static bool string_alloc_set_s (string_t *str_p, const char *s, long n)
{
    bool  set_b = false;

    str_p->s = malloc(n + 1);

    if (str_p->s)
    {
        str_p->capacity = n + 1;
        string_set_s(str_p, s, n);

        set_b = true;
    }

    return set_b;
}

static void string_set_s (string_t *str_p, const char *s, long n)
{
    str_p->length = n;

    strncpy(str_p->s, s, n);
    str_p->s[n] = '\0';
}

static char char_tolower (char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A' + 'a';

    return ch;
}

static char char_toupper (char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return ch - 'a' + 'A';
        
    return ch;
}


/*****************************************************************************
   Global Functions
*****************************************************************************/
/*****************************************************************************
 *
 *  NAME        : string_newb
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
string_t* string_newb (const char *s, long pos, long n)
{
    string_t  *str_p = malloc(sizeof(*str_p));

    if (str_p)
    {
        if (!string_initb(str_p, s, pos, n))
        {
            free(str_p);
            str_p = NULL;
        }
    }

    return str_p;
}

/*****************************************************************************
 *
 *  NAME        : string_initb
 *
 *  DESCRIPTION : Initialize a string_t structure
 *
 *  PARAMS      : s   - CString or pointer to character array
 *                pos - Start position in string s
 *                n   - Number of characters
 *
 *  RETURNS     : Nothing.
 *
 *****************************************************************************/
bool string_initb (string_t *str_p, const char *s, long pos, long n)
{
    if (!str_p)
        return false;

    if (!s)
    {
        str_p->s = NULL;
        str_p->capacity = 0;
        str_p->length = 0;

        return true;
    }

    if (pos < 0)
        return false;

    if (n < 0)
    {
        n = strlen(s);
        n -= pos;

        if (n < 0)
            return false;
    }

    if (n == 0)
    {
        str_p->s = "";
        str_p->capacity = 0;
        str_p->length = 0;

        return true;
    }
    
    return string_alloc_set_s(str_p, s + pos, n);
}

/*****************************************************************************
 *
 *  NAME        : string_setb
 *
 *  DESCRIPTION : Set the value of a string
 *
 *  PARAMS      : s   - CString or pointer to character array
 *                pos - Start position in string s
 *                n   - Number of characters
 *
 *  RETURNS     : Nothing.
 *
 *  NOTES       : Previously allocated memory if any are freed before
 *                allocating memory for new string.
 *
 *****************************************************************************/
bool string_setb (string_t *str_p, const char *s, long pos, long n)
{
    if (!str_p || !s)
        return false;

    if (pos < 0)
        return false;

    if (n < 0)
    {
        n = strlen(s);
        n -= pos;

        if (n < 0)
            return false;
    }

    if (n == 0)
    {
        if (str_p->capacity > 0)
        {
            str_p->length = 0;
            str_p->s[0] = '\0';
        }
        else
            str_p->s = "";

        return true;
    }

    if (n < str_p->capacity)
    {
        str_p->length = n;

        strncpy(str_p->s, s + pos, n);
        str_p->s[n] = '\0';

        return true;
    }

    if (str_p->capacity > 0)
    {
        free(str_p->s);

        str_p->capacity = 0;
        str_p->length = 0;
        str_p->s = NULL;
    }

    return string_alloc_set_s(str_p, s + pos, n);
}

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
string_t* string_delete (string_t *str_p)
{
    if (str_p)
    {
        if (str_p->capacity > 0)
            free(str_p->s);
        free(str_p);
    }

    return NULL;
}

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
void string_reset (string_t *str_p)
{
    if (str_p)
    {
        if (str_p->capacity > 0)
        {
            free(str_p->s);
            str_p->s = NULL;
        }
        str_p->capacity = 0;
        str_p->length = 0;
    }
}

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
string_t* string_clone (string_t *str_p)
{
    string_t  *nstr_p = NULL;

    if (str_p)
        nstr_p = string_newb(str_p->s, 0, str_p->length);

    return nstr_p;
}

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
void string_swap (string_t *str1_p, string_t *str2_p)
{
    if (str1_p && str2_p)
    {
        char  *tp;
        long   tl, tc;

        tp = str1_p->s;
        tl = str1_p->length;
        tc = str1_p->capacity;

        str1_p->s = str2_p->s;
        str1_p->length = str2_p->length;
        str1_p->capacity = str2_p->capacity;

        str2_p->s = tp;
        str2_p->length = tl;
        str2_p->capacity = tc;
    }
}

/*****************************************************************************
 *
 *  NAME        : string_reserve
 *
 *  DESCRIPTION : Reserves memory of given size
 *
 *  PARAMS      : size - Desired capacity
 *
 *  RETURNS     : true if memory was reserved
 *                false otherwise.
 *
 *****************************************************************************/
bool string_reserve (string_t *str_p, long size)
{
    if (!str_p)
        return false;

    if (size > str_p->capacity)
    {
        char *new_s = realloc(str_p->s, size + 1);
        if (!new_s)
            return false;

        str_p->s = new_s;
        str_p->capacity = size + 1;
    }

    return true;
}

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
const char* string_cstr (string_t *str_p)
{
    return str_p? str_p->s : NULL;
}
char* string_data (string_t *str_p)
{
    return str_p? str_p->s : NULL;
}

/*****************************************************************************
 *
 *  NAME        : string_at
 *
 *  DESCRIPTION : Get the character at the given position in the given string
 *
 *  PARAMS      : str_p - The string
 *                pos   - Position in the string
 *
 *  RETURNS     : Character at the given position
 *
 *****************************************************************************/
char string_at (string_t *str_p, long pos)
{
    if (!str_p || !str_p->s || pos < 0 || pos > str_p->length)
        return '\0';

    return str_p->s[pos];
}
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
long string_length (string_t *str_p)
{
    return str_p ? str_p->length : 0;
}

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
long string_capacity (string_t *str_p)
{
    return str_p ? str_p->capacity : 0;
}

/*****************************************************************************
 *
 *  NAME        : string_empty
 *
 *  DESCRIPTION : Check if the given string is empty
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string is empty
 *                false otherwise
 *
 *****************************************************************************/
bool string_empty (string_t *str_p)
{
    return str_p ? str_p->length == 0 : true;
}

/*****************************************************************************
 *
 *  NAME        : string_tolower
 *
 *  DESCRIPTION : Convert the given string to lowercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : No-Op if inputs were invalid.
 *
 *****************************************************************************/
void string_tolower (string_t *str_p)
{
    if (!str_p)
        return;

    for (long i = 0; i < str_p->length; i++)
        str_p->s[i] = char_tolower(str_p->s[i]);
}

/*****************************************************************************
 *
 *  NAME        : string_toupper
 *
 *  DESCRIPTION : Convert the given string to uppercase
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : Nothing
 *
 *  NOTES       : No-Op if inputs were invalid.
 *
 *****************************************************************************/
void string_toupper (string_t *str_p)
{
    if (!str_p)
        return;

    for (long i = 0; i < str_p->length; i++)
        str_p->s[i] = char_toupper(str_p->s[i]);
}

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
 *                If n is zero, then all characters of the strings are compared.
 *                If icase_b is true, then case-insensitive comparison is done.
 *                If icase_b is false, then case-sensitive comparison is done.
 *
 *****************************************************************************/
int string_compare (string_t *str1_p, string_t *str2_p, long n, bool icase_b)
{
    if (str1_p && str1_p->s)
    {
        if (str2_p && str2_p->s)
        {
            long  n1 = n,
                  n2 = n;

            if (n == 0)
                return 0;

            if (n1 < 0 || n1 > str1_p->length)
                n1 = str1_p->length;

            if (n2 < 0 || n2 > str2_p->length)
                n2 = str2_p->length;

            if (n1 < n2)
                return -1;
            else if (n1 > n2)
                return 1;
            else
            {
                int  cmp_result = 0;

                for (long i = 0; i < n1 && cmp_result == 0; i++)
                {
                    cmp_result =   icase_b
                                 ? char_tolower(str1_p->s[i]) - char_tolower(str2_p->s[i])
                                 : str1_p->s[i] - str2_p->s[i];
                }

                return cmp_result;
            }
        }
        else
            return 1;
    }
    else
    {
        if (str2_p && str2_p->s)
            return -1;

        return 0;
    }
}

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
bool string_starts_with (string_t *str_p, const char *prefix_p)
{
    if (!str_p || !str_p->s)
        return false;
    
    if (!prefix_p)
        return false;

    long  prefix_len = strlen(prefix_p);

    if (prefix_len > str_p->length)
        return false;

    /* All strings start with empty prefix */
    if (prefix_len == 0)
        return true;

    return 0 == strncmp(str_p->s, prefix_p, prefix_len);
}

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
 bool string_ends_with (string_t *str_p, const char *suffix_p)
{
    if (!str_p || !str_p->s)
        return false;
    
    if (!suffix_p)
        return false;

    long  suffix_len = strlen(suffix_p);

    if (suffix_len > str_p->length)
        return false;

    /* All strings start with empty prefix */
    if (suffix_len == 0)
        return true;

    return 0 == strncmp(str_p->s + (str_p->length - suffix_len), suffix_p, suffix_len);
}

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
long string_find (string_t *str_p, long pos, const char *substr_p)
{
    if (!str_p || !str_p->s)
        return -1;

    if (pos < 0 || pos > str_p->length)
        return -1;

    if (!substr_p)
        return -1;

    long  substring_len = strlen(substr_p);

    if (substring_len == 0)
        return pos;

    while (pos + substring_len <= str_p->length)
    {
        if (0 == strncmp(str_p->s + pos, substr_p, substring_len))
            return pos;

        pos++;
    }

    return -1;
}

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
long string_rfind (string_t *str_p, long last_pos, const char *substr_p)
{
    if (!str_p || !str_p->s)
        return -1;

    if (last_pos < 0 || last_pos > str_p->length)
        return -1;

    if (!substr_p)
        return -1;

    long  substring_len = strlen(substr_p);

    if (substring_len == 0)
        return last_pos;

    while (last_pos - substring_len >= 0)
    {
        if (0 == strncmp(str_p->s + (last_pos - substring_len), substr_p, substring_len))
            return last_pos - substring_len;
        last_pos--;
    }

    return -1;
}

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
string_t* string_substr (string_t *str_p, long pos, long n)
{
    return str_p ? string_newb(str_p->s, pos, n) : NULL;
}

/*****************************************************************************
 *
 *  NAME        : string_appendn
 *
 *  DESCRIPTION : Appends the given string to the end of the given string
 *
 *  PARAMS      : str_p - The string to append to
 *                s     - The string to append
 *                n     - Number of characters to append from s
 *
 *  RETURNS     : true if append was successful
 *                false otherwise
 *
 *****************************************************************************/
bool string_appendn (string_t *str_p, const char *s, long n)
{
    if (!str_p || !s)
        return false;

    if (n == 0)
        return true;

    if (n < 0)
        n = strlen(s);

    if (str_p->length + n >= str_p->capacity)
    {
        if (!string_reserve(str_p, str_p->length + n))
            return false;
    }

    strncpy(str_p->s + str_p->length, s, n);
    str_p->length += n;
    str_p->s[str_p->length] = '\0';

    return true;
}

/*****************************************************************************
 *
 *  NAME        : string_insertn
 *
 *  DESCRIPTION : Inserts the given string at the specified position in the
 *                given string
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
bool string_insertn (string_t *str_p, long pos, const char *s, long n)
{
    if (!str_p || !s)
        return false;

    if (pos < 0 || pos > str_p->length)
        return false;

    if (n == 0)
        return true;

    if (n < 0)
        n = strlen(s);

    if (str_p->length + n >= str_p->capacity)
    {
        char *new_s = malloc(str_p->length + n + 1);
        if (!new_s)
            return false;

        str_p->capacity = str_p->length + n + 1;

        if (pos > 0)
            strncpy(new_s, str_p->s, pos);
        strncpy(new_s + pos, s, n);
        strcpy(new_s + pos + n, str_p->s + pos);

        free(str_p->s);
        str_p->s = new_s;
        str_p->length += n;
    }
    else
    {
        memmove(str_p->s + pos + n, str_p->s + pos, str_p->length - pos + 1);
        strncpy(str_p->s + pos, s, n);
        str_p->length += n;
    }

    return true;
}

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
 *                false otherwise
 *
 *****************************************************************************/
bool string_replace (string_t *str_p, const char *ss_p, const char *rs_p, long n)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return false;

    if (!ss_p || !rs_p)
        return false;

    long  sslen = strlen(ss_p);

    if (sslen > str_p->length)
        return false;

    long  rslen = strlen(rs_p);

    if (sslen == 0 && rslen == 0)
        return true;

    if (n == 0)
        return true;

    long  max_num_replacements = (sslen == 0) ? (str_p->length + 1) : (str_p->length / sslen);

    if (n < 0 || n > max_num_replacements)
        n = max_num_replacements;

    if (sslen == rslen)
    {
        long  idx = 0;
        while (idx + sslen <= str_p->length && n > 0)
        {
            if (0 == strncmp(str_p->s + idx, ss_p, sslen))
            {
                strncpy(str_p->s + idx, rs_p, rslen);
                idx += rslen;

                n--;
            }
            else
                idx++;
        }
    }
    else if (sslen > rslen)
    {
        long  rd_idx = 0,
              wr_idx = 0;
        long  num_replacements = 0;

        while (rd_idx + sslen <= str_p->length && num_replacements < n)
        {
            if (0 == strncmp(str_p->s + rd_idx, ss_p, sslen))
            {
                if (rslen > 0)
                {
                    strncpy(str_p->s + wr_idx, rs_p, rslen);
                    wr_idx += rslen;
                }
                rd_idx += sslen;

                num_replacements++;
            }
            else
            {
                str_p->s[wr_idx++] = str_p->s[rd_idx++];
            }
        }

        strcpy(str_p->s + wr_idx, str_p->s + rd_idx);

        str_p->length = str_p->length - ((sslen - rslen) * num_replacements);
    }
    else // sslen < rslen
    {
        char  *wr_s,
              *rd_s;
        long   required_length = str_p->length + ((rslen - sslen) * n);

        if (required_length >= str_p->capacity)
        {
            wr_s = malloc(required_length + 1);
            if (!wr_s)
                return false;

            str_p->capacity = required_length + 1;

            rd_s = str_p->s;
        }
        else
        {
            wr_s = str_p->s;
            rd_s = str_p->s + ((rslen - sslen) * n);
            memmove(rd_s, str_p->s, str_p->length + 1);
        }

        long  rd_idx = 0,
              wr_idx;

        if (sslen == 0)
        {
            strcpy(wr_s, rs_p);
            wr_idx = rslen;
            n--;

            while (rd_idx < str_p->length && n > 0)
            {
                wr_s[wr_idx++] = rd_s[rd_idx++];

                strcpy(wr_s + wr_idx, rs_p);
                wr_idx += rslen;

                n--;
            }

            if (rd_idx < str_p->length)
                strcpy(wr_s + wr_idx, rd_s + rd_idx);

            str_p->length = required_length;
        }
        else
        {
            long  num_replacements = 0;

            wr_idx = 0;
            while (rd_idx + sslen <= str_p->length && num_replacements < n)
            {
                if (0 == strncmp(rd_s + rd_idx, ss_p, sslen))
                {
                    strncpy(wr_s + wr_idx, rs_p, rslen);
                    wr_idx += rslen;
                    rd_idx += sslen;

                    num_replacements++;
                }
                else
                {
                    wr_s[wr_idx++] = rd_s[rd_idx++];
                }
            }

            strcpy(wr_s + wr_idx, rd_s + rd_idx);

            str_p->length = str_p->length - ((sslen - rslen) * num_replacements);
        }

        if (wr_s != str_p->s)
        {
            free(str_p->s);
            str_p->s = wr_s;
        }
    }

    return true;
}

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
void string_remove (string_t *str_p, long pos, long n)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return;

    if (pos < 0 || pos > str_p->length)
        return;

    if (n < 0 || pos + n > str_p->length)
        return;

    memmove(str_p->s + pos, str_p->s + pos + n, str_p->length - (pos + n) + 1);
    str_p->length -= n;
}

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
void string_remove_prefix (string_t *str_p, long n)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return;

    if (n <= 0)
        return;

    if (n > str_p->length)
        n = str_p->length;

    memmove(str_p->s, str_p->s + n, str_p->length - n + 1);
    str_p->length -= n;
}
void string_remove_suffix (string_t *str_p, long n)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return;

    if (n <= 0)
        return;

    if (n > str_p->length)
        n = str_p->length;
    
    str_p->length -= n;
    str_p->s[str_p->length] = '\0';
}

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
 *  RETURNS     : Nothing
 *
 *****************************************************************************/
void string_trim (string_t *str_p)
{
    string_trim_leading_ws(str_p);
    string_trim_trailing_ws(str_p);
}
void string_trim_leading_ws (string_t *str_p)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return;

    long  i = 0;
    while (   i < str_p->length
           && (str_p->s[i] == ' ' || str_p->s[i] == '\t' || str_p->s[i] == '\n')
          )
    {
        i++;
    }
    if (i > 0)
    {
        memmove(str_p->s, str_p->s + i, str_p->length - i + 1);
        str_p->length -= i;
    }
}
void string_trim_trailing_ws (string_t *str_p)
{
    if (!str_p || !str_p->s || str_p->length == 0)
        return;

    long  i = str_p->length - 1;
    while (   str_p->length > 0
           && (str_p->s[i] == ' ' || str_p->s[i] == '\t' || str_p->s[i] == '\n')
          )
    {
        str_p->length--;
        i--;
    }
    str_p->s[str_p->length] = '\0';
}

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
string_t* string_join (string_t **sarray_p, const char *sep_p)
{
    string_t  *joined_str_p = NULL;

    if (   sarray_p
        && sep_p
       )
    {
        if (sarray_p[0] == NULL)
            joined_str_p = string_newb("", 0, 0);
        else
        {
            long  sep_len = strlen(sep_p),
                  total_slen = 0,
                  count = 0,
                  ii;
            
            while (sarray_p[count])
            {
                total_slen += sarray_p[count]->length;
                count++;
            }
            total_slen += (sep_len * (count - 1));

            if (total_slen == 0)
                joined_str_p = string_newb("", 0, 0);
            else
            {
                joined_str_p = string_newb(NULL, 0, 0);
                if (joined_str_p)
                {
                    joined_str_p->s = malloc(total_slen + 1);
                    if (joined_str_p->s)
                    {
                        joined_str_p->capacity = total_slen + 1;

                        if (sarray_p[0]->length == 0)
                        {
                            joined_str_p->length = 0;
                            joined_str_p->s[0] = '\0';
                        }
                        else
                        {
                            strcpy(joined_str_p->s, sarray_p[0]->s);
                            joined_str_p->length = sarray_p[0]->length;
                        }

                        for (ii = 1; ii < count; ii++)
                        {
                            strcpy(joined_str_p->s + joined_str_p->length, sep_p);
                            joined_str_p->length += sep_len;

                            if (sarray_p[ii]->length != 0)
                            {
                                strcpy(joined_str_p->s + joined_str_p->length, sarray_p[ii]->s);
                                joined_str_p->length += sarray_p[ii]->length;
                            }
                        }
                    }
                    else
                    {
                        free(joined_str_p);
                        joined_str_p = NULL;
                    }
                }
            }
        }
    }

    return joined_str_p;
}

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
string_t** string_split (string_t *str_p, const char *sep_p)
{
    string_t  **sarray_p = NULL;

    if (str_p && str_p->s && sep_p)
    {
        if (str_p->length == 0)
        {
            sarray_p = string_array_new(0);
        }
        else if (sep_p[0] == '\0')
        {
            sarray_p = string_array_new(str_p->length);

            if (sarray_p)
            {
                for (long ii = 0; ii < str_p->length; ii++)
                {
                    sarray_p[ii] = string_substr(str_p, ii, 1);

                    if (!sarray_p[ii])
                    {
                        string_array_del(sarray_p);
                        sarray_p = NULL;

                        break;
                    }
                }
            }
        }
        else
        {
            long  sep_len = strlen(sep_p);
            long  ppos, pos;
            long  count = 0;

            pos = string_find(str_p, 0, sep_p);
            while (pos != -1)
            {
                count++;
                pos = string_find(str_p, pos + sep_len, sep_p);
            }

            sarray_p = string_array_new(count + 1);

            if (sarray_p)
            {
                count = 0;
                ppos = 0;
                pos = string_find(str_p, 0, sep_p);
                while (pos != -1)
                {
                    sarray_p[count] = string_substr(str_p, ppos, pos - ppos);
                    if (!sarray_p[count])
                    {
                        string_array_del(sarray_p);
                        sarray_p = NULL;

                        break;
                    }
                    count++;

                    ppos = pos + sep_len;
                    pos = string_find(str_p, ppos, sep_p);
                }

                sarray_p[count] = string_substr(str_p, ppos, -1);
                if (!sarray_p[count])
                {
                    string_array_del(sarray_p);
                    sarray_p = NULL;
                }
            }
        }
    }

    return sarray_p;
}

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
string_t** string_ssplit (string_t *str_p, const char *sep_p)
{
    string_t  **sarray_p = NULL;

    if (str_p && str_p->s && sep_p)
    {
        if (str_p->length == 0)
        {
            sarray_p = string_array_new(0);
        }
        else if (sep_p[0] == '\0')
        {
            long  ii;

            sarray_p = string_array_new(str_p->length);

            if (sarray_p)
            {
                for (ii = 0; ii < str_p->length; ii++)
                {
                    sarray_p[ii] = string_substr(str_p, ii, 1);

                    if (!sarray_p[ii])
                    {
                        string_array_del(sarray_p);
                        sarray_p = NULL;

                        break;
                    }
                }
            }
        }
        else
        {
            long  ii,
                  i_str = 0,
                  pos = 0,
                  count = 0;

            for (ii = 0; ii < str_p->length; ii++)
                if (strchr(sep_p, str_p->s[ii]))
                    count++;

            sarray_p = string_array_new(count + 1);

            if (sarray_p)
            {
                for (ii = 0; ii < str_p->length; ii++)
                {
                    if (strchr(sep_p, str_p->s[ii]))
                    {
                        sarray_p[i_str] = string_substr(str_p, pos, ii - pos);

                        if (!sarray_p[i_str])
                        {
                            string_array_del(sarray_p);
                            sarray_p = NULL;

                            break;
                        }

                        pos = ii + 1;
                        i_str++;
                    }
                }

                if (sarray_p)
                {
                    sarray_p[i_str] = string_substr(str_p, pos, str_p->length - pos);

                    if (!sarray_p[i_str])
                    {
                        string_array_del(sarray_p);
                        sarray_p = NULL;
                    }
                }
            }
        }
    }

    return sarray_p;
}


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
string_t** string_array_new (long count)
{
    string_t  **sarray_p = NULL;

    if (count >= 0)
    {
        sarray_p = calloc(count + 1, sizeof(*sarray_p));
    }

    return sarray_p;
}

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
string_t** string_array_del (string_t **sarray_p)
{
    if (sarray_p)
    {
        long  ii = 0;

        while (sarray_p[ii])
        {
            string_delete(sarray_p[ii]);
            ii++;
        }
        free(sarray_p);
    }

    return NULL;
}

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
string_t** string_array_clone (string_t **sarray_p)
{
    string_t  **nsarray_p = NULL;

    if (sarray_p)
    {
        long  ii,
              count = 0;

        while (sarray_p[count])
            count++;

        nsarray_p = string_array_new(count);

        if (nsarray_p)
        {
            for (ii = 0; ii < count; ii++)
            {
                nsarray_p[ii] = string_clone(sarray_p[ii]);

                if (!nsarray_p[ii])
                {
                    string_array_del(nsarray_p);
                    nsarray_p = NULL;

                    break;
                }
            }
        }
    }

    return nsarray_p;
}
