
#include "load_examples.h"


#include "my_structs.h"
#include "find_knn.h"
#include "helpers.h"



// ======================================= Menu =======================================

void scanf_check( int number_of_output_arguments_expected , int actual_output_arguments   ){

    if( number_of_output_arguments_expected != actual_output_arguments){
        printf("Error scanning values, Expected %d but read %d \n" ,number_of_output_arguments_expected, actual_output_arguments );
        exit(1);
    }
}

char* select_file_to_load(){
    
    // "../knn_datasets_zero_based/knn_dataset0.txt" 
    char base_name[100] = "../knn_datasets_zero_based/knn_dataset" ;
    int number = 0;

    printf("\n");
    printf("Select dataset ID  / give '-1' to show availiable datasets / give '-2' to specify relative path to a file: ");      
    fflush(stdout);
    scanf_check(1,  scanf("%d", &number));

    if( number == -2 ){
        
        char* file_relative_path = (char*)malloc(1024*sizeof(char));

        printf("Enter the relative path to the file: ");
        fflush(stdout);
        scanf_check( 1, scanf("%1023s", file_relative_path));
      
        if (print_dataset_parameters_and_free(file_relative_path, false)){
            // This means the file was opened and the parameters of the file were printed 
            return file_relative_path ;
        }
        else{
            printf("The file could not open, perhaps it does not exist, try again.\n");
            printf("File that tried to open was: %s\n",file_relative_path);
            free(file_relative_path);
            return select_file_to_load();
        }



    }
    else if ( number != -1 ){
        // Print the modified string
        char* file_name =  generate_filename(base_name , number) ;
        //printf("%s\n", file_name);
        if (print_dataset_parameters_and_free(file_name , false)){
            return file_name; 
        }
        else{
            printf("The file could not open, perhaps it does not exist, try again.\n");
            printf("File that tried to open was: %s\n",file_name);
            return select_file_to_load();
        }
        
    }
    else{
        // Search and print the files with a specified format 
        number = 0 ; 
        printf("\n");
        while( print_dataset_parameters_and_free (generate_filename(base_name , number++) , true  ) ); //{ }
        // This while has an empty body!
        return select_file_to_load();
    }

}


char* generate_filename(char* base_name, int num){
    // Allocate memory for the new filename
    char* filename = malloc(strlen(base_name) + 32);  // 32 = strlen(".txt") + 1 (for null terminator) + 20 size of int in decimal places + extra 

    // Concatenate the base name, the number, and ".txt" to create the new filename
    sprintf(filename, "%s%d.txt", base_name, num);

    return filename;
}




bool print_dataset_parameters_and_free ( char* file_name , bool free_file_name ){
    // Variable to open file
    FILE *f;

    if ((f = fopen(file_name, "r")) == NULL) {
        //printf("Could not open file '%s' it returned a NULL pointer.\n",file_name);
        // File maybe does not exist 
        if(free_file_name){
            free(file_name);
        }
        return false ; 
    }

    int n , dim , k  ;    
    scanf_check( 3 , fscanf(f, "%d,%d,%d\n", &n , &dim , &k  ) );    

    printf("File: %s  \t" , file_name);
    printf("| %5d points | %5d dimentions | %3d knn precomputed |\n" ,n , dim , k  );
                   
    fclose(f);
    
    if( free_file_name){
        free(file_name);
    }

    return true ; 
}

bool does_the_file_exist( char* file_name ) {
    // Variable to open file
    FILE *f;
    if ((f = fopen(file_name, "r")) == NULL) {
        //printf("Could not open file '%s' it returned a NULL pointer.\n",file_name);
        // File maybe does not exist         
        return false ; 
    }
    return true ; 
}


