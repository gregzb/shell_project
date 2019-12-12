#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "d_string.h"
#include "parse.h"
#include <signal.h>

#include <limits.h>
#include <strings.h>
#include <fcntl.h>

void set_color(unsigned char color) {
  printf("\033[38;5;%hhum", color);
}

enum color {Black, DarkRed, DarkGreen, DarkYellow, DarkBlue, Purple, DarkCyan, LightGrey, DarkGrey, Red, Green, Yellow, Blue, Pink, Cyan, White};

void greeting() {
    set_color(Yellow);
    printf("EGG Shell v0.2 (Greg Zborovsky & Emma Choi)\n");
    set_color(White);
    fflush(stdout);
}

void print_user_info() {
  char cwd_buf[PATH_MAX+1];
  char username[256];
  getlogin_r(username, 256);

  char hostname[256];
  gethostname(hostname, 256);

  getcwd(cwd_buf, PATH_MAX);

  d_string d_cwd = d_string_from_c(cwd_buf);

  d_string tilde = d_string_from_c("~");
  d_string home_env = d_string_from_c(getenv("HOME"));

  if (d_string_find_substr(d_cwd, home_env) == 0) {
    d_string_replace(&d_cwd, home_env, tilde);
  }

  d_string_free(tilde);
  d_string_free(home_env);

  set_color(Green);
  printf("%s@%s:", username, hostname);
  set_color(DarkCyan);
  printf("%s", d_cwd.content);
  d_string_free(d_cwd);
  set_color(White);
  printf("$ ");
  fflush(stdout);
}

void process_args(d_string *args, int num_args, int* redir_info) {
  if (num_args >= 1) {
    if (strcasecmp(args[0].content, "exit") == 0) {
      //cleanup before leaving to make valgrind a happy ducky (but it still doesnt free the outside semicolon split arr)
      d_string_free_arr(args, num_args);
      exit(0);
    } else if (strcasecmp(args[0].content, "cd") == 0) {
      if (num_args >= 2) {
        int result = chdir(args[1].content);
        if (result != 0) {
          printf("%s\n", strerror(errno));
        }
      } else {
        chdir(getenv("HOME"));
        //take me home pls
      }
    } else {
      int result = fork();
      if (result) {
        //parent
        int status;
        wait(&status);
      } else {
        int i;
        for (i = 0; i < 7; i++) {
          int fd = redir_info[i];
          //printf("fd %i %d\n", i, fd);
          if (fd != -1) {
            if (i == 5) {
              dup2(fd, STDIN_FILENO);
              //redir_info[next_word_mode] = open(copy.content + start, O_RDONLY);
            } else if (i == 2 || i == 4){
              dup2(fd, STDOUT_FILENO);
              //redir_info[next_word_mode] = open(copy.content + start, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            } else if (i == 0 || i == 3){
              dup2(fd, STDOUT_FILENO);
              //redir_info[next_word_mode] = open(copy.content + start, O_WRONLY | O_APPEND | O_CREAT, 0644);
            } else if (i == 1){
              dup2(STDOUT_FILENO, STDERR_FILENO);
              //redir_info[next_word_mode] = -2;
            }
          }
        }

        char ** c_arr = d_string_arr_to_c(args, num_args);
        int result = execvp(c_arr[0], c_arr);
        if (result != 0) {
          printf("%s\n", strerror(errno));
        }
        //some cleanup for if execvp fails bc why not
        free(c_arr);
        d_string_free_arr(args, num_args);
        exit(result); //uhh
      }
    }
  }
}

