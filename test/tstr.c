/****************************************************************************
 *
 * FILE NAME     : tstr.c
 *
 * DESCRIPTION   : unit tests for string_t
 *
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "kmrUtils/str.h"

/****************************************************************************
 * Helpers
 *****************************************************************************/

static void test_string_new_delete(void)
{
    string_t *s;

    s = string_new(NULL);
    assert(s != NULL);
    assert(s->s == NULL);

    s = string_new("");
    assert(s != NULL);
    assert(string_length(s) == 0);
    assert(strcmp(string_cstr(s), "") == 0);

    string_delete(s);

    s = string_new("hello");

    assert(s != NULL);
    assert(string_length(s) == 5);
    assert(strcmp(string_cstr(s), "hello") == 0);

    string_delete(s);

    assert(string_delete(NULL) == NULL);
}

static void test_string_newn(void)
{
    string_t *s;

    s = string_newn("abcdef", 3);

    assert(s != NULL);
    assert(strcmp(string_cstr(s), "abc") == 0);
    assert(string_length(s) == 3);

    string_delete(s);
}

static void test_string_newb(void)
{
    string_t *s;

    s = string_newb("abcdef", 2, 3);

    assert(s != NULL);
    assert(strcmp(string_cstr(s), "cde") == 0);
    assert(string_length(s) == 3);

    string_delete(s);
}

static void test_string_init(void)
{
    string_t s;

    assert(string_init(&s, "hello") == true);

    assert(strcmp(string_cstr(&s), "hello") == 0);
    assert(string_length(&s) == 5);
}

static void test_string_initn(void)
{
    string_t s;

    assert(string_initn(&s, "abcdef", 4) == true);

    assert(strcmp(string_cstr(&s), "abcd") == 0);
    assert(string_length(&s) == 4);
}

static void test_string_initb(void)
{
    string_t s;

    assert(string_initb(&s, "abcdef", 1, 3) == true);

    assert(strcmp(string_cstr(&s), "bcd") == 0);
}

static void test_string_set(void)
{
    string_t s;

    assert(string_init(&s, "hello") == true);

    assert(string_set(&s, "world") == true);

    assert(strcmp(string_cstr(&s), "world") == 0);
    assert(string_length(&s) == 5);
}

static void test_string_setn(void)
{
    string_t s;

    assert(string_init(&s, "hello") == true);

    assert(string_setn(&s, "abcdef", 3) == true);

    assert(strcmp(string_cstr(&s), "abc") == 0);
}

static void test_string_setb(void)
{
    string_t s;

    assert(string_init(&s, "hello") == true);

    assert(string_setb(&s, "abcdef", 2, 2) == true);

    assert(strcmp(string_cstr(&s), "cd") == 0);
}

static void test_string_clone(void)
{
    string_t *s1, *s2;

    assert(string_clone(NULL) == NULL);

    s1 = string_new("hello");
    s2 = string_clone(s1);

    assert(s2 != NULL);
    assert(s2 != s1);
    assert(string_compare(s1, s2, -1, false) == 0);

    string_delete(s1);
    string_delete(s2);
}

static void test_string_swap(void)
{
    string_t *a, *b;

    a = string_new("hello");
    b = string_new("world");

    string_swap(a, b);

    assert(strcmp(string_cstr(a), "world") == 0);
    assert(strcmp(string_cstr(b), "hello") == 0);

    string_delete(a);
    string_delete(b);
}

static void test_string_reserve(void)
{
    string_t *s;
    long old_capacity;

    s = string_new("abc");

    old_capacity = string_capacity(s);

    assert(string_reserve(s, 100) == true);

    assert(string_capacity(s) >= 100);
    assert(string_capacity(s) >= old_capacity);

    assert(strcmp(string_cstr(s), "abc") == 0);

    string_delete(s);
}

static void test_string_array_new_delete(void)
{
    string_t **arr;

    assert(string_array_new(-1) == NULL);

    arr = string_array_new(3);

    assert(arr != NULL);
    assert(arr[0] == NULL);
    assert(arr[1] == NULL);
    assert(arr[2] == NULL);

    assert(string_array_del(NULL) == NULL);
    assert(string_array_del(arr) == NULL);
}

