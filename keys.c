#include "keys.h"
#include "closure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void keylist_init(KeyList *kl) {
    kl->keys = NULL;
    kl->count = 0;
    kl->capacity = 0;
}

void keylist_free(KeyList *kl) {
    free(kl->keys);
    kl->keys = NULL;
    kl->count = 0;
    kl->capacity = 0;
}

static bool keylist_add(KeyList *kl, ConjuntoAtributo key) {
    if (kl->count == kl->capacity) {
        size_t new_cap = (kl->capacity == 0) ? 16 : kl->capacity * 2;
        ConjuntoAtributo *tmp = realloc(kl->keys, new_cap * sizeof(ConjuntoAtributo));
        if (!tmp) return false;
        kl->keys = tmp;
        kl->capacity = new_cap;
    }
    kl->keys[kl->count++] = key;
    return true;
}

// Verifica se 'cand' é um superconjunto de alguma chave já encontrada (via bitmask).
// Garante a minimalidade: se já temos uma chave menor contida em 'cand', 'cand' não é chave candidata.
static bool is_superset_of_any(const KeyList *kl, ConjuntoAtributo cand) {
    for (size_t i = 0; i < kl->count; i++) {
        if ((cand & kl->keys[i]) == kl->keys[i]) {
            return true;
        }
    }
    return false;
}

// Gera combinações de 'remaining' atributos a partir de 'opt' (backtracking)
static void search_combinations(const Schema *s, KeyList *out,
                                const ConjuntoAtributo *opt, int n_opt,
                                int start, int remaining, ConjuntoAtributo current) {
    // Poda: se o conjunto atual já contém uma chave candidata, não expande
    if (is_superset_of_any(out, current)) {
        return;
    }

    if (remaining == 0) {
        if (compute_closure(s, current) == s->universe) {
            keylist_add(out, current);
        }
        return;
    }

    // O limite 'n_opt - remaining' evita recursões inúteis onde faltariam elementos para completar 'remaining'
    for (int i = start; i <= n_opt - remaining; i++) {
        search_combinations(s, out, opt, n_opt, i + 1, remaining - 1, current | opt[i]);
    }
}

// Comparador para qsort: converte bitmasks para strings (ex: "AB", "CD") para ordenar alfabeticamente
static int cmp_keys_lex(const void *a, const void *b) {
    ConjuntoAtributo ka = *(const ConjuntoAtributo *)a;
    ConjuntoAtributo kb = *(const ConjuntoAtributo *)b;
    char sa[27], sb[27];
    ConjuntoAtributo_to_str(ka, sa);
    ConjuntoAtributo_to_str(kb, sb);
    return strcmp(sa, sb);
}

void find_candidate_keys(const Schema *s, KeyList *out) {
    keylist_init(out);
    if (s->universe == 0) return;

    ConjuntoAtributo all_lhs = 0;
    ConjuntoAtributo effective_rhs = 0;

    // Classifica os atributos analisando apenas dependências não triviais (RHS fora do LHS)
    for (size_t i = 0; i < s->count; i++) {
        ConjuntoAtributo non_trivial_rhs = s->dfs[i].rhs & ~s->dfs[i].lhs;
        if (non_trivial_rhs != 0) {
            all_lhs |= s->dfs[i].lhs;
            effective_rhs |= non_trivial_rhs;
        }
    }

    // 1. Núcleo (Core): atributos que nunca são determinados por ninguém (fora do RHS)
    // precisam estar em TODAS as chaves candidatas.
    ConjuntoAtributo core = s->universe & ~effective_rhs;

    // Se apenas o núcleo já determina o universo todo, ele é a única chave candidata possível
    if (compute_closure(s, core) == s->universe) {
        keylist_add(out, core);
        return;
    }

    // 2. Opcionais: atributos que não estão no núcleo e aparecem no LHS (podem ajudar a fechar o universo).
    // Atributos que só aparecem no RHS (nunca no LHS) são descartados, pois não determinam nada novo.
    ConjuntoAtributo optional_mask = (s->universe & ~core) & all_lhs;
    ConjuntoAtributo opt[26];
    int n_opt = 0;
    for (int b = 0; b < 26; b++) {
        ConjuntoAtributo bit = 1u << b;
        if (optional_mask & bit) {
            opt[n_opt++] = bit;
        }
    }

    // 3. Busca em largura por tamanho (k = 1, 2, ...): garante que chaves menores sejam encontradas
    // antes das maiores, permitindo que a poda por superconjunto funcione corretamente.
    for (int k = 1; k <= n_opt; k++) {
        search_combinations(s, out, opt, n_opt, 0, k, core);
    }

    // Ordena as chaves lexicograficamente para saída determinística
    if (out->count > 1) {
        qsort(out->keys, out->count, sizeof(ConjuntoAtributo), cmp_keys_lex);
    }
}

void print_candidate_keys(const Schema *s) {
    KeyList kl;
    find_candidate_keys(s, &kl);
    char buf[27];
    for (size_t i = 0; i < kl.count; i++) {
        ConjuntoAtributo_to_str(kl.keys[i], buf);
        printf("%s\n", buf);
    }
    keylist_free(&kl);
}