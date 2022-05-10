#ifndef MAZEENV_H
#define MAZEENV_H

#include <stdio.h>
#include <stdlib.h>

char** maze;
int** visited;
int rows;
int cols;
int start_row;
int start_col;
int state_row;
int state_col;
int goal_row;
int goal_col;

enum terrain{
    unknown,
    wall,
    known,
    goal,
    crumb
};

enum action{
     up,
     down,
     left,
     right,
     number_actions 
};

typedef enum action action ;

struct envOutput{
  int new_col;
  int new_row;
  double reward;
  int done;
  int has_moved;

};

typedef struct envOutput envOutput;

void alloc_maze();

void maze_make(char* );

void maze_render();

void maze_reset();

envOutput maze_step(action a); 

// Returns an arrow corresponding to the action - up, down, left or right.
char graphical_move(int action);

void test_envOutput();
int rand_col();
int rand_row();

int rand_coord();
void coord(int coordonnee, int* r, int* c);
int case_coord(int coord_r, int coord_c);
void test_coord_converter();

action env_action_sample();

void alloc_visited();

void init_visited();
void reset_visited();
void print_visited();
void crash_visited();

void quit_maze();

#endif /* MAZEENV_H */
