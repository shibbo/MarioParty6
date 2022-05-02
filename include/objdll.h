#ifndef __OBJDLL_H__
#define __OBJDLL_H__

#include "types.h"
#include "dolphin/os/OSModule.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* mDLLName;           // _0
    OSModuleHeader* mModuleInfo;      // _4
    void* bss;                      // _8
    u32 _C;
} omDLL;

typedef struct {
    const char* relName;    // _0
    const char* _4;
} omDLLListEntry;

void omDLLDBGOut(void);
void omDLLInit(omDLLListEntry *);
int omDLLStart(s16, s16);
void omDLLNumEnd(s16, s16);
void omDLLEnd(s16, s16);
void omDLLLink(omDLL **, s16, s16);
void omDLLUnlink(omDLL *, s16);
int omDLLSearch(s16);
omDLL* omDLLInfoGet(s16);
void omDLLInfoDump(OSModuleInfo *);
void omDLLHeaderDump(OSModuleHeader *);

static omDLLListEntry* omDLLFileList;
static omDLL* omDLLinfoTbl[0x14];

#ifdef __cplusplus
};
#endif

#endif