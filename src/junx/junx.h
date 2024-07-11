#ifndef _junx_h_
#define _junx_h_

#ifdef WINDOWS
#   include <windows.h>
#else
#   include <unistd.h>
#   include <sys/types>
#endif

#include "static_params.h"

#ifdef WINDOWS
#   define  JUNX_UNUSED(x)  do{(void)(x);}while (0)


#else
#   define  JUNX_UNUSED(x)  (void)x;

#endif

typedef char ji8_t;
typedef unsigned char ju8_t;
typedef short ji16_t;
typedef unsigned short ju16_t;
typedef int ji32_t;
typedef unsigned int ju32_t;
typedef long ji64_t;
typedef long long ju64_t;
typedef float jf32_t;
typedef double jf64_t;

typedef size_t jsz_t;
typedef int    jerr_t;


















#endif

