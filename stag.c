// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
#define _XOPEN_SOURCE_EXTENDED 1
// Include wide char version of ncurses
#include <ncursesw/ncurses.h>
// For setlocale to enable ncurses wide char
#include <locale.h>

// stdio for file I/O
#include <stdio.h>

// Settings
#define HISTORY_SIZE 5

// View settings
#define X_MARGIN 1
#define Y_MARGIN 1

void print_values(float *values, int current_i) {
  // Print values to stdout, starting from one after newest (oldest) and
  // circle around to newest
  int i = current_i;
  for(i = current_i; i < current_i + HISTORY_SIZE; i++) {
    fprintf(stdout, "%.1f, ", values[i%HISTORY_SIZE]);
  }
  fprintf(stdout, "\n");
}

typedef struct stag_win {
  int x;
  int y;
  int width;
  int height;
  WINDOW *win;
} stag_win_t;

void init_stag_win(stag_win_t *win, int height, int width, int y, int x) {
  win->win = newwin(height, width, y, x);
  win->x = x;
  win->y = y;
  win->width = width;
  win->height = height;
}

int main(int argc, char **argv) {
  int status = 1;
  int values_i = 0;
  float v;
  float values[HISTORY_SIZE];
  int i;
  
  // Initialize ncurses
  int row, col;
  int ch;
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  getmaxyx(stdscr,row,col);
  halfdelay(5);
  refresh();

  // Y axis
  stag_win_t y_axis_win;
  init_stag_win(&y_axis_win, row-Y_MARGIN*2-2, 2, Y_MARGIN, col-X_MARGIN-2);
  for(i = 0; i < y_axis_win.height; i++) {
    mvwaddch(y_axis_win.win, i, 0, ACS_VLINE);
  }
  wrefresh(y_axis_win.win);
  

  // X axis
  stag_win_t x_axis_win;
  init_stag_win(&x_axis_win, 2, col-X_MARGIN*2-y_axis_win.width, row-Y_MARGIN-2, X_MARGIN);
  for(i = 0; i < x_axis_win.width; i++) {
    mvwaddch(x_axis_win.win, 0, i, ACS_HLINE);
  }
  wrefresh(x_axis_win.win);

  // Read floats to values, circle around after filling buffer 
  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &v);
    if(status == 1) {
      values[values_i] = v;
      values_i = (values_i+1) % HISTORY_SIZE;
      // print_values(values, values_i);
      //fprintf(stdout, "%f\n", v);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
    
    ch = getch();
    if(ch == 'q')
      break;
    refresh();
  }

  endwin();

}
