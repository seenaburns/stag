#include <string.h> // strlen
#include <stdio.h> // sprintf
#include <math.h> // ceil

#include "view.h"

void init_stag_win(stag_win_t *win, int height, int width, int y, int x) {
  // Delete if not null
  if(win->win != NULL)
    delwin(win->win);
  // Initialize window struct
  win->win = newwin(height, width, y, x);
  win->x = x;
  win->y = y;
  win->width = width;
  win->height = height;
}


void init_title_win(graph_t *graph) {
  margins_t margins = *(graph->margins);
  init_stag_win(graph->title_win,
                TITLE_HEIGHT,
                graph->cols - (margins.l+margins.r) - Y_AXIS_SIZE,
                margins.t,
                margins.l);
}

void init_yaxis_win(graph_t *graph) {
  margins_t margins = *(graph->margins);
  init_stag_win(graph->y_win,
                graph->lines - (margins.t+margins.b) - TITLE_HEIGHT,
                Y_AXIS_SIZE,
                margins.t + TITLE_HEIGHT,
                graph->cols - margins.r - Y_AXIS_SIZE);
}

void init_graph_win(graph_t *graph) {
  margins_t margins = *(graph->margins);
  init_stag_win(graph->graph_win,
                graph->lines - (margins.t+margins.b) - TITLE_HEIGHT,
                graph->cols - (margins.l+margins.r) - Y_AXIS_SIZE,
                margins.t + TITLE_HEIGHT,
                margins.l);
}

void initialize_windows(graph_t *graph) {
  init_title_win(graph);
  init_yaxis_win(graph);
  init_graph_win(graph);
}

void format_axis_value(char *dest, float v) {
  // Populate dest with formatted value, e.g. 1000->1k, 1000000->1M
  char unitprefixes[9] = " KMGTPEZY";
  float remainder = v;

  // Special case for v < 1 and 0
  if(v < 0.0001) {
    sprintf(dest, "0");
    return;
  } else if(v < 1) {
    sprintf(dest, ".%.3f", v);
    return;
  }
  
  int i = 0;
  for(i = 0; i < 9 && remainder >= 1000; i++)
    remainder /= 1000;

  sprintf(dest, "%-.0f%c", remainder, unitprefixes[i]);
}

void draw_y_axis(graph_t *graph) {
  if (graph->y_win->width <= 0)
      return;

  // Extract values from graph
  stag_win_t *y_win = graph->y_win;
  int splits = graph->y_splits;
  int min = graph->scale_min;
  int max = graph->scale_max;

  // Draw axis line
  int i = 0;
  for(i = 0; i < y_win->height; i++)
    mvwaddch(y_win->win, i, 0, ACS_VLINE);

  // Draw axis values
  char axis_label[5];
  // Max
  mvwaddch(y_win->win, 0, 0, ACS_LTEE);
  format_axis_value(&axis_label[0], max);
  mvwprintw(y_win->win, 0, 2, "%s", axis_label);
  // Min
  mvwaddch(y_win->win, y_win->height-1, 0, ACS_LTEE);
  format_axis_value(&axis_label[0], min);
  mvwprintw(y_win->win, y_win->height-1, 2, "%s", axis_label);

  // Draw split labels
  // Only consider porition of y_win other than max/min labels (height - 2)
  // 1. Value accuracy at display height (value from display pos not on i)
  // 2. Evenly distribute labels
  int height = y_win->height-2;
  float v_step = (float)(max-min)/(height+1); // + 1 to not reach max at height
  float h_step = (float)height/splits;
  for(i = 0; i < splits; i++) {
    int offset = (height - (int)floor((splits-1) * h_step))/2;
    int h = (int)floor(i*h_step) + offset;
    int v = floor((height - h) * v_step + min);
    
    mvwaddch(y_win->win, h+1, 0, ACS_LTEE);
    format_axis_value(&axis_label[0], v);
    mvwprintw(y_win->win, h+1, 2, "%s", axis_label);
  }
}

int centered_x(stag_win_t *win, char *s) {
  // Return starting x to place text as centered
  int x = ((win->width)-strlen(s))/2;
  if(x < 0)
    return 0;

  return x;
}

void draw_title(stag_win_t *title_win, char *title) {
  if (title_win->width <= 0)
      return;

  // Draw title to window, centered
  char partial_title[title_win->width];
  strncpy(partial_title, title, title_win->width);

  // If title too long for window, add ...
  if ((unsigned long) title_win->width < strlen(title)) {
    partial_title[title_win->width-1] = '.';
    partial_title[title_win->width-2] = '.';
    partial_title[title_win->width-3] = '.';
  }

  int startx = centered_x(title_win, partial_title);
  mvwaddnstr(title_win->win, 0, startx, partial_title, title_win->width);
}

void draw_bar(graph_t *graph, float v, int age) {
  // Draw bar for the value v on graph, oldest at far right
  if (graph->graph_win->width <= 0)
      return;

  // Extract values
  stag_win_t *graph_win = graph->graph_win;
  float min = graph->scale_min;
  float max = graph->scale_max;  
  int x = graph_win->width-age*graph->bar_width;
  
  // Fail conditions
  if(max-min == 0)
    return;
  if(x+graph->bar_width < 0)
    return;

  float height =  round((v-min)/(max-min) * graph_win->height);

  int j = 0;
  float i = 0;
  wattron(graph_win->win, A_REVERSE);
  for(j = 0; j < graph->bar_width; j++) {
    for(i = 0; i < height; i++)
      mvwaddch(graph_win->win, graph_win->height-1-i, x+j, ' ');
  }
  wattroff(graph_win->win,A_REVERSE);
}

void draw_graph_axis(stag_win_t *graph_win) {
  if (graph_win->width <= 0)
      return;

  // Draw underline for the x axis
  int i = 0;
  wattron(graph_win->win, A_UNDERLINE);
  for(i = 0; i < graph_win->width; i++)
    mvwaddch(graph_win->win, graph_win->height-1, i, ' ');
  wattroff(graph_win->win, A_UNDERLINE);
}
