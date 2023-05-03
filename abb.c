#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct abb_nodo {
	struct abb_nodo *izq;
	struct abb_nodo *der;
	char *clave;
	void *dato;

} abb_nodo_t;

struct abb {
	abb_nodo_t *raiz;
	abb_comparar_clave_t comparar_clave;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter {
	const abb_t *arbol;
	pila_t *pila;
};


/* *****************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

abb_nodo_t *abb_nodo_crear(const char *clave, void *dato) {
	abb_nodo_t *abb_nodo_creado = malloc(sizeof(abb_nodo_t));
	if (!abb_nodo_creado)
		return NULL;

	abb_nodo_creado->clave = strdup(clave);
	if (!abb_nodo_creado->clave) {
		free(abb_nodo_creado);
		return NULL;
	}
	abb_nodo_creado->dato = dato;
	abb_nodo_creado->izq = NULL;
	abb_nodo_creado->der = NULL;

	return abb_nodo_creado;
}

void abb_nodo_destruir(abb_nodo_t *abb_nodo) {
	free(abb_nodo->clave);
	free(abb_nodo);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t *abb_creado = malloc(sizeof(abb_t));
	if (!abb_creado)
		return NULL;

	abb_creado->raiz = NULL;
	abb_creado->comparar_clave = cmp;
	abb_creado->destruir_dato = destruir_dato;
	abb_creado->cantidad = 0;

	return abb_creado;
}

abb_nodo_t *buscar_padre_nodo(abb_nodo_t *raiz, abb_nodo_t *hijo, abb_comparar_clave_t comparar_clave) {
	int res_cmp = comparar_clave(hijo->clave, raiz->clave);

	if (raiz->der == hijo || raiz->izq == hijo) 
		return raiz;

	if (res_cmp < 0) 
		return buscar_padre_nodo(raiz->izq, hijo, comparar_clave);
	
	return buscar_padre_nodo(raiz->der, hijo, comparar_clave);		
} 

abb_nodo_t *buscar_padre_nodo_nuevo(abb_nodo_t *raiz, const char *clave, abb_comparar_clave_t comparar_clave) {
	int res_cmp = comparar_clave(clave, raiz->clave);
	if ((res_cmp < 0 && !raiz->izq) || (res_cmp > 0 && !raiz->der))
		return raiz;

	if (res_cmp < 0)  
		return buscar_padre_nodo_nuevo(raiz->izq, clave, comparar_clave);
	return buscar_padre_nodo_nuevo(raiz->der, clave, comparar_clave);	
}

abb_nodo_t *buscar_nodo(abb_nodo_t *raiz, const char *clave, abb_comparar_clave_t comparar_clave) {
	if (!raiz)
		return NULL;
	
	int res_cmp = comparar_clave(clave, raiz->clave);
	if (res_cmp == 0) 
		return raiz;
		
	if (res_cmp < 0)
		return buscar_nodo(raiz->izq, clave, comparar_clave);
	if (res_cmp > 0) 
		return buscar_nodo(raiz->der, clave, comparar_clave);

	return NULL;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	abb_nodo_t *nodo_encontrado = buscar_nodo(arbol->raiz, clave, arbol->comparar_clave);
	if (nodo_encontrado) {
		if (arbol->destruir_dato) 
			arbol->destruir_dato(nodo_encontrado->dato);
		nodo_encontrado->dato = dato;
		return true;
	}

	abb_nodo_t *abb_nodo_nuevo = abb_nodo_crear(clave, dato);
	if (!abb_nodo_nuevo)
		return false;
	arbol->cantidad++;

	if (!arbol->raiz) { 
		arbol->raiz = abb_nodo_nuevo;
		return true;
	}

	abb_nodo_t *nodo_padre = buscar_padre_nodo_nuevo(arbol->raiz, clave, arbol->comparar_clave);
	int cmp_padre = arbol->comparar_clave(clave, nodo_padre->clave);
	if (cmp_padre > 0) 
		nodo_padre->der = abb_nodo_nuevo;
	else
		nodo_padre->izq = abb_nodo_nuevo;
	
	return true;
}

abb_nodo_t *buscar_reemplazo(abb_nodo_t *nodo_borrar) {
	abb_nodo_t *nodo_reemplazo = nodo_borrar->der;

	while (nodo_reemplazo->izq) 
		nodo_reemplazo = nodo_reemplazo->izq;
	
	return nodo_reemplazo;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	if (!arbol->raiz)
		return NULL;

	abb_nodo_t *nodo_borrar = buscar_nodo(arbol->raiz, clave, arbol->comparar_clave);
	if (!nodo_borrar)
		return NULL;
	
	void *dato = nodo_borrar->dato;

	if (!nodo_borrar->der && !nodo_borrar->izq) {
		if (nodo_borrar != arbol->raiz) {		
			abb_nodo_t *nodo_padre = buscar_padre_nodo(arbol->raiz, nodo_borrar, arbol->comparar_clave);
			int res_cmp = arbol->comparar_clave(clave, nodo_padre->clave);
			if (res_cmp < 0)
				nodo_padre->izq = NULL;
			else 
				nodo_padre->der = NULL;
		}	
		else 
			arbol->raiz = NULL;
		
		abb_nodo_destruir(nodo_borrar);
		arbol->cantidad--;
		return dato;
	}

	if (!nodo_borrar->der || !nodo_borrar->izq) {
		abb_nodo_t *nodo_hijo = nodo_borrar->der ? nodo_borrar->der : nodo_borrar->izq;

		if (nodo_borrar != arbol->raiz) { 
			abb_nodo_t *nodo_padre = buscar_padre_nodo(arbol->raiz, nodo_borrar, arbol->comparar_clave);

			int res_cmp = arbol->comparar_clave(clave, nodo_padre->clave);
			if (res_cmp < 0)
				nodo_padre->izq = nodo_hijo;
			else 
				nodo_padre->der = nodo_hijo;
		}
		else  
			arbol->raiz = nodo_hijo;
		
		abb_nodo_destruir(nodo_borrar);
		arbol->cantidad--;
		return dato;
	}

	abb_nodo_t *nodo_reemplazo = buscar_reemplazo(nodo_borrar);
	abb_nodo_t *padre_nodo_reemplazo = buscar_padre_nodo(arbol->raiz, nodo_reemplazo, arbol->comparar_clave);

	char *clave_aux = nodo_reemplazo->clave;
    void *dato_aux = nodo_reemplazo->dato;

	nodo_reemplazo->clave = nodo_borrar->clave;
    nodo_borrar->clave = clave_aux;
    nodo_borrar->dato = dato_aux;
	
	if (nodo_borrar->der == nodo_reemplazo) {
		if (nodo_reemplazo->der)
			padre_nodo_reemplazo->der = nodo_reemplazo->der; 
		else 
			padre_nodo_reemplazo->der = NULL;	
		
		abb_nodo_destruir(nodo_reemplazo);
		arbol->cantidad--;
		return dato;	
	}

	if (nodo_reemplazo->der) 
		padre_nodo_reemplazo->izq = nodo_reemplazo->der; 
	else 
		padre_nodo_reemplazo->izq = NULL;
	
	abb_nodo_destruir(nodo_reemplazo);
	arbol->cantidad--;

	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	if (!arbol->raiz)
		return NULL;
		
	abb_nodo_t *abb_nodo = buscar_nodo(arbol->raiz, clave, arbol->comparar_clave);
	if (!abb_nodo) 
		return NULL;
		
	return abb_nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	if (!buscar_nodo(arbol->raiz, clave, arbol->comparar_clave))
		return false;

	return true;
}

size_t abb_cantidad(const abb_t *arbol) {
	return arbol->cantidad;
}

void destruir_arbol(abb_nodo_t *raiz, abb_destruir_dato_t destruir_dato) {
	if (!raiz) 
		return;
	destruir_arbol(raiz->izq, destruir_dato);
	destruir_arbol(raiz->der, destruir_dato);

	if (destruir_dato) 
		destruir_dato(raiz->dato);
	abb_nodo_destruir(raiz);
}

void abb_destruir(abb_t *arbol) {
	destruir_arbol(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/
bool _abb_in_order(abb_nodo_t *raiz, bool visitar(const char *, void *, void *), void *extra) {
	if (!raiz)
		return true;	
	if (!_abb_in_order(raiz->izq, visitar, extra))
		return false;
	if (!visitar(raiz->clave, raiz->dato, extra))
		return false;
	return _abb_in_order(raiz->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	_abb_in_order(arbol->raiz, visitar, extra);
}


/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t *abb_iter_creado = malloc(sizeof(abb_iter_t));
	if (!abb_iter_creado) 
		return NULL;

	abb_iter_creado->pila = pila_crear();
	if (!abb_iter_creado->pila) {
		free(abb_iter_creado);
		return NULL;
	}

	abb_iter_creado->arbol = arbol;

	abb_nodo_t *abb_nodo_izq = arbol->raiz;
	while (abb_nodo_izq) {
		if (!pila_apilar(abb_iter_creado->pila, abb_nodo_izq)) {
			pila_destruir(abb_iter_creado->pila);
			free(abb_iter_creado);
		}
		abb_nodo_izq = abb_nodo_izq->izq;
	}
	return abb_iter_creado;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter))
		return false;
	abb_nodo_t *abb_nodo_desapilado = pila_desapilar(iter->pila);
	abb_nodo_t * abb_nodo_apilado = abb_nodo_desapilado->der;
	while (abb_nodo_apilado) {
		if (!pila_apilar(iter->pila, abb_nodo_apilado))
			return false;
		abb_nodo_apilado = abb_nodo_apilado->izq;
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if (pila_esta_vacia(iter->pila))
		return NULL;
	abb_nodo_t *abb_nodo_actual = pila_ver_tope(iter->pila);
	return abb_nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}