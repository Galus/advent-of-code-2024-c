#include "util.h"

int compare_integers(const void *a, const void *b) {
    int *ia = (int *)a; // Cast to integer pointers
    int *ib = (int *)b;
    return *ia - *ib;    // Difference returns <0, 0, or >0
}

