#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAX_ATTRS 26 // Representa o intervalo de A até Z

// Representa um conjunto de atributos (ex: X = {A, B, C})
typedef struct {
    bool attrs[MAX_ATTRS]; 
} AttrSet;

// Representa uma Dependência Funcional (left -> right)
typedef struct {
    AttrSet left;  // LHS (Determinante)
    AttrSet right; // RHS (Dependente)
} FD;

// Representa o esquema relacional lido do arquivo .fds
typedef struct {
    AttrSet U;       // Universo de atributos (U)
    FD *fds;         // Array dinâmico de dependências funcionais (F)
    int num_fds;     // Quantidade de DFs armazenadas
} Schema;

#endif