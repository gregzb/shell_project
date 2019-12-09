#pragma once

//capacity doesnt include the null
struct d_string {
  int length;
  int capacity;
  char* content;
};

typedef struct d_string d_string;

d_string d_string_init(int length);
void d_string_resize(d_string * d_str, int new_length);
d_string d_string_from_c(char * c_str);
d_string d_string_copy(d_string d_str);
void d_string_insert(int idx, d_string *d_dest, d_string d_src);
void d_string_insert_char(int idx, d_string *d_dest, char c);
void d_string_append(d_string *d_dest, d_string d_src);
void d_string_append_char(d_string *d_dest, char c);
int d_string_find(d_string d_str, char c);
int d_string_count(d_string d_str, char c);
int d_string_find_substr(d_string d_str, d_string c);
int d_string_match_substr(int idx, d_string d_str, d_string c);
int d_string_replace(d_string *d_str, d_string to_replace, d_string new);
void d_string_delete(d_string *d_str, int start, int end);
void d_string_slice(d_string *d_str, int start, int end);
// void d_string_split(d_string d_str, char* delimiters, d_string** arr_split, int* filled);
int d_string_split(d_string d_str, char* delimiters, d_string** arr_split_out);
char ** d_string_arr_to_c(d_string *arr, int num_elems);
void d_string_print(d_string d_str);
void d_string_free(d_string d_str);
void d_string_free_arr(d_string *arr, int num_elems);

// for split, return array of d_strings, and num of elements
// use strsep, keep strsepping and converting to d_string
// free any unncessary stuff

//SHould prob use strtok_r()
