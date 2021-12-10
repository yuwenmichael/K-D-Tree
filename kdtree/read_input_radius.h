#ifndef READ_INPUT_RADIUSH
#define READ_INPUT_RADIUSH

#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX_LEN_PER_FIELD 128
#define NOT_FOUND 0
#define FOUND 1

/*find the matched query and output to the output.txt*/
void output(FILE *output_file, tree_t *kdtree);

/*get the queries and append it into a linked list*/
void get_query(list_t *list);

/*this funciton is for queries processing*/
void remove_new_line(char *str);

/*traverse the kd tree to find the location based on radius*/
void traverse_tree_radius(one_query_t *data, tree_t *root, int *num_compare, FILE *fp, int *num_found);

/*print out the data when it is within the radius*/
void print_data_radius(one_query_t *new, tree_t *current_node, FILE *fp);

/*traverse the linked query to get matched data*/
void traverse_query(list_t *query_list,FILE *fp, tree_t *kdtree);

/*get the keys from the stdin*/
list_t *creat_list();

/*free the query list*/
void free_list_query(list_t *list);
#endif