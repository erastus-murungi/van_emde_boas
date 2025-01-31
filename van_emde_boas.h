#ifndef VAN_EMDE_BOAS_VAN_EMDE_BOAS_H
#define VAN_EMDE_BOAS_VAN_EMDE_BOAS_H

#define swap(x, y) do { register typeof(x) swap = x; x = y; y = swap; } while (0)

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <sys/time.h>
#include <stdio.h>

typedef int32_t key_t;

//#pragma pack(1)

#define LEAFSIZE ((size_t) (16))
#define NODESIZE ((size_t) (32))


struct veb_node {
    key_t max, min;
    key_t u;
    struct veb_node **cluster;
    struct veb_node *summary;
};

typedef struct veb_node veb_node;

static inline key_t id(key_t k, key_t c, key_t i);

bool contains(veb_node *v, key_t k);

void insert(veb_node *v, key_t x);

void delete(veb_node *v, key_t x);

void veb_free(veb_node *v);

key_t predecessor(veb_node *v, key_t x);

key_t successor(veb_node *v, key_t x);

key_t inorder(veb_node *v, key_t *A, key_t na);

static inline key_t minimum(veb_node *v);

static inline key_t maximum(veb_node *v);

veb_node *new_veb(key_t u);

// utilities
void to_string(veb_node *v);

void random_array(key_t *A, key_t u, size_t na);

void print_array(key_t *A, key_t na);

size_t sizeof_veb(veb_node *v);

#endif //VAN_EMDE_BOAS_VAN_EMDE_BOAS_H
