#include "StdAfx.h"
#include "ThreadStackManager.h"
#include "ClrThreadWalker.h"
#include "ClrStackFrame.h"
#include "ClrStackWalker.h"

#pragma managed
namespace ClrInspector
{


ThreadStackManager::ThreadStackManager(void)
{
	r_SyncBlockManager = gcnew SyncBlockManager();
	r_ThreadStacks = gcnew List<ThreadStack^>();
	r_ownedObjects = gcnew Dictionary<UInt64, ThreadStack^>();
	r_ownedNames = gcnew Dictionary<UInt64, String^>();
	r_DeadLocks = gcnew List<DeadLock^>();
	r_processedOwnedObjects = gcnew Dictionary<UInt64, UInt64>(); 
}

void ThreadStackManager::CreateThreadStacks(void)
{
	r_SyncBlockManager->RetrieveSyncBlocks();

	ClrThreadWalker tw;
	tw.Initialize();

	HRESULT hr = S_OK;
	
	while((hr = tw.Next()) == S_OK)
	{	
		ClrStackFrame* pSf = NULL;

		try
		{
			ClrStackWalker sw;
			hr = sw.Create(&pSf);
			String^ domainName = gcnew String(tw.DomainName());
			ThreadStack^ ts = gcnew ThreadStack(tw.CorThreadId(), tw.OsThreadId(), tw.DomainId(), domainName);
			ts->AddOwnedSyncBlocks(r_SyncBlockManager, this);
			
			ClrStackFrame* pStack = pSf;

			while(pStack)
			{
				String^ FrameName = gcnew String(pStack->pFrameName);
				String^ MethodName = gcnew String(pStack->pMethodName);
				String^ WatingOnName = gcnew String(pStack->pWatingOnName);
				String^ FileName = gcnew String(pStack->pFileName);
				String^ ModuleFileName = gcnew String(pStack->pModuleFileName);

				ThreadStackFrame^ frame = gcnew ThreadStackFrame(pStack->Esp, pStack->Eip, pStack->Address, pStack->WaitingOn, 
																pStack->InMonitorEnter>0, pStack->Managed>0, pStack->ArgCount, pStack->LocalsCount, 
																FrameName, MethodName, WatingOnName, FileName , pStack->Line, ModuleFileName, 
																(ThreadStackFrameStatus)pStack->Status, pStack->StackHeaderOffset);
				ts->AddFrame(frame);
				pStack = pStack->pNextFrame;

			}
			r_ThreadStacks->Add(ts);
		}
		catch(Exception^ e)
		{
			String^ s =e->ToString();
			throw;
		}
		finally
		{
			if(pSf)
			{
				delete pSf;
			}
		}
	}
	DetectDeadLocks();
}


void ThreadStackManager::AddOwnedObject(UInt64 object, String^ name , ThreadStack^ threadStack)
{
	r_ownedObjects->Add(object, threadStack);
	r_ownedNames->Add(object, name);
}

void ThreadStackManager::DetectDeadLocks()
{
	for each(ThreadStack^ stack in r_ThreadStacks)
	{
		for each(UInt64 objectOwned in stack->r_SyncBlockObjectsOwned)
		{
			if(! (r_processedOwnedObjects->ContainsKey(objectOwned)))
			{
				DeadLockItem ^ dli = gcnew DeadLockItem(stack, objectOwned, stack->_SyncBlockObjectWaitedOn); 
				TraverseDeadLockChain(dli);
			}
		}
	}
}

void ThreadStackManager::TraverseDeadLockChain(DeadLockItem^ head)
{
	DeadLock^ dl = gcnew DeadLock();
	dl->Add(head);

	Dictionary<UInt64, UInt64>^ owned = gcnew Dictionary<UInt64, UInt64>();

	UInt64 mainOwned = head->r_objectOwned;
	UInt64 waitingOn = head->r_objectWaitingOn;

	owned->Add(mainOwned, mainOwned);

	while(waitingOn>0)
	{
		if(r_ownedObjects->ContainsKey(waitingOn))
		{
			ThreadStack^ ts = r_ownedObjects[waitingOn];
			UInt64 nextWaitingOn = ts->_SyncBlockObjectWaitedOn;
			
			DeadLockItem ^ dli = gcnew DeadLockItem(ts, waitingOn, nextWaitingOn);
			dl->Add(dli);

			if(mainOwned==nextWaitingOn)
			{
				AddDeadLock(dl);
				break;
			}
			else if(owned->ContainsKey(nextWaitingOn)) // We have stumbled into an inner deadlock
			{
				break;
			}
			waitingOn = nextWaitingOn;
			owned->Add(waitingOn, waitingOn);
		}
		else
		{
			throw gcnew Exception("How can we wait on " + waitingOn + " when it is not owned?");
		}
	}
}

void ThreadStackManager::AddDeadLock(DeadLock^ deadLock)
{
	r_DeadLocks->Add(deadLock);

	for each(DeadLockItem^ item in deadLock->r_items)
	{
		r_processedOwnedObjects->Add(item->r_objectOwned, item->r_objectOwned);
	}

	// TODO: Ask the DLItems to clean themselves up for presentation
}

}
#pragma unmanaged