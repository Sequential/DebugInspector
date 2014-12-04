#pragma once
#pragma unmanaged

#include <windows.h>
#if defined(_MSC_VER)
#pragma warning(disable:4245)   // signed/unsigned mismatch
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4201)   // nonstandard extension used : nameless struct/union
#pragma warning(disable:4127)   // conditional expression is constant
#pragma warning(disable:4430)   // missing type specifier: C++ doesn't support default-int
#endif

#include <dbgeng.h>
#include "ReadVirtualCache.h"



extern PDEBUG_ADVANCED        g_ExtAdvanced;
extern PDEBUG_CLIENT          g_ExtClient;
extern PDEBUG_CONTROL2        g_ExtControl;
extern PDEBUG_DATA_SPACES     g_ExtData;
extern PDEBUG_DATA_SPACES2    g_ExtData2;
extern PDEBUG_REGISTERS       g_ExtRegisters;
extern PDEBUG_SYMBOLS         g_ExtSymbols;
extern PDEBUG_SYMBOLS2        g_ExtSymbols2;
extern PDEBUG_SYSTEM_OBJECTS  g_ExtSystem;
extern PDEBUG_SYSTEM_OBJECTS2 g_ExtSystem2;

const int mdNameLen = 2048;
enum JitType {UNKNOWN=0, JIT, EJIT, PJIT};
enum EEFLAVOR {UNKNOWNEE, MSCOREE, MSCORWKS, MSCOREND};
extern WCHAR g_mdName[mdNameLen];


#define EXT_RELEASE(Unk) \
    ((Unk) != NULL ? ((Unk)->Release(), (Unk) = NULL) : NULL)

#define moveDB(dst, src)\
{                                                                       \
    HRESULT ret = rvCache->Read((ULONG64)src, &(dst), sizeof(dst), NULL); \
    if (FAILED(ret)) return;                                            \


extern ReadVirtualCache *rvCache;

//MOVE(objAddr, ptr);
//Move<DWORD_PTR>(objAddr, ptr);

#define MOVE(dst, src) rvCache->Read((ULONG64)src, &(dst), sizeof(dst), NULL)
#define MOVEN(dst, src) rvCache->Read((ULONG64)src, &(dst), sizeof(dst), NULL)

 template <typename Buffer> struct Move 
 {
   HRESULT hr; 
   Move(Buffer& buffer, ULONG64 src) 
   { 
        hr = rvCache->Read(src, &(buffer), sizeof(buffer), NULL);
   }
   operator HRESULT(void) const { return hr; }
 };

 template <typename Buffer> struct MoveBlock
 {
   HRESULT hr; 
   MoveBlock(Buffer& buffer, ULONG64 src, size_t size) 
   { 
        hr = rvCache->Read(src, &(buffer), size, NULL);
   }
   operator HRESULT(void) const { return hr; }
 };

class DebugExtensions
{
public:
	static HRESULT Initialize(PULONG Version, PULONG Flags);
	static void    UnInitialize();
	static HRESULT Query(PDEBUG_CLIENT Client);
    static ULONG OSPageSize();

	void __cdecl Out(PCSTR Format, ...);
	void IncrementIndent ();
	void DecrementIndent ();
	void Indent ();
	void IP2MethodDesc(DWORD_PTR IP, DWORD_PTR &methodDesc, JitType &jitType, DWORD_PTR &gcinfoAddr);
	void ReloadSymbolWithLineInfo();
	EEFLAVOR GetEEFlavor ();
	size_t NextOSPageAddress (size_t addr);
	BOOL IsInterrupt();
	BOOL NameForObject(DWORD_PTR ObjAddr, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName);
	static HRESULT GetMDInfo(wchar_t * buffer, int bufferSize, CLRDATA_ADDRESS address);

	static HRESULT EnterDebugMode();
};
