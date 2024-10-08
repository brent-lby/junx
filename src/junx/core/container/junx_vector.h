﻿#ifndef _junx_vector_h_
#define _junx_vector_h_

#include "../../junx.h"
#include "../rtti/junx_object.h"


typedef struct _junx_vector junx_vector;

typedef struct 
{
    junx_object_static           _base;
    junx_vector*(*create) (int capa, int unit_size);
    jerr_t (*destroy) ( junx_vector** vec);
    int (*push_back)( junx_vector* vec,void* ptr);
    int (*push_front)(junx_vector* vec, void* ptr);
    int (*pop_back) ( junx_vector* vec);
    int (*pop_front) (junx_vector* vec);
    void* (*at)( junx_vector* vec, int idx);
    jerr_t(*expand)(junx_vector* vec, ji32_t exp_num);
} junx_vector_static;


 junx_vector_static* get_junx_vector_static();



#endif // !_junx_vector_h_

