#include "StdAfx.h"
#include "UmWaitDeadLock.h"

#pragma managed
namespace UnmanagedInspector
{

WaitDeadLock::WaitDeadLock(void)
{
	r_items = gcnew List<WaitDeadLockItem^>();
	_excludedFromDeadLockAnalysis = false;
}
void WaitDeadLock::Add(WaitDeadLockItem^ item)
{
	r_items->Add(item);
}
}
#pragma unmanaged