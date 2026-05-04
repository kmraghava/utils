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
#include <ctype.h>
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
struct str_s
{
    long  slen;
    char  s[0];
};


/*****************************************************************************
   Local Variables
*****************************************************************************/
static str_t   empty_string = { .slen = 0 };
static str_t  *estr_p = &empty_string;


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
str_t* str_new (const char *s)
{
    str_t  *str_p = NULL;
    
    if (s)
    {
        if (s[0] == '\0')
            str_p = estr_p;
        else
        {
            long  slen = strlen(s);

            str_p = malloc(sizeof(*str_p) + slen + 1);
            if (str_p)
            {
                str_p->slen = slen;
                strcpy(str_p->s, s);
            }
        }
    }

    return str_p;
}

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
str_t* str_del (str_t *str_p)
{
    if (str_p && str_p != estr_p)
        free(str_p);

    return NULL;
}

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
str_t* str_clone (str_t *str_p)
{
    str_t  *nstr_p = NULL;

    if (str_p)
    {
        if (str_p == estr_p)
            nstr_p = estr_p;
        else
        {
            nstr_p = malloc(sizeof(*nstr_p) + str_p->slen + 1);
            if (nstr_p)
            {
                nstr_p->slen = str_p->slen;
                strcpy(nstr_p->s, str_p->s);
            }
        }
    }

    return nstr_p;
}

/*****************************************************************************
 *
 *  NAME        : str_cstr
 *
 *  DESCRIPTION : Returns the C string representation of the given string
 *
 *  PARAMS      : str_p - String
 *
 *  RETURNS     : Returns C string.
 *                Returns "" if input was also empty_string().
 *
 *****************************************************************************/
const char* str_cstr (str_t *str_p)
{
    const char *cstr_p = NULL;

    if (str_p)
    {
        if (str_p == estr_p)
            cstr_p = "";
        else
            cstr_p = str_p->s;
    }

    return cstr_p;
}

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
str_t** str_array_new (long count)
{
    str_t  **sarray_p = NULL;

    if (count > 0)
    {
        sarray_p = calloc(count, sizeof(*sarray_p));
    }

    return sarray_p;
}

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
str_t** str_array_del (str_t **sarray_p)
{
    if (sarray_p)
    {
        long  ii = 0;

        while (sarray_p[ii])
        {
            str_del(sarray_p[ii]);
            ii++;
        }
        free(sarray_p);
    }

    return NULL;
}

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
str_t** str_array_clone (str_t **sarray_p)
{
    str_t  **nsarray_p = NULL;

    if (sarray_p)
    {
        long  ii,
              count = 0;

        while (sarray_p[count])
            count++;

        nsarray_p = str_array_new(count + 1);

        if (nsarray_p)
        {
            for (ii = 0; ii < count; ii++)
            {
                nsarray_p[ii] = str_clone(sarray_p[ii]);

                if (!nsarray_p[ii])
                {
                    str_array_del(nsarray_p);
                    nsarray_p = NULL;

                    break;
                }
            }
        }
    }

    return nsarray_p;
}

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
str_t* empty_str (void)
{
    return estr_p;
}

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
long str_length (str_t *str_p)
{
    return str_p ? str_p->slen : 0;
}

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
str_t* str_tolower (str_t *str_p)
{
    str_t  *lstr_p = NULL;

    if (str_p)
    {
        if (str_p == estr_p)
            lstr_p = estr_p;
        else
        {
            lstr_p = malloc(sizeof(*lstr_p) + str_p->slen + 1);
            if (lstr_p)
            {
                long  ii;

                lstr_p->slen = str_p->slen;
                for (ii = 0; ii < str_p->slen; ii++)
                {
                    lstr_p->s[ii] = tolower(str_p->s[ii]);
                }
                lstr_p->s[ii] = '\0';
            }
        }
    }

    return lstr_p;
}

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
str_t* str_toupper (str_t *str_p)
{
    str_t  *ustr_p = NULL;

    if (str_p)
    {
        if (str_p == estr_p)
            ustr_p = estr_p;
        else
        {
            ustr_p = malloc(sizeof(*ustr_p) + str_p->slen + 1);
            if (ustr_p)
            {
                long  ii;

                ustr_p->slen = str_p->slen;
                for (ii = 0; ii < str_p->slen; ii++)
                {
                    ustr_p->s[ii] = toupper(str_p->s[ii]);
                }
                ustr_p->s[ii] = '\0';
            }
        }
    }

    return ustr_p;
}

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
int str_compare (str_t *str1_p, str_t *str2_p, long n, bool icase_b)
{
    if (str1_p)
    {
        if (str2_p)
        {
            long  n1 = n,
                  n2 = n;

            if (n == 0)
                return 0;

            if (n1 < 0 || n1 > str1_p->slen)
                n1 = str1_p->slen;

            if (n2 < 0 || n2 > str2_p->slen)
                n2 = str2_p->slen;

            if (n1 < n2)
                return -1;
            else if (n1 > n2)
                return 1;
            else
            {
                str_t  *cmp1_p = str1_p,
                       *cmp2_p = str2_p;
                int     cmp_result;

                if (icase_b)
                {
                    cmp1_p = str_tolower(str1_p);
                    cmp2_p = str_tolower(str2_p);
                }

                cmp_result = strncmp(cmp1_p->s, cmp2_p->s, n1);

                if (icase_b)
                {
                    str_del(cmp1_p);
                    str_del(cmp2_p);
                }

                return cmp_result;
            }
        }
        else
            return 1;
    }
    else
    {
        if (str2_p)
            return -1;

        return 0;
    }
}

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
long str_contains (str_t *str_p, long pos, const char *substr_p)
{
    long  sspos = -1;

    if (   str_p
        && str_p != estr_p
        && pos >= 0
        && pos <= str_p->slen
        && substr_p
       )
    {
        char  *substr_pos_p = strstr(str_p->s + pos, substr_p);

        if (substr_pos_p)
            sspos = (long)(substr_pos_p - str_p->s);
    }

    return sspos;
}

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
str_t* str_substr (str_t *str_p, long pos, long n)
{
    str_t  *substr_p = NULL;

    if (   str_p
        && pos >= 0
        && pos <= str_p->slen
       )
    {
        if (   n < 0
            || (   n > 0
                && pos + n > str_p->slen
               )
            )
        {
            n = str_p->slen - pos;
        }

        if (n == 0)
            substr_p = estr_p;
        else
        {            
            substr_p = malloc(sizeof(*substr_p) + n + 1);

            if (substr_p)
            {
                substr_p->slen = n;
                strncpy(substr_p->s, str_p->s + pos, n);
                substr_p->s[n] = '\0';
            }
        }
    }

    return substr_p;
}