static void test_string_array_clone(void)
{
    string_t **arr1, **arr2;

    assert(string_array_clone(NULL) == NULL);

    arr1 = string_array_new(3);

    arr1[0] = string_new("abc");
    arr1[1] = string_new("def");
    arr1[2] = NULL;

    arr2 = string_array_clone(arr1);

    assert(arr2 != NULL);
    assert(arr2 != arr1);

    assert(string_compare(arr1[0], arr2[0], -1, false) == 0);
    assert(string_compare(arr1[1], arr2[1], -1, false) == 0);

    assert(arr1[0] != arr2[0]);
    assert(arr1[1] != arr2[1]);

    string_array_del(arr1);
    string_array_del(arr2);
}

static void test_string_length(void)
{
    string_t *s;

    assert(string_length(NULL) == 0);

    s = string_new("abcdef");

    assert(string_length(s) == 6);

    string_delete(s);
}

static void test_string_capacity(void)
{
    string_t *s;

    assert(string_capacity(NULL) == 0);

    s = string_new("abcdef");

    assert(string_capacity(s) >= 6);

    string_delete(s);
}

static void test_string_empty(void)
{
    string_t *s;

    assert(string_empty(NULL) == true);

    s = string_new("");

    assert(string_empty(s) == true);

    string_delete(s);

    s = string_new("abc");

    assert(string_empty(s) == false);

    string_delete(s);
}

static void test_string_data(void)
{
    string_t *s;
    char *p;

    s = string_new("hello");

    p = string_data(s);

    assert(p != NULL);

    p[0] = 'H';

    assert(strcmp(string_cstr(s), "Hello") == 0);

    string_delete(s);
}

static void test_string_at(void)
{
    string_t *s;

    s = string_new("abcdef");

    assert(string_at(s, 0) == 'a');
    assert(string_at(s, 5) == 'f');

    string_delete(s);
}

static void test_string_tolower(void)
{
    string_t *s;

    string_tolower(NULL);

    s = string_new("AbC123");

    string_tolower(s);

    assert(strcmp(string_cstr(s), "abc123") == 0);

    string_delete(s);
}

static void test_string_toupper(void)
{
    string_t *s;

    string_toupper(NULL);

    s = string_new("AbC123");

    string_toupper(s);

    assert(strcmp(string_cstr(s), "ABC123") == 0);

    string_delete(s);
}

static void test_string_compare(void)
{
    string_t *a, *b;

    assert(string_compare(NULL, NULL, -1, false) == 0);

    a = string_new("abc");

    assert(string_compare(a, NULL, -1, false) == 1);
    assert(string_compare(NULL, a, -1, false) == -1);

    b = string_new("abcd");

    assert(string_compare(a, b, -1, false) < 0);
    assert(string_compare(b, a, -1, false) > 0);

    string_delete(b);

    b = string_new("abd");

    assert(string_compare(a, b, -1, false) < 0);
    assert(string_compare(b, a, -1, false) > 0);

    string_delete(b);

    b = string_new("ABC");

    assert(string_compare(a, b, -1, true) == 0);
    assert(string_compare(a, b, -1, false) > 0);

    assert(string_compare(a, b, 0, false) == 0);

    b = string_delete(b);
    b = string_new("abz");

    assert(string_compare(a, b, 2, false) == 0);

    string_delete(a);
    string_delete(b);
}

static void test_compare_prefix_lengths(void)
{
    string_t *a, *b;

    a = string_new("abcdef");
    b = string_new("abcxyz");

    assert(string_compare(a, b, 3, false) == 0);
    assert(string_compare(a, b, 4, false) < 0);

    string_delete(a);
    string_delete(b);
}

static void test_compare_case_insensitive_partial(void)
{
    string_t *a, *b;

    a = string_new("AbCdEf");
    b = string_new("abcdef");

    assert(string_compare(a, b, 6, true) == 0);
    assert(string_compare(a, b, 6, false) != 0);

    string_delete(a);
    string_delete(b);
}

static void test_compare_negative_n(void)
{
    string_t *a, *b;

    a = string_new("abc");
    b = string_new("abc");

    assert(string_compare(a, b, -1, false) == 0);

    string_delete(a);
    string_delete(b);
}