int check_knn_request( int k_requested , char* file_name){
    // Variable to open file
    FILE *f;

    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Could not verify correct input for the k requestede because ");
        printf("I could not open file '%s' it returned a NULL pointer.\n",file_name);
        // File maybe does not exist 
        return k_requested ;
    }

    int n , dim , k  ;    
    scanf_check(3 , fscanf(f, "%d,%d,%d\n", &n , &dim , &k  ) );       
    fclose(f);

    int k_final = k_requested ;
    if ( k_requested == -1 ){
        // Then set it to the number of precomputed knn
        k_final = k ;
        printf("k was set to the number of precomputed knn which is : %d \n", k_final);
    }
    else if ( k_requested == -2 ){
        // Then set it to the max number possible
        k_final = n-1;        
        printf("k was set to maximum number of knn which is : %d \n", k_final);
    }
    else{
        if( k_requested < 0 ){
            // Then i guess throw an error
            k_final = (n>2) ? 2 : n ; // min(n,2)            
            printf("k was set to %d because input was negative\n", k_final);

        }
        else if ( k_requested >= n ){
            // Then set it to the max number possible
            k_final = n-1 ;
            printf("k was set to maximum number of knn which is : %d \n", k_final);
        }
    }

    return k_final;

}


void run_on_all_datasets(){
    
    // "knn_datasets_zero_baced/knn_dataset0.txt" 
    char base_name[100] = "knn_datasets_zero_baced/knn_dataset" ;
    int number = 0;
    char* file_to_load = generate_filename(base_name , number) ;
    while( print_dataset_parameters_and_free ( file_to_load , false  ) ){ 
        // This means the file_to_load exist 

        // Load and print data in ~~~ vector ~~~ format
        struct knn_dataset_vector  dataset_to_load_vector ;
        dataset_to_load_vector.file_name = file_to_load ;
        load_data_points_and_ans_vector(&dataset_to_load_vector);

        int knn_to_culculate = dataset_to_load_vector.k  ; 
        //knnresult final_knn =  kNN( dataset_to_load_vector.data_points ,(dataset_to_load_vector.data_points), dataset_to_load_vector.n, dataset_to_load_vector.n, dataset_to_load_vector.dim, knn_to_culculate );
        knnresult final_knn =  kNN_low_memory( dataset_to_load_vector.data_points ,(dataset_to_load_vector.data_points), dataset_to_load_vector.n, dataset_to_load_vector.n, dataset_to_load_vector.dim, knn_to_culculate );

        //printf("\n");
        
        check_correct_knn(dataset_to_load_vector ,final_knn );

        // write a funtion that checks the result if it is the same as the matlab culculated one 
        //printf("\n");
        free_dataset_struct_vector(&dataset_to_load_vector);

        // culculate the sqrt for the distances !!! 
        
        free_struct_result(&final_knn);

        number++;
        free(file_to_load);
        
        file_to_load = generate_filename(base_name , number) ;

    }
    free(file_to_load);

    exit(0);

}

