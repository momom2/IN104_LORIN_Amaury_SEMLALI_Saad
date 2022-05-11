#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>    

int min(int a, int b){
    return (a>b) ? b : a;
}

int max(int a, int b){
    return (a>b) ? a : b;
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
