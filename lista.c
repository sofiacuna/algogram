#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
	void *dato;
	struct nodo *prox;
} nodo_t;

struct lista {
	struct nodo *prim;
	struct nodo *ult;
	size_t largo;
};

struct lista_iter {
	nodo_t *actual;
	nodo_t *anterior;
	lista_t *lista;
};

/* *****************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

nodo_t *nodo_crear(void *dato) {
	nodo_t *nodo_creado = malloc(sizeof(nodo_t));
	if (!nodo_creado)
		return NULL;

	nodo_creado->dato = dato;
	nodo_creado->prox = NULL;

	return nodo_creado;
}

void nodo_destruir(nodo_t *nodo) {
	free(nodo);
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
	lista_t *lista_nueva = malloc(sizeof(lista_t));
	if (!lista_nueva)
		return NULL;
	lista_nueva->prim = NULL;
	lista_nueva->ult = NULL;
	lista_nueva->largo = 0;

	return lista_nueva;
}


bool lista_esta_vacia(const lista_t *lista) {
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t *nodo_nuevo = nodo_crear(dato);
	if (!nodo_nuevo)
		return false;

	if (lista_esta_vacia(lista)) {
		lista->prim = lista->ult = nodo_nuevo;
		lista->largo++;
		return true;
	}

	nodo_nuevo->prox = lista->prim;
	lista->prim = nodo_nuevo;
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t *nodo_nuevo = nodo_crear(dato);
	if (!nodo_nuevo)
		return false;

	if (lista_esta_vacia(lista)) {
		lista->prim = lista->ult = nodo_nuevo;
		lista->largo++;
		return true;
	}

	lista->ult->prox = nodo_nuevo;
	lista->ult = nodo_nuevo;
	lista->largo++;

	return true;
}

void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista))
		return NULL;

	nodo_t *nodo_aux = lista->prim;
	void *valor_borrado = nodo_aux->dato;

	if (!lista->prim->prox) {
		lista->prim = lista->ult = NULL;
		lista->largo--;
		nodo_destruir(nodo_aux);
		return valor_borrado;
	}

	lista->prim = nodo_aux->prox;
	lista->largo--;	
	nodo_destruir(nodo_aux);
	
	return valor_borrado;
}

void *lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista))
		return NULL;

	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista))
		return NULL;

	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
	while (lista->prim) {
		nodo_t *nodo = lista->prim;
		lista->prim = lista->prim->prox;
		if (destruir_dato)
			destruir_dato(nodo->dato);
		nodo_destruir(nodo);
	}

	free(lista);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t *actual = lista->prim;

	while (actual) {
		if (!visitar(actual->dato, extra))
			return;
		actual = actual->prox;
	}
}


/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t *lista_iter_creado = malloc(sizeof(lista_iter_t));
	if (!lista_iter_creado)
		return NULL;

	lista_iter_creado->actual = lista->prim;
	lista_iter_creado->anterior = NULL;
	lista_iter_creado->lista = lista;

	return lista_iter_creado;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter))
		return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
	if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter))
		return NULL;

	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	if (!iter->actual)
		return true;
	return false;
}

void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	if (!iter->anterior) {
		if (!lista_insertar_primero(iter->lista, dato))
			return false;
		iter->actual = iter->lista->prim;
		return true;
	}

	nodo_t * nodo_nuevo = nodo_crear(dato);
	if (!nodo_nuevo)
		return false;

	if (lista_iter_al_final(iter))
		iter->lista->ult = nodo_nuevo;

	iter->anterior->prox = nodo_nuevo;
	nodo_nuevo->prox = iter->actual;
	iter->actual = nodo_nuevo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter) || lista_esta_vacia(iter->lista))
		return NULL;

	void *dato_aux;
	nodo_t *nodo_prox = iter->actual->prox;
	nodo_t *nodo_actual = iter->actual;

	if (!iter->anterior) {
		dato_aux = lista_borrar_primero(iter->lista);
		iter->actual = nodo_prox;
		return dato_aux;
	}
	
	dato_aux = iter->actual->dato;
	iter->actual = iter->actual->prox;
	iter->anterior->prox = iter->actual;

	if (lista_iter_al_final(iter)) 
		iter->lista->ult = iter->anterior;


	nodo_destruir(nodo_actual);
	iter->lista->largo--;

	return dato_aux;
}