// ======================================= matrix =======================================
void load_data_points_and_ans(struct knn_dataset *  dataset_to_load ){
    char* file_name = (*dataset_to_load).file_name; 

    FILE *f;

    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Could not open file it returned a NULL pointer.\n");
        exit(1);
    }
    
    //printf("=============  File loading %s   ===============\n" , file_name);

    
    int n , dim , k  ;    
    scanf_check( 3,  fscanf(f, "%d,%d,%d\n", &n , &dim , &k  ) );    
    //printf("\nThe dataset has %d points with %d dimentions and %d knn precomputed\n" ,n , dim , k  );

    (*dataset_to_load).n = n ;
    (*dataset_to_load).dim = dim ;
    (*dataset_to_load).k = k ; 

    (*dataset_to_load).data_points = (double**) malloc(n * sizeof(double*));    
    (*dataset_to_load).knn_id = (int**) malloc(n * sizeof(int*));
    if ( (*dataset_to_load).data_points == NULL    ){        
        printf("Failed to allocate memmory for the data points.\n");    
        printf("malloc returned NUll pointer.\n");   
        fclose(f);
        exit(1);     
    }
    if ( (*dataset_to_load).knn_id == NULL    ){        
        printf("Failed to allocate memmory for the knn_id.\n");    
        printf("malloc returned NUll pointer.\n");               
        fclose(f);
        exit(1);     
    }
    
    for( int i = 0 ; i < n ; i ++){
        (*dataset_to_load).data_points[i] = (double*) malloc(dim * sizeof(double));    
        (*dataset_to_load).knn_id[i] = (int*) malloc(k * sizeof(int));

         if ( (*dataset_to_load).data_points[i] == NULL    ){        
            printf("Failed to allocate memmory for the data points.\n");    
            printf("malloc returned NUll pointer.\n");                 
            fclose(f);      
            exit(1);
        }
        if ( (*dataset_to_load).knn_id[i] == NULL    ){        
            printf("Failed to allocate memmory for the knn_id.\n");    
            printf("malloc returned NUll pointer.\n");                
            fclose(f);       
            exit(1);
        }

    }


    for( int line = 0 ; line < n ; line ++){
        // Load coordinates
        for( int i = 0 ; i < dim ; i ++){            
            scanf_check( 1 , fscanf(f, "%lf,", &((*dataset_to_load).data_points[line][i])  ) ); 
        }   
        // Load the id of the knn for each point
        for( int i = 0 ; i < k ; i ++){            
            scanf_check( 1 , fscanf(f, "%d,", &(*dataset_to_load).knn_id[line][i]  ) );
        }            
    }

    fclose(f);



}


void print_dataset_points( struct knn_dataset*  dataset_to_print ){
    printf("\nThere are %d points and they have %d dimentions.\n\n" , (*dataset_to_print).n , (*dataset_to_print).dim);
    for( int i = 0 ; i < (*dataset_to_print).n ; i ++){
        
        printf("ID: %3d |" , i );  
        for( int j =0 ; j < (*dataset_to_print).dim ; j++){
            printf(" %.15lf " , (*dataset_to_print).data_points[i][j] );  
        }
        printf("\n");
    }    
    printf("\n");
    
}

void print_dataset_knn_ids( struct knn_dataset*  dataset_to_print , int how_many_knn ){

    /*
    if( how_many_knn == NULL){
        how_many_knn = (*dataset_to_print).k ;        
        printf("The number of knn that will be printed is set to the maximum precomputed which is %d. \n" , how_many_knn);
    }
    */
    
    if ( how_many_knn > (*dataset_to_print).k ){        
        printf("You asked to print %d knn ids which is more than there are in the dataset precomputed.\n" ,how_many_knn );  
        how_many_knn = (*dataset_to_print).k ;        
        printf("The number of knn that will be printed is set to %d. \n" , how_many_knn);
    }


    printf("\nThere are %d points and the ids of %d knn are.\n\n" , (*dataset_to_print).n , how_many_knn);
    for( int i = 0 ; i < (*dataset_to_print).n ; i ++){
        
        printf("ID: %3d |" , i );  
        for( int j =0 ; j < how_many_knn ; j++){
            printf(" %3d " , (*dataset_to_print).knn_id[i][j] );  
        }
        printf("\n");
    }    
    printf("\n");
    
}

void free_dataset_struct( struct knn_dataset*  dataset_to_free ){

    // free the inside 
    for( int i = 0 ; i < (*dataset_to_free).n ; i ++){
        free((*dataset_to_free).data_points[i]);
        free((*dataset_to_free).knn_id[i]);
    }
    
    // free the double pointers
    free((*dataset_to_free).data_points);
    free((*dataset_to_free).knn_id);

}

