/**read_input_closest.c 
 * Created by yuwen michael zhang, student ID 1089117
 * Email: yuwenmichael@student.unimelb.edu.au
 * on 17 Sep 2020
 * This program will read the input from command line with x and y coordinates
 * It includes the tree traversal function which is used to find the cloeset location.
*/

#include "kdtree.h"
#include "read_input_closest.h"

void output(FILE *output_file, tree_t *kdtree){
    list_t *list;
    list = creat_list();
    assert((output_file != NULL) && (list != NULL));
    
    //get the query from the stdin
    get_query(list);

    //traverse the query and find the matched file
    traverse_query(list, output_file, kdtree);
}

/*get the keys into a linked list*/
void get_query(list_t *list){
    assert(list != NULL);
    
    //a buff for the current read query
    char current_query[MAX_LEN_PER_FIELD + 1];
    while(fgets(current_query, MAX_LEN_PER_FIELD + 1, stdin)){
        int found = NOT_FOUND;
        double x_coord = 0;
        double y_coord = 0;
        // check whether the new line character is in the current_query
        for(int i = 0; i< strlen(current_query); i++){
            if (current_query[i] == '\n'){
                found = FOUND;
            }
        }
        // give the string with a new line
        if (found == FOUND){
            remove_new_line(current_query);
        }
        char *field = strtok(current_query, " ");
        one_query_t *new;
        new = (one_query_t*) malloc(sizeof(*new));
        assert(new != NULL);
        new->next = NULL;
        // count coordinate position
        int count = 0;
        while(field != NULL){
            // if the count is 0, then the field is for x coordinate
            if(count == 0){
                new->x_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
                assert(new->x_str != NULL);
                strcpy(new->x_str, field);
                x_coord = strtod(field, NULL);
                new->x = x_coord;
                count++;
            // if the count is 1, then the field is for y coordinate
            }else if(count == 1){
                new->y_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
                assert(new->y_str != NULL);
                strcpy(new->y_str, field);
                y_coord = strtod(field, NULL);
                new->y = y_coord;
            }   
            field = strtok(NULL, " ");  
        }

        //continue to get the data
        if (list->foot == NULL) {
		    /* this is the first insertion into the list */
		    list->head = list->foot = new;
		} else {
			/*this part is after the first insertion */
			list->foot->next = new;
			list->foot = new;
		}
    }
}

/*remove the new line character in the input string*/
void remove_new_line(char *str){
    int len = strlen(str);
    str[len-1] = '\0';
}

/*create a list to store the queries read from the input file*/
list_t *creat_list() {
	list_t *lst;
	lst = (list_t*)malloc(sizeof(*lst));
	assert(lst != NULL);
	lst->head = lst->foot = NULL;
	return lst;
}

/*find the matched queries of the list*/
void traverse_query(list_t *query_list, FILE *fp, tree_t *kdtree){
    assert((query_list != NULL) && (fp != NULL) && (query_list->head != NULL) && (kdtree != NULL));
    one_query_t *new;
    new = query_list->head;
    // traverse the list to get the corresponding data
    while(new != NULL){
        // num for compare
        int num = 0;
        double smallest = -1;
        tree_t *node = NULL;
        node = (tree_t*) malloc(sizeof(tree_t));
        assert(node != NULL);
        // traverse the tree 
        traverse_tree(new->x, new->y, kdtree, &smallest, node, &num);
        // print out the data
        print_data(new, fp, node);
        fprintf(fp,"\n");
        printf("%s %s --> %d\n", new->x_str, new->y_str, num);
        // update the new node to the next node
        new = new->next;
        free(node);
    }
    free_list_query(query_list);
}

void print_data(one_query_t *new, FILE *fp, tree_t *current_node){
    assert((new != NULL) && (fp != NULL) && (current_node != NULL));
    // a list of node
    while(current_node != NULL){
        fprintf(fp,"%s %s --> ", new->x_str, new->y_str);
        fprintf(fp, "Census year: %s || ", current_node->information.census_year);
        fprintf(fp, "Block ID: %s || ", current_node->information.id.block_id);
        fprintf(fp, "Property ID: %s || ", current_node->information.id.property_id);
        fprintf(fp, "Base property ID: %s || ", current_node->information.id.base_property_id);
        fprintf(fp, "Trading name: %s || ", current_node->information.trading_name);
        fprintf(fp, "CLUE small area: %s || ", current_node->information.clue_small_area);
        fprintf(fp, "Industry (ANZSIC4) code: %s || ", current_node->information.industry_code);
        fprintf(fp, "Industry (ANZSIC4) description: %s || ", current_node->information.industry_description);
        fprintf(fp, "x coordinate: %.4lf || ", current_node->information.coord.x);
        fprintf(fp, "y coordinate: %.4lf || ", current_node->information.coord.y);
        fprintf(fp, "Location: %s || ", current_node->information.location);
        fprintf(fp, "\n");
        current_node = current_node->next;
    }
}
/*this function will traverse the tree */
void traverse_tree(double key_x, double key_y, tree_t *root, double *smallest, tree_t *current_node, int *num ){

    if(root == NULL){
        return;
    }
    // find the radius of the current root
    double radius_sq = sqrt(pow(root->information.coord.x - key_x,2) + pow(root->information.coord.y - key_y, 2));
    // update the smallest if it is the first time or smallest is larger than the current distance
    if(*smallest < 0 || *smallest > radius_sq){
        *smallest = radius_sq;
        *current_node = *root;
    }
    // split on  x or y depending on current depth of the node. if it is odd, we split on x else we split on y
    if((root->depth % 2 != 0 && key_x < root->information.coord.x) || (root->depth % 2 == 0 && key_y < root->information.coord.y) ){
        // check for the next node if it is valid, if it is valid, we will add one  
        if(root->left != NULL){
            *num += 1;
        }
        traverse_tree(key_x, key_y, root->left, smallest, current_node, num);
    }else{
        if(root->right != NULL){
             *num += 1;
        }
        traverse_tree(key_x, key_y, root->right, smallest, current_node, num);
    }
    // check the distance from the current key to the split dimension
    double distance_sq;
    if(root->depth % 2 != 0){
        distance_sq = sqrt(pow(root->information.coord.x - key_x, 2) );
    }else{
        distance_sq = sqrt(pow(root->information.coord.y - key_y ,2) );
    }
    // if the current distance is smaller than smallest, we need to check for the other end of the current root node
    if(distance_sq < *smallest ){
        tree_t *parent_next = root->right;
        if((root->depth %2 != 0 && key_x > root->information.coord.x) || (root->depth % 2 == 0 && key_y > root->information.coord.y)){
            parent_next = root->left;
        }
        if(parent_next != NULL){
            *num += 1;
        }
        traverse_tree(key_x, key_y, parent_next, smallest, current_node,num);
    }else{
        return;
    }

}

/*free the node with its corresponding data*/
void free_list_query(list_t *list){
    assert(list != NULL);
    one_query_t *new, *prev;
	new = list->head;
	while (new) {
		prev = new;
		new = new->next;
        free(prev->x_str);
        free(prev->y_str);
		free(prev);
	}
	free(list);
}

