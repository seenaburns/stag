// stdio for file I/O
#include <stdio.h>
// unistd.h for sleep
#include <unistd.h>

#define FREQUENCY 1

int main(int argc, char *argv[]) {
  int status = 1;
  float value;
  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &value);
    if(status == 0)
      fprintf(stdout, "Error reading data (%d)\n", status);
    else
      fprintf(stdout, "%f %d\n", value, status);
    sleep(FREQUENCY);
  }
}
