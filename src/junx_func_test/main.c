#include <stdio.h>
#include "junx/core/container/junx_vector.h"
#include "junx/core/rtti/junx_object.h"

void process_junx_objct(junx_object *  obj)
{


     printf("obj is %u\n", JUNX_METHODS(obj)->_type_id());
    
}

int main(int argc, char* argv[]) 
{
    long long num = 23;
    JUNX_UNUSED(argc);
    JUNX_UNUSED(argv);

    junx_vector* jvec = get_junx_vector_static()->create(5, sizeof(long long));
    if (NULL == jvec)
    {
        return -1;
    }


    process_junx_objct((junx_object*)(jvec));


    JUNX_DER_METHODS(jvec, junx_vector_static)->push_back(jvec, &num);

    get_junx_vector_static()->push_back(jvec, &num);
    num = 239082367234905;
    get_junx_vector_static()->push_back(jvec, &num);
    JUNX_METHODS(jvec)->_debug((junx_object*)jvec);

    get_junx_vector_static()->destroy(&jvec);
    return 0;
}