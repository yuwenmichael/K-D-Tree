#ifndef PROCESS_DATAH
#define PROCESS_DATAH

#include <string.h>
#include <stdio.h>


#define MAX_LEN_RECORD 512
#define MAX_QUOTE_NEXT_TO_COMMA 4


/*read the csv file*/
tree_t *read_csv(FILE *fp, tree_t *kdtree);

/*text-processing involved in the 'read_csv' funtion*/
void quote_position(char *buff, int *flag, int num_comma);
void replace_comma_1(char *buff, int num_comma);
void replace_comma_2(char *buff, int *index, int num_quote);

/*get the data from the csv file and insert it into the linked list*/
tree_t *get_data(char *buff, tree_t *kdtree);

/*text-processing involved in the 'get_data' function*/
void partial_cleanse(char *field);
void double_quote_to_single(char *str);
void further_cleanse(char *field);

#endif