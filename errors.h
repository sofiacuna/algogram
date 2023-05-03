#ifndef ERRORS_H
#define ERRORS_H

#define MSJ_USR_ERR_CANT_ARG "Error: cantidad de argumentos no válida"
#define MSJ_USR_ERR_ARCHIVO "Error: no se pudo abrir el archivo"
#define MSJ_USR_ERR_INICIO_ALGOGRAM "Error: no se pudo iniciar Algogram"
#define MSJ_USR_ERR_ARGUMENTO_INVALIDO "Error: argumento invalido"

#define MSJ_USR_ERR_MEMORIA "Error: Fallo de memoria dinámica"
//sospecho que esto esta mal declarado
typedef enum errores error_t;

const char *traducir_msj_error(error_t error); 


#endif


