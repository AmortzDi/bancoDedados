#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t ConjuntoAtributo;

typedef struct {
    ConjuntoAtributo lhs;
    ConjuntoAtributo rhs;
} DF;

typedef struct {
    ConjuntoAtributo universe;
    DF *dfs;
    size_t count;
    size_t capacity;
} Schema;

ConjuntoAtributo attr_bit(char c);
ConjuntoAtributo str_to_ConjuntoAtributo(const char *str);
void ConjuntoAtributo_to_str(ConjuntoAtributo set, char *buf);

#endif