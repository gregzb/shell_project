# shell_project

## Implemented features:
- basic parsing feature
- reading and seaparing multiple commands on one line with ";"
- full redirection using > and <
- color and use of username
- infinite length input allowed

## Attempted but Unsuccessful Implementations:
- Quotes
- Escapes

## To do section:
- Fix unsuccessful implementations
- pipes without popen()
- autcomplete with indent 

## Bugs:
- Visual glitches (cannot be fixed without non-canonical mode)

## Additional Information:

## Function headers:
- For strings:
  - `d_string d_string_init(int length);`
  - `void d_string_resize(d_string * d_str, int new_length);`
  - `d_string d_string_from_c(char * c_str);`
  - `d_string d_string_copy(d_string d_str);`
  - `void d_string_insert(int idx, d_string * d_dest, d_string d_src);`
  - `void d_string_insert_char(int idx, d_string *d_dest, char c);`
  - `void d_string_append(d_string *d_dest, d_string d_src);`
  - `void d_string_append_char(d_string *d_dest, char c);`
  - `int d_string_find(d_string d_str, char c);`
  - `int d_string_count(d_string d_str, char c);`
  - `int d_string_find_substr(d_string d_str, d_string c);`
  - `int d_string_match_substr(int idx, d_string d_str, d_string c);`
  - `int d_string_replace(d_string *d_str, d_string to_replace, d_string new);`
  - `void d_string_delete(d_string *d_str, int start, int end);`
  - `void d_string_slice(d_string *d_str, int start, int end);`
  - `int d_string_split(d_string d_str, char* delimiters, d_string** arr_split_out);`
  - `char ** d_string_arr_to_c(d_string *arr, int num_elems);`
  - `void d_string_print(d_string d_str);`
  - `void d_string_free(d_string d_str);`
  - `void d_string_free_arr(d_string *arr, int num_elems);`
- `void set_color(unsigned char color)`
- `void greeting()`
- `void print_user_info()`
- `void process_args(d_string *args, int num_args, int* redir_info)`
- `int parse_args(d_string d_str, d_string** arr_split_out, int* redir_info)`
- `void input_loop()`
- `void sighandler(int signo)`
- `int main()`
- `d_string d_string_escape(d_string d_str, char** literal_chars);`
