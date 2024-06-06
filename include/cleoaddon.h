#ifndef _CLEO_ADDON_H
#define _CLEO_ADDON_H

#include <stdint.h>
#include <string.h>
#include <list>

#define CLEO_RegisterOpcode(x, h) cleo->RegisterOpcode(x, h); cleo->RegisterOpcodeFunction(#h, h)
#define CLEO_Fn(h) void h (void *handle, uint32_t *ip, uint16_t opcode, const char *name)

struct ScriptAddonInfo
{
    ScriptAddonInfo()
    {
        workDir.clear();
        childThreads.clear();
        parentThread = NULL;
        scmFuncId = 0;
        isCustom = false;
        debugMode = false;
        enableThreadSaving = false;
    }

    // GetInterfaceVersion() == 1
    std::string workDir;
    std::list<void*> childThreads;
    void* parentThread;
    
    uint16_t scmFuncId : 10; // [0-1024] - 2^10
    uint16_t isCustom : 1; // [0-1]
    uint16_t debugMode : 1; // [0-1]
    uint16_t enableThreadSaving : 1; // [0-1]
};


struct cleo_addon_ifs_t
{
    uint32_t        (*GetInterfaceVersion)(); // why not

                    // Interface ver 1
    char*           (*ReadString)(void* handle, char* buf, size_t size);
    void            (*WriteString)(void* handle, const char* buf);
    uint32_t        (*GetStringMaxSize)(void* handle);
    char*           (*GetStringPointer)(void* handle);
    int             (*FormatString)(void* handle, char *str, size_t len, const char *format);
    void            (*AsciiToGXTChar)(const char* src, uint16_t* dst);
    const char*     (*GXTCharToAscii)(const uint16_t* src, uint8_t start);
    int             (*ValueForGame)(int for3, int forvc, int forsa, int forlcs, int forvcs);
    void            (*ThreadJump)(void* handle, int offset);
    void            (*SkipUnusedParameters)(void *handle); // different from SkipParamListEnd
    uint8_t*        (*GetScriptPC)(void *handle); // this contains a real code IN ALL GAMES
    void            (*PushStack)(void *handle);
    void            (*PopStack)(void *handle);
    bool&           (*GetCond)(void *handle);
    bool&           (*GetNotFlag)(void *handle);
    uint16_t&       (*GetLogicalOp)(void *handle);
    void            (*Interrupt)(void *handle);
    void            (*Skip1Byte)(void* handle);
    void            (*Skip2Bytes)(void* handle);
    void            (*Skip4Bytes)(void* handle);
    void            (*SkipBytes)(void* handle, uint32_t bytes);
    uint8_t         (*Read1Byte)(void* handle);
    uint16_t        (*Read2Bytes)(void* handle);
    uint32_t        (*Read4Bytes)(void* handle);
    uint8_t         (*Read1Byte_NoSkip)(void* handle);
    uint16_t        (*Read2Bytes_NoSkip)(void* handle);
    uint32_t        (*Read4Bytes_NoSkip)(void* handle);
    int*            (*GetLocalVars)(void* handle);
    uint8_t*&       (*GetPC)(void *handle); // this contains a real code ONLY IN GTASA (maybe LCS too), used for thread jumping &etc
    void            (*SkipOpcodeParameters)(void* handle, int count);
    int             (*GetVarArgCount)(void* handle);
    ScriptAddonInfo&(*GetAddonInfo)(void* handle);
    void            (*UpdateCompareFlag)(void* handle, uint8_t flag);
    bool            (*IsOpcodeAlreadyExists)(uint16_t opcode);
    bool            (*IsValidScriptHandle)(void* handle);
    std::string     (*ResolvePath)(void* handle, const char* path, const char* customWorkDir);
    void            (*AddGXTLabel)(const char* gxtLabel, const char* text);
};

#endif // _CLEO_ADDON_H