/*****************************************************************************
 *
 * FILE NAME     : test_str.c
 *
 * DESCRIPTION   : Unit tests for str_t
 *
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "kmrUtils/str.h"

/*****************************************************************************
 * Helpers
 *****************************************************************************/

static void test_str_new_del(void)
{
    str_t *s;

    s = str_new(NULL);
    assert(s == NULL);

    s = str_new("");
    assert(s == empty_str());
    assert(str_length(s) == 0);

    s = str_new("hello");
    assert(s != NULL);
    assert(s != empty_str());
    assert(str_length(s) == 5);

    str_del(s);

    /* Should be harmless */
    assert(str_del(NULL) == NULL);

    /* Should not free static empty string */
    assert(str_del(empty_str()) == NULL);
}

static void test_str_clone(void)
{
    str_t *s1, *s2;

    assert(str_clone(NULL) == NULL);

    s1 = empty_str();
    s2 = str_clone(s1);

    assert(s2 == empty_str());

    s1 = str_new("hello");
    s2 = str_clone(s1);

    assert(s2 != NULL);
    assert(s2 != s1);
    assert(str_compare(s1, s2, -1, false) == 0);

    str_del(s1);
    str_del(s2);
}

static void test_str_array_new_del(void)
{
    str_t **arr;

    assert(str_array_new(0) == NULL);
    assert(str_array_new(-1) == NULL);

    arr = str_array_new(3);

    assert(arr != NULL);
    assert(arr[0] == NULL);
    assert(arr[1] == NULL);
    assert(arr[2] == NULL);

    assert(str_array_del(NULL) == NULL);
    assert(str_array_del(arr) == NULL);
}

static void test_str_array_clone(void)
{
    str_t **arr1, **arr2;

    assert(str_array_clone(NULL) == NULL);

    arr1 = str_array_new(3);

    arr1[0] = str_new("abc");
    arr1[1] = str_new("def");
    arr1[2] = NULL;

    arr2 = str_array_clone(arr1);

    assert(arr2 != NULL);
    assert(arr2 != arr1);

    assert(str_compare(arr1[0], arr2[0], -1, false) == 0);
    assert(str_compare(arr1[1], arr2[1], -1, false) == 0);

    assert(arr1[0] != arr2[0]);
    assert(arr1[1] != arr2[1]);

    str_array_del(arr1);
    str_array_del(arr2);
}

static void test_str_length(void)
{
    str_t *s;

    assert(str_length(NULL) == 0);

    s = str_new("abcdef");

    assert(str_length(s) == 6);

    str_del(s);
}

static void test_str_tolower(void)
{
    str_t *s1, *s2, *expected;

    assert(str_tolower(NULL) == NULL);

    s1 = empty_str();
    s2 = str_tolower(s1);

    assert(s2 == empty_str());

    s1 = str_new("AbC123");
    expected = str_new("abc123");

    s2 = str_tolower(s1);

    assert(str_compare(s2, expected, -1, false) == 0);

    str_del(s1);
    str_del(s2);
    str_del(expected);
}

static void test_str_toupper(void)
{
    str_t *s1, *s2, *expected;

    assert(str_toupper(NULL) == NULL);

    s1 = empty_str();
    s2 = str_toupper(s1);

    assert(s2 == empty_str());

    s1 = str_new("AbC123");
    expected = str_new("ABC123");

    s2 = str_toupper(s1);

    assert(str_compare(s2, expected, -1, false) == 0);

    str_del(s1);
    str_del(s2);
    str_del(expected);
}

static void test_str_compare(void)
{
    str_t *a, *b;

    assert(str_compare(NULL, NULL, -1, false) == 0);

    a = str_new("abc");

    assert(str_compare(a, NULL, -1, false) == 1);
    assert(str_compare(NULL, a, -1, false) == -1);

    b = str_new("abcd");

    assert(str_compare(a, b, -1, false) < 0);
    assert(str_compare(b, a, -1, false) > 0);

    str_del(b);

    b = str_new("abd");

    assert(str_compare(a, b, -1, false) < 0);
    assert(str_compare(b, a, -1, false) > 0);

    str_del(b);

    b = str_new("ABC");

    assert(str_compare(a, b, -1, true) == 0);
    assert(str_compare(a, b, -1, false) > 0);

    /* n == 0 special case */
    assert(str_compare(a, b, 0, false) == 0);

    /* partial compare */
    b = str_del(b);
    b = str_new("abz");

    assert(str_compare(a, b, 2, false) == 0);

    str_del(a);
    str_del(b);
}

