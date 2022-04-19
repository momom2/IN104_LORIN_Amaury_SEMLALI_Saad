#ifndef LEARNING_H
#define LEARNING_H

#include <stdio.h>
#include <stdlib.h>


double** Q; // Q-table Q[rows*cols][number_actions]
// ATTENTION : rows et cols participent de la même variable. Q pourrait être réécrite en Q[rows][cols][number_actions]
// On passe de [rows][cols] à [rows*cols] et vice-versa par les fonctions coord et case_coord de mazeEnv.c.
// no reward table, it is implicit in envOutput

double learning_rate; // learning rate
double discount_rate; // discount rate
double epsilon; // for eps-greedy learning, probability to explore
int max_epoch;
int current_epoch;








#endif /* LEARNING_H */