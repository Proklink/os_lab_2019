#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>

#include "ex1/utils.h"
#include <pthread.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;

    for (int i = args->begin; i < args->end; i++) {
        sum += args->array[i];
    }

    return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
    static struct option options[] = {{"seed",        required_argument, 0, 0},
                                        {"array_size",  required_argument, 0, 0},
                                        {"threads_num", required_argument, 0, 0},
                                        {0, 0,                             0, 0}};


    int seed = -1;
    int array_size = -1;
    int threads_num = -1;

    while (true) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0: {
                        seed = atoi(optarg);
                        if (seed < 0) {
                            printf("Seed must be a positive number or 0. Now seed is %d\n", seed);
                            return -1;
                        }
                        break;
                    }

                    case 1:
                        array_size = atoi(optarg);
                        if (array_size < 0) {
                            printf("Array_size must be a positive number. Now array_size is %d\n", array_size);
                            return -1;
                        }
                        break;

                    case 2: {
                        threads_num = atoi(optarg);
                        if (threads_num <= 0) {
                            printf("Threads_num must be 1 or more. Now threads_num is %d\n", threads_num);
                            return -1;
                        }
                         
                        break;
                    }
                }
                break;
            }

            case '?':
                break;

            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }
   
    if (threads_num <= 0) {
        printf("Threads_num must be 1 or more. Now threads_num is %d\n", threads_num);
        return -1;
    }
 
    printf("\nseed: %d, array_size: %d, threads_num: %d",seed, array_size, threads_num);
   
    pthread_t threads[threads_num];
    
    int *array = malloc(sizeof(int) * array_size);
    
    GenerateArray(array, array_size, seed);
    struct SumArgs args[threads_num];

    for(int i = 0; i < threads_num; i++)
    {
        args[i].array = array;
        args[i].begin = i * array_size/threads_num;
        if (i == threads_num - 1 )
            args[i].end = array_size;
        else
            args[i].end = (i + 1) * array_size/threads_num;
        
    }

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)(args+i))) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }
  struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("It took %f milliseconds\n", elapsed_time);
  return 0;
}
