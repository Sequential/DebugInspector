#pragma once
#include "DeadLockItem.h"
#pragma managed
namespace ClrInspector
{
	using namespace System::Collections::Generic;
public ref class DeadLock
{
internal:
	initonly List<DeadLockItem^>^ r_items;
public:
	DeadLock(void);
	property List<DeadLockItem^>^ Items {List<DeadLockItem^>^ get(){return r_items;}}
internal:
	void Add(DeadLockItem^ item);
};
}
#pragma unmanaged
