#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  // your code here
  
for(int* p = array+ begin; p< array + end; p++){
  if(*p< min_max.min)
     min_max.min = *p;
  if(*p > min_max.max)
    min_max.max = *p;
  }
  return min_max;
}
