#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include <linux/limits.h>
#include <strings.h>
#include "header.h"

//enum color {Black, DarkRed, DarkGreen, DarkYellow, DarkBlue, Purple, DarkCyan, LightGrey, DarkGrey, Red, Green, Yellow, Blue, Pink, Cyan, White};

int main() {
  set_color(Yellow);
  printf("EGG Shell v0.1 (Greg Zborovsky & Emma Choi)\n");
  set_color(White);
  char cwd_buf[PATH_MAX+1];
  char username[256];
  getlogin_r(username, 256);

  getcwd(cwd_buf, PATH_MAX);

  set_color(Green);
  printf("%s: ", username);
  set_color(DarkCyan);
  printf("%s", cwd_buf);
  set_color(White);
  printf("$ ");

  char user_input[8192];
  while (fgets(user_input, 8192, stdin) != NULL) {
    int str_len = strlen(user_input);
    user_input[str_len-1] = 0;
    char ** args = parse_args(user_input);

    int num_args = 0;
    int i;
    for (i = 0; args[i] != 0; i++) {
      if (strlen(args[i]) != 0) {
        num_args += 1;
      }
    }

    if (num_args >= 1) {
      if (strcasecmp(args[0], "exit") == 0) {
        return 0;
      } else if (strcasecmp(args[0], "cd") == 0) {
        if (num_args >= 2) {
          int result = chdir(args[1]);
          if (result != 0) {
            printf("%s\n", strerror(errno));
          }
        } else {
          set_color(Red);
          printf("Syntax: cd <filepath>\n");
          set_color(White);
        }
      } else {
        int result = fork();
        if (result) {
          //parent
          int status;
          wait(&status);
        } else {
          //child
          execvp(args[0], args);
        }
      }
    }



    getcwd(cwd_buf, PATH_MAX);

    set_color(Green);
    printf("%s: ", username);
    set_color(DarkCyan);
    printf("%s", cwd_buf);
    set_color(White);
    printf("$ ");

    //free_args(args);
  }

  return 0;
}
