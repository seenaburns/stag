// String.h for strlen
#include <string.h>
// wchar for utf-8
#include <wchar.h>
// math for ceil
#include <math.h>

#include "view.h"

void mvwadd_wchar(WINDOW *win, int y, int x, const int c) {
  // wchar mvwaddch, must convert to cchar_t for ncursesw
  cchar_t cc = {
    0,    // attribute
    {c,0} // cchar
  };
  mvwadd_wch(win, y, x, &cc);
}

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

int centered_x(stag_win_t *win, char *s) {
  int x = ((win->width)-strlen(s))/2;
  if(x < 0)
    return 0;

  return x;
}

void draw_title(stag_win_t *title_win, char *title) {
  int i = 0;
  int title_i = 0;
  int title_len = strlen(title);

  for(i = 0; i<title_win->height && title_i < title_len; i++) {
    char partial_title[title_win->width];
    strncpy(partial_title, title+title_i, title_win->width);
    int startx = centered_x(title_win, partial_title);
    mvwaddnstr(title_win->win, i, startx, partial_title, title_win->width);
    title_i += title_win->width;
  }
  wrefresh(title_win->win);
}

void draw_bar(stag_win_t *graph_win, int x, float v, float max) {
  // No widechars for now
  // wchar_t upper_half_block = L'\u2584';
  // wchar_t lower_half_block = L'\u2584';
  // wchar_t full_block = L'\u2589';

  if(max <= 0)
    return;

  float height =  ceil(v/max * graph_win->height);

  // First block is half block
  float i = 0; 
  wattron(graph_win->win, A_REVERSE);
  for(i = 0; i < height; i++) {
    mvwaddch(graph_win->win, graph_win->height-1-i, x, ' ');
  }
  wattroff(graph_win->win,A_REVERSE);
 
  wattron(graph_win->win, A_UNDERLINE);
  mvwaddch(graph_win->win, graph_win->height-1, x-1, ' ');
  wattroff(graph_win->win, A_UNDERLINE);

  wrefresh(graph_win->win);  
  move(0,0);
}
