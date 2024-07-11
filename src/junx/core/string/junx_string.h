#ifndef _junx_core_string_jstr_h_
#define _junx_core_string_jstr_h_

#include "../rtti/junx_object.h"

typedef struct _junx_string junx_string;

typedef struct
{
    junx_object_static  _base;
    junx_string* (*create) (const char* str, ju32_t len, ju32_t capa);
    jerr_t(*destroy) (junx_string** str);

    void* (*at)(junx_string* str, int idx);
} junx_string_static;


#endif