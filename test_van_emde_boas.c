//
// Created by Erastus Murungi on 3/23/20.
//

#include "van_emde_boas.h"
#include <stdio.h>


int main(int argc, char **argv){
/*        if (argc < 2){
                fprintf(stderr, "run program with universe size");
        }
        long U = strtol(argv[1], NULL, 10);*/

        long U = 32;
        long u = __builtin_ctz(U);
        long i, n;
        n = 10;

        veb_node *root = new_veb(u);
        key_t a[10] = {1, 3, 16, 7, 8, 9, 14, 25, 6, 9};
        for (i = 0; i < n; i++){
                insert(root, a[i]);
                printf("predecessor %d: %d\n", a[i], predecessor(root, a[i]));
                printf(contains(root, a[i]) ? "true\n" : "false\n");
        }
        for (i = 0; i < n; i++){
                printf("successor %d: %d\n", a[i], successor(root, a[i]));
        }

        printf(contains(root, 11) ? "true\n" : "false\n");
        to_string(root);
        return EXIT_SUCCESS;
}