static void test_str_contains(void)
{
    str_t *s;

    s = str_new("hello world");

    assert(str_contains(NULL, 0, "hello") == -1);
    assert(str_contains(s, -1, "hello") == -1);
    assert(str_contains(s, 100, "hello") == -1);
    assert(str_contains(s, 0, NULL) == -1);

    assert(str_contains(s, 0, "hello") == 0);
    assert(str_contains(s, 0, "world") == 6);
    assert(str_contains(s, 3, "world") == 6);

    assert(str_contains(s, 0, "xyz") == -1);

    str_del(s);
}

static void test_str_substr(void)
{
    str_t *s, *sub, *expected;

    assert(str_substr(NULL, 0, 1) == NULL);

    s = str_new("abcdef");

    assert(str_substr(s, -1, 1) == NULL);
    assert(str_substr(s, 100, 1) == NULL);

    sub = str_substr(s, 0, 0);

    assert(sub == empty_str());

    sub = str_substr(s, 2, 2);
    expected = str_new("cd");

    assert(str_compare(sub, expected, -1, false) == 0);

    str_del(sub);
    str_del(expected);

    /* n < 0 */
    sub = str_substr(s, 2, -1);
    expected = str_new("cdef");

    assert(str_compare(sub, expected, -1, false) == 0);

    str_del(sub);
    str_del(expected);

    /* n exceeds bounds */
    sub = str_substr(s, 4, 100);
    expected = str_new("ef");

    assert(str_compare(sub, expected, -1, false) == 0);

    str_del(sub);
    str_del(expected);

    str_del(s);
}

static void test_str_join(void)
{
    str_t **arr;
    str_t *joined, *expected;

    assert(str_join(NULL, ",") == NULL);

    arr = str_array_new(1);

    assert(str_join(arr, NULL) == NULL);

    joined = str_join(arr, ",");

    assert(joined == empty_str());

    str_array_del(arr);

    arr = str_array_new(4);

    arr[0] = str_new("a");
    arr[1] = str_new("bb");
    arr[2] = str_new("ccc");
    arr[3] = NULL;

    joined = str_join(arr, "-");
    expected = str_new("a-bb-ccc");

    assert(str_compare(joined, expected, -1, false) == 0);

    str_del(joined);
    str_del(expected);
    str_array_del(arr);
}

