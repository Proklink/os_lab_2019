#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static struct option options[] = {{"k",        required_argument, 0, 0},
                                 {"mod",       required_argument, 0, 0},
                                 {"pnum",      required_argument, 0, 0},
                                 {0, 0,                          0, 0}};

int getArguments(int argc, char **argv, int *arg)
{

     while (true) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0: {
                        arg[0] = atoi(optarg);
                        if (arg[0] < 0) {
                            printf("The factorial argument must be a positive number or 0. Now factorial argument = %d\n", arg[0]);
                            return -1;
                        }
                        break;
                    }

                    case 1:
                        arg[1] = atoi(optarg);
                        if (arg[1] < 0) {
                            printf("The factorial modul's must be a positive number. Now modul = %d\n", arg[1]);
                            return -1;
                        }
                        break;

                    case 2: {
                        arg[2] = atoi(optarg);
                        if (arg[2] <= 0) {
                            printf("Threads_num must be 1 or more. Now threads_num = %d\n", arg[2]);
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
    return 0;
}

void Multy_thread_factorial(int* arg)
{
    int n = arg[0];
    int p = arg[1];
    int res = 1;
    printf("\nF:%d %d %d",n,p,res);
	while (n > 1) {
        pthread_mutex_lock(&mut);
		if (n % p == 0)
	        res *= n/p;
	    else
	        res *= n%p;
	    n--;
        pthread_mutex_unlock(&mut);
	}
    printf("\n%d %d %d",n,p,res);
	arg[2] = res % p;
}

int main(int argc, char **argv) {
    
    int k = -1;
    int mod = -1;
    int pnum = -1;
    int arg[3];
    
    if (getArguments(argc, argv, arg))
        return -1;

    k = arg[0];
     mod = arg[1];
     pnum = arg[2];


    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (k == -1 || mod == -1 || pnum == -1 ) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
            argv[0]);
        return 1;
    }
 
    pthread_t Threads_mass[pnum];

    printf("\n%d %d %d",arg[0],arg[1],arg[2]);
    for(int i = 0; i < pnum; i++)
        if (pthread_create(Threads_mass + i, NULL, (void *)Multy_thread_factorial,
			  (void *)arg) != 0) {
        perror("pthread_create");
        exit(1);
        }
    
    for(int i = 0; i < pnum; i++)
        if (pthread_join(Threads_mass[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }

    printf("\n%d!mod(%d) = %d\n", k, mod, arg[2]);

    return 0;
}