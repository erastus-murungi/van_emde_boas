//
// Created by Erastus Murungi on 3/23/20.
//

#include "van_emde_boas.h"
#include <stdio.h>


int main(int argc, char **argv) {
/*        if (argc < 3) {
                fprintf(stderr, "run program with universe size and number of integers to test\n");
                exit(EXIT_FAILURE);
        }
        long U = strtol(argv[1], NULL, 10);
        long u = __builtin_ctz(U);
        long i, na;

        na = strtol(argv[2], NULL, 10);*/

        long U, u, na, i;
        U = (1 << 16);
        u = __builtin_ctz(U);
        na = 200;
        if ((U & (U - 1))) {
                fprintf(stderr, "U must be a power of 2");
                exit(EXIT_FAILURE);
        }

        key_t *A = malloc(sizeof(key_t) * na);
        random_array(A, U, na);

//        print_array(A, na);
        veb_node *root = new_veb(u);

        for (i = 0; i < na; i++) {
                insert(root, A[i]);
//                printf("predecessor %.2d: %.2d\n", A[i], predecessor(root, A[i]));
        }
//        print_array(A, na);
        i = inorder(root, A, na);
        if (i != -1 && i < 1000)
                print_array(A, i);
        printf("printed size: %ld\n", i);

        uint8_t ok = 1;
        for (i = 0; i < na; i++) {
                ok &= contains(root, A[i]);
        }
        if (!ok){
                fprintf(stderr, "possible implementation error\n");
        }
        for (i = 0; i < na; i++) {
                printf("successor %.2d: %.2d\n", A[i], successor(root, A[i]));
        }
        ok = 1;
        for (i = 0; i < na; i++) {
                delete(root, A[i]);
                ok &= !contains(root, A[i]);
        }
        if (!ok){
                fprintf(stderr, "duplicates in input\n");
        }

        to_string(root);
        veb_free(root);
        free(A);
        return EXIT_SUCCESS;
}