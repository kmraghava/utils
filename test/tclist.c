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

size_t mem_usage = 0;


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
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 2: Push front/back
 */
int test_push_front_back()
{
    clist_t *ll = clist_new();

    int data[3] = { 10, 20, 30 };

    clist_push_back(ll, &data[0]);
    clist_push_front(ll, &data[1]);
    clist_push_back(ll, &data[2]);

    ASSERT_EQ(clist_count(ll), 3);

    int *data_p;

    clist_node_t *n = clist_first(ll);
    data_p = clist_member(n); 
    ASSERT_EQ(*data_p, 20);

    n = clist_next(n);
    data_p = clist_member(n); 
    ASSERT_EQ(*data_p, 10);

    n = clist_next(n);
    data_p = clist_member(n); 
    ASSERT_EQ(*data_p, 30);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 3: Insert at arbitrary positions
 */
int test_insert_at()
{
    clist_t *ll = clist_new();

    int data[4] = { 100, 200, 150, 300 };

    clist_insert_at(ll, 0, &data[0]);  // [100]
    clist_insert_at(ll, 1, &data[1]);  // [100,200]
    clist_insert_at(ll, 1, &data[2]);  // [100,150,200]
    clist_insert_at(ll, 3, &data[3]);  // [100,150,200,300]

    ASSERT_EQ(clist_count(ll), 4);

    int *data_p;
    
    data_p = clist_member_at(ll, 0); ASSERT_EQ(*data_p, 100);
    data_p = clist_member_at(ll, 1); ASSERT_EQ(*data_p, 150);
    data_p = clist_member_at(ll, 2); ASSERT_EQ(*data_p, 200);
    data_p = clist_member_at(ll, 3); ASSERT_EQ(*data_p, 300);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 4: Forward and reverse traversal
 */
int test_traversal()
{
    clist_t *ll = clist_new();

    int data[5] = {1,2,3,4,5};
    int i;

    for (i = 0; i < 5; i++)
        clist_push_back(ll, &data[i]);

    i = 0;
    clist_node_t *n;
    for (n = clist_first(ll); n != clist_tail(ll); n = clist_next(n))
        ASSERT_EQ(*((int *)clist_member(n)), data[i++]);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 5: Remove operations
 */
int test_remove()
{
    clist_t *ll = clist_new();

    int data[3] = { 10, 20, 30 };

    clist_push_back(ll, &data[0]);
    clist_push_back(ll, &data[1]);
    clist_push_back(ll, &data[2]);

    int *data_p;

    clist_node_t *n = clist_first(ll);

    clist_remove(ll, n);
    ASSERT_EQ(clist_count(ll), 2);

    data_p = clist_member_at(ll, 0); ASSERT_EQ(*data_p, 20);
    data_p = clist_member_at(ll, 1); ASSERT_EQ(*data_p, 30);

    clist_remove_at(ll, 1);
    ASSERT_EQ(clist_count(ll), 1);

    clist_remove_at(ll, 0);
    ASSERT_EQ(clist_count(ll), 0);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 6: Clear
 */
int test_clear()
{
    clist_t *ll = clist_new();

    int data[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i = 0; i < 10; i++)
        clist_push_back(ll, &data[i]);

    ASSERT_EQ(clist_count(ll), 10);

    clist_clear(ll);
    ASSERT_EQ(clist_count(ll), 0);
    ASSERT_EQ(clist_empty(ll), 1);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 7: Extend
 */
int test_extend()
{
    clist_t *ll1 = clist_new(), *ll2 = clist_new();

    int data1[3] = { 10, 20, 30 };
    int data2[5] = { 40, 50, 60, 70 };

    for (int i = 0; i < 3; i++) clist_push_back(ll1, &data1[i]);
    for (int i = 0; i < 4; i++) clist_push_back(ll2, &data2[i]);

    clist_extend(ll1, ll2);

    ASSERT_EQ(clist_count(ll1), 7);
    ASSERT_EQ(clist_count(ll2), 4);

    clist_del(ll1);
    clist_del(ll2);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

/*
 * Test 8: Append
 */
int test_append()
{
    clist_t *ll1 = clist_new(), *ll2 = clist_new();

    int data1[3] = { 10, 20, 30 };
    int data2[5] = { 40, 50, 60, 70 };

    for (int i = 0; i < 3; i++) clist_push_back(ll1, &data1[i]);
    for (int i = 0; i < 4; i++) clist_push_back(ll2, &data2[i]);

    clist_append(ll1, ll2);

    ASSERT_EQ(clist_count(ll1), 7);
    ASSERT_EQ(clist_count(ll2), 0);

    clist_del(ll1);
    clist_del(ll2);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

bool cmp_int (void *member_p, void *key_p)
{
    int *imember_p = member_p, *ikey_p = key_p;

    return *imember_p == *ikey_p;
}

/*
 * Test 9: Find
 */
int test_find()
{
    clist_t *ll = clist_new();

    int data[5] = { 10, 20, 30, 40, 50 };

    for (int i=0; i<5; i++)
        clist_push_back(ll, &data[i]);

    int *data_p;
    int key;

    for (int i=0; i<5; i++)
    {
        key = data[i];
        data_p = clist_find(ll, &key, cmp_int);

        ASSERT_TRUE(data_p != NULL);
        ASSERT_EQ(*data_p, key);
    }

    key = 100;
    data_p = clist_find(ll, &key, cmp_int);

    ASSERT_TRUE(data_p == NULL);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

    return 0;
}

bool cmp_str(void *member_p, void *key_p)
{
    char *smember_p = member_p, *skey_p = key_p;

    return 0 == strncmp(smember_p, skey_p, strlen(skey_p));
}

/*
 * Test 9: FindAll
 */
int test_find_all()
{
    clist_t *ll = clist_new();

    char *data[3] = { "cat", "cater", "caterpillar" };

    for (int i=0; i<3; i++)
        clist_push_back(ll, data[i]);

    clist_t *data_list;
    char *key;

    for (int i=0; i<3; i++)
    {
        key = data[i];
        data_list = clist_find_all(ll, key, cmp_str);

        ASSERT_TRUE(data_list != NULL);
        ASSERT_EQ(clist_count(data_list), 3-i);

        clist_del(data_list);
    }

    key = "caster";
    data_list = clist_find_all(ll, key, cmp_str);

    ASSERT_TRUE(data_list != NULL);
    ASSERT_EQ(clist_count(data_list), 0);
    clist_del(data_list);

    clist_del(ll);
    ASSERT_EQ(mem_usage, 0);

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
        test_clear,
        test_extend,
        test_append,
        test_find,
        test_find_all
    };

    const char* names[] = {
        "create_delete",
        "push_front_back",
        "insert_at",
        "traversal",
        "remove",
        "clear",
        "extend",
        "append",
        "find",
        "find_all"
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
