/**process_data.c
 * Created by yuwenmichael zhang student id 1089117
 * email: yuwenmichael@student.unimelb.edu.au
 * on 17 Sep 2020
 * this program contains all the preprocessing when reading the data file from the command line.
*/
#include "kdtree.h"
#include "process_data.h"

/*this funciton will read in the csv file*/
tree_t *read_csv(FILE *fp, tree_t *kdtree){
    //the buff is to store the line read from the csv file
    char buff[MAX_LEN_RECORD + 1];
    int row_count = 0;
    // in here, 3 will be the total byte for '\n','\r' and '\0'
    while(fgets(buff, MAX_LEN_RECORD + 1, fp)){
        //count for the quotes which close to the commas and comma in each line
        int num_quote = 0;
        int num_comma = 0;
        row_count++;
        // we skip the header
        if(row_count == 1){
            continue;
        }
        //the flag is to record where the '",' and ',"' appear
        //and the index is to store the index of these two cases
        int *flag;
        int *index;
        flag = (int*)malloc(strlen(buff) * sizeof(int));
        index = (int*)malloc(MAX_QUOTE_NEXT_TO_COMMA * sizeof(int));
        assert(flag != NULL);
        assert(index != NULL);
        // initialse the flag array
        for (int i = 0; i < strlen(buff); i++){
            flag[i] = 0;
        }
        // initialse the index array which is used to search for 
        // the index of quotes
        for (int i = 0; i < MAX_QUOTE_NEXT_TO_COMMA; i++){
            index[i] = 0;
        }
        // find the position of the quotes which near the comma
        quote_position(buff, flag, num_comma);
        // getting the index of the quote which is next to comma
        for (int i = 0; i < (int)strlen(buff); i++){
            if (flag[i] == 1){
                index[num_quote] = i;
                num_quote++;
            }
        }
        // replace the comma with '%'
        replace_comma_2(buff, index, num_quote);
        // get the data from the file and put it into the linked list
        kdtree = get_data(buff, kdtree);
        //free the index and flag 
        free(flag);
        free(index);
    }
    return kdtree;
}

/* this function is to find the position of double quote 
which is closed to comma eg: find '",' and ',"' */
void quote_position(char *buff, int *flag, int num_comma){
    for (int i = 0; i < strlen(buff); i++){
        //when hit '",' and ',"' we mark it as 1
        if ((buff[i] == '"') && (buff[i-1] == ',')){
            flag[i] = 1; 
        }else if ((buff[i-1] == '"') && (buff[i] == ',')){
            flag[i] = 1;
        }
        // if ',"' is closed to 'location'(the field name in the data file)
        // we don't want to record it  and simply record it as 2 and put it away
        if ((buff[i-1] == ',') && (buff[i] == '"') && (buff[i+1] == '(')){
            flag[i] = 2;
        }
        // record how many commas we have so far
        if (buff[i] == ','){
            num_comma ++;
        }
    }
    // change the comma in the 'location' to '%' 
    replace_comma_1(buff, num_comma);
}


/*replace the comma in 'location' to '%' so that when we use strtok() to
seperate the information based on comma
(the location in here means the filed called 'locaiton' in the csv file) */
void replace_comma_1(char *buff, int num_comma){
    int commas = 0;
    for (int i = 0; i < (int)strlen(buff); i++){
        if(buff[i] == ','){
            commas ++;
            if (commas == num_comma){
                buff[i] = '%';
            }
        }
    }
}

/*replace ',' to '%' other than 'location' so that strtok() can be used 
to seperate the csv data field badsed on comma */
void replace_comma_2(char *buff, int *index, int num_quote){

    // when there is 2 double quotes(next to comma)
    // 2 quotes means that there are 2 quotes around 'trading name' or 
    // 'Industry description'
    if(num_quote == 2){
        // loop through the buff and find the comma between two double quotes
        for (int i = 0; i < strlen(buff); i++){
            if((buff[i] == ',') && (index[0] < i) && (i < index[1])){
                buff[i] = '%';   
            }
        }
    }

    // when there is 4 double quotes(next to comma)
    // 4 quotes means that there are qutoes around 'trading name' and
    // 'Industry description'
    if(num_quote == 4){
        for (int i = 0; i < strlen(buff); i++){
            if((buff[i] == ',') && (index[0] < i) && (i < index[1])){
                buff[i] = '%';   
            }
        }
        for (int i = 0; i < strlen(buff); i++){
            if((buff[i] == ',') && (index[2] < i) && (i < index[3])){
                buff[i] = '%';   
            }
        }
    }
}

