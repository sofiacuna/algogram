#ifndef VALIDACION_EJECUCION_H
#define VALIDACION_EJECUCION_H

#include "algogram.h"

#include "post.h"
#include "errors.h"
#include "feed.h"


void ejecutar_login(FILE *entrada, algogram_t *algogram);

void ejecutar_logout(algogram_t *algogram);

void ejecutar_publicar(FILE *entrada, algogram_t *algogram);

void ejecutar_ver_siguiente_feed(algogram_t *algogram);

void ejecutar_likear_post(FILE *entrada, algogram_t *algogram);

void ejecutar_mostrar_likes(FILE *entrada, algogram_t *algogram);

#endif