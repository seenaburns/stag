// String.h for memset
#include <string.h>

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


void init_values(values_t *values) {
  memset(values->values, 0, HISTORY_SIZE);
  values->max = 0;
  values->size = HISTORY_SIZE;
  values->i = 0;
}

void add_value(values_t *values, float v) {
  // Add new value to values_t
  values->values[values->i] = v;
  values->i += 1;
  values->i %= values->size;
  values->max = find_max(values);
}

float previous_value(values_t *values) {
  // Return value at index i-1 (with wrapping)
  int i = (values->i + values->size -1) % values->size;
  return values->values[i];
}
