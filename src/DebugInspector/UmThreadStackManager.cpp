#include "StdAfx.h"
#include "UmThreadStackManager.h"
#include "UmThreadStack.h"
#include "UmWaitDeadLock.h"
#include "UmWaitDeadLockItem.h"

#include "stdio.h"


#pragma managed
namespace UnmanagedInspector
{
ThreadStackManager::ThreadStackManager(void)
{
	r_objectMap = gcnew Dictionary<Int64, WaitObject^>();
	r_handleToObjectMap = gcnew Dictionary<Int64, Int64>();
	r_threadAddressToIdMap = gcnew Dictionary<Int64, Int64>();		 
	r_threadIdToAddressMap = gcnew Dictionary<Int64, Int64>();	
	r_ownedObjectToThreadIdMap = gcnew Dictionary<Int64, Int32>();
	r_ownedObjectToThreadAddressMap = gcnew Dictionary<Int64, Int64>();
	r_waitBlockMap = gcnew Dictionary<Int64, WaitBlock^>();  
	r_threadStacks= gcnew Dictionary<Int64, ThreadStack^>(); 
	r_unOwnedObjects = gcnew List<WaitObject^>();
	r_processedOwnedObjects = gcnew Dictionary<Int64, Int64>();	
	r_waitDeadLocks = gcnew List<WaitDeadLock^>();
	r_suspectDeadLocks = gcnew List<WaitDeadLock^>();
	r_finalDeadLocks = gcnew List<WaitDeadLock^>();
	_loopCount = 0;
	
}

void ThreadStackManager::Initialize(Int64 osProcessId, Int64 KProcess)
{
	_osProcessId = osProcessId;
	_kProcess    = KProcess;
	_loopCount   = 0;
}

ThreadStack^  ThreadStackManager::AddThreadStack(ThreadStackWaitingOn waitingOn, Int32 debugThreadId,
					Int32 osThreadId, Int64 kThread, 	Int64 critSecAddress,
					Int32 timeout,  bool waitAll)
{
	ThreadStack^ ts = gcnew ThreadStack(this, waitingOn, debugThreadId,
							osThreadId, kThread, critSecAddress, timeout, waitAll);
	r_threadStacks[osThreadId] = ts;
	return ts;
}

void ThreadStackManager::PostProcess()
{
	for each(KeyValuePair<Int64, Int64>^ item in r_handleToObjectMap)
	{
		Int64 handle		= item->Key;
		Int64 objectAddress = item->Value;

		WaitObject^ wo = nullptr;
		
		if(r_objectMap->TryGetValue(objectAddress, wo))
		{
			wo->_userHandle = handle;
		}
	}

	for each(KeyValuePair<Int64, Int64>^ pair in r_threadAddressToIdMap)
	{
		r_ownedObjectToThreadIdMap[pair->Key] = (Int32)pair->Value;
	}

	for each(KeyValuePair<Int64, Int64>^ pair  in r_ownedObjectToThreadAddressMap)
	{
		r_ownedObjectToThreadIdMap[pair->Key] = r_threadAddressToIdMap[pair->Value];
	}

	for each(KeyValuePair<Int64, Int32>^ item in r_ownedObjectToThreadIdMap)
	{
		Int64 objectAddres  = item->Key;
		Int32 threadId = item->Value;

		WaitObject^ wo = r_objectMap[objectAddres];
		ThreadStack^ ts = r_threadStacks[threadId];
		ts->AddOwnedObject(wo);
	}

	for each(WaitBlock^ item in r_waitBlockMap->Values)
	{
		Int64 threadAddress = item->r_threadAddress;
		Int64 threadId = r_threadAddressToIdMap[threadAddress];
		ThreadStack^ ts = r_threadStacks[threadId];
		ts->AddWaitBlock(item);
	}

	for each(KeyValuePair<Int64, WaitObject^>^ pair in r_objectMap)
	{
		if(!r_ownedObjectToThreadIdMap->ContainsKey(pair->Key))
		{
			r_unOwnedObjects->Add(pair->Value);
		}
	}
}

void ThreadStackManager::Print()
{
	for each(ThreadStack^ threadStack in r_threadStacks->Values)
	{
		printf("Stack DBGID==%d ", threadStack->r_debugThreadId);
		printf("OSID==%d \n", threadStack->r_osThreadId);
		for each(ThreadStackFrame^ frame in threadStack->r_frames)
		{
			printf("%s \n", frame->r_functionName);
		}
		printf("\n");
	}


	for each(ThreadStack^ threadStack in r_threadStacks->Values)
	{
		for each(WaitBlock^ waitBlock in threadStack->r_waitBlocks->Values)
		{
			Int64 threadAddress = waitBlock->r_threadAddress;
			Int64 objectAddress = waitBlock->r_objectAddress;
			Int64 threadId = r_threadAddressToIdMap[threadAddress];
			ThreadStack^ tsA = r_threadStacks[threadId];
			Int32 owningThreadId = 0;
			r_ownedObjectToThreadIdMap->TryGetValue(objectAddress, owningThreadId);

			printf("WT: %08X ", threadAddress);
			printf("OS: %04X ", threadId);
			printf("OBJ: %08X ", objectAddress);
			printf("OWN: %08X \n", owningThreadId);
			
		}
	}

}
void ThreadStackManager::DetectDeadlocks()
{
	Print();
	// Do The WaitBlock Deadlocks
	r_processedOwnedObjects->Clear();
	for each(ThreadStack^ threadStack in r_threadStacks->Values)
	{
		for each(WaitBlock^ waitBlock in threadStack->r_waitBlocks->Values)
		{
			Int64 threadAddress = waitBlock->r_threadAddress;
			Int64 objectAddress = waitBlock->r_objectAddress;
			Int64 threadId = r_threadAddressToIdMap[threadAddress];
			ThreadStack^ tsA = r_threadStacks[threadId];
			Int32 owningThreadId = 0;
			if(r_ownedObjectToThreadIdMap->TryGetValue(objectAddress, owningThreadId))
			{
				if(!r_processedOwnedObjects->ContainsKey(objectAddress))
				{
					WaitObject^ woo = r_objectMap[objectAddress];
					ThreadStack^ tsB = r_threadStacks[owningThreadId];
					WaitDeadLockItem^ head = gcnew WaitDeadLockItem(tsA, woo, tsB, waitBlock);
					TraverseWaitDeadLockChain(head);
				}
			}
		}
	}

	// Now notify the participating threads
	for each(WaitDeadLock^ deadLock in r_waitDeadLocks)
	{
		for each(WaitDeadLockItem^ item in deadLock->r_items)
		{
			item->r_waitingThread->AddWaitDeadLock(item->r_waitBlock, deadLock);
		}
	}

	// Constantly Loop until all that remains is invariant
	_loopCount = IlliminateDeadLockChains();

	for each(WaitDeadLock^ deadLock in r_waitDeadLocks)
	{
		if(!deadLock->_excludedFromDeadLockAnalysis)
		{
			bool bExcluded = false;
			for each(WaitDeadLockItem^ item in deadLock->r_items)
			{
				if(item->r_waitingThread->ExcludedFromDeadlockAnalysis ||
				   item->r_owningThread->ExcludedFromDeadlockAnalysis)
				{
					bExcluded = true;
					break;
				}
			}
			if(!bExcluded)
			{
				r_finalDeadLocks->Add(deadLock);
			}
		}
	}
}

int ThreadStackManager::IlliminateDeadLockChains()
{
	bool bRunAgain;
	int i=0;
	do
	{
		++i;
		bRunAgain = false;

		for each(ThreadStack^ threadStack in r_threadStacks->Values)
		{
			if(!threadStack->ExcludedFromDeadlockAnalysis)
			{
				bool bExcluded = false;
				threadStack->CalculateDeadLockCandidicy(bExcluded);
				if(bExcluded)
				{
					bRunAgain = true;
				}
			}
		}

		for each(WaitDeadLock^ deadLock in r_waitDeadLocks)
		{
			if(!deadLock->_excludedFromDeadLockAnalysis)
			{
				bool bExcluded = false;
				for each(WaitDeadLockItem^ item in deadLock->r_items)
				{
					if(item->r_waitingThread->ExcludedFromDeadlockAnalysis ||
					   item->r_owningThread->ExcludedFromDeadlockAnalysis)
					{
						bExcluded = true;
						break;
					}
				}
				if(bExcluded)
				{
					bRunAgain = true;
					deadLock->_excludedFromDeadLockAnalysis = true;
				}
			}
		}
	}
	while(bRunAgain);
	return i;
}

void ThreadStackManager::TraverseWaitDeadLockChain(WaitDeadLockItem^ head)
{
	Int64 headObjectAddress = head->r_object->r_address;
	
	Stack<IEnumerator<WaitBlock^>^>^ iterStack = gcnew Stack<IEnumerator<WaitBlock^>^>();
	Stack<WaitDeadLockItem^>^ dliStack = gcnew Stack<WaitDeadLockItem^>();
	Stack<Dictionary<Int64, Int64>^>^ ownedAndProcessedStack = gcnew Stack<Dictionary<Int64, Int64>^>();

	IEnumerator<WaitBlock^>^ iter = head->r_owningThread->r_waitBlocks->Values->GetEnumerator();

	dliStack->Push(head);
	iterStack->Push(iter);
	ownedAndProcessedStack->Push(gcnew Dictionary<Int64, Int64>());

	while(iterStack->Count>0)
	{
		IEnumerator<WaitBlock^>^ iter = iterStack->Peek();
		
		if(!iter->MoveNext())
		{
			iterStack->Pop();
			dliStack->Pop();
			ownedAndProcessedStack->Pop();
		}
		else
		{
			Dictionary<Int64, Int64>^ oap = ownedAndProcessedStack->Peek();

			WaitBlock^ wb = iter->Current;
			Int64 threadAddress = wb->r_threadAddress;
			Int64 objectAddress = wb->r_objectAddress;
			Int64 threadId = r_threadAddressToIdMap[threadAddress];
			ThreadStack^ tsA = r_threadStacks[threadId];

			Int32 owningThreadId = 0;
			if(r_ownedObjectToThreadIdMap->TryGetValue(objectAddress, owningThreadId))
			{
				WaitObject^ woo = r_objectMap[objectAddress];
				ThreadStack^ tsB = r_threadStacks[owningThreadId];
				WaitDeadLockItem^ dli = gcnew WaitDeadLockItem(tsA, woo, tsB, wb);
				dliStack->Push(dli);

				if(objectAddress==headObjectAddress)
				{
					AddWaitDeadLock(CreateDeadLockFromChain(dliStack));
					dliStack->Pop();
				}
				else if(!oap->ContainsKey(objectAddress)) // Inner Deadlock Protection
				{
					oap->Add(objectAddress, objectAddress);
					Dictionary<Int64, Int64>^ oapNextLevel = gcnew Dictionary<Int64, Int64>(oap);
					ownedAndProcessedStack->Push(oapNextLevel);
					iterStack->Push(tsB->r_waitBlocks->Values->GetEnumerator());
				}	
			}
			else
			{
				AddSuspsectDeadLock(CreateDeadLockFromChain(dliStack));
			}
		}
	}
}

WaitDeadLock^ ThreadStackManager::CreateDeadLockFromChain(Stack<WaitDeadLockItem^>^ dliStack)
{
	WaitDeadLock^ dl = gcnew WaitDeadLock();
	for each(WaitDeadLockItem^ dli in dliStack)
	{
		dl->Add(dli);
	}
	return dl;
}
void ThreadStackManager::AddWaitDeadLock(WaitDeadLock^ deadLock)
{
	r_waitDeadLocks->Add(deadLock);

	for each(WaitDeadLockItem^ item in deadLock->r_items)
	{
		Int64 address = item->r_object->r_address;
		r_processedOwnedObjects[address] = address;
	}
}
void ThreadStackManager::AddSuspsectDeadLock(WaitDeadLock^ deadLock)
{
	r_suspectDeadLocks->Add(deadLock);
}

}
#pragma unmanaged