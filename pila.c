#include "pila.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL 20
#define FACTOR_REDIM_AUMENTO 2
#define FACTOR_REDIM_REDUCCION 2
#define FACTOR_RELACION_CANT_CAP 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
	pila_t *pila_nueva;

	if ((pila_nueva = malloc(sizeof(pila_t))) == NULL)
		return NULL;

	if ((pila_nueva->datos = malloc(CAPACIDAD_INICIAL * sizeof(void *))) == NULL){
		free(pila_nueva);
		return NULL;
	}

	pila_nueva->cantidad = 0;
	pila_nueva->capacidad = CAPACIDAD_INICIAL;

	return pila_nueva;
}


void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}


bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
	void *datos_aux;

	if ((datos_aux = realloc(pila->datos, capacidad_nueva * sizeof(void *))) == NULL)
		return false;

	pila->datos = (void **) datos_aux;
	pila->capacidad = capacidad_nueva;
	return true;
}


bool pila_apilar(pila_t *pila, void *valor) {
	if (pila->cantidad == pila->capacidad) {
		if (!pila_redimensionar(pila, FACTOR_REDIM_AUMENTO * pila->capacidad))
			return false;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}


void *pila_ver_tope(const pila_t *pila) {
	if (pila_esta_vacia(pila))
		return NULL;

	return pila->datos[pila->cantidad - 1];
}


void *pila_desapilar(pila_t *pila) {
	if (pila_esta_vacia(pila))
		return NULL;

	if (pila->cantidad * FACTOR_RELACION_CANT_CAP  <= pila->capacidad && pila->capacidad / FACTOR_REDIM_REDUCCION >= CAPACIDAD_INICIAL)
		pila_redimensionar(pila, pila->capacidad / FACTOR_REDIM_REDUCCION);
	
	return pila->datos[--pila->cantidad];
}