int parse_args(d_string d_str, d_string literal_chars, d_string** arr_split_out, d_string** literal_chars_out, int* redir_info) {
  int l;
  for (l = 0; l < 7; l++) {
    redir_info[l] = -1;
  }

  d_string literal_match = d_string_init(16);
  // for (l = 0; l < 8; l++) {
  //   d_string_append_char(&literal_match, '0');
  // }
  literal_match.length = 8;

  d_string redirectors[] = {
    d_string_from_c(" 2>>"),
    d_string_from_c(" &>"),
    d_string_from_c(" 2>"),
    d_string_from_c(">>"),
    d_string_from_c(">"),
    d_string_from_c("<"),
    d_string_from_c(" ")
  };
  int num_redirectors = 7;

  int num_elems = 8;
  *arr_split_out = malloc(sizeof(d_string) * num_elems);
  *literal_chars_out = malloc(sizeof(d_string) * num_elems);

  int next_word_mode = 6, cnt = 0, start = 0, end;
  for (end = 0; end < d_str.length; end++) {
    int found_delimiter = -1;

    int d;
    for (d = 0; d < num_redirectors && found_delimiter == -1; d++) {
      if (end + redirectors[d].length > d_str.length) continue;

      if (d_string_match_substr(end, d_str, redirectors[d]) == 1) {
        d_string literal_shorter = d_string_substr(literal_match, 0, redirectors[d].length);
        if (d_string_match_substr(end, literal_chars, literal_shorter) == 1 && d_string_free(literal_shorter)) { //should free as well
          found_delimiter = d;
          break;
        }
      }
    }

    if (found_delimiter != -1 || end == d_str.length - 1) {
      if (end == d_str.length-1 && found_delimiter == -1) end++;
      num_elems = d_string_resize_arr(*arr_split_out, num_elems, cnt);
      num_elems = d_string_resize_arr(*literal_chars_out, num_elems, cnt);

      if (end - start > 0) {
        d_string temp = d_string_substr(d_str, start, end);
        switch(next_word_mode) {
          case 6:
          (*literal_chars_out)[cnt] = d_string_substr(literal_chars, start, end);
          (*arr_split_out)[cnt++] = d_string_substr(d_str, start, end);
          break;
          case 5:
          redir_info[next_word_mode] = open(temp.content, O_RDONLY);
          break;
          case 2:
          case 4:
          redir_info[next_word_mode] = open(temp.content, O_WRONLY | O_TRUNC | O_CREAT, 0644);
          break;
          case 0:
          case 3:
          redir_info[next_word_mode] = open(temp.content, O_WRONLY | O_APPEND | O_CREAT, 0644);
          break;
          case 1:
          redir_info[next_word_mode] = open(temp.content, O_WRONLY | O_TRUNC | O_CREAT, 0644);
          //redir_info[next_word_mode] = -2;
          break;
        }
        d_string_free(temp);
      }

      next_word_mode = found_delimiter < next_word_mode ? found_delimiter : next_word_mode;
      end += redirectors[found_delimiter].length - 1;
      start = end + 1;
    }
  }

  int d;
  for (d = 0; d < num_redirectors; d++) {
    d_string_free(redirectors[d]);
  }
  d_string_free(literal_match);

  return cnt;
}

void process_input(d_string user_input, d_string literal_chars) {
  d_string *semi_split;
  d_string *semi_split_literals;
  int split_sides = d_string_safe_split(user_input, literal_chars, ';', &semi_split, &semi_split_literals);

  int prev_read = -1;

  int i;
  for (i = 0; i < split_sides; i++) {
    d_string *pipe_split;
    d_string *pipe_split_literals;
    int pipe_sides = d_string_safe_split(semi_split[i], semi_split_literals[i], '|', &pipe_split, &pipe_split_literals);

    int temp_pipe[2];
    pipe(temp_pipe);

    int k;
    for (k = 0; k < pipe_sides; k++) {
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
        process_args(args, num_args, fds);
      }

      d_string_free_arr(args, num_args);
      d_string_free_arr(args_literals, num_args);
    }

    d_string_free_arr(pipe_split, pipe_sides);
    d_string_free_arr(pipe_split_literals, pipe_sides);

    //close prev
    //set prev to the current
  }

  //finish up pipe handling here

  d_string_free_arr(semi_split, split_sides);
  d_string_free_arr(semi_split_literals, split_sides);
}

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

    //printf("%d, %d, %d\n", count_quotes(d_user_input, total_literal_chars), last_char, total_literal_chars.content[d_user_input.length-1]);
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
  //This becomes more important in raw mode
  //printf("Caught %d\n", signo);
}

int main() {
  //signal(SIGINT, sighandler);

  greeting();
  if (isatty(STDIN_FILENO)) {
    print_user_info();
  }
  input_loop();

  return 0;
}
