/*****************************************************************************
 *
 * FILE NAME     : strv.c
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
 *****************************************************************************/

/*****************************************************************************
   Include Files
*****************************************************************************/
#include <string.h>
#include "strv.h"


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
static char char_tolower (char ch);
static char char_toupper (char ch);


/*****************************************************************************
   Local Functions
*****************************************************************************/
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
stringv_t* empty_stringv (void)
{
    static stringv_t  empty_str = {"", 0L};

    return &empty_str;
}
stringv_t* true_stringv  (void)
{
    static stringv_t  true_str = {"true", 4L};

    return &true_str;
}
stringv_t* false_stringv (void)
{
    static stringv_t  false_str = {"false", 5L};

    return &false_str;
}

/*****************************************************************************
 *
 *  NAME        : stringv_setb
 *
 *  DESCRIPTION : Get length of the given string
 *
 *  PARAMS      : str_p - The string view
 *                s     - The CString
 *                pos   - Start position in s
 *                len   - Length of CString from pos in s
 *
 *  RETURNS     : void
 *
 *  NOTES       : The function is a no-op if input parameters are invalid.
 *                str_p is initialized only if it is safe to do so.
 *
 *****************************************************************************/
void stringv_setb (stringv_t *str_p, char *s, long pos, long len)
{
    if (!str_p || !s)
        return;

    if (pos < 0)
        return;

    if (len < 0)
    {
        len = strlen(s);
        len -= pos;

        if (len < 0)
            return;
    }

    str_p->s = s + pos;
    str_p->slen = len;
}

/*****************************************************************************
 *
 *  NAME        : stringv_get
 *
 *  DESCRIPTION : Get the character array from the string view
 *
 *  PARAMS      : str_p - The string view
 *
 *  RETURNS     : Pointer to the character array
 *                NULLL if str_p was invalid
 *
 *  NOTES       : stringv_t may be pointing to a character array that has more
 *                that stringv_t.slen characters. In otherwords, the returned
 *                string may not be null terminated the way you expect or may
 *                not be null terminated at all. So, use this function only if
 *                stringv operations haven't changed stringv_t.slen.
 *                If you are unsure, best way is to use stringv_copy to copy
 *                the string out to a buffer.
 *
 *****************************************************************************/
char* stringv_get (stringv_t *str_p)
{
    if (!str_p || !str_p->s)
        return NULL;

    return str_p->s;
}

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
char* stringv_copy (stringv_t *str_p, char *buffer_p, long buffer_size)
{
    if (!str_p || !str_p->s)
        return NULL;

    if (!buffer_p || buffer_size <= str_p->slen)
        return NULL;

    if (str_p->slen > 0)
        strncpy(buffer_p, str_p->s, str_p->slen);
    buffer_p[str_p->slen] = '\0';

    return buffer_p;
}

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
char stringv_at (stringv_t *str_p, long pos)
{
    if (!str_p || !str_p->s)
        return '\0';

    if (pos < 0 || pos >= str_p->slen)
        return '\0';

    return str_p->s[pos];
}

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
long stringv_length (stringv_t *str_p)
{
    return str_p? str_p->slen : -1;
}

/*****************************************************************************
 *
 *  NAME        : stringv_empty
 *
 *  DESCRIPTION : Check if the given string is empty
 *
 *  PARAMS      : str_p - The string
 *
 *  RETURNS     : true if the string is empty
 *                false otherwise
 *
 *****************************************************************************/
bool stringv_empty (stringv_t *str_p)
{
    return str_p ? str_p->slen == 0 : true;
}
bool stringv_blank (stringv_t *str_p)
{
    if (!str_p || !str_p->s)
        return true;

    for (long i = 0; i < str_p->slen; i++)
    {
        if (str_p->s[i] != ' ' && str_p->s[i] != '\t')
            return false;
    }

    return true;
}

