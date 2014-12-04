#pragma once
#pragma unmanaged

enum
{
    // Locate these enumerants up in a high index region
    // to avoid any possible conflicts with the public enumerants
    // that begin at zero.
    DACPRIV_REQUEST_THREAD_STORE_DATA = 0xf0000000,
    DACPRIV_REQUEST_APPDOMAIN_STORE_DATA,
    DACPRIV_REQUEST_APPDOMAIN_LIST,
    DACPRIV_REQUEST_APPDOMAIN_DATA,
    DACPRIV_REQUEST_APPDOMAIN_NAME,
    DACPRIV_REQUEST_APPDOMAIN_APP_BASE,
    DACPRIV_REQUEST_APPDOMAIN_PRIVATE_BIN_PATHS,
    DACPRIV_REQUEST_APPDOMAIN_CONFIG_FILE,
    DACPRIV_REQUEST_ASSEMBLY_LIST,
    DACPRIV_REQUEST_FAILED_ASSEMBLY_LIST,
    DACPRIV_REQUEST_ASSEMBLY_DATA,
    DACPRIV_REQUEST_ASSEMBLY_NAME,
    DACPRIV_REQUEST_ASSEMBLY_DISPLAY_NAME,
    DACPRIV_REQUEST_ASSEMBLY_LOCATION,
    DACPRIV_REQUEST_FAILED_ASSEMBLY_DATA,
    DACPRIV_REQUEST_FAILED_ASSEMBLY_DISPLAY_NAME,
    DACPRIV_REQUEST_FAILED_ASSEMBLY_LOCATION,
    DACPRIV_REQUEST_THREAD_DATA,
    DACPRIV_REQUEST_THREAD_THINLOCK_DATA,
    DACPRIV_REQUEST_CONTEXT_DATA,
    DACPRIV_REQUEST_METHODDESC_DATA,
    DACPRIV_REQUEST_METHODDESC_IP_DATA,
    DACPRIV_REQUEST_METHODDESC_NAME,
    DACPRIV_REQUEST_METHODDESC_FRAME_DATA,
    DACPRIV_REQUEST_CODEHEADER_DATA,
    DACPRIV_REQUEST_THREADPOOL_DATA,
    DACPRIV_REQUEST_WORKREQUEST_DATA,
    DACPRIV_REQUEST_OBJECT_DATA,
    DACPRIV_REQUEST_FRAME_NAME,
    DACPRIV_REQUEST_OBJECT_STRING_DATA,
    DACPRIV_REQUEST_OBJECT_CLASS_NAME,
    DACPRIV_REQUEST_METHODTABLE_NAME,
    DACPRIV_REQUEST_METHODTABLE_DATA,
    DACPRIV_REQUEST_EECLASS_DATA,
    DACPRIV_REQUEST_FIELDDESC_DATA,
    DACPRIV_REQUEST_MANAGEDSTATICADDR,
    DACPRIV_REQUEST_MODULE_DATA,
    DACPRIV_REQUEST_MODULEMAP_TRAVERSE,
    DACPRIV_REQUEST_MODULETOKEN_DATA,
//#ifdef _DEBUG
//    DACPRIV_REQUEST_MDA,
//#endif
    DACPRIV_REQUEST_PEFILE_DATA,
    DACPRIV_REQUEST_PEFILE_NAME,
    DACPRIV_REQUEST_ASSEMBLYMODULE_LIST,
    DACPRIV_REQUEST_GCHEAP_DATA,
    DACPRIV_REQUEST_GCHEAP_LIST,
    DACPRIV_REQUEST_GCHEAPDETAILS_DATA,
    DACPRIV_REQUEST_GCHEAPDETAILS_STATIC_DATA,
    DACPRIV_REQUEST_HEAPSEGMENT_DATA,
    DACPRIV_REQUEST_UNITTEST_DATA,
    DACPRIV_REQUEST_ISSTUB,
    DACPRIV_REQUEST_DOMAINLOCALMODULE_DATA,
    DACPRIV_REQUEST_DOMAINLOCALMODULEFROMAPPDOMAIN_DATA,
    DACPRIV_REQUEST_DOMAINLOCALMODULE_DATA_FROM_MODULE,
    DACPRIV_REQUEST_SYNCBLOCK_DATA,
    DACPRIV_REQUEST_SYNCBLOCK_CLEANUP_DATA,
    DACPRIV_REQUEST_HANDLETABLE_TRAVERSE,
    DACPRIV_REQUEST_RCWCLEANUP_TRAVERSE,
    DACPRIV_REQUEST_EHINFO_TRAVERSE,
    DACPRIV_REQUEST_STRESSLOG_DATA,
    DACPRIV_REQUEST_JITLIST,
    DACPRIV_REQUEST_JIT_HELPER_FUNCTION_NAME,
    DACPRIV_REQUEST_JUMP_THUNK_TARGET,
    DACPRIV_REQUEST_LOADERHEAP_TRAVERSE,
    DACPRIV_REQUEST_MANAGER_LIST,
    DACPRIV_REQUEST_JITHEAPLIST,
    DACPRIV_REQUEST_CODEHEAP_LIST,
    DACPRIV_REQUEST_METHODTABLE_SLOT,
    DACPRIV_REQUEST_VIRTCALLSTUBHEAP_TRAVERSE,
    DACPRIV_REQUEST_NESTEDEXCEPTION_DATA,
    DACPRIV_REQUEST_USEFULGLOBALS,
    DACPRIV_REQUEST_CLRTLSDATA_INDEX,
    DACPRIV_REQUEST_MODULE_FINDIL
};

