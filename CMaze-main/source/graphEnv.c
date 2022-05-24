#include "graphEnv.h" 
#include "functions.h"

#define POIDS_INFINI 1000


void alloc_graph(){
     graph = malloc(size * sizeof(double*));

     for(int i=0; i<size; i++) {
         graph[i] = malloc(size * sizeof(char*));
     }
}

void graph_make(char* file_name){
     printf("Creating graph...\n");
     double poids_infini = POIDS_INFINI;
     char c;
     double poids_total;
     int swap = 0;
     char size_s[9] ={'\0'};
     int size_i = 0;
     //printf("Opening file %s.\n", file_name);
     FILE* file = fopen(file_name, "r");
     if (file == NULL) {printf("Error in graph_make: fopen failed.\n");}
     //printf("File opened.\n");
     if (file) {
         /* lire la premiere ligne avant EOF */
         while( (c=getc(file)) != EOF) {

               if(c== '\n'){
               		  break;
               } else if(c == ',' && swap == 0){
  					  swap = 1;             
  			   } else if(c == ',' && swap == 1){
  					  swap = 2;		  
               } else if(swap == 0){
                      size_s[size_i]=c;
                      size_i++;
               } else if(swap == 1){
               		  start = c - '0';
               } else {
               		  goal = c - '0';
               }
     	 }
     }
     //printf("File read.\n");
     /* convertir le string en nombre */
     size = atoi(size_s);
     number_actions = size;

     //printf("Allocating graph.\n");
     alloc_graph();
     //printf("Graph allocated.\n");
     double poids; // Le poids de l'arête de i à j.
     for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            graph[i][j] = poids_infini;
        }
     }
     int i, j;
     for(int k=0;k<(size*size);++k){
        int scan_result = fscanf(file, "%d %d %lf\n", &i, &j, &poids);
        if(scan_result == 3){
            graph[i][j] = poids;
            graph[j][i] = poids;
            poids_total +=abs(poids);
        } else {
            break;
        }
     }
     
     max_reward = -poids_total;
     //printf("Closing file.\n");
     fclose(file);
     //printf("File closed.\n");
}

void graph_render(){
     for (int i=0; i<size; i++) {
     	 printf("[");
         for (int j=0; j<size; j++){
             printf("%.2lf,", graph[i][j]);
         }
         printf("]\n");
     }
     printf("\n");
}

void graph_reset(){
     state = start;
}

////  THIS IS WHERE THE REWARD FUNCTION LIES (IMPLICITLY) ////

envOutput graph_step(action a){
    double reward = -0.04;
    int done = 0;
    int moved_successfully = 0;
    envOutput stepOut;

    if(visited[a] == known){
        reward -= 0.1; // On préfère légèrement ne pas passer plusieurs fois au même endroit.
    }
    if(graph[state][a]>999){
        // On ne bouge pas.
        reward -= graph[state][a];
    } else {
        reward -= graph[state][a];
        state = a;
        moved_successfully = 1;
    }
    
    
    if(state == goal){
        if(visited[state] == objective){
            reward = max_reward;
            done   = 1;
        } else {
            printf("Warning in maze_step: objective not the goal?/n");
        }
    }

    //printf("Inside. Reward: %f, done: %d, col: %d, row: %d.\n", reward, done, state_col, state_row);
    stepOut.reward = reward;
    stepOut.done   = done;
    stepOut.new_state = state;
    stepOut.has_moved = moved_successfully;

   return stepOut;
}

void test_graph_step(int random){
    printf("Test de graph_step.\n");
    action a;
    int temp_state;
    if(!random){ // Tester une action particulière dans un état choisi.
        action a = 0;
        state = size-1;
        int temp_state = state;
        printf("Placed in (%d). Takes action ->%d.\n",temp_state,a);
        envOutput out = graph_step(a);
        printf("Reward: %f\nState: now in (%d).\nHas moved? %d\nDone? %d\n",  \
            out.reward,out.new_state,out.has_moved,out.done);
    } else { // Test randomisé.
        for(int i=0;i<100;++i){
            printf("\ni: %d\n",i);
            
            a = env_action_sample();
            state = rand_state();
            temp_state = state;
            printf("Placed in (%d). Takes action ->%d.\n",temp_state,a);
            envOutput out = graph_step(a);
            printf("Reward: %f\nState: now in (%d).\nHas moved? %d\nDone? %d\n",  \
                out.reward,out.new_state,out.has_moved,out.done);
        }
    }
    printf("Fin du test.\n");
}

int rand_state(){
    return (rand() % size);
}
int rand_coord(){
    return (rand() % size);
}

action env_action_sample(){
    return (action)(rand() % number_actions);
}

void alloc_visited()
{
    visited = malloc(size * sizeof(int));
}

void init_visited(){
    alloc_visited();
    
    int i;
    for (i = 0; i < size; ++i) {
        visited[i] = unknown;
    }
    visited[goal] = objective;
}
void reset_visited(){
    int i;
    for (i = 0; i < size; ++i) {
        visited[i] = unknown;
    }
    visited[goal] = objective;
}

void print_visited(){
    for(int i=0; i<size; ++i){
        printf("%d ",visited[i]);
    }
    printf("\n");
}
void crash_visited(){
    int i = 0;
    while(1){
        printf("i: %d, visited[i]: %d",i,visited[i]);
        ++i;
    }
}

void quit_graph(){
    printf("Press any key to exit.\n");
    getchar();
    for(int i=0; i<size; i++) {
        free(graph[i]);
    }
    free(graph);

    free(visited);
    printf("Graph destroyed.\n");
}