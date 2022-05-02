#ifndef FRAND_H
#define FRAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

inline u32 frandom(u32);
u32 frand(void);
f32 frandf(void);
s32 frandmod(s32);

static u32 frand_seed;

#ifdef __cplusplus
}
#endif


#endif // FRAND_H