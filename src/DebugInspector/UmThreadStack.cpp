#include "StdAfx.h"
#include "UmThreadStack.h"

#include "UmWaitBlock.h"
#include "UmWaitDeadLock.h"

#pragma managed
namespace UnmanagedInspector
{

ThreadStack::ThreadStack(ThreadStackManager^ tsm, ThreadStackWaitingOn waitingOn, Int32 debugThreadId,
					Int32 osThreadId, Int64 kThread, 	Int64 critSecAddress,
					Int32 timeout,  bool waitAll)

{
	r_tsm			= tsm;
	r_waitingOn		= waitingOn;
	r_debugThreadId	= debugThreadId;
	r_osThreadId	= osThreadId;
	r_kThread		= kThread;
	r_critSecAddress= critSecAddress;
	r_timeout		= timeout;
	r_waitAll		= waitAll;

	r_userSpaceWaitHandles = gcnew SortedDictionary<Int32, Int32>();
	r_frames = gcnew List<ThreadStackFrame^>();
	r_ownedObjects = gcnew SortedDictionary<Int64, WaitObject^>();
	r_waitBlocks = gcnew SortedDictionary<Int64, WaitBlock^>(); 
	r_waitDeadLocks = gcnew SortedDictionary<Int64, WaitDeadLock^>();

	_excludedFromDeadLockAnalysis = false;

}

void ThreadStack::AddUserSpaceWaitHandle(Int32 handle)
{
	r_userSpaceWaitHandles[handle] = handle;
}

ThreadStackFrame^ ThreadStack::AddFrame(String^ functionName, String^ fileName, 
								 Int32 lineNumber, Int64 instructionOffset, 
								 Int64 frameOffset, Int64 stackOffset, Int64 returnOffset, 
								 Int64 functTableEntry, array<Int64>^ params, array<Int64>^ reserved, 
								 Int32 frameNumber, bool isVirtual)
{
	ThreadStackFrame^ tsf = gcnew ThreadStackFrame(functionName, fileName, 
								 lineNumber, instructionOffset, 
								 frameOffset, stackOffset, returnOffset, functTableEntry,
								 params, reserved, frameNumber, isVirtual);

	r_frames->Add(tsf);
	return tsf;
}

void ThreadStack::AddOwnedObject(WaitObject^ ownedObject)
{
	r_ownedObjects[ownedObject->r_address] = ownedObject;
}

void ThreadStack::AddWaitBlock(WaitBlock^ waitBlock)
{
	r_waitBlocks[waitBlock->r_address] = waitBlock;
}

void ThreadStack::AddWaitDeadLock(WaitBlock^ waitBlock, WaitDeadLock^ waitDeadLock)
{
	r_waitDeadLocks[waitBlock->r_address] = waitDeadLock;
}

bool ThreadStack::ExcludedFromDeadlockAnalysis::get()
{
	return _excludedFromDeadLockAnalysis;
}

void ThreadStack::CalculateDeadLockCandidicy(bool% excluded)
{
	if(_excludedFromDeadLockAnalysis)
	{
		excluded = true;
		return;
	}

	bool bWaitAll      = false;
	bool bWaitAllFired = false;

	Dictionary<Int64, Int64>^ remaining = gcnew Dictionary<Int64, Int64>();
	for each(KeyValuePair<Int64, WaitBlock^>^ pair in r_waitBlocks)
	{
		Int64 address = pair->Key;
		bWaitAll = (pair->Value->r_waitType==WaitType::WaitAll);
		remaining[address] = address;
	}
	for each(KeyValuePair<Int64, WaitDeadLock^>^ wdlPair in r_waitDeadLocks)
	{
		Int64 wbAddress = wdlPair->Key;
		bool bExcluded = wdlPair->Value->_excludedFromDeadLockAnalysis; 
		bWaitAllFired |= !bExcluded;
		
		if(bExcluded && !bWaitAll)
		{
			_excludedFromDeadLockAnalysis = true;
			excluded = true;
			return;
		}
		remaining->Remove(wbAddress);
	}
	if(remaining->Count>0 && !bWaitAll)
	{
		_excludedFromDeadLockAnalysis = true;
		excluded = true;
		return;
	}
	if(bWaitAll && !bWaitAllFired)
	{
		_excludedFromDeadLockAnalysis = true;
		excluded = true;
		return;
	}
}

}
#pragma unmanaged
