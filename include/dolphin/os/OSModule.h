#ifndef __OSMODULE_H__
#define __OSMODULE_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 OSModuleID;
typedef struct OSModuleInfo OSModuleInfo;
typedef struct OSModuleHeader OSModuleHeader;
typedef struct OSModuleLink OSModuleLink;

struct OSModuleQueue {
    OSModuleInfo*   head;
    OSModuleInfo*   tail;
};

struct OSModuleLink {
    OSModuleInfo*   next;
    OSModuleInfo*   prev;
};

struct OSModuleInfo {
    OSModuleID id;
    OSModuleLink link;
    u32 numSections;
    u32 sectionInfoOffset;
    u32 nameOffset;
    u32 nameSize;
    u32 version;
};

struct OSModuleHeader {
    OSModuleInfo info;
    u32 bssSize;
    u32 relOffset;
    u32 impOffset;
    u32 impSize;
    u8 prologSection;
    u8 epilogSection;
    u8 unresolvedSection;
    u8 bssSection;
    u32 (*prolog)(void);
    u32 (*epilog)(void);
    //u32 prolog;
    //u32 epilog;
    u32 unresolved;
};

BOOL OSLink(OSModuleInfo *, void *);
BOOL OSUnlink(OSModuleInfo *);

#ifdef __cplusplus
}
#endif

#endif