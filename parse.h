#include "d_string.h"
#include <string.h>

/*
d_str, literal_chars, default_state -> d_str is the string to escape, literal_chars contains chars which should be treated literally, default_state used to remember whether in quotations or not
returns -> an escaped string
Returns an escaped string with quotations removed, escape details are output literal_chars
*/
d_string d_string_escape(d_string d_str, d_string* literal_chars, char default_state);

/*
d_str, literal_chars, delimiter, arr_split_out, literal_chars_out -> d_str is the string to split, literal_chars contains chars which should be treated literally, delimiter is char to split by, arr_split_out outputs the split-ed d_strings, literal_chars_out outputs the escapings for the splits
returns -> number of d_string in output array
Returns an array of d_strings who are separated by delimiter, but not touching literal characters
*/
int d_string_safe_split(d_string d_str, d_string literal_chars, char delimiter, d_string** arr_split_out, d_string** literal_chars_out);
/*
d_str, literal_chars-> d_str is the string to examine, literal_chars contains chars which should be treated literally
returns -> number of unescaped quotes in d_str
Returns number of unescaped quotes in d_str
*/
int count_quotes(d_string d_str, d_string literal_chars);

/*
d_str, literal_chars, arr_split_out, literal_chars_out, redir_info-> d_str is the command string to parse, literal_chars contains chars which should be treated literally, arr_split_out will contain the individual arguments, literal_chars_out will contain literal chars of individual args, redir_info will contain fds for redirection
returns -> number of args
parses args of d_str with respect to 7 redirection operators and literal characters
*/
int parse_args(d_string d_str, d_string literal_chars, d_string** arr_split_out, d_string** literal_chars_out, int* redir_info);
