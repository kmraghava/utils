
#include "kmrUtils/clist.h"
#include "kmrUtils/str.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "xmlnode.h"


typedef struct xmlattr_s {
    char          *name_p;
    char          *value_p;

    xmlnode_t     *owner_p;
    clist_node_t  *container_node_p;
} xmlattr_t;

struct xmlnode_s {
    char          *name_p;
    clist_t       *attrs_p;
    clist_t       *children_p;
    char          *text_p;

    xmlnode_t     *parent_p;
    clist_node_t  *container_node_p;
};


static bool xmlnode_add_attribute_no_check (xmlnode_t *node_p, xmlattr_t* attr_p) {
    if (!node_p->attrs_p) {
        node_p->attrs_p = clist_new();
        if (!node_p->attrs_p) {
            return false;
        }
    }

    attr_p->container_node_p = clist_push_back(node_p->attrs_p, attr_p);
    if (!attr_p->container_node_p) {
        return false;
    }
    attr_p->owner_p = node_p;

    return true;
}

static bool xmlnode_child_ancestor_of_node (xmlnode_t *node_p, xmlnode_t *child_p) {
    while (node_p->parent_p) {
        node_p = node_p->parent_p;
    }
    return (node_p == child_p);
}

static xmlnode_t* xmlnode_do_find_node (xmlnode_t *node_p, long search_levels, const char *name_p) {
    if (0 == strcmp(node_p->name_p, name_p)) {
        return node_p;
    }

    if (--search_levels == 0) {
        return NULL;
    }

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            xmlnode_t  *matched_node_p = xmlnode_do_find_node(clist_member(i_child_p), search_levels, name_p);
            if (matched_node_p) {
                return matched_node_p;
            }
        }
    }

    return NULL;
}

static void xmlnode_do_find_all_nodes (clist_t **matched_nodes_pp, xmlnode_t *node_p, long search_levels, const char *name_p) {
    if (0 == strcmp(node_p->name_p, name_p)) {
        if (!clist_push_back(*matched_nodes_pp, node_p)) {
            clist_del(*matched_nodes_pp);
            *matched_nodes_pp = NULL;
            return;
        }
    }

    if (--search_levels == 0) {
        return;
    }

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            xmlnode_do_find_all_nodes(matched_nodes_pp, clist_member(i_child_p), search_levels, name_p);
            if (*matched_nodes_pp == NULL) {
                return;
            }
        }
    }
}

static long xmlnode_size (xmlnode_t *node_p) {
    long count = 0;
    
    count += 1 + strlen(node_p->name_p); // [<node-name]

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *attr_p = clist_member(i_attr_p);
            count += strlen(attr_p->name_p) + strlen(attr_p->value_p) + 4; // [ attribute-name="attribute-value"]
        }
    }

    if (!node_p->children_p && !node_p->text_p) {
        count += 2; // [/>]
        return count;
    }

    count += 1; // [>]

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            count += xmlnode_size(clist_member(i_child_p));
        }
    }

    if (node_p->text_p) {
        count += strlen(node_p->text_p);
    }

    count += strlen(node_p->name_p) + 3; // [</node-name>]

    return count;
}

static long xmlnode_size_with_indent (xmlnode_t *node_p, long indent) {
    long count = indent * 4;
    
    count += 1 + strlen(node_p->name_p); // [<node-name]

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *attr_p = clist_member(i_attr_p);
            count += strlen(attr_p->name_p) + strlen(attr_p->value_p) + 4; // [ attribute-name="attribute-value"]
        }
    }

    if (!node_p->children_p && !node_p->text_p) {
        count += 3; // [/>\n]
        return count;
    }
    
    count += 2; // [>\n]

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            count += xmlnode_size_with_indent(clist_member(i_child_p), indent + 1);
        }
    }

    if (node_p->text_p) {
        count += ((indent + 1) * 4);
        count += strlen(node_p->text_p);
        count += 1; // [\n]
    }

    count += (indent * 4);
    count += strlen(node_p->name_p) + 3; // [</node-name>]
    count += 1; // [\n]

    return count;
}

static void xmlnode_write_to_string (xmlnode_t *node_p, string_t *str_p) {
    string_append(str_p, "<");
    string_append(str_p, node_p->name_p);

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *attr_p = clist_member(i_attr_p);
            string_append(str_p, " ");
            string_append(str_p, attr_p->name_p);
            string_append(str_p, "=\"");
            string_append(str_p, attr_p->value_p);
            string_append(str_p, "\"");
        }
    }

    if (!node_p->children_p && !node_p->text_p) {
        string_append(str_p, "/>");
        return;
    }

    string_append(str_p, ">");

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            xmlnode_write_to_string(clist_member(i_child_p), str_p);
        }
    }

    if (node_p->text_p) {
        string_append(str_p, node_p->text_p);
    }

    string_append(str_p, "</");
    string_append(str_p, node_p->name_p);
    string_append(str_p, ">");
}