/*This funciton will read the data per line into the struct*/
tree_t* get_data(char *buff,tree_t *kdtree){
    // a new node th keep track of the information
    tree_t *new;
    new = (tree_t*)malloc(sizeof(tree_t));
    assert(new != NULL);
    new->depth = 1;
    //seperate the data based on comma
    char *field = strtok(buff, ",");
    int field_count = 0;
    while(field != 0){  
        //get the comma back and remove additional double quote
        partial_cleanse(field);
        if(field_count == 0){
            new->information.census_year = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.census_year != NULL);
            strcpy(new->information.census_year, field);
        }
        if(field_count == 1){
            new->information.id.block_id = (char*)malloc((strlen(field) +1) * sizeof(char));
            assert(new->information.id.block_id != NULL);
            strcpy(new->information.id.block_id, field);
        }
        if(field_count == 2){
            new->information.id.property_id = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.id.property_id != NULL);
            strcpy(new->information.id.property_id, field);
        }
        if(field_count == 3){
            new->information.id.base_property_id = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.id.base_property_id != NULL);
            strcpy(new->information.id.base_property_id, field);
        }
        if(field_count == 4){
            new->information.clue_small_area = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.clue_small_area != NULL);
            strcpy(new->information.clue_small_area, field);
        }
        if(field_count == 5){
            //change the double quote to single quote if there exists double quote
            double_quote_to_single(field);
            new->information.trading_name = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.trading_name != NULL);
            strcpy(new->information.trading_name, field);
        }
        if(field_count == 6){
            new->information.industry_code = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.industry_code != NULL);
            strcpy(new->information.industry_code, field);
        }
        if(field_count == 7){
            new->information.industry_description = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.industry_description != NULL);
            strcpy(new->information.industry_description, field);
        }
        if(field_count == 8){
            new->information.coord.x_coord_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.coord.x_coord_str != NULL);
            new->information.coord.x = strtod(field, NULL);
            strcpy(new->information.coord.x_coord_str, field);
        }
        if(field_count == 9){
            new->information.coord.y_coord_str = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.coord.y_coord_str != NULL);
            new->information.coord.y = strtod(field, NULL);
            strcpy(new->information.coord.y_coord_str, field);
        }
        if(field_count == 10){
            // remove the quote('"') which is at the end of the record
            further_cleanse(field);
            new->information.location = (char*)malloc((strlen(field) + 1) * sizeof(char));
            assert(new->information.location != NULL);
            strcpy(new->information.location, field);
        }
        field = strtok(NULL, ",");  
        field_count++;
    }
    /*put the current node to the kdtree and update the new node*/
    kdtree = build_kdtree(kdtree, new);
    return kdtree;
}

/*get the comma back and remove additional double quote*/
void partial_cleanse(char *field){
    int len = strlen(field); 
    int final_len = 0;
    // get the comma back based on '%'
    for(int i = 0; i< len; i++){
        if(field[i] == '%'){
            field[i] = ',';
        }
    }
    // we need to remove the double quotes 
    //if there exist at the front or at the end of the field
    if(field[0] == '"'){
        for(int i = 1; i < len; i++){
            field[final_len] = field[i];
            final_len++;
        }
        field[final_len-1] = '\0';
    }
}

/*remove extra quotes in the trading name*/
void double_quote_to_single(char *str){
    int num_quote = 0;
    int final_len = 0;
    // record how many quote in the string
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == '"'){
            num_quote++;
        }
    }
    // if there are quotes in the string, we do quotes removal operation
    if (num_quote != 0){
        for (int i = 0; i < strlen(str); i++){
            str[final_len] = str[i];
            final_len++;
            // remove the extra quotes when it is a multiple of 2
            // so that the double quote will be change to single quote
            // visualisation: change from '""' to '"'
            if((str[i] == '"') && (num_quote != 0)){
                if (num_quote %2 == 0){
                    final_len--;
                }
                num_quote--;
            }
        }
        str[final_len] = '\0';
    }
}


/*remove the '"' in 'location'(a field name in the data file)*/
void further_cleanse(char *field){
    for (int i = 0; i < strlen(field); i++){
        if(field[i] == '"'){
            field[i] = '\0';
        }
    }
}

