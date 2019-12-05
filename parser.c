#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "strsep.h"

char ** parse_args( char * line ) {
  size_t str_len = strlen(line)+1;
  int num_args = 8;
  char ** final = calloc(sizeof(char *), num_args);
  char * copy = calloc(sizeof(char), str_len);
  char * copy_original = copy;
  strncpy(copy, line, str_len);
  char * temp = copy;
  int i;
  for (i = 0; (temp = cstrsep(&copy, " ")) != 0; i++) {
    if (i == num_args - 1) {
      num_args *= 2;
      final = realloc(final, num_args * sizeof(char *));
      int k;
      for (k = num_args / 2; k < num_args; k++) {
        final[k] = 0;
      }
    }
    int mini_len = strlen(temp);
    final[i] = calloc(sizeof(char), mini_len + 1);
    strncpy(final[i], temp, mini_len);
    final[i][mini_len] = 0;
  }
  free(copy_original);
  return final;
}
