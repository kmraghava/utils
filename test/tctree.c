#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "kmrUtils/ctree.h"

static bool match_int(void *member_p, void *key_p)
{
    return *(int*)member_p == *(int*)key_p;
}

/* ---------------- BASIC CREATION ---------------- */

static void test_basic_create()
{
    ctree_t *tree = ctree_new(NULL);
    assert(tree);
    assert(ctree_count(tree) == 0);
    assert(ctree_level(tree) == 0);

    tree = ctree_del(tree);
    assert(tree == NULL);
}

/* ---------------- NODE INSERT + COUNT ---------------- */

static void test_node_insert_and_count()
{
    ctree_t *tree = ctree_new(NULL);

    int a=1, b=2, c=3;

    ctree_node_t *n1 = ctree_node_new(tree, &a);
    ctree_node_t *n2 = ctree_node_new(tree, &b);
    ctree_node_t *n3 = ctree_node_new(tree, &c);

    assert(ctree_count(tree) == 3);
    assert(ctree_node_count(n1) == 1);

    tree = ctree_del(tree);
}

/* ---------------- SUBTREE + COUNT PROPAGATION ---------------- */

static void test_subtree_count_propagation()
{
    ctree_t *root = ctree_new(NULL);

    int a=1, b=2, c=3;

    ctree_node_t *n1 = ctree_node_new(root, &a);

    ctree_t *sub = ctree_new(n1);
    ctree_node_new(sub, &b);
    ctree_node_new(sub, &c);

    assert(ctree_count(sub) == 2);
    assert(ctree_node_count(n1) == 3);  // itself + subtree
    assert(ctree_count(root) == 3);

    root = ctree_del(root);
}

/* ---------------- NODE DELETE ---------------- */

static void test_node_delete()
{
    ctree_t *tree = ctree_new(NULL);

    int a=1, b=2;

    ctree_node_t *n1 = ctree_node_new(tree, &a);
    ctree_node_t *n2 = ctree_node_new(tree, &b);

    assert(ctree_count(tree) == 2);

    ctree_node_del(n1);

    assert(ctree_count(tree) == 1);

    tree = ctree_del(tree);
}

/* ---------------- SUBTREE REPLACEMENT ---------------- */

static void test_subtree_replace()
{
    ctree_t *root = ctree_new(NULL);
    int a=1, b=2, c=3;

    ctree_node_t *n1 = ctree_node_new(root, &a);

    ctree_t *sub1 = ctree_new(n1);
    ctree_node_new(sub1, &b);

    assert(ctree_count(root) == 2);

    ctree_t *sub2 = ctree_new(NULL);
    ctree_node_new(sub2, &c);

    ctree_node_set_subtree(n1, sub2);

    assert(ctree_count(root) == 2); // replaced subtree

    root = ctree_del(root);
}

/* ---------------- LEVEL CHECK ---------------- */

static void test_levels()
{
    ctree_t *root = ctree_new(NULL);
    int a=1, b=2;

    ctree_node_t *n1 = ctree_node_new(root, &a);
    ctree_t *sub = ctree_new(n1);
    ctree_node_t *n2 = ctree_node_new(sub, &b);

    assert(ctree_level(root) == 0);
    assert(ctree_node_level(n1) == 0);
    assert(ctree_level(sub) == 1);
    assert(ctree_node_level(n2) == 1);

    root = ctree_del(root);
}

/* ---------------- TRAVERSAL ---------------- */

static void test_traversal()
{
    ctree_t *tree = ctree_new(NULL);

    int a=1, b=2, c=3;

    ctree_node_t *n1 = ctree_node_new(tree, &a);
    ctree_node_t *n2 = ctree_node_new(tree, &b);
    ctree_node_t *n3 = ctree_node_new(tree, &c);

    assert(ctree_first_node(tree) == n1);
    assert(ctree_last_node(tree) == n3);

    assert(ctree_node_next(n1) == n2);
    assert(ctree_node_next(n2) == n3);
    assert(ctree_node_next(n3) == NULL);

    tree = ctree_del(tree);
}

/* ---------------- FIND ---------------- */

static void test_find()
{
    ctree_t *root = ctree_new(NULL);

    int a=1, b=2, c=3;

    ctree_node_t *n1 = ctree_node_new(root, &a);
    ctree_t *sub = ctree_new(n1);
    ctree_node_new(sub, &b);
    ctree_node_new(sub, &c);

    int key = 2;

    void *res = ctree_find(root, -1, &key, match_int);
    assert(res == &b);

    root = ctree_del(root);
}

/* ---------------- FIND WITH DEPTH LIMIT ---------------- */

static void test_find_depth_limit()
{
    ctree_t *root = ctree_new(NULL);

    int a=1, b=2;

    ctree_node_t *n1 = ctree_node_new(root, &a);
    ctree_t *sub = ctree_new(n1);
    ctree_node_new(sub, &b);

    int key = 2;

    void *res = ctree_find(root, 1, &key, match_int);
    assert(res == NULL); // depth limited

    root = ctree_del(root);
}

/* ---------------- FIND ALL ---------------- */

static void test_find_all()
{
    ctree_t *root = ctree_new(NULL);

    int a=1, b=2, c=2;

    ctree_node_new(root, &a);
    ctree_node_new(root, &b);
    ctree_node_new(root, &c);

    int key = 2;

    clist_t *list = ctree_find_all(root, -1, &key, match_int);
    assert(list);

    // Expect 2 matches
    assert(clist_count(list) == 2);

    clist_del(list);
    root = ctree_del(root);
}

/* ---------------- MAIN ---------------- */

void test_ctree()
{
    test_basic_create();
    test_node_insert_and_count();
    test_subtree_count_propagation();
    test_node_delete();
    test_subtree_replace();
    test_levels();
    test_traversal();
    test_find();
    test_find_depth_limit();
    test_find_all();

    printf("All tests passed.\n");
}
