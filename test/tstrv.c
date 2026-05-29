#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "kmrUtils/strv.h"

/* Simple test framework macros */
static int tests_run = 0;
static int tests_failed = 0;

#define assert_true(expr, msg) do { \
    if (!(expr)) { \
        printf("  [FAIL] %s:%d: %s\n", __FILE__, __LINE__, msg); \
        tests_failed++; \
        return; \
    } \
} while (0)

#define assert_int_eq(expected, actual, msg) do { \
    long e = (expected); \
    long a = (actual); \
    if (e != a) { \
        printf("  [FAIL] %s:%d: %s (Expected %ld, Got %ld)\n", __FILE__, __LINE__, msg, e, a); \
        tests_failed++; \
        return; \
    } \
} while (0)

#define assert_ptr_eq(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        printf("  [FAIL] %s:%d: %s (Expected %p, Got %p)\n", __FILE__, __LINE__, msg, (void*)(expected), (void*)(actual)); \
        tests_failed++; \
        return; \
    } \
} while (0)

#define RUN_TEST(test_func) do { \
    printf("Running %s...\n", #test_func); \
    tests_run++; \
    test_func(); \
} while (0)


/*****************************************************************************
 * 1. Tests for Initialization Families: stringv_setb (and macros set/setn)
 *****************************************************************************/
static void test_stringv_set_positive(void) {
    stringv_t sv = {NULL, 0};
    char text[] = "Hello World";
    
    // Test base macro wrapper stringv_set
    stringv_set(&sv, text);
    assert_ptr_eq(text, sv.s, "stringv_set pointing to original string address");
    assert_int_eq(11, sv.slen, "stringv_set auto-calculated full string length");
}

static void test_stringv_setn_edge_and_positive(void) {
    stringv_t sv = {NULL, 0};
    char text[] = "Hello World";
    
    // Positive bounded copy
    stringv_setn(&sv, text, 5);
    assert_ptr_eq(text, sv.s, "stringv_setn updates structural data base");
    assert_int_eq(5, sv.slen, "stringv_setn properly restricted explicit length window");
    
    // Edge case: length of 0
    stringv_setn(&sv, text, 0);
    assert_int_eq(0, sv.slen, "Length of 0 should update view safely to 0 size");
}

static void test_stringv_setb_bounds_and_negative(void) {
    stringv_t sv = { (char*)0xDEADBEEF, 999 }; // Sentinel values
    char text[] = "Hello World";

    // Positive case: starting at an offset pos
    stringv_setb(&sv, text, 6, 5);
    assert_ptr_eq(text + 6, sv.s, "Base pointer should offset correctly by pos");
    assert_int_eq(5, sv.slen, "Length should reflect the explicit n tracking window");

    // Negative case: No-Op on NULL structural destination
    stringv_setb(NULL, text, 0, 5); // Should not crash
    
    // Negative case: No-Op if source string pointer is NULL
    stringv_setb(&sv, NULL, 0, 5);
    assert_ptr_eq(text + 6, sv.s, "Structure must remain unmutated on invalid source parameter");
    assert_int_eq(5, sv.slen, "Structure length must remain unmutated on invalid source parameter");
}


/*****************************************************************************
 * 2. Tests for stringv_length
 *****************************************************************************/
static void test_stringv_length_cases(void) {
    stringv_t sv = {"Test", 4};
    assert_int_eq(4, stringv_length(&sv), "Valid structural tracking reading match");
    
    // Negative Case: NULL handle guard check
    assert_int_eq(-1, stringv_length(NULL), "NULL context parameter returns baseline length -1");
}


/*****************************************************************************
 * 3. Tests for stringv_compare
 *****************************************************************************/
static void test_stringv_compare_null_handling(void) {
    stringv_t sv = {"ABC", 3};
    
    // Documented rule requirements checked:
    assert_int_eq(-1, stringv_compare(NULL, &sv, 0, false), "str1 = NULL, str2 != NULL returns -1");
    assert_int_eq(1, stringv_compare(&sv, NULL, 0, false), "str1 != NULL, str2 = NULL returns +1");
    assert_int_eq(0, stringv_compare(NULL, NULL, 0, false), "Both arguments NULL returns 0");
}

static void test_stringv_compare_length_vs_ascii(void) {
    // Note: Documentation specifies comparison rules prioritizes LENGTH difference over ASCII sequence value
    char a[] = "A";   // length 1
    char bb[] = "BB"; // length 2
    char cc[] = "CC"; // length 2
    
    stringv_t sv_a = {a, 1};
    stringv_t sv_bb = {bb, 2};
    stringv_t sv_cc = {cc, 2};
    
    // Length wins condition check
    assert_true(stringv_compare(&sv_bb, &sv_a, -1, false) > 0, "Longer string is treated as greater, even if ASCII matches later");
    assert_true(stringv_compare(&sv_a, &sv_bb, -1, false) < 0, "Shorter string is treated as smaller");
    
    // Equal length drops to ASCII sorting check
    assert_true(stringv_compare(&sv_bb, &sv_cc, -1, false) < 0, "Equal lengths fallback to standard character byte comparison sorting");
    assert_int_eq(0, stringv_compare(&sv_bb, &sv_bb, -1, false), "Identical memory bounds structures match exactly");
}

static void test_stringv_compare_bounded_and_case_insensitive(void) {
    char s1[] = "Apple";
    char s2[] = "APPLE";
    stringv_t sv1 = {s1, 5};
    stringv_t sv2 = {s2, 5};
    
    // Case sensitivity checks
    assert_true(stringv_compare(&sv1, &sv2, -1, false) != 0, "Case-sensitive comparison detects delta casing matching");
    assert_int_eq(0, stringv_compare(&sv1, &sv2, -1, true), "Case-insensitive comparison bypasses variant casing profiles");
    
    // Bounded subset matching check via variable n parameter
    char alt[] = "Applet";
    stringv_t sv_alt = {alt, 6};
    assert_int_eq(0, stringv_compare(&sv1, &sv_alt, 3, false), "Restricting inspection window parameter matching bounded subset arrays ('App' matches)");
}


