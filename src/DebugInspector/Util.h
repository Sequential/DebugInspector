#pragma once
#pragma unmanaged

#include "malloc.h"
#include "DacpStructures.h"
#include "DebugExtensions.h"

#define MAX_CLASSNAME_LENGTH    1024

struct DumpStackFlag
{
    BOOL fEEonly;
    DWORD_PTR top;
    DWORD_PTR end;
};

class BaseObject
{
    /*MethodTable*/void    *m_pMethTab;
};


IMetaDataImport* MDImportForModule (DacpModuleData* pModule);


// Caller should guard against exception
// !!! mdName should have at least mdNameLen WCHAR
static HRESULT NameForTypeDef_s(mdTypeDef tkTypeDef, IMetaDataImport *pImport,
                              __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName);

HRESULT NameForToken_s(mdTypeDef mb, IMetaDataImport *pImport, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName,
                     bool bClassName);


