// String.h for strlen
#include <string.h>

#include "view.h"

void init_stag_win(stag_win_t *win, int height, int width, int y, int x) {
  win->win = newwin(height, width, y, x);
  win->x = x;
  win->y = y;
  win->width = width;
  win->height = height;
}

void draw_y_axis(stag_win_t *y_axis_win, int splits) {
  int i = 0;
  for(i = 0; i < y_axis_win->height; i++) {
    mvwaddch(y_axis_win->win, i, 0, ACS_VLINE);
  }
  mvwaddch(y_axis_win->win, 0, 0, ACS_LTEE);
  mvwaddch(y_axis_win->win, y_axis_win->height-1, 0, ACS_LTEE);
  wrefresh(y_axis_win->win);
}

void draw_x_axis(stag_win_t *x_axis_win, int splits) {
  int i = 0;
  for(i = 0; i < x_axis_win->width; i++) {
    mvwaddch(x_axis_win->win, 0, i, ACS_HLINE);
  }
  mvwaddch(x_axis_win->win, 0, 0, ACS_TTEE);
  mvwaddch(x_axis_win->win, 0, x_axis_win->width-1, ACS_TTEE);
  wrefresh(x_axis_win->win);
}

void draw_title(stag_win_t *title_win, char *title) {
  int startx = ((title_win->width)-strlen(title))/2;
  mvwaddstr(title_win->win, 0, startx, title);
  wrefresh(title_win->win);
}
