
#include "k_smallest.h"

// Function to find the k smallest elements and their indexes
int k_smallest(double* values,double* smallest_values, int* indexes, int n, int k) {
    
    if( k > n ){
        k = n ; 
    }

    /*
    // print the array 
    printf("The array is \n");
    for (int i = 0; i < n; i++) {
        printf("%2.2f ", values[i]);
    }
    printf("\n");
    */
    
    
    int elements_found = 0 ;
    int same_elements = 0 ; 

    while( elements_found < k ){
        same_elements = 0 ; 
        indexes[elements_found] = 0 ; 
        /*
        printf("Elemets found so far %d \n" , elements_found);
        for( int i = 0 ; i < elements_found ; i++){        
            printf(" %.2f " , values[indexes[i]]) ;
        }    
        printf("\n");
        */

        if ( elements_found > 0 ){
            while( values[indexes[elements_found]] <= values[indexes[elements_found-1]] ){
                //printf("Activation of protection ========== ");
                //printf("Start value before %.2f " , values[indexes[elements_found]]) ;
                indexes[elements_found]++;
                
                //printf("after is  %.2f \n" , values[indexes[elements_found]]) ;
            }
            //printf("Exited protection \n");
        }
        

        for( int i = 0 ; i < n ; i++ ){ 
            //printf("\tInternal iteration %d \n" , i );
            if( values[i] < values[indexes[elements_found]] ){
                if ( elements_found > 0  ){
                    if(values[i]> values[indexes[ elements_found-1]] ){
                        // do nothing, i didn't want to invert the condition
                    }
                    else{
                        continue ;
                    }
                }
                //printf("\tThere was a change in the min value \n");
                indexes[elements_found] = i ; 
                if (same_elements > 0 ){
                    //printf("\tForget the duplicates \n");
                }
                same_elements = 0 ; 

            }
            else if ( values[i] == values[indexes[elements_found]] && i != indexes[elements_found]){
                same_elements++;
                //printf("\tFound duplicate number %d \n" , same_elements+1);
                if(elements_found + same_elements < k ){
                    indexes[elements_found+same_elements] = i ; 
                }
            }
        }
        elements_found += same_elements ; 
        elements_found++;
    }
    
    /*
    if ( elements_found > k ){
        printf("There are multipule points with the same distance so only the first found were selected.\n");
        printf("The points that do not fit in the k closest (but have the same distance as the the kth point) are %d.\n" , elements_found-k );
    }
    */

    // Print the k smallest elements and their indexes
    //printf("The %d smallest elements are:\n", k);
    
    for (int i = 0; i < k; i++) {
        smallest_values[i] =  values[indexes[i]] ; 
        //printf("%.15f (index %d)\n", values[indexes[i]], indexes[i]);
        //printf(" %3d ", indexes[i]);
    }
    

    return elements_found-k ;
}



// Function to find the k smallest elements and their indexes ingoring an element
int k_smallest_without_an_element(double* values, double* smallest_values, int* indexes, int n, int k , int exception_index ) {
    
    if( k >= n ){
        k = n-1 ; 
    }

    /*
    // print the array 
    printf("The array is \n");
    for (int i = 0; i < n; i++) {
        printf("%2.2f ", values[i]);
    }
    printf("\n");
    */
    
    
    int elements_found = 0 ;
    int same_elements = 0 ; 

    while( elements_found < k ){
        same_elements = 0 ; 
        indexes[elements_found] = 0 ; 
        /*
        printf("Elemets found so far %d \n" , elements_found);
        for( int i = 0 ; i < elements_found ; i++){        
            printf(" %.2f " , values[indexes[i]]) ;
        }    
        printf("\n");
        */
        if( indexes[elements_found] == exception_index ){
            indexes[elements_found]++ ;
        }

        if ( elements_found > 0 ){
            while( values[indexes[elements_found]] <= values[indexes[elements_found-1]] ){
                //printf("Activation of protection ========== ");
                //printf("Start value before %.2f " , values[indexes[elements_found]]) ;
                indexes[elements_found]++;
                if( indexes[elements_found] == exception_index ){
                    indexes[elements_found]++ ;
                }
                
                //printf("after is  %.2f \n" , values[indexes[elements_found]]) ;
            }
            //printf("Exited protection \n");
        }
        

        for( int i = 0 ; i < n ; i++ ){ 
            //printf("\tInternal iteration %d \n" , i );
            if( i == exception_index ){
                continue ;
            }
            if( values[i] < values[indexes[elements_found]] ){
                if ( elements_found > 0  ){
                    if(values[i]> values[indexes[ elements_found-1]] ){
                        // do nothing, i didn't want to invert the condition
                    }
                    else{
                        continue ;
                    }
                }
                //printf("\tThere was a change in the min value \n");
                indexes[elements_found] = i ; 
                if (same_elements > 0 ){
                    //printf("\tForget the duplicates \n");
                }
                same_elements = 0 ; 

            }
            else if ( values[i] == values[indexes[elements_found]] && i != indexes[elements_found]){
                same_elements++;
                //printf("\tFound duplicate number %d \n" , same_elements+1);
                if(elements_found + same_elements < k ){
                    indexes[elements_found+same_elements] = i ; 
                }
            }
        }
        elements_found += same_elements ; 
        elements_found++;
    }
    
    /*
    if ( elements_found > k ){
        printf("There are multipule points with the same distance so only the first found were selected.\n");
        printf("The points that do not fit in the k closest (but have the same distance as the the kth point) are %d.\n" , elements_found-k );
    }
    */

    // Print the k smallest elements and their indexes
    //printf("\nThe %d smallest elements are:\n", k);
    
    //printf("ID: %3d |" , exception_index );  
    for (int i = 0; i < k; i++) {
        smallest_values[i] =  values[indexes[i]] ; 
        //printf("%.15f (index %d)\n", values[indexes[i]], indexes[i]);
        //printf(" %3d ", indexes[i]);
    }

    return elements_found-k ;
}