static void test_compare_empty_vs_nonempty(void)
{
    string_t *s;

    s = string_new("x");

    assert(string_compare(string_new(""), s, -1, false) < 0);
    assert(string_compare(s, string_new(""), -1, false) > 0);

    string_delete(s);
}

static void test_string_starts_with(void)
{
    string_t *s;

    s = string_new("hello world");

    assert(string_starts_with(s, "hello") == true);
    assert(string_starts_with(s, "world") == false);

    string_delete(s);
}

static void test_string_ends_with(void)
{
    string_t *s;

    s = string_new("hello world");

    assert(string_ends_with(s, "world") == true);
    assert(string_ends_with(s, "hello") == false);

    string_delete(s);
}

static void test_string_contains(void)
{
    string_t *s;

    s = string_new("hello world");

    assert(string_contains(s, "hello") == true);
    assert(string_contains(s, "world") == true);
    assert(string_contains(s, "xyz") == false);

    string_delete(s);
}

static void test_string_find(void)
{
    string_t *s;

    s = string_new("hello world");

    assert(string_find(NULL, 0, "hello") == -1);
    assert(string_find(s, 100, "hello") == -1);
    assert(string_find(s, 0, NULL) == -1);

    assert(string_find(s, 0, "hello") == 0);
    assert(string_find(s, 0, "world") == 6);
    assert(string_find(s, 3, "world") == 6);

    assert(string_find(s, 0, "xyz") == -1);

    string_delete(s);
}

static void test_contains_empty_substring(void)
{
    string_t *s;

    s = string_new("abcdef");

    assert(string_find(s, 0, "") == 0);
    assert(string_find(s, 3, "") == 3);
    assert(string_find(s, 6, "") == 6);

    string_delete(s);
}

static void test_contains_overlapping_pattern(void)
{
    string_t *s;

    s = string_new("aaaaa");

    assert(string_find(s, 0, "aa") == 0);
    assert(string_find(s, 1, "aa") == 1);
    assert(string_find(s, 2, "aa") == 2);
    assert(string_find(s, 3, "aa") == 3);

    string_delete(s);
}

static void test_contains_at_end_boundary(void)
{
    string_t *s;

    s = string_new("abcdef");

    assert(string_find(s, 5, "f") == 5);
    assert(string_find(s, 6, "") == 6);
    assert(string_find(s, 6, "x") == -1);

    string_delete(s);
}

static void test_string_rfind(void)
{
    string_t *s;

    s = string_new("abc def abc def");

    assert(string_rfind(NULL, -1, "abc") == -1);
    assert(string_rfind(s, -1, NULL) == -1);

    assert(string_rfind(s, s->length, "abc") == 8);
    assert(string_rfind(s, 8, "abc") == 0);
    assert(string_rfind(s, s->length, "def") == 12);
    assert(string_rfind(s, 12, "def") == 4);

    string_delete(s);
}

static void test_string_substr(void)
{
    string_t *s, *sub;

    assert(string_substr(NULL, 0, 1) == NULL);

    s = string_new("abcdef");

    sub = string_substr(s, 2, 2);

    assert(strcmp(string_cstr(sub), "cd") == 0);

    string_delete(sub);

    sub = string_substr(s, 2, -1);

    assert(strcmp(string_cstr(sub), "cdef") == 0);

    string_delete(sub);

    sub = string_substr(s, 4, 100);

    assert(strcmp(string_cstr(sub), "ef") == 0);

    string_delete(sub);

    string_delete(s);
}

static void test_substr_full_string(void)
{
    string_t *s, *sub;

    s = string_new("abcdef");

    sub = string_substr(s, 0, -1);

    assert(strcmp(string_cstr(sub), "abcdef") == 0);
    assert(sub != s);

    string_delete(sub);
    string_delete(s);
}

static void test_substr_last_character(void)
{
    string_t *s, *sub;

    s = string_new("abcdef");

    sub = string_substr(s, 5, 1);

    assert(strcmp(string_cstr(sub), "f") == 0);

    string_delete(sub);
    string_delete(s);
}

static void test_string_append(void)
{
    string_t *s;

    s = string_new("abc");

    assert(string_append(s, "def") == true);

    assert(strcmp(string_cstr(s), "abcdef") == 0);

    string_delete(s);
}

