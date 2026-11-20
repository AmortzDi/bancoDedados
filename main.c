#include <stdio.h>
#include <string.h>
#include "types.h"
#include "lerarquivo.h"
#include "closure.h"
#include "mincover.h"
#include "keys.h"
#include "normalform.h"

static void print_usage(void) {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "  ./fdtool closure --fds <arquivo.fds> --X <ATRIBUTOS>\n");
    fprintf(stderr, "  ./fdtool mincover --fds <arquivo.fds>\n");
    fprintf(stderr, "  ./fdtool keys --fds <arquivo.fds>\n");
    fprintf(stderr, "  ./fdtool normalform --fds <arquivo.fds>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *cmd = argv[1];
    const char *fds_file = NULL;
    const char *x_str = NULL;

    // Extrai os argumentos opcionais da linha de comando.
    // O pré-incremento (++i) consome o valor imediatamente seguinte à flag (ex: --fds <valor>).
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--fds") == 0 && i + 1 < argc) 
            fds_file = argv[++i];
        else if (strcmp(argv[i], "--X") == 0 && i + 1 < argc) 
            x_str = argv[++i];
        
    }

    if (!fds_file) {
        print_usage();
        return 1;
    }

    Schema schema;
    if (!parse_fds_file(fds_file, &schema)) {
        fprintf(stderr, "Falha ao ler o arquivo %s\n", fds_file);
        return 1;
    }

    // Roteamento dos subcomandos (buffers de 27 bytes comportam as 26 letras A-Z + terminador '\0')
    if (strcmp(cmd, "closure") == 0) {
        if (!x_str) {
            fprintf(stderr, "Faltou informar --X <ATRIBUTOS>\n");
            schema_free(&schema);
            return 1;
        }
        ConjuntoAtributo res = compute_closure(&schema, str_to_ConjuntoAtributo(x_str));
        char out[27];
        ConjuntoAtributo_to_str(res, out);
        printf("%s\n", out);
    } else if (strcmp(cmd, "mincover") == 0) {
        Schema mc;
        compute_mincover(&schema, &mc);
        char lhs_buf[27], rhs_buf[27];
        for (size_t i = 0; i < mc.count; i++) {
            ConjuntoAtributo_to_str(mc.dfs[i].lhs, lhs_buf);
            ConjuntoAtributo_to_str(mc.dfs[i].rhs, rhs_buf);
            printf("%s->%s\n", lhs_buf, rhs_buf);
        }
        schema_free(&mc);
    } else if (strcmp(cmd, "keys") == 0) 
        print_candidate_keys(&schema);
    else if (strcmp(cmd, "normalform") == 0) 
        check_normalform(&schema);
     else {
        print_usage();
        schema_free(&schema);
        return 1;
    }

    schema_free(&schema);
    return 0;
}