#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void forked_process_piping_test(int column, char* stype, int wd[2], int fd[2], int numRows, int numCols, char*** inputFile){

  char*** data = inputFile;
  char buffer[4096];
  char what[80];
  int nbytes;
  int n;

  while(1){
    char* ptr = buffer;
    // block until there is info in the what_pipe
    while((nbytes = read(wd[0], what, 80))==0){
    }
    // exit if error in what_pipe
    if(nbytes < 0){
      printf("error in what_pipe\n");
      break;
    }
    printf("server: selection %s\n", what);
    // check if client requests display
    if(strcmp(what,"display")==0){
      for(int r = 0; r < numRows; r++){
        if(strcmp(data[r][column], stype)==0){
          for(int c = 0; c < numCols; c++){
            if(c != column){
              n = sprintf(ptr, "%s", data[r][c]);
              ptr += n;
              n = sprintf(ptr, ",");
              ptr += n;
            }
          }
          n = sprintf(ptr, "\n");
          ptr += n;
        } 
      }
      nbytes = write(fd[1], buffer, 4096);
      printf("server: display %d\n", nbytes);
    }
    // check if client reqeusts save
    if(strcmp(what,"save")==0){
      for(int r = 0; r < numRows; r++){
        if(strcmp(data[r][column], stype)==0){
          for(int c = 0; c < numCols; c++){
            if(c != column){
              n = sprintf(ptr, "%s", data[r][c]);
              ptr += n;
              n = sprintf(ptr, ",");
              ptr += n;
            }
          }
          n = sprintf(ptr, "\n");
          ptr += n;
        } 
      }
      // write to file
      char fileName[80];
      sprintf(fileName, "output_%s_%d.txt", stype, column);
      FILE* file = fopen(fileName, "w");
      fprintf(file, "%s", buffer);
      fclose(file);
      printf("server: saving file\n");
    }
    // check if client requests summary
    if(strcmp(what,"summary")==0){
      int counter = 0;
      for(int r = 0; r < numRows; r++){
        if(strcmp(data[r][column], stype)==0){
          counter++;
        }
      }
      sprintf(buffer, "%d", counter);
      nbytes = write(fd[1], buffer, 4096);
      printf("server: counter: %d\n", counter);
    }
    // check if client requests exit
    if(strcmp(what, "exit")==0){
      printf("server: exit\n");
      break;
    } 
  }
}