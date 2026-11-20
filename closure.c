#include "closure.h"

// Calcula o fecho transitivo de um conjunto de atributos 'x' (X+),
// permitindo ignorar a DF no índice 'skip_idx' (útil para testar redundância na cobertura mínima).
ConjuntoAtributo compute_closure_skip(const Schema *s, ConjuntoAtributo x, size_t skip_idx) {
    // Inicializa o fecho garantindo que apenas atributos válidos do universo sejam considerados
    ConjuntoAtributo closure = x & s->universe;
    bool changed = true;

    // Repete as varreduras enquanto novos atributos continuarem sendo deduzidos (ponto fixo)
    while (changed) {
        changed = false;
        for (size_t i = 0; i < s->count; i++) {
            if (i == skip_idx) continue;

            ConjuntoAtributo lhs = s->dfs[i].lhs;
            ConjuntoAtributo rhs = s->dfs[i].rhs;

            // Se todos os atributos do lado esquerdo (LHS) já estão no fecho atual,
            // dispara a dependência e adiciona o lado direito (RHS) ao fecho.
            if ((closure & lhs) == lhs) {
                ConjuntoAtributo next = closure | rhs;
                if (next != closure) {
                    closure = next;
                    changed = true;
                }
            }
        }
    }
    return closure;
}

ConjuntoAtributo compute_closure(const Schema *s, ConjuntoAtributo x) {
    // Passa (size_t)-1 (maior valor possível de size_t) como sentinela para não ignorar nenhuma DF
    return compute_closure_skip(s, x, (size_t)-1);
}