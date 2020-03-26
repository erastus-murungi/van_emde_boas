#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"


#include "van_emde_boas.h"
#include <stdio.h>

static struct {
    uint32_t low;
    uint32_t high;
} t;

static inline bool empty(veb_node *v) {
        return v->min == -1;
}

static inline void split(key_t x, key_t k) {
        t.low = x & ((1 << (k >> 1)) - 1);
        t.high = x >> (k >> 1);
}

static inline key_t id(key_t k, key_t cid, key_t pos) {
        return ((1 << (k >> 1)) * cid) + pos;
}

static inline key_t minimum(veb_node *v) {
        return v->min;
}

static inline key_t maximum(veb_node *v) {
        return v->max;
}

static inline void empty_insert(veb_node *v, key_t x) {
        v->min = v->max = x;
}

bool contains(veb_node *v, key_t x) {
        /* iterative member search */
        if (!v) {
                fprintf(stderr, "null received instead of veb_node\n");
                exit(EXIT_FAILURE);
        }
        while (v->u != 1) {
                if (x == v->max || x == v->min)
                        return true;
                split(x, v->u);
                v = v->cluster[t.high];
                x = t.low;
        }
        split(1, x);
        return (x == v->max || x == v->min);
}

veb_node *new_veb(key_t u) {
        /** create a new veb tree */
        assert (0 < u && u < 32);

        veb_node *v;

        if (u == 1) {
                v = malloc(offsetof(struct veb_node, cluster));
                v->u = u; v->max = v->min = -1;

        } else {
                key_t lb, hb;
                v = malloc(sizeof(veb_node));
                lb = u >> 1;
                hb = u - lb;
                v->u = u; v->min = v->max = -1;
                v->summary = new_veb(hb);
                v->cluster = malloc(sizeof(veb_node *) * (1 << hb));

                for (int i = 0; i < (1 << hb); i++)
                        v->cluster[i] = new_veb(lb);
        }
        return v;
}

void insert(veb_node *v, key_t x) {
        if (x >= (1<<v->u)){
                fprintf(stderr, "%d, %U", x, (1 << v->u));
                to_string(v);
                exit(EXIT_FAILURE);
        }
        // split x into 2 w/2-bit words
        if (v->min == -1)
                return empty_insert(v, x);
        if (x < v->min) // the min is not recursively stored.
                swap(x, v->min);

        split(x, v->u);

        if (v->u > 1) {
#ifdef DEBUG
                printf("x: %.2d {high(x): %d, low(x): %d}\n", x, t.high, t.low);
#endif
                if (empty(v->cluster[t.high])) {
                        insert(v->summary, t.high);
                        split(x, v->u);
                        empty_insert(v->cluster[t.high], t.low);
                } else {
                        insert(v->cluster[t.high], t.low);
                }
        }
        if (x > v->max) {          // the max is stored as a duplicate so always update it
                v->max = x;
        }
}

void delete(veb_node *v, key_t x) {
        if (v->min == v->max) {
                v->min = v->max = -1;
        } else if (v->u == 1) {
                v->min = (x) ? 0 : 1;
                v->max = v->min;
        } else {
                key_t cid, smax;
                split(x, v->u);
                if (x == v->min) {
                        cid = v->summary->min;
                        x = id(v->u, cid, v->cluster[cid]->min);
                        v->min = x;
                }
                split(x, v->u); // x has changed
                delete(v->cluster[t.high], t.low);
                if (v->cluster[t.high]->min == -1) {
                        split(x, v->u);
                        delete(v->summary, t.high);
                        if (x == v->max) {
                                smax = maximum(v->summary);
                                if (smax == -1) {
                                        v->min = v->max = -1;
                                } else {
                                        v->max = id(v->u, smax, maximum(v->cluster[smax]));
                                }
                        }
                } else if (x == v->max) {
                        v->max = id(v->u, t.high, maximum(v->cluster[t.high]));
                }
        }
}

key_t predecessor(veb_node *v, key_t x) {
        if (v->u == 1) {
                return (x == 1 && v->min == 0) ? 0 : -1;
        } else if (v->max != -1 && x > v->max) {
                return v->max;
        } else {
                split(x, v->u);
                key_t m, offset;
                m = minimum(v->cluster[t.high]);
                // if the x > than the minimum element in x's cluster.
                // Recursively find the predecessor to low(x) cluster[high(x)]
                if (!empty(v->cluster[t.high]) && t.low > m) {
                        offset = predecessor(v->cluster[t.high], t.low); split(x, v->u);
                        return id(v->u, t.high, offset);
                } else {
                        // find the previous non_empty cluster
                        m = predecessor(v->summary, t.high);
                        // if there is no previous non_empty cluster, then the current cluster is the minimum cluster
                        // 1. either return the min if x > min:
                        if (m == -1) {
                                return (v->min != -1 && x > v->min) ? v->min : -1;
                        } else {
                                offset = maximum(v->cluster[m]);
                                return id(v->u, m, offset);
                        }
                }
        }
}

key_t successor(veb_node *v, key_t x) {
        // if we are at a leaf node: if max_elem = 1 and we are looking for successor of 0, then return 1
        // else the max elem is 0 or x == 1, successor is null
        if (v->u == 1) {
                return (x == 0 && v->max == 1) ? 1 : -1;
        } else if (v->min != -1 && x < v->min) {
                return v->min; // the successor is the min and x is the max in the pred cluster
        } else {
                key_t m, offset;
                split(x, v->u);
                m = maximum(v->cluster[t.high]);
                if (m != -1 && t.low < m) {
                        offset = successor(v->cluster[t.high], t.low); split(x, v->u);
                        return id(v->u, t.high, offset);
                } else {
                        m = successor(v->summary, t.high);
                        if (m == -1) return m;
                        else {
                                offset = minimum(v->cluster[m]);
                                return id(v->u, m, offset);
                        }
                }
        }
}

void to_string(veb_node *v) {
        if (v) {
                printf("|U|: %d, min: %d, max: %d\n", (1 << v->u), v->min, v->max);
        }
}

void veb_free(veb_node *v){
        if (v->u == 1) {
                return free(v);
        } else {
                key_t hs = v->u - (v->u >> 1);
                veb_free(v->summary);
                for (int i = 0; i < (1 << hs); i++)
                        veb_free(v->cluster[i]);
        }
}

#pragma clang diagnostic pop
