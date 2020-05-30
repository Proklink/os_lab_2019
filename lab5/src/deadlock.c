#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
int *Acommon = 0;
int *Bcommon = 0;

void F1(int *a)
{
    int b = *a;
    pthread_mutex_lock(&mut1);
    printf("\nthread1 takes the mut1");
    Acommon = &b;
    printf("\nAcommon = %d", *Acommon);
        pthread_mutex_lock(&mut2);
        printf("\nthread1 takes the mut2");
        Bcommon = &b;
        printf("\nBcommon = %d", *Bcommon);
        printf("\nthread1 is finishing...");
        //pthread_mutex_unlock(&mut2);
    //pthread_mutex_unlock(&mut1);
 
    

}

void F2(int *a)
{
    int b = *a;
    pthread_mutex_lock(&mut2);

    printf("\nthread2 takes the mut2");

    Bcommon = &b;

    printf("\nBcommon = %d", *Bcommon);

        pthread_mutex_lock(&mut1);

         printf("\nthread2 takes the mut1");

        Acommon = &b;

        printf("\nAcommon = %d", *Acommon);
        printf("\nthread2 is finishing...");
        //pthread_mutex_lock(&mut1);
    //pthread_mutex_unlock(&mut2);

    

}

int main()
{
    printf("\na");
    pthread_t Threads_mass[2];
    int a = 1;
    int b = 1;

        if (pthread_create(Threads_mass, NULL, (void *)F1,
			  (void *)&a) != 0) {
        perror("pthread_create");
        exit(1);
              }

        if (pthread_create(Threads_mass + 1, NULL, (void *)F2,
			  (void *)&b) != 0) {
        perror("pthread_create");
        exit(1);
              }
    
    for(int i = 0; i < 2; i++)
        if (pthread_join(Threads_mass[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
printf("\nb");
    return 0;
}

