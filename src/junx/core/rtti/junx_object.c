#include "junx_object.h"

unsigned short  junx_type_id(junx_object obj) 
{
    struct _junx_object* p = (struct _junx_object*)obj;
    return p->m_type;
}
struct meta_object g_meta_object = { junx_type_id };
struct meta_object* get_meta_object()
{
    return &g_meta_object;
}
