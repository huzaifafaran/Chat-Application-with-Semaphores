#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define FILLED 0
#define Ready 1
#define NotReady -1
#define TIMEOUT_SEC 6 // Timeout value in seconds

struct memory {
    char buff[100];
    int status, pid1, pid2;
};

struct memory* shmptr;
int sem_id;

// Semaphore initialization
void init_sem() {
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 0); // Initializing semaphore value to 0
}

// Semaphore operations: wait and signal
void sem_wait() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(sem_id, &buf, 1);
}

void sem_signal() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(sem_id, &buf, 1);
}

// Handler function to print message received from User2
void handler(int signum) {
    if (signum == SIGUSR2) {
        printf("Received From User2: ");
        puts(shmptr->buff);
        sem_signal(); // Signal after receiving message
    }
}

// Function to get current time in seconds
double get_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main() {
    int pid = getpid();
    int shmid;
    int key = 12345;

    shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666);
    shmptr = (struct memory*)shmat(shmid, NULL, 0);
    shmptr->pid1 = pid;
    shmptr->status = NotReady;

    init_sem();

    signal(SIGUSR2, handler);

    double start_time, current_time;
    int timeout_flag = 0;

    while (1) {
        printf("User1: ");
        fgets(shmptr->buff, 100, stdin);
        shmptr->status = FILLED;

        kill(shmptr->pid2, SIGUSR1); // Send message immediately

        start_time = get_current_time(); // Start the timer

        while (!timeout_flag && shmptr->status != Ready) {
            current_time = get_current_time();
            if (current_time - start_time > TIMEOUT_SEC) {
                timeout_flag = 1;
                break;
            }
        }

        if (timeout_flag) {
            //printf("Timeout occurred. Resending message...\n"); //This disturbs the users
            timeout_flag = 0; // Reset timeout flag
            continue;
        }

        sem_wait(); // Wait for acknowledgment
    }

    shmdt((void*)shmptr);
    semctl(sem_id, 0, IPC_RMID); // Clean up semaphore
    return 0;
}
