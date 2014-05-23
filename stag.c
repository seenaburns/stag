// stdio for file I/O
#include <stdio.h>

#define HISTORY_SIZE 5

void print_values(float *values, int current_i) {
  // Print values to stdout, starting from one after newest (oldest) and
  // circle around to newest
  int i = current_i;
  for(i = current_i; i < current_i + HISTORY_SIZE; i++) {
    fprintf(stdout, "%.1f, ", values[i%HISTORY_SIZE]);
  }
  fprintf(stdout, "\n");
}

int main(int argc, char *argv[]) {
  int status = 1;
  int values_i = 0;
  float v;
  float values[HISTORY_SIZE];

  // Read floats to values, circle around after filling buffer 
  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &v);
    if(status == 1) {
      values[values_i] = v;
      values_i = (values_i+1) % HISTORY_SIZE;
      print_values(values, values_i);
      //fprintf(stdout, "%f\n", v);
    } else {
      fprintf(stdout, "Error reading data (%d)\n", status);\
    }
  }
}