struct DacpThreadStoreData
{
    LONG threadCount;
    LONG unstartedThreadCount;
    LONG backgroundThreadCount;
    LONG pendingThreadCount;
    LONG deadThreadCount;
    CLRDATA_ADDRESS firstThread;
    CLRDATA_ADDRESS finalizerThread;
    CLRDATA_ADDRESS gcThread;
    DWORD fHostConfig;          // Uses hosting flags defined above
	
    HRESULT Request(IXCLRDataProcess* dac)
    {
        return dac->Request(DACPRIV_REQUEST_THREAD_STORE_DATA,
                            0, NULL,
                            sizeof(*this), (PBYTE)this);
    }
};

struct DacpSyncBlockData
{        
    CLRDATA_ADDRESS Object;
    BOOL            bFree; // if set, no other fields are useful
    
    // fields below provide data from this, so it's just for display
    CLRDATA_ADDRESS SyncBlockPointer;

	CLRDATA_ADDRESS NewInRtmVersion;

    UINT            MonitorHeld;
    UINT            Recursion;
    CLRDATA_ADDRESS HoldingThread;
    UINT            AdditionalThreadCount;
    CLRDATA_ADDRESS appDomainPtr;
    
    // SyncBlockCount will always be filled in with the number of SyncBlocks.
    // SyncBlocks may be requested from [1,SyncBlockCount]
    UINT            SyncBlockCount;

    // SyncBlockNumber must be from [1,SyncBlockCount]    
    // If there are no SyncBlocks, a call to Request with SyncBlockCount = 1
    // will return E_FAIL.
    HRESULT Request(IXCLRDataProcess* dac, UINT SyncBlockNumber)
    {
        return dac->Request(DACPRIV_REQUEST_SYNCBLOCK_DATA,
                            sizeof(SyncBlockNumber), (PBYTE)&SyncBlockNumber,
                            sizeof(*this), (PBYTE)this);    
    }
};

enum DacpObjectType { OBJ_STRING=0,OBJ_FREE,OBJ_OBJECT,OBJ_ARRAY,OBJ_OTHER };
struct DacpObjectData
{
    CLRDATA_ADDRESS EEClass;
    CLRDATA_ADDRESS MethodTable;
    DacpObjectType ObjectType;
    DWORD Size;
    CLRDATA_ADDRESS ElementTypeHandle;
    CorElementType ElementType;
    DWORD dwRank;
    DWORD dwNumComponents;
    DWORD dwComponentSize;
    CLRDATA_ADDRESS ArrayDataPtr;
    CLRDATA_ADDRESS ArrayBoundsPtr;
    CLRDATA_ADDRESS ArrayLowerBoundsPtr;

    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_OBJECT_DATA,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) &addr,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT GetObjectClassName(IXCLRDataProcess* dac,
                                      CLRDATA_ADDRESS addr, 
                                      ULONG32 iNameChars,
                                      __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_OBJECT_CLASS_NAME,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetStringObjectData(IXCLRDataProcess* dac,
                                       CLRDATA_ADDRESS addr, 
                                       ULONG32 iNameChars,
                                       __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_OBJECT_STRING_DATA,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }
};

struct DacpThreadArgs
{
    CLRDATA_ADDRESS addrThread;
    BOOL isFullDump;	// pass FALSE here if you don't have full memory access.
};


struct DacpThreadData
{
    DWORD corThreadId;
    DWORD osThreadId;
    int state;
    ULONG preemptiveGCDisabled;
    CLRDATA_ADDRESS allocContextPtr;
    CLRDATA_ADDRESS allocContextLimit;
    CLRDATA_ADDRESS context;
    CLRDATA_ADDRESS domain;
    CLRDATA_ADDRESS pSharedStaticData;
    CLRDATA_ADDRESS pUnsharedStaticData;
    CLRDATA_ADDRESS pFrame;
    DWORD lockCount;
    CLRDATA_ADDRESS firstNestedException; // Pass this pointer to DacpNestedExceptionInfo
    CLRDATA_ADDRESS fiberData;
    CLRDATA_ADDRESS lastThrownObjectHandle;

	CLRDATA_ADDRESS NewInRtmVersion;  // Added post Rotor / sscli

    CLRDATA_ADDRESS nextThread;

    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr, BOOL isFullDump=TRUE)
    {
        DacpThreadArgs dta = { addr, isFullDump };
        return dac->Request(DACPRIV_REQUEST_THREAD_DATA,
                            sizeof(DacpThreadArgs), (PBYTE)&dta,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT ThreadFromThinlockId(IXCLRDataProcess* dac, UINT thinLockId, CLRDATA_ADDRESS *outThread)
    {
        return dac->Request(DACPRIV_REQUEST_THREAD_THINLOCK_DATA,
                            sizeof(UINT *), (PBYTE)&thinLockId,
                            sizeof(CLRDATA_ADDRESS *), (PBYTE)outThread);
    }    
};

// for JITType
typedef enum JITTypes {TYPE_UNKNOWN=0,TYPE_JIT,TYPE_EJIT,TYPE_PJIT};

struct DacpCodeHeaderData
{        
    CLRDATA_ADDRESS GCInfo;
    JITTypes        JITType;
    CLRDATA_ADDRESS MethodDescPtr;
    CLRDATA_ADDRESS MethodStart;
    DWORD                    MethodSize;
    CLRDATA_ADDRESS ColdRegionStart;
    DWORD           ColdRegionSize;
    DWORD           HotRegionSize;
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS IPAddr)
    {
        return dac->Request(DACPRIV_REQUEST_CODEHEADER_DATA,
                            sizeof(IPAddr), (PBYTE)&IPAddr,
                            sizeof(*this), (PBYTE)this);    
    }
};

enum
{
    DACSTACKPRIV_REQUEST_FRAME_DATA = 0xf0000000
};

struct DacpFrameData
{
    CLRDATA_ADDRESS frameAddr;

