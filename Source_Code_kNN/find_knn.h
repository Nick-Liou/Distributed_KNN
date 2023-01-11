#ifndef find_knn_h
#define find_knn_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "my_structs.h"
#include "load_examples.h"
//#include "find_knn.h"

/*
typedef struct knnresult{
  int    * nidx;    //!< Indices (0-based) of nearest neighbors [m-by-k]
  double * ndist;   //!< Distance of nearest neighbors          [m-by-k]
  int      m;       //!< Number of query points                 [scalar]
  int      k;       //!< Number of nearest neighbors            [scalar]
} knnresult;



struct knnresult_MPI{
  int    * nidx;    //!< Indices (0-based) of nearest neighbors [m-by-k]
  double * ndist;   //!< Distance of nearest neighbors          [m-by-k]
  int      m;       //!< Number of query points                 [scalar]
  int      k;       //!< Number of nearest neighbors            [scalar]
  int      k_found; //!< Number of nearest neighbors so far     [scalar]
} knnresult_MPI;
*/



knnresult kNN(double * X, double * Y, int n, int m, int d, int k);


knnresult kNN_low_memory(double * X, double * Y, int n, int m, int d, int k);

knnresult kNN_low_memory_V2(double * X, double * Y, int n, int m, int d, int k);


knnresult kNN_MPI(  struct knn_dataset_vector_MPI* mine_dataset, struct knn_dataset_vector_MPI* working_dataset, int knn_to_find  );

knnresult kNN_reduce_and_free( knnresult A , knnresult B , int k );

//struct knnresult_MPI reduce_knnresult_MPI(  struct knnresult_MPI* mine_dataset, struct knnresult_MPI* working_dataset, int knn_to_find  );
void print_knnresult_ids_with_offset( knnresult struct_to_print_knn_ids , int offset , int total_points);

// ~~~~~~~~~~~~~


void culculate_sqrt( knnresult* struct_to_find_roots  );

void print_knnresult_ids( knnresult struct_to_find_roots );

void print_knnresult_distances( knnresult struct_to_print_knn_ids );

int  find_number_length( int number );

void free_struct_result(knnresult* knn_to_free);




#endif