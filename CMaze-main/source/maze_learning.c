#include "../include/mazeEnv.h"
#include "../include/functions.h"
#include "../include/maze_learning.h"
#include <time.h>

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

void initQ(int debug_mode){
    printf("Initializing Q.\n");
    if(debug_mode>0){
        printf("Number of actions: %d\nNumber of rows: %d\nNumber of cols: %d\n", number_actions, rows, cols);
    }
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
                if(debug_mode>2){
                    printf("%d %d %f  ", i, j, Q[case_coord(i,j)][k]);
                }
            }
            if(debug_mode>2){printf("\n");}
        }
    }
    printf("Initialization of Q complete.\n");
}
void initQ2(int debug_mode){
    printf("Initializing Q1 and Q2.\n");
    if(debug_mode>0){
        printf("Number of actions: %d\nNumber of rows: %d\nNumber of cols: %d\n", number_actions, rows, cols);
    }
    Q1 = malloc(rows*cols*sizeof(double*));
    Q2 = malloc(rows*cols*sizeof(double*));
    for(int i = 0; i<rows; ++i){
        for(int j = 0; j<cols; ++j){
            Q1[case_coord(i,j)] = malloc(number_actions*sizeof(double));
            Q2[case_coord(i,j)] = malloc(number_actions*sizeof(double));
        }
    }
    for(int k = 0; k<number_actions; ++k){
        for(int j = 0; j<cols; ++j) {
            for(int i = 0; i<rows; ++i){
                Q1[case_coord(i,j)][k]=0;
                Q2[case_coord(i,j)][k]=0;
                if(debug_mode>2){ // Poor implementation. TODO: rewrite it correctly.
                    printf("%d %d %f  ", i, j, Q1[case_coord(i,j)][k]);
                    printf("%d %d %f  ", i, j, Q2[case_coord(i,j)][k]); 
                }
            }
            if(debug_mode>2){printf("\n");}
        }
    }
    printf("Initialization of Q1 and Q2 complete.\n");
}




void reset_result_table(){
    for(int i=0;i<result_table_length;++i){
        result_table[i]=0;
    }
    result_table[result_table_length] = 0;
}

action rand_action_uniform(){
    int random_choice = rand()%number_actions;
    return (action) random_choice;
}
action rand_action_epsilon(double** Q,int coordonnee, double epsilon){
    double random_choice = randf(1);
    if (random_choice<epsilon){ //Explore!
        return rand_action_uniform();
    } 
    else { //Remember.
        return (action) argmax(Q[coordonnee],number_actions);
    }
}
action rand_action_epsilon2(double** Q1,double** Q2,int coordonnee, double epsilon){
    double random_choice = randf(1);
    if (random_choice<epsilon){ //Explore!
        return rand_action_uniform();
    } 
    else { //Remember.
        return (action) argmax2(Q1[coordonnee],Q2[coordonnee],number_actions);
    }
}


action rand_action_boltzmann_exploration(double** Q, int coordonnee, double temperature){
    double poids_max=somme(Q[coordonnee],number_actions,exponential,temperature);
    // Choix de l'action aléatoirement
    double random_chooser = randf(poids_max);
    for(action a=0;a<number_actions;a++){
        /////// WARNING: EXTREMELY VULNERABLE TO FLOAT ROUNDING ERRORS. TODO: REWRITE IN A MORE ROBUST WAY. //////
        if(random_chooser<exponential(Q[coordonnee][a],temperature)){
            return a;
        } else {
            random_chooser -= exponential(Q[coordonnee][a],temperature);
        }
    }
    if(random_chooser>0){
        printf("Warning in rand_action_boltzmann_exploration: random_chooser too high.\n");
        return 0;
    }
    else {
        printf("Warning in rand_action_boltzmann_exploration: didn't match properly.\n");
        return number_actions-1;
    }
}

action rand_action_boltzmann_exploration2(double** Q1, double** Q2, int coordonnee, double temperature){
    double* liste = malloc(number_actions * sizeof(double)); // liste = Q1[coordonnee] + Q2[coordonnee] added vectorially; its size is number_actions
    for(int i=0;i<number_actions;++i){
        liste[i] = Q1[coordonnee][i]+Q2[coordonnee][i];
    }
    double poids_max=somme(liste,number_actions,exponential,temperature);
    // Choix de l'action aléatoirement
    double random_chooser = randf(poids_max);
    for(action a=0;a<number_actions;a++){
        /////// WARNING: EXTREMELY VULNERABLE TO FLOAT ROUNDING ERRORS. TODO: REWRITE IN A MORE ROBUST WAY. //////
        if(random_chooser<exponential(liste[a],temperature)){
            free(liste);
            return a;
        } else {
            random_chooser -= exponential(liste[a],temperature);
        }
    }
    if(random_chooser>0){
        printf("Warning in rand_action_boltzmann_exploration: random_chooser too high.\n");
        free(liste);
        return 0;
    }
    else {
        printf("Warning in rand_action_boltzmann_exploration: didn't match properly.\n");
        free(liste);
        return number_actions-1;
    }
}

