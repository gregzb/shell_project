#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "d_string.h"

d_string d_string_init(int length) {
  d_string temp;
  if (length <= 0) {
    //ha ha dabbed on
    length = 1;
  }
  temp.length = 0;
  temp.capacity = length;
  temp.content = calloc(sizeof(char), length+1);
  //printf("Inited size %d\n", length);
  return temp;
}

//resizes capacity
void d_string_resize(d_string * d_str, int new_capacity) {
  d_str->content = realloc(d_str->content, sizeof(char) * new_capacity + 1);
  int k;
  for (k = d_str->capacity; k < new_capacity + 1; k++) {
    d_str->content[k] = 0;
  }
  d_str->capacity = new_capacity;
  //printf("Resized to %d\n", new_capacity);
}

d_string d_string_from_c(char * c_str) {
  d_string temp;
  int c_len = strlen(c_str);
  temp.length = c_len;
  temp.capacity = c_len;
  temp.content = calloc(sizeof(char), c_len+1);
  //printf("Created string size: %d\n", c_len);
  strncpy(temp.content, c_str, c_len);
  return temp;
}

d_string d_string_copy(d_string d_str) {
  d_string temp;
  temp.length = d_str.length;
  temp.capacity = d_str.capacity;
  temp.content = calloc(sizeof(char), d_str.capacity + 1);
  //printf("Made copy of size: %d\n", temp.capacity);
  strncpy(temp.content, d_str.content, d_str.length);
  return temp;
}

void d_string_insert(int idx, d_string *d_dest, d_string d_src) {
  int final_len = d_dest->length + d_src.length;
  while (final_len > d_dest->capacity) {
    d_string_resize(d_dest, d_dest->capacity*2);
  }
  int i;
  for (i = final_len-1; i >= final_len-(d_dest->length - idx-1) - 1; i--) {
    //printf("boo %d %d %d\n", i, i-d_src->length, d_dest->content[i - d_src->length]);
    d_dest->content[i] = d_dest->content[i - d_src.length];
  }
  for (i = 0; i < d_src.length; i++) {
    d_dest->content[i+idx] = d_src.content[i];
  }
  d_dest->length = final_len;
  d_dest->content[final_len] = 0;
}

void d_string_insert_char(int idx, d_string *d_dest, char c) {
  char* to_str = "\0\0";
  to_str[0] = c;
  d_string temp = d_string_from_c(to_str);
  d_string_insert(idx, d_dest, temp);
  d_string_free(temp);
}

void d_string_append(d_string *d_dest, d_string d_src) {
  d_string_insert(d_dest->length, d_dest, d_src);
}

void d_string_append_char(d_string *d_dest, char c) {
  d_string_insert_char(d_dest->length, d_dest, c);
}

int d_string_find(d_string d_str, char c) {
  for (int i = 0; i < d_str.length; i++) {
    if (d_str.content[i] == c) {
      return i;
    }
  }
  return -1;
}

int d_string_count(d_string d_str, char c) {
  int counter = 0;
  for (int i = 0; i < d_str.length; i++) {
    if (d_str.content[i] == c) {
      counter++;
    }
  }
  return counter;
}

int d_string_find_substr(d_string d_str, d_string c) {
  if (c.length == 0) return -1;
  int i;
  for (i = 0; i < d_str.length - (c.length - 1); i++) {
    int j;
    char ok = 1;
    for (j = 0; j < c.length; j++) {
      if (d_str.content[i + j] != c.content[j]) {
        ok = 0;
        break;
      }
    }
    if (ok) return i;
  }
  return -1;
}

int d_string_match_substr(int idx, d_string d_str, d_string c) {
  if (c.length == 0) return -1;
  int j;
  for (j = 0; j < c.length; j++) {
    if (d_str.content[idx + j] != c.content[j]) {
      return 0;
    }
  }
  return 1;
}

