#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "filestream_handling.h"

char open_redirection_file(d_string name, int next_word_mode) {
  switch(next_word_mode) {
    case 5:
    return open(name.content, O_RDONLY);
    break;
    case 2:
    case 4:
    return open(name.content, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    break;
    case 0:
    case 3:
    return open(name.content, O_WRONLY | O_APPEND | O_CREAT, 0644);
    break;
    case 1:
    return open(name.content, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    break;
  }
  return -1;
}

void apply_redirection(int* redir_info) {
  int i;
  for (i = 0; i < 7; i++) {
    int fd = redir_info[i];
    if (fd != -1) {
      if (i == 5) {
        dup2(fd, STDIN_FILENO);
      } else if (i == 0 || i == 2) {
        dup2(fd, STDERR_FILENO);
      } else if (i == 3 || i == 4) {
        dup2(fd, STDOUT_FILENO);
      } else if (i == 1) {
        dup2(fd, STDERR_FILENO);
        dup2(fd, STDOUT_FILENO);
      }
      close(fd);
    }
  }
}

void save_streams(int* streams) {
  streams[0] = dup(STDIN_FILENO);
  streams[1] = dup(STDOUT_FILENO);
  streams[2] = dup(STDERR_FILENO);
}

void restore_streams(int* streams) {
  dup2(streams[0], STDIN_FILENO);
  dup2(streams[1], STDOUT_FILENO);
  dup2(streams[2], STDERR_FILENO);
  close(streams[0]);
  close(streams[1]);
  close(streams[2]);
}
