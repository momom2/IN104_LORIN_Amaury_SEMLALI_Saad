#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <math.h>

extern int min(int a, int b);

extern int max(int a, int b);

extern void test_rand();

extern double listmax(double* list, int length);
extern int argmax(double* list, int length);
extern int argmax2(double* list1,double* list2, int length);

extern int msleep(long msec);

#endif
