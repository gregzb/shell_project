#include "parse.h"
#include "d_string.h"
#include "print_utils.h"
#include "filestream_handling.h"
#include <errno.h>
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
      i++;
    }
    else if (i < d_str.length - 1 && d_str.content[i] == '\\' && (state == 0 || d_str.content[i+1] == '"')) {
      d_string_append_char(literal_chars, (state << 1) | 1);
      d_string_append_char(&temp, d_str.content[i+1]);
      i+=2;
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
        num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
        num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);
        (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
        (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
      }
      start = end+1;
    } else if (end == d_str.length - 1) {
      end++;
      num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
      num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);
      (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
      (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
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

int parse_args(d_string d_str, d_string literal_chars, d_string** arr_split_out, d_string** literal_chars_out, int* redir_info) {
  int l;
  for (l = 0; l < 7; l++) {
    redir_info[l] = -1;
  }

  char error_found = 0;

  d_string literal_match = d_string_init(16);
  literal_match.length = 8;

  d_string redirectors[] = {
    d_string_from_c(" 2>>"),
    d_string_from_c(" &>"),
    d_string_from_c(" 2>"),
    d_string_from_c(">>"),
    d_string_from_c(">"),
    d_string_from_c("<"),
    d_string_from_c(" ")
  };
  int num_redirectors = 7;

  int num_elems = 8;
  *arr_split_out = malloc(sizeof(d_string) * num_elems);
  *literal_chars_out = malloc(sizeof(d_string) * num_elems);

  int next_word_mode = 6, cnt = 0, start = 0, end;
  for (end = 0; end < d_str.length; end++) {
    int found_delimiter = -1;

    int d;
    for (d = 0; d < num_redirectors && found_delimiter == -1; d++) {
      if (end + redirectors[d].length > d_str.length) continue;

      if (d_string_match_substr(end, d_str, redirectors[d]) == 1) {
        d_string literal_shorter = d_string_substr(literal_match, 0, redirectors[d].length);

        if (d_string_match_substr(end, literal_chars, literal_shorter) == 1 && d_string_free(literal_shorter)) { //should free as well
          found_delimiter = d;
          break;
        }
      }
    }

    if (found_delimiter != -1 || end == d_str.length - 1) {
      if (end == d_str.length-1 && found_delimiter == -1) end++;
      num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
      num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);

      if (end - start > 0) {
        d_string temp = d_string_substr(d_str, start, end);
        if (next_word_mode == 6) {
          (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
          (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
        } else {
          redir_info[next_word_mode] = open_redirection_file(temp, next_word_mode);
          if (redir_info[next_word_mode] < 0) {
            print_error(strerror(errno));
          }
        }
        d_string_free(temp);
      }

      next_word_mode = found_delimiter < next_word_mode ? found_delimiter : next_word_mode;
      end += redirectors[found_delimiter].length - 1;
      start = end + 1;
    }
  }

  int d;
  for (d = 0; d < num_redirectors; d++) {
    d_string_free(redirectors[d]);
  }
  d_string_free(literal_match);

  if (error_found) return -1;

  return cnt;
}
