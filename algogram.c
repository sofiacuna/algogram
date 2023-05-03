#include "algogram.h"
#include "errors.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CAPACIDAD_INICIAL_POST 16

struct algogram {
    hash_t *usuarios;
    vector_t *posts;
    size_t cant_posts;
    usuario_t *usuario_loggeado; 
};

algogram_t *algogram_crear(FILE *usuarios, cmp_func_t cmp_afinidad) {
    algogram_t *algogram_creado = malloc(sizeof(algogram_t));
    if (!algogram_creado)
        return NULL;

    algogram_creado->usuarios = hash_crear(usuario_destruir);
    if (!(algogram_creado->usuarios)) {
        free(algogram_creado);
        return NULL;
    }
    size_t cant_bytes_leidos = 0;
    char *nombre_usuario;
    id_usuario_t id_usuario = 0;

    while (!EOF) {
        getline(&nombre_usuario, &cant_bytes_leidos, usuarios);
        usuario_t *usuario = usuario_crear(nombre_usuario, id_usuario, cmp_afinidad);
        if (!usuario) {
            free(nombre_usuario);
            hash_destruir(algogram_creado->usuarios);
            free(algogram_creado);
            return NULL;
        }
        if (!hash_guardar(algogram_creado->usuarios, nombre_usuario, usuario)) {
            free(nombre_usuario);
            usuario_destruir(usuario); 
            hash_destruir(algogram_creado->usuarios);
            free(algogram_creado);
            return NULL;
        }
        id_usuario++;
    }
    free(nombre_usuario);

    algogram_creado->posts = vector_crear(CAPACIDAD_INICIAL_POST);
    if (!algogram_creado->posts) {
        hash_destruir(algogram_creado->usuarios);
        free(algogram_creado);
        return NULL;
    }
    algogram_creado->cant_posts = 0;
    algogram_creado->usuario_loggeado = NULL; 

    return algogram_creado;
}

bool esta_loggeado(algogram_t *algogram, error_t *error) {
    if (algogram->usuario_loggeado) { 
        *error = ERROR_USUARIO_LOGGEADO;
        return true;
    }
    *error = ERROR_USUARIO_NO_LOGGEADO; 
    return false;
}

bool login(algogram_t *algogram, const char *usuario, error_t *error) {     
    if (esta_loggeado(algogram, error))
        return false;

    algogram->usuario_loggeado = hash_obtener(algogram->usuarios, usuario);

    if (!algogram->usuario_loggeado) {
        *error = ERROR_USUARIO_NO_EXISTE;
        return false;    
    } 
    return true;
}

bool logout(algogram_t *algogram, error_t *error) {
    if (!esta_loggeado(algogram, error)) 
        return false;

    algogram->usuario_loggeado = NULL;
    return true;
}

bool publicar(algogram_t *algogram, const char *publicacion, error_t *error) {
    if (!esta_loggeado(algogram, error))
        return false; 
    post_t *nuevo_post = post_crear(publicacion, algogram->usuario_loggeado, algogram->cant_posts);
    if (!nuevo_post) {
       return false; 
    }

    if (!vector_guardar(algogram->posts, algogram->cant_posts, nuevo_post)) {
        post_destruir(nuevo_post);
        return false;
    }
    
    algogram->cant_posts++;

    hash_iter_t *iter = hash_iter_crear(algogram->usuarios);
    if (!iter) {
        post_destruir(nuevo_post);
        return false;
    } 

    while (!hash_iter_al_final(iter)) {
        char *nombre = hash_iter_ver_actual(iter);
        usuario_t *usuario = hash_obtener(algogram->usuarios, nombre);
        if (strcmp(usuario_obtener_nombre(usuario), usuario_obtener_nombre(algogram->usuario_loggeado)) == 0)
            continue;
        if (!feed_agregar_post(usuario, nuevo_post)) {
            post_destruir(nuevo_post);
            hash_iter_destruir(iter);
            return false;
        }
                    
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
   
    return true;
}

post_t *ver_siguiente_feed(algogram_t *algogram, error_t *error) {
    post_t *siguiente_post = feed_ver_siguiente_post(algogram->usuario_loggeado);   
    if (!algogram->usuario_loggeado || !siguiente_post) {
        *error = ERROR_VER_SIGUIENTE;
        return NULL;
    }

    return siguiente_post->post;
}

bool likear_post(algogram_t *algogram, id_post_t id_post, error_t *error) {
    if (!algogram->usuario_loggeado || (id_post > algogram->cant_posts - 1)) {
        *error = ERROR_LIKEAR_POST;
        return false;
    }

    return post_likear(algogram->posts[id_post], algogram->usuario_loggeado);
}
//dudosa
//opciones: como el unico tda que "sabe" que los likes estan implementados con un abb es el tda post deberia
//hacer alguna primitiva que devuelva un algo de likes o que directamente los imprima ??
const abb_t *mostrar_likes(algogram_t *algogram, id_post_t id_post, error_t *error) {
    if (id_post > algogram->cant_posts) {
        *error = ERROR_MOSTRAR_LIKES;
        return NULL;
    }

    return post_obtener_likes(algogram->posts[id_post]);   
}

void algogram_destruir(algogram_t *algogram) {
    for (size_t i = 0; i < algogram->cant_posts; i++)
        post_destruir(algogram->posts[i]);
        
    vector_destruir(algogram->posts);
    hash_destruir(algogram->usuarios);
    free(algogram);
}