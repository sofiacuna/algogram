#include "heap.h"

#include <stdio.h> // DESPUES BORRARLO SOLO PARA PRINTF
#include <stdlib.h>
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t


#define CAPACIDAD_INICIAL 32
#define POS_MAX 0
#define FACTOR_REDIMENSION 2
#define FACTOR_RELACION_CANT_CAP 4

struct heap {
    void **datos;
    size_t cant;
    size_t capacidad; 
    cmp_func_t cmp;
};

/* Funciones aux */
void swap(void **x, void **y) {
	void *aux;
	aux = *x;
	*x = *y;
	*y = aux;
}

size_t maximo(void *arreglo[], size_t largo, size_t pos_padre, cmp_func_t cmp){
    size_t pos_h_izq = 2 * pos_padre + 1, pos_h_der = 2 * pos_padre + 2;

    if (pos_h_izq >= largo)
        return pos_padre;

    if (pos_h_der >= largo) {
        int res_cmp_padre_hizq = cmp(arreglo[pos_padre], arreglo[pos_h_izq]);
        return res_cmp_padre_hizq < 0 ? pos_h_izq : pos_padre;
    }    

    int res_cmp_padre_hizq = cmp(arreglo[pos_padre], arreglo[pos_h_izq]),
        res_cmp_padre_hder = cmp(arreglo[pos_padre], arreglo[pos_h_der]);

    if (res_cmp_padre_hizq < 0 || res_cmp_padre_hder < 0) {
        int res_cmp_hijos = cmp(arreglo[pos_h_izq], arreglo[pos_h_der]);
        return res_cmp_hijos <= 0 ? pos_h_der : pos_h_izq;
    }

    return pos_padre;
}

void downheap(void *arreglo[], size_t largo, size_t pos_padre, cmp_func_t cmp) {
    if (pos_padre >= largo)
        return;

    size_t pos_max = maximo(arreglo, largo, pos_padre, cmp);

    if (pos_max != pos_padre) {
        swap(&arreglo[pos_max], &arreglo[pos_padre]);
        downheap(arreglo, largo, pos_max, cmp);
    }
}

void heapify(void *arreglo[], size_t n, cmp_func_t cmp) {
    for (int i = (int)n - 1; i >= 0; i--) 
        downheap(arreglo, n, (size_t)i, cmp);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    
    while (cant != 0) { 
        swap(&(elementos[0]), &(elementos[cant - 1]));
        cant--;
        downheap(elementos, cant, 0, cmp);
    }
}

void **crear_arreglo(size_t capacidad) {
	void **arreglo_nuevo = malloc(capacidad*sizeof(void *));
	if (!arreglo_nuevo)
		return NULL;

	for (size_t i = 0; i < capacidad; i++) 
		arreglo_nuevo[i] = NULL;
        
	return arreglo_nuevo;
}

/* Primitivas del heap */

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap_creado = malloc(sizeof(heap_t));
    if (!heap_creado) 
        return NULL;
     
    heap_creado->datos = crear_arreglo(CAPACIDAD_INICIAL); 
    if (!heap_creado->datos) {
        free(heap_creado);
        return NULL;
    }
    
    heap_creado->capacidad = CAPACIDAD_INICIAL;
    heap_creado->cant = 0;
    heap_creado->cmp = cmp;

    return heap_creado;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap)
        return NULL;
    heap->datos = crear_arreglo(n);
    for (size_t i = 0; i < n; i++) 
        heap->datos[i] = arreglo[i];        
    
    heapify(heap->datos, n, cmp);
    heap->cant = n;
    heap->capacidad = n;
    heap->cmp = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cant; i++) 
            destruir_elemento(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap_cantidad(heap) == 0;
}

void upheap(void **arreglo, size_t pos_hijo, cmp_func_t cmp) {
    if (pos_hijo == 0)
        return;

    size_t pos_padre = (pos_hijo - 1)/2;

    if (cmp(arreglo[pos_padre], arreglo[pos_hijo]) < 0) {
        swap(&arreglo[pos_padre], &arreglo[pos_hijo]);
        upheap(arreglo, pos_padre, cmp);
    }
    
}

bool heap_redimensionar(heap_t *heap, size_t capacidad_nueva) {
    void *datos_aux = realloc(heap->datos, capacidad_nueva * sizeof(void *));
	if (!datos_aux)
		return false;

	heap->datos = (void **) datos_aux;
	heap->capacidad = capacidad_nueva;
	return true;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap->cant == heap->capacidad) {
        if (!heap_redimensionar(heap, FACTOR_REDIMENSION * heap->capacidad))
            return false;
    }

    heap->datos[heap->cant] = elem;
    upheap(heap->datos, heap->cant, heap->cmp);
    heap->cant++;
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) 
        return NULL;
    return heap->datos[POS_MAX];
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap))
        return NULL; 
    void *dato_desencolado = heap->datos[0];
    swap(&heap->datos[0], &heap->datos[heap->cant - 1]);
    heap->cant--;

    downheap(heap->datos, heap->cant, 0, heap->cmp);

	if (heap->cant * FACTOR_RELACION_CANT_CAP  <= heap->capacidad &&
     heap->capacidad / FACTOR_REDIMENSION >= CAPACIDAD_INICIAL)
		heap_redimensionar(heap, heap->capacidad / FACTOR_REDIMENSION);
	
    return dato_desencolado;
}

