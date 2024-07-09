#ifndef _junx_object_h_
#define _junx_object_h_

#include "../../junx.h"


typedef void* junx_object;

#define JUNX_NULL_OBJECT    ((junx_object)NULL)
struct _junx_object
{
    unsigned short  m_type;
};

enum
{
    jt_invalid = 0,
    jt_vector = 1,
};

struct meta_object
{
    unsigned short  (*type_id)();
};
struct meta_object* get_meta_object();

#endif
