#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clist.h"

/*
 * Simple ASSERT macros
 */
#define ASSERT_TRUE(expr) \
    do { \
        if (!(expr)) { \
            printf("FAIL: %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #expr); \
            return 1; \
        } \
    } while (0)

#define ASSERT_EQ(a,b) \
    do { \
        if ((a) != (b)) { \
            printf("FAIL: %s:%d: ASSERT_EQ(%s, %s) → %d != %d\n", \
                __FILE__, __LINE__, #a, #b, (int)(a), (int)(b)); \
            return 1; \
        } \
    } while (0)


/*
 * Test 1: Create/Delete
 */
int test_create_delete()
{
    clist_t *ll = clist_new();
    ASSERT_TRUE(ll != NULL);
    ASSERT_EQ(clist_count(ll), 0);
    ASSERT_EQ(clist_empty(ll), 1);

    ll = clist_del(ll);
    ASSERT_TRUE(ll == NULL);
    return 0;
}

/*
 * Test 2: Push front/back
 */
int test_push_front_back()
{
    clist_t *ll = clist_new();

    clist_push_back(ll, (void*)10);
    clist_push_front(ll, (void*)20);
    clist_push_back(ll, (void*)30);

    ASSERT_EQ(clist_count(ll), 3);

    clist_iterator_t *it = clist_begin(ll);
    ASSERT_EQ((long)clist_get(it), 20);

    it = clist_iterator_next(it);
    ASSERT_EQ((long)clist_get(it), 10);

    it = clist_iterator_next(it);
    ASSERT_EQ((long)clist_get(it), 30);

    clist_del(ll);
    return 0;
}

/*
 * Test 3: Insert at arbitrary positions
 */
int test_insert_at()
{
    clist_t *ll = clist_new();

    clist_insert_at(ll, 0, (void*)100);  // [100]
    clist_insert_at(ll, 1, (void*)200);  // [100,200]
    clist_insert_at(ll, 1, (void*)150);  // [100,150,200]
    clist_insert_at(ll, 3, (void*)300);  // [100,150,200,300]

    ASSERT_EQ(clist_count(ll), 4);
    ASSERT_EQ((long)clist_get_at(ll, 0), 100);
    ASSERT_EQ((long)clist_get_at(ll, 1), 150);
    ASSERT_EQ((long)clist_get_at(ll, 2), 200);
    ASSERT_EQ((long)clist_get_at(ll, 3), 300);

    clist_del(ll);
    return 0;
}

/*
 * Test 4: Forward and reverse traversal
 */
int test_traversal()
{
    clist_t *ll = clist_new();

    for (long i = 1; i <= 5; i++)
        clist_push_back(ll, (void*)i);

    int expected_forward[] = {1,2,3,4,5};
    int i = 0;

    clist_iterator_t *it;
    for (it = clist_begin(ll); it != clist_end(ll); it = clist_iterator_next(it))
        ASSERT_EQ((long)clist_get(it), expected_forward[i++]);

    int expected_reverse[] = {5,4,3,2,1};
    i = 0;

    for (it = clist_rbegin(ll); it != clist_rend(ll); it = clist_iterator_prev(it))
        ASSERT_EQ((long)clist_get(it), expected_reverse[i++]);

    clist_del(ll);
    return 0;
}

/*
 * Test 5: Remove operations
 */
int test_remove()
{
    clist_t *ll = clist_new();

    clist_push_back(ll, (void*)10);
    clist_push_back(ll, (void*)20);
    clist_push_back(ll, (void*)30);

    clist_iterator_t *it = clist_begin(ll);
    ASSERT_EQ((long)clist_remove(it), 10);

    ASSERT_EQ(clist_count(ll), 2);
    ASSERT_EQ((long)clist_get_at(ll, 0), 20);
    ASSERT_EQ((long)clist_get_at(ll, 1), 30);

    ASSERT_EQ((long)clist_remove_at(ll, 1), 30);
    ASSERT_EQ(clist_count(ll), 1);

    ASSERT_EQ((long)clist_remove_at(ll, 0), 20);
    ASSERT_EQ(clist_count(ll), 0);

    clist_del(ll);
    return 0;
}

/*
 * Test 6: Clear
 */
int test_clear()
{
    clist_t *ll = clist_new();

    for (long i = 0; i < 10; i++)
        clist_push_back(ll, (void*)i);

    ASSERT_EQ(clist_count(ll), 10);

    clist_clear(ll);
    ASSERT_EQ(clist_count(ll), 0);
    ASSERT_EQ(clist_empty(ll), 1);

    clist_del(ll);
    return 0;
}

/*
 * Main test runner
 */
void test_clist (void)
{
    printf("Running clist unit tests...\n");

    int (*tests[])() = {
        test_create_delete,
        test_push_front_back,
        test_insert_at,
        test_traversal,
        test_remove,
        test_clear
    };

    const char* names[] = {
        "create_delete",
        "push_front_back",
        "insert_at",
        "traversal",
        "remove",
        "clear"
    };

    int num = sizeof(tests)/sizeof(tests[0]);

    for (int i = 0; i < num; i++)
    {
        int r = tests[i]();
        if (r != 0) {
            printf("❌ Test FAILED: %s\n", names[i]);
            return;
        }
        printf("✔️ Test PASSED: %s\n", names[i]);
    }
}
