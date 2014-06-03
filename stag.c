// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
#define _XOPEN_SOURCE_EXTENDED 1
// For setlocale to enable ncurses wide char
#include <locale.h>
// stdio for file I/O
#include <stdio.h>
// getopt for argument parsing (getopt_long)
#include <getopt.h>
// string for strncpy
#include <string.h>

// view for ncurses functionality
#include "view.h"
// data for values history
#include "data.h"

int main(int argc, char **argv) {
  char title[256] = "stag";

  char opt;
  struct option long_options[] =
    {
      {"title", required_argument, 0, 't'},
      {0,0,0,0}
    };
  int option_index = 0;
  while((opt = getopt_long(argc, argv, "t:", long_options, &option_index)) != -1) {
    switch (opt) {
      case 't':
        strncpy(title, optarg, 255);
        break;

      default:
        break;
    }
  }

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
                row-(T_MARGIN+B_MARGIN)-TITLE_HEIGHT,
                Y_AXIS_SIZE,
                T_MARGIN+TITLE_HEIGHT,
                col-R_MARGIN-Y_AXIS_SIZE);
  draw_y_axis(&y_axis_win, 0);
  
  stag_win_t title_win;
  init_stag_win(&title_win,
                TITLE_HEIGHT,
                col-(L_MARGIN+R_MARGIN),
                T_MARGIN,
                L_MARGIN);
  draw_title(&title_win, title);

  stag_win_t graph_win;
  init_stag_win(&graph_win,
                row-(T_MARGIN+B_MARGIN)-TITLE_HEIGHT,
                col-(L_MARGIN+R_MARGIN)-Y_AXIS_SIZE,
                T_MARGIN+TITLE_HEIGHT,
                L_MARGIN);
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

      int i = 0;
      for(i = 0; i<values.size; i++) {
        int j = (values.i+i) % values.size;
        int offset = (values.size - 1) - i;
        draw_bar(&graph_win,
                 graph_win.width-1-offset,
                 values.values[j],
                 values.max);
      }
      wrefresh(graph_win.win);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
  }

  endwin();

}
