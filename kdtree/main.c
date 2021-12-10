/** main.c
 * Created by yuwen michael zhang on 17 Sep 2020
 * email: yuwenmichael@student.unimelb.edu.au 
 * student id: 1089117
 * 
 * This program uses a kd tree to store the data and using the special features of kd tree to search efficiently.
 * This program includes two stages, one is finding the closest location from the queried position,
 * the second stages is to find the locations within a certain radius.
 * After finding location, we need to output it to an output file called output.txt
 * 
 * To find the closest location to a certain position, type:
 * make map1
 * then type: 
 * ./map1 file.csv output.txt < input.txt
 * 
 * To find the locations within a certain radius, type:
 * make map2
 * then type: 
 * ./map2 file.csv output.txt < input.txt
 * 
*/
#include "kdtree.h"
#include "process_data.h"
#include "read_input_radius.h"
#include "read_input_closest.h"

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "Incorrect arguments\n");
        fprintf(stderr, "Run in the form\n");
        fprintf(stderr, "\\map datafile outputfile < inputfile\n");
        exit(1);
    }

     //read in the data from the command line
    FILE *data = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");
    assert(data != NULL);
    assert(output_file != NULL);
    
    // read the csv file and start building the tree
    tree_t *kdtree = NULL;
    kdtree = read_csv(data, kdtree);
    output(output_file, kdtree);
    
    // close the file
    fclose(data);
    fclose(output_file);

    // free the node and its data in the kdtree
    free_tree(kdtree);
}