/*****************************************************************************
 *
 *  NAME        : str_replace
 *
 *  DESCRIPTION : Create a new string by replacing the first n occurrences of
 *                the given substring in the given string with the given
 *                replacement.
 *
 *  PARAMS      : str_p - The string
 *                ss_p  - The substring to replace
 *                rs_p  - The replacement string
 *                n     - Number of occurrences to replace
 *
 *  RETURNS     : new string with replacements
 *                NULL if str_p is NULL or if there is a malloc failure
 *                Equivalent to str_clone() if n = 0 or if ss_p is not
 *                found in str_p
 *
 *****************************************************************************/
str_t* str_replace (str_t *str_p, const char *ss_p, const char *rs_p, long n)
{
    str_t  *replaced_str_p = NULL;

    if (   str_p
        && ss_p
        && rs_p
       )
    {
        if (str_p == estr_p)
            replaced_str_p = estr_p;
        else if (n == 0)
            replaced_str_p = str_clone(str_p);
        else if (ss_p[0] == '\0')
        {
            if (rs_p[0] == '\0')
                replaced_str_p = str_clone(str_p);
            else
            {
                long  rslen = strlen(rs_p);
                long  total_length;

                if (n < 0 || n > str_p->slen + 1)
                    n = str_p->slen + 1;

                total_length = str_p->slen + (rslen * n);
                replaced_str_p = malloc(sizeof(*replaced_str_p) + total_length + 1);

                if (replaced_str_p)
                {
                    strcpy(replaced_str_p->s, rs_p);
                    replaced_str_p->slen = rslen;

                    for (long ii = 0; ii < n-1; ii++)
                    {
                        replaced_str_p->s[replaced_str_p->slen] = str_p->s[ii];
                        replaced_str_p->slen++;

                        strcpy(replaced_str_p->s + replaced_str_p->slen, rs_p);
                        replaced_str_p->slen += rslen;
                    }
                }
            }
        }
        else
        {
            long  sslen = strlen(ss_p);
            long  rslen = strlen(rs_p);
            long  ppos, pos;
            long  count = 0;
            long  total_length;

            pos = str_contains(str_p, 0, ss_p);
            while (pos != -1)
            {
                count++;
                pos = str_contains(str_p, pos + sslen, ss_p);
            }

            if (count == 0)
                replaced_str_p = str_clone(str_p);
            else
            {
                if (n < 0 || n > count)
                    n = count;

                total_length = str_p->slen + ((rslen - sslen) * n);
                replaced_str_p = malloc(sizeof(*replaced_str_p) + total_length + 1);

                if (replaced_str_p)
                {
                    replaced_str_p->slen = 0;
                    ppos = 0;
                    
                    for (long ii = 0; ii < n; ii++)
                    {
                        pos = str_contains(str_p, ppos, ss_p);
                        strncpy(replaced_str_p->s + replaced_str_p->slen, str_p->s + ppos, pos - ppos);
                        replaced_str_p->slen += (pos - ppos);

                        strcpy(replaced_str_p->s + replaced_str_p->slen, rs_p);
                        replaced_str_p->slen += rslen;

                        ppos = pos + sslen;
                    }

                    if (ppos < str_p->slen)
                    {
                        strcpy(replaced_str_p->s + replaced_str_p->slen, str_p->s + ppos);
                        replaced_str_p->slen += (str_p->slen - ppos);
                    }
                }
            }
        }
    }

    return replaced_str_p;
}

