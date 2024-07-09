#ifndef _junx_vector_h_
#define _junx_vector_h_

#include "../../junx.h"
#include "../rtti/junx_object.h"

typedef struct 
{
    junx_object (*create) (int capa, int unit_size);
    void (*destroy) (junx_object* vec);
    int (*push_back)(junx_object vec,void* ptr);
    int (*pop_back) (junx_object vec);
    void* (*at)(junx_object vec, int idx);
} junx_vector_meta_t;

 extern junx_vector_meta_t meta_vector;
 junx_vector_meta_t* get_meta_vector();


#endif // !_junx_vector_h_

