// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
#define _XOPEN_SOURCE_EXTENDED 1

#include <locale.h> // setlocale to enable ncurses wide char
#include <stdio.h> // file I/O
#include <getopt.h> // argument parsing (getopt_long)
#include <string.h> // strncpy
#include <stdlib.h> // atoi

#include "view.h" // ncurses functionality
#include "data.h" // values history

typedef struct margins {
  int t, r, b, l;
} margins_t;

void init_margins_from_string(margins_t *margins, char *s) {
  // String of format t,r,b,l
  margins->t = atoi(strsep(&s, ","));
  margins->r = atoi(strsep(&s, ","));
  margins->b = atoi(strsep(&s, ","));
  margins->l = atoi(strsep(&s, ","));
}

int main(int argc, char **argv) {
  char title[256] = "stag";
  char margin_s[100] = "1,1,0,1"; //t,r,b,l as in css
  int scale = -1;

  char opt;
  struct option long_options[] =
    {
      {"title", required_argument, 0, 't'}, // Graph title
      {"margin", required_argument, 0, 'm'}, // Window margins, t,r,b,l
      {"scale", required_argument, 0, 's'}, // max y value
      {0,0,0,0}
    };
  int option_index = 0;
  while((opt = getopt_long(argc, argv, "t:m:s:", long_options, &option_index)) != -1) {
    switch (opt) {
      case 't':
        strncpy(title, optarg, 255);
        break;

      case 'm':
        strncpy(margin_s, optarg, 99);
        break;

      case 's':
        scale = atoi(optarg);
        break;

      default:
        break;
    }
  }
  margins_t margins;
  init_margins_from_string(&margins, margin_s);

  int status = 1;
  
  // Initialize ncurses
  int row, col;
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  getmaxyx(stdscr,row,col);
  halfdelay(5);
  refresh();

  // Y axis
  stag_win_t y_axis_win;
  init_stag_win(&y_axis_win,
                row-(margins.t+margins.b)-TITLE_HEIGHT,
                Y_AXIS_SIZE,
                margins.t+TITLE_HEIGHT,
                col-margins.r-Y_AXIS_SIZE);
  draw_y_axis(&y_axis_win, 0);
  
  stag_win_t title_win;
  init_stag_win(&title_win,
                TITLE_HEIGHT,
                col-(margins.l+margins.r),
                margins.t,
                margins.l);
  draw_title(&title_win, title);

  stag_win_t graph_win;
  init_stag_win(&graph_win,
                row-(margins.t+margins.b)-TITLE_HEIGHT,
                col-(margins.l+margins.r)-Y_AXIS_SIZE,
                margins.t+TITLE_HEIGHT,
                margins.l);
  wrefresh(graph_win.win);

  // Read floats to values, circle around after filling buffer 
  float v;
  values_t values;
  init_values(&values);

  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &v);
    if(status == 1) {
      add_value(&values, v);

      // Redraw graph
      wclear(graph_win.win);
      wrefresh(graph_win.win);

      draw_graph_axis(&graph_win);

      int barmax = values.max;
      if(scale >= 0)
        barmax = scale;

      int i = 0;
      for(i = 0; i<values.size; i++) {
        int j = (values.i+i) % values.size;
        int offset = (values.size - 1) - i;
        draw_bar(&graph_win,
                 graph_win.width-1-offset,
                 values.values[j],
                 barmax);
      }
      wrefresh(graph_win.win);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
  }

  endwin();

}
