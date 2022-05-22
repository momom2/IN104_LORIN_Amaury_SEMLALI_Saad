#ifndef EXTRAENV_H
#define EXTRAENV_H

#include <stdio.h>
#include <stdlib.h>
//undirected weighted graph
double** graph;
int** visited;
int size;
int start;
int state;
int goal;

enum terrain{
    unknown,
    known,
    goal,
};

//The nopes are numbered for a matter of simplicity
int number_actions=size;

for (int i=0,i<size+1,i++){
	typedef i action ;
	}
typedef numberactions action ;

struct envOutput{
  int new_state;
  double reward;
  int done;
  int has_moved;

};

typedef struct envOutput envOutput;

void alloc_graph();

void graph_make(double* );

void graph_render();

void graph_reset();

envOutput graph_step(action a); 

// Returns 1 in the coefficient whose row is i and column is j of the graph, corresponding to the action go from the node i to the node j.
char graphical_move(int action);

void test_envOutput();
int rand_state();

action env_action_sample();

void alloc_visited();

void init_visited();
void reset_visited();
void print_visited();
void crash_visited();

void quit_graph();

#endif /* EXTRAENV_H */
