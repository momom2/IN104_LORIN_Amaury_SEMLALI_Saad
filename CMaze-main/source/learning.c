#include "../include/mazeEnv.h"
#include "../include/learning.h"

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


int greedy_learning(int* current_row, int* current_col, float alpha, float epsilon){
	char Q[rows*cols][number_actions];



}


int main(){
   maze_make("maze.txt");
   init_visited();

   printf("%d, %d \n", rows, cols);
   printf("number of actions :  %d \n", number_actions); 
   maze_render();
   /*   ...   
   add_crumbs();*/
   maze_render();
   printf("Résultat de recherche par apprentissage.\n Appuyer sur une touche pour continuer.\n");
   getchar();

   return 0;
}