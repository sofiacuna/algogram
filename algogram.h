#ifndef ALGOGRAM_H
#define ALGOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// TDA utilizados
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "vector.h"

#include "post.h"
#include "errors.h"
#include "feed.h"

typedef int64_t id_post_t;

typedef struct algogram algogram_t;

algogram_t *algogram_crear(FILE *usuarios, cmp_func_t cmp_afinidad);

bool login(algogram_t *algogram, const char *usuario, error_t *error);

bool logout(algogram_t *algogram, error_t *error);

bool publicar(algogram_t *algogram, const char *publicacion, error_t *error);

post_t *ver_siguiente_feed(algogram_t *algogram, error_t *error);

bool likear_post(algogram_t *algogram, id_post_t id_post, error_t *error);

const abb_t *mostrar_likes(algogram_t *algogram, id_post_t id_post, error_t *error);

void algogram_destruir(algogram_t *algogram); 


#endif
