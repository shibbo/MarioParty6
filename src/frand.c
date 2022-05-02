#include "frand.h"
#include "dolphin/os.h"

u32 frandom(u32 a1) {
    u32 div;
    u32 mod;
    u32 rand;

    if (a1 == 0) {
        a1 = OSGetTime();
        a1 = a1 ^ 0xD826BC89;
    }

    div = a1 / 0x1F31D;
    mod = a1 - div * 0x1F31D;
    a1 = 0xB14 * div;
    a1 = a1 - (0x41A7 * mod);
    return a1;
}

u32 frand() {
    return frand_seed = frandom(frand_seed);
}

f32 frandf() {
    u32 val = frand();
    f32 ret;

    val &= 0x7FFFFFFF;
    ret = val / 2147483648.0f;
    return ret;
}

s32 frandmod(s32 a1) {
    s32 ret;
    frand();
    ret = (frand_seed & 0x7FFFFFFF) % a1;
    return ret;
}