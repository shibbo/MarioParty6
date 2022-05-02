#ifndef __MALLOC_H__
#define __MALLOC_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void* HuMemDirectMalloc(int, int);

void HuMemDirectFree(void *);

void HuMemDCFlushAll(void *);

#ifdef __cplusplus
}
#endif

#endif