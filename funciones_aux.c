#include "funciones_aux.h"

#define BASE_NUMERO 10

bool validar_param_numero(char *param, size_t *num) {
    char *endPtr;
    
    *num = (size_t) strtol(param, &endPtr, BASE_NUMERO);

    if (endPtr && *endPtr)
        return false;

    return true;
}

//Revisar
// poner las estructuras de abb_nodo_t
void imprimir_arbol(abb_nodo_t *raiz, abb_nodo_t *padre) {
    if (!raiz)
        return;
    imprimir_arbol(raiz->izq, raiz);
    if (padre) {
        printf("%s\n", raiz->clave);
    }
    imprimir_arbol(raiz->der, raiz);
}

void abb_imprimir(abb_t *arbol) {
    imprimir_arbol(arbol->raiz, NULL);
}