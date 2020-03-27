//
// Created by Erastus Murungi on 3/23/20.
//

#include "van_emde_boas.h"
#include <stdio.h>

long get_milli(){
        struct timeval curr_time;
        gettimeofday(&curr_time, NULL);
        return curr_time.tv_sec * (int) 1e3 + curr_time.tv_usec / (int) 1e3;
}


int main(void) {
        long U, u, na, i;
        U = (1 << 26);
        u = __builtin_ctz(U);
        na = 50000000;
        if ((U & (U - 1))) {
                fprintf(stderr, "U must be a power of 2");
                exit(EXIT_FAILURE);
        }

        key_t *A = malloc(sizeof(key_t) * na);
        random_array(A, U, na);
        //print_array(A, na);

        long t1 = get_milli();
        veb_node *root = new_veb(u);
        t1 = get_milli() - t1;
        printf("construction took %ld milliseconds\n", t1);
        t1 = get_milli();
        for (i = 0; i < na; i++) {
                insert(root, A[i]);
        }
        t1 = get_milli() - t1;
        printf("%ld insertions took %ld milliseconds\n", na, t1);
        to_string(root);

        t1 = get_milli();
        for (i = 0; i < na; i++){
                predecessor(root, A[i]);
        //printf("predecessor %.2d: %.2d\n", A[i], predecessor(root, A[i]));
        }
        t1 = get_milli() - t1;
        printf("%ld predecessor queries took %ld milliseconds\n", na, t1);

        i = inorder(root, A, na);
        if (i != -1 && i < 1000)
                print_array(A, i);
        uint8_t ok = 1;
        t1 = get_milli();
        for (i = 0; i < na; i++) {
                ok &= contains(root, A[i]);
        }

        t1 = get_milli() - t1;
        printf("%ld searches took %ld milliseconds\n", na, t1);
        //if (!ok){
        //fprintf(stderr, "possible implementation error\n");
        //}

        t1 = get_milli();
        for (i = 0; i < na; i++) {
                successor(root, A[i]);
        //printf("successor %.2d: %.2d\n", A[i], successor(root, A[i]));
        }

        t1 = get_milli() - t1;
        printf("%ld successor queries took %ld milliseconds\n", na, t1);
        //ok = 1;

        t1 = get_milli();
        for (i = 0; i < na; i++) {
                delete(root, A[i]);
        //ok &= !contains(root, A[i]);
        }

        t1 = get_milli() - t1;
        printf("%ld deletions took %ld milliseconds\n", na, t1);
        // if (!ok){
        //fprintf(stderr, "duplicates in input\n");
        // }
        to_string(root);
        veb_free(root);
        free(A);
        return EXIT_SUCCESS;
}