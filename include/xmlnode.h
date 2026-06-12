
#ifndef __xmlnode_h
#define __xmlnode_h


#include <stdbool.h>
#include "kmrUtils/clist.h"


typedef struct xmlattr_s  xmlattr_t;
typedef struct xmlnode_s  xmlnode_t;


extern xmlattr_t* xmlattr_new (const char *name_p, const char *value_p);
extern xmlattr_t* xmlattr_del (xmlattr_t *attr_p);

extern const char* xmlattr_name (xmlattr_t *attr_p);
extern const char* xmlattr_value (xmlattr_t *attr_p);

extern xmlnode_t* xmlnode_new (const char *name_p);
extern xmlnode_t* xmlnode_del (xmlnode_t *node_p);

extern const char* xmlnode_name (xmlnode_t *node_p);

extern bool xmlnode_add_attribute (xmlnode_t *node_p, xmlattr_t *attr_p);
extern bool xmlnode_set_attribute (xmlnode_t *node_p, const char *name_p, const char *value_p);
extern xmlattr_t* xmlnode_get_attribute (xmlnode_t *node_p, const char *name_p);
extern const char* xmlnode_get_attribute_value (xmlnode_t *node_p, const char *name_p);

extern bool xmlnode_delete_attribute (xmlnode_t *node_p, xmlattr_t *attr_p);
extern bool xmlnode_remove_attribute (xmlnode_t *node_p, xmlattr_t *attr_p);

extern long xmlnode_attribute_count (xmlnode_t *node_p);

extern xmlattr_t* xmlnode_first_attribute (xmlnode_t *node_p);
extern xmlattr_t* xmlnode_next_attribute (xmlnode_t *node_p, xmlattr_t *attr_p);

extern bool xmlnode_set_text (xmlnode_t *node_p, const char *text_p);
extern const char* xmlnode_get_text (xmlnode_t *node_p);
extern bool xmlnode_del_text (xmlnode_t *node_p);

extern bool xmlnode_add_child (xmlnode_t *node_p, xmlnode_t *child_p);
extern bool xmlnode_delete_child (xmlnode_t *node_p, xmlnode_t *child_p);
extern bool xmlnode_remove_child (xmlnode_t *node_p, xmlnode_t *child_p);

extern long xmlnode_child_count (xmlnode_t *node_p);

extern xmlnode_t* xmlnode_first_child (xmlnode_t *node_p);
extern xmlnode_t* xmlnode_next_child (xmlnode_t *node_p, xmlnode_t *child_p);

extern xmlnode_t* xmlnode_find_child (xmlnode_t *node_p, const char *child_name_p);

extern xmlnode_t* xmlnode_find_node (xmlnode_t *node_p, long search_levels, const char *name_p);
extern clist_t* xmlnode_find_all_nodes (xmlnode_t *node_p, long search_levels, const char *name_p);

extern char* xmlnode_tostring (xmlnode_t *node_p, bool indent_b);


#endif /* __xmlnode_h */