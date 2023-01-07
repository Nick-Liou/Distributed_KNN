# Find_knn
This is a project for the class Parallel-and-Distributed-Systems that finds the knn.
There are two aproaches a simple Sequential and a more complex Asynchronous parallel using OpenMPI

# Compile 

After downloading the files go to the folder "=====" and use: ```make```.
This will compile both the Sequential and the Asynchronous parallel versions and will produce two executables.

# Run the Sequential

To run the Sequential use : 
```
make run
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




run runmpi clean test runssh

# Load files

.......
