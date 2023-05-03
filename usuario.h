#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "heap.h"

typedef int64_t id_usuario_t;
typedef struct usuario usuario_t;

usuario_t *usuario_crear(const char *nombre, id_usuario_t id_usuario, cmp_func_t cmp_afinidad);

char *usuario_obtener_nombre(usuario_t *usuario);

id_usuario_t usuario_obtener_id(usuario_t *usuario);

void usuario_destruir(usuario_t *usuario);

#endif