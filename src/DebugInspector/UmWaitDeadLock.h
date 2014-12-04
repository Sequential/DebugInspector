#pragma once
#include "UmWaitDeadLockItem.h"
#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Collections::Generic;

public ref class WaitDeadLock
{
internal:
	initonly List<WaitDeadLockItem^>^ r_items;
	bool _excludedFromDeadLockAnalysis;
public:
	WaitDeadLock(void);

	property List<WaitDeadLockItem^>^ Items 
	{public: List<WaitDeadLockItem^>^ get(){return r_items;}}
internal:
	void Add(WaitDeadLockItem^ item);
};
}
#pragma unmanaged