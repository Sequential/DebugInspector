#pragma once
#pragma unmanaged
#include "LiveCLRDataTarget.h"
#include "DebugExtensions.h"
#include "ThreadStackManager.h"

using namespace System;
namespace Diag = System::Diagnostics;

extern IXCLRDataProcess* g_clrData;
extern DebugExtensions   g_ext;

#pragma managed
namespace ClrInspector 
{
public ref class Dac : IDisposable
{
public:
	Dac(Diag::Process^ process);
	ThreadStackManager^ ConstructThreadStacks();	
	~Dac();
};
}
#pragma unmanaged