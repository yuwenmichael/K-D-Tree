#ifndef READ_INPUT_CLOSESTH
#define READ_INPUT_CLOSESTH

#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX_LEN_PER_FIELD 128
#define NOT_FOUND 0
#define FOUND 1

/*find the matched query and output to the output.txt*/
void output(FILE *output_file, tree_t *kdtree);

/*get the query and append it into a linked list*/
void get_query(list_t *list);

/*this funciton is for queries processing*/
void remove_new_line(char *str);

/*print the data when it is the closest location to the queried location*/
void print_data(one_query_t *new, FILE *fp, tree_t *current_node);

/*traverse the tree and find the closest location*/
void traverse_tree(double key_x, double key_y, tree_t *root, double *smallest, tree_t *current_node, int *num);

/*traverse the queries from the stdin*/
void traverse_query(list_t *query_list,FILE *fp, tree_t *kdtree);

/*get the keys from the stdin*/
list_t *creat_list();

/*free the query list*/
void free_list_query(list_t *list);
#endif