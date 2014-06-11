#ifndef STAG_VIEW_H
#define STAG_VIEW_H

// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
// #define _XOPEN_SOURCE_EXTENDED 1
// #include <ncursesw/ncurses.h> // Include wide char version of ncurses
#include <ncurses.h> // Use standard ncurses until wchar actually used

// CONSTANTS
#define SCALE_FIXED_MODE 0
#define SCALE_DYNAMIC_MODE 1
#define SCALE_GLOBAL_MODE 2

typedef struct stag_win {
  int x, y, width, height;
  WINDOW *win;
} stag_win_t;

typedef struct margins {
  int t, r, b, l;
} margins_t;

// Graph window with properties
typedef struct graph {
  stag_win_t *graph_win; // graph window
  stag_win_t *y_win; // y axis window
  char *title;
  margins_t *margins;
  int bar_width;
  int y_splits; // number of intermediate labels on y axis
  int scale_mode; // dynamic, global or fixed
  float scale_min;
  float scale_max;
} graph_t;

void init_stag_win(stag_win_t *win, int height, int width, int y, int x);

void update_y_axis(graph_t *graph);
void draw_title(stag_win_t *title_win, char *title);
void draw_bar(stag_win_t *graph_win, int x, float v, int width, float min, float max);
void draw_graph_axis(stag_win_t *graph_win);

#endif
