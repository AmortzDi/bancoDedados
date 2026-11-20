#include "lerarquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void schema_init(Schema *s) {
    s->universe = 0;
    s->dfs = NULL;
    s->count = 0;
    s->capacity = 0;
}

void schema_free(Schema *s) {
    free(s->dfs);
    s->dfs = NULL;
    s->count = 0;
    s->capacity = 0;
    s->universe = 0;
}

bool schema_add_df(Schema *s, ConjuntoAtributo lhs, ConjuntoAtributo rhs) {
    // Redimensiona o array dinamicamente dobrando a capacidade atual (iniciando em 16)
    if (s->count == s->capacity) {
        size_t new_cap = (s->capacity == 0) ? 16 : s->capacity * 2;
        DF *tmp = realloc(s->dfs, new_cap * sizeof(DF));

        if (!tmp) 
            return false;
        
        s->dfs = tmp;
        s->capacity = new_cap;
    }
    s->dfs[s->count].lhs = lhs;
    s->dfs[s->count].rhs = rhs;
    s->count++;
    return true;
}

// Carrega o arquivo inteiro na memória ignorando espaços e quebras de linha
// para simplificar a análise sintática (parsing) posterior. 
static char *read_file_no_spaces(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    if (fsize < 0) {
        fclose(fp);
        return NULL;
    }

    char *buf = malloc((size_t)fsize + 1);
    if (!buf) {
        fclose(fp);
        return NULL;
    }

    size_t idx = 0;
    int ch;
    // Cast para unsigned char evita comportamento indefinido em caracteres acentuados/negativos
    while ((ch = fgetc(fp)) != EOF) {
        if (!isspace((unsigned char)ch)) {
            buf[idx++] = (char)ch;
        }
    }

    buf[idx] = '\0';
    fclose(fp);
    return buf;
}

bool parse_fds_file(const char *filename, Schema *schema) {
    schema_init(schema);
    
    char *content = read_file_no_spaces(filename);
   
    if (!content) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", filename);
        return false;
    }

    // 1. Localiza e extrai o conjunto Universo: esperado formato U{A,B,C...}
    const char *u_pos = strchr(content, 'U');
   
    if (!u_pos) { 
        free(content); 
        return false; 
    }
   
    const char *open_u = strchr(u_pos, '{');
    const char *close_u = open_u ? strchr(open_u, '}') : NULL;
    
    if (!open_u || !close_u) { free(content); return false; }

    // Mapeia cada letra maiúscula encontrada para sua respectiva máscara de bits 
    for (const char *c = open_u + 1; c < close_u; c++) {
        if (*c >= 'A' && *c <= 'Z') {
            schema->universe |= attr_bit(*c);
        }
    }

    // 2. Localiza o bloco de Dependências Funcionais (após o fechamento de U): F{...}
    const char *f_pos = strchr(close_u + 1, 'F');
    
    if (!f_pos) { 
        free(content); return false; 
    }
   
    const char *open_f = strchr(f_pos, '{');
    const char *close_f = open_f ? strchr(open_f, '}') : NULL;

    if (!open_f || !close_f) {
        free(content);
        return false; 
    }

    // 3. Faz o parsing de cada DF dentro de F{...} (ex: AB->C, D->EF;)
    const char *c = open_f + 1;
    while (c < close_f) {
        if (*c == ',' || *c == ';') {
            c++;
            continue;
        }

        // Lê os atributos do lado esquerdo (LHS) até encontrar a seta ou um separador
        ConjuntoAtributo lhs = 0;
        while (c < close_f && *c != '-' && *c != ',' && *c != ';') {
            lhs |= attr_bit(*c);
            c++;
        }

        // Valida e avança sobre o operador "->"
        if (c + 1 < close_f && c[0] == '-' && c[1] == '>') {
            c += 2;
        } else {
            // Erro de sintaxe: DF malformada (sem '->')
            free(content);
            schema_free(schema);
            return false;
        }

        // Lê os atributos do lado direito (RHS) até o próximo separador
        ConjuntoAtributo rhs = 0;
        while (c < close_f && *c != ',' && *c != ';') {
            rhs |= attr_bit(*c);
            c++;
        }

        // Registra a DF apenas se ambos os lados tiverem pelo menos um atributo válido
        if (lhs != 0 && rhs != 0) {
            if (!schema_add_df(schema, lhs, rhs)) {
                free(content);
                schema_free(schema);
                return false;
            }
        }
    }

    free(content);
    return true;
}