static void test_string_appendn(void)
{
    string_t *s;

    s = string_new("abc");

    assert(string_appendn(s, "defghi", 3) == true);

    assert(strcmp(string_cstr(s), "abcdef") == 0);

    string_delete(s);
}

static void test_string_insert(void)
{
    string_t *s;

    s = string_new("abef");

    assert(string_insert(s, 2, "cd") == true);

    assert(strcmp(string_cstr(s), "abcdef") == 0);

    string_delete(s);
}

static void test_string_insertn(void)
{
    string_t *s;

    s = string_new("abgh");

    assert(string_insertn(s, 2, "cdef", 2) == true);

    assert(strcmp(string_cstr(s), "abcdgh") == 0);

    string_delete(s);
}

static void test_replace_null_arguments(void)
{
    string_t *s = string_new("hello");

    assert(string_replace(NULL, "l", "x", 1) == false);
    assert(string_replace(s, NULL, "x", 1) == false);
    assert(string_replace(s, "l", NULL, 1) == false);

    string_delete(s);
}

static void test_replace_n_zero_no_change(void)
{
    string_t *s = string_new("hello world");

    assert(string_replace(s, "world", "earth", 0) == true);

    assert(strcmp(string_cstr(s), "hello world") == 0);

    string_delete(s);
}

static void test_replace_substring_not_found(void)
{
    string_t *s = string_new("abcdef");

    assert(string_replace(s, "xyz", "123", 5) == true);

    assert(strcmp(string_cstr(s), "abcdef") == 0);

    string_delete(s);
}

static void test_replace_replace_single_occurrence(void)
{
    string_t *s = string_new("abc abc abc");

    assert(string_replace(s, "abc", "xyz", 1) == true);

    assert(strcmp(string_cstr(s), "xyz abc abc") == 0);

    string_delete(s);
}

static void test_replace_replace_multiple_occurrences(void)
{
    string_t *s = string_new("abc abc abc");

    assert(string_replace(s, "abc", "xyz", 2) == true);

    assert(strcmp(string_cstr(s), "xyz xyz abc") == 0);

    string_delete(s);
}

static void test_replace_replace_all_occurrences_with_negative_n(void)
{
    string_t *s = string_new("one two one two one");

    assert(string_replace(s, "one", "1", -1) == true);

    assert(strcmp(string_cstr(s), "1 two 1 two 1") == 0);

    string_delete(s);
}

static void test_replace_replace_when_n_exceeds_occurrences(void)
{
    string_t *s = string_new("cat dog cat");

    assert(string_replace(s, "cat", "fox", 10) == true);

    assert(strcmp(string_cstr(s), "fox dog fox") == 0);

    string_delete(s);
}

static void test_replace_replacement_shorter_than_substring(void)
{
    string_t *s = string_new("aaaaaa");

    assert(string_replace(s, "aa", "b", -1) == true);

    assert(strcmp(string_cstr(s), "bbb") == 0);

    string_delete(s);
}

static void test_replace_replacement_longer_than_substring(void)
{
    string_t *s = string_new("xx yy xx");

    assert(string_replace(s, "xx", "LONG", -1) == true);

    assert(strcmp(string_cstr(s), "LONG yy LONG") == 0);

    string_delete(s);
}

static void test_replace_replace_with_empty_replacement(void)
{
    string_t *s = string_new("abc123abc123");

    assert(string_replace(s, "123", "", -1) == true);

    assert(strcmp(string_cstr(s), "abcabc") == 0);

    string_delete(s);
}

static void test_replace_empty_substring_and_empty_replacement(void)
{
    string_t *s = string_new("hello");

    assert(string_replace(s, "", "", 5) == true);

    assert(strcmp(string_cstr(s), "hello") == 0);

    string_delete(s);
}

static void test_replace_empty_substring_insert_prefix_once(void)
{
    string_t *s = string_new("abc");

    assert(string_replace(s, "", "X", 1) == true);

    assert(strcmp(string_cstr(s), "Xabc") == 0);

    string_delete(s);
}

static void test_replace_empty_substring_insert_multiple_times(void)
{
    string_t *s = string_new("abc");

    assert(string_replace(s, "", "-", 4) == true);

    assert(strcmp(string_cstr(s), "-a-b-c-") == 0);

    string_delete(s);
}

