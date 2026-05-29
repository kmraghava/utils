/****************************************************************************
 * FILE NAME     : ttree.c
 *
 * DESCRIPTION   : Comprehensive unit tests for tree_t
 ****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "kmrUtils/tree.h"


/****************************************************************************
 * Test container type
 ****************************************************************************/

typedef struct test_node_s
{
    int          id;
    const char  *name;

    tree_node_t  tnode;

} test_node_t;

/****************************************************************************
 * Helpers
 ****************************************************************************/

static test_node_t* make_node (test_node_t *node_p, int id, const char *name)
{
    node_p->id = id;
    node_p->name = name;

    tree_node_init(node_p->tnode);

    return node_p;
}

static bool match_by_id (tree_node_t *tnode_p, void *key_p)
{
    test_node_t  *node_p;
    int          *id_p;

    node_p = tree_get(tnode_p, test_node_t, tnode);
    id_p = key_p;

    return node_p->id == *id_p;
}

static bool match_by_name (tree_node_t *tnode_p, void *key_p)
{
    test_node_t  *node_p;
    char         *name_p;

    node_p = tree_get(tnode_p, test_node_t, tnode);
    name_p = key_p;

    return strcmp(node_p->name, name_p) == 0;
}

/****************************************************************************
 * Traversal recording helpers
 ****************************************************************************/

static int  iter_order[256];
static int  iter_count;

static void reset_iteration_state (void)
{
    memset(iter_order, 0, sizeof(iter_order));
    iter_count = 0;
}

static void record_iteration (tree_node_t *tnode_p, void *uptr_p)
{
    test_node_t  *node_p;

    (void)uptr_p;

    node_p = tree_get(tnode_p, test_node_t, tnode);

    iter_order[iter_count++] = node_p->id;
}

/****************************************************************************
 * tree_init()
 ****************************************************************************/

static void test_tree_init(void)
{
    tree_t  tree;

    tree_init(tree, NULL);

    assert(tree.parent_p == NULL);
    assert(list_empty(tree.nodes));
}

/****************************************************************************
 * tree_node_init()
 ****************************************************************************/

static void test_tree_node_init(void)
{
    test_node_t  node;

    make_node(&node, 1, "node");

    assert(node.tnode.parent_p == NULL);
    assert(node.tnode.sub_tree.parent_p == &node.tnode);
    assert(list_empty(node.tnode.sub_tree.nodes));
}

/****************************************************************************
 * tree_node_is_leaf()
 ****************************************************************************/

static void test_tree_node_is_leaf(void)
{
    tree_t       root;
    test_node_t  parent;
    test_node_t  child;

    tree_init(root, NULL);

    make_node(&parent, 1, "parent");
    make_node(&child, 2, "child");

    assert(tree_node_is_leaf(parent.tnode) == true);

    tree_add_node(root, parent.tnode);

    assert(tree_node_is_leaf(parent.tnode) == true);

    tree_add_node(parent.tnode.sub_tree, child.tnode);

    assert(tree_node_is_leaf(parent.tnode) == false);
    assert(tree_node_is_leaf(child.tnode) == true);

    tree_rem_node(parent.tnode.sub_tree, child.tnode);

    assert(tree_node_is_leaf(parent.tnode) == true);
}

/****************************************************************************
 * tree_add_node()
 ****************************************************************************/

static void test_tree_add_node(void)
{
    tree_t       root;
    test_node_t  n1;
    test_node_t  n2;

    tree_init(root, NULL);

    make_node(&n1, 1, "n1");
    make_node(&n2, 2, "n2");

    tree_add_node(root, n1.tnode);

    assert(n1.tnode.parent_p == &root);
    assert(tree_count(&root) == 1);

    tree_add_node(root, n2.tnode);

    assert(n2.tnode.parent_p == &root);
    assert(tree_count(&root) == 2);
}

/****************************************************************************
 * tree_rem_node()
 ****************************************************************************/

static void test_tree_rem_node(void)
{
    tree_t       root;
    test_node_t  n1;
    test_node_t  n2;

    tree_init(root, NULL);

    make_node(&n1, 1, "n1");
    make_node(&n2, 2, "n2");

    tree_add_node(root, n1.tnode);
    tree_add_node(root, n2.tnode);

    assert(tree_count(&root) == 2);

    tree_rem_node(root, n1.tnode);

    assert(n1.tnode.parent_p == NULL);
    assert(tree_count(&root) == 1);

    tree_rem_node(root, n2.tnode);

    assert(n2.tnode.parent_p == NULL);
    assert(tree_count(&root) == 0);
    assert(list_empty(root.nodes));
}

