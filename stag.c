// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
// #define _XOPEN_SOURCE_EXTENDED 1

// #include <locale.h> // setlocale to enable ncurses wide char
#include <stdio.h> // file I/O, fprintf, sprintf
#include <getopt.h> // argument parsing (getopt_long)
#include <string.h> // strncpy
#include <stdlib.h> // atoi, malloc, free

#include "view.h" // ncurses functionality
#include "data.h" // values history

// Settings
#define DEFAULT_T_MARGIN 1
#define DEFAULT_L_MARGIN 1
#define DEFAULT_R_MARGIN 1
#define DEFAULT_B_MARGIN 1

// Internal settings
#define Y_AXIS_SIZE 6
#define TITLE_HEIGHT 2
#define MAX_TITLE_LENGTH 256
#define MAX_MARGINS_LENGTH 30

// Constants
#define SCALE_DYNAMIC_MODE -1
#define SCALE_GLOBAL_MODE -2

// margins options struct
typedef struct margins {
  int t, r, b, l;
} margins_t;

int main(int argc, char **argv) {
  int status = 1;

  // Options for getopt_long
  struct option long_options[] =
  {
    {"title", required_argument, 0, 't'}, // Graph title
    {"margin", required_argument, 0, 'm'}, // Window margins, t,r,b,l
    {"scale", required_argument, 0, 's'}, // max y value
    {0,0,0,0}
  };

  char opt;
  int option_index = 0;

  // Set option defaults
  char title[MAX_TITLE_LENGTH] = "stag";  
  char margin_s[MAX_MARGINS_LENGTH];
  sprintf(margin_s, "%d,%d,%d,%d", DEFAULT_T_MARGIN,
          DEFAULT_R_MARGIN, DEFAULT_B_MARGIN, DEFAULT_L_MARGIN);
  int scale = SCALE_DYNAMIC_MODE;
  
  while((opt = getopt_long(argc, argv, "t:m:s:", long_options, &option_index)) != -1) {
    switch (opt) {
      case 't':
        strncpy(title, optarg, MAX_TITLE_LENGTH-1);
        break;

      case 'm':
        strncpy(margin_s, optarg, MAX_MARGINS_LENGTH-1);
        break;

      case 's':
        // Accept dynamic, global as options
        if(!strcmp(optarg, "dynamic"))
          scale = SCALE_DYNAMIC_MODE;
        else if(!strcmp(optarg, "global"))
          scale = SCALE_GLOBAL_MODE;
        else
          scale = atoi(optarg);
        break;

      default:
        break;
    }
  }
  // Create settings structs
  margins_t margins;
  char *ms = &margin_s[0];
  margins.t = atoi(strsep(&ms, ","));
  margins.r = atoi(strsep(&ms, ","));
  margins.b = atoi(strsep(&ms, ","));
  margins.l = atoi(strsep(&ms, ","));

  // Initialize ncurses
  int row, col;
  // setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
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
  draw_y_axis(&y_axis_win, 0, 0);
  
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
  draw_graph_axis(&graph_win);
  wrefresh(graph_win.win);

  // Read floats to values, circle around after filling buffer 
  float v;
  values_t values;
  init_values(&values, graph_win.width);

  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &v);
    if(status == 1) {
      add_value(&values, v);

      // Redraw graph
      wclear(graph_win.win);
      wrefresh(graph_win.win);

      draw_graph_axis(&graph_win);

      // Determine scale value
      int barmax = scale;
      if(scale == SCALE_DYNAMIC_MODE)
        barmax = values.max;
      else if(scale == SCALE_GLOBAL_MODE)
        barmax = values.global_max;
      else if(scale < SCALE_GLOBAL_MODE)
        barmax = 0;

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

      // Update y axis values
      draw_y_axis(&y_axis_win, 0, barmax);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
  }

  endwin();

  dealloc_values(&values);
}
