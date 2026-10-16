/* AM: P24182 */
/* Ονοματεπώνυμο: Manolis Tsikrikos */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

sem_t semA[2];   // Σηματοδότες για τις ενότητες A, B, C
sem_t semD[2];   // Σηματοδότες για τις ενότητες D, E, F

void *worker_left(void *arg)
{
    int i = *(int *)arg;
    printf("ID = %d\n", i);

    /* Ατέρμονος κύκλος εργασιών αριστερής ομάδας */
    while (1) {
        sem_wait(&semA[i]);
        printf("A.%d\n", i);

        sem_post(&semD[i]);

        sem_wait(&semA[i]);
        printf("B.%d\n", i);

        sem_post(&semD[i]);

        sem_wait(&semA[i]);
        printf("C.%d\n", i);

        sem_post(&semD[i]);
    }
    return NULL;
}

void *worker_right(void *arg)
{
    int i = *(int *)arg;
    printf("ID = %d\n", i + 2);

    /* Ατέρμονος κύκλος εργασιών δεξιάς ομάδας */
    while (1) {
        sem_wait(&semD[i]);
        printf("D.%d\n", i + 2);
        printf("E.%d\n", i + 2);

        sem_post(&semA[i]);

        sem_wait(&semD[i]);
        printf("F.%d\n", i + 2);

        sem_post(&semA[i]);
    }
    return NULL;
}

int main(void)
{
    pthread_t th[4];
    int idx[2] = {0, 1};

    /* Αρχικοποίηση όλων των σηματοδοτών */
    for (int i = 0; i < 2; i++) {
        sem_init(&semA[i], 0, 0);
        sem_init(&semD[i], 0, 0);
    }

    /* Δημιουργία νημάτων αριστερής πλευράς */
    pthread_create(&th[0], NULL, worker_left, &idx[0]);
    pthread_create(&th[1], NULL, worker_left, &idx[1]);

    /* Δημιουργία νημάτων δεξιάς πλευράς */
    pthread_create(&th[2], NULL, worker_right, &idx[0]);
    pthread_create(&th[3], NULL, worker_right, &idx[1]);

    /* Ενεργοποίηση της πρώτης ακολουθίας */
    sem_post(&semA[0]);

    /* Αναμονή τερματισμού όλων των νημάτων */
    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }

    return 0;
}