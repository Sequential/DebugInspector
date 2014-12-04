#pragma once
#include "StdAfx.h"
#include "KStructs.h"
#include "WaitSnapshot.h"

class Kernel
{
private:
	WaitSnapshot* _pCurrentSnapshot;
	ULONG64 _KProcessKernel;
	ULONG64 _KThreadKernel;

	ULONG64 _KProcess;
	ULONG32 _OSProcessId;
	int*    _pOffsets;

	bool _cannotAttachLocalKernel;
	DWORD _lastError;
public:
	Kernel(ULONG32 OSProcessId);
	HRESULT GetWaitChainInfo(WaitSnapshot& snapshot);
	bool DidAttachKernel(){return !_cannotAttachLocalKernel;}
	DWORD LastError(){return _lastError;}
public:
	~Kernel(void);
private:
	static void    SetupOffsets();
	HRESULT GetOffsetFromOSProcessId(ULONG32 OSProcessId, ULONG64& Offset);
	static HRESULT GetCommonVersionInfo(ULONG& win32Major, ULONG& win32Minor, ULONG& servicePack);
	HRESULT GetHandleFromObjectAddress(ULONG32 ObjectAddress, ULONG64 KProcess, ULONG32& Handle);
	static HRESULT GetObjectName(ULONG64 ObjectAddress, WCHAR* buffer, int size);

	HRESULT SetPriviledge(LPCTSTR lpszPrivilege);

	HRESULT EnterDebugMode();
	HRESULT Attach();
	HRESULT GetThreadInfo(ULONG32 EThreadAddress);
	HRESULT GetDispatcherInfo(ULONG32 ObjectAddress);
	HRESULT ProcessThreadWaitList();
	HRESULT ProcessHandles();
	HRESULT ProcessWaitList(ULONG32 HeadWaitBlockListAddress);
	HRESULT ProcessMutantList(ULONG32 EThreadAddress);
	
};
