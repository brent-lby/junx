#ifndef _junx_vector_h_
#define _junx_vector_h_

#include "../../junx.h"


typedef struct 
{
    junx_handle_t (*create) (int capa, int unit_size);
    void (*destroy) (junx_handle_t* vec);
    int (*push_back)(junx_handle_t vec,void* ptr);
    void* (*at)(junx_handle_t vec, int idx);
} junx_vector_meta_t;

extern junx_vector_meta_t meta_vector;
#endif // !_junx_vector_h_

