#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

static struct termios
old,
raw;

static void
restore_term_attrs()
{
    tcsetattr(0, TCSAFLUSH, &old);
}

#define CTRL_C 3
#define CTRL_D 4
#define ESC    28

static void
wait_tap()
{
    int c = getchar();

    if (c == EOF || c == CTRL_C || c == CTRL_D || c == ESC) {
        printf("\n");
        exit(EXIT_SUCCESS);
    }
}

static uint64_t
ms()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    return ((uint64_t)now.tv_sec  * 1000ull)
         + ((uint64_t)now.tv_usec / 1000ull);
}

int
main()
{
    if (tcgetattr(STDIN_FILENO, &old) < 0) {
        if (errno == ENOTTY) {
            fprintf(stderr, "termpo must be run from an interactive terminal\n");
        } else {
            perror("tcgetattr");
        }

        exit(EXIT_FAILURE);
    }

    raw = old;
    cfmakeraw(&raw);

    atexit(restore_term_attrs);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    wait_tap();

    uint64_t start_ms = ms();
    uint64_t taps = 0;

    while (1) {
        wait_tap();

        taps++;

        double diff_ms = ms() - start_ms;

        double bpm = (taps * 60000.0) / diff_ms;

        printf(" %5.1lf bpm", bpm);

        if (bpm < 80) {
            printf(" (maybe %.1lf bpm)", bpm * 2);
        } else if (bpm > 160) {
            printf(" (maybe %.1lf bpm)", bpm / 2);
        }

        /* lmao. - TODO use an ANSI escape code to clear the line instead */
        printf("                   \r");
    }
}
