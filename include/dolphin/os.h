#ifndef __OS_H__
#define __OS_H__

#ifdef __cplusplus
extern "C" {
#endif

void OSPanic(const char *, int, const char *, ...);
void OSReport(const char *, ...);

#ifdef __cplusplus
}
#endif

#endif