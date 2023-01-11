#ifndef load_examples_h
#define load_examples_h

//#define MAX_SIZE_OF_RELATIVE_PATH 1024
//#define MAX_SIZE_OF_RELATIVE_PATH_MINUS_ONE 1023

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "my_structs.h"
#include "find_knn.h"

/*
struct knn_dataset{
    char* file_name;
    int n , dim , k  ;
    double** data_points ;
    int** knn_id ;    
}knn_dataset;

struct knn_dataset_vector{
    char* file_name;
    int n , dim , k  ;
    double* data_points ;
    int* knn_id ;    
}knn_dataset_vector;

struct knn_dataset_vector_MPI{ 
    char* file_name;
    int n , dim , k  ; // This k has the number of precomputed knns in the file
    int n_most , n_total , offset;
    double* data_points ;
    int* knn_id ;    
}knn_dataset_vector_MPI;
*/


// Menu 
void scanf_check( int number_of_output_arguments_expected , int actual_output_arguments   );
char* select_file_to_load() ;
char* generate_filename(char* base_name, int num);
bool print_dataset_parameters_and_free ( char* file_name , bool free_file_name  );
void run_on_all_datasets();

int check_knn_request( int k_requested , char* file_name);

bool does_the_file_exist( char* file_name );

// Matrix form
void load_data_points_and_ans( struct knn_dataset*  dataset_to_load );

void print_dataset_points( struct knn_dataset*  dataset_to_print );

void print_dataset_knn_ids( struct knn_dataset*  dataset_to_print , int how_many_knn );

void free_dataset_struct( struct knn_dataset*  dataset_to_free );

// Vector form
void load_data_points_and_ans_vector( struct knn_dataset_vector*  dataset_to_load_vector );

void print_dataset_points_vector( struct knn_dataset_vector*  dataset_to_print_vector );

void print_dataset_knn_ids_vector( struct knn_dataset_vector*  dataset_to_print_vector , int how_many_knn );

void free_dataset_struct_vector( struct knn_dataset_vector*  dataset_to_free_vector );

// Vector form MPI

void load_data_points_MPI_vector( struct knn_dataset_vector_MPI*  dataset_to_load_vector , int rank , int total_processes);

void print_dataset_points_MPI_vector( struct knn_dataset_vector_MPI*  dataset_to_print_vector , int rank );

void initialize_datasets(  struct knn_dataset_vector_MPI* process_dataset , struct knn_dataset_vector_MPI*  working_dataset , struct knn_dataset_vector_MPI*  receiving_dataset , int rank);

void free_dataset_MPI_struct( struct knn_dataset_vector_MPI dataset_to_free);

// Check results
bool check_correct_knn ( struct knn_dataset_vector  dataset_to_check_vector , knnresult algorithm_found_knn );

bool check_correct_knn_MPI ( struct knn_dataset_vector_MPI  dataset_to_check_vector , knnresult algorithm_found_knn ) ;

#endif