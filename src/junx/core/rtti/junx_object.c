#include "junx_object.h"


#include "../container/junx_vector.h"

extern junx_vector_static g_junx_vector_static;

static junx_object_static* s_junx_objects_static[2] = {
    NULL,
    (junx_object_static*) &g_junx_vector_static,
};


static ju16_t s_junx_objects_count = sizeof(s_junx_objects_static) / sizeof(junx_object*);

