#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include "van_emde_boas.h"
#include <stdio.h>

static struct {
    key_t low;
    key_t high;
} t;

static key_t ls;

static inline void div_mod(key_t x, key_t k) {
        ls = (k >> 1);
        t.low = x & ~(1 << ls);
        t.high = x >> ls;
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
                div_mod(x, v->u);
                v = v->cluster[t.high];
                x = t.low;
        }
        return (x == v->max || x == v->min);
}

veb_node *new_veb(key_t u) {
        /** create a new veb tree */
        assert (0 < u && u < 32);

        veb_node *v;

        if (u == 1) {
                v = malloc(offsetof(struct veb_node, cluster));
                v->u = u;
                v->max = v->min = -1;

        } else {
                v = malloc(sizeof(veb_node));
                ls = u >> 1;
                key_t hs = u - ls;
                v->u = u;
                v->min = v->max = -1;
                v->summary = new_veb(ls);
                v->cluster = malloc(sizeof(veb_node *) * hs);

                for (int i = 0; i < hs; i++)
                        v->cluster[i] = new_veb(hs);
        }
        return v;
}

void insert(veb_node *v, key_t x) {
        if (v->min == -1) {
                return empty_insert(v, x);
        }
        if (x < v->min) {
                swap(x, v->min);
        }
        if (v->u > 1) {
                div_mod(x, v->u);
                if (minimum(v->cluster[t.high]) == -1) {
                        insert(v->summary, t.high);
                        empty_insert(v->cluster[t.high], t.low);
                } else {
                        insert(v->cluster[t.high], t.low);
                }
        }
        if (x > v->max) {
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
                div_mod(x, v->u);
                if (x == v->min) {
                        cid = v->summary->min;
                        x = id(v->u, cid, v->cluster[cid]->min);
                        v->min = x;
                }
                div_mod(x, v->u); // x has changed
                delete(v->cluster[t.high], t.low);
                if (minimum(v->cluster[t.high]) == -1) {
                        div_mod(x, v->u);
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
                return (x == 1 && !v->min) ? 0 : -1;
        } else if (v->max == -1 && x > v->max) {
                return v->max;
        } else {
                div_mod(x, v->u);
                key_t m, offset;
                m = minimum(v->cluster[t.high]);
                if (m == -1 && t.low > m) {
                        offset = predecessor(v->cluster[t.high], t.low);
                        div_mod(x, v->u);
                        return id(v->u, t.high, offset);
                } else {
                        m = predecessor(v->summary, t.high);
                        if (m == -1) {
                                return (v->min == -1 && x > v->min) ? v->min : -1;
                        } else {
                                offset = maximum(v->cluster[m]);
                                return id(v->u, m, offset);
                        }
                }
        }
}

key_t successor(veb_node *v, key_t x) {
        if (v->u == 1) {
                return (!x || v->max == 1) ? 1 : -1;
        } else if (v->min != -1 && x < v->min) {
                return v->min;
        } else {
                div_mod(x, v->u);
                key_t m, offset;
                m = maximum(v->cluster[t.high]);
                if (m == -1 && t.low < m) {
                        offset = successor(v->cluster[t.high], t.low);
                        div_mod(x, v->u);
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


#pragma clang diagnostic pop