#include "junx_string.h"
#include <stdio.h>

static jerr_t destroy(junx_string** str);
static jerr_t junx_string_expand(junx_string* str, ji32_t exp_num);
extern junx_string_static g_junx_string_static;



 static unsigned short jstr_type_id()
 {
     return jt_string;
 }

 static junx_object* jstr_alloc()
 {
     junx_string* str = (junx_string*)malloc(sizeof(junx_string));
     if (str == NULL)
     {
         return NULL;
     }
     str->_meta = &g_junx_string_static;
     str->_ptr = NULL;
     str->_capacity = 0;
     str->_size = 0;
     return (junx_object*) str;
 }

 static jerr_t jstr_destory(junx_object** ego)
 {
     junx_string** str = (junx_string**) ego;
     return destroy(str);
 }

 static junx_object* jstr_clone(junx_object* ego)
 {
     JUNX_UNUSED(ego);
     return NULL;
 }

 static ji32_t jstr_compare(junx_object* ego, junx_object* other)
 {
     JUNX_UNUSED(ego);
     JUNX_UNUSED(other);
     return 0;
 }

 static jerr_t junx_string_init(junx_object* obj)
 {
     junx_string* str = (junx_string*)obj;
     if (str == NULL)
     {
         return -1;
     }

     str->_meta = &g_junx_string_static;
     str->_ptr = NULL;
     str->_capacity = 0;
     str->_size = 0;
     return 0;
 }

 static void junx_string_reset(junx_object* obj)
 {
     junx_string* str = (junx_string*)obj;
     if (str == NULL)
     {
         return;
     }
     if ((str)->_ptr != NULL)
     {
         free((str)->_ptr);
         (str)->_ptr = NULL;
     }
     (str)->_capacity = 0;
     (str)->_size = 0;
 }

 static void junx_string_debug(struct _junx_object* ego)
 {
     junx_string* _ego = (junx_string*)ego;
     printf("string @0x%p:\n",ego);
     printf("\tcapacity : %u:\n", _ego->_capacity);
     printf("\tsize : %u \n",_ego->_size);
     printf("\t[%s]\n",_ego->_ptr);

 }


static junx_string* create (const char* cstr, ji32_t len, ji32_t capa)
{

    junx_string* str = (junx_string*)malloc(sizeof(junx_string));
    if (str == NULL)
    {
        return NULL;
    }    

    if (cstr == NULL) {
        junx_string_reset((junx_object*)str);
        return str;
    } 

    if (len < 0) {
        len = (ji32_t) strlen(cstr);
    }

    if (capa < len)
        capa = len;

    str->_ptr =(char*) malloc(capa);
    if (str->_ptr == NULL)
    {
        free(str);
        return NULL;
    }
    strncpy_s(str->_ptr, capa, cstr, len);
    str->_meta = &g_junx_string_static;
    str->_capacity = capa;
    str->_size = len;

    return str;
}
static jerr_t destroy(junx_string** str)
{
    if (str != NULL && *str != NULL)
    {
        if ((*str)->_ptr != NULL)
        {
            free((*str)->_ptr);
            (*str)->_ptr = NULL;
        }
        (*str)->_capacity = 0;
        (*str)->_size = 0;
        free(*str);
        *str = NULL;
    }
    return 0;
}

static void* at(junx_string* str, int idx)
{
    if (str == NULL)
    {
        return NULL;
    }

    if (idx < 0 || idx >= str->_size)
    {
        return NULL;
    }

    return str->_ptr + idx;
}

static jerr_t junx_string_expand(junx_string* str, ji32_t exp_num) 
{
    if (str == NULL)
    {
        return -1;
    }

    if (exp_num < 1)
    {
        return -2;
    }
    char* tmp = (char*)realloc(str->_ptr, (exp_num + str->_capacity));
    if (tmp == NULL)
    {
        return -3;
    }
    str->_capacity = exp_num + str->_capacity;
    str->_ptr = tmp;
    return 0;
}

const char* junx_string_c_str(junx_string* ego)
{
    return ego->_ptr;
}

junx_string_static g_junx_string_static = {
    { jstr_type_id
      , jstr_alloc
      , jstr_destory
      , jstr_clone
      , jstr_compare
      , junx_string_init
      , junx_string_reset
      , junx_string_debug
    }

      , create, destroy, junx_string_c_str, at, junx_string_expand };


junx_string_static* get_junx_string_static()
{
    return &g_junx_string_static;
}