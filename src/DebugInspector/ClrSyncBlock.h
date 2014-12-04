#pragma once
#pragma unmanaged

struct ClrSyncBlock
{
	ULONG64 Object;
    BOOL    bFree; 
    ULONG64 SyncBlockPointer;
    UINT    MonitorHeld;
    UINT    Recursion;
    ULONG64 HoldingThread;
    UINT    AdditionalThreadCount;
    ULONG64 AppDomainPtr;
    UINT    SyncBlockCount;
	DWORD   OsThreadId;
	ULONG   ClrThreadId;
	ULONG64 Reserved;
	WCHAR   ObjectName[2048];
};