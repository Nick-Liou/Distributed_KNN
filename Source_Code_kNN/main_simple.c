#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "my_structs.h"

#include "load_examples.h"
#include "find_knn.h"
#include "helpers.h"

// compile with make 
// run with make run 

// By: Liouliakis Nikolaos 10058

int main(int argc, char** argv){

    

    // Time stuff
    struct timespec start, end;    
    double time_to_load, time_to_find_kNN, elapsed_total;
    
    
    char* file_to_load ;
	int knn_to_culculate ;
    // Choose file to load
    // Max size of file_to_load is 1024 char, 
    
    // File name example 
    // "knn_datasets_zero_based/knn_dataset0.txt" ;
    file_to_load = select_file_to_load();    
    knn_to_culculate = 0;    
    printf("Enter how many knn to find: ");
    scanf_check( 1 , scanf("%d", &knn_to_culculate)   );
    printf("\n");
    knn_to_culculate = check_knn_request( knn_to_culculate ,  file_to_load );
    
    // Start time mesurment for loading         ~~~~~
    clock_gettime(CLOCK_MONOTONIC, &start);


    // load dataset
    struct knn_dataset_vector  dataset_to_load_vector ;
    dataset_to_load_vector.file_name = file_to_load ;
    load_data_points_and_ans_vector(&dataset_to_load_vector);

    // Find time it took to load the file       ~~~~~    
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_to_load = (double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_nsec - start.tv_nsec) / 1000000;

    
    // Start time mesurment for kNN             ~~~~~
    clock_gettime(CLOCK_MONOTONIC, &start);

    // find knn
    knnresult final_knn =  kNN_low_memory_V2( dataset_to_load_vector.data_points ,(dataset_to_load_vector.data_points), dataset_to_load_vector.n, dataset_to_load_vector.n, dataset_to_load_vector.dim, knn_to_culculate );
    // find sqrt
    culculate_sqrt(&final_knn);

    // Find time it took to fidn KNN the file   ~~~~~    
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_to_find_kNN = (double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_nsec - start.tv_nsec) / 1000000;


    // check result    
    check_correct_knn(dataset_to_load_vector ,final_knn );


    // print result
    print_knnresult_ids(final_knn);
    print_knnresult_distances(final_knn);

    // free dataset and result
    free_struct_result(&final_knn);

    print_dataset_parameters_and_free(file_to_load , true);



    elapsed_total = time_to_load + time_to_find_kNN ;
    printf("Time to load dataset: %f ms\n", time_to_load);
    printf("Time to find kNN    : %f ms\n", time_to_find_kNN);
    printf("Total time          : %f ms\n", elapsed_total );

    printf("\nSuccesfull termination of the program!:) \n") ; 

    return 0 ;
}