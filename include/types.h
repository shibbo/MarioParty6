#pragma once

typedef unsigned long long  u64;
typedef   signed long long  s64;
typedef unsigned int        u32;
typedef   signed int        s32;
typedef unsigned short      u16;
typedef   signed short      s16;
typedef unsigned char       u8;
typedef   signed char       s8;

#ifndef BOOL
typedef int                 BOOL;
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef NULL
#ifdef  __cplusplus
#define NULL                0
#else
#define NULL                ((void *)0)
#endif
#endif