
# Makefile
# By Liouliakis Nikolaos 


CC_MPI    	= mpicc 
CC 			= gcc
CFLAGS 		= -O3
CEXTRAFLAGS = -Wall 

# if all errors are to be suppressed
CEXTRAFLAGS += #-w


# if that specific error is to be suppressed.
CEXTRAFLAGS += 

MAIN_MPI = main_mpi.c
MAIN_SIMPLE = main_simple.c
FILES_TO_COMPILE = load_examples.c  find_knn.c  k_smallest.c helpers.c
EXE_NAME_MPI = knn_program_MPI_nick.exe 
EXE_NAME_SIMPLE = knn_program_nick.exe 

# number of processes
NP = 4
TIME = 3
PARTICION = testing

program: 
	$(CC_MPI) 	$(CFLAGS) $(CEXTRAFLAGS) $(MAIN_MPI) 	$(FILES_TO_COMPILE) -o $(EXE_NAME_MPI) 		-lm
	$(CC) 	 	$(CFLAGS) $(CEXTRAFLAGS) $(MAIN_SIMPLE) $(FILES_TO_COMPILE) -o $(EXE_NAME_SIMPLE) 	-lm
#


.PHONY: run runmpi clean test runssh

run:
	./$(EXE_NAME_SIMPLE)
runmpi:
	mpirun -np $(NP) ./$(EXE_NAME_MPI) 
#

runssh:
	srun -n $(NP) -t $(TIME) ./$(EXE_NAME_MPI) 
# -p $(PARTICION)


clean:
	 rm -f $(EXE_NAME_MPI)
	 rm -f $(EXE_NAME_SIMPLE)
#	del $(EXE_NAME)
test:	
	echo hello world


