#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

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