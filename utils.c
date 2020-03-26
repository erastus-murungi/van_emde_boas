//
// Created by Erastus Murungi on 3/26/20.
//
#include "van_emde_boas.h"

void random_array(key_t *A, key_t u, size_t na){
        size_t i;
        struct timeval ts;
        gettimeofday(&ts, NULL);
        srandom(ts.tv_usec ^ ts.tv_sec);  // Seed the PRNG

        // random() % (max_number + 1 - minimum_number) + minimum_number
        for (i = 0; i < na; i++){
                A[i] = random() % u;
        }
}

void print_array(key_t *A, key_t na) {
/**
 * simple helper to print an array in python style
 */
        if (na == 0) {
                printf("[]\n");
                return;
        }
        printf("[");
        for (int i = 0; i < (na - 1); i++) {
                printf("%d, ", A[i]);
        }
        printf("%d]\n", A[na - 1]);
}
