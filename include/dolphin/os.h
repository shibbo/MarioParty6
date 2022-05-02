#ifndef __OS_H__
#define __OS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef s64 OSTime;

void OSPanic(const char *, int, const char *, ...);
void OSReport(const char *, ...);

OSTime OSGetTime(void);

#ifdef __cplusplus
}
#endif

#endif