static void test_str_split(void)
{
    str_t *s;
    str_t **arr;

    assert(str_split(NULL, ",") == NULL);

    s = str_new("abc");

    assert(str_split(s, NULL) == NULL);

    str_del(s);

    /* empty string */
    arr = str_split(empty_str(), ",");

    assert(arr != NULL);
    assert(arr[0] == NULL);

    str_array_del(arr);

    /* split into chars */
    s = str_new("abc");

    arr = str_split(s, "");

    assert(arr != NULL);

    assert(str_compare(arr[0], str_new("a"), -1, false) == 0);
    assert(str_compare(arr[1], str_new("b"), -1, false) == 0);
    assert(str_compare(arr[2], str_new("c"), -1, false) == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);

    /* normal split */
    arr = str_split(s, "b");

    assert(arr != NULL);

    assert(str_compare(arr[0], str_new("a"), -1, false) == 0);
    assert(str_compare(arr[1], str_new("c"), -1, false) == 0);
    assert(arr[2] == NULL);

    str_array_del(arr);

    /* multi-char separator */
    str_del(s);

    s = str_new("a:-b:-c");

    arr = str_split(s, ":-");

    assert(arr != NULL);

    assert(str_compare(arr[0], str_new("a"), -1, false) == 0);
    assert(str_compare(arr[1], str_new("b"), -1, false) == 0);
    assert(str_compare(arr[2], str_new("c"), -1, false) == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_str_ssplit(void)
{
    str_t *s;
    str_t **arr;

    assert(str_ssplit(NULL, ",") == NULL);

    s = str_new("abc");

    assert(str_ssplit(s, NULL) == NULL);

    str_del(s);

    /* empty string */
    arr = str_ssplit(empty_str(), ",");

    assert(arr != NULL);
    assert(arr[0] == NULL);

    str_array_del(arr);

    /* split into chars */
    s = str_new("abc");

    arr = str_ssplit(s, "");

    assert(arr != NULL);

    assert(str_compare(arr[0], str_new("a"), -1, false) == 0);
    assert(str_compare(arr[1], str_new("b"), -1, false) == 0);
    assert(str_compare(arr[2], str_new("c"), -1, false) == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);

    /* char-set split */
    str_del(s);

    s = str_new("a:b-c");

    arr = str_ssplit(s, ":-");

    assert(arr != NULL);

    assert(str_compare(arr[0], str_new("a"), -1, false) == 0);
    assert(str_compare(arr[1], str_new("b"), -1, false) == 0);
    assert(str_compare(arr[2], str_new("c"), -1, false) == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_replace_null_arguments(void)
{
    str_t *s = str_new("hello");

    assert(str_replace(NULL, "l", "x", 1) == NULL);
    assert(str_replace(s, NULL, "x", 1) == NULL);
    assert(str_replace(s, "l", NULL, 1) == NULL);

    str_del(s);
}

static void test_replace_empty_string_singleton(void)
{
    str_t *r = str_replace(empty_str(), "a", "b", 1);

    assert(r == empty_str());
}

static void test_replace_n_zero_returns_clone(void)
{
    str_t *s = str_new("hello world");
    str_t *r = str_replace(s, "world", "earth", 0);

    assert(r != NULL);
    assert(r != s);
    assert(strcmp(str_cstr(r), "hello world") == 0);
    assert(str_length(r) == strlen("hello world"));

    str_del(s);
    str_del(r);
}

static void test_replace_substring_not_found(void)
{
    str_t *s = str_new("abcdef");
    str_t *r = str_replace(s, "xyz", "123", 5);

    assert(r != NULL);
    assert(r != s);
    assert(strcmp(str_cstr(r), "abcdef") == 0);
    assert(str_length(r) == 6);

    str_del(s);
    str_del(r);
}

static void test_replace_replace_single_occurrence(void)
{
    str_t *s = str_new("abc abc abc");
    str_t *r = str_replace(s, "abc", "xyz", 1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "xyz abc abc") == 0);
    assert(str_length(r) == strlen("xyz abc abc"));

    str_del(s);
    str_del(r);
}

static void test_replace_replace_multiple_occurrences(void)
{
    str_t *s = str_new("abc abc abc");
    str_t *r = str_replace(s, "abc", "xyz", 2);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "xyz xyz abc") == 0);
    assert(str_length(r) == strlen("xyz xyz abc"));

    str_del(s);
    str_del(r);
}

static void test_replace_replace_all_occurrences_with_negative_n(void)
{
    str_t *s = str_new("one two one two one");
    str_t *r = str_replace(s, "one", "1", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "1 two 1 two 1") == 0);
    assert(str_length(r) == strlen("1 two 1 two 1"));

    str_del(s);
    str_del(r);
}

static void test_replace_replace_when_n_exceeds_occurrences(void)
{
    str_t *s = str_new("cat dog cat");
    str_t *r = str_replace(s, "cat", "fox", 10);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "fox dog fox") == 0);
    assert(str_length(r) == strlen("fox dog fox"));

    str_del(s);
    str_del(r);
}

static void test_replace_replacement_shorter_than_substring(void)
{
    str_t *s = str_new("aaaaaa");
    str_t *r = str_replace(s, "aa", "b", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "bbb") == 0);
    assert(str_length(r) == 3);

    str_del(s);
    str_del(r);
}

static void test_replace_replacement_longer_than_substring(void)
{
    str_t *s = str_new("xx yy xx");
    str_t *r = str_replace(s, "xx", "LONG", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "LONG yy LONG") == 0);
    assert(str_length(r) == strlen("LONG yy LONG"));

    str_del(s);
    str_del(r);
}

static void test_replace_replace_with_empty_replacement(void)
{
    str_t *s = str_new("abc123abc123");
    str_t *r = str_replace(s, "123", "", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "abcabc") == 0);
    assert(str_length(r) == 6);

    str_del(s);
    str_del(r);
}

