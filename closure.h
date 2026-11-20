#ifndef CLOSURE_H
#define CLOSURE_H

#include "types.h"

ConjuntoAtributo compute_closure(const Schema *s, ConjuntoAtributo x);
ConjuntoAtributo compute_closure_skip(const Schema *s, ConjuntoAtributo x, size_t skip_idx);

#endif