#ifndef my_structs_h
#define my_structs_h


// Struct definitions to avoid recursive inclusions of headers ;


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
} ;



struct knn_dataset{
    char* file_name;
    int n , dim , k  ;
    double** data_points ;
    int** knn_id ;    
};

struct knn_dataset_vector{
    char* file_name;
    int n , dim , k  ;
    double* data_points ;
    int* knn_id ;    
};

struct knn_dataset_vector_MPI{ 
    char* file_name;
    int n , dim , k  ; // This k has the number of precomputed knns in the file
    int n_most , n_total , offset;
    double* data_points ;
    int* knn_id ;    
};




#endif
