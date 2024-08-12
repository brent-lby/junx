#include <stdlib.h>
#include <stdio.h>
#include "junx_vector.h"
#include "../rtti/junx_object.h"

static jerr_t destroy(junx_vector** vecp);
static jerr_t junx_vector_expand(junx_vector* vec, ji32_t exp_num);
extern junx_vector_static g_junx_vector_static;

struct _junx_vector
{
    junx_vector_static* _meta;
    char* _ptr;
    int _capacity;
    int _size;
    int _unit_size;
};

 static unsigned short jvec_type_id()
 {
     return jt_vector;
 }

 static junx_object* jvec_alloc()
 {
     junx_vector* vecp = (junx_vector*)malloc(sizeof(junx_vector));
     if (vecp == NULL)
     {
         return NULL;
     }
     vecp->_meta = &g_junx_vector_static;
     vecp->_ptr = NULL;
     vecp->_capacity = 0;
     vecp->_size = 0;
     return (junx_object*) vecp;
 }

 static jerr_t jvec_destory(junx_object** ego)
 {
     junx_vector** vecp = (junx_vector**) ego;
     return destroy(vecp);
 }

 static junx_object* jvec_clone(junx_object* ego)
 {
     JUNX_UNUSED(ego);
     return NULL;
 }

 static ji32_t jvec_compare(junx_object* ego, junx_object* other)
 {
     JUNX_UNUSED(ego);
     JUNX_UNUSED(other);
     return 0;
 }

 static jerr_t junx_vector_init(junx_object* obj)
 {
     junx_vector* vecp = (junx_vector*)obj;
     if (vecp == NULL)
     {
         return -1;
     }

     vecp->_meta = &g_junx_vector_static;
     vecp->_ptr = NULL;
     vecp->_capacity = 0;
     vecp->_size = 0;
     vecp->_unit_size = 0;


     return 0;
 }

 static void junx_vector_reset(junx_object* obj)
 {
     junx_vector* vecp = (junx_vector*)obj;
     if (vecp == NULL)
     {
         return;
     }
     if ((vecp)->_ptr != NULL)
     {
         free((vecp)->_ptr);
         (vecp)->_ptr = NULL;
     }
     (vecp)->_capacity = 0;
     (vecp)->_size = 0;
     (vecp)->_unit_size = 0;

 }

 static void junx_vector_debug(struct _junx_object* ego)
 {
     ji32_t i = 0;
     ji32_t j = 0;
     junx_vector* _ego = (junx_vector*)ego;
     printf("vector @0x%p:\n",ego);
     printf("\tcapacity : %u:\n", _ego->_capacity);
     printf("\tsize : %u * %u = %u\n",_ego->_size, _ego->_unit_size, _ego->_size * _ego->_unit_size);
     for (i = 0; i < _ego->_size; i++)
     {
         printf("\telement %u : ", i);
         for (j = 0; j < _ego->_unit_size; j++)
         {
             printf("%02x ", (ju8_t)_ego->_ptr[i * _ego->_unit_size + j] );
         }
         printf("\n");
     }
 }


static junx_vector* create (int capa, int unit_size)
{
    junx_vector* vecp = (junx_vector*)malloc(sizeof(junx_vector));
    if (vecp == NULL)
    {
        return NULL;
    }
    vecp->_ptr =(char*) calloc(capa, unit_size);
    if (vecp->_ptr == NULL)
    {
        free(vecp);
        return NULL;
    }
    vecp->_meta = &g_junx_vector_static;
    vecp->_capacity = capa;
    vecp->_size = 0;
    vecp->_unit_size = unit_size;
    return vecp;
}
static jerr_t destroy(junx_vector** vecp)
{
    if (vecp != NULL && *vecp != NULL)
    {
        if ((*vecp)->_ptr != NULL)
        {
            free((*vecp)->_ptr);
            (*vecp)->_ptr = NULL;
        }
        (*vecp)->_capacity = 0;
        (*vecp)->_size = 0;
        (*vecp)->_unit_size = 0;
        free(*vecp);
        *vecp = NULL;
    }
    return 0;
}
static int push_back(junx_vector*vecp, void* ptr)
{
    if (vecp == NULL)
    {
        return -1;
    }
    if (ptr == NULL)
    {
        return -2;
    }
    
    if (vecp->_capacity - vecp->_size < 1 )
    {
        if (junx_vector_expand(vecp, 4) < 0)
        {
            return -3;
        }
    }
    memcpy(vecp->_ptr+vecp->_size*vecp->_unit_size, ptr, vecp->_unit_size);
    vecp->_size = vecp->_size + 1;
    return 0;
}

static int junx_vector_push_front(junx_vector* vecp, void* ptr)
{
    if (vecp == NULL)
    {
        return -1;
    }
    if (ptr == NULL)
    {
        return -2;
    }

    if (vecp->_capacity - vecp->_size < 1)
    {
        if (junx_vector_expand(vecp, 4) < 0)
        {
            return -3;
        }
    }
    memmove(vecp->_ptr + vecp->_unit_size, vecp->_ptr, vecp->_size * vecp->_unit_size);
    memcpy(vecp->_ptr, ptr, vecp->_unit_size);
    vecp->_size = vecp->_size + 1;
    return 0;
}

static int pop_back(junx_vector* vecp)
{
    if (vecp == NULL)
    {
        return -1;
    }

    if ( vecp->_size < 1)
    {
        return -3;
    }
    --vecp->_size;
    return 0;
}

static int junx_vector_pop_front(junx_vector* vecp)
{
    if (vecp == NULL)
    {
        return -1;
    }

    if (vecp->_size < 1)
    {
        return -3;
    }
    memmove(vecp->_ptr, vecp->_ptr + 1 * vecp->_unit_size, vecp->_size * vecp->_unit_size);
    --vecp->_size;
    return 0;
}

static void* at(junx_vector* vecp, int idx)
{
    if (vecp == NULL)
    {
        return NULL;
    }

    if (idx < 0 || idx >= vecp->_size)
    {
        return NULL;
    }

    return vecp->_ptr + (idx * vecp->_unit_size);
}

static jerr_t junx_vector_expand(junx_vector* vec, ji32_t exp_num) 
{
    if (vec == NULL)
    {
        return -1;
    }

    if (exp_num < 1)
    {
        return -2;
    }
    char* tmp = (char*)realloc(vec->_ptr, (exp_num + vec->_capacity) * vec->_unit_size);
    if (tmp == NULL)
    {
        return -3;
    }
    vec->_capacity = exp_num + vec->_capacity;
    vec->_ptr = tmp;
    return 0;
}

junx_vector_static g_junx_vector_static = {
    { jvec_type_id
      , jvec_alloc
      , jvec_destory
      , jvec_clone
      , jvec_compare
      , junx_vector_init
      , junx_vector_reset
      , junx_vector_debug
    }

      , create, destroy, push_back, junx_vector_push_front, pop_back, junx_vector_pop_front, at, junx_vector_expand };


junx_vector_static* get_junx_vector_static()
{
    return &g_junx_vector_static;
}