// ======================================= vector =======================================
void load_data_points_and_ans_vector( struct knn_dataset_vector*  dataset_to_load_vector ){

    char* file_name = (*dataset_to_load_vector).file_name; 

    FILE *f;

    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Could not open file it returned a NULL pointer.\n");
        exit(1);
    }
    
    //printf("=============  File loading %s   ===============\n" , file_name);

    
    int n , dim , k  ;    
    scanf_check(3 , fscanf(f, "%d,%d,%d\n", &n , &dim , &k  ) );    
    //printf("\nThe dataset has %d points with %d dimentions and %d knn precomputed\n" ,n , dim , k  );

    (*dataset_to_load_vector).n = n ;
    (*dataset_to_load_vector).dim = dim ;
    (*dataset_to_load_vector).k = k ; 

    (*dataset_to_load_vector).data_points = (double*) malloc(n*dim * sizeof(double));    
    (*dataset_to_load_vector).knn_id = (int*) malloc(n * k * sizeof(int));
    if ( (*dataset_to_load_vector).data_points == NULL    ){        
        printf("Failed to allocate memmory for the data points.\n");    
        printf("malloc returned NUll pointer.\n");                       
        fclose(f);
        exit(1);
    }
    if ( (*dataset_to_load_vector).knn_id == NULL    ){        
        printf("Failed to allocate memmory for the knn_id.\n");    
        printf("malloc returned NUll pointer.\n");               
        fclose(f);
        exit(1);       
    }
   


    for( int line = 0 ; line < n ; line ++){
        // Load coordinates
        for( int i = 0 ; i < dim ; i ++){            
            scanf_check(1 , fscanf(f, "%lf,", &((*dataset_to_load_vector).data_points[line*dim+ i])  ) );
        }   
        // Load the id of the knn for each point
        for( int i = 0 ; i < k ; i ++){            
            scanf_check( 1,  fscanf(f, "%d,", &(*dataset_to_load_vector).knn_id[line*k+i]  ) );
        }            
    }

                   
    fclose(f);

}

void print_dataset_points_vector( struct knn_dataset_vector*  dataset_to_print_vector ){
    printf("\nThere are %d points and they have %d dimentions.\n\n" , (*dataset_to_print_vector).n , (*dataset_to_print_vector).dim);
    for( int i = 0 ; i < (*dataset_to_print_vector).n ; i ++){
        
        printf("ID: %3d |" , i );  
        for( int j =0 ; j < (*dataset_to_print_vector).dim ; j++){
            printf(" %.15lf " , (*dataset_to_print_vector).data_points[i*(*dataset_to_print_vector).dim +j] );  
        }
        printf("\n");
    }    
    printf("\n");
    
}


void print_dataset_knn_ids_vector( struct knn_dataset_vector*  dataset_to_print_vector , int how_many_knn ){
    /*
    if( how_many_knn == NULL){
        how_many_knn = (*dataset_to_print_vector).k ;        
        printf("The number of knn that will be printed is set to the maximum precomputed which is %d. \n" , how_many_knn);
    }
    */
    
    if ( how_many_knn > (*dataset_to_print_vector).k ){        
        printf("You asked to print %d knn ids which is more than there are in the dataset precomputed.\n" ,how_many_knn );  
        how_many_knn = (*dataset_to_print_vector).k ;        
        printf("The number of knn that will be printed is set to %d. \n" , how_many_knn);
    }


    printf("\nThere are %d points and the ids of %d knn are.\n\n" , (*dataset_to_print_vector).n , how_many_knn);
    for( int i = 0 ; i < (*dataset_to_print_vector).n ; i ++){
        
        printf("ID: %3d |" , i );  
        for( int j =0 ; j < how_many_knn ; j++){
            printf(" %3d " , (*dataset_to_print_vector).knn_id[i*(*dataset_to_print_vector).k +j] );  
        }
        printf("\n");
    }    
    printf("\n");
}

