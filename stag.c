// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
// #define _XOPEN_SOURCE_EXTENDED 1

// #include <locale.h> // setlocale to enable ncurses wide char
#include <stdio.h> // file I/O, fprintf, sprintf
#include <getopt.h> // argument parsing (getopt_long)
#include <string.h> // strncpy
#include <stdlib.h> // atoi, malloc, free
#include <signal.h> // signals for resizing

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
#define MAX_SPLITS 15

// sig_atomic_t resized = 0;
void handle_winch(int sig) {
  // Handle ncurses resizing signal
  endwin();
  refresh();
  clear();
  mvprintw(0,0, "Resized window %dx%d", LINES, COLS);
  refresh();
}

int main(int argc, char **argv) {
  int status = 1;

  // Initialize graph properties with defaults
  graph_t graph;
  margins_t margins;
  char title[MAX_TITLE_LENGTH] = "stag";  

  graph.title = title;
  
  graph.margins = &margins;
  graph.margins->t = DEFAULT_T_MARGIN;
  graph.margins->r = DEFAULT_R_MARGIN;
  graph.margins->b = DEFAULT_B_MARGIN;
  graph.margins->l = DEFAULT_L_MARGIN;

  graph.y_splits = 0;
  graph.bar_width = 1;
  graph.scale_mode = SCALE_DYNAMIC_MODE;
  graph.scale_min = 0;
  graph.scale_max = 0;
  

  // Read options
  char opt;
  int option_index = 0;
  struct option long_options[] =
  {
    {"title", required_argument, 0, 't'}, // Graph title
    {"margin", required_argument, 0, 'm'}, // Window margins, t,r,b,l
    {"scale", required_argument, 0, 's'}, // max y value
    {"width", required_argument, 0, 'w'}, // bar width
    {"split", required_argument, 0, 'y'}, // y axis splits
    {0,0,0,0}
  };

  while((opt = getopt_long(argc, argv, "t:m:s:w:y:", long_options, &option_index)) != -1) {
    switch (opt) {
      case 't':
        // Title
        strncpy(title, optarg, MAX_TITLE_LENGTH-1);
        break;

      case 'm':
        // Margin
        graph.margins->t = atoi(strsep(&optarg, ","));
        graph.margins->r = atoi(strsep(&optarg, ","));
        graph.margins->b = atoi(strsep(&optarg, ","));
        graph.margins->l = atoi(strsep(&optarg, ","));
        break;

      case 's':
        // Scale (for y axis)
        // Accept dynamic, global as options
        if(!strcmp(optarg, "dynamic")) {
          graph.scale_mode = SCALE_DYNAMIC_MODE;
        } else if(!strcmp(optarg, "global")) {
          graph.scale_mode = SCALE_GLOBAL_MODE;
        } else if(strchr(optarg, ',')){
          graph.scale_mode = SCALE_FIXED_MODE;
          graph.scale_min = atoi(strsep(&optarg, ","));
          graph.scale_max = atoi(strsep(&optarg, ","));
        } else {
          printf("%s not recognized as input to --scale. See --help\n", optarg);
          exit(1);
        }
        break;
        
      case 'w':
        graph.bar_width = atoi(optarg);
        if(graph.bar_width < 1)
          graph.bar_width = 1;
        break;

      case 'y':
        // Y axis split
        graph.y_splits = atoi(optarg);
        if(graph.y_splits < 0)
          graph.y_splits = 0;
        else if(graph.y_splits > MAX_SPLITS)
          graph.y_splits = MAX_SPLITS;
        break;

      default:
        break;
    }
  }
  
  // Initialize ncurses
  int row, col;
  // setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  getmaxyx(stdscr,row,col);
  halfdelay(5);
  refresh();

  signal(SIGWINCH, handle_winch);

  // Title
  stag_win_t title_win;
  init_stag_win(&title_win,
                TITLE_HEIGHT,
                col-(margins.l+margins.r),
                margins.t,
                margins.l);
  draw_title(&title_win, title);
  wrefresh(title_win.win);
  
  // Y axis
  stag_win_t y_axis_win;
  init_stag_win(&y_axis_win,
                row-(margins.t+margins.b)-TITLE_HEIGHT,
                Y_AXIS_SIZE,
                margins.t+TITLE_HEIGHT,
                col-margins.r-Y_AXIS_SIZE);
  graph.y_win = &y_axis_win;
  draw_y_axis(&graph);
  wrefresh(graph.y_win->win);
  
  // Graph content window
  stag_win_t graph_win;
  init_stag_win(&graph_win,
                row-(margins.t+margins.b)-TITLE_HEIGHT,
                col-(margins.l+margins.r)-Y_AXIS_SIZE,
                margins.t+TITLE_HEIGHT,
                margins.l);
  graph.graph_win = &graph_win;
  draw_graph_axis(&graph_win);
  wrefresh(graph.graph_win->win);

  // Read floats to values, circle around after filling buffer 
  float v;
  values_t values;
  init_values(&values, graph_win.width);

  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &v);
    if(status == 1) {
      // Update values and axis scale
      add_value(&values, v);

      if(graph.scale_mode == SCALE_DYNAMIC_MODE)
        graph.scale_max = values.max;
      else if(graph.scale_mode == SCALE_GLOBAL_MODE)
        graph.scale_max = values.global_max;
      else if(graph.scale_max <= 0 || graph.scale_max < graph.scale_min)
        graph.scale_max = graph.scale_min;


      // Update graph
      wclear(graph.graph_win->win);
      wrefresh(graph.graph_win->win);

      draw_graph_axis(&graph_win);

      int i = 0;
      for(i = 0; i<values.size; i++) {
        int j = (values.i+i) % values.size;
        int age = values.size - i;
        draw_bar(&graph,
                 values.values[j],
                 age);
      }
      wrefresh(graph.graph_win->win);

      // Redraw y_axis
      wclear(graph.y_win->win);
      wrefresh(graph.y_win->win);
      draw_y_axis(&graph);
      wrefresh(graph.y_win->win);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
  }

  endwin();

  dealloc_values(&values);
}
