#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ctree.h"

/* -----------------------------
 * Test Helpers
 * ----------------------------- */

typedef struct {
    int id;
} test_obj_t;

static bool match_by_id(void *member_p, void *key_p)
{
    test_obj_t *obj = (test_obj_t *)member_p;
    int *key = (int *)key_p;

    return obj && key && (obj->id == *key);
}

static test_obj_t* new_obj(int id)
{
    test_obj_t *o = malloc(sizeof(*o));
    o->id = id;
    return o;
}

/* -----------------------------
 * Tests
 * ----------------------------- */

void test_ctree_create_destroy()
{
    ctree_t *tree = ctree_new();
    assert(tree != NULL);

    tree = ctree_del(tree);
    assert(tree == NULL);
}

void test_ctree_add_root_nodes()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);

    ctree_node_t *n1 = ctree_add(tree, NULL, o1);
    ctree_node_t *n2 = ctree_add(tree, NULL, o2);

    assert(n1 != NULL);
    assert(n2 != NULL);
    assert(ctree_member(n1) == o1);
    assert(ctree_member(n2) == o2);

    ctree_del(tree);
    free(o1);
    free(o2);
}

void test_ctree_add_children()
{
    ctree_t *tree = ctree_new();

    test_obj_t *root_obj = new_obj(10);
    ctree_node_t *root = ctree_add(tree, NULL, root_obj);

    test_obj_t *child1_obj = new_obj(11);
    test_obj_t *child2_obj = new_obj(12);

    ctree_node_t *child1 = ctree_add(tree, root, child1_obj);
    ctree_node_t *child2 = ctree_add(tree, root, child2_obj);

    assert(child1 != NULL);
    assert(child2 != NULL);

    assert(ctree_member(child1) == child1_obj);
    assert(ctree_member(child2) == child2_obj);

    ctree_del(tree);
    free(root_obj);
    free(child1_obj);
    free(child2_obj);
}

void test_ctree_find_basic()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);

    ctree_node_t *n1 = ctree_add(tree, NULL, o1);
    ctree_node_t *n2 = ctree_add(tree, n1, o2);
    ctree_add(tree, n2, o3);

    int key = 2;
    void *res = ctree_find(tree, NULL, -1, &key, match_by_id);

    assert(res == o2);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
}

void test_ctree_find_with_level_limit()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);

    ctree_node_t *n1 = ctree_add(tree, NULL, o1);
    ctree_add(tree, n1, o2);

    int key = 2;

    /* limit search to level 0 -> should NOT find */
    void *res = ctree_find(tree, NULL, 0, &key, match_by_id);
    assert(res == NULL);

    /* allow deeper search */
    res = ctree_find(tree, NULL, -1, &key, match_by_id);
    assert(res == o2);

    ctree_del(tree);
    free(o1);
    free(o2);
}

void test_ctree_remove_node()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);

    ctree_node_t *root = ctree_add(tree, NULL, o1);
    ctree_node_t *child = ctree_add(tree, root, o2);

    /* remove child */
    ctree_rem_node(tree, child);

    int key = 2;
    void *res = ctree_find(tree, NULL, -1, &key, match_by_id);

    assert(res == NULL);

    ctree_del(tree);
    free(o1);
    free(o2);
}

void test_ctree_remove_subtree()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);

    ctree_node_t *root = ctree_add(tree, NULL, o1);
    ctree_node_t *child = ctree_add(tree, root, o2);
    ctree_add(tree, child, o3);

    /* remove subtree at child */
    ctree_rem_node(tree, child);

    int key2 = 2;
    int key3 = 3;

    assert(ctree_find(tree, NULL, -1, &key2, match_by_id) == NULL);
    assert(ctree_find(tree, NULL, -1, &key3, match_by_id) == NULL);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
}

void test_ctree_find_all_basic()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(1);  // duplicate key

    ctree_add(tree, NULL, o1);
    ctree_add(tree, NULL, o2);

    int key = 1;

    clist_t *list = ctree_find_all(tree, NULL, -1, &key, match_by_id);

    assert(list != NULL);

    /* You may want to assert list size if API exists */
    /* e.g., assert(clist_size(list) == 2); */

    clist_del(list);
    ctree_del(tree);
    free(o1);
    free(o2);
}

void test_ctree_null_safety()
{
    int key = 1;

    /* Should not crash */
    assert(ctree_find(NULL, NULL, -1, &key, match_by_id) == NULL);
    assert(ctree_find(NULL, NULL, -1, NULL, match_by_id) == NULL);
    assert(ctree_find(NULL, NULL, -1, &key, NULL) == NULL);
}

void test_ctree_node_count_basic()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);

    ctree_node_t *root = ctree_add(tree, NULL, o1);
    assert(ctree_node_count(root) == 1);

    ctree_node_t *child = ctree_add(tree, root, o2);
    assert(ctree_node_count(root) == 2);
    assert(ctree_node_count(child) == 1);

    ctree_node_t *grandchild = ctree_add(tree, child, o3);
    assert(ctree_node_count(child) == 2);
    assert(ctree_node_count(root) == 3);

    assert(ctree_count(tree) == 3);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
}