/****************************************************************************
 * tree_get()
 ****************************************************************************/

static void test_tree_get(void)
{
    test_node_t  node;
    test_node_t  *recovered_p;

    make_node(&node, 123, "hello");

    recovered_p = tree_get(&node.tnode, test_node_t, tnode);

    assert(recovered_p == &node);
    assert(recovered_p->id == 123);
    assert(strcmp(recovered_p->name, "hello") == 0);
}

/****************************************************************************
 * tree_foreach_node()
 ****************************************************************************/

static void test_tree_foreach_node(void)
{
    tree_t       root;
    test_node_t  n1;
    test_node_t  n2;
    test_node_t  n3;

    tree_node_t  *tnode_p;

    int           ids[8];
    int           count = 0;

    tree_init(root, NULL);

    make_node(&n1, 1, "n1");
    make_node(&n2, 2, "n2");
    make_node(&n3, 3, "n3");

    tree_add_node(root, n1.tnode);
    tree_add_node(root, n2.tnode);
    tree_add_node(root, n3.tnode);

    tree_foreach_node(root, tnode_p)
    {
        test_node_t *node_p;

        node_p = tree_get(tnode_p, test_node_t, tnode);

        ids[count++] = node_p->id;
    }

    assert(count == 3);

    /* insertion order */
    assert(ids[0] == 1);
    assert(ids[1] == 2);
    assert(ids[2] == 3);
}

/****************************************************************************
 * tree_foreach_member()
 ****************************************************************************/

static void test_tree_foreach_member(void)
{
    tree_t       root;
    test_node_t  n1;
    test_node_t  n2;
    test_node_t  n3;

    test_node_t *node_p;

    int           sum = 0;
    int           count = 0;

    tree_init(root, NULL);

    make_node(&n1, 10, "n1");
    make_node(&n2, 20, "n2");
    make_node(&n3, 30, "n3");

    tree_add_node(root, n1.tnode);
    tree_add_node(root, n2.tnode);
    tree_add_node(root, n3.tnode);

    tree_foreach_member(root, test_node_t, tnode, node_p)
    {
        sum += node_p->id;
        count++;
    }

    assert(count == 3);
    assert(sum == 60);
}

static void test_tree_foreach_member_empty_tree(void)
{
    tree_t       root;
    test_node_t *node_p;

    int           count = 0;

    tree_init(root, NULL);

    tree_foreach_member(root, test_node_t, tnode, node_p)
        count++;

    assert(count == 0);
}

static void test_tree_foreach_member_nested_loops(void)
{
    tree_t       root;
    test_node_t  n1;
    test_node_t  n2;

    test_node_t *outer_p;
    test_node_t *inner_p;

    int           count = 0;

    tree_init(root, NULL);

    make_node(&n1, 1, "n1");
    make_node(&n2, 2, "n2");

    tree_add_node(root, n1.tnode);
    tree_add_node(root, n2.tnode);

    tree_foreach_member(root, test_node_t, tnode, outer_p)
    {
        tree_foreach_member(root, test_node_t, tnode, inner_p)
        {
            count++;

            assert(outer_p != NULL);
            assert(inner_p != NULL);
        }
    }

    assert(count == 4);
}

/****************************************************************************
 * tree_count()
 ****************************************************************************/

static void test_tree_count(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;
    test_node_t  c;
    test_node_t  d;

    tree_init(root, NULL);

    make_node(&a, 1, "a");
    make_node(&b, 2, "b");
    make_node(&c, 3, "c");
    make_node(&d, 4, "d");

    assert(tree_count(NULL) == 0);
    assert(tree_count(&root) == 0);

    tree_add_node(root, a.tnode);
    assert(tree_count(&root) == 1);

    tree_add_node(root, b.tnode);
    assert(tree_count(&root) == 2);

    tree_add_node(a.tnode.sub_tree, c.tnode);
    assert(tree_count(&root) == 3);

    tree_add_node(c.tnode.sub_tree, d.tnode);
    assert(tree_count(&root) == 4);

    assert(tree_node_count(NULL) == 0);
    assert(tree_node_count(&a.tnode) == 3);
    assert(tree_node_count(&b.tnode) == 1);
    assert(tree_node_count(&c.tnode) == 2);
    assert(tree_node_count(&d.tnode) == 1);
}

/****************************************************************************
 * tree_level()
 ****************************************************************************/