/*****************************************************************************
 *
 *  NAME        : stringv_tolower
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
void stringv_tolower (stringv_t *str_p)
{
    if (!str_p || !str_p->s)
        return;

    for (long i = 0; i < str_p->slen; i++)
        str_p->s[i] = char_tolower(str_p->s[i]);
}

/*****************************************************************************
 *
 *  NAME        : stringv_toupper
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
void stringv_toupper (stringv_t *str_p)
{
    if (!str_p || !str_p->s)
        return;

    for (long i = 0; i < str_p->slen; i++)
        str_p->s[i] = char_toupper(str_p->s[i]);
}

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
int stringv_compare (stringv_t *str1_p, stringv_t *str2_p, long n, bool icase_b)
{
    if (str1_p && str1_p->s)
    {
        if (str2_p && str2_p->s)
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
 *  NAME        : stringv_starts_with
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
bool stringv_starts_with (stringv_t *str_p, const char *prefix_p)
{
    if (!str_p || !str_p->s)
        return false;
    
    if (!prefix_p)
        return false;

    long  prefix_len = strlen(prefix_p);

    if (prefix_len > str_p->slen)
        return false;

    /* All strings start with empty prefix */
    if (prefix_len == 0)
        return true;

    return 0 == strncmp(str_p->s, prefix_p, prefix_len);
}

/*****************************************************************************
 *
 *  NAME        : stringv_ends_with
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
 bool stringv_ends_with (stringv_t *str_p, const char *suffix_p)
{
    if (!str_p || !str_p->s)
        return false;
    
    if (!suffix_p)
        return false;

    long  suffix_len = strlen(suffix_p);

    if (suffix_len > str_p->slen)
        return false;

    /* All strings start with empty prefix */
    if (suffix_len == 0)
        return true;

    return 0 == strncmp(str_p->s + (str_p->slen - suffix_len), suffix_p, suffix_len);
}

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
long stringv_span (stringv_t *str_p, const char *accept_p)
{
    if (!str_p || !str_p->s)
        return 0;

    if (!accept_p)
        return 0;

    return strspn(str_p->s, accept_p);
}

/*****************************************************************************
 *
 *  NAME        : stringv_find
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
long stringv_find (stringv_t *str_p, long pos, const char *substr_p)
{
    if (!str_p || !str_p->s)
        return -1;

    if (pos < 0 || pos > str_p->slen)
        return -1;

    if (!substr_p)
        return -1;

    long  substring_len = strlen(substr_p);

    if (substring_len == 0)
        return pos;

    while (pos + substring_len <= str_p->slen)
    {
        if (0 == strncmp(str_p->s + pos, substr_p, substring_len))
            return pos;

        pos++;
    }

    return -1;
}

/*****************************************************************************
 *
 *  NAME        : stringv_rfind
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
long stringv_rfind (stringv_t *str_p, long last_pos, const char *substr_p)
{
    if (!str_p || !str_p->s)
        return -1;

    if (last_pos < 0 || last_pos > str_p->slen)
        return -1;

    if (!substr_p)
        return -1;

    long  substring_len = strlen(substr_p);

    if (substring_len == 0)
        return last_pos;

    last_pos++;
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
void stringv_substr (stringv_t *in_p, long pos, long n, stringv_t *out_p)
{
    if (!in_p || !out_p)
        return;

    if (!in_p->s)
        return;

    if (pos < 0 || pos > in_p->slen)
        return;

    if (n < 0 || n > in_p->slen - pos)
        n = in_p->slen - pos;
    
    out_p->s = in_p->s + pos;
    out_p->slen = n;
}

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
void stringv_remove_prefix (stringv_t *str_p, long n)
{
    if (!str_p || !str_p->s || str_p->slen == 0)
        return;

    if (n <= 0)
        return;

    if (n > str_p->slen)
        n = str_p->slen;

    str_p->s += n;
    str_p->slen -= n;
}
void stringv_remove_suffix (stringv_t *str_p, long n)
{
    if (!str_p || !str_p->s || str_p->slen == 0)
        return;

    if (n <= 0)
        return;

    if (n > str_p->slen)
        n = str_p->slen;
    
    str_p->slen -= n;
}

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
void stringv_trim (stringv_t *str_p)
{
    stringv_trim_leading_ws(str_p);
    stringv_trim_trailing_ws(str_p);
}
void stringv_trim_leading_ws (stringv_t *str_p)
{
    if (!str_p || !str_p->s || str_p->slen == 0)
        return;

    long  i = 0;
    while (   i < str_p->slen
           && (str_p->s[i] == ' ' || str_p->s[i] == '\t' || str_p->s[i] == '\n')
          )
    {
        str_p->s++;
        str_p->slen--;

        i++;
    }
}
void stringv_trim_trailing_ws (stringv_t *str_p)
{
    if (!str_p || !str_p->s || str_p->slen == 0)
        return;

    long  i = str_p->slen - 1;
    while (   str_p->slen > 0
           && (str_p->s[i] == ' ' || str_p->s[i] == '\t' || str_p->s[i] == '\n')
          )
    {
        str_p->slen--;
        i--;
    }
}

