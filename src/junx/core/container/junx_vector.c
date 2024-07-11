#include <stdlib.h>
#include "junx_vector.h"
#include "../rtti/junx_object.h"

static jerr_t destroy(junx_vector** vecp);

struct _junx_vector
{
    struct _junx_object _base;
    char* _ptr;
    int _capacity;
    int _size;
    int _unit_size;
};

 static unsigned short jvec_type_id(junx_object* ego)
 {
     junx_vector* vecp = (junx_vector*)ego;
     if (vecp == NULL)
     {
         return jt_invalid;
     }
     return vecp->_base.m_type;
 }

 static junx_object* jvec_alloc()
 {
     junx_vector* vecp = (junx_vector*)malloc(sizeof(junx_vector));
     if (vecp == NULL)
     {
         return NULL;
     }
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

     vecp->_base.m_type = jt_vector;
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
    vecp->_base.m_type = jt_vector;
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
        return -3;
    }
    memcpy(vecp->_ptr+vecp->_size, ptr, vecp->_unit_size);
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

junx_vector_static g_junx_vector_static = {
    { jvec_type_id
      , jvec_alloc
      , jvec_destory
      , jvec_clone
      , jvec_compare
    , junx_vector_init
    , junx_vector_reset}
      , create, destroy, push_back,pop_back, at };

junx_vector_static* get_junx_vector_static()
{
    return &g_junx_vector_static;
}