static void test_tree_level(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;
    test_node_t  c;
    test_node_t  d;

    tree_init(root, NULL);

    make_node(&a, 1, "a");
    make_node(&b, 2, "b");
    make_node(&c, 3, "c");
    make_node(&d, 4, "d");

    tree_add_node(root, a.tnode);
    tree_add_node(a.tnode.sub_tree, b.tnode);
    tree_add_node(b.tnode.sub_tree, c.tnode);
    tree_add_node(c.tnode.sub_tree, d.tnode);

    assert(tree_level(NULL) == 0);
    assert(tree_level(&root) == 0);

    assert(tree_node_level(NULL) == 0);

    /*
     * Root-level nodes are level 0.
     */
    assert(tree_node_level(&a.tnode) == 0);

    assert(tree_node_level(&b.tnode) == 1);
    assert(tree_node_level(&c.tnode) == 2);
    assert(tree_node_level(&d.tnode) == 3);

    assert(tree_level(&a.tnode.sub_tree) == 1);
    assert(tree_level(&b.tnode.sub_tree) == 2);
    assert(tree_level(&c.tnode.sub_tree) == 3);
}

/****************************************************************************
 * tree_iterate()
 ****************************************************************************/

static void test_tree_iterate_empty_tree(void)
{
    tree_t  root;

    tree_init(root, NULL);

    reset_iteration_state();

    tree_iterate(&root, record_iteration, NULL);

    assert(iter_count == 0);
}

static void test_tree_iterate_null_arguments(void)
{
    tree_iterate(NULL, record_iteration, NULL);

    tree_t  root;

    tree_init(root, NULL);

    tree_iterate(&root, NULL, NULL);
}

static void test_tree_iterate_postorder(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;
    test_node_t  c;
    test_node_t  d;
    test_node_t  e;

    /*
     * Structure:
     *
     * root
     *  ├── a
     *  │    ├── c
     *  │    └── d
     *  └── b
     *       └── e
     */

    tree_init(root, NULL);

    make_node(&a, 1, "a");
    make_node(&b, 2, "b");
    make_node(&c, 3, "c");
    make_node(&d, 4, "d");
    make_node(&e, 5, "e");

    tree_add_node(root, a.tnode);
    tree_add_node(root, b.tnode);

    tree_add_node(a.tnode.sub_tree, c.tnode);
    tree_add_node(a.tnode.sub_tree, d.tnode);

    tree_add_node(b.tnode.sub_tree, e.tnode);

    reset_iteration_state();

    tree_iterate(&root, record_iteration, NULL);

    assert(iter_count == 5);

    /*
     * Postorder traversal.
     */
    assert(iter_order[0] == 3);
    assert(iter_order[1] == 4);
    assert(iter_order[2] == 1);
    assert(iter_order[3] == 5);
    assert(iter_order[4] == 2);
}

static void test_tree_node_iterate_single_node(void)
{
    test_node_t  node;

    make_node(&node, 99, "node");

    reset_iteration_state();

    tree_node_iterate(&node.tnode, record_iteration, NULL);

    assert(iter_count == 1);
    assert(iter_order[0] == 99);
}

/****************************************************************************
 * tree_find_node()
 ****************************************************************************/

static void test_tree_find_null_arguments(void)
{
    tree_t       root;
    test_node_t  node;

    int          id = 1;

    tree_init(root, NULL);

    make_node(&node, 1, "node");

    assert(tree_find_node(NULL, 1, &id, match_by_id) == NULL);
    assert(tree_find_node(&root, 1, &id, NULL) == NULL);
    assert(tree_find_node(&root, 0, &id, match_by_id) == NULL);

    assert(tree_node_find_node(NULL, 1, &id, match_by_id) == NULL);
    assert(tree_node_find_node(&node.tnode, 1, &id, NULL) == NULL);
    assert(tree_node_find_node(&node.tnode, 0, &id, match_by_id) == NULL);
}

static void test_tree_find_root_level(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;

    tree_node_t *found_p;

    int           id = 2;

    tree_init(root, NULL);

    make_node(&a, 1, "a");
    make_node(&b, 2, "b");

    tree_add_node(root, a.tnode);
    tree_add_node(root, b.tnode);

    found_p = tree_find_node(&root, 1, &id, match_by_id);

    assert(found_p == &b.tnode);
}

static void test_tree_find_nested_depth(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;
    test_node_t  c;

    tree_node_t *found_p;

    int           id = 3;

    tree_init(root, NULL);

    make_node(&a, 1, "a");
    make_node(&b, 2, "b");
    make_node(&c, 3, "c");

    tree_add_node(root, a.tnode);
    tree_add_node(a.tnode.sub_tree, b.tnode);
    tree_add_node(b.tnode.sub_tree, c.tnode);

    /*
     * depth=1 searches only root level.
     */
    found_p = tree_find_node(&root, 1, &id, match_by_id);
    assert(found_p == NULL);

    /*
     * depth=2 searches children.
     */
    found_p = tree_find_node(&root, 2, &id, match_by_id);
    assert(found_p == NULL);

    /*
     * depth=3 reaches grandchild.
     */
    found_p = tree_find_node(&root, 3, &id, match_by_id);
    assert(found_p == &c.tnode);
}