void free_dataset_struct_vector( struct knn_dataset_vector*  dataset_to_free_vector ){
   
    // free the double pointers
    free((*dataset_to_free_vector).data_points);
    free((*dataset_to_free_vector).knn_id);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MPI ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void load_data_points_MPI_vector( struct knn_dataset_vector_MPI*  dataset_to_load_vector , int rank , int total_processes){

    char* file_name = (*dataset_to_load_vector).file_name; 

    FILE *f;

    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Rank %d Could not open file it returned a NULL pointer.\n" , rank);
        exit(1);
    }
    
    //printf("=============  File loading %s   ===============\n" , file_name);

    
    int n , dim , k  ;    
    scanf_check(3, fscanf(f, "%d,%d,%d\n", &n , &dim , &k  ) );
    //printf("\nThe dataset has %d points with %d dimentions and %d knn precomputed\n" ,n , dim , k  );
    int n_total = n ;
    int n_most = n / total_processes ;
    int n_last = n_most ;

    // If n_total is not divisible by total_processes
    if( n_total % total_processes != 0){
        // Then the majority will take one more
        n_most++;
        // The last process will take less (its not way less the maximum less it can take is as many as there are processes -1 ) (in a very special senario when n_total = )
        n_last = n_total % n_most ;
        
    }

    if ( n_most * ( total_processes - 1 ) + n_last !=  n_total ){
        if( rank == 0 ){
            printf("The number of points is %d and the number of processes is %d , the points could not load in this manny processes, try again with less processes. \n", n_total , total_processes);
        }
        //debug(rank,"Error with the number of points to load, %d  vs the real %d \n" , n_most * ( total_processes - 1 ) + n_last , n_total);
        exit(1);
    }


    

    // Here we will make n = n_mine ,  if we are the last process then it will be less
    n = ( rank != total_processes - 1) ? n_most : n_last ;

    (*dataset_to_load_vector).n = n ;
    (*dataset_to_load_vector).n_most = n_most ;         // Extra
    (*dataset_to_load_vector).n_total = n_total ;       // Extra
    (*dataset_to_load_vector).offset = rank*n_most ;    // Extra
    (*dataset_to_load_vector).dim = dim ;
    (*dataset_to_load_vector).k = k ; 
    

    (*dataset_to_load_vector).data_points = (double*) malloc(n*dim * sizeof(double));    
    (*dataset_to_load_vector).knn_id = (int*) malloc(n * k * sizeof(int));
    if ( (*dataset_to_load_vector).data_points == NULL    ){        
        printf("Rank %d Failed to allocate memmory for the data points, malloc returned NUll pointer.\n" , rank);                       
        fclose(f);
        exit(1);
    }
    if ( (*dataset_to_load_vector).knn_id == NULL    ){        
        printf("Rank %d Failed to allocate memmory for the knn_id, malloc returned NUll pointer.\n", rank);               
        fclose(f);
        exit(1);       
    }
   

    // Skip the previous n_most*rank elements
    //debug(rank,"Before skiping most elements\n");
    for( int line = 0 ; line < n_most*rank ; line ++){
        // Load coordinates
        for( int i = 0 ; i < dim ; i ++){            
            scanf_check( 0, fscanf(f, "%*f," ) );
        }   
        // Load the id of the knn for each point
        for( int i = 0 ; i < k ; i ++){            
            scanf_check(0, fscanf(f, "%*d," ));
        }            
    }
    //debug(rank,"After skiping most elements\n");
    // Read my lines n changes depending on the rank, so it will work for the last process as well 
    for( int line = 0 ; line < n ; line ++){
        // Load coordinates
        
        for( int i = 0 ; i < dim ; i ++){            
            scanf_check(1, fscanf(f, "%lf,", &((*dataset_to_load_vector).data_points[line*dim+ i])  ) );
        }   
        // Load the id of the knn for each point
        for( int i = 0 ; i < k ; i ++){            
            scanf_check(1, fscanf(f, "%d,", &(*dataset_to_load_vector).knn_id[line*k+i]  ) );
        }            
    }  
    

                   
    fclose(f);


}

