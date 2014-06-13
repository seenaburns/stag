#ifndef STAG_DATA_H
#define STAG_DATA_H

#define HISTORY_SIZE 5

// Circular array of values
typedef struct values {
  float *values;
  float max;
  float global_max;
  int size;
  int i;
} values_t;

void init_values(values_t *values, size_t history_size);
void resize_values(values_t *values, size_t history_size);
void add_value(values_t *values, float v);
float previous_value(values_t *values);
void dealloc_values(values_t *values);

#endif
