#define _POSIX_C_SOURCE 200809L
#include "usuario.h"
#include "heap.h"


struct usuario {
    char *nombre;
    id_usuario_t id_usuario;
    heap_t *feed;
};

usuario_t *usuario_crear(const char *nombre, id_usuario_t id_usuario, cmp_func_t cmp_afinidad) {
    usuario_t *usuario_creado =  malloc(sizeof(usuario_t));
    if (!usuario_creado)
        return NULL;
    
    usuario_creado->feed = feed_crear(cmp_afinidad);
    if (!usuario_creado->feed) {
        free(usuario_creado);
        return NULL; 
    }

    usuario_creado->nombre = strdup(nombre);
    if (!usuario_creado->nombre) {
        heap_destruir(usuario_creado->feed, NULL);
        free(usuario_creado);
        return NULL;
    }
    
    usuario_creado->id_usuario = id_usuario;

    return usuario_creado;
}

char *usuario_obtener_nombre(usuario_t *usuario) {
    return usuario->nombre;
}

id_usuario_t usuario_obtener_id(usuario_t *usuario) {
    return usuario->id_usuario;
}

void usuario_destruir(usuario_t *usuario) {
    free(usuario->nombre);
    heap_destruir(usuario->feed, NULL);
    free(usuario);
}