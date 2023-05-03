#ifndef FEED_H
#define FEED_H

#include "heap.h"
#include "post.h"
#includr "usuario.h"

typedef heap_t feed_t;

feed_item_t *feed_item_crear(id_usuario_t id_usuario, post_t *post) {

feed_t *feed_crear();

void feed_item_destruir(feed_item_t *feed_item);

bool feed_agregar_post(usuario_t *usuario, post_t *post);

post_t *feed_obtener_siguiente_post(usuario_t *usuario);

#endif