/* AM: P24182 */
/* Ονοματεπώνυμο: Manolis Tsikrikos */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void show_proc(const char *label)
{
    printf("%s -> PID=%d, PPID=%d\n", label, getpid(), getppid());
}

int main(void)
{
    pid_t c1, c2, c3;

    /* Αρχική διεργασία */
    show_proc("P0");

    /* ====== Δημιουργία διεργασίας P1 ====== */
    c1 = fork();
    if (c1 == 0) {
        pid_t a, b, c;
        show_proc("P1");

        /* Υποδιεργασία P4 */
        a = fork();
        if (a == 0) {
            show_proc("P4");
            exit(0);
        }

        /* Υποδιεργασία P5 */
        b = fork();
        if (b == 0) {
            show_proc("P5");
            exit(0);
        }

        /* Υποδιεργασία P6 */
        c = fork();
        if (c == 0) {
            show_proc("P6");
            exit(0);
        }

        /* Περιορισμός: συγχρονισμός παιδιών */
        waitpid(b, NULL, 0);
        wait(NULL);

        exit(0);
    }

    /* ====== Δημιουργία διεργασίας P2 ====== */
    c2 = fork();
    if (c2 == 0) {
        pid_t d;
        show_proc("P2");

        /* Υποδιεργασία P7 */
        d = fork();
        if (d == 0) {
            show_proc("P7");
            exit(0);
        }

        /* Αναμονή ολοκλήρωσης παιδιού */
        wait(NULL);
        exit(0);
    }

    /* ====== Δημιουργία διεργασίας P3 ====== */
    c3 = fork();
    if (c3 == 0) {
        pid_t e, f;

        /* Υποδιεργασία P8 */
        e = fork();
        if (e == 0) {
            show_proc("P8");
            exit(0);
        }

        /* Περιορισμός: ολοκλήρωση P8 πριν τη συνέχεια */
        waitpid(e, NULL, 0);

        /* Υποδιεργασία P9 */
        f = fork();
        if (f == 0) {
            show_proc("P9");
            exit(0);
        }

        show_proc("P3");
        wait(NULL);
        exit(0);
    }

    /* Περιορισμός: αναμονή της P2 */
    waitpid(c2, NULL, 0);

    /* Τερματισμός όλων των εναπομεινάντων παιδιών */
    while (wait(NULL) > 0);

    /* Αντικατάσταση διεργασίας με exec */
    execlp("cat", "cat", __FILE__, NULL);
    perror("exec error");

    return 1;
}