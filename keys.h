#ifndef KEYS_H
#define KEYS_H

#include "types.h"
#include <stddef.h>

typedef struct {
    ConjuntoAtributo *keys;
    size_t count;
    size_t capacity;
} KeyList;

void keylist_init(KeyList *kl);
void keylist_free(KeyList *kl);
void find_candidate_keys(const Schema *s, KeyList *out);
void print_candidate_keys(const Schema *s);

#endif