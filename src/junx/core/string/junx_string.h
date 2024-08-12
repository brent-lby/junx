#ifndef _junx_core_string_jstr_h_
#define _junx_core_string_jstr_h_

#include "../../junx.h"
#include "../rtti/junx_object.h"


typedef struct _junx_string junx_string;


typedef struct
{
    junx_object_static           _base;
    junx_string* (*create) (const char* str,  ji32_t len, ji32_t capa);
    jerr_t(*destroy) (junx_string** ego);
    const char* (*c_str)(junx_string* ego);
    void* (*at)(junx_string* ego, int idx);
    jerr_t(*expand)(junx_string* ego, ji32_t exp_num);
} junx_string_static;

struct _junx_string
{
    junx_string_static* _meta;
    char* _ptr;
    int _capacity;
    int _size;
} ;

junx_string_static* get_junx_string_static();




#endif // !_junx_core_string_jstr_h_

