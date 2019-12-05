#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* cstrsep(char** str, char* delimiter) {

  char * original = *str;

  int delimiters = strlen(delimiter);

  int curr = 0;
  while (original[curr] != 0) {
    int j;
    for (j = 0; j < delimiters; j++) {
      char delimiter_char = delimiter[j];
      if (original[curr] == delimiter_char) {
        original[curr] = 0;
        *str = original+curr+1;
        return original;
      }
    }
    curr += 1;
  }
  *str = original+curr+1;
  if (curr == 0) {
    return NULL;
  }
  return original;
}
