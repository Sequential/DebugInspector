#pragma once
#include "UmWaitObject.h"
#include "UmWaitBlock.h"
#include "UmThreadStackEnums.h"


#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Collections::Generic;

ref class ThreadStack;
ref class WaitDeadLockItem;
ref class WaitDeadLock;

public ref class ThreadStackManager
{
internal:
	Int64    _osProcessId;
	Int64    _kProcess;
	initonly Dictionary<Int64, WaitObject^>^  r_objectMap;
	initonly Dictionary<Int64, Int64>^		  r_handleToObjectMap;
	initonly Dictionary<Int64, Int64>^		  r_threadAddressToIdMap;
	initonly Dictionary<Int64, Int64>^		  r_threadIdToAddressMap;
	initonly Dictionary<Int64, Int32>^		  r_ownedObjectToThreadIdMap;
	initonly Dictionary<Int64, Int64>^		  r_ownedObjectToThreadAddressMap;
	initonly Dictionary<Int64, WaitBlock^>^   r_waitBlockMap;
	initonly Dictionary<Int64, ThreadStack^>^ r_threadStacks;


	initonly List<WaitDeadLock^>^ r_waitDeadLocks;
	initonly List<WaitDeadLock^>^ r_suspectDeadLocks;
	initonly List<WaitDeadLock^>^ r_finalDeadLocks;

	initonly List<WaitObject^>^ r_unOwnedObjects;

	int _loopCount;

private:
	initonly Dictionary<Int64, Int64>^ r_processedOwnedObjects;
internal:
	ThreadStackManager(void);
public:
	void Initialize(Int64 osProcessId, Int64 KProcess);

	Int64 GetThreadAddress(Int64 osThreadId)
	{
		if(r_threadIdToAddressMap->ContainsKey(osThreadId))
			return r_threadIdToAddressMap[osThreadId];

		return -1;
	}

	property Int64 ProcessId{public: Int64 get(){return _osProcessId;}}
	property Int64 KProcess {public: Int64 get(){return _kProcess;   }}
	property Int32 LoopCount{public: Int32 get(){return _loopCount;}}

	property Dictionary<Int64, WaitBlock^>^ WaitBlockMap 
	{public: Dictionary<Int64, WaitBlock^>^ get(){return r_waitBlockMap;}}

	property Dictionary<Int64, WaitObject^>^ ObjectMap 
	{public: Dictionary<Int64, WaitObject^>^ get(){return r_objectMap;}}

	property Dictionary<Int64, Int64>^ HandleToObjectMap 
	{public: Dictionary<Int64, Int64>^ get(){return r_handleToObjectMap;}}

	property Dictionary<Int64, Int64>^ ThreadAddressToIdMap 
	{public: Dictionary<Int64, Int64>^ get(){return r_threadAddressToIdMap;}}

	property Dictionary<Int64, Int64>^ ThreadIdToAddressMap 
	{public: Dictionary<Int64, Int64>^ get(){return r_threadIdToAddressMap;}}

	property Dictionary<Int64, Int32>^ OwnedObjectToThreadIdMap 
	{public: Dictionary<Int64, Int32>^ get(){return r_ownedObjectToThreadIdMap;}}

	property Dictionary<Int64, ThreadStack^>^ ThreadStackMap 
	{public: Dictionary<Int64, ThreadStack^>^ get(){return r_threadStacks;}}

	property List<WaitDeadLock^>^ SuspectWaitBlockChains 
	{public: List<WaitDeadLock^>^ get(){return r_suspectDeadLocks;}}

	property List<WaitDeadLock^>^ WaitDeadLocks 
	{public: List<WaitDeadLock^>^ get(){return r_waitDeadLocks;}}

	property List<WaitDeadLock^>^ ThreadDeadLocks 
	{public: List<WaitDeadLock^>^ get(){return r_finalDeadLocks;}}

	property List<WaitObject^>^ UnownedObjects 
	{public: List<WaitObject^>^ get(){return r_unOwnedObjects;}}




internal:
	ThreadStack^  AddThreadStack(ThreadStackWaitingOn waitingOn, Int32 debugThreadId,
					Int32 osThreadId, Int64 kThread, 	Int64 critSecAddress,
					Int32 timeout,  bool waitAll);

	void AddThreadId(Int64 threadAddress, Int64 threadId)
	{
		r_threadAddressToIdMap[threadAddress] = threadId;
		r_threadIdToAddressMap[threadId] = threadAddress;
	}
	void PostProcess();
	void DetectDeadlocks();
private:
	static WaitDeadLock^ CreateDeadLockFromChain(Stack<WaitDeadLockItem^>^ dliStack);
	void TraverseWaitDeadLockChain(WaitDeadLockItem^ head);	
	void AddWaitDeadLock(WaitDeadLock^ deadLock);
	void AddSuspsectDeadLock(WaitDeadLock^ deadLock);
	int  IlliminateDeadLockChains();

	void Print();

};
}
#pragma unmanaged
