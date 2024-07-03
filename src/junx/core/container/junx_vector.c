#include "junx_vector.h"
#include <stdlib.h>

typedef struct
{
    char* _ptr;
    int _capacity;
    int _size;
    int _unit_size;
} junx_vector_t;
junx_handle_t create (int capa, int unit_size) 
{
    junx_vector_t* vec = (junx_vector_t*)malloc(sizeof(junx_vector_t));
    if (vec == NULL)
    {
        return NULL;
    }
    vec->_ptr =(char*) calloc(capa, unit_size);
    if (vec->_ptr == NULL)
    {
        free(vec);
        return NULL;
    }
    vec->_capacity = capa;
    vec->_size = 0;
    vec->_unit_size = unit_size;
    return vec;
}
void destroy(junx_handle_t* pvec)
{
    junx_vector_t** vec = (junx_vector_t**) pvec;
    if (vec != NULL && *vec != NULL)
    {
        if ((*vec)->_ptr != NULL)
        {
            free((*vec)->_ptr);
            (*vec)->_ptr = NULL;
        }
        (*vec)->_capacity = 0;
        (*vec)->_size = 0;
        (*vec)->_unit_size = 0;
        free(*vec);
        *vec = NULL;
    }
}
int push_back(junx_handle_t pvec, void* ptr)
{
    if (pvec == NULL)
    {
        return -1;
    }
    if (ptr == NULL)
    {
        return -2;
    }
    
    junx_vector_t* vec = (junx_vector_t*)pvec;
    if (vec->_capacity - vec->_size < 1 )
    {
        return -3;
    }
    memcpy(vec->_ptr+vec->_size, ptr, vec->_unit_size);
    vec->_size = vec->_size + 1;
    return 0;
}

void* at(junx_handle_t pvec, int idx)
{
    if (pvec == NULL)
    {
        return NULL;
    }

    junx_vector_t* vec = (junx_vector_t*)pvec;
    if (idx < 0 || idx >= vec->_size)
    {
        return NULL;
    }

    return vec->_ptr + (idx * vec->_unit_size);
}
junx_vector_meta_t meta_vector = {create, destroy, push_back, at};