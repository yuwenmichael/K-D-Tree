/** kdtree.c
 * Created by yu wen michael zhang, student ID 1089117
 * Email: yuwenmichael@student.unimelb.edu.au
 * on 17 Sep 2020
 * This program contains all the kdtree related function, including
 * freeing the KDtree and building the tree
*/
#include "kdtree.h"

tree_t *build_kdtree(tree_t *parent, tree_t *new){
    tree_t **insertLocation = &parent;
    int level = 1;
    // record the current level: whether it is splited by x or by y
    while(*insertLocation){
        // if the new node is larger than the current insertLocation and is splited by x, we go right
        if(compare_x(new, (*insertLocation)) && (level % 2 != 0) ){

            // if two corredinate is the same, we append it to the list 
            if(compare_y(new, (*insertLocation)) == BOTH_SAME){

                // first time insert in to the linked list
                if((*insertLocation)->next == NULL){
                    insertLocation = &((*insertLocation)->next);
                }else{
                    // other times insert into the linked list
                    while((*insertLocation)->next != NULL){
                        insertLocation = &((*insertLocation)->next);
                    }
                    insertLocation = &((*insertLocation)->next);
                }
                // we break out of the loop and don't insert it into the tree
                break;
            }else{
                insertLocation = &((*insertLocation)->right);
            }
            level++;
            new->depth++;

        // if the new node is smaller than the current insertLocation and is splited by x, we go left
        } else if (new->information.coord.x < (*insertLocation)->information.coord.x && (level % 2 != 0)){
            insertLocation = &((*insertLocation)->left);
            level++;
            new->depth++;

        //if the new node is larger than current insertLocation and is splited by y, we go right
        } else if(compare_y(new, (*insertLocation)) && (level % 2 == 0)){

            // if two corredinate is the same, we append it to the list 
            if(compare_x(new, (*insertLocation)) == BOTH_SAME){
                // first time insert 
                if((*insertLocation)->next == NULL){
                    insertLocation = &((*insertLocation)->next);
                }else{
                    // other times insert
                    while((*insertLocation)->next != NULL){
                        insertLocation = &((*insertLocation)->next);
                    }
                    insertLocation = &((*insertLocation)->next);
                }
                // we break out of the loop and don't insert it into the tree
                break;
            }else{
                insertLocation = &((*insertLocation)->right);
            }
            level++;
            new->depth++;
        }
        //if the new node is smaller than current insertLocation and is splited by y, we go right
        else if (new->information.coord.y < (*insertLocation)->information.coord.y && (level % 2 == 0)){
            insertLocation = &((*insertLocation)->left);
            level++;
            new->depth++;
        }
	}
    // after finding the insertLocation, we let it point to the new node
    (*insertLocation) = new;
    (*insertLocation)->left = NULL;
    (*insertLocation)->right = NULL;
    (*insertLocation)->next = NULL;

    return parent;
}

/*compare two coordinate and record whether they are both the same or not*/
int compare_x(tree_t *new, tree_t *current){
    // the same
    if(strcmp(new->information.coord.x_coord_str,current->information.coord.x_coord_str) == 0){
        return BOTH_SAME;
    }
    // have difference and the first one is larger than the second one
    if(new->information.coord.x > current->information.coord.x){
        return PARTIAL_SAME;
    }
    return FAIL;
}

/*compare two coordinate and record whether they are both the same or not*/
int compare_y(tree_t *new, tree_t *current){
    // the same
    if(strcmp(new->information.coord.y_coord_str,current->information.coord.y_coord_str) == 0){
        return BOTH_SAME;
    }
    // have difference and the first one is larger than the second one
    if(new->information.coord.y > current->information.coord.y){
        return PARTIAL_SAME;
    }
    return FAIL;
}

/*free the tree recursively*/
void free_tree(tree_t *kdtree){
    if(!kdtree){
        return;
    }
    free_tree(kdtree->left);
    free_tree(kdtree->right);
    free_data(kdtree);
    free(kdtree);
}

/*free the data in the current node*/
void free_data(tree_t *node){
    tree_t *new, *prev;
    if(node->next != NULL){
        new = node->next;
        while(new){
            prev = new;
            new = new->next;
            free_node(prev);
            free(prev);
        }
    }
    free_node(node);
}

/*free the data in the current node */
void free_node(tree_t *prev){
    free(prev->information.census_year);
    free(prev->information.clue_small_area);
    free(prev->information.id.base_property_id);
    free(prev->information.id.block_id);
    free(prev->information.id.property_id);
    free(prev->information.industry_code);
    free(prev->information.industry_description);
    free(prev->information.trading_name);
    free(prev->information.coord.x_coord_str);
    free(prev->information.coord.y_coord_str);
    free(prev->information.location);
}