#include "mazeEnv.h"
#include "../include/functions.h"
#include "learning.h"
#include <time.h>



/*
void add_crumbs(){
     for (int i=0; i<rows; i++){
          for (int j=0; j<cols; j++){
              if (visited[i][j] ==crumb){
                  maze[i][j] ='.';
              }
          }
     }
     maze[start_row][start_col]= 's';
}
*/

// INITIALIZATION
// Does not belong here, move to main when finished.
void init(){
printf("Initializing.\n");
max_epoch = 100;
current_epoch = 0;
learning_rate = 0.1;
discount_rate = 0.9;
epsilon = 0.95;

// TODO: malloc pour Q 
for(int k = 0; k<number_actions; ++k){
    for(int j = 0; j<cols; ++j) {
        for(int i = 0; i<rows; ++i){
            Q[i][j][k]=0;
        }
    }
}

printf("Initialization complete.\n");
}
/*
while(current_epoch<max_epoch){
    // Initialize starting position.
    maze_reset();    
    while(state_col != goal_col || state_row != goal_row){
    
    
    


     
        ...
    
    }
}
*/






int main(){
    init();
    maze_make("maze.txt");
    init_visited();
 
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    //maze_render();
   
    // THIS IS A STUB //
    printf("Pas d'entraînement.\n");
    // TRAIN THE MODEL//

    // TEST IT.
    srand(time(NULL));
    //test_envOutput();
    //test_rand();
    test_coord_converter();

    ////////////////////
    //maze_render();
    printf("Résultat de recherche par apprentissage.\n Appuyer sur une touche pour continuer.\n");
    getchar();
 
    return 0;
} 