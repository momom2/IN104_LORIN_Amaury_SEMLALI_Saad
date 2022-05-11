#ifndef LEARNING_H
#define LEARNING_H

#include <stdio.h>
#include <stdlib.h>


double** Q; 				// Q-table Q[rows*cols][number_actions]
// ATTENTION : rows et cols participent de la même variable. Q pourrait être réécrite en Q[rows][cols][number_actions]
// On passe de [rows][cols] à [rows*cols] et vice-versa par les fonctions coord et case_coord de mazeEnv.c.

// No reward table, it is implicit in envOutput.

double learning_rate; 		// learning rate
double discount_rate;	 	// discount rate
double epsilon; 			// For epsilon-greedy learning, probability to explore.
double temperature;         // Parameter for Boltzmann exploration.
int max_epoch; 				// Total number of times the agent will play.
int current_epoch;	 		// Tracks the number of times the agent has played.
	
//extern int rows; 			// From mazeEnv.h
//extern int cols; 			// From mazeEnv.h

enum training_mode{
    epsilon_greedy,
    boltzmann_exploration,
    number_training_modes, 	// Modes after this are not implemented - yet.
};

// Allocate Q and initialize constants for training.
void initQ(); 				

// Choose a random action.
action rand_action_uniform();

// Choose an action using epsilon-greedy decision-making.
action rand_action_epsilon(double** Q,int coordonnee, double epsilon);

// Returns 1 iff the coordinate corresponds to the goal's location and done is 1.
int goal_reached(int coordonnee, int done);

// Prints the action recommended by Q on each case.
void printQ(double** Q);

// Update Q over one epoch of training, according to the chosen training mode.
void train_one_epoch(double epsilon, double temperature, int training_mode);

// RELEASE THE AGENT!
int main();


void quit();


#endif /* LEARNING_H */