    // Could also be implemented for IXCLRDataFrame if desired.
    HRESULT Request(IXCLRDataStackWalk* dac)
    {
        return dac->Request(DACSTACKPRIV_REQUEST_FRAME_DATA,
                            0, NULL,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT GetFrameName(IXCLRDataProcess* dac,
                                 CLRDATA_ADDRESS VTableForFrame,
                                 ULONG32 iNameChars,
                                 __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_FRAME_NAME,
                            sizeof(VTableForFrame), (PBYTE)&VTableForFrame,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

};

struct DacpMethodDescData
{
    BOOL            bHasNativeCode;
    BOOL            bIsDynamic;
    WORD            wSlotNumber;
    CLRDATA_ADDRESS NativeCodeAddr;
    // Useful for breaking when a method is jitted.
    CLRDATA_ADDRESS AddressOfNativeCodeSlot;
    
    CLRDATA_ADDRESS MethodDescPtr;
    CLRDATA_ADDRESS MethodTablePtr;
    CLRDATA_ADDRESS EEClassPtr;
    CLRDATA_ADDRESS ModulePtr;
    
    CLRDATA_ADDRESS PreStubAddr;
    mdToken                  MDToken;
    CLRDATA_ADDRESS GCInfo;
    WORD                      JITType;
    CLRDATA_ADDRESS GCStressCodeCopy;

    // This is only valid if bIsDynamic is true
    CLRDATA_ADDRESS managedDynamicMethodObject;
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_METHODDESC_DATA,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(*this), (PBYTE)this);
    }

    HRESULT RequestFromIP(IXCLRDataProcess* dac, CLRDATA_ADDRESS IPaddr)
    {
        return dac->Request(DACPRIV_REQUEST_METHODDESC_IP_DATA,
                            sizeof(IPaddr), (PBYTE)&IPaddr,
                            sizeof(*this), (PBYTE)this);
    }

    HRESULT RequestFromFrame(IXCLRDataProcess* dac, CLRDATA_ADDRESS FrameAddr)
    {
        return dac->Request(DACPRIV_REQUEST_METHODDESC_FRAME_DATA,
                            sizeof(FrameAddr), (PBYTE)&FrameAddr,
                            sizeof(*this), (PBYTE)this);
    }


    static HRESULT GetMethodName(IXCLRDataProcess* dac,
                                 CLRDATA_ADDRESS addrMethodDesc,
                                 ULONG32 iNameChars,
                                 __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_METHODDESC_NAME,
                            sizeof(addrMethodDesc), (PBYTE)&addrMethodDesc,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }
};

enum DacpAppDomainDataStage {
    STAGE_CREATING,
    STAGE_READYFORMANAGEDCODE,
    STAGE_ACTIVE,
    STAGE_OPEN,
    STAGE_UNLOAD_REQUESTED,
    STAGE_EXITING,
    STAGE_EXITED,
    STAGE_FINALIZING,
    STAGE_FINALIZED,
    STAGE_HANDLETABLE_NOACCESS,
    STAGE_CLEARED,
    STAGE_COLLECTED,
    STAGE_CLOSED
};

struct DacpAppDomainData
{
    CLRDATA_ADDRESS AppDomainPtr;   // useful to keep in the structure
    CLRDATA_ADDRESS AppSecDesc;
    CLRDATA_ADDRESS pLowFrequencyHeap;
    CLRDATA_ADDRESS pHighFrequencyHeap;
    CLRDATA_ADDRESS pStubHeap;
    CLRDATA_ADDRESS DomainLocalBlock;
    CLRDATA_ADDRESS pDomainLocalModules;    
    // The creation sequence number of this app domain (starting from 1)
    DWORD dwId;
    LONG AssemblyCount;
    LONG FailedAssemblyCount;
    DacpAppDomainDataStage appDomainStage; 
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_APPDOMAIN_DATA,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT GetName(IXCLRDataProcess* dac,
                           CLRDATA_ADDRESS AppDomainaddr,
                           ULONG32 iNameChars,
                           __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_APPDOMAIN_NAME,
                            sizeof(AppDomainaddr), (PBYTE)&AppDomainaddr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetApplicationBase(IXCLRDataProcess* dac,
                                      CLRDATA_ADDRESS AppDomainaddr,
                                      ULONG32 iNameChars,
                                      __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_APPDOMAIN_APP_BASE,
                            sizeof(AppDomainaddr), (PBYTE)&AppDomainaddr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetPrivateBinPaths(IXCLRDataProcess* dac,
                                      CLRDATA_ADDRESS AppDomainaddr,
                                      ULONG32 iNameChars,
                                      __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_APPDOMAIN_PRIVATE_BIN_PATHS,
                            sizeof(AppDomainaddr), (PBYTE)&AppDomainaddr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetConfigFile(IXCLRDataProcess* dac,
                                 CLRDATA_ADDRESS AppDomainaddr,
                                 ULONG32 iNameChars,
                                 __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_APPDOMAIN_CONFIG_FILE,
                            sizeof(AppDomainaddr), (PBYTE)&AppDomainaddr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetAssemblies(IXCLRDataProcess* dac,
                                 CLRDATA_ADDRESS addr, ULONG32 iArraySize,
                                 CLRDATA_ADDRESS Assemblies[])
    {
        return dac->Request(DACPRIV_REQUEST_ASSEMBLY_LIST,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(CLRDATA_ADDRESS)*iArraySize,
                            (PBYTE) Assemblies);
    }

    static HRESULT GetFailedAssemblies(IXCLRDataProcess* dac,
                                 CLRDATA_ADDRESS addr, ULONG32 iArraySize,
                                 CLRDATA_ADDRESS Assemblies[])
    {
        return dac->Request(DACPRIV_REQUEST_FAILED_ASSEMBLY_LIST,
                            sizeof(addr), (PBYTE)&addr,
                            sizeof(CLRDATA_ADDRESS)*iArraySize,
                            (PBYTE) Assemblies);
    }
};


typedef enum ModuleMapType
{
    TYPEDEFTOMETHODTABLE, TYPEREFTOMETHODTABLE
};
typedef void (*MODULEMAPTRAVERSE)(UINT index, CLRDATA_ADDRESS methodTable,LPVOID token);
struct DacpModuleMapTraverseArgs
{
    ModuleMapType mmt;
    CLRDATA_ADDRESS Module;    
    MODULEMAPTRAVERSE Callback;
    LPVOID token;
};

struct DacpModuleFindILArgs 
{
    CLRDATA_ADDRESS Module;    
    CLRDATA_ADDRESS rva;
};

struct DacpModuleData
{
    CLRDATA_ADDRESS File; // A PEFile addr
    CLRDATA_ADDRESS  ilBase;
    CLRDATA_ADDRESS metadataStart;
    SIZE_T metadataSize;
    CLRDATA_ADDRESS Assembly; // Assembly pointer
    BOOL bIsReflection;
    BOOL bIsPEFile;
    SIZE_T dwBaseClassIndex;
    IXCLRDataModule *ModuleDefinition;
    SIZE_T dwDomainNeutralIndex;

    DWORD dwTransientFlags;
    
    CLRDATA_ADDRESS TypeDefToMethodTableMap;
    CLRDATA_ADDRESS TypeRefToMethodTableMap;
    CLRDATA_ADDRESS MethodDefToDescMap;
    CLRDATA_ADDRESS FieldDefToDescMap;
    CLRDATA_ADDRESS MemberRefToDescMap;
    CLRDATA_ADDRESS FileReferencesMap;
    CLRDATA_ADDRESS ManifestModuleReferencesMap;

    CLRDATA_ADDRESS pLookupTableHeap;
    CLRDATA_ADDRESS pThunkHeap;
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_MODULE_DATA,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) &addr,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT DoMapTraverse(IXCLRDataProcess* dac,CLRDATA_ADDRESS addr, ModuleMapType mmt, 
        MODULEMAPTRAVERSE pFunc, LPVOID token)
    {
        DacpModuleMapTraverseArgs args = { mmt, addr, pFunc, token };
        return dac->Request(DACPRIV_REQUEST_MODULEMAP_TRAVERSE,
                            sizeof(DacpModuleMapTraverseArgs), (BYTE *)&args,
                            0, NULL);    
    }    

    static HRESULT FindIL(IXCLRDataProcess * dac, CLRDATA_ADDRESS addrModule, CLRDATA_ADDRESS rva,
            CLRDATA_ADDRESS *pAddr) // pAddr is the IL address on successful completion.
    {
        DacpModuleFindILArgs args = { addrModule, rva };
        return dac->Request(DACPRIV_REQUEST_MODULE_FINDIL,
            sizeof(DacpModuleFindILArgs), (BYTE *)&args,
            sizeof(CLRDATA_ADDRESS), (PBYTE)pAddr);
    }
};

// to test
struct DacpManagedStaticArgs
{
    CLRDATA_ADDRESS ThreadAddr;
    DWORD Slot;
};

struct DacpFieldDescData
{
    CorElementType Type;
    CorElementType sigType;     // ELEMENT_TYPE_XXX from signature. We need this to disply pretty name for String in minidump's case
    CLRDATA_ADDRESS MTOfType; // NULL if Type is not loaded
    
    CLRDATA_ADDRESS ModuleOfType;
    mdTypeDef TokenOfType;
    
    mdFieldDef mb;
    CLRDATA_ADDRESS MTOfEnclosingClass;
    DWORD dwOffset;
    BOOL bIsThreadLocal;
    BOOL bIsContextLocal;
    BOOL bIsStatic;
    CLRDATA_ADDRESS NextField;
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_FIELDDESC_DATA,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) &addr,
                            sizeof(*this), (PBYTE)this);
    }