void print_dataset_points_MPI_vector( struct knn_dataset_vector_MPI*  dataset_to_print_vector , int rank ){
    printf("\nRank %d| There are %d points and they have %d dimentions.\n\n" , rank ,  (*dataset_to_print_vector).n , (*dataset_to_print_vector).dim);
    for( int i = 0 ; i < (*dataset_to_print_vector).n ; i ++){
        
        printf("Rank %d| ID:%*d |" , rank , find_number_length( (*dataset_to_print_vector).n_total ) ,i + (*dataset_to_print_vector).offset  );  
        for( int j =0 ; j < (*dataset_to_print_vector).dim ; j++){
            //printf(" %.15lf " , (*dataset_to_print_vector).data_points[i*(*dataset_to_print_vector).dim +j] );  
            //printf(" %15lf " , (*dataset_to_print_vector).data_points[i*(*dataset_to_print_vector).dim +j] );  
            printf(" %15f " , (*dataset_to_print_vector).data_points[i*(*dataset_to_print_vector).dim +j] );  
        }
        printf("\n");
    }    
    printf("\n");
    
}


void initialize_datasets(  struct knn_dataset_vector_MPI* process_dataset , struct knn_dataset_vector_MPI*  working_dataset , struct knn_dataset_vector_MPI*  receiving_dataset , int rank){
    
    (*working_dataset).file_name    = (*process_dataset).file_name ;
    (*receiving_dataset).file_name  = (*process_dataset).file_name ; 

    
    (*working_dataset).dim     = (*process_dataset).dim ;
    (*receiving_dataset).dim   = (*process_dataset).dim ; 

    
    (*working_dataset).k     = (*process_dataset).k ;
    (*receiving_dataset).k   = (*process_dataset).k ; 
    

    (*working_dataset).n_most     = (*process_dataset).n_most ;
    (*receiving_dataset).n_most   = (*process_dataset).n_most ; 
    

    (*working_dataset).n_total     = (*process_dataset).n_total ;
    (*receiving_dataset).n_total   = (*process_dataset).n_total ; 

    // Where we expect the data we receive to come 
    (*working_dataset).data_points     = (double*) malloc( (*process_dataset).dim * (*process_dataset).n_most * sizeof(double) ) ; 
    (*receiving_dataset).data_points   = (double*) malloc( (*process_dataset).dim * (*process_dataset).n_most * sizeof(double) ) ; 

    if ( (*working_dataset).data_points == NULL ){
        printf("Rank %d Failed to allocate memmory for the data points on the working dataset, (malloc returned NUll pointer)\n" , rank) ;
        exit(1);
    }
    
    if ( (*receiving_dataset).data_points == NULL ){
        printf("Rank %d Failed to allocate memmory for the data points on the receiving dataset, (malloc returned NUll pointer)\n" , rank) ;
        exit(1);
    }
    
    
    (*working_dataset).n     =  0 ;
    (*receiving_dataset).n   =  0 ; 

    (*working_dataset).knn_id     =  NULL ;
    (*receiving_dataset).knn_id   =  NULL ; 
    

}


void free_dataset_MPI_struct( struct knn_dataset_vector_MPI dataset_to_free){

    if ( dataset_to_free.knn_id != NULL){
        free(dataset_to_free.knn_id) ; 
    }

    if (  dataset_to_free.data_points != NULL   ){
        free(dataset_to_free.data_points) ;
        
    }

}



