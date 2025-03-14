#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define FILLED 0
#define Ready 1
#define NotReady -1

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
    }
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

    while (1) {
        while (shmptr->status != Ready)
            continue;

        sleep(1);

        printf("User1: ");
        fgets(shmptr->buff, 100, stdin);
        shmptr->status = FILLED;

        kill(shmptr->pid2, SIGUSR1);
    }

    shmdt((void*)shmptr);
    semctl(sem_id, 0, IPC_RMID); // Clean up semaphore
    return 0;
}
