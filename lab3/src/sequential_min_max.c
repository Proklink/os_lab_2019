#include <stdio.h>
#include <stdlib.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
    //main должна принимать 2 аргумента
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

//первый аргумент это целове число, необходимое для рандомизации псевдо-случайных чисел
  int seed = atoi(argv[1]);
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }

//второй аргумент это размер массива
  int array_size = atoi(argv[2]);
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

//создание массива
  int *array = malloc(array_size * sizeof(int));
  GenerateArray(array, array_size, seed);//заполнение массива случайными числами
  struct MinMax min_max = GetMinMax(array, 0, array_size);//получение минмального и максимального элемента массива
  free(array);//освобождение памяти

//печать результата
  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);

  return 0;
}
