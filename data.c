#include <string.h> // memset
#include <stdlib.h> // malloc, free

#include "data.h"

float find_max(values_t *values) {
  // Find max value in values_t
  int i = 0;
  float max = -1;
  for(i = 0; i < values->size; i++) {
    if(values->values[i] > max)
      max = values->values[i];
  }

  return max;
}


void init_values(values_t *values, size_t history_size) {
  values->values = malloc(history_size*sizeof(float));
  memset(values->values, 0, history_size*sizeof(float));
  values->max = 0;
  values->global_max = 0;
  values->size = history_size;
  values->i = 0;
}

void add_value(values_t *values, float v) {
  // Add new value to values_t
  values->values[values->i] = v;
  values->i += 1;
  values->i %= values->size;
  float new_max = find_max(values);
  values->max = new_max;
  if(new_max > values->global_max)
    values->global_max = new_max;
}

float previous_value(values_t *values) {
  // Return value at index i-1 (with wrapping)
  int i = (values->i + values->size -1) % values->size;
  return values->values[i];
}

void dealloc_values(values_t *values) {
  free(values->values);
  values->values = NULL;
}
