#include "normalform.h"
#include "lerarquivo.h"
#include "closure.h"
#include "keys.h"
#include <stdio.h>

static bool has_df_local(const Schema *s, ConjuntoAtributo lhs, ConjuntoAtributo rhs) {
    for (size_t i = 0; i < s->count; i++) 
        if (s->dfs[i].lhs == lhs && s->dfs[i].rhs == rhs) 
            return true;
    
    return false;
}

void check_normalform(const Schema *s) {
    // 1. Encontrar chaves candidatas e unir todos os bits para formar o conjunto de atributos primos
    // (atributos que pertencem a pelo menos uma chave candidata)   
    KeyList kl;
    find_candidate_keys(s, &kl);

    
    ConjuntoAtributo prime_attrs = 0;
    for (size_t i = 0; i < kl.count; i++) 
        prime_attrs |= kl.keys[i];
    

    // 2. Decompor o lado direito (RHS) em atributos individuais (decomposição)
    // Isso facilita a validação da 3FN, onde a condição de "atributo primo" é avaliada por atributo isolado.
    Schema unit;
    schema_init(&unit);
    unit.universe = s->universe;

    for (size_t i = 0; i < s->count; i++) {
        ConjuntoAtributo lhs = s->dfs[i].lhs;
        ConjuntoAtributo rhs = s->dfs[i].rhs;
        for (int b = 0; b < 26; b++) {
            ConjuntoAtributo bit = 1u << b;
            if ((rhs & bit) && !(lhs & bit)) {
                // Mantém apenas se o bit estiver no RHS e NÃO estiver no LHS (ignora DFs triviais como AB -> A)
                if (!has_df_local(&unit, lhs, bit)) 
                    schema_add_df(&unit, lhs, bit);  
            }
        }
    }

    // 3. Avaliar regras das formas normais:
    // - BCNF: LHS precisa ser superchave (fecho de LHS == Universo)
    // - 3FN:  LHS precisa ser superchave OU RHS precisa ser atributo primo
    bool bcnf_ok = true;
    bool nf3_ok = true;

    for (size_t i = 0; i < unit.count; i++) {
        ConjuntoAtributo lhs = unit.dfs[i].lhs;
        ConjuntoAtributo rhs = unit.dfs[i].rhs;

        bool is_superkey = (compute_closure(s, lhs) == s->universe);
        if (!is_superkey) {
            bcnf_ok = false;

            // Verifica se o bit do RHS está contido na máscara de atributos primos
            bool rhs_is_prime = ((prime_attrs & rhs) == rhs);
            if (!rhs_is_prime) 
                nf3_ok = false;
            
        }
    }

    // 4. Imprimir cabeçalho de status
    printf("BCNF: %s\n", bcnf_ok ? "OK" : "VIOLATIONS");
    printf("3NF: %s\n", nf3_ok ? "OK" : "VIOLATIONS");

    // 5. Detalhar violações de BCNF
    char lhs_buf[27], rhs_buf[27];
    for (size_t i = 0; i < unit.count; i++) {
        ConjuntoAtributo lhs = unit.dfs[i].lhs;
        ConjuntoAtributo rhs = unit.dfs[i].rhs;

        bool is_superkey = (compute_closure(s, lhs) == s->universe);
        if (!is_superkey) {
            ConjuntoAtributo_to_str(lhs, lhs_buf);
            ConjuntoAtributo_to_str(rhs, rhs_buf);
            printf("VIOLATION BCNF: %s->%s (%s not superkey)\n",
                   lhs_buf, rhs_buf, lhs_buf);
        }
    }

    // 6. Detalhar violações de 3FN
    for (size_t i = 0; i < unit.count; i++) {
        ConjuntoAtributo lhs = unit.dfs[i].lhs;
        ConjuntoAtributo rhs = unit.dfs[i].rhs;

        bool is_superkey = (compute_closure(s, lhs) == s->universe);
        bool rhs_is_prime = ((prime_attrs & rhs) == rhs);
        if (!is_superkey && !rhs_is_prime) {
            ConjuntoAtributo_to_str(lhs, lhs_buf);
            ConjuntoAtributo_to_str(rhs, rhs_buf);
            printf("VIOLATION 3NF: %s->%s (%s not prime, %s not superkey)\n",
                   lhs_buf, rhs_buf, rhs_buf, lhs_buf);
        }
    }

    schema_free(&unit);
    keylist_free(&kl);
}