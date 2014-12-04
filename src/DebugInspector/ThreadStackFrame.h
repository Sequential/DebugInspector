#pragma once
#include "ThreadStackFrameArg.h"
#include "ThreadStackFrameLocal.h"

#pragma managed
namespace ClrInspector
{
	using namespace System;
	using namespace System::Collections::Generic;

public enum ThreadStackFrameStatus
{
	Ok						= 0,
	PossibleUnmanagedThread	= 1,
	UninitializedStack		= 2,
	GetFrameDataFailed		= 3,
	GetContextDataFailed	= 4,
	Interrupted				= 5
};

public ref class ThreadStackFrame
{
internal:
	initonly UInt32	 r_Esp;
	initonly UInt32	 r_Eip;
	initonly UInt64  r_Address;
	initonly UInt64  r_StackHeaderOffset;
	initonly UInt64  r_WaitingOn;
	initonly bool    r_InMonitorEnter;
	initonly bool    r_Managed;
	initonly Int32   r_ArgCount;
	initonly Int32   r_LocalsCount;
	initonly String^ r_FrameName;
	initonly String^ r_MethodName;
	initonly String^ r_WatingOnName;
	initonly String^ r_ModuleFileName;
	initonly String^ r_FileName;
	initonly UInt32  r_Line;

	initonly ThreadStackFrameStatus r_Status;

	initonly List<ThreadStackFrameArg^>^ r_FrameArgs; 
	initonly List<ThreadStackFrameLocal^>^ r_FrameLocals;
	
	//ClrStackFrameLocal* pFirstLocal;

internal:
	ThreadStackFrame(UInt32 esp, UInt32 eip, UInt64 address, UInt64 waitingOn, 
					 bool inMonitorEnter, bool managed, Int32 argCount, Int32 localsCount, 
					 String^ frameName, String^ methodName, String^ waitingOnName, 
					 String^ fileName, UInt32 line, String^ moduleFileName, ThreadStackFrameStatus status, UInt64 stackHeaderOffset);
public:
	property UInt32  Esp			{UInt32  get(){return r_Esp;}}
	property UInt32  Eip			{UInt32  get(){return r_Eip;}}
	property UInt64  Address		{UInt64  get(){return r_Address;}}
	property UInt64  WaitingOn		{UInt64  get(){return r_WaitingOn;}}
	property UInt64  Offset		    {UInt64  get(){return r_StackHeaderOffset;}}
	property bool    InMonitorEnter	{bool    get(){return r_InMonitorEnter;}}
	property bool    Managed	    {bool    get(){return r_Managed;}}
	property UInt32  ArgCount		{UInt32  get(){return r_ArgCount;}}
	property UInt32  LocalsCount	{UInt32  get(){return r_LocalsCount;}}
	property String^ FrameName		{String^ get(){return r_FrameName;}}
	property String^ MethodName		{String^ get(){return r_MethodName;}}
	property String^ WatingOnName	{String^ get(){return r_WatingOnName;}}
	property String^ FileName		{String^ get(){return r_FileName;}}
	property UInt32  Line			{UInt32  get(){return r_Line;}}
	property String^ ModuleFileName {String^ get(){return r_ModuleFileName;}}

	property ThreadStackFrameStatus Status {ThreadStackFrameStatus get(){return r_Status;}}


	property String^ ModuleName 
	{
		String^ get()
		{
			int length = r_ModuleFileName->Length;

			if(length==0)
				return  r_ModuleFileName;

			int index = r_ModuleFileName->LastIndexOf("\\");
			if(index>=0 && index < length)
			{
				return r_ModuleFileName->Substring(index+1, length - index - 1);
			}
			return r_ModuleFileName;
		}
	}

internal:
	void AddArg(ThreadStackFrameArg^ arg);
	void AddLocal(ThreadStackFrameLocal^ arg);
	

	

    

};

}
#pragma unmanaged