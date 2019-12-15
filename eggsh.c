#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>

#include "print_utils.h"
#include "d_string.h"
#include "parse.h"
#include "filestream_handling.h"

/*
args, num_args, redir_info, pipe -> array of args, length of array, what/where to redirect, pipe read/write ends
returns -> void
Runs the command present in the args array while performing redirection and applying setup for piping
*/
void process_args(d_string *args, int num_args, int* redir_info, int* pipe) {
  if (num_args >= 1) {
    if (strcmp(args[0].content, "exit") == 0) {
      //cleanup before leaving to make valgrind a happy ducky (but it still doesnt free the outside semicolon split arr)
      d_string_free_arr(args, num_args);
      exit(0);
    } else if (strcmp(args[0].content, "cd") == 0) {
      apply_redirection(redir_info);
      if (num_args >= 2) {
        int result = chdir(args[1].content);
        if (result != 0) {
          print_error(strerror(errno));
        }
      } else {
        chdir(getenv("HOME"));
      }
    } else {
      int result = fork();
      if (result) {
        //parent
        if (pipe[0] >= 0 && pipe[1] >= 0) {
          close(pipe[1]);
          dup2(pipe[0], STDIN_FILENO);
          close(pipe[0]);
        }
        int status;
        wait(&status);
      } else {
        if (pipe[0] >= 0 && pipe[1] >= 0) {
          close(pipe[0]);
          dup2(pipe[1], STDOUT_FILENO);
          close(pipe[1]);
        }

        apply_redirection(redir_info);

        char ** c_arr = d_string_arr_to_c(args, num_args);
        int result = execvp(c_arr[0], c_arr);
        if (result != 0) {
          print_error(strerror(errno));
        }
        free(c_arr);
        d_string_free_arr(args, num_args);
        exit(result); //uhh
      }
    }
  }
}

/*
user_input, literal_chars -> A full user input, literal chars of user input
returns -> void
Separates the user input by ;, then by |, then parses, then passes to execute function, then frees stuff
*/
void process_input(d_string user_input, d_string literal_chars) {
  d_string *semi_split;
  d_string *semi_split_literals;
  int split_sides = d_string_safe_split(user_input, literal_chars, ';', &semi_split, &semi_split_literals);

  int prev_read = -1;

  int default_streams[3];
  save_streams(default_streams);

  int i;
  for (i = 0; i < split_sides; i++) {
    d_string *pipe_split;
    d_string *pipe_split_literals;
    int pipe_sides = d_string_safe_split(semi_split[i], semi_split_literals[i], '|', &pipe_split, &pipe_split_literals);

    int k;
    for (k = 0; k < pipe_sides; k++) {

      int temp_pipe[] = {-1, -1};
      if (k < pipe_sides - 1) {
        pipe(temp_pipe);
      }

      int fds[7];

      d_string *args;
      d_string *args_literals;

      int num_args = parse_args(pipe_split[k], pipe_split_literals[k], &args, &args_literals, fds);

      int j;
      for (j = 0; j < num_args; j++) {
        //its not separating, parse args broken
        if (args[j].content[0] == '~' && args_literals[j].content[0] == 0) {
          d_string tilde = d_string_from_c("~");
          d_string home_env = d_string_from_c(getenv("HOME"));

          d_string_replace(&args[j], tilde, home_env);

          d_string_free(tilde);
          d_string_free(home_env);
        }
      }
      if (num_args > 0) {
        process_args(args, num_args, fds, temp_pipe);
      }

      d_string_free_arr(args, num_args);
      d_string_free_arr(args_literals, num_args);
    }

    d_string_free_arr(pipe_split, pipe_sides);
    d_string_free_arr(pipe_split_literals, pipe_sides);
  }

  restore_streams(default_streams);

  d_string_free_arr(semi_split, split_sides);
  d_string_free_arr(semi_split_literals, split_sides);
}

/*
returns -> void
Continually asks user for input, calls escape, allows for arbirary length input
*/
void input_loop() {
  d_string d_user_input = d_string_init(32);
  d_string total_literal_chars = d_string_init(32);
  char user_input[256];
  int default_val = 0;
  while (fgets(user_input, 256, stdin) != NULL) {
    d_string temp = d_string_from_c(user_input);
    d_string literal_chars;
    d_string escaped = d_string_escape(temp, &literal_chars, default_val);

    d_string_append(&d_user_input, escaped);
    d_string_append(&total_literal_chars, literal_chars);

    d_string_free(escaped);
    d_string_free(literal_chars);

    char last_char = d_user_input.content[d_user_input.length-1];

    if (count_quotes(d_user_input, total_literal_chars) % 2 == 0 && (last_char == '\n' && total_literal_chars.content[d_user_input.length-1] == 0)) {
      d_string_delete(&d_user_input, d_user_input.length-1, d_user_input.length);
      d_string_delete(&total_literal_chars, total_literal_chars.length-1, total_literal_chars.length);
      process_input(d_user_input, total_literal_chars);

      d_string_free(d_user_input);
      d_string_free(total_literal_chars);
      d_user_input = d_string_init(32);
      total_literal_chars = d_string_init(32);
      if (isatty(STDIN_FILENO)) {
          print_user_info();
        }
    } else if (total_literal_chars.content[d_user_input.length-1] != 0){
      default_val = total_literal_chars.content[d_user_input.length-1] >> 1;
    }
  }
}

void sighandler(int signo) {

}

int main() {
  signal(SIGINT, sighandler);

  greeting();
  if (isatty(STDIN_FILENO)) {
    print_user_info();
  }
  input_loop();

  return 0;
}
