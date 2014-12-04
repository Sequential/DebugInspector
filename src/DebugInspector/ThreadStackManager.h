#pragma once
#include "SyncBlock.h"
#include "SyncBlockManager.h"
#include "ThreadStack.h"
#include "DeadLockItem.h"
#include "DeadLock.h"

using namespace System;
using namespace System::Collections::Generic;

#pragma managed
namespace ClrInspector
{

public ref class ThreadStackManager 
{
private:
	initonly SyncBlockManager^ r_SyncBlockManager;
	initonly List<ThreadStack^>^ r_ThreadStacks;
	initonly Dictionary<UInt64, ThreadStack^>^ r_ownedObjects; 
	initonly Dictionary<UInt64, String^>^ r_ownedNames; 
	initonly List<DeadLock^>^ r_DeadLocks;
	initonly Dictionary<UInt64, UInt64>^ r_processedOwnedObjects;
	
public:
	ThreadStackManager(void);
	void CreateThreadStacks();

	property List<ThreadStack^>^  ThreadStacks {List<ThreadStack^>^  get(){return r_ThreadStacks;}}
	property List<DeadLock^>^     DeadLocks    {List<DeadLock^>^     get(){return r_DeadLocks;}}

	String^ GetOwnedName(UInt64 object)
	{
		String^ name;
		r_ownedNames->TryGetValue(object, name); 
		return name;
	}

internal:

	void AddOwnedObject(UInt64 object, String^ name, ThreadStack^ threadStack);
	void DetectDeadLocks();

private:
	void TraverseDeadLockChain(DeadLockItem^ head);
	void AddDeadLock(DeadLock^ deadLock);
};
}
#pragma unmanaged