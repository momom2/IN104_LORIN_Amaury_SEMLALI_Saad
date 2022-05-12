CC = gcc
CFLAGS = -Wall -Werror
# Pas de CFLAGS, déjà en alias dans bashrc : -Wall -Werror ??? A vérifier : bashrc bizarre...
INC = -Iinclude/
SRC = source/
#$(info EXEC before = $(EXEC))
EXEC = dfs_target
#$(info EXEC after = $(EXEC))
#$(info DIRECTORY before = $(DIRECTORY))
DIRECTORY = ~/ # Make sure you're in the right directory!
#$(info DIRECTORY after = $(DIRECTORY))


all : dfs learning


clean :
	rm -rf $(SRC)%.o
	rm -rf $(SRC)%.out

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -o $@ -c $< $(CFLAGS)

#$(SRC)%.o : $(SRC)%.c
#	$(CC) $(INC) -o $@ -c $< $(CFLAGS)

dfs : $(SRC)dfs.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

learning : $(SRC)learning.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

double_learning : $(SRC)double_q-learning.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

sarsa : $(SRC)sarsa.o $(SRC)functions.o $(SRC)mazeEnv.o
	
	$(CC) $(INC) -o $@.out $^ $(CFLAGS)

zip:
	tar -zcvf prog.tgz $(DIRECTORY) 


#Ce que je veux :
#gcc -c trucs.c
#...
#gcc -o a.out trucs.o
