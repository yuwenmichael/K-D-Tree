#ifndef KDTREEH
#define KDTREEH

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BOTH_SAME 1
#define PARTIAL_SAME 2
#define FAIL 0
/*a struct to represent the information for the company*/
typedef struct{
    char *block_id;
    char *property_id;
    char *base_property_id;
} all_id_t;

typedef struct{
    char *x_coord_str;
    char *y_coord_str;
    double x;
    double y;
} coord_t;

typedef struct{
    char *census_year;
    all_id_t id;
    char *clue_small_area;
    char *trading_name;
    char *industry_code;
    char *industry_description;
    char *location;
    coord_t coord;
} information_t;

// tree struct

typedef struct node tree_t;

struct node {
    information_t information;
    tree_t *next;
    tree_t *left;
    tree_t *right;
    int depth;
};


/*a linked list struct for input coordinate name*/
typedef struct one_query one_query_t;

struct one_query{
    char *x_str;
    char *y_str;
    char *r_str;
    double x;
    double y;
    double r;
    one_query_t *next;
};

typedef struct {
	one_query_t *head;
	one_query_t *foot;
} list_t;

/*function used to build the tree*/
tree_t *build_kdtree(tree_t *kdtree, tree_t *new);

/* compare the x coordinate*/
int compare_x(tree_t *new, tree_t *current);
/* compare the Y coordinate*/
int compare_y(tree_t *new, tree_t *current);

/*functions for freeing the node and its data inside it*/
void free_tree(tree_t *kdtree);
void free_node(tree_t *prev);
void free_data(tree_t *node);
#endif