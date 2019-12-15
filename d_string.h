#pragma once

//capacity doesnt include the null
struct d_string {
  int length;
  int capacity;
  char* content;
};

typedef struct d_string d_string;

/*
length -> initial capacity
returns -> a new d_string
Intializes a d_string
*/
d_string d_string_init(int length);

/*
d_str, new_capacity -> d_string to modify, new string capacity
returns -> void
Changes the internal capacity of the d_string, zeros out the rest
*/
void d_string_resize(d_string * d_str, int new_length);

/*
c_str -> a c-style string (char *)
returns -> a new d_string
Creates a d_string from a c_string
*/
d_string d_string_from_c(char * c_str);

/*
d_str -> d_string to copy
returns -> a copy of d_str
Creates a d_string and copies the old d_str into the new one
*/
d_string d_string_copy(d_string d_str);

/*
idx, d_dest, d_src -> index to insert at, d_dest to add to, d_src is the string to insert
returns -> void
Inserts d_src into d_dest at index idx
*/
void d_string_insert(int idx, d_string *d_dest, d_string d_src);

/*
idx, d_dest, c -> index to insert at, d_dest to add to, d_src is the character to insert
returns -> void
Inserts c into d_dest at index idx
*/
void d_string_insert_char(int idx, d_string *d_dest, char c);

/*
d_dest, d_src -> d_dest to add to, d_src is the string to insert
returns -> void
Inserts d_src into d_dest at end of d_dest
*/
void d_string_append(d_string *d_dest, d_string d_src);

/*
d_dest, c -> d_dest to add to, d_src is the character to insert
returns -> void
Inserts c into d_dest at end of d_dest
*/
void d_string_append_char(d_string *d_dest, char c);

/*
d_str, c -> d_str to search through, c is character to find
returns -> index of where c was found, or -1 if not
Finds the first index of char c in d_str
*/
int d_string_find(d_string d_str, char c);

/*
d_str, c -> d_str to search through, c is character to count
returns -> number of times that c appears in d_str
Counts number of times that the character c appears in d_str
*/
int d_string_count(d_string d_str, char c);

/*
d_str, c -> d_str is the string to search through, c is string to find
returns -> index of where c was found in d_str, or -1 if not
Finds the first occurence of c in d_str
*/
int d_string_find_substr(d_string d_str, d_string c);

/*
idx, d_str, c -> idx is the index at which to check, d_str is the string to search through, c is string to find
returns -> 1 if found, -1 if not
returns 1 if at index idx of d_str string c can be found, otherwise -1
*/
int d_string_match_substr(int idx, d_string d_str, d_string c);

/*
d_str, to_replace, new -> d_str is being operated on, to_replace is the string to replace, new is the string to insert
returns -> index of replacement
Finds the location of to_replace in d_str, and if found, deletes it and inserts new in its place
*/
int d_string_replace(d_string *d_str, d_string to_replace, d_string new);

/*
d_str, start, end -> d_str is the string being operated on, start is begin deletion index, end is end delection index
returns -> void
Will remove the characters at indexes [start, end) from d_str
*/
void d_string_delete(d_string *d_str, int start, int end);

/*
d_str, start, end -> d_str is the string being operated on, start is begin  index, end is end index
returns -> void
Will remove all chaacters besides the characters at indexes [start, end) from d_str
*/
void d_string_slice(d_string *d_str, int start, int end);

/*
d_str, start, end -> d_str is the string being operated on, start is begin  index, end is end index
returns -> a new d_string
Instead of modifying d_str, a new d_string is created. Otherwise identical to d_string_slice
*/
d_string d_string_substr(d_string d_str, int start, int end);

/*
d_str, delimiters, arr_split_out -> d_str is the string being operated on, delimiters is a list of character delimiters, arr_split_out is where to store the split results
returns -> number of args in arr_split_out
d_str is not modified, arr_split_out is an array of d_strings that is malloc'ed and will contain all of the separated d_strings
*/
int d_string_split(d_string d_str, char* delimiters, d_string** arr_split_out);

/*
arr, num_elems -> arr is arr of d_strings, num_elems is num of d_strings in arr
returns -> an array of c-style string
creates and returns array of c-style strings from d_strings (useful for exec-ing)
*/
char ** d_string_arr_to_c(d_string *arr, int num_elems);

/*
d_str -> the d_string to print
returns -> void
prints a d_string, both text and some info (debug)
*/
void d_string_print(d_string d_str);

/*
d_str -> the d_string to free
returns -> 1
frees the char * in a d_string
*/
char d_string_free(d_string d_str);

/*
arr, num_elems -> arr is the arr of d_strings to free, num_elems is num of d_string
returns -> 1
frees all the d_string in an arr and the array itself
*/
char d_string_free_arr(d_string *arr, int num_elems);

/*
arr, num_elems, cnt -> arr is the d_string arr to resize, num_elems is the current size, cnt is the current idx
returns -> new current size
if arr is too small for the current index, resize it
*/
int d_string_resize_arr(d_string *arr, int num_elems, int cnt);
