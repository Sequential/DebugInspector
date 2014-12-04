#pragma once

#include "DacpStructures.h"
#include "Dac.h"
#pragma unmanaged
class ClrThreadWalker
{
private:
	ULONG				_originalThreadId;
    DacpThreadStoreData _threadStore;
	DacpThreadData		_thread;   
	CLRDATA_ADDRESS		_enumThread;
	wchar_t				_domainName[1024];
public:
	ClrThreadWalker(void);
	~ClrThreadWalker(void);

	HRESULT Initialize();

	HRESULT Next();

	DWORD OsThreadId() {return _thread.osThreadId;}
	DWORD CorThreadId(){return _thread.corThreadId;}
	CLRDATA_ADDRESS DomainId(){return _thread.domain;}
	wchar_t* DomainName(){return _domainName;}


};
