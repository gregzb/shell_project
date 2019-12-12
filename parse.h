#include "d_string.h"
#include <string.h>

d_string d_string_escape(d_string d_str, d_string* literal_chars, char default_state);
int d_string_safe_split(d_string d_str, d_string literal_chars, char delimiter, d_string** arr_split_out, d_string** literal_chars_out);
int count_quotes(d_string d_str, d_string literal_chars);
