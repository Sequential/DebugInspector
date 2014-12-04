#pragma unmanaged

#include "StdAfx.h"
#include "DebugExtensions.h"
#include "DacpStructures.h"
#include "Dac.h"
#include "DbgHelp.h"
#include "Windows.h"

PDEBUG_ADVANCED        g_ExtAdvanced;
PDEBUG_CLIENT          g_ExtClient;
PDEBUG_CONTROL2        g_ExtControl;
PDEBUG_DATA_SPACES     g_ExtData;
PDEBUG_DATA_SPACES2    g_ExtData2;
PDEBUG_REGISTERS       g_ExtRegisters;
PDEBUG_SYMBOLS         g_ExtSymbols;
PDEBUG_SYMBOLS2        g_ExtSymbols2;
PDEBUG_SYSTEM_OBJECTS  g_ExtSystem;
PDEBUG_SYSTEM_OBJECTS2 g_ExtSystem2;

DWORD   g_Indent = 0;
BOOL    g_ControlC = FALSE;

WCHAR   g_mdName[mdNameLen];

ReadVirtualCache g_special_rvCacheSpace;
ReadVirtualCache *rvCache = &g_special_rvCacheSpace;

HRESULT DebugExtensions::Initialize(PULONG Version, PULONG Flags)
{
	//EnterDebugMode();

	IDebugClient *DebugClient;
    PDEBUG_CONTROL DebugControl;
    HRESULT Hr;

    *Version = DEBUG_EXTENSION_VERSION(1, 0);
    *Flags = 0;

    if ((Hr = DebugCreate(__uuidof(IDebugClient),
                          (void **)&DebugClient)) != S_OK)
    {
        return Hr;
    }
    if ((Hr = DebugClient->QueryInterface(__uuidof(IDebugControl),
                                              (void **)&DebugControl)) != S_OK)
    {
        return Hr;
    }

   /* ExtensionApis.nSize = sizeof (ExtensionApis);
    if ((Hr = DebugControl->GetWindbgExtensionApis64(&ExtensionApis)) != S_OK) {
        return Hr;
    }*/

    Query(DebugClient);

	//Hr = DebugClient->ConnectSession(0,0);
    /*if (IsMiniDumpFileNODAC())
    {
        ExtOut ("------------------------------------------------------------\n");
        ExtOut ("clrAddin.dll needs a full memory dump for complete functionality.\n");
        ExtOut ("You can create one with .dump /ma <filename>\n");
        ExtOut ("------------------------------------------------------------\n");
    }*/
    //UnInitialize();

    DebugControl->Release();
    DebugClient->Release();
    return S_OK;
}

HRESULT DebugExtensions::EnterDebugMode() 
{
	//HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, _OSProcessId);

   HANDLE hToken;
   TOKEN_PRIVILEGES tp;
   LUID luid;
   TOKEN_PRIVILEGES tpPrevious;
   DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES); 

   if(!::OpenProcessToken(GetCurrentProcess(),
   TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
     return E_FAIL; 

   if(!LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luid )) 
     return E_FAIL;
   tp.PrivilegeCount           = 1;
   tp.Privileges[0].Luid       = luid;
   tp.Privileges[0].Attributes = 0; 

   AdjustTokenPrivileges(hToken,FALSE,&tp,
   sizeof(TOKEN_PRIVILEGES),&tpPrevious,&cbPrevious);
   if (GetLastError() != ERROR_SUCCESS) 
     return E_FAIL; 

   tpPrevious.PrivilegeCount       = 1;
   tpPrevious.Privileges[0].Luid   = luid;
   tpPrevious.Privileges[0].Attributes|=(SE_PRIVILEGE_ENABLED); 

   AdjustTokenPrivileges(hToken,FALSE, &tpPrevious,cbPrevious, 
   NULL,NULL);
   if (GetLastError() != ERROR_SUCCESS) 
     return E_FAIL; 

   CloseHandle(hToken);
   return S_OK; 

} 

void DebugExtensions::UnInitialize()
{
    g_ExtClient = NULL;
    EXT_RELEASE(g_ExtAdvanced);
    EXT_RELEASE(g_ExtControl);
    EXT_RELEASE(g_ExtData);
    EXT_RELEASE(g_ExtData2);
    EXT_RELEASE(g_ExtRegisters);
    EXT_RELEASE(g_ExtSymbols);
    EXT_RELEASE(g_ExtSymbols2);
    EXT_RELEASE(g_ExtSystem);
}


