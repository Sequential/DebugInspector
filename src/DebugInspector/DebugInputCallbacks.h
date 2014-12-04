#pragma once
#include "stdafx.h"

class DebugInputCallbacks: public IDebugInputCallbacks
{
public:
	DebugInputCallbacks(void){}
public:
	~DebugInputCallbacks(void){}

	// IUnknown.
    STDMETHOD(QueryInterface)(REFIID InterfaceId, PVOID* Interface)
	{
		 if (InterfaceId == IID_IUnknown || InterfaceId == IID_IDebugInputCallbacks)
		{
			*Interface = (IDebugInputCallbacks*)this;
			return S_OK;
		}
		else
		{
			*Interface = NULL;
			return E_NOINTERFACE;
		}
	}

	STDMETHOD_(ULONG, AddRef)(){return S_OK;}
	STDMETHOD_(ULONG, Release)(){return S_OK;}

    // IDebugInputCallbacks.

	STDMETHOD(StartInput)(ULONG BufferSize)
	{
		//g_ExtControl->ReturnInput("Mark");
		return S_OK;
	}

	STDMETHOD(EndInput)()
	{
		return S_OK;
	}



};