#pragma once
#include "stdafx.h"

class DebugOutputCallbacks: public IDebugOutputCallbacks
{
public:
	DebugOutputCallbacks(void){}
public:
	~DebugOutputCallbacks(void){}

	// IUnknown.
    STDMETHOD(QueryInterface)(REFIID InterfaceId, PVOID* Interface)
	{
		 if (InterfaceId == IID_IUnknown || InterfaceId == IID_IDebugOutputCallbacks)
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

    // IDebugOutputCallbacks.

	STDMETHOD(Output)(ULONG mask, PCSTR text)
	{
		//printf("DBGHELP:    Mask %d: Text %s \n", mask, text);
		return S_OK;
	}

};