#include "objdll.h"
#include "dvd.h"
#include "malloc.h"
#include "dolphin/os.h"
#include "MSL_C/string.h"

void omDLLDBGOut(void) {
    OSReport("DLL DBG OUT\n");
}

void omDLLInit(omDLLListEntry *pList) {
    int i;

    OSReport("DLL DBG OUT\n");

    for (i = 0; i < 0x14; i++) {
        omDLLinfoTbl[i] = NULL;
    }

    omDLLFileList = pList;
}

int omDLLStart(s16 a1, s16 a2) {
    int dll_idx;
    omDLL* dll;

    OSReport("DLLStart %d %d\n", a1, a2);
    dll_idx = omDLLSearch(a1);

    if (dll_idx > 0 && !a2) {
        dll = omDLLinfoTbl[dll_idx];
        OSReport("objdll>Already Loaded %s(%08x %08x)\n", dll->mDLLName, dll->mModuleInfo, dll->bss);
        omDLLInfoDump(&dll->mModuleInfo->info);
        omDLLHeaderDump(dll->mModuleInfo);
        HuMemDCFlushAll(memset(dll->bss, 0, dll->mModuleInfo->bssSize));
        dll->_C = dll->mModuleInfo->prolog();
        OSReport("objdll> %s prolog end\n", dll->mDLLName);
        return dll_idx;
    }
    else {
        for (dll_idx = 0; dll_idx < 0x14; dll_idx++) {
            if (omDLLinfoTbl[dll_idx] == NULL) {
                break;
            }
        }

        if (dll_idx == 0x14) {
            return -1;
            
        }
        
        omDLLLink(&omDLLinfoTbl[dll_idx], a1, 1);
        return dll_idx;
    }
}

void omDLLNumEnd(s16 dllno, s16 a2) {
    s16 dll_idx;

    if (dllno < 0) {
        OSReport("objdll>omDLLNumEnd Invalid dllno %d\n", dllno);
        
    }
    else {
        OSReport("objdll>omDLLNumEnd %d %d\n", dllno, a2);

        dll_idx = omDLLSearch(dllno);
        if (dll_idx < 0) {
            OSReport("objdll>omDLLNumEnd not found DLL No%d\n", dllno);
        }
        else {
            omDLLEnd(dll_idx, a2);
        }
    }
}

void omDLLEnd(s16 module_idx, s16 a2) {
    OSReport("objdll>omDLLEnd %d %d\n", module_idx, a2);

    if (a2 == 1) {
        OSReport("objdll>End DLL:%s\n", omDLLinfoTbl[module_idx]->mDLLName);
        omDLLUnlink(omDLLinfoTbl[module_idx], TRUE);
        omDLLinfoTbl[module_idx] = NULL;
    }
    else {
        omDLL* dll = omDLLinfoTbl[module_idx];
        OSReport("objdll>Call Epilog\n");
        dll->mModuleInfo->epilog();
        OSReport("objdll>End DLL stayed:%s\n", omDLLinfoTbl[module_idx]->mDLLName);
    }

    OSReport("objdll>End DLL finish\n");
}

void omDLLLink(omDLL **pDLL, s16 a2, s16 a3) {
    omDLLListEntry* dll_entry;
    omDLL* new_dll;

    dll_entry = &omDLLFileList[a2];
    OSReport("objdll>Link DLL:%s\n", dll_entry->relName);
    new_dll = HuMemDirectMalloc(0, sizeof(omDLL));
    *pDLL = new_dll;
    new_dll->mDLLName = dll_entry->relName;
    new_dll->mModuleInfo = HuDvdDataReadDirect(dll_entry->relName, 0);
    new_dll->bss = HuMemDirectMalloc(0, new_dll->mModuleInfo->bssSize);

    if (OSLink(&new_dll->mModuleInfo->info, new_dll->bss) != 1) {
        OSReport("objdll>++++++++++++++++ DLL Link Failed\n");
    }

    omDLLInfoDump(&new_dll->mModuleInfo->info);
    omDLLHeaderDump(new_dll->mModuleInfo);
    OSReport("objdll>LinkOK %08x %08x\n", new_dll->mModuleInfo, new_dll->bss);

    if (a3 == 1) {
        OSReport("objdll> %s prolog start\n", dll_entry->relName);
        new_dll->_C = new_dll->mModuleInfo->prolog();
        OSReport("objdll> %s prolog end\n", dll_entry->relName);
    }
}

void omDLLUnlink(omDLL *pDLL, s16 a2) {
    OSReport("odjdll>Unlink DLL:%s\n", pDLL->mDLLName);

    if (a2 == 1) {
        OSReport("objdll>Unlink DLL epilog\n");
        pDLL->mModuleInfo->epilog();
        OSReport("objdll>Unlink DLL epilog finish\n");
    }

    if (OSUnlink(&pDLL->mModuleInfo->info) != TRUE) {
        OSReport("objdll>+++++++++++++++++ DLL Unlink Failed\n");
    }

    HuMemDirectFree(pDLL->bss);
    HuMemDirectFree(pDLL->mModuleInfo);
    HuMemDirectFree(pDLL);
}

int omDLLSearch(s16 rel_idx) {
    omDLLListEntry* entry;
    int i;
    omDLL* cur_dll;

    entry = &omDLLFileList[rel_idx];
    OSReport("Search:%s\n", entry->relName);

    for (i = 0; i < 0x14; i++) {
        cur_dll = omDLLinfoTbl[i];

        if (cur_dll != NULL && !strcmp(cur_dll->mDLLName, entry->relName)) {
            OSReport("+++++++++++ Find%d: %s\n", i, cur_dll->mDLLName);
            return i;
        }
    }

    return -1;
}


// not used in Mario Party 6, but used in other Hudson games
omDLL* omDLLInfoGet(s16 dllno) {
    return omDLLinfoTbl[dllno];
}

void omDLLInfoDump(OSModuleInfo *pDLLInfo) {
    OSReport("===== DLL Module Info dump ====\n");
    OSReport("                   ID:0x%08x\n", pDLLInfo->id);
    OSReport("             LinkPrev:0x%08x\n", pDLLInfo->link.prev);
    OSReport("             LinkNext:0x%08x\n", pDLLInfo->link.next);
    OSReport("          Section num:%d\n", pDLLInfo->numSections);
    OSReport("Section info tbl ofst:0x%08x\n", pDLLInfo->sectionInfoOffset);
    OSReport("           nameOffset:0x%08x\n", pDLLInfo->nameOffset);
    OSReport("             nameSize:%d\n", pDLLInfo->nameSize);
    OSReport("              version:0x%08x\n", pDLLInfo->version);
    OSReport("===============================\n");
}

void omDLLHeaderDump(OSModuleHeader *pModuleHeader) {
    OSReport("==== DLL Module Header dump ====\n");
    OSReport("          bss Size:0x%08x\n", pModuleHeader->bssSize);
    OSReport("        rel Offset:0x%08x\n", pModuleHeader->relOffset);
    OSReport("        imp Offset:0x%08x\n", pModuleHeader->impOffset);
    OSReport("    prolog Section:%d\n", pModuleHeader->prologSection);
    OSReport("    epilog Section:%d\n", pModuleHeader->epilogSection);
    OSReport("unresolved Section:%d\n", pModuleHeader->unresolvedSection);
    OSReport("       prolog func:0x%08x\n", pModuleHeader->prolog);
    OSReport("       epilog func:0x%08x\n", pModuleHeader->epilog);
    OSReport("   unresolved func:0x%08x\n", pModuleHeader->unresolved);
    OSReport("================================\n");
}