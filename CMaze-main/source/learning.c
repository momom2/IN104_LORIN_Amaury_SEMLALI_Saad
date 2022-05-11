#include "mazeEnv.h"
#include "../include/functions.h"
#include "learning.h"
#include <time.h>
//extern int rows;
//extern int cols;

#define ARRAY_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

/*  ///  LEGACY CODE  ///
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

void initQ(){
    printf("Initializing.\n");
    printf("Number of actions: %d\nNumber of rows: %d\nNumber of cols: %d\n", number_actions, rows, cols);
    Q = malloc(rows*cols*sizeof(double*));
    for(int i = 0; i<rows; ++i){
        for(int j = 0; j<cols; ++j){
            Q[case_coord(i,j)] = malloc(number_actions*sizeof(double));
        }
    }
    for(int k = 0; k<number_actions; ++k){
        for(int j = 0; j<cols; ++j) {
            for(int i = 0; i<rows; ++i){
                Q[case_coord(i,j)][k]=0;
                //printf("%d %d %f  ", i, j, Q[case_coord(i,j)][k]);
            }
            //printf("\n");
        }
    }
    printf("Initialization complete.\n");
}



action rand_action_uniform(){
    int random_choice = rand()%number_actions;
    return (action) random_choice;
}
action rand_action_epsilon(double** Q,int coordonnee, double epsilon){
    int temp_random_choice = rand()%10000;
    double random_choice = temp_random_choice/10000;
    if (random_choice<epsilon){ //Explore!
        return rand_action_uniform();
    } 
    else { //Remember.
        return (action) argmax(Q[coordonnee],number_actions);
    }
}
action rand_action_boltzmann_exploration(double** Q, int coordonnee, double temperature){
    double poids_max=somme(Q[coordonnee],number_actions,exponential,temperature);
    // Choix de l'action aléatoirement
    double random_chooser = randf(poids_max);
    for(action a=up;a<number_actions;a++){
        /////// WARNING: EXTREMELY VULNERABLE TO FLOAT ROUNDING ERRORS. TODO: REWRITE IN A MORE ROBUST WAY. //////
        if(random_chooser<exponential(Q[coordonnee][a],temperature)){
            return a;
        } else {
            random_chooser -= exponential(Q[coordonnee][a],temperature);
        }
    }
    if(random_chooser>0){
        printf("Error in rand_action_boltzmann_exploration. random_chooser too high.\n");
        return up;
    }
    else {
        printf("Error in rand_action_boltzmann_exploration. Didn't match properly.\n");
        return right;
    }
}

int goal_reached(int coordonnee, int done){
    int goal_coord = case_coord(goal_row, goal_col);
    if (coordonnee == goal_coord){
        if (done == 1) {
            return 1;
        }
        printf("Error: information inconsistent between done and coordonnee.\n");
    }
    return 0;
}

void printQ(double** Q){
    printf("\n");
    for(int i=0;i<rows;++i){
        for(int j=0;j<cols;++j){
            printf("%c ",graphical_move(argmax(Q[case_coord(i,j)],number_actions)));
        }
        printf("\n");
    }
    printf("\n");
}

void train_one_epoch(double epsilon, double temperature, int training_mode){
    // Initialize starting position.
    maze_reset();
        
    int time = 0;
    double current_reward;
    int current_coord = case_coord(state_row,state_col);
    int new_row;
    int new_col;
    int new_coord;
    int done = 0;
    envOutput stepOut;
    
    // Run around in the maze!
    while(!goal_reached(current_coord, done) && time < 500){ 
        //Choose movement.
        action action_chosen;
        if(training_mode == epsilon_greedy){
            action_chosen = rand_action_epsilon(Q, current_coord, epsilon);
        } else {
            if(training_mode == boltzmann_exploration){
                action_chosen = rand_action_boltzmann_exploration(Q, current_coord, temperature);
            } else {
                printf("Training mode unrecognized in train_one_epoch.\n");
                action_chosen = rand_action_uniform();
            }
        }
        //Move.
        stepOut = maze_step(action_chosen);
        current_reward = stepOut.reward;
        done = stepOut.done;
        new_row = stepOut.new_row;
        new_col = stepOut.new_col;
        new_coord = case_coord(new_row,new_col);
        //printf("Tic.\n");
        //Update Q.
        //printf("Test: %f\n", Q[current_coord][a]);
        Q[current_coord][action_chosen] += learning_rate*(current_reward + discount_rate*(listmax(Q[new_coord],number_actions)) - Q[current_coord][action_chosen]);
        //printf("Toc.\n");

        coord(current_coord, &state_row, &state_col);
        if(visited[state_row][state_col] == unknown){
            visited[state_row][state_col] = known;
        }

        current_coord = new_coord;
        coord(current_coord, &state_row, &state_col);
        time++;
    }
}

int main(){
    
    srand(time(NULL));
    // IF test_mode IS SET TO 1, NO AGENT WILL BE SPAWNED NOR TRAINED. 
    int test_mode = 0;
    // debug_mode ACTIVATES A FEW printfS HERE AND THERE. The bigger, the more, like a verbose mode! 
    int debug_mode = 1;

    // INITIALIZATION
    maze_make("maze.txt");
    init_visited();
    if (!test_mode) {

        long sleeping_time = 000; // Time slept (in milliseconds) for graphical/testing purposes.
                                   // set to 0 if you only want to train the agent.
        if(sleeping_time>200){
            printf("Warning: the agent will take a full nap between each epoch: %ld ms.\nYou must change it!\nPlease input a sleeping_time (in ms): (recommended value is 50)\n",sleeping_time);
            int check = scanf("%ld", &sleeping_time);
            getchar();
            if (check == EOF) {
                printf("Wrong entry.\n");
                sleeping_time = 0;
            }
            if (check == 0) {
                while (fgetc(stdin) != '\n') // Read until a newline is found
                    ;
                printf("Wrong entry.\n");
                sleeping_time = 0;
            }
            if(sleeping_time<0 || sleeping_time>200){
                printf("Good try.\n");
                sleeping_time = 000;
            }
        }

        int training_mode = epsilon_greedy;
        printf("Training mode chosen: epsilon_greedy.\n");
        max_epoch = 500;
        current_epoch = 0;
        learning_rate = 0.1;
        discount_rate = 0.9;
        epsilon = 1; // Epsilon gets decreased as epochs go.
        temperature = 1; // Temperature gets decreased as epochs go.
        // In fact, epsilon and temperature are always the same values, but they are used for different things. They are a sacrifice I am willing to make.
        printf("sleeping time: %ld\n",sleeping_time);
        initQ();
        
        if(debug_mode>2){
            printQ(Q);
            maze_render();
        }
        
        // TRAIN THE MODEL//
        printf("Enter to start training.\n");
        
        getchar();

        while(current_epoch<max_epoch){
            if(debug_mode>0){
                printf("Current epoch: %d\nepsilon: %f, temperature: %f\n",current_epoch,epsilon,temperature);
            }
            train_one_epoch(epsilon, temperature, training_mode);
            
            reset_visited();

            if(debug_mode>1){
                printQ(Q);
            }
            msleep(sleeping_time);
            current_epoch++;
            epsilon = maxf(0.95*epsilon,0.01);
            temperature = maxf(0.95*temperature,0.01);
        }
    }

    //////////// TEST ZONE. ////////////
    if(test_mode){
        //int is_random = 0;

        
        //crash_visited();
        //test_somme(is_random);
        //test_envOutput(is_random);
        //test_rand();
        test_randf();
        //test_coord_converter();
    }
    ////////////////////////////////////

    if(!test_mode){
        printf("Résultat de recherche par apprentissage.\nAppuyer sur une touche pour afficher.\n");
        getchar();
        

        ////////////////////
        if(debug_mode>0){
            printQ(Q);
            /*printf("Length of visited: %lux%lu\nLength of maze: %lux%lu\nLength of Q: %lux%lu\n", \
                ARRAY_LENGTH(visited),ARRAY_LENGTH(visited[0]),                             \
                ARRAY_LENGTH(maze),ARRAY_LENGTH(maze[0]),                                   \
                ARRAY_LENGTH(Q),ARRAY_LENGTH(Q[0]));
            */ // Dysfunctional code; ARRAY_LENGTH only works on static arrays.
            maze_render();
        }
        if(debug_mode>1){
            print_visited();
        }
        printf("Fin.\n");
        
        // Free everything. Information wants to be free, and so do mice!
        quit();
    }
    else {
        quit_maze();
    }
    return 0;
}


void quit(){
    quit_maze();
    for(int i = 0; i<rows; ++i){
        //printf("i: %d\n",i);
        //printf("%d",cols);
        //printf("Tic.\n");
        for(int j = 0; j<cols; ++j){
            free(Q[case_coord(i,j)]);
        }
    } 
    printf("Agent terminated.\n");
}