    // Only to be called for ThreadLocal statics which are of ELEMENT_TYPE_CLASS
    static HRESULT CalculateAddrForManagedStatic(IXCLRDataProcess* dac, CLRDATA_ADDRESS ThreadAddr,
        DWORD Slot,CLRDATA_ADDRESS *pOutAddr)
    {
        DacpManagedStaticArgs args = { ThreadAddr, Slot };
        return dac->Request(DACPRIV_REQUEST_MANAGEDSTATICADDR,
                            sizeof(DacpManagedStaticArgs), (PBYTE) &args,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) pOutAddr);

    }
};


struct DacpEEClassData
{
    CLRDATA_ADDRESS MethodTable;
    CLRDATA_ADDRESS Module;
    WORD wNumVtableSlots;
    WORD wNumMethodSlots;
    WORD wNumInstanceFields;
    WORD wNumStaticFields;
    DWORD dwClassDomainNeutralIndex;
    DWORD dwAttrClass; // cached metadata
    mdTypeDef cl; // Metadata token    

    CLRDATA_ADDRESS FirstField; // If non-null, you can retrieve more
    
    WORD wThreadStaticOffset;
    WORD wThreadStaticsSize;
    WORD wContextStaticOffset;
    WORD wContextStaticsSize;
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_EECLASS_DATA,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) &addr,
                            sizeof(*this), (PBYTE)this);
    }
};

