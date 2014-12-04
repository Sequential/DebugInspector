#include "StdAfx.h"
#include "DeadLockItem.h"
#pragma managed
namespace ClrInspector
{

DeadLockItem::DeadLockItem(ThreadStack^ threadStack, UInt64 objectOwned, UInt64 objectWaitingOn)
{
	r_threadStack = threadStack;
	r_objectOwned = objectOwned;
	r_objectWaitingOn = objectWaitingOn;
}

}
#pragma unmanaged