bool check_correct_knn ( struct knn_dataset_vector  dataset_to_check_vector , knnresult algorithm_found_knn ) {
    // Using the precomputed knn IDs in the file it checks to make sure the algorithm found the correct knn :)


    int k_dataset = dataset_to_check_vector.k ; 
    int k_knn = algorithm_found_knn.k ;

    if ( dataset_to_check_vector.n == 0 ){
        printf("There are no points to check for correct results.\n");
        return false;
    }

    if( k_knn == 0 ){
        printf("The knn algorithm wasn't asked to culculate any knn so there can be no check for correct results.\n");
        return false;
    }

    if( k_dataset == 0 ){
        printf("The dataset does not have precomputed knn so there can be no double check if the results are correct.\n");
        return false;
    }

    bool checked_all = true ;
    if ( k_dataset < k_knn ){
        printf("The precomputed knns are less than the ones found by the knn algorithm, so only the first %d of the %d will be checked\n" , k_dataset, k_knn);
        checked_all = false ;
    }
    int k_min = (k_dataset > k_knn) ? k_knn : k_dataset ; 
    bool correct = true ; 
    int number_of_mistakes = 0 ;

    for( int i = 0 ; i < k_min ; i++  ){
        for( int j = 0 ; j < algorithm_found_knn.m ; j++ ){
            if ( algorithm_found_knn.nidx[j*k_knn + i] != dataset_to_check_vector.knn_id[j*k_dataset+i] ){
                //Then they are wrong
                correct = false ;
                number_of_mistakes++;
                printf("For i = %2d and j = %2d the correct is %2d but found %2d \n", i , j ,dataset_to_check_vector.knn_id[j*k_dataset+i] ,algorithm_found_knn.nidx[j*k_knn + i] );
            }
            /*
            else{
                printf("For i = %2d and j = %2d the correct is %2d AND found %2d \n", i , j ,dataset_to_check_vector.knn_id[j*k_dataset+i] ,algorithm_found_knn.nidx[j*k_knn + i] );
            
            }
            */
        }
    }

    if ( correct ){
        printf("The knn algorithm results were double checked with the precomputed (which were verified in matlab).\n");
    }
    else{
        printf("Mistake finding the knn !\n");
    }

    return correct && checked_all ;
}




bool check_correct_knn_MPI ( struct knn_dataset_vector_MPI  dataset_to_check_vector , knnresult algorithm_found_knn ) {
    // Using the precomputed knn IDs in the file it checks to make sure the algorithm found the correct knn :)


    int k_dataset = dataset_to_check_vector.k ; 
    int k_knn = algorithm_found_knn.k ;

    if ( dataset_to_check_vector.n == 0 ){
        printf("There are no points to check for correct results.\n");
        return true;
    }

    if( k_knn == 0 ){
        printf("The knn algorithm wasn't asked to culculate any knn so there can be no check for correct results.\n");
        return true;
    }

    if( k_dataset == 0 ){
        printf("The dataset does not have precomputed knn so there can be no double check if the results are correct.\n");
        return true;
    }

    bool checked_all = true ;
    if ( k_dataset < k_knn ){
        printf("The precomputed knns are less than the ones found by the knn algorithm, so only the first %d of the %d will be checked\n" , k_dataset, k_knn);
        checked_all = false ;
    }

    int k_min = (k_dataset > k_knn) ? k_knn : k_dataset ; 
    bool correct = true ; 
    int number_of_mistakes = 0 ;

    for( int i = 0 ; i < k_min ; i++  ){
        for( int j = 0 ; j < algorithm_found_knn.m ; j++ ){
            if ( algorithm_found_knn.nidx[j*k_knn + i] != dataset_to_check_vector.knn_id[j*k_dataset+i] ){
                //Then they are wrong
                correct = false ;
                number_of_mistakes++;
                printf("For i = %2d and j = %2d the correct is %2d but found %2d \n", i , j ,dataset_to_check_vector.knn_id[j*k_dataset+i] ,algorithm_found_knn.nidx[j*k_knn + i] );
            }
            /*
            else{
                printf("For i = %2d and j = %2d the correct is %2d AND found %2d \n", i , j ,dataset_to_check_vector.knn_id[j*k_dataset+i] ,algorithm_found_knn.nidx[j*k_knn + i] );
            
            }
            */
        }
    }

    if ( correct ){
        printf("The knn algorithm results were double checked with the precomputed (which were verified in matlab).\n");
    }
    else{
        printf("Mistakes finding the knn are: %d  ! <==============================\n" , number_of_mistakes);
    }

    return correct && checked_all ;
}