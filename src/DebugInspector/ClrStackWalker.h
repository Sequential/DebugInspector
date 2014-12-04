#pragma once
#pragma unmanaged

#include "DacpStructures.h"
#include "ClrStackFrame.h"

class ClrStackWalker
{
public:
	ClrStackWalker();
	~ClrStackWalker(void);

	HRESULT Create(ClrStackFrame** ppClrFrame);
private:
	void RetrieveArgsAndLocals(IXCLRDataStackWalk *pStackWalk, ClrStackFrame& frame, BOOL bArgs, BOOL bLocals);
	HRESULT CheckMonitorEnter(IXCLRDataStackWalk *pStackWalk, ClrStackFrame& frame);
	HRESULT CheckThreadJoin(IXCLRDataStackWalk *pStackWalk, CONTEXT& context, ClrStackFrame& frame);
};