static void test_replace_empty_substring_with_negative_n(void)
{
    string_t *s = string_new("abc");

    assert(string_replace(s, "", "+", -1) == true);

    assert(strcmp(string_cstr(s), "+a+b+c+") == 0);

    string_delete(s);
}

static void test_replace_overlapping_substrings(void)
{
    string_t *s = string_new("aaaaa");

    assert(string_replace(s, "aa", "X", -1) == true);

    assert(strcmp(string_cstr(s), "XXa") == 0);

    string_delete(s);
}

static void test_replace_occurrence_at_end(void)
{
    string_t *s;

    s = string_new("abc123");

    assert(string_replace(s, "123", "XYZ", 1) == true);

    assert(strcmp(string_cstr(s), "abcXYZ") == 0);

    string_delete(s);
}

static void test_replace_occurrence_at_beginning(void)
{
    string_t *s;

    s = string_new("123abc");

    assert(string_replace(s, "123", "XYZ", 1) == true);

    assert(strcmp(string_cstr(s), "XYZabc") == 0);

    string_delete(s);
}

static void test_replace_entire_string(void)
{
    string_t *s;

    s = string_new("abcdef");

    assert(string_replace(s, "abcdef", "X", 1) == true);

    assert(strcmp(string_cstr(s), "X") == 0);

    string_delete(s);
}

static void test_replace_with_same_string(void)
{
    string_t *s;

    s = string_new("abc abc");

    assert(string_replace(s, "abc", "abc", -1) == true);

    assert(strcmp(string_cstr(s), "abc abc") == 0);

    string_delete(s);
}

static void test_replace_partial_overlap_not_revisited(void)
{
    string_t *s;

    s = string_new("abababa");

    assert(string_replace(s, "aba", "X", -1) == true);

    assert(strcmp(string_cstr(s), "XbX") == 0);

    string_delete(s);
}

static void test_replace_remove_entire_contents(void)
{
    string_t *s;

    s = string_new("xxxx");

    assert(string_replace(s, "x", "", -1) == true);

    assert(strcmp(string_cstr(s), "") == 0);
    assert(string_length(s) == 0);

    string_delete(s);
}

static void test_replace_separator_style_pattern(void)
{
    string_t *s;

    s = string_new("a,b,c,d");

    assert(string_replace(s, ",", "::", 2) == true);

    assert(strcmp(string_cstr(s), "a::b::c,d") == 0);

    string_delete(s);
}

static void test_remove_prefix(void)
{
    string_t *s;

    s = string_new("abcdef");

    string_remove_prefix(s, 2);

    assert(strcmp(string_cstr(s), "cdef") == 0);

    string_delete(s);
}

static void test_remove_suffix(void)
{
    string_t *s;

    s = string_new("abcdef");

    string_remove_suffix(s, 2);

    assert(strcmp(string_cstr(s), "abcd") == 0);

    string_delete(s);
}

static void test_trim(void)
{
    string_t *s;

    s = string_new(" \t hello world \n ");

    string_trim(s);

    assert(strcmp(string_cstr(s), "hello world") == 0);

    string_delete(s);
}

static void test_trim_leading_ws(void)
{
    string_t *s;

    s = string_new(" \t hello");

    string_trim_leading_ws(s);

    assert(strcmp(string_cstr(s), "hello") == 0);

    string_delete(s);
}

static void test_trim_trailing_ws(void)
{
    string_t *s;

    s = string_new("hello \t \n");

    string_trim_trailing_ws(s);

    assert(strcmp(string_cstr(s), "hello") == 0);

    string_delete(s);
}

static void test_string_join(void)
{
    string_t **arr;
    string_t *joined;

    assert(string_join(NULL, ",") == NULL);

    arr = string_array_new(4);

    arr[0] = string_new("a");
    arr[1] = string_new("bb");
    arr[2] = string_new("ccc");
    arr[3] = NULL;

    joined = string_join(arr, "-");

    assert(joined != NULL);
    assert(strcmp(string_cstr(joined), "a-bb-ccc") == 0);

    string_delete(joined);
    string_array_del(arr);
}