struct DacpSlotArgs
{
    CLRDATA_ADDRESS mtaddr;
    ULONG32 SlotNumber;	
};

struct DacpMethodTableData
{
    BOOL bIsFree; // everything else is NULL if this is true.
    CLRDATA_ADDRESS Class;
    CLRDATA_ADDRESS ParentMethodTable;
    WORD wNumInterfaces;
    WORD wTotalVtableSlots;
    DWORD BaseSize;
    DWORD ComponentSize;
    BOOL bIsShared; // flags & enum_flag_DomainNeutral
    DWORD sizeofMethodTable;
    BOOL bIsDynamic;
    BOOL bContainsPointers;
    
    HRESULT Request(IXCLRDataProcess* dac, CLRDATA_ADDRESS addr)
    {
        return dac->Request(DACPRIV_REQUEST_METHODTABLE_DATA,
                            sizeof(CLRDATA_ADDRESS), (PBYTE) &addr,
                            sizeof(*this), (PBYTE)this);
    }

    static HRESULT GetMethodTableName(IXCLRDataProcess* dac,
                                      CLRDATA_ADDRESS mtaddr,
                                      ULONG32 iNameChars,
                                      __out_ecount (iNameChars) LPWSTR pwszName)
    {
        return dac->Request(DACPRIV_REQUEST_METHODTABLE_NAME,
                            sizeof(mtaddr), (PBYTE)&mtaddr,
                            sizeof(WCHAR)*iNameChars, (PBYTE) pwszName);
    }

    static HRESULT GetSlot(IXCLRDataProcess* dac,
                                    CLRDATA_ADDRESS mtaddr,
                                    ULONG32 SlotNumber,
                                    CLRDATA_ADDRESS *SlotValue)
    {
        DacpSlotArgs dsa = { mtaddr, SlotNumber };
        return dac->Request(DACPRIV_REQUEST_METHODTABLE_SLOT,
                            sizeof(DacpSlotArgs), (PBYTE) &dsa,
                            sizeof(CLRDATA_ADDRESS),(PBYTE) SlotValue);
    }
};



