#include "StdAfx.h"
#include "Dac.h"
#include "DacpStructures.h"
#include "DebugExtensions.h"
#include "ClrInspectorStructs.h"
#include "ClrThreadWalker.h"

#include "ClrStackFrame.h"
#include "ClrStackWalker.h"
#include "DebugEventCallbacks.h"

#include "SyncBlockManager.h"
#include "LicenseProcessor.h"
#include "DebugContext.h"

#include "ExpiredException.h"

using namespace System;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

LiveCLRDataTarget* g_clrTarget;
IXCLRDataProcess* g_clrData;


#pragma managed
namespace ClrInspector
{

Dac::Dac(Process^ process)
{
	LicenseProcessor license;
	if(!license.IsLicenseValid())
	{
		String^ msg = gcnew String(license.GetLicenseText());
		throw gcnew ExpiredException(msg);
	}

	ULONG version;
	ULONG flags;
	DebugExtensions::Initialize(&version, &flags);

	g_clrTarget = new LiveCLRDataTarget(process);
	g_ext.ReloadSymbolWithLineInfo();

	DebugEventCallbacks cb;
	
	IDebugEventCallbacks* pCB = &cb;

	g_ExtClient->SetEventCallbacks(pCB);

	HRESULT hr = g_ExtClient->AttachProcess(0, process->Id, DEBUG_ATTACH_NONINVASIVE);

	hr = g_ExtControl->WaitForEvent(0, INFINITE);
	DebugContext::Initialize();
	String ^ path = Assembly::Load("mscorlib")->CodeBase->Replace("file:///", "");
	path = Directory::GetParent(path)->FullName;
	path = Path::Combine(path, "mscordacwks.dll");
	pin_ptr<const wchar_t> wch = PtrToStringChars(path);
	const size_t newsize = 1024;
	wchar_t wcstring[newsize];
    wcscpy_s(wcstring, wch);
	HMODULE hModule = ::LoadLibrary(wcstring);

	if(!hModule)
	{
		int err = ::GetLastError();
		throw gcnew Win32Exception(err, "LoadLibrary(mscordacwks.dll) failed.  Error = " + err + ".");
	}

	FARPROC pProc = ::GetProcAddress(hModule, "CLRDataCreateInstance");
	if (!pProc)
    {
        int err = ::GetLastError();
        throw gcnew Win32Exception(err, "GetProcAddress(CLRDataCreateInstance) failed.  Error = " + err + ".");
    }

	void* ifacePtr;
	REFIID iid = __uuidof(IXCLRDataProcess);

    PFN_CLRDataCreateInstance entry = (PFN_CLRDataCreateInstance)pProc;

	ICLRDataTarget* target = (ICLRDataTarget*)g_clrTarget;

    HRESULT status;    
    if ((status = entry(iid, target, &ifacePtr)) != S_OK)
    {
        FreeLibrary(hModule);
    }
    else
    {
        g_clrData = reinterpret_cast<IXCLRDataProcess*>(ifacePtr);
    }


	
}

ThreadStackManager^ Dac::ConstructThreadStacks()
{
	ThreadStackManager^ tsm = gcnew ThreadStackManager();
	tsm->CreateThreadStacks();
	return tsm;
}

Dac::~Dac()
{
	g_ExtClient->DetachProcesses();

	if(g_clrTarget)
	{
		delete g_clrTarget;
	}
}

}