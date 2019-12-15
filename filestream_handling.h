#include "d_string.h"

/*
name, next_word_mode -> name of file, which redirect operator needs the file
returns -> void
sets color to red and prints to stderr
*/
char open_redirection_file(d_string name, int next_word_mode);

/*
streams -> array of length 7 from which to apply redirection
returns -> void
each index represents a different redirect operator and has an fd (multiple of same redirector or same file stream redirector is undefined)
*/
void apply_redirection(int* redir_info);

/*
streams -> array of length 3 in which to save streams
returns -> void
uses dup on STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO
*/
void save_streams(int* streams);

/*
streams -> array of length 3 from which to restore streams
returns -> void
dup2's the backups back to STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO
*/
void restore_streams(int* streams);
