#include "errors.h"

enum errores {
    ERROR_USUARIO_LOGGEADO, 
    ERROR_USUARIO_NO_EXISTE, 
    ERROR_USUARIO_NO_LOGGEADO,
    ERROR_VER_SIGUIENTE,
    ERROR_LIKEAR_POST,
    ERROR_MOSTRAR_LIKES
};

const char *traducir_msj_error(error_t error) {
    switch (error) {
        case ERROR_USUARIO_LOGGEADO:
            return "Error: Ya habia un usuario loggeado\n";
        case ERROR_USUARIO_NO_EXISTE:
            return "Error: usuario no existente\n";
        case ERROR_USUARIO_NO_LOGGEADO:
            return "Error: no habia usuario loggeado\n";
        case ERROR_VER_SIGUIENTE:
            return "Usuario no loggeado o no hay mas posts para ver\n";
        case ERROR_LIKEAR_POST:
            return "Error: Usuario no loggeado o Post inexistente\n";
        case ERROR_MOSTRAR_LIKES:
            return "Error: Post inexistente o sin likes\n"; 
        default:
            return "Error desconocido\n";
    }
}