static void test_replace_empty_substring_and_empty_replacement(void)
{
    str_t *s = str_new("hello");
    str_t *r = str_replace(s, "", "", 5);

    assert(r != NULL);
    assert(r != s);
    assert(strcmp(str_cstr(r), "hello") == 0);
    assert(str_length(r) == 5);

    str_del(s);
    str_del(r);
}

static void test_replace_empty_substring_insert_prefix_once(void)
{
    str_t *s = str_new("abc");
    str_t *r = str_replace(s, "", "X", 1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "Xabc") == 0);
    assert(str_length(r) == 4);

    str_del(s);
    str_del(r);
}

static void test_replace_empty_substring_insert_multiple_times(void)
{
    str_t *s = str_new("abc");
    str_t *r = str_replace(s, "", "-", 4);

    /*
     * Current implementation behavior:
     * "-a-b-c-"
     */
    assert(r != NULL);
    assert(strcmp(str_cstr(r), "-a-b-c-") == 0);
    assert(str_length(r) == strlen("-a-b-c-"));

    str_del(s);
    str_del(r);
}

static void test_replace_empty_substring_with_negative_n(void)
{
    str_t *s = str_new("abc");
    str_t *r = str_replace(s, "", "+", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "+a+b+c+") == 0);
    assert(str_length(r) == strlen("+a+b+c+"));

    str_del(s);
    str_del(r);
}

static void test_replace_empty_substring_n_exceeds_length_plus_one(void)
{
    str_t *s = str_new("ab");
    str_t *r = str_replace(s, "", "Z", 100);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "ZaZbZ") == 0);
    assert(str_length(r) == strlen("ZaZbZ"));

    str_del(s);
    str_del(r);
}

static void test_replace_overlapping_substrings(void)
{
    str_t *s = str_new("aaaaa");
    str_t *r = str_replace(s, "aa", "X", -1);

    /*
     * Non-overlapping replacement behavior.
     * Matches at positions 0 and 2.
     */
    assert(r != NULL);
    assert(strcmp(str_cstr(r), "XXa") == 0);
    assert(str_length(r) == 3);

    str_del(s);
    str_del(r);
}

/*****************************************************************************
 * Additional semantic and regression-oriented tests for str_replace()
 *****************************************************************************/

static void test_replace_empty_substring_zero_length_input(void)
{
    str_t *r;

    r = str_replace(empty_str(), "", "X", -1);

    /*
     * Empty string has exactly one empty-substring position.
     */
    assert(r == empty_str());

    /*
     * Current API special-cases empty_str().
     * This test mainly ensures no crash/regression.
     */
}

static void test_replace_exact_occurrence_count(void)
{
    str_t *s, *r;

    s = str_new("x x x");

    /*
     * Exactly two replacements.
     */
    r = str_replace(s, "x", "y", 2);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "y y x") == 0);
    assert(str_length(r) == 5);

    str_del(r);
    str_del(s);
}

static void test_replace_occurrence_at_end(void)
{
    str_t *s, *r;

    s = str_new("abc123");

    r = str_replace(s, "123", "XYZ", 1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "abcXYZ") == 0);
    assert(str_length(r) == 6);

    str_del(r);
    str_del(s);
}

static void test_replace_occurrence_at_beginning(void)
{
    str_t *s, *r;

    s = str_new("123abc");

    r = str_replace(s, "123", "XYZ", 1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "XYZabc") == 0);
    assert(str_length(r) == 6);

    str_del(r);
    str_del(s);
}

static void test_replace_entire_string(void)
{
    str_t *s, *r;

    s = str_new("abcdef");

    r = str_replace(s, "abcdef", "X", 1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "X") == 0);
    assert(str_length(r) == 1);

    str_del(r);
    str_del(s);
}

static void test_replace_with_same_string(void)
{
    str_t *s, *r;

    s = str_new("abc abc");

    r = str_replace(s, "abc", "abc", -1);

    /*
     * Semantic correctness:
     * result content must remain identical.
     */
    assert(r != NULL);
    assert(strcmp(str_cstr(r), "abc abc") == 0);
    assert(str_length(r) == 7);

    /*
     * Should still be a distinct allocation.
     */
    assert(r != s);

    str_del(r);
    str_del(s);
}

