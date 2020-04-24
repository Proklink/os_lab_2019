#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"



int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if(seed < 0)
                {
                    printf("seed nust be a nonnegative");
                    return 1;
                }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if(array_size < 0)
                {
                    printf("array_size nust be a nonnegative");
                    return 1;
                }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
             if(pnum < 0)
                {
                    printf("pnum nust be a nonnegative");
                    return 1;
                }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }


  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
  
  printf("(%d): \n ",getpid());
   for (int i = 0; i < array_size; i++) {printf("%d ",array[i]);}

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

   int p[2];
   int range = (array_size)/pnum;

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();

    if (child_pid >= 0) {

      // successful fork
      
      active_child_processes += 1;

      if (child_pid == 0) {
        // child process
        struct MinMax temp;

        printf("\nI (%d) appeared! My parent: %d, i: %d",getpid(),getppid(), i);
        // parallel somehow
        if (i == pnum-1){
            temp = GetMinMax(array, i*range, array_size );
            printf("\niteration - %d:min: %d, max: %d", i,temp.min,temp.max);
        }
        else
            {
                temp = GetMinMax(array, i*range, (i+1)*range-1);
                printf("\niteration - %d:min: %d, max: %d", i,temp.min,temp.max);
            }

        if (with_files) {
          // use files here
            FILE* f;
            if((f = fopen("Pipe","a"))==NULL)
                {
                    printf("Error openning file\n");
                    return 1;
                }
            fprintf(f, "%d",temp.min);
            fprintf(f, "%d",temp.max);
            fclose(f);
        } else {
          // use pipe here
          if(pipe(p) < 0)
          {
              printf("Pipe failed!\n");
              return 1;
          }
          else 
          {
              printf("pipe is working");
              close(p[0]); //close read end
              write(p[1],&temp,sizeof(temp));
          }
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }


  while (active_child_processes > 0) {
    // your code here
    wait(NULL);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  

    FILE* f;
    if((f = fopen("Pipe","a"))==NULL)
    {
        printf("Error openning file\n");
        return 1;
    }
  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
            fscanf(f, "%d",&min_max.min);
            fscanf(f, "%d",&min_max.max);
            
    } else {
      // read from pipes
      close(p[1]);
      read(p[0],&min_max,sizeof(min_max));
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }
    fclose(f);

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("\n(%d) is saying: Min: %d\n",getpid(), min_max.min);
  printf("(%d) is saying: Max: %d\n",getpid(), min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
