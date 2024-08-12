#ifndef _junx_object_h_
#define _junx_object_h_

#include "../../junx.h"

enum ju16_t
{
    jt_vector = 0,
    jt_string,
    jt_count,
};

typedef struct _junx_object {
    struct _junx_object_static* _meta;
} junx_object;



struct _junx_object_static
{
    unsigned short (*_type_id)();
    struct _junx_object* (*_alloc)();
    jerr_t (*_destory)(struct _junx_object** ego);
    struct _junx_object* (*_clone)(struct _junx_object* ego);
    ji32_t (*_compare)(struct _junx_object* ego, struct _junx_object* other);

    jerr_t(*_init)(struct _junx_object* obj);
    void (*_reset)(struct _junx_object* obj);
    void (*_debug)(struct _junx_object* ego);
};

typedef struct _junx_object_static junx_object_static;



#define JUNX_DER(x)    ((junx_object_static*) (get_##x##_static()))
#define JUNX_NULL_OBJECT    ((junx_object*)NULL)
#define JUNX_METHODS(x) (((junx_object*) (x))->_meta)
#define JUNX_DER_METHODS(x,der)   ((der*)(((junx_object*)(x))->_meta))



#endif