static void test_join_single_element(void)
{
    string_t **arr;
    string_t *joined;

    arr = string_array_new(2);

    arr[0] = string_new("only");
    arr[1] = NULL;

    joined = string_join(arr, ",");

    assert(strcmp(string_cstr(joined), "only") == 0);

    string_delete(joined);
    string_array_del(arr);
}

static void test_join_empty_separator(void)
{
    string_t **arr;
    string_t *joined;

    arr = string_array_new(4);

    arr[0] = string_new("ab");
    arr[1] = string_new("cd");
    arr[2] = string_new("ef");
    arr[3] = NULL;

    joined = string_join(arr, "");

    assert(strcmp(string_cstr(joined), "abcdef") == 0);

    string_delete(joined);
    string_array_del(arr);
}

static void test_join_with_empty_strings(void)
{
    string_t **arr;
    string_t *joined;

    arr = string_array_new(4);

    arr[0] = string_new("");
    arr[1] = string_new("x");
    arr[2] = string_new("");
    arr[3] = NULL;

    joined = string_join(arr, ":");

    assert(strcmp(string_cstr(joined), ":x:") == 0);

    string_delete(joined);
    string_array_del(arr);
}

static void test_string_split(void)
{
    string_t *s;
    string_t **arr;

    assert(string_split(NULL, ",") == NULL);

    s = string_new("a:-b:-c");

    arr = string_split(s, ":-");

    assert(arr != NULL);

    assert(strcmp(string_cstr(arr[0]), "a") == 0);
    assert(strcmp(string_cstr(arr[1]), "b") == 0);
    assert(strcmp(string_cstr(arr[2]), "c") == 0);
    assert(arr[3] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_split_separator_not_present(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("abcdef");

    arr = string_split(s, ",");

    assert(strcmp(string_cstr(arr[0]), "abcdef") == 0);
    assert(arr[1] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_split_separator_at_beginning(void)
{
    string_t *s;
    string_t **arr;

    s = string_new(",abc");

    arr = string_split(s, ",");

    assert(strcmp(string_cstr(arr[0]), "") == 0);
    assert(strcmp(string_cstr(arr[1]), "abc") == 0);
    assert(arr[2] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_split_separator_at_end(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("abc,");

    arr = string_split(s, ",");

    assert(strcmp(string_cstr(arr[0]), "abc") == 0);
    assert(strcmp(string_cstr(arr[1]), "") == 0);
    assert(arr[2] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_split_consecutive_separators(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("a,,b");

    arr = string_split(s, ",");

    assert(strcmp(string_cstr(arr[0]), "a") == 0);
    assert(strcmp(string_cstr(arr[1]), "") == 0);
    assert(strcmp(string_cstr(arr[2]), "b") == 0);
    assert(arr[3] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_split_entire_string_as_separator(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("XYZ");

    arr = string_split(s, "XYZ");

    assert(strcmp(string_cstr(arr[0]), "") == 0);
    assert(strcmp(string_cstr(arr[1]), "") == 0);
    assert(arr[2] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_string_ssplit(void)
{
    string_t *s;
    string_t **arr;

    assert(string_ssplit(NULL, ",") == NULL);

    s = string_new("a:b-c");

    arr = string_ssplit(s, ":-");

    assert(strcmp(string_cstr(arr[0]), "a") == 0);
    assert(strcmp(string_cstr(arr[1]), "b") == 0);
    assert(strcmp(string_cstr(arr[2]), "c") == 0);
    assert(arr[3] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_ssplit_separator_not_present(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("abcdef");

    arr = string_ssplit(s, ",");

    assert(strcmp(string_cstr(arr[0]), "abcdef") == 0);
    assert(arr[1] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_ssplit_consecutive_delimiters(void)
{
    string_t *s;
    string_t **arr;

    s = string_new("a::b--c");

    arr = string_ssplit(s, ":-");

    assert(strcmp(string_cstr(arr[0]), "a") == 0);
    assert(strcmp(string_cstr(arr[1]), "") == 0);
    assert(strcmp(string_cstr(arr[2]), "b") == 0);
    assert(strcmp(string_cstr(arr[3]), "") == 0);
    assert(strcmp(string_cstr(arr[4]), "c") == 0);
    assert(arr[5] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_ssplit_delimiter_at_edges(void)
{
    string_t *s;
    string_t **arr;

    s = string_new(":abc-");

    arr = string_ssplit(s, ":-");

    assert(strcmp(string_cstr(arr[0]), "") == 0);
    assert(strcmp(string_cstr(arr[1]), "abc") == 0);
    assert(strcmp(string_cstr(arr[2]), "") == 0);
    assert(arr[3] == NULL);

    string_array_del(arr);
    string_delete(s);
}

static void test_array_clone_empty_array(void)
{
    string_t **arr1, **arr2;

    arr1 = string_array_new(1);

    arr2 = string_array_clone(arr1);

    assert(arr2 != NULL);
    assert(arr2 != arr1);
    assert(arr2[0] == NULL);

    string_array_del(arr1);
    string_array_del(arr2);
}

static void test_case_conversion_no_alpha(void)
{
    string_t *s;

    s = string_new("1234!@#$");

    string_tolower(s);

    assert(strcmp(string_cstr(s), "1234!@#$") == 0);

    string_toupper(s);

    assert(strcmp(string_cstr(s), "1234!@#$") == 0);

    string_delete(s);
}

/****************************************************************************
 * Main
 *****************************************************************************/

void test_string(void)
{
    void (*string_test_fns[])(void) = {
        test_string_new_delete,
        test_string_newn,
        test_string_newb,
        test_string_init,
        test_string_initn,
        test_string_initb,
        test_string_set,
        test_string_setn,
        test_string_setb,
        test_string_clone,
        test_string_swap,
        test_string_reserve,
        test_string_array_new_delete,
        test_string_array_clone,
        test_string_length,
        test_string_capacity,
        test_string_empty,
        test_string_data,
        test_string_at,
        test_string_tolower,
        test_string_toupper,
        test_string_compare,
        test_compare_prefix_lengths,
        test_compare_case_insensitive_partial,
        test_compare_negative_n,
        test_compare_empty_vs_nonempty,
        test_string_starts_with,
        test_string_ends_with,
        test_string_contains,
        test_string_find,
        test_contains_empty_substring,
        test_contains_overlapping_pattern,
        test_contains_at_end_boundary,
        test_string_rfind,
        test_string_substr,
        test_substr_full_string,
        test_substr_last_character,
        test_string_append,
        test_string_appendn,
        test_string_insert,
        test_string_insertn,
        test_replace_null_arguments,
        test_replace_n_zero_no_change,
        test_replace_substring_not_found,
        test_replace_replace_single_occurrence,
        test_replace_replace_multiple_occurrences,
        test_replace_replace_all_occurrences_with_negative_n,
        test_replace_replace_when_n_exceeds_occurrences,
        test_replace_replacement_shorter_than_substring,
        test_replace_replacement_longer_than_substring,
        test_replace_replace_with_empty_replacement,
        test_replace_empty_substring_and_empty_replacement,
        test_replace_empty_substring_insert_prefix_once,
        test_replace_empty_substring_insert_multiple_times,
        test_replace_empty_substring_with_negative_n,
        test_replace_overlapping_substrings,
        test_replace_occurrence_at_end,
        test_replace_occurrence_at_beginning,
        test_replace_entire_string,
        test_replace_with_same_string,
        test_replace_partial_overlap_not_revisited,
        test_replace_remove_entire_contents,
        test_replace_separator_style_pattern,
        test_remove_prefix,
        test_remove_suffix,
        test_trim,
        test_trim_leading_ws,
        test_trim_trailing_ws,
        test_string_join,
        test_join_single_element,
        test_join_empty_separator,
        test_join_with_empty_strings,
        test_string_split,
        test_split_separator_not_present,
        test_split_separator_at_beginning,
        test_split_separator_at_end,
        test_split_consecutive_separators,
        test_split_entire_string_as_separator,
        test_string_ssplit,
        test_ssplit_separator_not_present,
        test_ssplit_consecutive_delimiters,
        test_ssplit_delimiter_at_edges,
        test_array_clone_empty_array,
        test_case_conversion_no_alpha,
        NULL
    };

    int count;

    for (count = 0; string_test_fns[count] != NULL; count++) {
        string_test_fns[count]();
    }

    printf("All %d string_t tests passed\n", count);
}