HRESULT DebugExtensions::Query(PDEBUG_CLIENT Client)
{    
	HRESULT Status;
    if ((Status = Client->QueryInterface(__uuidof(IDebugAdvanced),
                                 (void **)&g_ExtAdvanced)) != S_OK)
    {
        goto Fail;
    }
    if ((Status = Client->QueryInterface(__uuidof(IDebugControl2),
                                 (void **)&g_ExtControl)) != S_OK)
    {
        goto Fail;
    }
    if ((Status = Client->QueryInterface(__uuidof(IDebugDataSpaces),
                                 (void **)&g_ExtData)) != S_OK)
    {
        goto Fail;
    }    
    if ((Status = Client->QueryInterface(__uuidof(IDebugDataSpaces2),
                                 (void **)&g_ExtData2)) != S_OK)
    {
        goto Fail;
    }

    if ((Status = Client->QueryInterface(__uuidof(IDebugRegisters),
                                 (void **)&g_ExtRegisters)) != S_OK)
    {
        goto Fail;
    }
    if ((Status = Client->QueryInterface(__uuidof(IDebugSymbols),
                                 (void **)&g_ExtSymbols)) != S_OK)
    {
        goto Fail;
    }
    if (Client->QueryInterface(__uuidof(IDebugSymbols2), 
                                 (void **)&g_ExtSymbols2) != S_OK)
    {
        g_ExtSymbols2 = NULL;
    }
    if ((Status = Client->QueryInterface(__uuidof(IDebugSystemObjects),
                                         (void **)&g_ExtSystem)) != S_OK)
    {
        goto Fail;
    }
	if ((Status = Client->QueryInterface(__uuidof(IDebugSystemObjects2),
                                         (void **)&g_ExtSystem2)) != S_OK)
    {
        goto Fail;
    }

    g_ExtClient = Client;

    return S_OK;

 Fail:
    UnInitialize();
    return Status;
}


ULONG DebugExtensions::OSPageSize ()
{
    static ULONG pageSize = 0;
    if (pageSize == 0)
        g_ExtControl->GetPageSize(&pageSize);

    return pageSize;
}


void __cdecl DebugExtensions::Out(PCSTR Format, ...)
{
    va_list Args;
    
    va_start(Args, Format);
    Indent();
    g_ExtControl->OutputVaList(DEBUG_OUTPUT_NORMAL, Format, Args);
    va_end(Args);
}

void DebugExtensions::IncrementIndent ()
{
    g_Indent++;
}

void DebugExtensions::DecrementIndent ()
{
    g_Indent--;
}

void DebugExtensions::Indent ()
{
    for (DWORD i = 0; i < g_Indent; i++)
        g_ExtControl->Output (DEBUG_OUTPUT_NORMAL, "    ");
}


void DebugExtensions::IP2MethodDesc (DWORD_PTR IP, DWORD_PTR &methodDesc, JitType &jitType,
                    DWORD_PTR &gcinfoAddr)
{

    CLRDATA_ADDRESS EIP = (CLRDATA_ADDRESS) IP;
    DacpCodeHeaderData codeHeaderData;
    
    methodDesc = NULL;
    gcinfoAddr = NULL;
    
    if (codeHeaderData.Request(g_clrData,(CLRDATA_ADDRESS)EIP) != S_OK)
    {        
        return;
    }

    methodDesc = (DWORD_PTR) codeHeaderData.MethodDescPtr;
    jitType = (JitType) codeHeaderData.JITType;
    gcinfoAddr = (DWORD_PTR) codeHeaderData.GCInfo;    
}


void DebugExtensions::ReloadSymbolWithLineInfo()
{
    static BOOL bLoadSymbol = FALSE;
    if (!bLoadSymbol)
    {
        ULONG Options;
        g_ExtSymbols->GetSymbolOptions (&Options);
        if (!(Options & SYMOPT_LOAD_LINES))
        {
            g_ExtSymbols->AddSymbolOptions (SYMOPT_LOAD_LINES);
            g_ExtSymbols->Reload ("/f mscoree.dll");
            EEFLAVOR flavor = GetEEFlavor ();
            if (flavor == MSCORWKS)
                g_ExtSymbols->Reload ("/f mscorwks.dll");
        }
        
        // reload mscoree.pdb and mscorjit.pdb to get line info
        bLoadSymbol = TRUE;
    }
}

EEFLAVOR DebugExtensions::GetEEFlavor ()
{
    EEFLAVOR flavor = UNKNOWNEE;    
    
    if (SUCCEEDED(g_ExtSymbols->GetModuleByModuleName("mscorwks",0,NULL,NULL))) {
        flavor = MSCORWKS;
    }
    return flavor;
}

size_t DebugExtensions::NextOSPageAddress (size_t addr)
{
    size_t pageSize = OSPageSize();
    return (addr+pageSize)&(~(pageSize-1));
}

BOOL DebugExtensions::IsInterrupt() 
{
    if (g_ControlC) {
        return g_ControlC;
    }
    if (g_ExtControl->GetInterrupt() == S_OK)
        g_ControlC = TRUE;
    return g_ControlC;
}

BOOL DebugExtensions::NameForObject(DWORD_PTR ObjAddr, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName)
{
    mdName[0] = L'\0';
    if (DacpObjectData::GetObjectClassName(g_clrData,(CLRDATA_ADDRESS) ObjAddr, (ULONG32) capacity_mdName, mdName)!=S_OK)
    {
        return FALSE;
    }
    return TRUE;
}

HRESULT DebugExtensions::GetMDInfo(wchar_t * buffer, int bufferSize, CLRDATA_ADDRESS address)
{
	HRESULT hr = S_OK;
	DacpMethodDescData MethodDescData;
    if (hr = MethodDescData.Request(g_clrData,(CLRDATA_ADDRESS)address) != S_OK)
    {
        g_ext.Out("%p is not a MethodDesc\n", (ULONG64)address);
        return hr;
	}
    if (hr = DacpMethodDescData::GetMethodName(g_clrData, address, bufferSize, buffer) != S_OK)
    {
        wcscpy_s(buffer, bufferSize,L"UNKNOWN");        
        return hr;        
    }
	return hr;
}