/**read_input_radius.c 
 * Created by yuwen michael zhang, student ID 1089117
 * Email: yuwenmichael@student.unimelb.edu.au
 * on 17 Sep 2020
 * This program will read the input from command line with x y coordinates and the radius.
 * It includes the functions for tree traversal with radius and print the matched data to the output.txt file.
*/

#include "kdtree.h"
#include "read_input_radius.h"
/*output the matched data to output.txt*/
void output(FILE *output_file, tree_t *kdtree){
    // create linked list for the queries
    list_t *list;
    list = creat_list();
    assert((output_file != NULL) && (list != NULL));
    
    //get the queries from the stdin
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
        double radius = 0;
        // check whether the new line character is in the current query
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
        new->next = NULL;
        assert(new != NULL);
        // count coordinate position
        int count = 0;
        // get the x_coord, y_coord and radius into the query struct
        while(field != NULL){
            if(count == 0){
                new->x_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
                assert(new->x_str != NULL);
                strcpy(new->x_str, field);
                x_coord = strtod(field, NULL);
                new->x = x_coord;
                count++;

            }else if(count == 1){
                new->y_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
                assert(new->y_str != NULL);
                strcpy(new->y_str, field);
                y_coord = strtod(field, NULL);
                new->y = y_coord;
                count++;
            }else if(count == 2){
                new->r_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
                assert(new->r_str != NULL);
                strcpy(new->r_str, field);
                radius = strtod(field, NULL);
                new->r = radius;
            }
            field = strtok(NULL, " ");  
        }

        //record the query and put it into the query linked list
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

/*create a list to store the query read from the input file*/
list_t *creat_list() {
	list_t *lst;
	lst = (list_t*)malloc(sizeof(*lst));
	assert(lst != NULL);
	lst->head = lst->foot = NULL;
	return lst;
}

/*find the matched location from the kdtree*/
void traverse_query(list_t *query_list, FILE *fp, tree_t *kdtree){
    assert((query_list != NULL) && (fp != NULL) && (query_list->head != NULL) && (kdtree != NULL));
    one_query_t *new;
    new = query_list->head;
    while(new != NULL){
        int num_compare = 0;
        int num_found = 0;
        traverse_tree_radius(new, kdtree, &num_compare, fp, &num_found);
        // if nothing is found, we fprintf not found to the output file
        if(num_found == 0){
            fprintf(fp,"%s %s %s --> NOTFOUND\n",new->x_str, new->y_str, new->r_str);
        }
        printf("%s %s %s --> %d\n",new->x_str, new->y_str, new->r_str, num_compare);
        fprintf(fp,"\n");
        new = new->next;
    }
    free_list_query(query_list);
}
/*this function will traverse the tree and find the locations which are within the radius*/
void traverse_tree_radius(one_query_t *data, tree_t *root, int *num_compare, FILE *fp, int *num_found){

    if(root == NULL){
        return;
    }
    // find the radius of the current root
    double radius_sq = sqrt(pow(root->information.coord.x - data->x, 2) + pow(root->information.coord.y - data->y, 2));
    // if the current radius is smmaller than the query radius, we output it
    if(radius_sq < data->r){
        *num_found += 1;
        print_data_radius(data, root, fp);
    }
    // check the distance from the current splited dimension and the corresponding portion of the key
    double distance_sq;
    if(root->depth % 2 != 0){
        distance_sq = sqrt(pow(root->information.coord.x - data->x, 2) );
    }else{
        distance_sq = sqrt(pow(root->information.coord.y - data->y ,2) );
    }
    // if the current distance is larger than the radius, we continue down to the left or right of the branch only
    if(distance_sq > data->r){
        if((root->depth % 2 != 0 && data->x < root->information.coord.x) || (root->depth % 2 == 0 && data->y < root->information.coord.y) ){

        // check for the next node if it is valid, if it is valid, we will add one  
            if(root->left != NULL){
                *num_compare += 1;
            }
            traverse_tree_radius(data, root->left, num_compare, fp, num_found);
        }else{
            if(root->right != NULL){
                *num_compare += 1;
            }
            traverse_tree_radius(data, root->right, num_compare, fp, num_found);
        }
    // if the current disrance is smaller than the radius, we need to check both branches
    }else{
        if(root->left != NULL){
            *num_compare += 1;
        }
        traverse_tree_radius(data, root->left, num_compare, fp, num_found);
        if(root->right != NULL){
            *num_compare += 1;
        }
        traverse_tree_radius(data, root->right, num_compare, fp, num_found);
    }

}

/*this function will print out the matched data to the output.txt file*/
void print_data_radius(one_query_t *new, tree_t *current_node, FILE *fp){
    tree_t *pt;
    pt = current_node;
    // check whether there are more nodes with the same locaiton
    while(pt){
        fprintf(fp,"%s %s %s--> ", new->x_str, new->y_str, new->r_str);
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
        pt = pt->next;
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
        free(prev->r_str);
		free(prev);
	}
	free(list);
}



