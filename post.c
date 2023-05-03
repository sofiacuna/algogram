#include "post.h"

#include "abb.h"
#include "usuario.h"

typedef int64_t id_post_t;
typedef int64_t id_usuario_t;

struct post {
    id_post_t id_post; 
    usuario_t *autor;
    char *publicacion;
    size_t cant_likes;
    abb_t *likes;
};

post_t *post_crear(const char *publicacion, usuario_t *autor, size_t id_post) {
    post_t *post_creado = malloc(sizeof(post_t));
    if (!post_creado)
        return NULL;
    
    post_creado->publicacion = strdup(publicacion);
    if (!post_creado->publicacion) {
        free(post_creado);
        return NULL;
    }
    // Destruir datos le pusimos NULL.Ver si necesitamos despues 
    post_creado->likes = abb_crear(strcmp, NULL);
    if (!post_creado->likes) {
        free(post_creado->publicacion);
        free(post_creado);
        return NULL;        
    }
    post_creado->cant_likes = 0;

    post_creado->id_post = id_post;
    post_creado->autor = autor;
}

abb_t *post_obtener_likes(post_t *post) {
    return post->likes;
}

bool post_likear(post_t *post, usuario_t *usuario) {
    if (abb_pertenece(post->likes, usuario)) 
        return true;

    if (!abb_guardar(post->likes, usuario, NULL))
        return false;
    
    post->cant_likes++;
}

size_t post_obtener_cant_likes(post_t *post) {
    return post->cant_likes;
}

id_post_t post_obtener_id(post_t *post) {
    return post->id_post;
}

const char *post_obtener_publicacion(post_t *post) {
    return post->publicacion;
} 

usuario_t *post_obtener_autor(post_t *post) {
    return post->autor;
}

void post_destruir(post_t *post) {
    free(post->publicacion);
    abb_destruir(post->likes);
    free(post);
}
