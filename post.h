#ifndef POST_H
#define POST_H

#include "usuario.h"
#include "abb.h"


typedef struct post post_t;

post_t *post_crear(const char *publicacion, usuario_t *autor, size_t id_post);

abb_t *post_obtener_likes(post_t *post); 

bool post_likear(post_t *post, usuario_t *usuario);
size_t post_obtener_cant_likes(post_t *post);

id_post_t post_obtener_id(post_t *post); 

const char *post_obtener_publicacion(post_t *post);

usuario_t *post_obtener_autor(post_t *post);

void post_destruir(post_t *post);

#endif