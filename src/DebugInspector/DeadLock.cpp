#include "StdAfx.h"
#include "DeadLock.h"

#pragma managed
namespace ClrInspector
{

DeadLock::DeadLock(void)
{
	r_items = gcnew List<DeadLockItem^>();
}

void DeadLock::Add(DeadLockItem^ item)
{
	r_items->Add(item);
}

}
#pragma unmanaged