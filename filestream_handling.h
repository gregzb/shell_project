#include "d_string.h"
char open_redirection_file(d_string name, int next_word_mode);
void save_streams(int* streams);
void restore_streams(int* streams);
void apply_redirection(int* redir_info);