void test_ctree_node_count_multiple_children()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);
    test_obj_t *o4 = new_obj(4);

    ctree_node_t *root = ctree_add(tree, NULL, o1);

    ctree_node_t *c1 = ctree_add(tree, root, o2);
    ctree_node_t *c2 = ctree_add(tree, root, o3);

    /* root should see both children */
    assert(ctree_node_count(root) == 3);

    /* add grandchild under c1 */
    ctree_add(tree, c1, o4);

    assert(ctree_node_count(c1) == 2);
    assert(ctree_node_count(root) == 4);

    assert(ctree_count(tree) == 4);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
    free(o4);
}

void test_ctree_node_count_after_removal()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);
    test_obj_t *o4 = new_obj(4);

    ctree_node_t *root = ctree_add(tree, NULL, o1);
    ctree_node_t *c1 = ctree_add(tree, root, o2);
    ctree_node_t *c2 = ctree_add(tree, root, o3);
    ctree_node_t *g1 = ctree_add(tree, c1, o4);

    assert(ctree_count(tree) == 4);

    /*
     * Tree:
     * root
     * ├── c1
     * │   └── g1
     * └── c2
     *
     * Counts:
     * g1 = 0
     * c1 = 1
     * c2 = 0
     * root = 3
     */

    assert(ctree_node_count(root) == 4);
    assert(ctree_node_count(c1) == 2);

    /* remove subtree c1 (removes c1 + g1 = 2 nodes) */
    ctree_rem_node(tree, c1);

    assert(ctree_node_count(root) == 2);   // only c2 remains

    assert(ctree_count(tree) == 2);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
    free(o4);
}

void test_ctree_node_count_remove_leaf_vs_subtree()
{
    ctree_t *tree = ctree_new();

    test_obj_t *o1 = new_obj(1);
    test_obj_t *o2 = new_obj(2);
    test_obj_t *o3 = new_obj(3);

    ctree_node_t *root = ctree_add(tree, NULL, o1);
    ctree_node_t *c1 = ctree_add(tree, root, o2);
    ctree_node_t *c2 = ctree_add(tree, root, o3);

    assert(ctree_node_count(root) == 3);

    /* remove leaf */
    ctree_rem_node(tree, c2);
    assert(ctree_node_count(root) == 2);

    /* remove remaining child */
    ctree_rem_node(tree, c1);
    assert(ctree_node_count(root) == 1);

    ctree_rem_node(tree, root);
    assert(ctree_count(tree) == 0);

    ctree_del(tree);
    free(o1);
    free(o2);
    free(o3);
}

void test_ctree_node_count_mixed_operations()
{
    ctree_t *tree = ctree_new();

    test_obj_t *objs[6];
    for (int i = 0; i < 6; i++)
        objs[i] = new_obj(i);

    ctree_node_t *root = ctree_add(tree, NULL, objs[0]);
    ctree_node_t *a = ctree_add(tree, root, objs[1]);
    ctree_node_t *b = ctree_add(tree, root, objs[2]);
    ctree_node_t *c = ctree_add(tree, a, objs[3]);
    ctree_node_t *d = ctree_add(tree, a, objs[4]);
    ctree_node_t *e = ctree_add(tree, c, objs[5]);

    /*
     * Structure:
     * root
     * ├── a
     * │   ├── c
     * │   │   └── e
     * │   └── d
     * └── b
     *
     * Total descendants of root = 5
     */

    assert(ctree_node_count(root) == 6);
    assert(ctree_node_count(a) == 4);
    assert(ctree_node_count(c) == 2);

    /* remove c subtree (c + e = 2 nodes) */
    ctree_rem_node(tree, c);

    assert(ctree_node_count(a) == 2);      // only d remains
    assert(ctree_node_count(root) == 4);   // a,d,b

    /* remove b */
    ctree_rem_node(tree, b);
    assert(ctree_node_count(root) == 3);

    ctree_rem_node(tree, root);
    assert(ctree_count(tree) == 0);

    ctree_del(tree);

    for (int i = 0; i < 6; i++)
        free(objs[i]);
}

/* -----------------------------
 * Main
 * ----------------------------- */

void test_ctree()
{
    test_ctree_create_destroy();
    test_ctree_add_root_nodes();
    test_ctree_add_children();
    test_ctree_find_basic();
    test_ctree_find_with_level_limit();
    test_ctree_remove_node();
    test_ctree_remove_subtree();
    test_ctree_find_all_basic();
    test_ctree_null_safety();
    test_ctree_node_count_basic();
    test_ctree_node_count_multiple_children();
    test_ctree_node_count_after_removal();
    test_ctree_node_count_remove_leaf_vs_subtree();
    test_ctree_node_count_mixed_operations();

    printf("All ctree tests passed!\n");
}

