#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern int min(int a, int b);

extern int max(int a, int b);
extern double maxf(double a, double b);

extern double randf(double maximum);
extern void test_randf();
extern void test_rand();

extern double listmax(double* list, int length);
extern int argmax(double* list, int length, int* preference);
extern int argmax2(double* list1,double* list2, int length);

extern int msleep(long msec);


/* ///// OBSOLETE CODE, DOES NOT WORK. TODO: REIMPLEMENT SOMME CLEANLY, BASED ON THIS.
   ///// cf https://stackoverflow.com/questions/72189642/how-to-cast-function-into-a-struct-in-c/72192354#72192354

struct double_fun{
	double (*function)(double x);
};
// Test functions.
extern struct double_fun square;
extern struct double_fun identity;
extern struct double_fun inverse;
extern struct double_fun twice;

// Exponential function.
extern double exponential_temp(double x,double temperature);
extern struct double_fun exponential(double temperature);

*/
double identity(double x, double temperature);
double square(double x, double temperature);
double inverse(double x, double temperature);
double twice(double x, double temperature);
double exponential(double x, double temperature);


double somme(double* liste, int length, double (*function)(double, double) , double temperature);
extern void test_somme();

int get_latest_results(int* result_table,int result_table_length);

void print_list(double* list, int length);

#endif