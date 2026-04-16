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
 * Dummy allocator
 */
static void *my_malloc(size_t s) { return malloc(s); }
static void my_free(void *p) { free(p); }

static const allocator_t custom_alloc = {
    .malloc  = my_malloc,
    .free    = my_free,
    .calloc  = NULL,
    .realloc = NULL
};


/*
 * Test 1: Create/Delete
 */
int test_create_delete()
{
    clist_t *ll = clist_new(&custom_alloc);
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
    clist_t *ll = clist_new(&custom_alloc);

    clist_set_member_size(ll, sizeof(int));

    int data;

    data = 10; clist_push_back(ll, &data);
    data = 20; clist_push_front(ll, &data);
    data = 30; clist_push_back(ll, &data);

    ASSERT_EQ(clist_count(ll), 3);

    int *data_p;

    clist_iterator_t *it = clist_begin(ll);
    data_p = clist_get(it); 
    ASSERT_EQ(*data_p, 20);

    it = clist_iterator_next(it);
    data_p = clist_get(it); 
    ASSERT_EQ(*data_p, 10);

    it = clist_iterator_next(it);
    data_p = clist_get(it); 
    ASSERT_EQ(*data_p, 30);

    clist_del(ll);
    return 0;
}

/*
 * Test 3: Insert at arbitrary positions
 */
int test_insert_at()
{
    clist_t *ll = clist_new(&custom_alloc);
    
    clist_set_member_size(ll, sizeof(int));

    int data;

    data = 100; clist_insert_at(ll, 0, &data);  // [100]
    data = 200; clist_insert_at(ll, 1, &data);  // [100,200]
    data = 150; clist_insert_at(ll, 1, &data);  // [100,150,200]
    data = 300; clist_insert_at(ll, 3, &data);  // [100,150,200,300]

    ASSERT_EQ(clist_count(ll), 4);

    int *data_p;
    
    data_p = clist_get_at(ll, 0); ASSERT_EQ(*data_p, 100);
    data_p = clist_get_at(ll, 1); ASSERT_EQ(*data_p, 150);
    data_p = clist_get_at(ll, 2); ASSERT_EQ(*data_p, 200);
    data_p = clist_get_at(ll, 3); ASSERT_EQ(*data_p, 300);

    clist_del(ll);
    return 0;
}

/*
 * Test 4: Forward and reverse traversal
 */
int test_traversal()
{
    clist_t *ll = clist_new(&custom_alloc);

    clist_set_member_size(ll, sizeof(int));

    for (int i = 1; i <= 5; i++)
        clist_push_back(ll, &i);

    int expected[] = {1,2,3,4,5};
    int i = 0;

    clist_iterator_t *it;
    for (it = clist_begin(ll); it != clist_end(ll); it = clist_iterator_next(it))
        ASSERT_EQ(*((int *)clist_get(it)), expected[i++]);

    clist_del(ll);
    return 0;
}

/*
 * Test 5: Remove operations
 */
int test_pop()
{
    clist_t *ll = clist_new(&custom_alloc);

    clist_set_member_size(ll, sizeof(int));

    int data;

    data = 10; clist_push_back(ll, &data);
    data = 20; clist_push_back(ll, &data);
    data = 30; clist_push_back(ll, &data);

    int *data_p;

    clist_iterator_t *it = clist_begin(ll);
    data_p = clist_pop(it); ASSERT_EQ(*data_p, 10);
    custom_alloc.free(data_p);

    ASSERT_EQ(clist_count(ll), 2);
    data_p = clist_get_at(ll, 0); ASSERT_EQ(*data_p, 20);
    data_p = clist_get_at(ll, 1); ASSERT_EQ(*data_p, 30);

    data_p = clist_pop_at(ll, 1); ASSERT_EQ(*data_p, 30);
    custom_alloc.free(data_p);
    ASSERT_EQ(clist_count(ll), 1);

    data_p = clist_pop_at(ll, 0); ASSERT_EQ(*data_p, 20);
    custom_alloc.free(data_p);
    ASSERT_EQ(clist_count(ll), 0);

    clist_del(ll);
    return 0;
}

/*
 * Test 6: Clear
 */
int test_clear()
{
    clist_t *ll = clist_new(&custom_alloc);

    clist_set_member_size(ll, sizeof(int));

    for (int i = 0; i < 10; i++)
        clist_push_back(ll, &i);

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
        test_pop,
        test_clear
    };

    const char* names[] = {
        "create_delete",
        "push_front_back",
        "insert_at",
        "traversal",
        "pop",
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