static void xmlnode_write_to_string_with_indent (xmlnode_t *node_p, string_t *str_p, long indent) {
    for (long i = 0; i < indent; i++) {
        string_append(str_p, "    ");
    }
    
    string_append(str_p, "<");
    string_append(str_p, node_p->name_p);

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *attr_p = clist_member(i_attr_p);
            string_append(str_p, " ");
            string_append(str_p, attr_p->name_p);
            string_append(str_p, "=\"");
            string_append(str_p, attr_p->value_p);
            string_append(str_p, "\"");
        }
    }

    if (!node_p->children_p && !node_p->text_p) {
        string_append(str_p, "/>\n");
        return;
    }
    
    string_append(str_p, ">\n");

    if (node_p->children_p) {
        clist_node_t  *i_child_p;
        clist_foreach(node_p->children_p, i_child_p) {
            xmlnode_write_to_string_with_indent(clist_member(i_child_p), str_p, indent + 1);
        }
    }

    if (node_p->text_p) {
        for (long i = 0; i <= indent; i++) {
            string_append(str_p, "    ");
        }
        string_append(str_p, node_p->text_p);
        string_append(str_p, "\n");
    }

    for (long i = 0; i <= indent; i++) {
        string_append(str_p, "    ");
    }
    string_append(str_p, "</");
    string_append(str_p, node_p->name_p);
    string_append(str_p, ">\n");
}


xmlattr_t* xmlattr_new (const char *name_p, const char *value_p) {
    if (!name_p || *name_p == '\0' || !value_p) {
        return NULL;
    }

    char       *attribute_name_p = NULL,
               *attribute_value_p = NULL;
    xmlattr_t  *attr_p = NULL;

    attribute_name_p = strdup(name_p);
    if (!attribute_name_p) {
        return NULL;
    }

    attribute_value_p = strdup(value_p);
    if (!attribute_value_p) {
        goto FATAL;
    }

    attr_p = malloc(sizeof(*attr_p));
    if (!attr_p) {
        goto FATAL;
    }

    attr_p->name_p = attribute_name_p;
    attr_p->value_p = attribute_value_p;

    attr_p->owner_p = NULL;
    attr_p->container_node_p = NULL;

    return attr_p;

FATAL:
    free(attribute_name_p);
    if (attribute_value_p) {
        free(attribute_value_p);
    }
    if (attr_p) {
        free(attr_p);
    }
    return NULL;
}

xmlattr_t* xmlattr_del (xmlattr_t *attr_p) {
    if (attr_p) {
        if (attr_p->owner_p) {
            xmlnode_remove_attribute(attr_p->owner_p, attr_p);
        }
        free(attr_p->name_p);
        free(attr_p->value_p);
        free(attr_p);
    }
    return NULL;
}

const char* xmlattr_name (xmlattr_t *attr_p) {
    if (!attr_p) {
        return NULL;
    }
    return attr_p->name_p;
}

const char* xmlattr_value (xmlattr_t *attr_p) {
    if (!attr_p) {
        return NULL;
    }
    return attr_p->value_p;
}

xmlnode_t* xmlnode_new (const char *name_p) {
    if (!name_p || *name_p == '\0') {
        return NULL;
    }

    char *nodename_p = strdup(name_p);
    if (!nodename_p) {
        return NULL;
    }

    xmlnode_t *node_p = malloc(sizeof(*node_p));
    if (!node_p) {
        free(nodename_p);
        return NULL;
    }

    node_p->name_p = nodename_p;
    node_p->attrs_p = NULL;
    node_p->children_p = NULL;
    node_p->text_p = NULL;

    node_p->parent_p = NULL;
    node_p->container_node_p = NULL;

    return node_p;
}

xmlnode_t* xmlnode_del (xmlnode_t *node_p) {
    if (node_p) {
        if (node_p->parent_p) {
            xmlnode_remove_child(node_p->parent_p, node_p);
        }
        free(node_p->name_p);
        if (node_p->text_p) {
            free(node_p->text_p);
        }
        if (node_p->attrs_p) {
            while (clist_count(node_p->attrs_p) > 0) {
                xmlattr_del(clist_member(clist_first(node_p->attrs_p)));
            }
            clist_del(node_p->attrs_p);
        }
        if (node_p->children_p) {
            while (clist_count(node_p->children_p) > 0) {
                xmlnode_del(clist_member(clist_first(node_p->children_p)));
            }
            clist_del(node_p->children_p);
        }
        free(node_p);
    }
    return NULL;
}

