#include "graphEnv.h" 
#include "functions.h"

void alloc_graph(){
     graph = malloc(size * sizeof(double*));

     for(int i=0; i<size; i++) {
         graph[i] = malloc(size * sizeof(char*));
     }
}

void graph_make(char* file_name){
     printf("Creating maze...\n");
     int i; int j; double poids; // Le poids de l'arête de i à j.
     char c;
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
               } else if(c == "," && swap == 0){
  					  swap = 1;             
  			   } else if(c == "," && swap == 1){
  					  swap = 2;		  
               } else if(swap == 0){
                      size_s[size_i]=c;
                      size_i++;
               } else if(swap == 1){
               		  start = atoi(c);
               } else {
               		  goal = atoi(c);
               }
     	 }
     }
     //printf("File read.\n");

     /* convertir le string en nombre */
     size = atoi(size_s);
     //printf("Allocating graph.\n");
     alloc_graph();
     //printf("Graph allocated.\n");
     for (int i=0; i<(size*size); i++){
         scan_result = fscanf(file, "%d %d %lf\n", &i, &j, &poids);
         if(scan_result == 3){
         	graph[i][j] = poids;
         	graph[j][i] = poids;
         }
         else{
         	break;
         }
     }
     //printf("Closing file.\n");
     fclose(file);
     //printf("File closed.\n");
}

void graph_render(){
     for (int i=0; i<size; i++) {
     	 printf("[");
         for (int j=0; j< cols; j++){
             printf("%.2lf,", maze[i][j]);
         }
         printf("]\n");
     }
     printf("\n");
}
