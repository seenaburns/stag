// stdio for file I/O
#include <stdio.h>

int main(int argc, char *argv[]) {
  int status = 1;
  float value;

  while(status != EOF) {
    status = fscanf(stdin, "%f\n", &value);
    if(status == 1)
      fprintf(stdout, "%f\n", value);
    else
      fprintf(stdout, "Error reading data (%d)\n", status);
  }
}
