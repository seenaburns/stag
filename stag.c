#include <stdio.h> // file I/O, fprintf, sprintf
#include <getopt.h> // argument parsing (getopt_long)
#include <string.h> // strncpy
#include <stdlib.h> // atoi, malloc, free
#include <signal.h> // signals for resizing

#include "view.h" // ncurses functionality
#include "data.h" // values history


// Internal settings
#define Y_AXIS_SIZE 6
#define MAX_TITLE_LENGTH 256
#define MAX_MARGINS_LENGTH 30
#define MAX_SPLITS 15

// Default Settings
#define DEFAULT_T_MARGIN 1
#define DEFAULT_R_MARGIN 1
#define DEFAULT_L_MARGIN Y_AXIS_SIZE+DEFAULT_R_MARGIN-2
#define DEFAULT_B_MARGIN 1

// Default usage value
const char stag_usage_string[] =
"\n"
"Usage: stag  [-t  TITLE]  [-m  MARGIN]  [-s  SCALE_MODE]  [-w  BAR_WIDTH]  [-y Y_SPLITS]\n"
"\n"
"Options:\n"
"       -t, --title=TITLE\n"
"              Set the title displayed above the graph.\n"
"\n"
"       -m, --margin=T,R,B,L\n"
"              Set the margin around the edge of the graph, defined as a comma \n"
"              delimited list of 4 integers: top,right,bottom,left.\n"
"\n"
"       -s, --scale=SCALE_MODE\n"
"              Choose how to scale the y-axis of the graph.\n"
"              dynamic: update the max using the values currently on the graph\n"
"              global: update the max to be the global maximum of all data seen\n"
"              min,max: specify the max and min using two integers\n"
"\n"
"       -w, --width=BAR_WIDTH\n"
"              Set the width of the bars.\n"
"\n"
"       -y, --split=Y_SPLITS\n"
"              Set the number of intermediate y-axis ticks between the min and max.\n"
"\n";

// Display usage info
void usage(){
  printf(stag_usage_string);
  exit(0);
}

sig_atomic_t resized = 0;
void handle_winch(int sig) {
  // Handle ncurses resizing signal
  resized = 1;
}

int main(int argc, char **argv) {
  if (argc > 1 &&
      (strcmp(argv[1], "--help") == 0 ||
       strcmp(argv[1], "-h") == 0))
      usage();

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
        if(sscanf(optarg,
                  "%d,%d,%d,%d",
                  &graph.margins->t,
                  &graph.margins->r,
                  &graph.margins->b,
                  &graph.margins->l) != 4) {
          printf("%s not recognized as input to --margin. See --help\n", optarg);
          exit(1);
        }
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
  // setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  halfdelay(5);
  refresh();

  signal(SIGWINCH, handle_winch);

  graph.cols = COLS;
  graph.lines = LINES;
  
  // Initialize windows
  stag_win_t title_win;
  title_win.win = NULL;
  graph.title_win = &title_win;

  stag_win_t y_axis_win;
  y_axis_win.win = NULL;
  graph.y_win = &y_axis_win;

  stag_win_t graph_win;
  graph_win.win = NULL;
  graph.graph_win = &graph_win;

  initialize_windows(&graph);

  // Draw title
  draw_title(&title_win, title);
  wrefresh(title_win.win);
  
  // Draw y a_xis
  draw_y_axis(&graph);
  wrefresh(graph.y_win->win);
  
  // Draw graph x axis
  draw_graph_axis(&graph_win);
  wrefresh(graph.graph_win->win);


  // Read floats to values, circle around after filling buffer 
  // Initialize values
  float v;
  values_t values;
  init_values(&values, graph_win.width);

  // Don't exit loop even on error, just display
  while(1) {

    if(resized) {
      // Reinitialize ncurses with refresh to get new dimensions
      endwin();
      refresh(); 
      // mvprintw(0, 1, "Resized %dx%d", COLS, LINES);
      
      // If a resize signal occured, reset windows
      graph.cols = COLS;
      graph.lines = LINES;
      initialize_windows(&graph);

      // Erase to handle areas that are now in margins
      clear();
      refresh();

      // Redraw title
      draw_title(&title_win, title);
      wrefresh(title_win.win);

      // Resize values
      resize_values(&values, graph.graph_win->width);

      resized = 0;
    }
    
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
      werase(graph.graph_win->win);
      wnoutrefresh(graph.graph_win->win);

      draw_graph_axis(&graph_win);

      int i = 0;
      for(i = 0; i<values.size; i++) {
        int j = (values.i+i) % values.size;
        int age = values.size - i;
        draw_bar(&graph,
                 values.values[j],
                 age);
      }
      wnoutrefresh(graph.graph_win->win);

      // Redraw y_axis
      werase(graph.y_win->win);
      draw_y_axis(&graph);
      wnoutrefresh(graph.y_win->win);

      doupdate();
    } else if (!feof(stdin)) {
      // fscanf can return EOF on read / encoding error or EOF
      // only print error if not from stdin having an EOF
      mvprintw(0, 0, "Error reading data (%d)\n", status);
      refresh();
    }
  }

  endwin();

  dealloc_values(&values);
}

