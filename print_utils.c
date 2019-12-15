#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "d_string.h"
#include "print_utils.h"

void set_color(unsigned char color) {
  printf("\033[38;5;%hhum", color);
}

void set_err_color(unsigned char color) {
  fprintf(stderr, "\033[38;5;%hhum", color);
}

enum color {Black, DarkRed, DarkGreen, DarkYellow, DarkBlue, Purple, DarkCyan, LightGrey, DarkGrey, Red, Green, Yellow, Blue, Pink, Cyan, White};

void greeting() {
    set_color(Yellow);
    printf("EGG Shell v0.2 (Greg Zborovsky & Emma Choi)\n");
    set_color(White);
    fflush(stdout);
}

void print_error(char* error) {
  set_err_color(Red);
  fprintf(stderr, "%s\n", error);
  set_err_color(White);
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
