#ifndef ABB_H
#define ABB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


typedef struct abb_iter abb_iter_t;

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

/* Crea el abb. 
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si la clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

/* Itera un abb con recorrido in order
	Pre: el abb fue creado.
	Post: Se itera el abb y se aplica una funcion pasada por parametro 
	a cada elemento. */

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

/* Crea un iterador 
	Pre: el abb fue creado.
	Post: devuelve un iterador que apunta a la raíz. En caso de estar vacío
	 apunta a NULL */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Mueve el iterador al siguiente elemento siguiendo recorrido in order. Devuelve true mientras no haya terminado,
	false si llegó al último elemento de la rama. 
	Pre: el iterador fue creado.
	Post: el iterador se encuentra en el siguiente elemento de la rama. */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Obtiene la clave actual. La clave no se puede modificar ni liberar. Si el elemento no está vacío devuelve su valor, 
	caso contrario devuelve NULL.
	Pre: el iterador fue creado.
	Post: se devuelve el valor del elemento actual de la lista cuando no está vacío. */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Comprueba si terminó la iteración. Devuelve true si no hay siguiente elemento al cual avanzar, false si se puede seguir iterando 
	Pre: el iterador fue creado.
	Post: devuelve true si el iterador llegó al final de la lista. */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador
	Pre: el iterador fue creado.
	Post: se eliminó el iterador. */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif