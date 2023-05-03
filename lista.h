#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

/* Crea una lista.
	Post: devuelve una nueva lista vacía. */
lista_t *lista_crear(void);

/* Devuelve true si la lista no tiene elementos, false en caso contrario. 
	Pre: la lista fue creada. */
bool lista_esta_vacia(const lista_t *lista);

/* Agrega un nuevo elemento a la lista en la primera posición. Devuelve false en caso de error. 
	Pre: la lista fue creada.
	Post: se agregó un nuevo elemento a la lista, dato se encuentra al principio de la lista. */
bool lista_insertar_primero(lista_t *lista, void *dato);

/* Agrega un nuevo elemento a la lista en la ultima posición. Devuelve false en caso de error. 
	Pre: la lista fue creada.
	Post: se agregó un nuevo elemento a la lista, dato se encuentra al final de la lista. */
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/* Saca el primer elemento de la lista. Si la lista tiene elementos,
	se quita el primero de la lista, y se devuelve su valor. Si está vacía devuelve NULL. 
	Pre: la lista fue creada.
	Post: se devolvió el valor del primer elemento anterior, la lista contiene un elemento menos,
	si la lista no estaba vacía. */ 
void *lista_borrar_primero(lista_t *lista);

/* Obtiene el valor del primer elemento de la lista. Si la lista tiene elementos,
	se devuelve el valor del primero, si está vacía devuelve NULL. 
	Pre: la lista fue creada.
	Post: se devolvió el primer elemento de la lista cuando no está vacía. */
void *lista_ver_primero(const lista_t *lista);

/* Obtiene el valor del último elemento de la lista. Si la lista tiene elementos,
	se devuelve el valor del último, si está vacía devuelve NULL. 
	Pre: la lista fue creada.
	Post: se devolvió el último elemento de la lista cuando no está vacía. */
void *lista_ver_ultimo(const lista_t* lista);

/* Devuelve el tamaño de la lista.
	Pre: la lista fue creada. */
size_t lista_largo(const lista_t *lista);

/* Destruye la lista. 
	Pre: la lista fue creada. 
	Post: se eliminaron todos los elementos de la lista. */
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/


/* Itera una lista
	Pre: la lista fue creada.
	Post: Se itero la lista y se aplica una funcion pasada por parametro 
	a cada elemento. */
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

/* Crea un iterador 
	Pre: la lista fue creada.
	Post: devuelve un iterador que apunta al primer elemento de la lista. */
lista_iter_t *lista_iter_crear(lista_t *lista);

/* Mueve el iterador al siguiente elemento de la lista. Devuelve true mientras no haya terminado la lista,
	false si llegó al último elemento de la lista. 
	Pre: el iterador fue creado.
	Post: el iterador se encuentra en el siguiente elemento. */
bool lista_iter_avanzar(lista_iter_t *iter);

/* Obtiene el valor del elemento actual. Si el elemento no está vacío devuelve su valor, 
	caso contrario devuelve NULL.
	Pre: el iterador fue creado.
	Post: se devuelve el valor del elemento actual de la lista cuando no está vacío. */
void *lista_iter_ver_actual(const lista_iter_t *iter);

/* Devuelve true si no hay siguiente elemento al cual avanzar, false si se puede seguir iterando 
	Pre: el iterador fue creado.
	Post: devuelve true si el iterador llegó al final de la lista. */
bool lista_iter_al_final(const lista_iter_t *iter);

/* Destruye el iterador
	Pre: el iterador fue creado.
	Post: se eliminaron todos los elementos del iterador. */
void lista_iter_destruir(lista_iter_t *iter);

/* Se inserta un elemento donde esta el iterador. Devuelve false en caso de error. 
	Pre: el iterador fue creado.
	Post: se insertó el elemento con el dato en la posición actual del iterador.
	El iterador ahora se encuentra en el elemento insertado. */
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/* Se quita el elemento de la lista donde está el iterador. Devuelve el dato del elemento borrado.
	Devuelve NULL si la lista está vacía. 
	Pre: el iterador fue creado.
	Post: se eliminó el elemento. El iterador se encuentra en el elemento siguiente al borrado. */  
void *lista_iter_borrar(lista_iter_t *iter);

#endif