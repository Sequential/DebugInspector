#pragma once
#include "UmThreadStackEnums.h"
#include "UmThreadStackFrame.h"
#include "UmThreadStackManager.h"

#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Collections::Generic;

public ref class ThreadStack
{
internal:
	initonly ThreadStackManager^  r_tsm;
	initonly ThreadStackWaitingOn r_waitingOn;
	initonly Int32	              r_debugThreadId;
	initonly Int32	              r_osThreadId;
	initonly Int64	              r_kThread;
	initonly Int64	              r_critSecAddress;
	initonly Int32	              r_timeout;
	initonly bool	              r_waitAll;

	initonly SortedDictionary<Int32, Int32>^			r_userSpaceWaitHandles;
	initonly List<ThreadStackFrame^>^			r_frames;
	initonly SortedDictionary<Int64, WaitObject^>^	r_ownedObjects;
	initonly SortedDictionary<Int64, WaitBlock^>^	    r_waitBlocks;
	initonly SortedDictionary<Int64, WaitDeadLock^>^   r_waitDeadLocks;

private:
	bool _excludedFromDeadLockAnalysis;

public:
	ThreadStack(ThreadStackManager^ tsm, ThreadStackWaitingOn waitingOn, Int32 debugThreadId,
					Int32 osThreadId, Int64 kThread, 	Int64 critSecAddress,
					Int32 timeout,  bool waitAll);
	property ThreadStackWaitingOn WaitingOn{ThreadStackWaitingOn  get(){return r_waitingOn; }}

	property Int32 DebugThreadId {Int32 get(){return r_debugThreadId; }}
	property Int32 OsThreadId	 {Int32 get(){return r_osThreadId;    }}
	property Int64 KTrhead		 {Int64 get(){return r_kThread;       }}
	property Int64 CritSecAddress{Int64 get(){return r_critSecAddress;}}
	property Int32 Timeout		 {Int32 get(){return r_timeout;       }}
	property bool  WaitAll		 {bool  get(){return r_waitAll;       }}
	
	property List<ThreadStackFrame^>^ StackFrames 
	{public: List<ThreadStackFrame^>^ get(){return r_frames;}}

	property SortedDictionary<Int64, WaitObject^>^ OwnedObjects
	{public: SortedDictionary<Int64, WaitObject^>^ get(){return r_ownedObjects;}}

	property SortedDictionary<Int64, WaitBlock^>^ WaitBlocks 
	{public: SortedDictionary<Int64, WaitBlock^>^ get(){return r_waitBlocks;}}

	property SortedDictionary<Int64, WaitDeadLock^>^ WaitDeadLocks 
	{public: SortedDictionary<Int64, WaitDeadLock^>^ get(){return r_waitDeadLocks;}}

internal:
	void AddUserSpaceWaitHandle(Int32 handle);
	void AddOwnedObject(WaitObject^ ownedObject);
	void AddWaitBlock(WaitBlock^ waitBlock);

	ThreadStackFrame^ AddFrame(String^ functionName, String^ fileName, 
								 Int32 lineNumber, Int64 instructionOffset, 
								 Int64 frameOffset, Int64 stackOffset, Int64 returnOffset, 
								 Int64 functTableEntry, array<Int64>^ params, array<Int64>^ reserved, 
								 Int32 frameNumber, bool isVirtual);

	void AddWaitDeadLock(WaitBlock^ waitBlock, WaitDeadLock^ waitDeadLock);



	void CalculateDeadLockCandidicy(bool% excluded);
	property bool ExcludedFromDeadlockAnalysis{bool get();};
};
}
#pragma unmanaged
