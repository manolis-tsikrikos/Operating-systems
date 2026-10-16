/* AM: P24182 */
/* Ονοματεπώνυμο: Manolis Tsikrikos */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 10000000

int shared[3] = {0, 0, 0};        // Κοινόχρηστοι μετρητές
pthread_mutex_t locks[3];        // Μηχανισμοί αμοιβαίου αποκλεισμού

typedef struct {
    int tid;                     // Αναγνωριστικό νήματος
} thread_arg;

/* Ασφαλής αύξηση ενός μετρητή */
void increase(int idx)
{
    pthread_mutex_lock(&locks[idx]);
    shared[idx]++;
    pthread_mutex_unlock(&locks[idx]);
}

void *worker(void *arg)
{
    int id = ((thread_arg *)arg)->tid;

    /* Νήματα 0 και 1: ίδια ακολουθία ενημερώσεων */
    if (id == 0 || id == 1) {
        for (int i = 0; i < N; i++) increase(0);
        for (int i = 0; i < N; i++) increase(1);
        for (int i = 0; i < N; i++) increase(0);
    }

    /* Νήμα 2: διαφορετική σειρά ενημερώσεων */
    if (id == 2) {
        for (int i = 0; i < N; i++) increase(2);
        for (int i = 0; i < N; i++) increase(0);
        for (int i = 0; i < N; i++) increase(1);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[3];
    thread_arg args[3];

    /* Ρύθμιση mutexes και ορισμός αναγνωριστικών */
    for (int i = 0; i < 3; i++) {
        pthread_mutex_init(&locks[i], NULL);
        args[i].tid = i;
    }

    /* Εκκίνηση των νημάτων */
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    /* Αναμονή ολοκλήρωσης όλων των νημάτων */
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Εμφάνιση τελικών τιμών μετρητών */
    printf("counter[0] = %d\n", shared[0]);
    printf("counter[1] = %d\n", shared[1]);
    printf("counter[2] = %d\n", shared[2]);

    /* Αποδέσμευση πόρων συγχρονισμού */
    for (int i = 0; i < 3; i++) {
        pthread_mutex_destroy(&locks[i]);
    }

    return 0;
}