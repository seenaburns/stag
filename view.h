#ifndef STAG_VIEW_H
#define STAG_VIEW_H

#include <ncurses.h>

// SETTINGS
#define Y_AXIS_SIZE 6
#define TITLE_HEIGHT 2

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
  stag_win_t *title_win; // title window
  int cols;
  int lines;
  char *title;
  margins_t *margins;
  int bar_width;
  int y_splits; // number of intermediate labels on y axis
  int scale_mode; // dynamic, global or fixed
  float scale_min;
  float scale_max;
} graph_t;

void initialize_windows(graph_t *graph);
void init_stag_win(stag_win_t *win, int height, int width, int y, int x);
void init_title_win(graph_t *graph);
void init_yaxis_win(graph_t *graph);
void init_graph_win(graph_t *graph);

void draw_y_axis(graph_t *graph);
void draw_title(stag_win_t *title_win, char *title);
void draw_bar(graph_t *graph, float v, int age);
void draw_graph_axis(stag_win_t *graph_win);

#endif
