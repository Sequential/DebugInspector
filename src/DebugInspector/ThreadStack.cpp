#include "StdAfx.h"
#include "ThreadStack.h"
#include "ThreadStackManager.h"

#pragma managed
namespace ClrInspector
{

ThreadStack::ThreadStack(UInt32 corThreadId, UInt32 osThreadId, UInt64 domainId, String^ domainName)
{
	r_ThreadStackFrames = gcnew List<ThreadStackFrame^>();
	r_SyncBlockObjectsOwned = gcnew List<UInt64>();
	_SyncBlockObjectWaitedOn = 0;

	r_CorThreadId = corThreadId;
	r_OsThreadId = osThreadId;
	r_DomainId = domainId;
	r_DomainName = domainName;
}

void ThreadStack::AddFrame(ThreadStackFrame^ frame)
{
	r_ThreadStackFrames->Add(frame);
	UInt64 wo = frame->WaitingOn;
	if(wo!=0)
	{
		if(!r_SyncBlockObjectsOwned->Contains(wo))
		{
			if(_SyncBlockObjectWaitedOn!=0)
				throw gcnew ArgumentException("Thread is already waiting on a SyncBlock");

			_SyncBlockObjectWaitedOn = wo;
			_SyncBlockObjectWaitedOnName = frame->WatingOnName;
		}
	}
}

void ThreadStack::AddOwnedSyncBlocks(SyncBlockManager^ sbm, ThreadStackManager^ tsm)
{
	tsm->AddOwnedObject(r_CorThreadId, "this", this);
	r_SyncBlockObjectsOwned->Add(r_CorThreadId);

	for each(SyncBlock^ sb in sbm->r_syncBlocks)
	{
		if(sb->r_osThreadId == r_OsThreadId)
		{
			r_SyncBlockObjectsOwned->Add(sb->r_object);
			tsm->AddOwnedObject(sb->r_object, sb->r_objectName, this);
		}
	}
}

}
#pragma unmanaged