/*****************************************************************************
 * 4. Tests for stringv_starts_with / stringv_ends_with
 *****************************************************************************/
static void test_stringv_affixes(void) {
    char txt[] = "GatewayFirewall";
    stringv_t sv = {txt, 15};
    
    // Positive Functional Checks
    assert_true(stringv_starts_with(&sv, "Gateway"), "Starts-with affirmative matching tracking logic");
    assert_true(stringv_ends_with(&sv, "Firewall"), "Ends-with affirmative matching tracking logic");
    
    // Edge Cases / False checks
    assert_true(!stringv_starts_with(&sv, "Way"), "Prefix string misalignment must fail correctly");
    assert_true(!stringv_ends_with(&sv, "Gate"), "Suffix string misalignment must fail correctly");
    
    // Negative Parameter bounds guards
    assert_true(!stringv_starts_with(NULL, "Gateway"), "NULL structural reference returns false natively");
    assert_true(!stringv_starts_with(&sv, NULL), "NULL target parameter evaluation safety fallback protection");
}


/*****************************************************************************
 * 5. Tests for stringv_contains
 *****************************************************************************/
static void test_stringv_contains_cases(void) {
    char payload[] = "{" "\"key\"" ":" "\"value\"" "}"; // JSON structural simulation array 
    stringv_t sv = {payload, 15};
    
    // Positive structural index finding
    assert_int_eq(2, stringv_find_first(&sv, "key"), "Valid sequence indexing position target located accurately");
    assert_int_eq(8, stringv_find(&sv, 3, "value"), "Offset position index scanning window skips past early data sections");
    
    // Edge/Negative cases
    assert_true(!stringv_contains(&sv, "missing"), "Returns false when search target string sequence cannot be resolved");
    assert_true(!stringv_contains(NULL, "key"), "NULL instance configuration structures return false baseline index seamlessly");
}


/*****************************************************************************
 * 6. Tests for stringv_substr
 *****************************************************************************/
static void test_stringv_substring_extraction(void) {
    char data[] = "CambiumNetworks";
    stringv_t in = {data, 15};
    stringv_t out = {NULL, 0};
    
    // Positive bounded extraction
    stringv_substr(&in, 7, 7, &out);
    assert_ptr_eq(data + 7, out.s, "Extracted pointer boundary targets adjusted slice offset base location");
    assert_int_eq(7, out.slen, "Extracted layout slice window tracked size perfectly");
    
    // Documented Rule: If n < 0 or pos + n exceeds length, return remaining remainder sequence tail
    stringv_substr(&in, 7, -1, &out);
    assert_int_eq(8, out.slen, "Negative length requests fall over to slice maximum remainder content length automatically");
    
    stringv_substr(&in, 7, 20, &out);
    assert_int_eq(8, out.slen, "Out-of-bounds size parameters clamp down to preserve existing maximum string limits");

    // Edge case: n = 0 results in an empty string view
    stringv_substr(&in, 2, 0, &out);
    assert_int_eq(0, out.slen, "Requesting 0 length cleanly creates an empty string_view context frame");
}


/*****************************************************************************
 * 7. Tests for stringv_remove_prefix / stringv_remove_suffix
 *****************************************************************************/
static void test_stringv_affix_removal(void) {
    char raw[] = "PrefixDataSuffix";
    stringv_t sv = {raw, 16};
    
    // Positive Functional shrinkage checks
    stringv_remove_prefix(&sv, 6); // Drops "Prefix"
    assert_ptr_eq(raw + 6, sv.s, "Prefix pointer window shifted directly forward past target byte size offset");
    assert_int_eq(10, sv.slen, "Internal view length decreased on prefix calculation correctly");
    
    stringv_remove_suffix(&sv, 6); // Drops "Suffix" from remainder tracking frame
    assert_ptr_eq(raw + 6, sv.s, "Suffix extraction leaves initial read location point locked stable");
    assert_int_eq(4, sv.slen, "Structural size boundaries dropped down effectively");
    
    // Edge case: Request to drop more characters than are currently tracked in the system window
    stringv_remove_suffix(&sv, 50);
    assert_int_eq(0, sv.slen, "Over-dropping clamps structural slice length precisely down to zero size safely");
}


/*****************************************************************************
 * Test Driver Suite Initialization Orchestration
 *****************************************************************************/
void test_stringv (void)
{
    printf("==================================================\n");
    printf("         STARTING UTILS::stringv UNIT TESTS          \n");
    printf("==================================================\n");
    
    RUN_TEST(test_stringv_set_positive);
    RUN_TEST(test_stringv_setn_edge_and_positive);
    RUN_TEST(test_stringv_setb_bounds_and_negative);
    RUN_TEST(test_stringv_length_cases);
    RUN_TEST(test_stringv_compare_null_handling);
    RUN_TEST(test_stringv_compare_length_vs_ascii);
    RUN_TEST(test_stringv_compare_bounded_and_case_insensitive);
    RUN_TEST(test_stringv_affixes);
    RUN_TEST(test_stringv_contains_cases);
    RUN_TEST(test_stringv_substring_extraction);
    RUN_TEST(test_stringv_affix_removal);
    
    printf("==================================================\n");
    printf("TEST EXECUTION RESULTS REPORT SUMMARY:\n");
    printf("  Total Test Suites Dispatched: %d\n", tests_run);
    printf("  Total Failed Assertion Blocks: %d\n", tests_failed);
    printf("==================================================\n");
}