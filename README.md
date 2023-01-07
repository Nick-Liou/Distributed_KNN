# Find_knn
This is a project for the class Parallel-and-Distributed-Systems that finds the knn.
There are two aproaches a simple Sequential and a more complex Asynchronous parallel using OpenMPI

# Compile 

After downloading the files go to the folder "=====" and use: ```make```.
This will compile both the Sequential and the Asynchronous parallel versions and will produce two executables.

If you do not have "make" isntalled you can compile the programms with:

```
gcc     -O3 -Wall  main_simple.c load_examples.c  find_knn.c  k_smallest.c helpers.c -o knn_program_nick.exe  	 -lm
mpicc   -O3 -Wall  main_mpi.c 	 load_examples.c  find_knn.c  k_smallest.c helpers.c -o knn_program_MPI_nick.exe -lm
```

# Run the Sequential

To run the Sequential use : 
```
make run
```

or:
```
./knn_program_nick.exe 
```

# Run the Asynchronous parallel (```OpenMPI```)

To run the Asynchronous parallel use : 
```
make runmpi
```

The above will run using as default 4 processes, to change the number of processes use:
```
make runmpi NP=5
```

or you can run it using:
```
mpirun -np 4 ./knn_program_MPI_nick.exe 
```
You can change 4 to the number of processes you want.


# Load files

The programm needs a dataset to load the points. Those datasets should have a very specific format for the programm to be able to use them.
There are many examples of how those datasets should be formated in the folder "===========". You can also specify the relative path to a dataset file.


To sum up it should be a txt file where in the first row there should be 3 integers seperated by a comma ",".
The first is the number of points in the dataset, the second is the number of dimentions in the dataset and the third is the number of precomputed knn (which can be zero).
An example for the first row is : "10,2,9" This means n = 10 , d = 2 and k = 9 

Then there should be n rows (one for each point). Each row should have d + k numbers.
The first d numbers of each row are the coordinates of the point (they can be floating point numbers with ".") and should be seperated by a comma "," the last k numbers are the zero-based ID of the knn for that point.
An example for the second row is : "0.489764395788231,0.498364051982143,7,2,3,6,5,4,8,1,9"  
Since this is the point with ID zero , in the knn there is no point with ID zero.

The examples above are from the dataset 0 with name: "knn_dataset0.txt". 
An example of a dataset where k = 0 is the dataset 24 with name: "knn_dataset24.txt".

Note:
The number of precomputed knn is used to check the knn the program finds and should be at most n-1. If the program is asked to calculate more knn that the number of precomputed then it will only check the ones that were precomputed.




