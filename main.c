#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "rawMode.h"
#include "strcmp.h"

int main(){
  enableRawMode();

  char c;
  char *line;

  DIR * dir;
  char ** matchingDirectories = malloc(200 * sizeof(char *));
  //This will only PROPERLY support directories with less than 200 things rn
  //GREG PLEASE FIX THESE NUMBERS!
  int i;
  for (i = 0; i < 200; i++ ){
    matchingDirectories[i] = malloc(500 * sizeof(char *));
  }

  int numDir;

  struct dirent * nextFile;
  struct stat file;
  while (read(STDIN_FILENO, &c, 1) == 1){
    if (c == '\n'){
      printf("output: %s\n", line);
      strcpy(line, "");
    }if (c == '\t'){
      dir = opendir("./");
      numDir = 0;
      if (errno > 0){
        printf("%s\n", strerror(errno));
        exit(0);
      }
      nextFile = readdir(dir);
      while (nextFile != NULL){
        char * name = nextFile -> d_name;
        printf("name: %s\n", name);
        printf("size of line %lu\n", strlen(line));
        if (strncmp(line, name, strlen(line)) == 0){
          printf("THIS IS IMPORTANT\n");
          strcpy(matchingDirectories[numDir], name);
          printf("matchingDirectories[numDir]: %s\n", matchingDirectories[numDir]);
          numDir++;
        }
        nextFile = readdir(dir);
      }

      printf("numDir: %d\n", numDir);

      int dirn;
      if (numDir == 0){
        //printf("THIS SHOULD BE TRUE\n");
        //printf("matchingDirectories[0]: %s\n", matchingDirectories[0]);
        strcpy(line, matchingDirectories[0]);
      }else{
        printf("%d\n", numDir);
        int lowestComp = 0;
        int dirn = 0;
        int smallestLength = strlen(matchingDirectories[0]);
        int flip = 1;
        printf("dirn: %d\n", dirn);
        printf("numDir: %d\n", numDir);
        for(dirn = 1; dirn < numDir; dirn++){
          printf("dirn: %d\n", dirn );
          printf("numDir: %d\n", numDir + 1);
          if (strlen(matchingDirectories[dirn]) < smallestLength){
            smallestLength = strlen(matchingDirectories[dirn]);
          }
        }
        printf("smallestLength %d\n", smallestLength);
        for(lowestComp = 0; lowestComp < smallestLength && (flip == 1); lowestComp++){
          char c = matchingDirectories[0][lowestComp];
          printf("%c\n", c);
          printf("dirn: %d, numDir + 1: %d, flip: %d\n", dirn, numDir + 1, flip);
          //printf("%s\n", );

          for (dirn = 1; dirn < numDir && (flip == 1); numDir ++){
            printf("YES\n");
            if (strcmp(&matchingDirectories[dirn][lowestComp], &c) != 0){
              printf("Here\n");
              strncpy(line, matchingDirectories[dirn], lowestComp + 1);
              printf("matchingDirectories[dirn]: %s\n", matchingDirectories[dirn]);
              printf("lowestComp + 1: %d\n", lowestComp + 1);
              printf("line %s\n", line);
              flip = 0;
            }
          }
          printf("Did it finish?!\n");
        }
        printf("How about this?\n");

        /**
        int lowestComp = sizeof(matchingDirectories[0]);
        printf("This is working at least :(\n");
        for (dirn = 0; dirn < numDir + 1; dirn++){
          //printf("How about this?\n");
          //strcmp(matchingDirectories[dirn], line);
          //printf("B\n");
          //myStrcmp(matchingDirectories[dirn], line);
          //printf("A\n");
          //WHAT I HAVE TO DO IS CHECK FOR SIMILARITIES AMONGST THE DIRECTORIES!
          printf("lowestComp: %d\n", lowestComp);
          printf("myStrcmp: %d\n", myStrcmp(matchingDirectories[dirn], line));
          int i = 0;


          if (strcmp(matchingDirectories[dirn], line) > 0 && myStrcmp(matchingDirectories[dirn], line) < lowestComp){
            printf("Its not going into this?\n");
            lowestComp = myStrcmp(matchingDirectories[dirn], line);
            strncpy(line, matchingDirectories[dirn], lowestComp);
          }

        }
        **/
      }
      /** PRINT OUT THE REST OF WHATEVER YOU DECIDED TO PUT RIGHT NEXT TO USER INPUT
      char * difference;
      strncpy(difference, matchingDirectories[dirn] + sizeof(line), matchingDirectories[dirn] - lowestComp);
      printf("%s", difference);
      **/

      closedir(dir);
    }else{
      strcat(line, &c);
    }
  }

  return 0;
}
