#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "d_string.h"
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

//maybe make redirinfo store strings so it can be replaced with ~ home thing
int parse_args(d_string d_str, d_string** arr_split_out, int* redir_info) {
  //printf("INITIAL: %s\n", d_str.content);

  int l;
  for (l = 0; l < 7; l++) {
    redir_info[l] = -1;
  }

  //ESTABLISH PRECEDENCE LMAO
  char * delimiters[] = {" 2>>", " &>", " 2>", " >>", ">", "<", " "};
  int delimiter_lengths[] = {4, 3, 3, 3, 1, 1, 1};
  int num_delims = 7;

  int num_elems = 8;
  *arr_split_out = malloc(sizeof(d_string) * num_elems);
  d_string copy = d_string_copy(d_str);

  d_string space = d_string_from_c(" ");
  d_string_insert(0, &copy, space);
  //d_string_append(&copy, space);
  d_string_free(space);

  //printf("COPY: %s\n", copy.content);

  int found_word = 1;

  int next_word_mode = 6;

  int counter = 0;

  int start = 0;
  int str_pos;
  for (str_pos = 0; str_pos < copy.length; str_pos++) {

    char current_char = copy.content[str_pos];

    int found_delimiter = -1;

    int d;
    for (d = 0; d < num_delims && found_delimiter == -1; d++) {
      char * delimiter = delimiters[d];
      int delimiter_length = delimiter_lengths[d];

      if (str_pos + delimiter_length > copy.length) continue;

      int valid = 1;

      int k;
      for (k = 0; k < delimiter_length; k++) {
        if (copy.content[str_pos+k] != delimiter[k]) {
          valid = 0;
          break;
        }
      }

      if (valid) {
        //printf("yerr %d %d\n", str_pos, delimiter_length);
        found_delimiter = d;
        found_word = 0;
        break;
      }
      //printf("FINISHED DEL LOOP %d, %d\n", str_pos, d);
    }

    //printf("YEET %d\n", found_delimiter >= 0 || str_pos == copy.length - 1);

    if (found_delimiter >= 0 || str_pos == copy.length - 1) {
      //fprintf(stderr, "FOUND DELIM AT: %d %d %d %d %s\n", str_pos, found_delimiter, copy.capacity, copy.length, copy.content);
      if (counter >= num_elems) {
        num_elems *= 2;
        *arr_split_out = realloc(*arr_split_out, sizeof(d_string) * num_elems);
      }

      if (found_delimiter >= 0) {
        int delimiter_length = delimiter_lengths[found_delimiter];
        int k;
        //fprintf(stderr, "CHONK1\n");
        for (k = 0; k < delimiter_length; k++) {
          copy.content[str_pos + k] = 0;
        }
      }
      //fprintf(stderr, "CHONK2\n");
      if (str_pos - start > 0 || str_pos == copy.length - 1) {
        //printf("%d\n", next_word_mode);
        if (next_word_mode == 6) {
          found_word = 1;
          //fprintf(stderr, "CHONK3\n");
          //printf("START: %d DELIM: %d\n", start, found_delimiter);
          //printf("%s\n", copy.content+start);
          //d_string fuck = d_string_from_c(copy.content + start);
          //fprintf(stderr, "CHONK3.2\n");
          //d_string_print(fuck);
          //fprintf(stderr, "CHONK3.5\n");
            (*arr_split_out)[counter++] = d_string_from_c(copy.content + start);
          //fprintf(stderr, "CHONK4\n");
        } else {
          //fprintf(stderr, "CHONK5\n");
          if (next_word_mode == 5) {
            redir_info[next_word_mode] = open(copy.content + start, O_RDONLY);
          } else if (next_word_mode == 2 || next_word_mode == 4){
            redir_info[next_word_mode] = open(copy.content + start, O_WRONLY | O_TRUNC | O_CREAT, 0644);
          } else if (next_word_mode == 0 || next_word_mode == 3){
            redir_info[next_word_mode] = open(copy.content + start, O_WRONLY | O_APPEND | O_CREAT, 0644);
          } else if (next_word_mode == 1){
            redir_info[next_word_mode] = -2;
          }
          //fprintf(stderr, "CHONK6\n");
        }
      }
        //fprintf(stderr, "CHONK7\n");
      next_word_mode = found_delimiter < next_word_mode ? found_delimiter : next_word_mode;
      str_pos += 0; //str pos will get incremented

      start = str_pos + 1;
    }

  }

  d_string_free(copy);
  return counter;
}

void input_loop() {
  char user_input[8192];
  while (fgets(user_input, 8192, stdin) != NULL) {
    int str_len = strlen(user_input);
    user_input[str_len-1] = 0;
    d_string input = d_string_from_c(user_input);

    d_string *semi_split;
    int split_sides = d_string_split(input, ";", &semi_split);

    d_string_free(input);

    int i;
    for (i = 0; i < split_sides; i++) {
      d_string semi = semi_split[i];

      d_string *pipe_split;
      int pipe_sides = d_string_split(semi, "|", &pipe_split);

      int k;
      for (k = 0; k < pipe_sides; k++) {
        d_string pipe_side = pipe_split[k];

        d_string *args;
        //int num_args = d_string_split(pipe_side, " ", &args);
        int fds[7];
        int num_args = parse_args(pipe_side, &args, fds);
        //printf("%d args\n", num_args);
        //printf("FINISHED PARSE\n");

        int j;
        for (j = 0; j < num_args; j++) {
          if (args[j].content[0] == '~') {
            d_string tilde = d_string_from_c("~");
            d_string home_env = d_string_from_c(getenv("HOME"));

            d_string_replace(&args[j], tilde, home_env);

            d_string_free(tilde);
            d_string_free(home_env);
          }
          //printf("%s\n", args[j].content);
        }

        process_args(args, num_args, fds);
        d_string_free_arr(args, num_args);
      }

      d_string_free_arr(pipe_split, pipe_sides);
    }

    d_string_free_arr(semi_split, split_sides);

    if (isatty(STDIN_FILENO)) {
      print_user_info();
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
