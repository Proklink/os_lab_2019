#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

//функция  для заполнения массива случайными числами
void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand()%100;
  }
}
