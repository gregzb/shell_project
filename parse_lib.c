#include "parse_lib.h"
#include "d_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char is_digit(char c) {
  return '0' <= c && c <= '9';
}

d_string d_string_escape(d_string d_str, char** literal_chars) {
  d_string temp = d_string_init(8);
  *literal_chars = calloc(sizeof(char), d_str.length);

  int i;
  for (i = 0; i < d_str.length;) {
    int j;
    if (i < d_str.length - 1) {
      if (d_str.content[i] == '\\') {
        (*literal_chars)[temp.length] = 1;
        d_string_append_char(&temp, d_str.content[i+1]);
        i+=2;
      } else {
        d_string_append_char(&temp, d_str.content[i]);
        i+=1;
      }
    }
  }

  *literal_chars = realloc(*literal_chars, sizeof(char) * temp.length);

  return temp;
}
