#define  _POSIX_C_SOURCE 200809L
#include "main.h"

int main(int argc, char *argv[]) {
    if (argc < CANT_MAX_ARGUMENTOS_VALIDOS) {
        printf("%s\n", MSJ_USR_ERR_CANT_ARG);
        return EXIT_FAILURE;
    }

    FILE *arch_usuarios = fopen(argv[1], "r");
    if (!arch_usuarios) {
       printf("%s\n", MSJ_USR_ERR_ARCHIVO);
       return EXIT_FAILURE;
    }

    algogram_t *algogram = algogram_crear(arch_usuarios, _cmp_afinidad);
    if (!algogram) {
        fprintf("%s\n", MSJ_USR_ERR_INICIO_ALGOGRAM);
        return EXIT_FAILURE;
    } 
    
    while (!EOF) {
        char *comando_leido;
        size_t cant_bytes_leidos = 0;
        getline(&comando_leido, &cant_bytes_leidos , stdin);
        
        if (strcmp(comando_leido, "login") == 0) {
            ejecutar_login(stdin, algogram);
            continue; 
        }
        if (strcmp(comando_leido, "logout") == 0) {
            ejecutar_logout(algogram);
            continue;
        }
        if (strcmp(comando_leido, "publicar") == 0) {
            ejecutar_publicar(stdin, algogram);
            continue;
        }
        if (strcmp(comando_leido, "ver_siguiente_feed") == 0) {
            ejecutar_ver_siguiente_feed(stdin, algogram);
            continue;
        }
        if (strcmp(comando_leido, "likear_post") == 0) {
            ejecutar_likear_post(stdin, algogram);
            continue;   
        }
        if (strcmp(comando_leido, "mostrar_likes") == 0) {
            ejecutar_mostrar_likes(stdin, algogram);
            continue;
        }
        free(comando_leido);   
        printf("%s\n", MSJ_USR_ERR_ARGUMENTO_INVALIDO);
    }
    algogram_destruir(algogram);
    
    return EXIT_SUCCESS;
}


