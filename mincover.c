#include "mincover.h"
#include "lerarquivo.h"
#include "closure.h"

static bool has_df(const Schema *s, ConjuntoAtributo lhs, ConjuntoAtributo rhs) {
    for (size_t i = 0; i < s->count; i++) {
        if (s->dfs[i].lhs == lhs && s->dfs[i].rhs == rhs) {
            return true;
        }
    }
    return false;
}

void compute_mincover(const Schema *in, Schema *out) {
    schema_init(out);
    out->universe = in->universe;

    // Passo 1: Decompor o lado direito (RHS) em atributos individuais (RHS unitário)
    // Exemplo: A -> BC vira A -> B e A -> C.
    for (size_t i = 0; i < in->count; i++) {
        ConjuntoAtributo lhs = in->dfs[i].lhs;
        ConjuntoAtributo rhs = in->dfs[i].rhs;
        
        for (int b = 0; b < 26; b++) {
            ConjuntoAtributo bit = 1u << b;
            
            // Ignora DFs triviais (onde o atributo do RHS já faz parte do LHS) e duplicadas
            if ((rhs & bit) && !(lhs & bit)) {
                if (!has_df(out, lhs, bit)) {
                    schema_add_df(out, lhs, bit);
                }
            }
        }
    }

    // Passo 2: Remover atributos estranhos (redundantes) do lado esquerdo (LHS)
    // Testa remover cada atributo de LHS: se o fecho do que sobrou ainda determina RHS, o atributo era desnecessário.
    for (size_t i = 0; i < out->count; i++) {
        ConjuntoAtributo lhs = out->dfs[i].lhs;
        ConjuntoAtributo rhs = out->dfs[i].rhs;

        for (int b = 0; b < 26; b++) {
            ConjuntoAtributo bit = 1u << b;
            if (lhs & bit) {
                ConjuntoAtributo reduced = lhs & ~bit; // Cria cópia do LHS sem o bit atual
                if (reduced != 0) {
                    ConjuntoAtributo cl = compute_closure(out, reduced);
                    if ((cl & rhs) == rhs) {
                        lhs = reduced;
                        out->dfs[i].lhs = lhs; // Atualiza diretamente no array para influenciar as próximas iterações
                    }
                }
            }
        }
    }

    // Deduplicação in-place (sem alocar memória extra):
    // Reduzir o LHS pode fazer duas DFs ficarem iguais (ex: AB->C e AD->C viram ambas A->C).
    size_t write_idx = 0;
    for (size_t i = 0; i < out->count; i++) {
        bool dup = false;
        for (size_t j = 0; j < write_idx; j++) {
            if (out->dfs[j].lhs == out->dfs[i].lhs &&
                out->dfs[j].rhs == out->dfs[i].rhs) {
                dup = true;
                break;
            }
        }
        if (!dup) {
            out->dfs[write_idx++] = out->dfs[i];
        }
    }
    out->count = write_idx;

    // Passo 3: Remover dependências funcionais redundantes
    // Calcula o fecho de LHS ignorando a própria DF 'i'. Se ainda alcançar RHS, a DF é dedutível pelas outras.
    // Nota: 'i' só avança no 'else' porque, ao remover um item, os elementos seguintes recuam uma posição.
    for (size_t i = 0; i < out->count; ) {
        ConjuntoAtributo cl = compute_closure_skip(out, out->dfs[i].lhs, i);
        if ((cl & out->dfs[i].rhs) == out->dfs[i].rhs) {
            for (size_t j = i; j + 1 < out->count; j++) 
                out->dfs[j] = out->dfs[j + 1];
            out->count--;
        } else 
            i++;     
    }
}