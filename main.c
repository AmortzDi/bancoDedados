#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "parser.h"
#include "closure.h"
#include "mincover.h"
#include "keys.h"
#include "normalform.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso incorreto. Exemplo: fdtool closure-fds <arquivo.fds> --X <ATRIBUTOS>\n");
        return 1;
    }

    char *command = argv[1];
    char *filename = argv[2];

    // TODO: Implementar a função parse_file no parser.c
    // Schema schema = parse_file(filename);

    if (strcmp(command, "closure-fds") == 0) {
        if (argc != 5 || strcmp(argv[3], "--X") != 0) {
            fprintf(stderr, "Uso: fdtool closure-fds <arquivo.fds> --X <ATRIBUTOS>\n");
            return 1;
        }
        char *x_attrs = argv[4];
        // TODO: Chamar função do closure.c
        // calculate_closure(&schema, x_attrs);
        printf("Rodando closure-fds para %s\n", x_attrs);

    } else if (strcmp(command, "mincover") == 0) {
        // TODO: Chamar função do mincover.c
        printf("Rodando mincover\n");

    } else if (strcmp(command, "keys") == 0) {
        // TODO: Chamar função do keys.c
        printf("Rodando keys\n");

    } else if (strcmp(command, "normalform") == 0) {
        // TODO: Chamar função do normalform.c
        printf("Rodando normalform\n");

    } else {
        fprintf(stderr, "Comando desconhecido: %s\n", command);
        return 1;
    }

    // free(schema.fds); // Liberar memória alocada no parser
    return 0;
}