#include <stdio.h>
#include "junx/core/container/junx_vector.h"

void process_junx_objct(junx_object obj)
{
    printf("type is %u\n", get_meta_object()->type_id(obj) );
}

int main(int argc, char* argv[]) 
{
    long long num = 23;
    JUNX_UNUSED(argc);
    JUNX_UNUSED(argv);

    junx_object jvec = get_meta_vector()->create(5, sizeof(long long));
    if (JUNX_NULL_OBJECT == jvec)
    {
        return -1;
    }


    process_junx_objct(jvec);
    get_meta_vector()->push_back(jvec, &num);
    get_meta_vector()->destroy(&jvec);
    return 0;
}