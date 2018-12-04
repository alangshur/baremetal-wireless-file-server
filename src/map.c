#include "map.h"
#include "strings.h"
#include "malloc.h"
#include "printf.h"
#include <stdbool.h>

// define struct
struct map_node_t {
    unsigned hash;
    void* value;
    map_node_t* next;
};

// hash string name
static unsigned map_hash(const char* str) {
    unsigned hash = 5381;
    while (*str) hash = ((hash << 5) + hash) ^ *str++;
    return hash;
}

// create new map node
static map_node_t* map_newnode(const char* key, void* value, int vsize) {
    map_node_t* node;
    int ksize = strlen(key) + 1;
    int voffset = ksize + ((sizeof(void*) - ksize) % sizeof(void*));

    // attempt to allocate node
    node = malloc(sizeof(*node) + voffset + vsize);
    if (!node) return NULL;

    // fill in node
    memcpy(node + 1, key, ksize);
    node->hash = map_hash(key);
    node->value = ((char*) (node + 1)) + voffset;
    memcpy(node->value, value, vsize);
    return node;
}

// get map bucket id
static int map_bucketidx(map_base_t* m, unsigned hash) {
    return hash & (m->nbuckets - 1);
}

// add new map node
static void map_addnode(map_base_t* m, map_node_t* node) {
    int n = map_bucketidx(m, node->hash);
    node->next = m->buckets[n];
    m->buckets[n] = node;
}

// resize map
static int map_resize(map_base_t* m, int nbuckets) {
    map_node_t *nodes, *node, *next;
    map_node_t** buckets;
    int i; 

    // chain nodes together
    nodes = NULL;
    i = m->nbuckets;
    while (i--) {
        node = (m->buckets)[i];
        while (node) {
            next = node->next;
            node->next = nodes;
            nodes = node;
            node = next;
        }
    }

    // reset buckets
    buckets = realloc(m->buckets, sizeof(*m->buckets) * nbuckets);
    if (buckets != NULL) {
        m->buckets = buckets;
        m->nbuckets = nbuckets;
    }

    if (m->buckets) {
        memset(m->buckets, 0, sizeof(*m->buckets) * m->nbuckets);
        
        // re-add nodes to bucket
        node = nodes;
        while (node) {
            next = node->next;
            map_addnode(m, node);
            node = next;
        }
    }

    // return error code
    return (buckets == NULL) ? -1 : 0;
}

// get map ref
static map_node_t** map_getref(map_base_t* m, const char* key) {
    unsigned hash = map_hash(key);
    map_node_t** next;
    
    // get ref
    if (m->nbuckets > 0) {
        next = &m->buckets[map_bucketidx(m, hash)];
        while (*next) {
            if ((*next)->hash == hash && !strcmp((char*) (*next + 1), key)) {
                return next;
            }

            next = &(*next)->next;
        }
    }

    return NULL;
}

void map_deinit_(map_base_t* m) {
    map_node_t *next, *node;
    int i;
    i = m->nbuckets;

    // remove every node
    while (i--) {
        node = m->buckets[i];
        while (node) {
            next = node->next;
            free(node);
            node = next;
        }
    }

    free(m->buckets);
}

void *map_get_(map_base_t* m, const char* key) {
    map_node_t **next = map_getref(m, key);
    return next ? (*next)->value : NULL;
}

int map_set_(map_base_t* m, const char* key, void* value, int vsize) {
    int n, err;
    map_node_t **next, *node;
    
    // find and replace existing node
    next = map_getref(m, key);
    if (next) {
        memcpy((*next)->value, value, vsize);
        return 0;
    }
    
    // add new node
    node = map_newnode(key, value, vsize);
    if (node == NULL) goto fail;
    if (m->nnodes >= m->nbuckets) {
        n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
        err = map_resize(m, n);
        if (err) goto fail;
    }

    // add node
    map_addnode(m, node);
    m->nnodes++;
    return 0;

    // fail map set
    fail:
    if (node) free(node);
    return -1;
}

void map_remove_(map_base_t* m, const char* key) {
    map_node_t* node;
    map_node_t** next = map_getref(m, key);
    
    // remove node
    if (next) {
        node = *next;
        *next = (*next)->next;
        free(node);
        m->nnodes--;
    }
}

map_iter_t map_iter_(void) {
    map_iter_t iter;
    iter.bucketidx = -1;
    iter.node = NULL;
    return iter;
}

const char *map_next_(map_base_t* m, map_iter_t* iter) {
    if (iter->node) {
        iter->node = iter->node->next;
        if (iter->node == NULL) goto nextBucket;
    } 
    else {
        nextBucket:
        do {
            if (++iter->bucketidx >= m->nbuckets) {
                return NULL;
            }
            iter->node = m->buckets[iter->bucketidx];
        } 
        while (iter->node == NULL);
    }

    return (char*) (iter->node + 1);
}

bool map_contains_(map_base_t* m, const char* key) {
    const char* iter_key;
    map_iter_t iter = map_iter_();

    // iterate through map elements
    while((iter_key = map_next_(m, &iter))) {
        if (!strcmp(iter_key, key)) return true;
    }

    return false;
}

unsigned int map_size_(map_base_t* m) {
    const char* iter_key;
    map_iter_t iter = map_iter_();
    unsigned int counter = 0;

    // iterate through map elements
    while((iter_key = map_next_(m, &iter))) counter++;
    return counter;
}

void map_print_(map_base_t* m) {
    const char* iter_key;
    map_iter_t iter = map_iter_();

    // iterate through map elements
    while((iter_key = map_next_(m, &iter))) 
        printf("%s -> %s\n", iter_key, (char*) *((int*) map_get_(m, iter_key)));
}