/*****************************************************************************
 *
 *  NAME        : str_join
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
str_t* str_join (str_t **sarray_p, const char *sep_p)
{
    str_t  *joined_str_p = NULL;

    if (   sarray_p
        && sep_p
       )
    {
        if (sarray_p[0] == NULL)
        {
            joined_str_p = estr_p;
        }
        else
        {
            long  sep_len = strlen(sep_p),
                  total_slen = 0,
                  count = 0,
                  ii;
            
            while (sarray_p[count])
            {
                total_slen += sarray_p[count]->slen;
                count++;
            }
            total_slen += sep_len * (count - 1);

            joined_str_p = malloc(sizeof(*joined_str_p) + total_slen + 1);

            if (joined_str_p)
            {
                strcpy(joined_str_p->s, sarray_p[0]->s);
                joined_str_p->slen = sarray_p[0]->slen;

                for (ii = 1; ii < count; ii++)
                {
                    strcpy(joined_str_p->s + joined_str_p->slen, sep_p);
                    joined_str_p->slen += sep_len;

                    strcpy(joined_str_p->s + joined_str_p->slen, sarray_p[ii]->s);
                    joined_str_p->slen += sarray_p[ii]->slen;
                }
            }
        }
    }

    return joined_str_p;
}

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
str_t** str_split (str_t *str_p, const char *sep_p)
{
    str_t  **sarray_p = NULL;

    if (str_p && sep_p)
    {
        if (str_p == estr_p)
        {
            sarray_p = str_array_new(1);
        }
        else if (sep_p[0] == '\0')
        {
            long  ii;

            sarray_p = str_array_new(str_p->slen + 1);

            if (sarray_p)
            {
                for (ii = 0; ii < str_p->slen; ii++)
                {
                    sarray_p[ii] = str_substr(str_p, ii, 1);

                    if (!sarray_p[ii])
                    {
                        str_array_del(sarray_p);
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

            pos = str_contains(str_p, 0, sep_p);
            while (pos != -1)
            {
                count++;
                pos = str_contains(str_p, pos + sep_len, sep_p);
            }

            sarray_p = str_array_new(count + 2);

            if (sarray_p)
            {
                count = 0;
                ppos = 0;
                pos = str_contains(str_p, 0, sep_p);
                while (pos != -1)
                {
                    sarray_p[count] = str_substr(str_p, ppos, pos - ppos);
                    if (!sarray_p[count])
                    {
                        str_array_del(sarray_p);
                        sarray_p = NULL;

                        break;
                    }
                    count++;

                    ppos = pos + sep_len;
                    pos = str_contains(str_p, ppos, sep_p);
                }

                sarray_p[count] = str_substr(str_p, ppos, -1);
                if (!sarray_p[count])
                {
                    str_array_del(sarray_p);
                    sarray_p = NULL;
                }
            }
        }
    }

    return sarray_p;
}

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
 *                If sep_p is "", then the given string is split into
 *                constituent characters and the resulting array is returned.
 *
 *                sep_p is a set of characters. Therefore delimiter is any
 *                of the characters in sep_p. For example, if sep_p = ":-",
 *                then "a:b-c" will be split into a, b, c.
 *
 *****************************************************************************/
str_t** str_ssplit (str_t *str_p, const char *sep_p)
{
    str_t  **sarray_p = NULL;

    if (str_p && sep_p)
    {
        if (str_p == estr_p)
        {
            sarray_p = str_array_new(1);
        }
        else if (sep_p[0] == '\0')
        {
            long  ii;

            sarray_p = str_array_new(str_p->slen + 1);

            if (sarray_p)
            {
                for (ii = 0; ii < str_p->slen; ii++)
                {
                    sarray_p[ii] = str_substr(str_p, ii, 1);

                    if (!sarray_p[ii])
                    {
                        str_array_del(sarray_p);
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

            for (ii = 0; ii < str_p->slen; ii++)
                if (strchr(sep_p, str_p->s[ii]))
                    count++;

            sarray_p = str_array_new(count + 2);

            if (sarray_p)
            {
                for (ii = 0; ii < str_p->slen; ii++)
                {
                    if (strchr(sep_p, str_p->s[ii]))
                    {
                        sarray_p[i_str] = str_substr(str_p, pos, ii - pos);

                        if (!sarray_p[i_str])
                        {
                            str_array_del(sarray_p);
                            sarray_p = NULL;

                            break;
                        }

                        pos = ii + 1;
                        i_str++;
                    }
                }

                if (sarray_p)
                {
                    sarray_p[i_str] = str_substr(str_p, pos, str_p->slen - pos);

                    if (!sarray_p[i_str])
                    {
                        str_array_del(sarray_p);
                        sarray_p = NULL;
                    }
                }
            }
        }
    }

    return sarray_p;
}

