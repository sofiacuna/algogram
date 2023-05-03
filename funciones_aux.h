#ifndef FUNCIONES_AUX_H
#define FUNCIONES_AUX_H

#include <stdlib.h>
#include <stdbool.h>

bool validar_param_numero(char *param, size_t *num);
void imprimir_arbol(abb_nodo_t *raiz, abb_nodo_t *padre);
void abb_imprimir(abb_t *arbol);

#endif
