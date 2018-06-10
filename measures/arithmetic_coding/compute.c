#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define S (1 / (double) (1L << 32L))
#define R 32.
#define N 100000

static inline int randbit(double p) {
    return (double) (rand()) / (double) (RAND_MAX) < (1 - p) ? 0 : 1;
}

long long make_exp(double p) {
    long long n = 0;
    double l = 1.;
    while (l >= S) {
        if (randbit(p)) {
            l *= p;
        } else {
            l *= (1 - p);
        }
        n++;
    }
    return n - 1;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    double total = 0;
    char *end = NULL;
    double p = strtod(argv[1], &end);
    for (int i = 0; i < N; i++) {
        total += (double) (make_exp(p)) / R;
    }
    printf("%f\n", total / (double) (N));
}
