#pragma once

#include "KStructs.h"
#include "DebugStackFrameBuffer.h"
#include "HandleBuffer.h"

class DebugStackTrace
{
public:
	ULONG				  _debugThreadId;
	ULONG				  _systemThreadId;
	WaitingFor			  _waitType; 
	ULONG64				  _critSecAddress;
	DWORD				  _timeout;
	BOOL				  _bWaitAll;
	DebugStackFrameBuffer _frameBuffer;
	HandleBuffer		  _handleBuffer;
	HandleBuffer          _owningThreadBuffer;
public:
	DebugStackTrace();
	HRESULT Initialize(ULONG id, ULONG sysId);
public:
	~DebugStackTrace(void);
};
