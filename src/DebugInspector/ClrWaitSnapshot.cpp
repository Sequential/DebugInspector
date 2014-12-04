#include "StdAfx.h"
#include "ClrWaitSnapshot.h"
#include "DebugEventCallbacks.h"
#include "DebugInputCallbacks.h"
#include "DebugOutputCallbacks.h"
#include "Kernel.h"
#include "Process.h"
#include "Dac.h"

#include "UmWaitObject.h"
#include "UmThreadStackManager.h"
#include "UmThreadStackEnums.h"
#include "UmThreadStack.h"

#pragma managed
namespace UnmanagedInspector
{

	using namespace System::Security::Principal;

ClrWaitSnapshot::ClrWaitSnapshot(Diag::Process^ process)
{
	_process = process;

	ULONG version;
	ULONG flags;
	DebugExtensions::Initialize(&version, &flags);
}

ThreadStackManager^ ClrWaitSnapshot::Initialize()
{
	HRESULT hr = S_OK;

	//IDebugInputCallbacks* pDICB = new DebugInputCallbacks();
	//IDebugOutputCallbacks* pDOCB = new DebugOutputCallbacks();
	//hr = g_ExtClient->SetInputCallbacks(pDICB);
	//hr = g_ExtClient->SetOutputCallbacks(pDOCB);

	// TODO: Symbol Server
	char* symbolpath="srv*C:\\Windows\\Symbols*http://msdl.microsoft.com/download/symbols;";
	//char* symbolpath="srv*C:\\Windows\\Symbols*http://msdl.microsoft.com/download/symbols;";
	//char* symbolpath="C:\\Windows\\Symbols;";

	hr = g_ExtSymbols->SetSymbolPath(symbolpath);

	//hr = g_ExtSymbols->AddSymbolOptions(SYMOPT_DEBUG);

	// must set
	//_NT_SYMBOL_PROXY
	//_NT_SYMBOL_PATH 
	//hr = g_ExtSymbols->Reload("/f mscorwks.dll");

	DebugEventCallbacks eventCallabacks;
	IDebugEventCallbacks* pCb = &eventCallabacks;

	hr = g_ExtClient->SetEventCallbacks(pCb);

	WaitSnapshot snapshot;
	Kernel kernel(_process->Id);
	hr = kernel.GetWaitChainInfo(snapshot);
	bool noLocalKernelDebugger= false;
	if(FAILED(hr))
	{
		noLocalKernelDebugger = !kernel.DidAttachKernel();;
	}

	hr = g_ExtClient->DetachProcesses();
	hr = g_ExtClient->EndSession(0);

	if(noLocalKernelDebugger)
	{
		throw gcnew Exception(kernel.LastError() + " - Local Kernel Debugging is Not available. You need the Debug privilidge or to be an Admin. Vista users need to boot up with the /debug switch");
	}

	ThreadStackManager ^ tsm = gcnew ThreadStackManager();
	ProcessWaitSnapshot(snapshot, tsm);

	hr = g_ExtSymbols->SetSymbolPath(symbolpath);

	hr = g_ExtClient->SetEventCallbacks(pCb);
	hr = g_ExtClient->AttachProcess(0L, _process->Id, DEBUG_ATTACH_NONINVASIVE);
	hr = g_ExtControl->WaitForEvent(0, INFINITE);

	ULONG ThreadCount;
	hr = g_ExtSystem->GetNumberThreads(&ThreadCount);

	ULONG* pIds    = new ULONG[ThreadCount];
	ULONG* pSysIds = new ULONG[ThreadCount];
	hr = g_ExtSystem->GetThreadIdsByIndex(0, ThreadCount, pIds, pSysIds);

	for(ULONG i=0;i<ThreadCount;++i)
	{
		ULONG id = pIds[i];
		ULONG sysId = pSysIds[i];
		DebugStackTrace st;
		st.Initialize(id, sysId);
		ProcessDebugStackTrace(st, tsm);
	}
	hr = g_ExtClient->DetachProcesses();
	hr = g_ExtClient->EndSession(0);

	tsm->PostProcess();
	tsm->DetectDeadlocks();
	return tsm;
}

void ClrWaitSnapshot::ProcessWaitSnapshot(WaitSnapshot& snapshot, ThreadStackManager ^ tsm)
{
	tsm->Initialize(snapshot._OSProcessId, snapshot._KProcess);

	{
		ObjectBuffer& objectBuffer = snapshot._objectBuffer;
		ObjectEntry*  pObjectEntry = objectBuffer._firstBuffer;
		int index = 0;
		for(int i=0;i<objectBuffer._size;++i)
		{
			ObjectEntry*  pCurrent = (pObjectEntry+index);
			Int64 address = (pCurrent)->Address;
			String^ name = gcnew String(pCurrent->Name);
			WaitObject^ wo = gcnew WaitObject(address, pCurrent->Type, 
											  pCurrent->Size, pCurrent->SignalState, name);
			
			tsm->r_objectMap->default[address] = wo;

			if(++index== ObjectBuffer::MAX_OFFSET)
			{
				pObjectEntry = (ObjectEntry* )(pObjectEntry+ObjectBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}
	{
		HandleBuffer& handleBuffer = snapshot._handleBuffer;
		HandleEntry* pHandleEntry = handleBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<handleBuffer._size;++i)
		{
			HandleEntry*  pCurrent = (pHandleEntry+index);

			Int64 address = pCurrent->Address;
			Int64 handle = (ULONG64)pCurrent->Handle;
			
			tsm->r_handleToObjectMap->default[handle] = address;

			if(++index== HandleBuffer::MAX_OFFSET)
			{
				pHandleEntry = (HandleEntry* )(pHandleEntry+HandleBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}

	{
		OwnedBuffer& ownedBuffer = snapshot._ownedBuffer;
		OwnedEntry* pOwnedEntry = ownedBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<ownedBuffer._size;++i)
		{
			OwnedEntry*  pCurrent = (pOwnedEntry+index);

			Int64 address = pCurrent->Address;
			Int64 threadAddress = pCurrent->ThreadAddress;

			tsm->r_ownedObjectToThreadAddressMap->default[address] = threadAddress;

			if(++index== OwnedBuffer::MAX_OFFSET)
			{
				pOwnedEntry = (OwnedEntry* )(pOwnedEntry+OwnedBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}

	{
		HandleBuffer& threadBuffer = snapshot._threadBuffer;
		HandleEntry* pThreadEntry = threadBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<threadBuffer._size;++i)
		{
			HandleEntry*  pCurrent = (pThreadEntry+index);

			Int64 address = pCurrent->Address;
			Int64 handle = (ULONG64)pCurrent->Handle;
			
			tsm->AddThreadId(address, handle);

			if(++index== HandleBuffer::MAX_OFFSET)
			{
				pThreadEntry = (HandleEntry* )(pThreadEntry+HandleBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}

	{
		WaitBlockBuffer& waitBuffer = snapshot._waitBuffer;
		WaitBlockEntry* pWaitEntry = waitBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<waitBuffer._size;++i)
		{
			WaitBlockEntry*  pCurrent = (pWaitEntry+index);

			Int64 address = pCurrent->Address;
			Int64 objectAddress = pCurrent->ObjectAddress;
			Int64 threadaddress = pCurrent->ThreadAddress;
			Int16 waitKey = pCurrent->WaitKey;
			WaitType waitType = (WaitType)pCurrent->WaitType; 
			
			WaitBlock^ wb = gcnew WaitBlock(address, objectAddress, threadaddress, waitKey, waitType);
			
			tsm->r_waitBlockMap->default[address] = wb;

			if(++index== WaitBlockBuffer::MAX_OFFSET)
			{
				pWaitEntry = (WaitBlockEntry* )(pWaitEntry+WaitBlockBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}
}

void ClrWaitSnapshot::ProcessDebugStackTrace(DebugStackTrace& debugStackTrace, ThreadStackManager ^ tsm)
{
	ThreadStackWaitingOn wo = (ThreadStackWaitingOn)(int)debugStackTrace._waitType;

	Int64 kThread = tsm->GetThreadAddress(debugStackTrace._systemThreadId);
	
	ThreadStack^ ts = tsm->AddThreadStack(wo, debugStackTrace._debugThreadId,
		debugStackTrace._systemThreadId, kThread, debugStackTrace._critSecAddress, 
		debugStackTrace._timeout, debugStackTrace._waitType!=0);

	{
		HandleBuffer& owningThreadBuffer = debugStackTrace._owningThreadBuffer;
		HandleEntry* pOwningThreadEntry = owningThreadBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<owningThreadBuffer._size;++i)
		{
			HandleEntry*  pCurrent = (pOwningThreadEntry+index);

			Int64 osThreadId = pCurrent->Address;
			Int64 handle = (ULONG64)pCurrent->Handle;

			Int64 objAddress = 0;
			if(tsm->r_handleToObjectMap->TryGetValue(handle,objAddress))
			{
				tsm->r_ownedObjectToThreadIdMap[objAddress] = (Int32)osThreadId;
			}
			else
			{
				// TODO: Add Error Log
			}

			if(++index== HandleBuffer::MAX_OFFSET)
			{
				pOwningThreadEntry = (HandleEntry* )(pOwningThreadEntry+HandleBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}

	{
		HandleBuffer& handleBuffer = debugStackTrace._handleBuffer;
		HandleEntry* pHandleEntry = handleBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<handleBuffer._size;++i)
		{
			HandleEntry*  pCurrent = (pHandleEntry+index);

			Int32 handle = (ULONG32)pCurrent->Handle;

			ts->AddUserSpaceWaitHandle(handle);

			if(++index== HandleBuffer::MAX_OFFSET)
			{
				pHandleEntry = (HandleEntry* )(pHandleEntry+HandleBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}

	{
		DebugStackFrameBuffer& frameBuffer = debugStackTrace._frameBuffer;
		DebugStackFrameEntry* pFrameEntry = frameBuffer._firstBuffer;

		int index = 0;
		for(int i=0;i<frameBuffer._size;++i)
		{
			DebugStackFrameEntry*  pCurrent = (pFrameEntry+index);

			String^ functionName = gcnew String(pCurrent->FunctionName);
			String^ fileName     = gcnew String(pCurrent->FileName);
			Int32	lineNumber = pCurrent->LineNo;
			
			DEBUG_STACK_FRAME& frame = pCurrent->DebugFrame;

			Int64   instructionOffset	= frame.InstructionOffset;
			Int64   frameOffset			= frame.FrameOffset;
			Int64   stackOffset			= frame.StackOffset;
			Int64   returnOffset		= frame.ReturnOffset;
			Int64   functTableEntry		= frame.FuncTableEntry;
			Int32   frameNumber			= frame.FrameNumber;
			bool    isVirtual			= frame.Virtual!=0;

			array<Int64>^ params   = gcnew array<Int64>(4);
			array<Int64>^ reserved = gcnew array<Int64>(6);


			params[0] = frame.Params[0];
			params[1] = frame.Params[1];
			params[2] = frame.Params[2];
			params[3] = frame.Params[3];

			reserved[0] = frame.Reserved[0];
			reserved[1] = frame.Reserved[1];
			reserved[2] = frame.Reserved[2];
			reserved[3] = frame.Reserved[3];
			reserved[4] = frame.Reserved[4];
			reserved[5] = frame.Reserved[5];

			ts->AddFrame(functionName, fileName, lineNumber,
						 instructionOffset, frameOffset, stackOffset, returnOffset, 
						 functTableEntry, params, reserved, frameNumber, isVirtual);

			if(++index== DebugStackFrameBuffer::MAX_OFFSET)
			{
				pFrameEntry = (DebugStackFrameEntry* )(pFrameEntry+DebugStackFrameBuffer::MAX_OFFSET)->Address;
				index = 0;
			}
		}
	}
}

}
#pragma unmanaged
