/* test_list.c
 *
 * Comprehensive unit tests for the macro-based list defined in list.h
 * Compile: cd cmblib/utils; gcc -Iinclude test/tlist.c -o build/test/list
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* simple container used across tests */
typedef struct item {
    int value;
    list_node_t node;
} item_t;

/* delete callback */
static void delete_cb(item_t *it) {
    free(it);
}

/* helper to allocate an item */
static item_t * alloc_item(int v) {
    item_t *it = malloc(sizeof(*it));
    it->value = v;
    it->node.prev = NULL;
    it->node.next = NULL;
    it->node.mark = 0;
    return it;
}

/* ---------------- Tests ---------------- */

static void test_init_and_empty(void) {
    list_t l;
    list_init(l);
    assert(list_empty(l));
    assert(l.count == 0);
    /* head/tail linkage */
    assert(l.head.next == &l.tail);
    assert(l.tail.prev == &l.head);
    printf("test_init_and_empty passed\n");
}

static void test_push_back_and_first_last_member(void) {
    list_t l;
    list_init(l);

    item_t *a = alloc_item(1);
    list_add(l, a->node); /* push_back */
    assert(!list_empty(l));
    assert(l.count == 1);

    item_t *first = list_first_member(l, item_t, node);
    item_t *last  = list_last_member(l, item_t, node);
    assert(first == a && last == a);

    /* pointers in node should be set and not NULL */
    assert(a->node.prev == &l.head);
    assert(a->node.next == &l.tail);

    list_remove(l, a->node);
    assert(list_empty(l));
    assert(a->node.prev == NULL && a->node.next == NULL);

    free(a);
    printf("test_push_back_and_first_last_member passed\n");
}

static void test_push_front_and_insert_after_before(void) {
    list_t l;
    list_init(l);

    item_t *a = alloc_item(10);
    item_t *b = alloc_item(20);
    item_t *c = alloc_item(30);

    list_push_front(l, a->node); /* a */
    list_push_front(l, b->node); /* b,a */
    /* now insert c before a (i.e., into middle) */
    list_insert_before(l, list_first(l)->next /* which is a */, c->node);
    /* Validate order via foreach_member */
    list_node_t *np;
    item_t *m;
    int idx = 0;
    int vals[3];
    list_foreach_member(l, np, item_t, node, m) {
        vals[idx++] = m->value;
    }
    /* Values should be: b, c, a  => 20,30,10 */
    assert(idx == 3);
    assert(vals[0] == 20 && vals[1] == 30 && vals[2] == 10);

    /* cleanup */
    list_delete_all(l, item_t, node, delete_cb);
    assert(list_empty(l));

    printf("test_push_front_and_insert_after_before passed\n");
}

static void test_remove_first_and_last(void) {
    list_t l;
    list_init(l);

    item_t *a = alloc_item(1), *b = alloc_item(2), *c = alloc_item(3);
    list_add(l, a->node);
    list_add(l, b->node);
    list_add(l, c->node); /* a,b,c */

    /* remove_first should remove a */
    list_remove_first(l);
    assert(l.count == 2);
    /* a's node pointers should be NULL (it was removed) */
    assert(a->node.prev == NULL && a->node.next == NULL);

    /* remove_last should remove c */
    list_remove_last(l);
    assert(l.count == 1);
    assert(c->node.prev == NULL && c->node.next == NULL);

    /* remaining should be b */
    item_t *rem = list_first_member(l, item_t, node);
    assert(rem->value == 2);

    list_delete_all(l, item_t, node, delete_cb);
    /* free removed nodes */
    free(a);
    free(c);

    printf("test_remove_first_and_last passed\n");
}

static void test_remove_all_behavior(void) {
    list_t l;
    list_init(l);

    item_t *items[3];
    for (int i = 0; i < 3; ++i) {
        items[i] = alloc_item(i + 100);
        list_add(l, items[i]->node);
    }
    assert(l.count == 3);

    /* call list_remove_all (patched implementation) */
    list_remove_all(l);
    assert(list_empty(l));
    assert(l.count == 0);

    /* nodes must have prev/next cleared */
    for (int i = 0; i < 3; ++i) {
        assert(items[i]->node.prev == NULL && items[i]->node.next == NULL);
        free(items[i]);
    }

    printf("test_remove_all_behavior passed\n");
}

static void test_delete_and_delete_all(void) {
    list_t l;
    list_init(l);

    for (int i = 0; i < 4; ++i) {
        item_t *it = alloc_item(i);
        list_add(l, it->node);
    }
    assert(l.count == 4);

    /* delete first node using list_delete */
    list_delete(l, list_first(l), item_t, node, delete_cb);
    assert(l.count == 3);

    /* delete all remaining using list_delete_all */
    list_delete_all(l, item_t, node, delete_cb);
    assert(list_empty(l));
    assert(l.count == 0);

    printf("test_delete_and_delete_all passed\n");
}

