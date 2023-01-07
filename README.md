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

This will run using as default 4 processes, to change the number of processes use:
```
make runmpi NP=5
```

or where you can change 4 to the number of processes you want:
```
mpirun -np 4 ./knn_program_MPI_nick.exe 
```



# Load files

run runmpi clean test runssh


.......