static void test_tree_find_not_found(void)
{
    tree_t       root;
    test_node_t  a;

    tree_node_t *found_p;

    int           id = 999;

    tree_init(root, NULL);

    make_node(&a, 1, "a");

    tree_add_node(root, a.tnode);

    found_p = tree_find_node(&root, 10, &id, match_by_id);

    assert(found_p == NULL);
}

static void test_tree_find_first_match(void)
{
    tree_t       root;
    test_node_t  a;
    test_node_t  b;

    tree_node_t *found_p;

    char         name[] = "dup";

    tree_init(root, NULL);

    make_node(&a, 1, "dup");
    make_node(&b, 2, "dup");

    tree_add_node(root, a.tnode);
    tree_add_node(root, b.tnode);

    found_p = tree_find_node(&root, 1, name, match_by_name);

    /*
     * First insertion-order match.
     */
    assert(found_p == &a.tnode);
}

static void test_tree_node_find_current_node_match(void)
{
    test_node_t  node;

    tree_node_t *found_p;

    int           id = 55;

    make_node(&node, 55, "node");

    found_p = tree_node_find_node(&node.tnode, 1, &id, match_by_id);

    assert(found_p == &node.tnode);
}

/****************************************************************************
 * Parent-child relationship invariants
 ****************************************************************************/

static void test_parent_child_relationships(void)
{
    tree_t       root;
    test_node_t  parent;
    test_node_t  child;
    test_node_t  grandchild;

    tree_init(root, NULL);

    make_node(&parent, 1, "parent");
    make_node(&child, 2, "child");
    make_node(&grandchild, 3, "grandchild");

    tree_add_node(root, parent.tnode);
    tree_add_node(parent.tnode.sub_tree, child.tnode);
    tree_add_node(child.tnode.sub_tree, grandchild.tnode);

    assert(parent.tnode.parent_p == &root);
    assert(child.tnode.parent_p == &parent.tnode.sub_tree);
    assert(grandchild.tnode.parent_p == &child.tnode.sub_tree);

    assert(parent.tnode.sub_tree.parent_p == &parent.tnode);
    assert(child.tnode.sub_tree.parent_p == &child.tnode);
    assert(grandchild.tnode.sub_tree.parent_p == &grandchild.tnode);
}

/****************************************************************************
 * Removal semantics
 ****************************************************************************/

static void test_remove_subtree_root_only_detaches_parent(void)
{
    tree_t       root;
    test_node_t  parent;
    test_node_t  child;

    tree_init(root, NULL);

    make_node(&parent, 1, "parent");
    make_node(&child, 2, "child");

    tree_add_node(root, parent.tnode);
    tree_add_node(parent.tnode.sub_tree, child.tnode);

    assert(tree_count(&root) == 2);

    tree_rem_node(root, parent.tnode);

    /*
     * Removing parent detaches only that node from root.
     * Child still remains attached to parent's subtree.
     */

    assert(parent.tnode.parent_p == NULL);

    assert(tree_node_count(&parent.tnode) == 2);

    assert(child.tnode.parent_p == &parent.tnode.sub_tree);

    assert(tree_count(&root) == 0);
}

/****************************************************************************
 * Main entry
 ****************************************************************************/
void test_tree (void)
{
    void (*tree_test_fns[])(void) = {
        test_tree_init,
        test_tree_node_init,
        test_tree_node_is_leaf,
        test_tree_add_node,
        test_tree_rem_node,
        test_tree_get,
        test_tree_foreach_node,
        test_tree_foreach_member,
        test_tree_foreach_member_empty_tree,
        test_tree_foreach_member_nested_loops,
        test_tree_count,
        test_tree_level,
        test_tree_iterate_empty_tree,
        test_tree_iterate_null_arguments,
        test_tree_iterate_postorder,
        test_tree_node_iterate_single_node,
        test_tree_find_null_arguments,
        test_tree_find_root_level,
        test_tree_find_nested_depth,
        test_tree_find_not_found,
        test_tree_find_first_match,
        test_tree_node_find_current_node_match,
        test_parent_child_relationships,
        test_remove_subtree_root_only_detaches_parent,
        NULL
    };

    int count;

    for (count = 0; tree_test_fns[count] != NULL; count++) {
        tree_test_fns[count]();
    }

    printf("All %d tree_t tests passed\n", count);
}