/* iterate and foreach tests */
static int cb_sum;
static void cb_add_node(list_node_t *n) {
    item_t *it = list_get(n, item_t, node);
    cb_sum += it->value;
}
static void cb_add_member(item_t *it) {
    cb_sum += it->value;
}

static void test_iteration_macros(void) {
    list_t l;
    list_init(l);

    for (int i = 1; i <= 5; ++i) {
        item_t *it = alloc_item(i);
        list_add(l, it->node);
    }
    assert(l.count == 5);

    /* test list_iterate (node-cb) */
    cb_sum = 0;
    list_iterate(l, cb_add_node);
    assert(cb_sum == 1+2+3+4+5);

    /* test list_iterate_members (member-cb) */
    cb_sum = 0;
    list_iterate_members(l, item_t, node, cb_add_member);
    assert(cb_sum == 1+2+3+4+5);

    /* test foreach_member order */
    int idx = 0;
    list_node_t *np;
    item_t *m;
    list_foreach_member(l, np, item_t, node, m) {
        assert(m->value == ++idx);
    }
    assert(idx == 5);

    list_delete_all(l, item_t, node, delete_cb);
    printf("test_iteration_macros passed\n");
}

static int cb_cidx = 0;
static int cb_collected[3] = {-1,-1,-1};
static void cb_collect_node(list_node_t *n) {
    item_t *it = list_get(n, item_t, node);
    cb_collected[cb_cidx++] = it->value;
}

static void test_marking_and_marked_iterators(void) {
    list_t l;
    list_init(l);

    item_t *items[6];
    for (int i = 0; i < 6; ++i) {
        items[i] = alloc_item(i);
        list_add(l, items[i]->node);
    }
    /* mark a few */
    list_mark_node(items[1]->node);
    list_mark_node(items[3]->node);
    list_mark_node(items[5]->node);

    /* test list_iterate_marked (node callback) */
    list_iterate_marked(l, cb_collect_node);
    assert(cb_cidx == 3);
    assert(cb_collected[0] == 1 && cb_collected[1] == 3 && cb_collected[2] == 5);

    /* test list_iterate_marked_members */
    cb_sum = 0;
    list_iterate_marked_members(l, item_t, node, cb_add_member);
    assert(cb_sum == 1+3+5);

    /* test list_remove_marked removes marked nodes and clears node pointers */
    list_remove_marked(l);
    assert(l.count == 3); /* 6 - 3 marked removed = 3 left */

    /* confirm removed items have NULL prev/next */
    assert(items[1]->node.prev == NULL && items[1]->node.next == NULL);
    assert(items[3]->node.prev == NULL && items[3]->node.next == NULL);
    assert(items[5]->node.prev == NULL && items[5]->node.next == NULL);

    /* delete remaining and free removed */
    list_delete_all(l, item_t, node, delete_cb);
    free(items[1]); free(items[3]); free(items[5]);
    printf("test_marking_and_marked_iterators passed\n");
}

static void test_delete_marked(void) {
    list_t l;
    list_init(l);

    item_t *items[4];
    for (int i = 0; i < 4; ++i) {
        items[i] = alloc_item(10 + i);
        list_add(l, items[i]->node);
    }
    list_mark_node(items[0]->node);
    list_mark_node(items[2]->node);

    list_delete_marked(l, item_t, node, delete_cb);
    /* two deleted */
    assert(l.count == 2);

    /* remaining should be 11 and 13 */
    assert(list_first_member(l, item_t, node)->value == 11);
    assert(list_last_member(l, item_t, node)->value == 13);

    /* free survivors */
    list_delete_all(l, item_t, node, delete_cb);
    printf("test_delete_marked passed\n");
}

static void test_append(void) {
    list_t from, to;
    list_init(from);
    list_init(to);

    /* from: 1,2 ; to: 100 */
    item_t *a = alloc_item(1); list_add(from, a->node);
    item_t *b = alloc_item(2); list_add(from, b->node);
    item_t *c = alloc_item(100); list_add(to, c->node);

    list_append(from, to);
    /* to.count should now be 3, from should be empty */
    assert(to.count == 3);
    assert(list_empty(from));
    /* verify order in 'to': c, a, b */
    int seen1 = 0, seen2 = 0, seen100 = 0;
    list_node_t *np;
    item_t *m;
    int idx = 1;
    list_foreach_member(to, np, item_t, node, m) {
        if (m->value == 1) seen1 = idx;
        else if (m->value == 2) seen2 = idx;
        else if (m->value == 100) seen100 = idx;
        idx++;
    }
    assert(seen100 == 1 && seen1 == 2 && seen2 == 3);

    list_delete_all(to, item_t, node, delete_cb);
    printf("test_append passed\n");
}

/* run all tests */
void test_list (void) {
    test_init_and_empty();
    test_push_back_and_first_last_member();
    test_push_front_and_insert_after_before();
    test_remove_first_and_last();
    test_remove_all_behavior();
    test_delete_and_delete_all();
    test_iteration_macros();
    test_marking_and_marked_iterators();
    test_delete_marked();
    test_append();

    printf("ALL LIST MACRO TESTS PASSED\n");
}
