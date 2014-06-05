#ifndef STAG_VIEW_H
#define STAG_VIEW_H

// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
// #define _XOPEN_SOURCE_EXTENDED 1
// #include <ncursesw/ncurses.h> // Include wide char version of ncurses
#include <ncurses.h> // Use standard ncurses until wchar actually used

// View settings
// Margins: Top, Left, Right, Bottom
#define T_MARGIN 1
#define L_MARGIN 1
#define R_MARGIN 1
#define B_MARGIN 1

// Y Axis size
#define Y_AXIS_SIZE 2

// Title padding
#define TITLE_HEIGHT 2


typedef struct stag_win {
  int x;
  int y;
  int width;
  int height;
  WINDOW *win;
} stag_win_t;

void init_stag_win(stag_win_t *win, int height, int width, int y, int x);
void draw_y_axis(stag_win_t *y_axis_win, int splits);
void draw_title(stag_win_t *title_win, char *title);
void draw_bar(stag_win_t *graph_win, int x, float v, float max);
void draw_graph_axis(stag_win_t *graph_win);

#endif