static void test_replace_partial_overlap_not_revisited(void)
{
    str_t *s, *r;

    s = str_new("abababa");

    /*
     * Non-overlapping search progression.
     *
     * Replace "aba":
     * matches at positions 0 and 4.
     */
    r = str_replace(s, "aba", "X", -1);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "XbX") == 0);

    str_del(r);
    str_del(s);
}

static void test_replace_empty_substring_two_insertions(void)
{
    str_t *s, *r;

    s = str_new("abc");

    /*
     * Replace first two empty-substring positions.
     *
     * Expected:
     * "_a_bc"
     */
    r = str_replace(s, "", "_", 2);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "_a_bc") == 0);
    assert(str_length(r) == 5);

    str_del(r);
    str_del(s);
}

static void test_replace_empty_substring_middle_behavior(void)
{
    str_t *s, *r;

    s = str_new("abc");

    /*
     * Three insertion points:
     * before a
     * between a/b
     * between b/c
     */
    r = str_replace(s, "", ".", 3);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), ".a.b.c") == 0);
    assert(str_length(r) == 6);

    str_del(r);
    str_del(s);
}

static void test_replace_remove_entire_contents(void)
{
    str_t *s, *r;

    s = str_new("xxxx");

    r = str_replace(s, "x", "", -1);

    /*
     * All content removed.
     */
    assert(r != NULL);

    /*
     * Current implementation returns allocated empty string,
     * not necessarily empty_str().
     *
     * Validate semantics only.
     */
    assert(strcmp(str_cstr(r), "") == 0);
    assert(str_length(r) == 0);

    str_del(r);
    str_del(s);
}

static void test_replace_separator_style_pattern(void)
{
    str_t *s, *r;

    s = str_new("a,b,c,d");

    r = str_replace(s, ",", "::", 2);

    assert(r != NULL);
    assert(strcmp(str_cstr(r), "a::b::c,d") == 0);

    str_del(r);
    str_del(s);
}

static void test_replace_no_mutation_of_original(void)
{
    str_t *s, *r;

    s = str_new("hello world");

    r = str_replace(s, "world", "earth", 1);

    assert(r != NULL);

    /*
     * Original string must remain unchanged.
     */
    assert(strcmp(str_cstr(s), "hello world") == 0);

    /*
     * Returned string must contain replacement.
     */
    assert(strcmp(str_cstr(r), "hello earth") == 0);

    str_del(r);
    str_del(s);
}

static void test_compare_prefix_lengths(void)
{
    str_t *a, *b;

    a = str_new("abcdef");
    b = str_new("abcxyz");

    /*
     * First 3 chars identical.
     */
    assert(str_compare(a, b, 3, false) == 0);

    /*
     * First 4 chars differ.
     */
    assert(str_compare(a, b, 4, false) < 0);

    str_del(a);
    str_del(b);
}

static void test_compare_case_insensitive_partial(void)
{
    str_t *a, *b;

    a = str_new("AbCdEf");
    b = str_new("abcdef");

    assert(str_compare(a, b, 6, true) == 0);
    assert(str_compare(a, b, 6, false) != 0);

    str_del(a);
    str_del(b);
}

static void test_compare_negative_n(void)
{
    str_t *a, *b;

    a = str_new("abc");
    b = str_new("abc");

    /*
     * Negative n means compare entire strings.
     */
    assert(str_compare(a, b, -1, false) == 0);

    str_del(a);
    str_del(b);
}

static void test_compare_empty_strings(void)
{
    assert(str_compare(empty_str(), empty_str(), -1, false) == 0);
}

static void test_compare_empty_vs_nonempty(void)
{
    str_t *s;

    s = str_new("x");

    assert(str_compare(empty_str(), s, -1, false) < 0);
    assert(str_compare(s, empty_str(), -1, false) > 0);

    str_del(s);
}

static void test_contains_empty_substring(void)
{
    str_t *s;

    s = str_new("abcdef");

    /*
     * strstr() semantics:
     * empty substring matches immediately.
     */
    assert(str_contains(s, 0, "") == 0);
    assert(str_contains(s, 3, "") == 3);
    assert(str_contains(s, 6, "") == 6);

    str_del(s);
}

