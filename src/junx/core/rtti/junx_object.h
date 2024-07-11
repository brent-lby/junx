#ifndef _junx_object_h_
#define _junx_object_h_

#include "../../junx.h"

enum ju16_t
{
    jt_invalid = 0,
    jt_vector = 1,
    jt_count = 2,
};

struct _junx_object
{
    unsigned short  m_type;
};

typedef struct _junx_object junx_object;

#define JUNX_NULL_OBJECT    ((junx_object*)NULL)



struct _junx_object_static
{
    unsigned short (*_type_id)(junx_object* ego);
    junx_object* (*_alloc)();
    jerr_t (*_destory)(junx_object** ego);
    junx_object* (*_clone)(junx_object* ego);
    ji32_t (*_compare)(junx_object* ego, junx_object* other);

    jerr_t(*_init)(junx_object* obj);
    void (*_reset)(junx_object* obj);
};

typedef struct _junx_object_static junx_object_static;

#define JUNX_DER(x)    ((junx_object_static*) (get_##x##_static()))


junx_object_static* junx_base_methods(const junx_object* obj);

#define JUNX_METHODS(obj, cls)  ((cls*)junx_base_methods(obj))

#endif
