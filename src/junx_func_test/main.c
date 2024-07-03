#include "junx/core/container/junx_vector.h"
 
int main(int argc, char* argv[]) 
{
    JUNX_UNUSED(argc);
    JUNX_UNUSED(argv);

    junx_handle_t jvec = meta_vector.create(5, sizeof(long long));
    if (JUNX_NULL_HANDLE == jvec)
    {
        return -1;
    }
    meta_vector.destroy(&jvec);
    return 0;
}