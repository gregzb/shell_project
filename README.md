# shell_project
by Greg Zborovsky and Emma Choi

## Implemented features:
- color support and use of username
- command execution
- arbitrary length input
- reading and separating multiple commands on one line with ";"
- multiple, any order redirection supporting >, <, >>, 2>, 2>>, &>
- infinite piping
- support of quotations
- support for character escaping
- support for accidental spaces
- cwd, hostname, username are only displayed if stdin is coming from a terminal
- basic error handling

## Attempted but Unsuccessful Implementations:
- tab autocompletion using readline()
- tab autocompletion using raw mode (code is in rawMode branch)

## To do section:
- Fix unsuccessful implementations
- pipes without popen()
- autcomplete with indent 

## Bugs:
- Visual glitches (cannot be fixed without non-canonical mode

## Additional Information:

## Files and Function headers:
string.c
- `d_string d_string_init();`
  - input: int length
  - returns: a new d_string
  - Intializes a d_string
- `void d_string_resize();`
  - input: d_str, new_capacity -> d_string to modify, new string capacity
  - returns: void
  - Changes the internal capacity of the d_string, zeros out the rest
- `d_string d_string_from_c();`
  - input char * c_str -> a c-style string
  - returns: d_string new d_string
  - Creates a d_string from a c_string
- `d_string d_string_copy();`
  - input: d_str d_string to copy
  - returns: d_string copy of d_str
  - Creates a d_string and copies the old d_str into the new one
- `void d_string_insert();`
  - input: int idx -> index to insert at
  d_string *d_dest -> d_dest to add to
  d_string d_src -> d_src is the string to insert
  - returns: void
  - Inserts d_src into d_dest at index idx
- `void d_string_insert_char();`
  - input: int idx -> index to insert at
  d_string * d_dest -> d_dest to add to
  - char c -> d_src is the character to insert
- `void d_string_append();`
  - input: d_string * d_dest -> d_dest to add to d_src is the string to insert
  d_string d_src -> d_src is the string to insert
  - returns: void
  - Inserts d_src into d_dest at end of d_dest
- `void d_string_append_char();`
  - input: d_string * d_dest -> d_dest to add to
  char c -> d_src is the character to insert
  - returns: void
  - Inserts c into d_dest at end of d_dest
- `int d_string_find();`
  - input: d_string d_str -> d_str to search through
  char c -> character to find
  - returns: int index of where c was found, or -1 if not
  - Finds the first index of char c in d_str
-  `int d_string_count();`
  - input: d_string d_str ->  d_str to search through,
  char c -> c is character to count
  - returns int number of times that c appears in d_str
  - Counts number of times that the character c appears in d_str
- `int d_string_find_substr();`
  - input: d_string d_str -> string to search through
  d_string c -> string to find
  - returns: int index of where c was found in d_str, or -1 if not
  - Finds the first occurence of c in d_str
- `int d_string_match_substr();`
  - input: int idx -> index at which to check, 
  d_string d_str -> d_str is the string to search through
  d_string c -> c is string to find
  - returns -> int 1 if found, -1 if not
  - returns 1 if at index idx of d_str string c can be found, otherwise -1
- `int d_string_replace();
  - input: d_string d_str -> being operated on
  d_string to_replace -> the string to replace
  d_string new -> is the string to insert
  returns: int index of replacement
  - Finds the location of to_replace in d_str, and if found, deletes it and inserts new in its place
- `void d_string_delete();`
  - input: d_string * d_str, start -> the string being operated on
  int start -> begin deletion index
  int end -> end delection index
  - returns: void
  - Will remove the characters at indexes `[start, end)` from d_str
- `void d_string_slice();`
  - input: d_string d_str -> the string being operated on
  int start ->  begin index
  int end -> end index
  - returns: void
  - Will remove all chaacters besides the characters at indexes  `[start, end)` from d_str
- `d_string d_string_substr();`
  - input: d_str -> d_str is the string being operated on
  int start -> begin index
  int end -> end index
  - returns: d_string
  - Instead of modifying d_str, a new d_string is created. Otherwise identical to d_string_slice
- `int d_string_split();`
  - d_string d_str -> the string being operated on
  char * delimiters -> a list of character delimiters
  d_string ** arr_split_out -> where to store the split results
  - returns: int number of args in arr_split_out
  - d_str is not modified, arr_split_out is an array of d_strings that is malloc'ed and will contain all of the separated d_strings
- `char ** d_string_arr_to_c();`
  - input: d_string arr -> arr of d_strings
  int num_elems -> num of d_strings in arr
  - returns: an array of c-style string
  - creates and returns array of c-style strings from d_strings (useful for exec-ing)
- `void d_string_print();`
  - input: d_string d_str -> the d_string to print
  - returns: void
  - prints a d_string, both text and some info (debug)
- `char d_string_free();`
  - input: d_string d_str: the d_string to free
  returns: 1
  - frees the char * in a d_string
- `char d_string_free_arr();`
  - input: d_string * arr -> arr of d_strings to free
  int num_elems -> num of d_string
  -returns: int 1
  - frees all the d_string in an arr and the array itself
- `int d_string_resize_arr();`
  - input: d_string arr is the d_string arr to resize
  int num_elems -> the current size
  int cnt -> the current idx
  - returns: new current size
  - if arr is too small for the current index, resize it

filestream_handling.c
- `char open_redirection_file();`
  - input: d_string name -> name of file
  int next_word_mode
  - returns: void
  - sets color to red and prints to stderr
- `void apply_redirection();`
  - input: int * redir_info
  - returns: void
  - each index represents a different redirect operator and has an fd (multiple of same redirector or same file stream redirector is undefined)
- `void save_streams();`
  - input: int * streams -> array of length 3 in which to save streams
  - returns: void
  - uses dup on STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO
- `void restore_streams();`
  - input: int * streams -> array of length 3 from which to restore streams
  - returns: void
  - dup2's the backups back to STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO

parse.c
- `d_string d_string_escape();`
  - input: d_string d_str -> the string to escape
  d_string * literal_chars -> contains chars which should be treated literally
  char default_state used to remember whether in quotations or not
  - returns: d_string an escaped string
  - Returns an escaped string with quotations removed, escape details are output literal_chars
- `int d_string_safe_split();`
  - input: d_string d_str is the string to split
  d_string literal_chars contains chars which should be treated literally
  char delimiter is char to split by
  d_string ** arr_split_out outputs the split-ed d_strings
  d_string ** literal_chars_out outputs the escapings for the splits
  - returns: number of d_string in output array
  - Returns an array of d_strings who are separated by delimiter, but not touching literal characters
- `int count_quotes();`
  - input: d_string d_str is the string to examine
  d_string literal_chars contains chars which should be treated literally
  - returns: number of unescaped quotes in d_str
  - Returns number of unescaped quotes in d_str
- `int parse_args();`
  - d_string d_str is the command string to parse
  d_string literal_chars contains chars which should be treated literally
  d_string ** arr_split_out will contain the individual arguments
  d_string ** literal_chars_out will contain literal chars of individual args 
  int * redir_info will contain fds for redirection
  returns: number of args
  parses args of d_str with respect to 7 redirection operators and literal characters

print_utils.c
- `void set_color();`
  - input: unsigned char color -> sets all future stdout text color to this
  - returns: void
  - sets all future stdout text color to specified color
- `void set_err_color();`
  - input: color -> sets all future stderr text color to this
  - returns: void
  - sets all future stderr text color to specified color
- `void greeting();`
  - input: none
  - returns: void
  - prints introduction to eggsh
- `void print_error(char* error);
  - input: char * error -> string to print to stderr
  - returns: void
  - sets color to red and prints to stderr
- `void print_user_info();`
  - input: none
  - returns: void
  - prints user directory, hostname, username, etc

eggsh.c
- `void process_args(d_string *args, int num_args, int* redir_info)`
  - input: d_string args-> array of args
  int num_args -> length of array
  int * redir_info -> what/where to redirectpipe read/write ends
  int * pipe -> pipe read/write ends
  - returns: void
  - Runs the command present in the args array while performing redirection and applying setup for piping
- `void process_input(d_string user_input, d_string literal_chars)`
  - d_string user_input -> A full user input
  d_string literal_chars -> literal chars of user input
  - returns: void
  - Separates the user input by ;, then by |, then parses, then passes to execute function, then frees stuff
- `void input_loop()`
  - input: none
  - returns: void
  - Continually asks user for input, calls escape, allows for arbirary length input
- `int main()`
