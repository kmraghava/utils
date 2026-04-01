#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "json_field.h"

static void test_json_field_get() {
    printf("Running test_json_field_get...\n");

    // Setup a complex JSON structure
    json_t *root = json_pack("{s:{s:{s:[{s:i, s:i}, {s:i, s:i}]}}}", 
        "deviceParams", 
            "rfParams", 
                "channels", 
                    "freq", 3550, "power", 10,
                    "freq", 3560, "power", 12
    );

    // Test 1: Simple nested retrieval
    json_t *val = json_field_get(root, "deviceParams.rfParams.channels[0].freq");
    assert(val != NULL && json_is_integer(val));
    assert(json_integer_value(val) == 3550);

    // Test 2: Second array element retrieval
    val = json_field_get(root, "deviceParams.rfParams.channels[1].power");
    assert(val != NULL && json_integer_value(val) == 12);

    // Test 3: Non-existent path
    assert(json_field_get(root, "deviceParams.nonExistent") == NULL);
    assert(json_field_get(root, "deviceParams.rfParams.channels[5]") == NULL);

    // Test 4: Null or empty path
    assert(json_field_get(root, NULL) == NULL);
    assert(json_field_get(root, "") == NULL);

    json_decref(root);
    printf("test_json_field_get passed!\n\n");
}

static void test_json_field_set_object_paths() {
    printf("Running test_json_field_set_object_paths...\n");
    json_t *root = NULL;

    // Test 1: Create nested objects from NULL root
    bool ok = json_string_set(&root, "deviceParams.antennaName", "Parabolic");
    assert(ok == true);
    assert(json_is_object(root));
    
    const char *ret = json_string_get(root, "deviceParams.antennaName");
    assert(ret != NULL && strcmp(ret, "Parabolic") == 0);

    // Test 2: Set another field in the same structure
    json_bool_set(&root, "deviceParams.status", true);
    assert(json_is_true(json_field_get(root, "deviceParams.status")));

    json_decref(root);
    printf("test_json_field_set_object_paths passed!\n\n");
}

static void test_json_field_set_array_paths() {
    printf("Running test_json_field_set_array_paths...\n");
    json_t *root = json_object();

    // Test 1: Create array with padding (index 2)
    // Path: data.list[2]
    json_integer_set(&root, "data.list[2]", 100);
    
    json_t *list = json_field_get(root, "data.list");
    assert(json_is_array(list));
    assert(json_array_size(list) == 3);
    assert(json_is_null(json_array_get(list, 0))); // Padding check
    assert(json_is_null(json_array_get(list, 1))); // Padding check
    assert(json_integer_value(json_array_get(list, 2)) == 100);

    // Test 2: Overwrite an existing array index
    json_integer_set(&root, "data.list[2]", 200);
    assert(json_integer_value(json_field_get(root, "data.list[2]")) == 200);

    // Test 3: Add object inside array
    json_string_set(&root, "data.list[0].key", "value");
    assert(json_is_object(json_array_get(list, 0)));
    assert(strcmp(json_string_get(root, "data.list[0].key"), "value") == 0);

    json_decref(root);
    printf("test_json_field_set_array_paths passed!\n\n");
}

static void test_json_field_root_array() {
    printf("Running test_json_field_root_array...\n");
    json_t *root = NULL;

    // Test 1: Path starting with '[' creates array root
    json_string_set(&root, "[0].name", "first");
    assert(json_is_array(root));
    assert(json_array_size(root) == 1);
    assert(strcmp(json_string_get(root, "[0].name"), "first") == 0);

    // Test 2: Deep array padding at root
    json_string_set(&root, "[2].name", "third");
    assert(json_array_size(root) == 3);
    assert(json_is_null(json_array_get(root, 1)));

    json_decref(root);
    printf("test_json_field_root_array passed!\n\n");
}

static void test_error_conditions() {
    printf("Running test_error_conditions...\n");
    
    // Test 1: Invalid root pointer
    assert(json_field_set_new(NULL, "path", json_true()) == false);

    // Test 2: Null path
    json_t *root = json_object();
    assert(json_field_set_new(&root, NULL, json_true()) == false);

    // Test 3: Type mismatch (setting array index on a string)
    json_string_set(&root, "a", "not_an_object");
    // This should fail because 'a' is a string, not an object/array
    assert(json_string_set(&root, "a.b", "fail") == false);

    json_decref(root);
    printf("test_error_conditions passed!\n\n");
}

void test_json_field() {
    printf("--- STARTING JSON_FIELD UNIT TESTS ---\n\n");

    test_json_field_get();
    test_json_field_set_object_paths();
    test_json_field_set_array_paths();
    test_json_field_root_array();
    test_error_conditions();

    printf("--- ALL TESTS PASSED SUCCESSFULLY ---\n");
}
