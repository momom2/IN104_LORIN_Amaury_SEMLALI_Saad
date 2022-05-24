CC = gcc
CFLAGS = -Wall -Werror -lm
INC = -Iinclude/
SRC = source/
#$(info EXEC before = $(EXEC))
EXEC = dfs_target
#$(info EXEC after = $(EXEC))
#$(info DIRECTORY before = $(DIRECTORY))
DIRECTORY = ~/ # Make sure you're in the right directory!
#$(info DIRECTORY after = $(DIRECTORY))


all : dfs maze_learning graph_learning


clean :
	rm -rf $(SRC)%.o
	rm -rf $(SRC)%.out

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -o $@ -c $< $(CFLAGS)

#$(SRC)%.o : $(SRC)%.c
#	$(CC) $(INC) -o $@ -c $< $(CFLAGS)

dfs : $(SRC)dfs.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

maze_learning : $(SRC)maze_learning.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

graph_learning : $(SRC)graph_learning.o $(SRC)functions.o $(SRC)graphEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)


sarsa : $(SRC)sarsa.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

zip:
	tar -zcvf prog.tgz $(DIRECTORY) 


#Ce que je veux :
#gcc -c trucs.c
#...
#gcc -o a.out trucs.o
