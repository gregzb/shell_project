#include "parse.h"
#include "d_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char is_digit(char c) {
  return '0' <= c && c <= '9';
}

d_string d_string_escape(d_string d_str, d_string* literal_chars, char default_state) {
  d_string temp = d_string_init(8);
  *literal_chars = d_string_init(8);

  char state = default_state;

  int i;
  for (i = 0; i < d_str.length;) {
    if (d_str.content[i] == '"') {
      state = !state;
      //printf("Flipped state\n");
      i++;
    }
    //int j;
    else if (i < d_str.length - 1) {
      if (d_str.content[i] == '\\' && (state == 0 || d_str.content[i+1] == '"')) {
        //(*literal_chars)[temp.length] = 1;
        d_string_append_char(literal_chars, (state << 1) | 1);
        d_string_append_char(&temp, d_str.content[i+1]);
        i+=2;
      } else {
        d_string_append_char(literal_chars, (state << 1) | 0);
        d_string_append_char(&temp, d_str.content[i]);
        i++;
      }
    } else {
      d_string_append_char(literal_chars, (state << 1) | 0);
      d_string_append_char(&temp, d_str.content[i]);
      i++;
    }
  }

  return temp;
}

int d_string_safe_split(d_string d_str, d_string literal_chars, char delimiter, d_string** arr_split_out, d_string** literal_chars_out){
  int num_elems = 8;
  *arr_split_out = malloc(sizeof(d_string) * num_elems);
  *literal_chars_out = malloc(sizeof(d_string) * num_elems);

  int cnt = 0, start = 0, end;
  for (end = 0; end < d_str.length; end++) {
    if (d_str.content[end] == delimiter && literal_chars.content[end] == 0) {
      if (end - start > 0) {
        //copy.content[end] = 0;
        num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
        num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);
        (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
        (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
        //(*arr_split_out)[cnt++] = d_string_from_c(copy.content + start);
      }
      start = end+1;
    } else if (end == d_str.length - 1) {
      end++;
      num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
      num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);
      (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
      (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
      //(*arr_split_out)[cnt++] = d_string_from_c(copy.content + start);
    }
  }

  return cnt;
}

int count_quotes(d_string d_str, d_string literal_chars) {
  int quote_count = 0;

  for (int i = 0; i < d_str.length; i++) {
    if (d_str.content[i] == '"' && (literal_chars.content[i] & 1) == 0) {
      quote_count++;
    }
  }
  return quote_count;
}
