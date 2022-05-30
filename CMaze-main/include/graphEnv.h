#ifndef GRAPHENV_H
#define GRAPHENV_H

#include <stdio.h>
#include <stdlib.h>

//undirected weighted graph
double** graph;
int* visited;
int size; // The graph has size nodes, each is a state.

int start; // Between 0 and size-1: a certain node.
int state; // Between 0 and size-1: a certain node.
int goal; // Between 0 and size-1: a certain node.

double max_reward;
double poids_infini=100; // Poids d'une arête que l'on veut impossible à traverser.

enum terrain{
    unknown,
    known,
    very_known,
    objective,
    crumb
};

int number_actions; // number_actions = size
// The possible actions are ints from 0 to number_actions-1: going to the selected node.
typedef int action;


struct envOutput{
  int new_state;
  double reward;
  int done;
  int has_moved;

};

typedef struct envOutput envOutput;

void alloc_graph();

void graph_make(char* file_name);

void graph_render();

void graph_reset();

envOutput graph_step(action a); 


void test_graph_step();
int rand_state();
int rand_coord();

action env_action_sample();

void alloc_visited();

void init_visited();
void reset_visited();
void print_visited();
void crash_visited();

void quit_graph();

#endif /* GRAPHENV_H */
