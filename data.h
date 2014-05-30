#ifndef STAG_DATA_H
#define STAG_DATA_H

#define HISTORY_SIZE 5

// Circular array of values
typedef struct values {
  float values[HISTORY_SIZE];
  float max;
  int size;
  int i;
} values_t;

void init_values(values_t *values);
void add_value(values_t *values, float v);

#endif
