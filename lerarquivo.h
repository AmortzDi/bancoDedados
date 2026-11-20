#ifndef LERARQUIVO_H
#define LERARQUIVO_H

#include "types.h"
#include <stdbool.h>

void schema_init(Schema *s);
void schema_free(Schema *s);
bool schema_add_df(Schema *s, ConjuntoAtributo lhs, ConjuntoAtributo rhs);
bool parse_fds_file(const char *filename, Schema *schema);

#endif