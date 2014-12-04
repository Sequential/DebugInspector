#include "StdAfx.h"
#include "DebugOutputCallbacks.h"

DebugOutputCallbacks::DebugOutputCallbacks(void)
{
}

DebugOutputCallbacks::~DebugOutputCallbacks(void)
{
}


	// IUnknown.
STDMETHODIMP DebugOutputCallbacks::QueryInterface(REFIID InterfaceId, PVOID* Interface)
	{
		if (InterfaceId == IID_IUnknown )//|| InterfaceId == IID_IDebugOutputCallbacks)
		{
			*Interface = (IDebugOutputCallbacks*)this;
			// No need to refcount as this class is contained.
			return S_OK;
		}
		else
		{
			*Interface = NULL;
			return E_NOINTERFACE;
		}
	}
STDMETHODIMP_(ULONG) DebugOutputCallbacks::AddRef(){return S_OK;}
	
STDMETHODIMP_(ULONG) DebugOutputCallbacks::Release(){return S_OK;}
	

    
STDMETHODIMP DebugOutputCallbacks::Output(ULONG Mask, PCSTR Text)
	{
		printf("DBGENG: %d %s", Mask, Text);
		return S_OK;
	}