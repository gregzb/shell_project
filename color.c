#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void set_color(unsigned char color) {
  printf("\033[38;5;%hhum", color);
}

enum color {Black, DarkRed, DarkGreen, DarkYellow, DarkBlue, Purple, DarkCyan, LightGrey, DarkGrey, Red, Green, Yellow, Blue, Pink, Cyan, White};
