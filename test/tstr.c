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

/*****************************************************************************
 * Main
 *****************************************************************************/

void test_str (void)
{
    test_str_new_del();
    test_str_clone();
    test_str_array_new_del();
    test_str_array_clone();
    test_str_length();
    test_str_tolower();
    test_str_toupper();
    test_str_compare();
    test_str_contains();
    test_str_substr();
    test_str_join();
    test_str_split();
    test_str_ssplit();

    printf("All str_t tests passed\n");
}