const char* xmlnode_name (xmlnode_t *node_p) {
    if (!node_p) {
        return NULL;
    }

    return node_p->name_p;
}

bool xmlnode_add_attribute (xmlnode_t *node_p, xmlattr_t *attr_p) {
    if (!node_p || !attr_p) {
        return false;
    }

    if (attr_p->container_node_p) {
        return false;
    }

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *node_attr_p = clist_member(i_attr_p);
            if (0 == strcmp(node_attr_p->name_p, attr_p->name_p)) {
                return false;
            }
        }
    }

    return xmlnode_add_attribute_no_check(node_p, attr_p);
}

bool xmlnode_set_attribute (xmlnode_t *node_p, const char *name_p, const char *value_p) {
    if (!node_p || !name_p || *name_p == '\0' || !value_p) {
        return false;
    }

    if (node_p->attrs_p) {
        clist_node_t  *i_attr_p;
        clist_foreach(node_p->attrs_p, i_attr_p) {
            xmlattr_t  *attr_p = clist_member(i_attr_p);
            if (0 == strcmp(attr_p->name_p, name_p)) {
                char  *attribute_value_p = strdup(value_p);
                if (!attribute_value_p) {
                    return false;
                }
                free(attr_p->value_p);
                attr_p->value_p = attribute_value_p;
                return true;
            }
        }
    }

    xmlattr_t  *attr_p = xmlattr_new(name_p, value_p);
    if (!attr_p) {
        return false;
    }

    if (!xmlnode_add_attribute_no_check(node_p, attr_p)) {
        xmlattr_del(attr_p);
        return false;
    }
    return true;
}

xmlattr_t* xmlnode_get_attribute (xmlnode_t *node_p, const char *name_p) {
    if (!node_p || !name_p || *name_p == '\0') {
        return NULL;
    }

    if (!node_p->attrs_p) {
        return NULL;
    }

    clist_node_t  *i_attr_p;
    clist_foreach(node_p->attrs_p, i_attr_p) {
        xmlattr_t  *attr_p = clist_member(i_attr_p);
        if (0 == strcmp(attr_p->name_p, name_p)) {
            return attr_p;
        }
    }

    return NULL;
}

const char* xmlnode_get_attribute_value (xmlnode_t *node_p, const char *name_p) {
    if (!node_p || !name_p || *name_p == '\0') {
        return NULL;
    }

    if (!node_p->attrs_p) {
        return NULL;
    }

    clist_node_t  *i_attr_p;
    clist_foreach(node_p->attrs_p, i_attr_p) {
        xmlattr_t  *attr_p = clist_member(i_attr_p);
        if (0 == strcmp(attr_p->name_p, name_p)) {
            return attr_p->value_p;
        }
    }

    return NULL;
}

bool xmlnode_delete_attribute (xmlnode_t *node_p, xmlattr_t *attr_p) {
    if (!xmlnode_remove_attribute(node_p, attr_p)) {
        return false;
    }
    xmlattr_del(attr_p);
    return true;
}

bool xmlnode_remove_attribute (xmlnode_t *node_p, xmlattr_t *attr_p) {
    if (!node_p || !node_p->attrs_p || !attr_p) {
        return false;
    }

    if (!attr_p->container_node_p || attr_p->owner_p != node_p) {
        return false;
    }

    clist_remove(node_p->attrs_p, attr_p->container_node_p);
    attr_p->container_node_p = NULL;
    attr_p->owner_p = NULL;
    return true;
}

long xmlnode_attribute_count (xmlnode_t *node_p) {
    if (!node_p) {
        return 0;
    }
    return clist_count(node_p->attrs_p);
}

xmlattr_t* xmlnode_first_attribute (xmlnode_t *node_p) {
    if (!node_p || clist_count(node_p->attrs_p) == 0) {
        return NULL;
    }

    return clist_member(clist_first(node_p->attrs_p));
}

xmlattr_t* xmlnode_next_attribute (xmlnode_t *node_p, xmlattr_t *attr_p) {
    if (!node_p || !attr_p) {
        return NULL;
    }

    if (!attr_p->container_node_p || attr_p->owner_p != node_p) {
        return NULL;
    }

    clist_node_t  *next_attr_node_p = clist_next(attr_p->container_node_p);
    if (next_attr_node_p == clist_tail(node_p->attrs_p)) {
        return NULL;
    }

    return clist_member(next_attr_node_p);
}

bool xmlnode_set_text (xmlnode_t *node_p, const char *text_p) {
    if (!node_p || !text_p) {
        return false;
    }

    char  *node_text_p = strdup(text_p);
    if (!node_text_p) {
        return false;
    }

    if (node_p->text_p) {
        free(node_p->text_p);
    }
    node_p->text_p = node_text_p;

    return true;
}

