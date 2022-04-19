#include "mazeEnv.h" 
#include "functions.h"

void alloc_maze(){
     maze = malloc(rows * sizeof(char*));

     for(int i=0; i<rows; i++) {
         maze[i] = malloc(cols * sizeof(char*));
     }
}

void maze_make(char* file_name){
     //printf("Begin maze_make.\n");
     char c;
     char rows_s[3] ={'\0'};
     char cols_s[3] ={'\0'};
     int rows_i = 0;
     int cols_i = 0;
     int swap = 0;
     //printf("Opening file %s.\n", file_name);
     FILE* file = fopen(file_name, "r");
     if (file == NULL) {printf("Error: fopen failed.\n");}
     //printf("File opened.\n");
     if (file) {
         /* lire la premiere ligne avant EOF */
         while( (c=getc(file)) != EOF) {
               if(c== '\n'){
                      break;
               } else if (c==',') {
                      swap = 1;
               } else if (!swap) {
                      rows_s[rows_i]=c;
                      rows_i++;
               } else {
                      cols_s[cols_i]= c;
                      cols_i++;
               }
         }
     }
     //printf("File read.\n");

     /* convertir le string en nombre */
     rows = atoi(rows_s);
     cols = atoi(cols_s);
     //printf("Allocating maze.\n");
     alloc_maze();
     //printf("Maze allocated.\n");
     for (int i=0; i<rows; i++){
         for (int j=0; j < cols; j++){
             c = getc(file);

             if (c=='\n'){
                 c = getc(file);
             } else if (c == 's'){
               start_row = i;
               start_col = j;
             } else if ( c == 'g'){
               goal_row = i;
               goal_col = j;
             }

             maze[i][j] = c;
         }
     }
     //printf("Closing file.\n");
     fclose(file);
     //printf("File closed.\n");
}


void maze_render(){
     for (int i=0; i<rows; i++) {
         for (int j=0; j< cols; j++){
             printf("%c ", maze[i][j]);
         }
         printf("\n");
     }
     printf("\n");
}


void maze_reset(){
     state_row = start_row;
     state_col = start_col;
}



envOutput maze_step(action a){
    double reward = -0.04;
    int done = 0;
    envOutput stepOut;

    if (a==up){
        if(state_row <= 0) { // On se cogne contre le bord.
            //printf("Ouch.\n");
            reward -= 0.5;
            state_row = 0;
        } else if(visited[state_row-1][state_col] == wall) { // On se cogne contre un mur.
            //printf("Ouch.\n");
            reward -= 0.5;
        } else if (visited[state_row-1][state_col] == known){ // On revient sur nos pas.
            reward -= 0.2;
            state_row -=1;
        } else { // On avance.
            state_row -=1;
        }
    }else if (a==down){
        if(state_row >= rows) { // On se cogne contre le bord.
            //printf("Ouch.\n");
            reward -= 0.5;
            state_row = rows;
        } else if(visited[state_row+1][state_col] == wall) { // On se cogne contre un mur.
            //printf("Ouch.\n");
            reward -= 0.5;
        } else if (visited[state_row+1][state_col] == known){ // On revient sur nos pas.
            reward -= 0.2;
            state_row +=1;
        } else { // On avance.
            state_row +=1;
        }
    }else if (a==right){
        if(state_col >= cols) { // On se cogne contre le bord.
            //printf("Ouch.\n");
            reward -= 0.5;
            state_col = cols;
        } else if(visited[state_row][state_col+1] == wall) { // On se cogne contre un mur.
            //printf("Ouch.\n");
            reward -= 0.5;
        } else if (visited[state_row][state_col+1] == known){ // On revient sur nos pas.
            reward -= 0.2;
            state_col +=1;
        } else {
            state_col +=1;
        }
    }else if (a==left){
        if(state_col <= 0) { // On se cogne contre le bord.
            //printf("Ouch.\n");
            reward -= 0.5;
            state_col = 0;
        } else if(visited[state_row][state_col-1] == wall) { // On se cogne contre un mur.
            //printf("Ouch.\n");
            reward -= 0.5;
        } else if (visited[state_row][state_col-1] == known){ // On revient sur nos pas.
            reward -= 0.2;
            state_col -=1;
        } else { // On avance.
            state_row -=1;
        }
    }
    
    if((state_row == goal_row) && (state_col == goal_col)){
       reward = 1;
       done   = 1;
    }

    //printf("Inside. Reward: %f, done: %d, col: %d, row: %d.\n", reward, done, state_col, state_row);
    stepOut.reward = reward;
    stepOut.done   = done;
    stepOut.new_col = state_col;
    stepOut.new_row = state_row; 

   return stepOut;
}

void test_envOutput(){
    printf("Test de envOutput.\n");
    for(int i=0;i<10;++i){
        action a = env_action_sample();
        state_col = rand_col();
        int temp_state_col = state_col;
        state_row = rand_row();
        int temp_state_row = state_row;
        envOutput out = maze_step(a);
        printf("Reward: %f\nState: moved from (%d,%d) to (%d,%d) with action %d.\nDone? %d\n",  \
            out.reward,temp_state_row,temp_state_col,state_row,state_col,a,out.done);
    }
    printf("Fin du test.\n");
}
//test_envOuptut();

int rand_col(){
    return (rand() % cols);
}
int rand_row(){
    return (rand() % rows);
}
int rand_coord(){
    return (rand() % (rows*cols));
}

void coord(int coordonnee, int* r, int* c){
    *r = coordonnee / cols;
    *c = coordonnee % cols;
}
    
int case_coord(int coord_r, int coord_c){
    return cols*coord_r + coord_c;
}

void test_coord_converter(){
    printf("Test de case_coord.\n");
    for(int i=0;i<10;++i){
        state_col = rand_col();
        state_row = rand_row();
        int coordonnee = case_coord(state_row, state_col);
        printf("Coordonnee de (%d,%d): %d.\n",  \
            state_row,state_col,coordonnee);
    }
    printf("Test de coord.\n");

    for(int i=0;i<10;++i){
        int coordonnee = rand_coord();
        int* pcol = &state_col;
        int* prow = &state_row;
        coord(coordonnee, prow, pcol);
        printf("Coordonnee de (%d,%d): %d.\n",  \
            state_row,state_col,coordonnee);
    }
    printf("Fin du test.\n");
}


action env_action_sample(){
  return (enum action)(rand() % number_actions);
}

void alloc_visited()
{
        visited = malloc(rows * sizeof(int*));
        int i;
        for (i = 0; i < rows; ++i){
                visited[i] = malloc(cols * sizeof(int*));
        }
}


void init_visited()
{
        alloc_visited();

        int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (maze[i][j] == '+') {
                                visited[i][j] = wall;
                        } else if (maze[i][j] == 'g') {
                                visited[i][j] = goal;
                        } else {
                                visited[i][j] = unknown;
                        }
                }
        }
}

