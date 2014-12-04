#include "StdAfx.h"
#include "ClrThreadWalker.h"

#pragma unmanaged

ClrThreadWalker::ClrThreadWalker(void)
{
	_domainName[0] = '\0';
}

ClrThreadWalker::~ClrThreadWalker(void)
{
	g_ExtSystem->SetCurrentThreadId(_originalThreadId);
}

HRESULT ClrThreadWalker::Initialize()
{
	HRESULT hr = g_ExtSystem->GetCurrentThreadId(&_originalThreadId);
	if(FAILED(hr)) return hr;

    if ((hr = _threadStore.Request(g_clrData)) != S_OK)
    {
		g_ext.Out("Failed to request ThreadStore\n");
        return hr;

    } 
	_thread.osThreadId = -1;
	return S_OK;
}

HRESULT ClrThreadWalker::Next()
{
	if(_thread.osThreadId==-1)
	{
		_enumThread = _threadStore.firstThread;
	}
	else
	{
		_enumThread = _thread.nextThread;
	}

	if(!_enumThread) 
		return S_FALSE;

	HRESULT hr = S_OK;
	ULONG id=0;
	while(true)
	{
		if ((hr = _thread.Request(g_clrData, _enumThread, 1)) != S_OK)
		{
			g_ext.Out("Failed to request Thread at %p\n", _enumThread);
			return hr;
		}
		
		if (g_ExtSystem->GetThreadIdBySystemId (_thread.osThreadId, &id) != S_OK)
		{
			_enumThread = _thread.nextThread;  

			if(!_enumThread) 
				return S_FALSE;

			continue;
		}
		break;
	}

	hr = DacpAppDomainData::GetName(g_clrData, _thread.domain, 1024, _domainName);
	if(FAILED(hr))
	{
		_domainName[0] = '\0';
	}

	g_ExtSystem->SetCurrentThreadId(id);
	return S_OK;
}
