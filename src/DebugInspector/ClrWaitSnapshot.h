#pragma once
#include "WaitSnapshot.h"
#include "DebugStackTrace.h"
#include "UmThreadStackManager.h"


#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Diagnostics;
	namespace Diag = System::Diagnostics;

public ref class ClrWaitSnapshot
{
private:
	initonly Diag::Process^ _process;
public:
	ClrWaitSnapshot(Diag::Process^ process);
	ThreadStackManager^ Initialize();
private:
	void ProcessWaitSnapshot(WaitSnapshot& snapshot, ThreadStackManager ^ tsm);
	void ProcessDebugStackTrace(DebugStackTrace& debugStackTrace, ThreadStackManager ^ tsm);
};
}
#pragma unmanaged