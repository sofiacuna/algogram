#include "feed.h"

#include "heap.h"
#include "post.h"
#include "usuario.h"


typedef struct feed_item {
    id_usuario_t id_usuario; 
    post_t *post;
} feed_item_t;
// Segun el diagrama va a usuarios


// Estas primitivas viven aca.
int _cmp_afinidad(const void *a, const void *b);
int cmp_afinidad(const feed_item_t *a, const feed_item_t *b);


feed_item_t *feed_item_crear(id_usuario_t id_usuario, post_t *post) {
    feed_item_t *feed_item = malloc(sizeof(feed_item));
    if (!feed_item)
        return NULL;
    
    feed_item->id_usuario = id_usuario;
    feed_item->post = post;
    
    return feed_item;
}

feed_t *feed_crear() {
    feed_t *feed_creado = heap_crear(cmp_afinidad);
    if (!feed_creado) 
        return NULL; 
    return feed_creado;
}

void feed_item_destruir(feed_item_t *feed_item) {
    free(feed_item);
}

bool feed_agregar_post(usuario_t *usuario, post_t *post) {
    feed_item_t *item_feed = feed_item_crear(usuario_obtener_id(usuario), post);
    if (!item_feed) {
        post_destruir(post);
        return false;     
    }
    //usuario_obtener_feed() ?
    if (!heap_encolar(usuario->feed, item_feed) {
        feed_item_destruir(item_feed); 
        post_destruir(nuevo_post);
        return false;
    }
}

post_t *feed_obtener_siguiente_post(usuario_t *usuario) {
    feed_item_t  *item_feed = heap_desencolar(usuario->feed);
    return item_feed->post;
}

int _cmp_afinidad(const void *a, const void *b) {
    return cmp_afinidad(a, b);
}

int cmp_afinidad(const feed_item_t *a, const feed_item_t *b) {
    usuario_t *autor_post_a = post_obtener_autor(a->post);
    usuario_t *autor_post_b = post_obtener_autor(b->post);
    int distancia_a = abs(usuario_obtener_id(autor_post_a) - usuario_obtener_id(a));
    int distancia_b = abs(usuario_obtener_id(autor_post_b) - usuario_obtener_id(b));

    if (distancia_a < distancia_b)
        return 1; 
    if (distancia_a > distancia_b) 
        return -1;
    if (distancia_a == distancia_b) {
        if (post_obtener_id(a) < post_obtener_id(b)) 
            return 1;
        if (post_obtener_id(a) > post_obtener_id(b))
            return -1;
    }
}
// Estructura auxiliar para el heap tenga información

// meter al heap esta estructura 