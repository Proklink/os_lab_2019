#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

bool kill_flag = false;

void handle_alarm(int sig)
{
    kill_flag = true;
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum =-1;
  bool with_files = false;
  int timeout = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
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
            if(seed<=0)
            {
                printf("seed must be a positive");
                return 1;
            }

            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
             if(array_size<=0)
            {
                printf("array_size must be a positive");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
             if(pnum<=0)
            {
                printf("pnum must be a positive");
                return 1;
            }
            break;
          case 3:
            timeout = atoi(optarg);

             if(timeout <= 0)
            {
                printf("timeout must be a positive");
                return 1;
            }
            break;
          case 4:
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

  if (seed == -1 || array_size == -1 || pnum == -1 ) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }
    
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

    FILE* pf;
    int* p = (int*)calloc(2,sizeof(int));
     
    if(with_files)
    {
        if((pf=fopen("PIPE.txt","w"))==NULL)
        {
            printf("\nopenning file failed");
            return 1;
        }
    }
    else
    {
        if(pipe(p)<0)
            {   printf("\npipe is failed");
                return 1;
            }
    }

    int range = array_size/pnum;
    
    if (timeout != -1){
        signal( SIGALRM, handle_alarm );
        alarm( timeout );
    }

    pid_t children[pnum];

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
          struct MinMax temp;

        // child process
        if (i != pnum-1){
            temp = GetMinMax(array, i*range, (i+1)*range - 1);

        }
        else{
            temp = GetMinMax(array, i*range, array_size-1);
        }
        // parallel somehow
        
        if (with_files) {
          // use files here
          
          fprintf(pf, "%d ",temp.min);
            fprintf(pf, "%d\n",temp.max);
            fclose(pf);
        } else {
          // use pipe here
            
            close(p[0]); //close read end
            write(p[1],&temp.max,sizeof(int));
            write(p[1],&temp.min,sizeof(int));
            close(p[1]);
            
        }
        return 0;
      }
      if (child_pid > 0)
        children[i] = child_pid;
        

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
    if (with_files){
        fclose(pf);
        if((pf=fopen("PIPE.txt","r"))==NULL)
        {
             
            printf("\nopenning file failed");
            return 1;
        
        }
    }
    int status;
  while (active_child_processes > 0) {
    // your code here
    if (kill_flag == true){
        printf("\nThe child processes is killing...");
        for (int i =0; i<pnum; i++)
            kill(children[i], SIGKILL);
        return 1;
    }
    
   if (waitpid(-1, &status, WNOHANG) > 0) { 
            active_child_processes -= 1;
        }

  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
      fscanf(pf, "%d ",&min);
        fscanf(pf, "%d\n",&max);
    } else {
      // read from pipes
      
      close(p[1]);
      read(p[0],&max,sizeof(int));
      read(p[0],&min,sizeof(int));
        
        close(p[0]);
      
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  if(with_files)
    fclose(pf);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  
  free(array);
    free(p);
    
  printf("\nMin: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}