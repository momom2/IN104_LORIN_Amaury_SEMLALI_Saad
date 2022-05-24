# IN104_LORIN_Amaury_SEMLALI_Saad
Reinforcement learning implemented to escape a maze and/or find the shortest path on a graph
by Sâad IDRISSI SEMLALI and Amaury LORIN
for educational and entertainment purposes.

To use this you need:
-A C compiler.
-(optional) A maze or graph provided in the right format.


How to use this (short version):
-in a terminal, in the CMaze-main directory, use the command
	make all
-then try
	./dfs.out
	./maze_learning.out
	./graph_learning.out



How to train an agent and use it to escape a maze or find the shortest path on a graph (long version):

-Choose whether you want to escape a maze or find the shortest path on a graph.
-In CMaze-main/source/, open the file maze_learning.c or graph_learning, as applicable.
-In the main function (around lines 360~390), check that:
	-test_mode = 0
	-debug_mode = 0 (You can set it higher for more in depth details, but it's not very visible unless you know what you're looking for)
-At the beginning of the INITIALIZATION (around lines 370~400), input your filename as the argument of maze_make or graph_make, as applicable, and choose accordingly your training_mode and learning_type:

The choices are, for training_mode:
	-epsilon_greedy
	-boltzmann_exploration
		 for learning_type:
   	 -single_learning
 	 -double_learning
 	 -sarsa_learning
If you don't know the difference,by default choose :
	training_mode = epsilon_greedy  
	learning_type = single_learning

-In a terminal, cd to the directory CMaze-main.
-Use 
	make maze_learning
	or
	make graph_learning
-Then use
	./maze_learning.out
	or
	./graph_learning.out


Enjoy!






How to write your own maze or graph in the right format:


-For a maze:
	-The file should have a .txt as a file name extension.
	-On the first line, give the number of rows r and columns c as follows: "r,c" where r and c are integers. The coma is important.
	-Then, you should put "+" for a wall, "s" for the starting position of the agent, "g" for the exit or the goal of the agent and " " for walkable terrain.
		-There should be exactly one starting position and one goal.
		-The maze should be fully enclosed in walls (not stricly necessary, but proper behavior is not guaranteed if this is not respected).
		-There should be a walkable path from the starting position to the goal (idem).
	-Lastly, end your file with a carriage return.
As examples, see maze.txt and bigmaze.txt

-For a graph (weighted, bidirectional):
	-The file should be a .txt
	-On the first line, give the number of nodes N, the starting node s and the exit (or goal) node g as follows: "N,s,g" where N is an integer and s and g are integers ranging between 0 and N-1.
	-Then, for each edge of your graph between nodes i and j of weight w: add a line "i j w" where i and j are integers between 0 and N-1, w is a double much smaller than 1000.
		-If you give more edges than N², the subsequent edges will be ignored.
		-You can give your edges negative weight, but proper behavior is not guaranteed then.
		-If an edge is listed more than once, only the last weight will be taken into account.
	-Lastly, end your file with a carriage return.
As examples, see graph1.txt and graph2.txt



In-depth tempering:
Don't touch those unless you know what you're doing.

-By default, impassable edges are considered arbitrarily to be edges with the weight 1000. Their weight can be increased (but not made infinite) by modifying POIDS_INFINI (infinite weight) at the beginning of graphEnv.c in CMaze-main/source/.

-By default, agents stop training after 100 consecutive games where they reach the exit. This value can be increased by increasing max_super_winrate in maze_learning.c or graph_learning.c (lines around 410~430).
-By default, agents who never manage to win enough consecutive games will stop training after 10000 games. This value can be changed by changed max_epoch in maze_learning.c or graph_learning.c (lines around 400~420).
-By default, agents who don't manage to find the goal during a game will stop and lose the game after 500 moves. This value can be changed by changing max_time in maze_learning.c or graph_learning.c (lines around 400~420).
-By default, agents will train as fast as possible in a given environment. You can tell them to take a nap between each game by increasing sleeping_time in maze_learning.c or graph_learning.c (lines around 380~400).






As a bonus, you can solve the maze using a depth-first-search algorithm with 
	make dfs
	./dfs.out
(Credits: Ms.Sao Mai NGUYEN)

For this project, we used the msleep function, provided on StackOverflow, last edited by "Neuron - Freedom for Ukraine" at the time of this writing.
https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds/1157217#1157217
