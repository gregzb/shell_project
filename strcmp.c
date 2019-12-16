#include <stdio.h>
#include <string.h>

int myStrcmp(char *leftStr, char *rightStr){
  //int currentComp = strcmp(leftStr, rightStr);
  printf("leftStr: %s\n", leftStr);
  printf("rightStr: %s\n", rightStr);

  int i = 0;
  printf("rightStr length: %lu\n", strlen(rightStr));
  printf("leftStr length: %lu\n", strlen(leftStr));
  if (strcmp(leftStr, rightStr) < 0){
    while(leftStr[i] == rightStr[i] && i < strlen(rightStr)){
      //printf("It's this!\n");
      i++;
    }
    return i;
  }if (strcmp(leftStr, rightStr) > 0){
    while(leftStr[i] == rightStr[i] && i < strlen(leftStr)){
      //printf("It's this!\n");
      i++;
    }
    return i;
  }
  printf("i %d\n", i);
  return strlen(leftStr);
  /**
  if (currentComp < 0){
    int i = 0;
    while(leftStr[i] == rightStr[i]){
      i++;
    }
    return i;
  }if (currentComp > 0){
    int i = 0;
    while(leftStr[i] == rightStr[i]){
      i++;
    }
    return i;
  }
  **/
}