static void test_contains_overlapping_pattern(void)
{
    str_t *s;

    s = str_new("aaaaa");

    assert(str_contains(s, 0, "aa") == 0);
    assert(str_contains(s, 1, "aa") == 1);
    assert(str_contains(s, 2, "aa") == 2);
    assert(str_contains(s, 3, "aa") == 3);

    str_del(s);
}

static void test_contains_at_end_boundary(void)
{
    str_t *s;

    s = str_new("abcdef");

    assert(str_contains(s, 5, "f") == 5);
    assert(str_contains(s, 6, "") == 6);
    assert(str_contains(s, 6, "x") == -1);

    str_del(s);
}

static void test_substr_full_string(void)
{
    str_t *s, *sub;

    s = str_new("abcdef");

    sub = str_substr(s, 0, -1);

    assert(strcmp(str_cstr(sub), "abcdef") == 0);
    assert(sub != s);

    str_del(sub);
    str_del(s);
}

static void test_substr_empty_from_end(void)
{
    str_t *s, *sub;

    s = str_new("abcdef");

    sub = str_substr(s, 6, 0);

    assert(sub == empty_str());

    str_del(s);
}

static void test_substr_last_character(void)
{
    str_t *s, *sub;

    s = str_new("abcdef");

    sub = str_substr(s, 5, 1);

    assert(strcmp(str_cstr(sub), "f") == 0);
    assert(str_length(sub) == 1);

    str_del(sub);
    str_del(s);
}

static void test_join_single_element(void)
{
    str_t **arr;
    str_t *joined;

    arr = str_array_new(2);

    arr[0] = str_new("only");
    arr[1] = NULL;

    joined = str_join(arr, ",");

    assert(strcmp(str_cstr(joined), "only") == 0);
    assert(str_length(joined) == 4);

    str_del(joined);
    str_array_del(arr);
}

static void test_join_empty_separator(void)
{
    str_t **arr;
    str_t *joined;

    arr = str_array_new(4);

    arr[0] = str_new("ab");
    arr[1] = str_new("cd");
    arr[2] = str_new("ef");
    arr[3] = NULL;

    joined = str_join(arr, "");

    assert(strcmp(str_cstr(joined), "abcdef") == 0);

    str_del(joined);
    str_array_del(arr);
}

static void test_join_with_empty_strings(void)
{
    str_t **arr;
    str_t *joined;

    arr = str_array_new(4);

    arr[0] = empty_str();
    arr[1] = str_new("x");
    arr[2] = empty_str();
    arr[3] = NULL;

    joined = str_join(arr, ":");
    
    assert(strcmp(str_cstr(joined), ":x:") == 0);

    str_del(joined);
    str_array_del(arr);
}

