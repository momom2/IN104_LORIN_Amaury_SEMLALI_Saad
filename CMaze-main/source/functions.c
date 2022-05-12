#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>    
#include <math.h>

int min(int a, int b){
    return (a>b) ? b : a;
}

int max(int a, int b){
    return (a>b) ? a : b;
}
double maxf(double a, double b){
    return (a>b) ? a : b;
}

// randf(): Generate random double between 0 and maximum.
double randf(double maximum){
    return ((double)rand()/(double)(RAND_MAX)) * maximum;
}

void test_randf(){
    printf("Testing randf with 25000 as maximum.\n");
    for (int i=0;i<10;++i){
        printf("Random number : %f\n", randf(25000));
    }
    printf("Testing randf with 0.0001 as maximum.\n");
    for (int i=0;i<10;++i){
        printf("Random number : %f\n", randf(0.0001));
    }
    printf("Test finished.\n");
}
void test_rand(){
    printf("Testing rand.\n");
    for (int i=0;i<10;++i){
        printf("Random number : %d\n", rand());
    }
    printf("Test finished.\n");
}

// listmax(): Return the maximum of the list.
double listmax(double* list, int length){
    if (length<=0){
        printf("Error in list length. Listmax failed.\n");
        return -1;
    }
    double temp_max = list[0]; 
    for(int i=1;i<length;++i){
        if (list[i]>temp_max){
            temp_max = list[i];
        }
    }
    return temp_max;
}

// argmax(): Return the index of the first maximal element of the list.
int argmax(double* list, int length){
    if (length<=0){
        printf("Error in list length. Argmax failed.\n");
        return -1;
    }
    double temp_argmax = 0;
    double temp_max = list[0]; 
    for(int i=1;i<length;++i){
        if (list[i]>temp_max){
            temp_argmax = i;
            temp_max = list[i];
        }
    }
    return temp_argmax;
}
int argmax2(double* list1,double* list2,int length){
    if (length<=0){
        printf("Error in list length. Argmax failed.\n");
        return -1;
    }
    double temp_argmax = 0;
    double temp_max = list1[0]+list2[0]; 
    for(int i=1;i<length;++i){
        if (list1[i]+list2[i]>temp_max){
            temp_argmax = i;
            temp_max = list1[i]+list2[i];
        }
    }
    return temp_argmax;
}
/* msleep(): Sleep for the requested number of milliseconds. */
// Copy-pasted from https://www.codegrepper.com/code-examples/c/c+sleep+milliseconds //
int msleep(long msec){
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


/* ///// OBSOLETE CODE, DOES NOT WORK. TODO: REIMPLEMENT SOMME CLEANLY, BASED ON THIS.
   ///// cf https://stackoverflow.com/questions/72189642/how-to-cast-function-into-a-struct-in-c/72192354#72192354

double square_fun(double x){
    return x*x;
}
struct double_fun square = square_fun;
double identity_fun(double x){
    return x;
}
struct double_fun identity = identity_fun;
double inverse_fun(double x){
    return 1/x;
}
struct double_fun inverse = inverse_fun;
double twice_fun(double x){
    return 2*x;
}
struct double_fun twice = twice_fun;

// Exponential function.
double exponential_temp(double x, double temperature){
    return exp(x/temperature); // From math.h
}

struct double_fun exponential(double temperature){
    double aux_fun(double x){return exponential_temp(x, temperature);};
    struct double_fun aux = aux_fun;
    return aux;
}
*/

// Test functions for somme.
double identity(double x, double temperature){
    return x;
}
double square(double x, double temperature){
    return x*x;
}
double inverse(double x, double temperature){
    return 1/x;
}
double twice(double x, double temperature){
    return 2*x;
}
double exponential(double x, double temperature){
    return exp(x/temperature);
}

double somme(double* liste, int length, double (*function)(double, double), double temperature){
    double poids = 0;
    for(int i=0;i<length;++i){
        poids = poids + function(liste[i],temperature);
    }
    return poids;
}
    
void test_somme(int random){
    if(!random){
        double liste[5] = {1,2,5,3,4};
        double temperature = 2;
        printf("liste = {1,2,5,3,4}, temperature = 2\n");
        printf("somme(liste,5,identity,temperature) = %f\n",somme(liste,5,identity, temperature));
        printf("somme(liste,5,square,temperature) = %f\n",somme(liste,5,square,temperature));
        printf("somme(liste,5,inverse,temperature) = %f\n",somme(liste,5,inverse,temperature));
        double liste_vide[0] = {};
        printf("somme({},0,square,temperature) = %f\n",somme(liste_vide,0,square,temperature));
        printf("somme(liste,5,exponential,temperature) = %f\n",somme(liste,5,exponential,temperature));
    } else {
        // TODO: generate random lists and test them with random modifiers, printing everything on the way...
        
    }
}

void print_latest_results(result_table, result_table_length){
    int sum = 0;
    for(int i=0;i<result_table_length;++i){
        result = result_table[i];
        if(result != 1 && result != 0){
            printf("Error: invalid result_table in print_latest_results.\n");
        } else {
            sum += result;
        }
    double winrate = (double) sum/result_table_length;
    printf("Win rate: %2f\n",winrate);
    }
}