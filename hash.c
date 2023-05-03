#define _POSIX_C_SOURCE 200809L
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_INICIAL 32
#define FACTOR_REDIMENSION 2
#define FACTOR_OCUPACION_MIN 10


typedef enum estado {
	VACIO,
	OCUPADO, 
	BORRADO 
} estado_t;

typedef struct campo {
	char *clave;
	void *dato;
	estado_t estado;
} campo_t;

struct hash {
	campo_t *tabla;
	size_t capacidad;
	size_t cantidad_efectiva;
	size_t cantidad; // Tiene en cuenta los borrados 
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
	size_t indice;
	const hash_t *hash;
};

/* Función de hash extraída del libro El Lenguaje de Programación C
 Kernighan and Ritchie 2da Ed - Pags 128, 129 */

size_t funcion_hash(const char *s) {
	size_t hashval;

 	for (hashval = 0; *s != '\0'; s++)
 		hashval = (size_t)(*s) + 313131 * hashval;
 	return hashval;
} 

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

campo_t *crear_tabla(size_t capacidad) {
	campo_t *tabla_nueva = malloc(capacidad*sizeof(campo_t));
	if (!tabla_nueva)
		return NULL;

	for (size_t i = 0; i < capacidad; i++) {
		tabla_nueva[i].estado = VACIO;
		tabla_nueva[i].clave = NULL;
		tabla_nueva[i].dato = NULL;
	}	
	return tabla_nueva;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash_nuevo = malloc(sizeof(hash_t));
	if (!hash_nuevo)
		return NULL;

	hash_nuevo->capacidad = CAPACIDAD_INICIAL;
	hash_nuevo->cantidad_efectiva = 0;
	hash_nuevo->cantidad = 0;
	hash_nuevo->destruir_dato = NULL;
	
	hash_nuevo->tabla = crear_tabla(hash_nuevo->capacidad);
	if (!hash_nuevo->tabla) {
		free(hash_nuevo);
		return NULL;
	}

	if (destruir_dato)
		hash_nuevo->destruir_dato = destruir_dato;
	
	return hash_nuevo;
}

bool hash_redimensionar(hash_t *hash, size_t capacidad_nueva) { 
	campo_t *tabla_nueva = crear_tabla(capacidad_nueva);
	if (!tabla_nueva)
		return false;

	for (size_t j = 0; j < hash->capacidad; j++) {
		if (hash->tabla[j].estado == OCUPADO) {
			size_t posicion_nueva = funcion_hash(hash->tabla[j].clave) % capacidad_nueva;
			void *dato_aux = hash->tabla[j].dato;
			char *clave_aux = hash->tabla[j].clave;
				
			for (size_t i = 0; i < capacidad_nueva; i++) {
				size_t posicion_actual = (posicion_nueva + i) % capacidad_nueva;
				campo_t *campo_actual = &(tabla_nueva[posicion_actual]);

				if (campo_actual->estado == VACIO) {
					campo_actual->clave = clave_aux;
					campo_actual->estado = OCUPADO;			
					campo_actual->dato = dato_aux;	
					break;	
				}
			}
		}
	}

	free(hash->tabla);
	hash->tabla = tabla_nueva;
	hash->cantidad = hash->cantidad_efectiva;
	hash->capacidad = capacidad_nueva;

    return true;	
} 

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if (!clave) 
		return false;

	double factor_de_carga = (double)hash->cantidad / (double)hash->capacidad; 
	if (factor_de_carga >= 0.7) {
		if (!hash_redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) 
			return false;
	}

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		campo_t *campo_actual = &(hash->tabla[posicion_actual]);

		if (campo_actual->estado == VACIO) {
			campo_actual->clave = strdup(clave);
			hash->cantidad_efectiva++;
			hash->cantidad++;
			campo_actual->estado = OCUPADO;			
			campo_actual->dato = dato;		
			return true;
		}
		if (campo_actual->estado == OCUPADO) {
			if (strcmp(campo_actual->clave, clave) == 0) {
				if (hash->destruir_dato)
					(hash->destruir_dato)(campo_actual->dato);
				campo_actual->dato = dato;
				return true;
			}
		} 
	}
	return true;
}

void hash_destruir(hash_t *hash) {
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].estado == OCUPADO) {
			hash_borrar(hash, hash->tabla[i].clave);
			if (hash->destruir_dato)
				(hash->destruir_dato)(hash->tabla[i].dato);
		}
 	}
	free(hash->tabla);
	free(hash);
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		campo_t *campo_actual = &(hash->tabla[posicion_actual]);

		if (campo_actual->estado == VACIO )
			return NULL;
		if (campo_actual->estado == OCUPADO) {
			if (strcmp(campo_actual->clave, clave) == 0) 
					return campo_actual->dato;
		}
	}
	return NULL;  
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		campo_t *campo_actual = &(hash->tabla[posicion_actual]);

		if (campo_actual->estado == VACIO)
			return false;
		if (campo_actual->estado == OCUPADO) {
			if (strcmp(campo_actual->clave, clave) == 0) 
				return true;
		}
	}
	return false;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad_efectiva;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		campo_t *campo_actual = &(hash->tabla[posicion_actual]);

		if (campo_actual->estado == OCUPADO) { 
			if (strcmp(campo_actual->clave, clave) == 0) {
				free(campo_actual->clave);
				campo_actual->clave = NULL;

				hash->cantidad_efectiva--;
				campo_actual->estado = BORRADO;
				return campo_actual->dato;
			}
		}
	}

	if (hash->cantidad_efectiva * FACTOR_OCUPACION_MIN <= hash->capacidad && hash->capacidad / FACTOR_REDIMENSION >= CAPACIDAD_INICIAL )
		hash_redimensionar(hash, hash->capacidad / FACTOR_REDIMENSION);
	
	return NULL;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR DEL HASH
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t *iter = malloc(sizeof(hash_iter_t));
	if (!iter)
		return NULL;

	iter->indice = hash->capacidad;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].estado == OCUPADO){
			iter->indice = i;
			break;
		}
	}
	
	iter->hash = hash;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	for (size_t i = iter->indice + 1; i < iter->hash->capacidad; i++) {
		if (iter->hash->tabla[i].estado == OCUPADO){
			iter->indice = i;
			return true;
		}
	}
	iter->indice = iter->hash->capacidad;
	return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if (hash_iter_al_final(iter))
		return NULL;
	return iter->hash->tabla[iter->indice].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	if (iter->indice == iter->hash->capacidad)
		return true;
	return false;	
}

void hash_iter_destruir(hash_iter_t *iter) {
	free(iter);
}