static void test_split_separator_not_present(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("abcdef");

    arr = str_split(s, ",");

    assert(arr != NULL);
    assert(strcmp(str_cstr(arr[0]), "abcdef") == 0);
    assert(arr[1] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_split_separator_at_beginning(void)
{
    str_t *s;
    str_t **arr;

    s = str_new(",abc");

    arr = str_split(s, ",");

    assert(strcmp(str_cstr(arr[0]), "") == 0);
    assert(strcmp(str_cstr(arr[1]), "abc") == 0);
    assert(arr[2] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_split_separator_at_end(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("abc,");

    arr = str_split(s, ",");

    assert(strcmp(str_cstr(arr[0]), "abc") == 0);
    assert(strcmp(str_cstr(arr[1]), "") == 0);
    assert(arr[2] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_split_consecutive_separators(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("a,,b");

    arr = str_split(s, ",");

    assert(strcmp(str_cstr(arr[0]), "a") == 0);
    assert(strcmp(str_cstr(arr[1]), "") == 0);
    assert(strcmp(str_cstr(arr[2]), "b") == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_split_entire_string_as_separator(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("XYZ");

    arr = str_split(s, "XYZ");

    assert(strcmp(str_cstr(arr[0]), "") == 0);
    assert(strcmp(str_cstr(arr[1]), "") == 0);
    assert(arr[2] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_ssplit_separator_not_present(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("abcdef");

    arr = str_ssplit(s, ",");

    assert(strcmp(str_cstr(arr[0]), "abcdef") == 0);
    assert(arr[1] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_ssplit_consecutive_delimiters(void)
{
    str_t *s;
    str_t **arr;

    s = str_new("a::b--c");

    arr = str_ssplit(s, ":-");

    assert(strcmp(str_cstr(arr[0]), "a") == 0);
    assert(strcmp(str_cstr(arr[1]), "") == 0);
    assert(strcmp(str_cstr(arr[2]), "b") == 0);
    assert(strcmp(str_cstr(arr[3]), "") == 0);
    assert(strcmp(str_cstr(arr[4]), "c") == 0);
    assert(arr[5] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_ssplit_delimiter_at_edges(void)
{
    str_t *s;
    str_t **arr;

    s = str_new(":abc-");

    arr = str_ssplit(s, ":-");

    assert(strcmp(str_cstr(arr[0]), "") == 0);
    assert(strcmp(str_cstr(arr[1]), "abc") == 0);
    assert(strcmp(str_cstr(arr[2]), "") == 0);
    assert(arr[3] == NULL);

    str_array_del(arr);
    str_del(s);
}

static void test_array_clone_empty_array(void)
{
    str_t **arr1, **arr2;

    arr1 = str_array_new(1);

    arr2 = str_array_clone(arr1);

    assert(arr2 != NULL);
    assert(arr2 != arr1);
    assert(arr2[0] == NULL);

    str_array_del(arr1);
    str_array_del(arr2);
}

static void test_case_conversion_no_alpha(void)
{
    str_t *s, *lower, *upper;

    s = str_new("1234!@#$");

    lower = str_tolower(s);
    upper = str_toupper(s);

    assert(strcmp(str_cstr(lower), "1234!@#$") == 0);
    assert(strcmp(str_cstr(upper), "1234!@#$") == 0);

    str_del(lower);
    str_del(upper);
    str_del(s);
}


/*****************************************************************************
 * Main
 *****************************************************************************/

void test_str (void)
{
    void (*str_test_fns[])(void) = {
        test_str_new_del,
        test_str_clone,
        test_str_array_new_del,
        test_str_array_clone,
        test_str_length,
        test_str_tolower,
        test_str_toupper,
        test_str_compare,
        test_str_contains,
        test_str_substr,
        test_str_join,
        test_str_split,
        test_str_ssplit,
        test_replace_null_arguments,
        test_replace_empty_string_singleton,
        test_replace_n_zero_returns_clone,
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
        test_replace_empty_substring_n_exceeds_length_plus_one,
        test_replace_overlapping_substrings,
        test_replace_empty_substring_zero_length_input,
        test_replace_exact_occurrence_count,
        test_replace_occurrence_at_end,
        test_replace_occurrence_at_beginning,
        test_replace_entire_string,
        test_replace_with_same_string,
        test_replace_partial_overlap_not_revisited,
        test_replace_empty_substring_two_insertions,
        test_replace_empty_substring_middle_behavior,
        test_replace_remove_entire_contents,
        test_replace_separator_style_pattern,
        test_replace_no_mutation_of_original,
        test_compare_prefix_lengths,
        test_compare_case_insensitive_partial,
        test_compare_negative_n,
        test_compare_empty_strings,
        test_compare_empty_vs_nonempty,
        test_contains_empty_substring,
        test_contains_overlapping_pattern,
        test_contains_at_end_boundary,
        test_substr_full_string,
        test_substr_empty_from_end,
        test_substr_last_character,
        test_join_single_element,
        test_join_empty_separator,
        test_join_with_empty_strings,
        test_split_separator_not_present,
        test_split_separator_at_beginning,
        test_split_separator_at_end,
        test_split_consecutive_separators,
        test_split_entire_string_as_separator,
        test_ssplit_separator_not_present,
        test_ssplit_consecutive_delimiters,
        test_ssplit_delimiter_at_edges,
        test_array_clone_empty_array,
        test_case_conversion_no_alpha,
        NULL
    };
    int count;

    for (count = 0; str_test_fns[count] != NULL; count++) {
        str_test_fns[count]();
    }

    printf("All %d str_t tests passed\n", count);
}