int goal_reached(int coordonnee, int done){
    int goal_coord = case_coord(goal_row, goal_col);
    if (coordonnee == goal_coord){
        if (done == 1) {
            return 1;
        }
        printf("Warning in goal_reached: information inconsistent between done and coordonnee.\n");
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

int train_one_epoch(double** Q, double epsilon, double temperature, int training_mode, int max_time, int debug_mode){
    // Initialize starting position.
    maze_reset();
        
    int time = 0;
    double current_reward;
    int current_coord = case_coord(state_row,state_col);
    action action_chosen;
    int new_row;
    int new_col;
    int new_coord;
    int done = 0;
    envOutput stepOut;
    
    // Run around in the maze!
    while(!goal_reached(current_coord, done) && time < max_time){ 
        //Choose movement.
        if(training_mode == epsilon_greedy){
            action_chosen = rand_action_epsilon(Q, current_coord, epsilon);
        } else if(training_mode == boltzmann_exploration){
            action_chosen = rand_action_boltzmann_exploration(Q, current_coord, temperature);
        } else {
            printf("Warning in train_one_epoch: training_mode unrecognized.\n");
            action_chosen = rand_action_uniform();
        }

        //Move.
        stepOut = maze_step(action_chosen);
        current_reward = stepOut.reward;
        done = stepOut.done;
        new_row = stepOut.new_row;
        new_col = stepOut.new_col;
        new_coord = case_coord(new_row,new_col);

        //Update Q.
        if(debug_mode>3){
            printf("Test: %f\n", Q[current_coord][action_chosen]);
        }
        Q[current_coord][action_chosen] += learning_rate*(current_reward + discount_rate*(listmax(Q[new_coord],number_actions)) - Q[current_coord][action_chosen]);

        coord(current_coord, &state_row, &state_col);
        if(visited[state_row][state_col] == unknown){
            visited[state_row][state_col] = known;
        }

        current_coord = new_coord;
        coord(current_coord, &state_row, &state_col);
        time++;
    }
    if (goal_reached(current_coord,done)){
        return 1;
    } else if (time>=max_time){
        printf("The goal is not reached.\n");
        return 0;
    } else {
        printf("Warning in train_one_epoch: invalid end of training for this epoch.\n");
        return -1;
    }
}
int train_one_epoch2(double** Q1, double** Q2, double epsilon, double temperature, int training_mode, int max_time, int debug_mode){
    // Initialize starting position.
    maze_reset();
        
    int time = 0;
    double current_reward;
    int current_coord = case_coord(state_row,state_col);
    action action_chosen;
    int new_row;
    int new_col;
    int new_coord;
    int done = 0;
    envOutput stepOut;
    
    // Run around in the maze!
    while(!goal_reached(current_coord, done) && time < max_time){ 
        //Choose movement.
        if(training_mode == epsilon_greedy){
            action_chosen = rand_action_epsilon2(Q1, Q2, current_coord, epsilon);
        } else if(training_mode == boltzmann_exploration){
            action_chosen = rand_action_boltzmann_exploration2(Q1, Q2, current_coord, temperature);
        } else {
            printf("Warning in train_one_epoch2: training mode unrecognized.\n");
            action_chosen = rand_action_uniform();
        }

        //Move.
        stepOut = maze_step(action_chosen);
        current_reward = stepOut.reward;
        done = stepOut.done;
        new_row = stepOut.new_row;
        new_col = stepOut.new_col;
        new_coord = case_coord(new_row,new_col);

        //Update Q1,Q2.
        if(debug_mode>3){
            printf("Test: %f\n", Q1[current_coord][action_chosen]);
            printf("Test: %f\n", Q2[current_coord][action_chosen]);
        }
        
        int choix=rand()%2;
        if(debug_mode>3){
            printf("Choix: %d\n",choix);
        }
        
        if (choix==0){
            Q1[current_coord][action_chosen] += learning_rate*(current_reward + discount_rate*(listmax(Q2[new_coord],number_actions)) - Q1[current_coord][action_chosen]);
        }
        else{
            Q2[current_coord][action_chosen] += learning_rate*(current_reward + discount_rate*(listmax(Q1[new_coord],number_actions)) - Q2[current_coord][action_chosen]);
        }

        coord(current_coord, &state_row, &state_col);
        if(visited[state_row][state_col] == unknown){
            visited[state_row][state_col] = known;
        }

        current_coord = new_coord;
        coord(current_coord, &state_row, &state_col);
        time++;
    }
    if (goal_reached(current_coord,done)){
        return 1;
    } else if (time>=max_time){
        printf("The goal is not reached.\n");
        return 0;
    } else {
        printf("Warning in train_one_epoch2: invalid end of training for this epoch.\n");
        return -1;
    }
}
int train_one_epochSARSA(double** Q, double epsilon, double temperature, int training_mode, int max_time, int debug_mode){
    // Initialize starting position.
    maze_reset();
        
    int time = 0;
    double current_reward;
    int current_coord = case_coord(state_row,state_col);
    action action_chosen; 
    int new_row;
    int new_col;
    int new_coord;
    int done = 0;
    envOutput stepOut;
    action new_action_chosen;

    // Consider movement.
    if(training_mode == epsilon_greedy){
        action_chosen = rand_action_epsilon(Q, current_coord, epsilon);
    } else if(training_mode == boltzmann_exploration){
        action_chosen = rand_action_boltzmann_exploration(Q, current_coord, temperature);
    } else {
        printf("Warning in train_one_epochSARSA: training_mode unrecognized.\n");
        action_chosen = rand_action_uniform();
    }
    
    // Run around in the maze!
    while(!goal_reached(current_coord, done) && time < max_time){ 
        //Move.
        stepOut = maze_step(action_chosen);
        current_reward = stepOut.reward;
        done= stepOut.done;
        new_row = stepOut.new_row;
        new_col = stepOut.new_col;
        new_coord = case_coord(new_row,new_col);
        // Choose movement.
        if(training_mode == epsilon_greedy){
            new_action_chosen = rand_action_epsilon(Q, new_coord, epsilon);
        } else if(training_mode == boltzmann_exploration){
            new_action_chosen = rand_action_boltzmann_exploration(Q, new_coord, temperature);
        } else {
            printf("Warning in train_one_epochSARSA: training_mode unrecognized.\n");
            new_action_chosen = rand_action_uniform();
        }

        //Update Q.
        if(debug_mode>3){
            printf("Test: %f\n", Q[current_coord][action_chosen]);
        }
        Q[current_coord][action_chosen] += learning_rate*(current_reward + (discount_rate*Q[new_coord][new_action_chosen]) - Q[current_coord][action_chosen]);

        coord(current_coord, &state_row, &state_col);
        if(visited[state_row][state_col] == unknown){
            visited[state_row][state_col] = known;
        }

        current_coord = new_coord;
        action_chosen = new_action_chosen;
        coord(current_coord, &state_row, &state_col);
        time++;
    }
    if (goal_reached(current_coord,done)){
        return 1;
    } else if (time>=max_time){
        printf("The goal is not reached.\n");
        return 0;
    } else {
        printf("Warning in train_one_epoch: invalid end of training for this epoch.\n");
        return -1;
    }
}

int main(){    
    srand(time(NULL));
    // IF test_mode IS SET TO 1, NO AGENT WILL BE SPAWNED NOR TRAINED. 
    int test_mode = 0;
    // debug_mode ACTIVATES A FEW printfS HERE AND THERE. The bigger, the more, like a verbose mode! 
    int debug_mode = 2;

    ////////////// INITIALIZATION //////////////
    maze_make("bigmaze.txt");
    init_visited();
    int training_mode = boltzmann_exploration;                  // No current way to display the training mode automatically.
    int learning_type = sarsa_learning;                 // Idem.
    printf("Training mode chosen: Boltzmann exploration.\n");   // Be sure to edit it if you change the training mode.
    printf("Learning type chosen: SARSA.\n");           // Idem.
        
    if (!test_mode) {

        long sleeping_time = 000;   // Time slept (in milliseconds) for graphical/testing purposes.
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
                printf("Nice try! 0_°\n");
                sleeping_time = 000;
            }
        }

        max_epoch = 10000;
        max_time = 500;
        current_epoch = 0;
        learning_rate = 0.1;
        discount_rate = 0.9;
        epsilon = 1; // Epsilon gets decreased as epochs go.
        temperature = 1; // Temperature gets decreased as epochs go.
        // In fact, epsilon and temperature are always the same values as implemented here, but they are used for different things.
        max_super_winrate = 1;
        printf("sleeping time: %ld\n",sleeping_time);
        if(learning_type == simple_learning || learning_type == sarsa_learning){
            initQ(debug_mode);
        } else if(learning_type == double_learning){
            initQ2(debug_mode);
        } else {
            printf("Warning during Q initialization: unsupported learning_type.\n");
        }
        
        if(debug_mode>2){
            if(learning_type == simple_learning || learning_type == sarsa_learning){
                printQ(Q);
            } else if(learning_type == double_learning){
                printQ(Q1);
                printQ(Q2);
            } else {
                printf("Warning during Q initialization: unsupported learning_type.\n");
            }
            maze_render();
        }
        
        ////////////// END OF INITIALIZATION //////////////




        // TRAIN THE MODEL //
        printf("Press any key to start training.\n");
        
        getchar();
        double winrate = 0; // Percentage of games won among the last 100.
        int super_winrate = 0; // Consecutive games without error.
        while(super_winrate<max_super_winrate && current_epoch<max_epoch){
            if(debug_mode>1){
                printf("Current epoch: %d\nepsilon: %f, temperature: %f\n",current_epoch,epsilon,temperature);
            }
            reset_visited();
            if(learning_type == simple_learning){
                result_table[current_epoch%result_table_length] = train_one_epoch(Q, epsilon, temperature, training_mode, max_time, debug_mode);
            } else if(learning_type == double_learning){
                result_table[current_epoch%result_table_length] = train_one_epoch2(Q1, Q2, epsilon, temperature, training_mode, max_time, debug_mode);
            } else if(learning_type == sarsa_learning){
                result_table[current_epoch%result_table_length] = train_one_epochSARSA(Q, epsilon, temperature, training_mode, max_time, debug_mode);
            } else {
                printf("Warning during training: unsupported learning_type.\n");
            }
            result_table[result_table_length] = current_epoch;
            

            if(debug_mode>2){
                if(learning_type == simple_learning){
                    printQ(Q);
                } else if(learning_type == double_learning){
                    printQ(Q1);
                    printQ(Q2);
                } else {
                    printf("Warning during training: unsupported learning_type.\n");
                }
                
            }
            msleep(sleeping_time);
            winrate = get_latest_results(result_table,result_table_length);
            if(debug_mode>0){
                printf("Win rate: %.0f%%\n",winrate);
            }
            if(winrate == 100){
                super_winrate +=1;
            } else {
                super_winrate = 0;
            }
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
        //test_randf();
        //test_coord_converter();
    }
    ////////////////////////////////////

    if(!test_mode){
        printf("\nResults of the training.\nPress any key to display.\n");
        getchar();
        

        ////////////////////
        if(debug_mode>0){

            if(learning_type == simple_learning || learning_type == sarsa_learning){
                printQ(Q);
            } else if(learning_type == double_learning){
                printQ(Q1);
                printQ(Q2);
            } else {
                printf("Warning in result display: unsupported learning_type.\n");
            }
            maze_render();
        }
        if(debug_mode>2){
            print_visited();
        }
        printf("The end.\nHope you enjoyed it! ^-^\n");
        
        // Free everything. Information wants to be free, and so do mice!
        if(learning_type == simple_learning || learning_type == sarsa_learning){
            quit(debug_mode);
        } else if(learning_type == double_learning){
            quit2(debug_mode);
        } else {
            printf("Warning when quitting: unsupported learning_type.\n");
        }
        
    }
    else {
        quit_maze();
    }
    return 0;
}


void quit(int debug_mode){
    if(debug_mode>0){
        printf("Quitting...\n");
    }
    quit_maze();
    for(int i = 0; i<rows; ++i){
        
        
        for(int j = 0; j<cols; ++j){
            free(Q[case_coord(i,j)]);
        }
    }
    free(Q);
    printf("Agent terminated.\n");
}
void quit2(int debug_mode){
    if(debug_mode>0){
        printf("Quitting...\n");
    }
    quit_maze();
    for(int i = 0; i<rows; ++i){
        if(debug_mode>2){
            printf("i: %d\n",i);
            printf("%d",cols);
        }
        for(int j = 0; j<cols; ++j){
            free(Q1[case_coord(i,j)]);
            free(Q2[case_coord(i,j)]);
        }
    }
    free(Q1);
    free(Q2);
    printf("Agent terminated.\n");
}

