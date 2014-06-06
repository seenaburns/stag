#ifndef STAG_VIEW_H
#define STAG_VIEW_H

// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
// #define _XOPEN_SOURCE_EXTENDED 1
// #include <ncursesw/ncurses.h> // Include wide char version of ncurses
#include <ncurses.h> // Use standard ncurses until wchar actually used

typedef struct stag_win {
  int x;
  int y;
  int width;
  int height;
  WINDOW *win;
} stag_win_t;

void init_stag_win(stag_win_t *win, int height, int width, int y, int x);
void draw_y_axis(stag_win_t *y_axis_win, float min, float max);
void draw_title(stag_win_t *title_win, char *title);
void draw_bar(stag_win_t *graph_win, int x, float v, float max);
void draw_graph_axis(stag_win_t *graph_win);

#endif
