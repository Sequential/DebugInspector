#pragma once
#include "DbgEng.h"

class DebugOutputCallbacks : public IDebugOutputCallbacks 
{
public:
	DebugOutputCallbacks(void);
	~DebugOutputCallbacks(void);

	STDMETHOD(QueryInterface)(REFIID InterfaceId, PVOID* Interface);
	STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

	STDMETHOD(Output)(ULONG Mask, PCSTR Text);
};



 
