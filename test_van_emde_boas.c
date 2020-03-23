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

        long U = 16;
        long u = __builtin_ctz(U);

        veb_node *root = new_veb(u);
        key_t a[5] = {1, 3, 6, 7, 8};
        for (int i = 0; i < 5; i++){
                insert(root, a[i]);
                printf(contains(root, a[i]) ? "true\n" : "false\n");
        }
        return EXIT_SUCCESS;
}