const char* xmlnode_get_text (xmlnode_t *node_p) {
    if (!node_p) {
        return NULL;
    }
    return node_p->text_p;
}

bool xmlnode_del_text (xmlnode_t *node_p) {
    if (!node_p) {
        return false;
    }

    if (node_p->text_p) {
        free(node_p->text_p);
        node_p->text_p = NULL;
    }

    return true;
}

bool xmlnode_add_child (xmlnode_t *node_p, xmlnode_t *child_p) {
    if (!node_p || !child_p)
        return false;

    if (node_p == child_p || xmlnode_child_ancestor_of_node(node_p, child_p)) {
        return false;
    }

    if (child_p->container_node_p) {
        return false;
    }

    if (!node_p->children_p) {
        node_p->children_p = clist_new();
        if (!node_p->children_p) {
            return false;
        }
    }

    child_p->container_node_p = clist_push_back(node_p->children_p, child_p);
    if (!child_p->container_node_p) {
        return false;
    }

    child_p->parent_p = node_p;
    return true;
}

bool xmlnode_delete_child (xmlnode_t *node_p, xmlnode_t *child_p) {
    if (!xmlnode_remove_child(node_p, child_p)) {
        return false;
    }
    xmlnode_del(child_p);
    return true;
}

bool xmlnode_remove_child (xmlnode_t *node_p, xmlnode_t *child_p) {
    if (!node_p || !node_p->children_p || !child_p) {
        return false;
    }

    if (!child_p->container_node_p || child_p->parent_p != node_p) {
        return false;
    }

    clist_remove(node_p->children_p, child_p->container_node_p);

    child_p->parent_p = NULL;
    child_p->container_node_p = NULL;

    return true;
}

long xmlnode_child_count (xmlnode_t *node_p) {
    if (!node_p) {
        return 0;
    }
    return clist_count(node_p->children_p);
}

xmlnode_t* xmlnode_first_child (xmlnode_t *node_p) {
    if (!node_p || clist_count(node_p->children_p) == 0) {
        return NULL;
    }

    return clist_member(clist_first(node_p->children_p));
}

xmlnode_t* xmlnode_next_child (xmlnode_t *node_p, xmlnode_t *child_p) {
    if (!node_p || !child_p) {
        return NULL;
    }

    if (!child_p->container_node_p || child_p->parent_p != node_p) {
        return NULL;
    }

    clist_node_t  *next_child_node_p = clist_next(child_p->container_node_p);
    if (next_child_node_p == clist_tail(node_p->children_p)) {
        return NULL;
    }

    return clist_member(next_child_node_p);
}

xmlnode_t* xmlnode_find_child (xmlnode_t *node_p, const char *child_name_p) {
    if (!node_p || !node_p->children_p || !child_name_p || *child_name_p == '\0') {
        return NULL;
    }

    clist_node_t  *i_child_p;
    clist_foreach(node_p->children_p, i_child_p) {
        xmlnode_t  *child_p = clist_member(i_child_p);
        if (0 == strcmp(child_p->name_p, child_name_p)) {
            return child_p;
        }
    }

    return NULL;
}

xmlnode_t* xmlnode_find_node (xmlnode_t *node_p, long search_levels, const char *name_p) {
    if (!node_p || search_levels <= 0 || !name_p || *name_p == '\0') {
        return NULL;
    }

    return xmlnode_do_find_node(node_p, search_levels, name_p);
}

clist_t* xmlnode_find_all_nodes (xmlnode_t *node_p, long search_levels, const char *name_p) {
    if (!node_p || search_levels <= 0 || !name_p || *name_p == '\0') {
        return NULL;
    }

    clist_t  *matched_nodes_p = clist_new();
    if (!matched_nodes_p) {
        return NULL;
    }

    xmlnode_do_find_all_nodes(&matched_nodes_p, node_p, search_levels, name_p);
    return matched_nodes_p;
}

char* xmlnode_tostring (xmlnode_t *node_p, bool indent_b) {
    if (!node_p) {
        return NULL;
    }

    string_t  str;
    string_init(&str, NULL);

    bool memory_reserved_b =   indent_b
                             ? string_reserve(&str, xmlnode_size_with_indent(node_p, 0))
                             : string_reserve(&str, xmlnode_size(node_p));
    if (!memory_reserved_b) {
        return NULL;
    }

    if (indent_b) {
        xmlnode_write_to_string_with_indent(node_p, &str, 0);
    } else {
        xmlnode_write_to_string(node_p, &str);
    }

    return str.s;
}
