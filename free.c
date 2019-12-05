#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_args(char ** args) {
  size_t num_args = strlen((char *) args);
  int i;
  for (i = 0; i < num_args; i++) {
    free(args[i]);
  }
  free(args);
}
