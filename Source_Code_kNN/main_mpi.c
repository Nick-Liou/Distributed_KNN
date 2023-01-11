#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <mpi.h>


#include "my_structs.h"

#include "load_examples.h"
#include "find_knn.h"
#include "helpers.h"

enum mpi_tags { send_data_points = 0 , receive_data_points = 0 , 
                send_number_of_data_points = 1 , receive_number_of_data_points = 1 ,
                send_offset = 2 , receive_offset = 2,
                send_distances = 3 , receive_distances = 3 ,
                send_knn_ids = 4 , receive_knn_ids = 4 
                };

// compile with make 
// run with make run 

// By: Liouliakis Nikolaos 10058

int main(int argc, char** argv){

    // Initialize MPI
	MPI_Init(&argc, &argv);

    // Time stuff
    struct timespec start, end;    
    double time_to_load, time_to_find_kNN, elapsed_total;
    

	// Get the rank and size of the current process
	int rank, number_of_processes;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
	
	int broadcaster_rank = 0 ;

    if ( broadcaster_rank >= number_of_processes ){
        broadcaster_rank = 0 ;
    }
	
	if( rank == broadcaster_rank ){
		printf("\n");
		printf("Hello, World! From main process with rank %d from a total of %d \n" , rank , number_of_processes );
	}
	

    char* file_to_load ;
	int knn_to_culculate ;
    // Max size of file_to_load is 1024 char, 
	if ( rank == broadcaster_rank){
        // Choose file to load
        
        // File name example 
        // "knn_datasets_zero_based/knn_dataset0.txt" ;
        file_to_load = select_file_to_load();
        
        //debug(rank , "File to load %s \n" , file_to_load);


        knn_to_culculate = 0;    
        printf("Enter how many knn to find: ");
        fflush(stdout);
        scanf_check( 1 , scanf("%d", &knn_to_culculate)   );
        printf("\n");
        knn_to_culculate = check_knn_request( knn_to_culculate ,  file_to_load );
        fflush(stdout);
    }
    else{
        file_to_load = (char*) malloc(1024*sizeof(char));
        sleep(5);
    }

    
    //debug(rank, "Waiting for barrier\n");
	//MPI_Barrier(MPI_COMM_WORLD);
    
    // Broadcast the name of the file to load
    MPI_Bcast( (void*) &(file_to_load[0]), 1024 , MPI_CHAR , broadcaster_rank, MPI_COMM_WORLD );

    // Broadcast the number of knn_to_culculate
    MPI_Bcast( (void*) &knn_to_culculate , 1 , MPI_INT , broadcaster_rank, MPI_COMM_WORLD );

    

    
    //debug(rank , "knn to find %d and file to load %s \n" , knn_to_culculate ,  file_to_load);

    // Make sure all processes can find and open the file 
    bool can_i_find_the_file = does_the_file_exist(file_to_load);
    bool can_all_processes_find_the_file = false ; 
    // Make sure all processes can find the file.
    MPI_Allreduce( &can_i_find_the_file, &can_all_processes_find_the_file , 1 ,    MPI_C_BOOL ,    MPI_LAND ,    MPI_COMM_WORLD);
    //debug(rank , "Value send is %d  and reduced value %d \n", can_i_find_the_file , can_all_processes_find_the_file  );

    //MPI_Allreduce(    void* send_data,    void* recv_data,    int count,    MPI_Datatype datatype,    MPI_Op op,    MPI_Comm communicator);

    if ( !can_i_find_the_file ){
        // Then the file was found by this process :)
        debug(rank , "Could not find the file %s \n", file_to_load);
    }

    if( !can_all_processes_find_the_file){
        // If some process did not find the file end the program

        // Finalize MPI
        MPI_Finalize();

        if ( rank == 0 ){
            debug(rank , "At least one process counld not find/open the file %s \n" , file_to_load);
        }
        free(file_to_load);
        return 0 ;
    }

    
    bool extensive_prints = false ; 
    bool extensive_sleeps = false ; 
    //if (extensive_prints)
    //if(extensive_sleeps)

    // At this point we have read the file name to load and made sure all processes can open it 
    // and also they all have the requested number of knn to find
    
    // Start time mesurment for loading     ~~~~~
    clock_gettime(CLOCK_MONOTONIC, &start);

    struct knn_dataset_vector_MPI  process_dataset ; 
    process_dataset.file_name = file_to_load ;    
    load_data_points_MPI_vector( &process_dataset ,  rank , number_of_processes);

    if (extensive_prints) sleep(rank); // sleep is needed to not blend the prints, maybe it should be higher
    if (extensive_prints) print_dataset_points_MPI_vector(  &process_dataset  , rank );

    // Find time it took to load the file   ~~~~~    
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_to_load = (double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_nsec - start.tv_nsec) / 1000000;

    
    // Start time mesurment for kNN             ~~~~~
    clock_gettime(CLOCK_MONOTONIC, &start);
        
    // Now each process has loaded it's data

    struct knn_dataset_vector_MPI  working_dataset ; 
    struct knn_dataset_vector_MPI  receiving_dataset ; 
    // In this function we malloc for the dataset points we expect to get/sent
    initialize_datasets( &process_dataset , &working_dataset , &receiving_dataset , rank);

    bool is_it_fist_iteration = true ;
    struct knn_dataset_vector_MPI*  working_dataset_pointer     = &process_dataset   ; 
    struct knn_dataset_vector_MPI*  receiving_dataset_pointer   = &receiving_dataset ; 
    struct knn_dataset_vector_MPI*  temp_dataset_pointer ;
    
    
    // Find to who i send and from who i get data
    int rank_next = (rank+1) % number_of_processes ;
    int rank_previous = (rank == 0) ? number_of_processes-1 : rank-1 ;
    // Send data to the next and get data from the previous


    knnresult knn_of_my_points ;
    knnresult knn_of_my_points_vs_working ;


    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Start loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    for ( int i = 0 ; i < number_of_processes - 1 ; i++ ){
        if (extensive_prints) debug(rank, "About to send data <<<<<<\n");
        // MPI_Request request_send_data_points_size   ,   request_receive_data_points_size    ;
        MPI_Request request_send_offset             ,   request_recieve_offset              ;
        MPI_Request request_send_data_points        ,   request_receive_data_points         ;
        
        // int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,int tag, MPI_Comm comm, MPI_Request *request);
        // MPI_Isend( &( (*working_dataset_pointer).n )       , 1 , MPI_INT , rank_next , send_number_of_data_points  , MPI_COMM_WORLD , &request_send_data_points_size  );
        MPI_Isend( &( (*working_dataset_pointer).offset )  , 1 , MPI_INT , rank_next , send_offset                 , MPI_COMM_WORLD , &request_send_offset  );
        MPI_Isend( &( (*working_dataset_pointer).data_points[0] ) , (*working_dataset_pointer).n * process_dataset.dim , MPI_DOUBLE , rank_next , send_data_points , MPI_COMM_WORLD , &request_send_data_points  );

        //int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
        // MPI_Irecv( &( (*receiving_dataset_pointer).n )     , 1 , MPI_INT , rank_previous, receive_number_of_data_points     , MPI_COMM_WORLD , &request_receive_data_points_size);
        MPI_Irecv( &( (*receiving_dataset_pointer).offset ), 1 , MPI_INT , rank_previous, receive_offset                    , MPI_COMM_WORLD , &request_recieve_offset);
        MPI_Irecv( &( (*receiving_dataset_pointer).data_points[0] ), process_dataset.n_most * process_dataset.dim, MPI_DOUBLE , rank_previous, receive_data_points , MPI_COMM_WORLD , &request_receive_data_points);

        if (extensive_prints) debug(rank, "After send data <<<<<<\n");


        // find knn for mine data  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ do stuff ~~~~~~~~~~~~~~~~
        if (extensive_sleeps) sleep(rank);
        if (extensive_prints) debug(rank , "Before calling knn_MPI ======\n");
        if (extensive_prints) printf("\nRank %d| There are %d points and they have %d dimentions in the working dataset.\n" , rank ,  (*working_dataset_pointer).n , (*working_dataset_pointer).dim);
        
        // Calculate stuff 
        knn_of_my_points_vs_working = kNN_MPI(  &process_dataset , working_dataset_pointer ,  knn_to_culculate  ) ;
        
        if (extensive_prints) debug(rank , "Mine vs working \n");
        if (extensive_prints) print_knnresult_ids_with_offset( knn_of_my_points_vs_working , process_dataset.offset , process_dataset.n_total );

        // Reduce the result 
        if(is_it_fist_iteration){
            knn_of_my_points = knn_of_my_points_vs_working ;
        }
        else{
            knn_of_my_points = kNN_reduce_and_free(knn_of_my_points , knn_of_my_points_vs_working , knn_to_culculate);
        }

        // Print the reduced knnresult
        if (extensive_prints) debug(rank , "Reduced knn result  \n");
        if (extensive_prints) print_knnresult_ids_with_offset( knn_of_my_points , process_dataset.offset , process_dataset.n_total );
        if (extensive_prints) debug(rank , "After calling knn_MPI ======\n");
        if (extensive_prints) debug(rank, "About to recieve data @@@@@@@\n");
        
    

        

        // reduce those 

        // MPI_Status status_send_data_points_size ,   status_receive_data_points_size ;
        MPI_Status status_send_offset           ,   status_recieve_offset           ;
        MPI_Status status_send_data_points      ,   status_receive_data_points      ;

        
        // Wait for the ones I send
        // MPI_Wait( &request_send_data_points_size    , &status_send_data_points_size     );
        MPI_Wait( &request_send_offset              , &status_send_offset               );
        MPI_Wait( &request_send_data_points         , &status_send_data_points          );

        // Wait for the ones i recieve
        // MPI_Wait( &request_receive_data_points_size , &status_receive_data_points_size  );
        MPI_Wait( &request_recieve_offset           , &status_recieve_offset            );
        MPI_Wait( &request_receive_data_points      , &status_receive_data_points       );

        // This could have been send but it is probably faster this way  this is wrong !!!!!!!!!!!!!!!!!!
        //int guess_for_recieved_size = ( (*receiving_dataset_pointer).offset != process_dataset.n_total - (*receiving_dataset_pointer).n_most * (number_of_processes - 1) ) ? process_dataset.n_most : 

        // The expected offset that the last process should have
        int guess_for_recieved_size =  (*receiving_dataset_pointer).n_most * (number_of_processes - 1);
        // If it is equal to the expected set it to n_total - expected else the common size
        
        guess_for_recieved_size = ( guess_for_recieved_size != (*receiving_dataset_pointer).offset ) ? process_dataset.n_most : process_dataset.n_total - guess_for_recieved_size ; 

        (*receiving_dataset_pointer).n = guess_for_recieved_size ;

        //(*receiving_dataset_pointer).n ==== =( (*receiving_dataset_pointer).offset !=  (*receiving_dataset_pointer).n_most * (number_of_processes - 1)  ) ? process_dataset.n_most : process_dataset.n_total % process_dataset.n_most ;
        if (extensive_prints) debug(rank, "Recived data @@@@@@@\n");
        

        // change the pointers  // if it is the first iteration change the pointer to the woriking_datset 
        temp_dataset_pointer        = is_it_fist_iteration ? &working_dataset :  working_dataset_pointer  ;
        working_dataset_pointer     = receiving_dataset_pointer ;
        receiving_dataset_pointer   = temp_dataset_pointer ;
        is_it_fist_iteration = false ;
        
            

        if (extensive_prints) debug(rank, "Waiting for barrier in main loop\n");
        if (extensive_prints) MPI_Barrier(MPI_COMM_WORLD);
        if (extensive_sleeps) sleep(2);
        
        if( rank == 0 ){            
            printf("\n==================================== end of iteration %d ==================================== \n",i);
        }

        if (extensive_sleeps) sleep(1);
        if (extensive_prints) debug(rank,"End of loop iteration %d \n" , i );
        if (extensive_prints) MPI_Barrier(MPI_COMM_WORLD);
        if (extensive_sleeps) sleep(2);
        
        
        
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
    }

    // Calculate stuff 
    knn_of_my_points_vs_working = kNN_MPI(  &process_dataset , working_dataset_pointer ,  knn_to_culculate  ) ;    

    if(extensive_prints) print_knnresult_ids_with_offset( knn_of_my_points_vs_working , process_dataset.offset , process_dataset.n_total );

    // Reduce the result 
    if(is_it_fist_iteration){
        knn_of_my_points = knn_of_my_points_vs_working ;    // This would happen iff #processes == 1
    }
    else{
        knn_of_my_points = kNN_reduce_and_free(knn_of_my_points , knn_of_my_points_vs_working , knn_to_culculate);
    }


    if(extensive_prints) debug(rank,"Exited main loop !!!!!!!!!!!! about to free stuff \n");
    if(extensive_sleeps) sleep(5);

    check_correct_knn_MPI (  process_dataset , knn_of_my_points ) ;
   


    // Here we should collect the results to the main process;
    knnresult final_results_from_all ;
    final_results_from_all.k = knn_to_culculate ;
    final_results_from_all.m = process_dataset.n_total;

    int n_most = process_dataset.n_most ;
    

    // First we will clear out some memory !
    free_dataset_MPI_struct(process_dataset);   // this will not be used again, we have the knn_of_my_points
    free_dataset_MPI_struct(working_dataset);
    free_dataset_MPI_struct(receiving_dataset);

    if(extensive_sleeps) sleep(5);

    if( is_it_fist_iteration ){        
        // Culculate the sqrt of the distances!
        culculate_sqrt(&knn_of_my_points);
        final_results_from_all = knn_of_my_points ;
    }
    else{

        if ( rank != broadcaster_rank ){
            // here we will send our results to the main process
            MPI_Request request_send_knn_ids ;
            MPI_Status status_send_knn_ids ;        
            MPI_Isend( knn_of_my_points.nidx , knn_of_my_points.m * knn_of_my_points.k , MPI_INT    , broadcaster_rank , send_knn_ids   , MPI_COMM_WORLD , &request_send_knn_ids  ); 
            culculate_sqrt(&knn_of_my_points); // Culculate the sqrt of the distances!
            MPI_Send( knn_of_my_points.ndist , knn_of_my_points.m * knn_of_my_points.k , MPI_DOUBLE , broadcaster_rank , send_distances , MPI_COMM_WORLD );
            MPI_Wait( &request_send_knn_ids  , &status_send_knn_ids );
            //debug(rank, "Both distances and ids are send \n");
        }
        else{
            MPI_Request* requests_receive_dist_and_ids      = (MPI_Request*) malloc( sizeof(MPI_Request) * (number_of_processes - 1 ) * 2 ) ;
            MPI_Status*  status_receive_knn_ids             = (MPI_Status*)  malloc( sizeof(MPI_Status)  * (number_of_processes - 1 ) * 2 ) ;   

            if( requests_receive_dist_and_ids == NULL || status_receive_knn_ids == NULL ){
                debug(rank, "Could not allocate memmory for the MPI Requests and/or Status, malloc returned NULL pointer. \n");
                exit(0);
            }

            final_results_from_all.ndist = (double*) malloc( sizeof(double) * final_results_from_all.k * final_results_from_all.m ) ;
            final_results_from_all.nidx  = (int*)    malloc( sizeof(int)    * final_results_from_all.k * final_results_from_all.m ) ;

            if( final_results_from_all.ndist == NULL || final_results_from_all.nidx == NULL ){
                debug(rank, "Could not allocate memmory for the knnresult to store the final result, malloc returned NULL pointer. \n");
                exit(0);
            }

            // I need to copy my knnresult to the final result and receive form the othres 

            int dummy_var = 0 ; 
            for( int i = 0 ; i < number_of_processes ; i++ ){
                if ( i == broadcaster_rank ){
                    continue;
                }                
                // for the last process that has probably less points it just wont fill all the points, its ok
                MPI_Irecv( &( final_results_from_all.nidx [i*n_most*knn_to_culculate] ), n_most*knn_to_culculate , MPI_INT    , i, receive_knn_ids , MPI_COMM_WORLD , &(requests_receive_dist_and_ids[2*dummy_var]));
                MPI_Irecv( &( final_results_from_all.ndist[i*n_most*knn_to_culculate] ), n_most*knn_to_culculate , MPI_DOUBLE , i, receive_distances , MPI_COMM_WORLD , &(requests_receive_dist_and_ids[2*dummy_var + 1]));

                dummy_var++;
            }

            // Culculate the sqrt of the distances!
            culculate_sqrt(&knn_of_my_points); 
            // copy my results to the final resutls
            for( int j = 0 ; j < knn_of_my_points.m * knn_to_culculate ; j++ ){
                final_results_from_all.nidx [broadcaster_rank*n_most*knn_to_culculate + j ] = knn_of_my_points.nidx [j];
                final_results_from_all.ndist[broadcaster_rank*n_most*knn_to_culculate + j ] = knn_of_my_points.ndist[j];

            }

            for ( int i = 0 ; i < 2 * (number_of_processes - 1) ; i++ ){
                if(extensive_prints) debug(rank, "Waiting to receive %d of %d \n", i+1 , 2 * (number_of_processes - 1));
                MPI_Wait( &(requests_receive_dist_and_ids[i])  , &(status_receive_knn_ids[i]) );
            }

            free(requests_receive_dist_and_ids);
            free(status_receive_knn_ids);
            
        }
    }

    // Find time it took to fidn KNN the file   ~~~~~    
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_to_find_kNN = (double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_nsec - start.tv_nsec) / 1000000;



    // free the 2 malloc inside : knn_of_my_points    
    if (!is_it_fist_iteration){
        free_struct_result(&knn_of_my_points);
    }
    // the other should already have been freed

    if ( rank == broadcaster_rank ){
        //debug(rank , "Print the collected results for all points : \n\n");
        print_knnresult_ids(final_results_from_all);
        print_knnresult_distances(final_results_from_all);

        free_struct_result(&final_results_from_all);

        
        elapsed_total = time_to_load + time_to_find_kNN ;
        printf("Time to load dataset                : %f ms (this is only for the rank %d )\n", time_to_load , rank);
        printf("Time to find kNN                    : %f ms\n", time_to_find_kNN);
        printf("Total time (the sum of the above)   : %f ms\n", elapsed_total );

    }
    else{
        sleep(3);
    }
    free(file_to_load);
    
    if(extensive_prints) debug(rank,"Before calling finalize. \n");

    if(rank == 0 ) printf("\n");
	MPI_Finalize();
    printf("Succesful termination of process %d \n" , rank);

    return 0 ;
}
