#include "validacion_ejecucion.h"

#include "post.h"
#include "feed.h"


void ejecutar_login(FILE *entrada, algogram_t *algogram) {
    char *nombre_usuario;
    size_t cant_bytes_leidos = 0;
    getline(&nombre_usuario, &cant_bytes_leidos, entrada);    
    error_t error;
    if (!login(algogram, nombre_usuario, &error)) 
        printf("%s\n", traducir_msj_error(error));
    else
        printf("Hola %s\n", nombre_usuario);
    free(nombre_usuario);
}

void ejecutar_logout(algogram_t *algogram) {
    error_t error;
    if (!logout(algogram, &error))
        printf("%s\n", traducir_msj_error(error));
    else
        printf("Adios\n"); 
}

void ejecutar_publicar(FILE *entrada, algogram_t *algogram) {
    char *publicacion;
    size_t cant_bytes_leidos = 0;
    getline(&publicacion, &cant_bytes_leidos , entrada); 

    error_t error;    
    if (!publicar(algogram, publicacion, error)
        printf("%s\n", traducir_msj_error(error));
    else
        printf("Post publicado\n");
    
    free(publicacion);
}

void ejecutar_ver_siguiente_feed(algogram_t *algogram) {
    error_t error;
    post_t *siguiente_feed = ver_siguiente_feed(algogram, &error); 
    if (!siguiente_feed) 
        printf("%s\n", traducir_msj_error(error));
    else {
        printf("Post ID %d\n", post_obtener_id(siguiente_feed));
        printf("%s dijo: %s\n", siguiente_feed->autor->nombre, post_obtener_publicacion(siguiente_feed)); 
        printf("Likes: %zu\n", post_obtener_cant_likes(siguiente_feed));
    }
}

void ejecutar_likear_post(FILE *entrada, algogram_t *algogram) {
    char *post_a_likear;
    size_t cant_bytes_leidos = 0;
    getline(&post_a_likear, &cant_bytes_leidos, entrada);
    size_t id_post;
    if (!validar_param_numero(post_a_likear, &id_post)) {
        printf("%s\n", MSJ_USR_ERR_PARAM_INVALIDO);
        free(post_a_likear);
        return;
    } 

    error_t error;    
    if (!likear_post(algogram, id_post, error) 
        printf("%s\n", traducir_msj_error(error));
    else
        printf("Post publicado\n");
    free(post_a_likear);
}  

void ejecutar_mostrar_likes(FILE *entrada, algogram_t *algogram) {
    char *post_mostrar_likes;
    size_t cant_bytes_leidos = 0;
    getline(&post_mostrar_likes, &cant_bytes_leidos, entrada);
    size_t id_post;
    if (!validar_param_numero(post_mostrar_likes, &id_post)) {
        printf("%s\n", MSJ_USR_ERR_PARAM_INVALIDO);
        free(post_mostrar_likes);
        return;
    } 

    error_t error; 
    abb_t *likes = mostrar_likes(algogram, id_post, &error); 
    if (!likes)
        printf("%s\n", traducir_msj_error(error)); 
    else {
        printf("El post tiene %d likes:\n", algogram->posts[id_post].cant_likes); 
        abb_imprimir(likes);    
    }
// El post tiene 4 likes:
//   chorch
//   chicho1994
//   eldiego
//   mondi
}
