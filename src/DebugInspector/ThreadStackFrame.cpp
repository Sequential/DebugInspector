#include "StdAfx.h"
#include "ThreadStackFrame.h"

using namespace System;
#pragma managed
namespace ClrInspector
{


ThreadStackFrame::ThreadStackFrame(UInt32 esp, UInt32 eip, UInt64 address, UInt64 waitingOn, 
					 bool inMonitorEnter, bool managed, Int32 argCount, Int32 localsCount, 
					 String^ frameName, String^ methodName, String^ waitingOnName, 
					 String^ fileName, UInt32 line, String^ moduleFileName, ThreadStackFrameStatus status, UInt64 stackHeaderOffset)
{
	r_Esp			= esp;
	r_Eip			= eip;
	r_Address		= address;
	r_WaitingOn		= waitingOn;
	r_InMonitorEnter= inMonitorEnter;
	r_Managed       = managed;
	r_ArgCount		= argCount;
	r_LocalsCount	= localsCount;
	r_FrameName		= frameName;
	r_MethodName	= methodName;
	r_WatingOnName	= waitingOnName;
	r_Status		= status;
	r_FileName		= fileName;
	r_Line			= line;
	r_ModuleFileName= moduleFileName;
	r_StackHeaderOffset = stackHeaderOffset;

	r_FrameArgs   = gcnew List<ThreadStackFrameArg^>();
	r_FrameLocals = gcnew List<ThreadStackFrameLocal^>();
}


void ThreadStackFrame::AddArg(ThreadStackFrameArg^ arg)
{
	r_FrameArgs->Add(arg);
}

void ThreadStackFrame::AddLocal(ThreadStackFrameLocal^ local)
{
	r_FrameLocals->Add(local);
}

}
#pragma unmanaged