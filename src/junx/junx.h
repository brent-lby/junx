#ifndef _junx_h_
#define _junx_h_

#ifdef WINDOWS
#   include <windows.h>
#else
#   include <unistd.h>
#   include <sys/types>
#endif

#ifdef WINDOWS
#   define  JUNX_UNUSED(x)  do{(void)(x);}while (0)


#else
#   define  JUNX_UNUSED(x)  (void)x;

#endif

typedef void* junx_handle_t;
#define JUNX_NULL_HANDLE    ((junx_handle_t)NULL)














#endif

