#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Inspiration from: https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/

#define MAX_PHILOSOPHERS 10
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + num_philosophers - 1) % num_philosophers
#define RIGHT (phnum + 1) % num_philosophers

int num_philosophers;
int mode;  // Mode 1: Standard way, Mode 2: Alternating way
int state[MAX_PHILOSOPHERS];
int *phil;

sem_t mutex;
sem_t *S;

void test(int phnum)
{
    if (state[phnum] == HUNGRY &&
        state[LEFT] != EATING &&
        state[RIGHT] != EATING) {
        state[phnum] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n", phnum + 1,
               (mode == 1 || phnum % 2 == 0) ? LEFT + 1 : RIGHT + 1,
               (mode == 1 || phnum % 2 == 0) ? phnum + 1 : LEFT + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        sem_post(&S[phnum]);
    }
}

void take_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum);
    sem_post(&mutex);
    sem_wait(&S[phnum]);
    sleep(1);
}

void put_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n", phnum + 1,
           (mode == 1 || phnum % 2 == 0) ? LEFT + 1 : RIGHT + 1,
           (mode == 1 || phnum % 2 == 0) ? phnum + 1 : LEFT + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void *philosopher(void *num)
{
    while (1) {
        int *i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <mode> <number_of_philosophers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mode = atoi(argv[1]);
    num_philosophers = atoi(argv[2]);

    if (num_philosophers <= 1 || num_philosophers > MAX_PHILOSOPHERS) {
        printf("Number of philosophers must be between 2 and %d\n", MAX_PHILOSOPHERS);
        exit(EXIT_FAILURE);
    }

    phil = malloc(num_philosophers * sizeof(int));
    S = malloc(num_philosophers * sizeof(sem_t));

    int i;
    pthread_t *thread_id = malloc(num_philosophers * sizeof(pthread_t));

    sem_init(&mutex, 0, 1);

    for (i = 0; i < num_philosophers; i++) {
        phil[i] = i;
        sem_init(&S[i], 0, 0);
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < num_philosophers; i++)
        pthread_join(thread_id[i], NULL);

    free(phil);
    free(S);
    free(thread_id);

    return 0;
}
