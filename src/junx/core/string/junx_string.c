#include "junx_string.h"

struct _junx_string
{
    char _fixed[JUNX_STC_PARAM_JSTR_DFL_CAPA];
    char* _dynamic;
    ju32_t _length;
    ju32_t _capacity;
};