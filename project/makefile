FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm
CC = gcc
EXEC = cluster
FILES = cluster.o spmatArray.o divideIntoTwo.o matrixStructure.o matrixShifting.o modularityGroups.o group.o divideIntoGroups.o powerIteration.o modularityMaximization.o


all: $(FILES)
	$(CC) $(FILES) -o $(EXEC) $(LIBS)

clean:
	rm -f $(FILES)

$(EXEC): $(FILES)
	$(CC) $(FILES) -o $@ $(LIBS)

cluster.o: cluster.c divideIntoGroups.h matrixShifting.h
	$(CC) $(FLAGS) -c $*.c

spmatArray.o: spmatArray.c spmatArray.h error_handler.h
	$(CC) $(FLAGS) -c $*.c

divideIntoTwo.o: divideIntoTwo.c divideIntoTwo.h modularityMaximization.h powerIteration.h
	$(CC) $(FLAGS) -c $*.c

matrixStructure.o: matrixStructure.c matrixStructure.h spmatArray.h 
	$(CC) $(FLAGS) -c $*.c

matrixShifting.o: matrixShifting.c matrixShifting.h matrixStructure.h
	$(CC) $(FLAGS) -c $*.c

modularityGroups.o: modularityGroups.c modularityGroups.h group.h error_handler.h
	$(CC) $(FLAGS) -c $*.c

group.o: group.h group.c
	$(CC) $(FLAGS) -c $*.c

divideIntoGroups.o: divideIntoGroups.c divideIntoGroups.h divideIntoTwo.h
	$(CC) $(FLAGS) -c $*.c

powerIteration.o: powerIteration.c powerIteration.h matrixStructure.h modularityGroups.h
	$(CC) $(FLAGS) -c $*.c

modularityMaximization.o: modularityMaximization.c modularityMaximization.h matrixStructure.h modularityGroups.h
	$(CC) $(FLAGS) -c $*.c

