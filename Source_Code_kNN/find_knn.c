
#include <math.h>

#include "find_knn.h"
#include "k_smallest.h"
#include "find_knn.h"

/*
typedef struct knnresult{
  int    * nidx;    //!< Indices (0-based) of nearest neighbors [m-by-k]
  double * ndist;   //!< Distance of nearest neighbors          [m-by-k]
  int      m;       //!< Number of query points                 [scalar]
  int      k;       //!< Number of nearest neighbors            [scalar]
} knnresult;

*/


//! Compute k nearest neighbors of each point in X [m-by-d]
/*

  \param  X      Query data points               [m-by-d]
  \param  Y      Corpus data points              [n-by-d]
  \param  m      Number of query points          [scalar]
  \param  n      Number of corpus points         [scalar]
  \param  d      Number of dimensions            [scalar]
  \param  k      Number of neighbors             [scalar]

  \return  The kNN result
*/

knnresult kNN_low_memory(double * X, double * Y, int n, int m, int d, int k){    
    // X has m points
    // Y has n points
    // They both have d dimention 
    
    
    bool same_points = (X == Y) ;

    // edge cases
    if ( k < 0 ){
        int what_to_set_k = 0 ;
        printf("Inside kNN, k was %d which is negative so it was set to %d  \n" , k , what_to_set_k );
        k = what_to_set_k ; 
    }
    if( k >= n){
        if(same_points){
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n-1 );
            k = n-1 ; 
        }
        else if (k != n){
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n );
            k = n ; 
        }
    }
    
    knnresult knn_found ;
    knn_found.nidx =     (int*) malloc( m*k*sizeof(int));
    knn_found.ndist = (double*) malloc( m*k*sizeof(double));
    knn_found.m = m ;
    knn_found.k = k ; 

    if (knn_found.nidx == NULL){             
        printf("Failed to allocate memmory for the Indices (0-based) of nearest neighbors.\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }
    if (knn_found.ndist == NULL){             
        printf("Failed to allocate memmory for the Distance of nearest neighbors  .\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // Find the distance of each point in X with each point in Y

    double* distances_matrix = (double*) malloc( n*sizeof(double) ) ;
    if ( distances_matrix == NULL){             
        printf("Failed to allocate memmory for the Distances matrix.\n");
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // X[i][dimention] = X[i*d + dimention]
    //distances_matrix[X][Y]  = distances_matrix[X*n+Y] // the distance of X from Y 
    double temp_sum ; 
    int extra_points_with_same_distance = 0  ; 

    // X has m points
    // Y has n points
    // They both have d dimention 
    //printf("The D matrix is: \n");
    //printf("The knn ids matrix is: \n");
    if ( same_points ){
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same 
            //extra_points_with_same_distance += k_smallest_without_an_element( distances_matrix , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), m ,  k , i) ;  
            extra_points_with_same_distance += k_smallest_without_an_element( distances_matrix , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k , i) ;            
            //printf(" \n" );
        }
    }
    else{
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same 
            //extra_points_with_same_distance += k_smallest( &(distances_matrix[i*n]) , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), m ,  k) ;      
            extra_points_with_same_distance += k_smallest( distances_matrix, &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k) ;            
            //printf(" \n" );
        }
    }

    /*
    if( n == m ){
        printf(" \n Make sure it is symmetric \n" );
        for(int i = 0 ; i < m ; i++ ){     
            printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){            
                temp_sum = distances_matrix[ i*n+j ] - distances_matrix[ i+n*j ] ;
                printf(" %.15lf " , temp_sum);
            }        
            printf(" \n" );
        }
    }
    */


    //free
    free(distances_matrix);

    return knn_found ;


}


knnresult kNN(double * X, double * Y, int n, int m, int d, int k){    
    // X has m points
    // Y has n points
    // They both have d dimention 
    
    
    bool same_points = (X == Y) ;

    // edge cases
    if ( k < 0 ){
        int what_to_set_k = 2 ;
        printf("Inside kNN, k was %d which is negative so it was set to %d  \n" , k , what_to_set_k );
        k = what_to_set_k ; 
    }
    if( k > n){
        if(same_points){
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n-1 );
            k = n-1 ; 
        }
        else{
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n );
            k = n ; 
        }
    }

    knnresult knn_found ;
    knn_found.nidx =     (int*) malloc( m*k*sizeof(int));
    knn_found.ndist = (double*) malloc( m*k*sizeof(double));
    knn_found.m = m ;
    knn_found.k = k ; 

    if (knn_found.nidx == NULL){             
        printf("Failed to allocate memmory for the Indices (0-based) of nearest neighbors.\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }
    if (knn_found.ndist == NULL){             
        printf("Failed to allocate memmory for the Distance of nearest neighbors  .\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // Find the distance of each point in X with each point in Y

    // No need to malloc all this, just n*sizeof(double) would be enough
    double* distances_matrix = (double*) malloc( m*n*sizeof(double) ) ;
    if ( distances_matrix == NULL){             
        printf("Failed to allocate memmory for the Distances matrix.\n");
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // X[i][dimention] = X[i*d + dimention]
    //distances_matrix[X][Y]  = distances_matrix[X*n+Y] // the distance of X from Y 
    double temp_sum ; 
    int extra_points_with_same_distance = 0  ; 

    // X has m points
    // Y has n points
    // They both have d dimention 
    //printf("The D matrix is: \n");
    //printf("The knn ids matrix is: \n");
    if ( same_points ){
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ i*n+j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same 
            extra_points_with_same_distance += k_smallest_without_an_element( &(distances_matrix[i*n]) , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k , i) ;            
            //printf(" \n" );
        }
    }
    else{
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ i*n+j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same 
            extra_points_with_same_distance += k_smallest( &(distances_matrix[i*n]) , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k) ;            
            //printf(" \n" );
        }
    }

    /*
    if( n == m ){
        printf(" \n Make sure it is symmetric \n" );
        for(int i = 0 ; i < m ; i++ ){     
            printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){            
                temp_sum = distances_matrix[ i*n+j ] - distances_matrix[ i+n*j ] ;
                printf(" %.15lf " , temp_sum);
            }        
            printf(" \n" );
        }
    }
    */


    //free
    free(distances_matrix);

    return knn_found ;


}



knnresult kNN_low_memory_V2(double * X, double * Y, int n, int m, int d, int k){    
    // X has m points
    // Y has n points
    // They both have d dimention 
    
    
    bool same_points = (X == Y) ;

    // edge cases
    if ( k < 0 ){
        int what_to_set_k = 2 ;
        printf("Inside kNN, k was %d which is negative so it was set to %d  \n" , k , what_to_set_k );
        k = what_to_set_k ; 
    }
    if( k > n){
        if(same_points){
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n-1 );
            k = n-1 ; 
        }
        else{
            printf("Inside kNN, k was %d that was greater than the number of points in Y so it was set to %d (which is the number of points in Y) \n" , k , n );
            k = n ; 
        }
    }

    knnresult knn_found ;
    knn_found.nidx =     (int*) malloc( m*k*sizeof(int));
    knn_found.ndist = (double*) malloc( m*k*sizeof(double));
    knn_found.m = m ;
    knn_found.k = k ; 

    if (knn_found.nidx == NULL){             
        printf("Failed to allocate memmory for the Indices (0-based) of nearest neighbors.\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }
    if (knn_found.ndist == NULL){             
        printf("Failed to allocate memmory for the Distance of nearest neighbors  .\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // Find the distance of each point in X with each point in Y

    double* distances_matrix = (double*) malloc( n*sizeof(double) ) ;
    if ( distances_matrix == NULL){             
        printf("Failed to allocate memmory for the Distances matrix.\n");
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // X[i][dimention] = X[i*d + dimention]
    double temp_sum ; 
    int extra_points_with_same_distance = 0  ; 

    // X has m points
    // Y has n points
    // They both have d dimention 
    //printf("The D matrix is: \n");
    //printf("The knn ids matrix is: \n");
    if ( same_points ){
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same 
            extra_points_with_same_distance += k_smallest_without_an_element( &(distances_matrix[0]) , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k , i) ;            
            //printf(" \n" );
        }
    }
    else{
        for(int i = 0 ; i < m ; i++ ){     
            //printf(" %2d " , i);   
            for(int j = 0 ; j < n ; j++ ){
                temp_sum = 0 ; 
                for (int dimention = 0 ; dimention < d ; dimention++ ){
                    // (xi_d - yj_d)^2
                    temp_sum += (X[i*d + dimention] - Y[j*d + dimention])*(X[i*d + dimention] - Y[j*d + dimention]) ; 
                }
                // Sum of (xi_d - yj_d)^2
                distances_matrix[ j ] = temp_sum  ;
                //printf(" %.15lf " , temp_sum);
            }
            // Given that X and Y is the same             
            extra_points_with_same_distance += k_smallest( &(distances_matrix[0]) , &(knn_found.ndist[i*k]), &(knn_found.nidx[i*k]), n ,  k) ;            
            //printf(" \n" );
        }
    }


    //free
    free(distances_matrix);

    return knn_found ;


}



knnresult kNN_MPI(  struct knn_dataset_vector_MPI* mine_dataset, struct knn_dataset_vector_MPI* working_dataset, int knn_to_find  ){

    // min of those two 
    int k =  (*working_dataset).n < knn_to_find ? (*working_dataset).n : knn_to_find ;
    
    // if they are the same dataset then we can not find more knn than there are points-1
    if ( (*working_dataset).data_points == (*mine_dataset).data_points ){
        if ( (*mine_dataset).n == k ){
            k--;
        }
    }

    //printf("Before calling knn_lowe_memory\n");
    //knnresult temp_result_test= kNN             ((*mine_dataset).data_points, (*working_dataset).data_points, (*working_dataset).n , (*mine_dataset).n , (*working_dataset).dim,  k) ;
    knnresult temp_result = kNN_low_memory_V2   ((*mine_dataset).data_points, (*working_dataset).data_points, (*working_dataset).n , (*mine_dataset).n , (*working_dataset).dim,  k) ;
    knnresult temp_result_test = kNN_low_memory   ((*mine_dataset).data_points, (*working_dataset).data_points, (*working_dataset).n , (*mine_dataset).n , (*working_dataset).dim,  k) ;
    

    // make sure they are the same
    
    
    if (temp_result.k != temp_result_test.k){
        printf("Different k ========================================== \n");
    }
    if (temp_result.m != temp_result_test.m){
        printf("Different m ========================================== \n");
    }
    for ( int i = 0 ; i < temp_result.k * temp_result.m; i++ ){
        if( temp_result.nidx[i] !=  temp_result_test.nidx[i]  ){            
            printf("Different nidx i= %d  , correct %d wrong %d  ========================================= \n" , i ,temp_result.nidx[i] , temp_result_test.nidx[i]  );

        }
        if( temp_result.ndist[i] !=  temp_result_test.ndist[i]  ){            
            printf("Different ndist i= %d  , correct %lf wrong %lf  ========================================== \n" , i ,temp_result.ndist[i] , temp_result_test.ndist[i]  );

        }
        
    }

    free_struct_result(&temp_result_test);
    


    //printf("After calling knn_lowe_memory\n");
    // here we add the offset to the result     

    for ( int i = 0 ; i < temp_result.k * temp_result.m; i++ ){
        temp_result.nidx[i] += (*working_dataset).offset ;
        //printf("The index of the closest before offset is :%d after offset is %d  \n", temp_result.nidx[i] -(*working_dataset).offset  ,  temp_result.nidx[i] );
    }

    return temp_result ;

}

knnresult kNN_reduce_and_free( knnresult A , knnresult B , int k ){
    // Take two knnresults structs and reduce them

    knnresult reduced_result ;
    if( A.m != B.m){
        printf("Error !! A.m = %d with A.k = %d and B.m = %d with B.k = %d crash \n" , A.m , A.k, B.m , B.k );
        exit(3);
    }
    reduced_result.m = A.m;
    reduced_result.k = (k <= A.k + B.k) ? k : A.k + B.k ; //  = min(k , A.k+B.k )
    reduced_result.ndist = (double*) malloc( sizeof(double) * reduced_result.m * reduced_result.k ) ;
    reduced_result.nidx  = (int*)    malloc( sizeof(int)    * reduced_result.m * reduced_result.k ) ;
    
    // Make sure the malloc were fine :)
    if (reduced_result.nidx == NULL){             
        printf("Failed to allocate memmory for the Indices (0-based) of nearest neighbors.\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }
    if (reduced_result.ndist == NULL){             
        printf("Failed to allocate memmory for the Distance of nearest neighbors  .\n");    
        printf("malloc returned NUll pointer.\n");   
        exit(1);  
    }

    // For each point
    for( int point_id = 0 ; point_id < reduced_result.m ; point_id++ ){

        // Find the k smallest from the two results for that point
        int i_A = 0 ;
        int j_B = 0 ;
        int index_r = 0 ;

        while( index_r < reduced_result.k && i_A < A.k  && j_B < B.k  ){
            if (  A.ndist[ point_id*A.k + i_A ] < B.ndist[ point_id*B.k + j_B ]  ){
                reduced_result.ndist[point_id*reduced_result.k+index_r] = A.ndist[ point_id*A.k + i_A ] ;
                reduced_result.nidx [point_id*reduced_result.k+index_r] = A.nidx [ point_id*A.k + i_A ] ;
                i_A++;
            }
            else{
                reduced_result.ndist[point_id*reduced_result.k+index_r] = B.ndist[ point_id*B.k + j_B ] ;
                reduced_result.nidx [point_id*reduced_result.k+index_r] = B.nidx [ point_id*B.k + j_B ] ;
                j_B++;
            }
            index_r++;
        }

        // Copy the rest of the points in A to the result
        while( index_r < reduced_result.k && i_A < A.k  ){
            reduced_result.ndist[point_id*reduced_result.k+index_r] = A.ndist[ point_id*A.k + i_A ] ;
            reduced_result.nidx [point_id*reduced_result.k+index_r] = A.nidx [ point_id*A.k + i_A ] ;
            i_A++;  
            index_r++;
        }

        
        // Copy the rest of the points in B to the result
        while( index_r < reduced_result.k  && j_B < B.k  ){            
            reduced_result.ndist[point_id*reduced_result.k+index_r] = B.ndist[ point_id*B.k + j_B ] ;
            reduced_result.nidx [point_id*reduced_result.k+index_r] = B.nidx [ point_id*B.k + j_B ] ;
            j_B++;            
            index_r++;
        }


    }


    // free the input which is now uselles
    free_struct_result(&A);
    free_struct_result(&B);

    return reduced_result;

}

void culculate_sqrt( knnresult* struct_to_find_roots  ){

    int total_length = (*struct_to_find_roots).m * (*struct_to_find_roots).k ; 

    for( int i = 0 ; i < total_length ; i++ ){
        (*struct_to_find_roots).ndist[i] = sqrt( (*struct_to_find_roots).ndist[i] ) ;
    }

}


int find_number_length( int number ){

    int order = 0 ; 

    do{
        number /= 10 ;
        order++ ;
    }while( number != 0 );

    return order ;

}

void print_knnresult_ids( knnresult struct_to_print_knn_ids ){

    int m = struct_to_print_knn_ids.m ;
    int k = struct_to_print_knn_ids.k ;
    int digits_to_print = find_number_length(m-1);

    printf("The result has %d points and %d knn \n\n" , m , k  );

    for( int i = 0 ; i < m ; i++ ){
        printf("ID: %*d :" , digits_to_print, i ) ;
        for( int j = 0 ; j < k ; j++ ){
            printf(" %*d " , digits_to_print ,struct_to_print_knn_ids.nidx[i*k+j]);
            
        }
        printf("\n");
    }

    printf("\n");
    printf("The result has %d points and %d knn \n" , m , k  );

}

void print_knnresult_ids_with_offset( knnresult struct_to_print_knn_ids , int offset , int total_points){

    int m = struct_to_print_knn_ids.m ;
    int k = struct_to_print_knn_ids.k ;
    int digits_to_print = find_number_length(total_points-1);

    printf("The result has %d points and %d knn \n\n" , m , k  );

    for( int i = 0 ; i < m ; i++ ){
        printf("ID: %*d :" , digits_to_print, i+offset ) ;
        for( int j = 0 ; j < k ; j++ ){
            printf(" %*d " , digits_to_print ,struct_to_print_knn_ids.nidx[i*k+j]);
            
        }
        printf("\n");
    }

    printf("\n");
    printf("The result has %d points and %d knn \n" , m , k  );

}


void print_knnresult_distances( knnresult struct_to_print_knn_ids ){

    int m = struct_to_print_knn_ids.m ;
    int k = struct_to_print_knn_ids.k ;
    int digits_to_print = find_number_length(m-1);

    printf("The result has %d points and %d knn \n\n" , m , k  );

    for( int i = 0 ; i < m ; i++ ){
        printf("ID: %*d :" , digits_to_print, i ) ;
        for( int j = 0 ; j < k ; j++ ){
            printf(" %.15lf "  ,struct_to_print_knn_ids.ndist[i*k+j]);
            
        }
        printf("\n");
    }

    printf("\n");
    printf("The result has %d points and %d knn \n" , m , k  );

}


void free_struct_result(knnresult* knn_to_free){

    free( (*knn_to_free).ndist );
    free( (*knn_to_free).nidx );

}












