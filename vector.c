#include "vector.h"


struct vector {
    size_t tam;
    void **datos;
};


void vector_destruir(vector_t *vector) {
    for (size_t i = 0; i < vector->tam; i++) {
        free(vector->datos[i]);
    }
    free(vector->datos);
    free(vector);
}

// bool vector_obtener(vector_t *vector, size_t pos, void *valor) {
//     if (pos > vector->tam - 1 || vector->tam == 0) {
//         return false;
//     }
//     *valor = *(vector->datos[pos]); 
//     return true;
// }

//quizas deberia ser asi :
void *vector_obtener(vector_t *vector, size_t pos) {
    if (pos > vector->tam - 1 || vector->tam == 0) 
        return NULL;
    
    return vector->datos[pos];
}


bool vector_guardar(vector_t *vector, size_t pos, void *valor) {
    if (pos > vector->tam - 1 || vector->tam == 0) {
        return false;
    }
    vector->datos[pos] = valor;  
    return true;
}

size_t vector_largo(vector_t *vector) {
    return vector->tam; 
}

vector_t *vector_crear(size_t tam) {
    vector_t *vector = malloc(sizeof(vector_t));

    if (vector == NULL) {
        return NULL;
    }
    vector->datos = malloc(tam * sizeof(int *));

    if (tam > 0 && vector->datos == NULL) {
        free(vector);
        return NULL;
    }
    vector->tam = tam;
    return vector;
}
//quizas habria que modificar esto tambien
bool vector_redimensionar(vector_t *vector, size_t tam_nuevo) {
    int *datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));

    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}
