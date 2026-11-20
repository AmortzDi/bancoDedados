#include "types.h"

//Mapeia 'A'..'Z' para os bits 0..25 
ConjuntoAtributo attr_bit(char c) {
    return (c >= 'A' && c <= 'Z') ? (1u << (c - 'A')) : 0u;
}

ConjuntoAtributo str_to_ConjuntoAtributo(const char *str) {
    ConjuntoAtributo set = 0;
    for (const char *p = str; *p != '\0'; p++) {
        set |= attr_bit(*p);
    }
    return set;
}

// Varre de 0 a 25 para garantir saída sempre em ordem alfabética 
void ConjuntoAtributo_to_str(ConjuntoAtributo set, char *buf) {
    size_t idx = 0;
    for (int i = 0; i < 26; i++) {
        if (set & (1u << i)) {
            buf[idx++] = (char)('A' + i);
        }
    }
    buf[idx] = '\0';
}