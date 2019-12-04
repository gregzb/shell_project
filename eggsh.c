#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include <linux/limits.h>
#include <strings.h>

char* strsep(char** str, char* delimiter) {

  char * original = *str;

  int delimiters = strlen(delimiter);

  int curr = 0;
  while (original[curr] != 0) {
    int j;
    for (j = 0; j < delimiters; j++) {
      char delimiter_char = delimiter[j];
      if (original[curr] == delimiter_char) {
        original[curr] = 0;
        *str = original+curr+1;
        return original;
      }
    }
    curr += 1;
  }
  *str = original+curr+1;
  if (curr == 0) {
    return NULL;
  }
  return original;
}

// char *
// strsep(char **stringp, const char *delim)
// {
// 	char *s;
// 	const char *spanp;
// 	int c, sc;
// 	char *tok;
// 	if ((s = *stringp) == NULL)
// 		return (NULL);
// 	for (tok = s;;) {
// 		c = *s++;
// 		spanp = delim;
// 		do {
// 			if ((sc = *spanp++) == c) {
// 				if (c == 0)
// 					s = NULL;
// 				else
// 					s[-1] = 0;
// 				*stringp = s;
// 				return (tok);
// 			}
// 		} while (sc != 0);
// 	}
// 	/* NOTREACHED */
// }

char ** parse_args( char * line ) {
  size_t str_len = strlen(line)+1;
  int num_args = 8;
  char ** final = calloc(sizeof(char *), num_args);
  char * copy = calloc(sizeof(char), str_len);
  char * copy_original = copy;
  strncpy(copy, line, str_len);
  char * temp = copy;
  int i;
  for (i = 0; (temp = strsep(&copy, " ")) != 0; i++) {
    if (i == num_args - 1) {
      num_args *= 2;
      final = realloc(final, num_args * sizeof(char *));
      int k;
      for (k = num_args / 2; k < num_args; k++) {
        final[k] = 0;
      }
    }
    int mini_len = strlen(temp);
    final[i] = calloc(sizeof(char), mini_len + 1);
    strncpy(final[i], temp, mini_len);
    final[i][mini_len] = 0;
  }
  free(copy_original);
  return final;
}

void free_args(char ** args) {
  size_t num_args = strlen((char *) args);
  int i;
  for (i = 0; i < num_args; i++) {
    free(args[i]);
  }
  free(args);
}

void set_color(unsigned char color) {
  printf("\033[38;5;%hhum", color);
}

enum color {Black, DarkRed, DarkGreen, DarkYellow, DarkBlue, Purple, DarkCyan, LightGrey, DarkGrey, Red, Green, Yellow, Blue, Pink, Cyan, White};

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
