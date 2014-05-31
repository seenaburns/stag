// Define _XOPEN_SOURCE_EXTENDED for wide char functions in ncurses
#define _XOPEN_SOURCE_EXTENDED 1
// For setlocale to enable ncurses wide char
#include <locale.h>
// stdio for file I/O
#include <stdio.h>

// view for ncurses functionality
#include "view.h"
// data for values history
#include "data.h"

int main(int argc, char **argv) {
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
                row-(T_MARGIN+B_MARGIN)-X_AXIS_SIZE-TITLE_HEIGHT,
                Y_AXIS_SIZE,
                T_MARGIN+TITLE_HEIGHT,
                col-R_MARGIN-Y_AXIS_SIZE);
  draw_y_axis(&y_axis_win, 0);
  

  // X axis
  stag_win_t x_axis_win;
  init_stag_win(&x_axis_win,
                X_AXIS_SIZE,
                col-(L_MARGIN+R_MARGIN)-Y_AXIS_SIZE,
                row-B_MARGIN-X_AXIS_SIZE,
                L_MARGIN);
  draw_x_axis(&x_axis_win, 0);

  stag_win_t title_win;
  init_stag_win(&title_win,
                TITLE_HEIGHT,
                col-(L_MARGIN+R_MARGIN),
                T_MARGIN,
                L_MARGIN);
  char title[] = "An example title that could be very long and extend past the length of a single line";
  draw_title(&title_win, title);

  stag_win_t graph_win;
  init_stag_win(&graph_win,
                row-(T_MARGIN+B_MARGIN)-X_AXIS_SIZE-TITLE_HEIGHT,
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

      int i = 0;
      for(i = 0; i<values.size; i++) {
        int j = (values.i+i) % values.size;
        int offset = (values.size - 1) - i;
        draw_bar(&graph_win,
                 graph_win.width-1-offset,
                 values.values[j],
                 values.max);
      }
      mvwprintw(graph_win.win, 2, 10, "%.0f", v);
      mvwprintw(graph_win.win, 3, 10, "%.0f", values.max);
      wrefresh(graph_win.win);
    } else {
      //fprintf(stdout, "Error reading data (%d)\n", status);
    }
  }

  endwin();

}