void d_string_delete(d_string *d_str, int start, int end) {
  if (start < 0 || start >= d_str->length || end < 0 || end >= d_str->length + 1 || end <= start) {
    return;
  }
  //d_str->length -= end - start;
  int i;
  for (int i = end; i < d_str->length; i++) {
    d_str->content[start + i - end] = d_str->content[i];
  }
  d_str->length -= end - start;
  d_str->content[d_str->length] = 0;
  //set last to 0
}

void d_string_slice(d_string *d_str, int start, int end) {
  if (start < 0 || start >= d_str->length || end < 0 || end >= d_str->length + 1 || end <= start) {
    return;
  }
  d_string_delete(d_str, 0, start);
  d_string_delete(d_str, end - start, d_str->length);
}

int d_string_split(d_string d_str, char* delimiters, d_string** arr_split_out) {
  int num_elems = 8;
  *arr_split_out = malloc(sizeof(d_string) * num_elems);
  //printf("Arr inited to %d\n", num_elems);

  d_string copy = d_string_copy(d_str);

  char* rest = copy.content;
  char* token;

  int counter = 0;

  while ((token = strtok_r(rest, delimiters, &rest))) {
    if (counter >= num_elems - 1) {
      num_elems *= 2;
      *arr_split_out = realloc(*arr_split_out, sizeof(d_string) * num_elems);
      //printf("Arr resized to %d\n", num_elems);
    }
    (*arr_split_out)[counter++] = d_string_from_c(token);
  }

  d_string_free(copy);
  return counter;
}

char ** d_string_arr_to_c(d_string *arr, int num_elems) {
  char **temp = calloc(sizeof(char*), num_elems+1);
  //printf("C Arr inited to %d\n", num_elems+1);
  int i;
  for (i = 0; i < num_elems; i++) {
    temp[i] = arr[i].content;
  }
  return temp;
}

int d_string_replace(d_string *d_str, d_string to_replace, d_string new) {
  int substr = d_string_find_substr(*d_str, to_replace);
  if (substr < 0) return -1;
  d_string_delete(d_str, substr, substr + to_replace.length);
  d_string_insert(substr, d_str, new);
  return substr;
}

// void d_string_split(d_string d_str, char* delimiters, d_string** arr_split, int* filled) {
//   *filled = 0;
//   int num_elems = 8;
//   *arr_split = calloc(sizeof(d_string), num_elems);
//
//   int counter = 0;
//
//   int num_delims = strlen(delimiters);
//
//   int start = 0;
//   int end = 0;
//   for (end = 0; end < d_str.length; end++) {
//     char c = d_str.content[end];
//     int j;
//     for (j = 0; j < num_delims; j++) {
//       char delim = delimiters[j];
//       if (c == delim) {
//         d_string temp = d_string_init(end-start);
//         d_string copy = d_string_copy(d_str);
//         d_string_slice(&copy, start, end);
//         d_string_append(&temp, &copy);
//         (*arr_split)[counter++] = temp;
//
//         char delim_temp[2] = "\0\0";
//         delim_temp[0] = delim;
//
//         if (counter >= num_elems-3) {
//           num_elems *= 2;
//           *arr_split = realloc(*arr_split, num_elems * sizeof(d_string));
//         }
//
//         (*arr_split)[counter++] = temp;
//         (*arr_split)[counter++] = d_string_from_c(delim_temp);
//         *filled += 2;
//         d_string_free(copy);
//         end++;
//         start = end;
//         break;
//       }
//     }
//   }
// }

void d_string_print(d_string d_str) {
  //assumes byte at index length is null
  printf("d_string: (%d, %d) %s", d_str.length, d_str.capacity, d_str.content);
}

void d_string_free(d_string d_str) {
  free(d_str.content);
}

void d_string_free_arr(d_string *arr, int num_elems) {
  int i;
  for (i = 0; i < num_elems; i++) {
    free(arr[i].content);
  }
  free(arr);
}
