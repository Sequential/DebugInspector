#include "StdAfx.h"
#include "UmWaitDeadLockItem.h"

#pragma managed
namespace UnmanagedInspector
{

WaitDeadLockItem::WaitDeadLockItem(ThreadStack^ waitingThread, WaitObject^ object, ThreadStack^ owningThread, WaitBlock^ waitBlock)
{
	r_waitingThread	= waitingThread;
	r_object		= object;
	r_owningThread	= owningThread;
	r_waitBlock     = waitBlock;
}
}
#pragma unmanaged