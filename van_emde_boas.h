//
// Created by Erastus Murungi on 3/22/20.
//

#ifndef VAN_EMDE_BOAS_VAN_EMDE_BOAS_H
#define VAN_EMDE_BOAS_VAN_EMDE_BOAS_H
/*
 * goal: U
 */

#define swap(x, y) do { typeof(x) swap = x; x = y; y = swap; } while (0)

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

typedef int32_t key_t;

struct veb_node {
    key_t max, min;
    key_t u;
    struct veb_node **cluster;
    struct veb_node *summary;
};

typedef struct veb_node veb_node;

static inline key_t id(key_t k, key_t cid, key_t pos);

bool contains(veb_node *v, key_t k);

void insert(veb_node *v, key_t x);

void delete(veb_node *v, key_t x);

void veb_free(veb_node *v);

key_t predecessor(veb_node *v, key_t x);

key_t successor(veb_node *v, key_t x);

static inline key_t minimum(veb_node *v);

static inline key_t maximum(veb_node *v);

veb_node *new_veb(key_t u);

void to_string(veb_node *v);



#endif //VAN_EMDE_BOAS_VAN_EMDE_BOAS_H



