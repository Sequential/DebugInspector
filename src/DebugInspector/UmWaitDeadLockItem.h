#pragma once
#include "UmWaitObject.h"
#include "UmThreadStack.h"
#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Collections::Generic;

public ref class WaitDeadLockItem
{
internal:
	initonly ThreadStack^ r_waitingThread;
	initonly WaitObject^  r_object;
	initonly ThreadStack^ r_owningThread;
	initonly WaitBlock^   r_waitBlock;

internal:
	WaitDeadLockItem(ThreadStack^ threadStackA, WaitObject^ waitingForX, ThreadStack^ ownedByB, WaitBlock^  waitBlock);

public:
	property ThreadStack^  WaitingThread 
	{public: ThreadStack^ get(){return r_waitingThread;}}

	property ThreadStack^  OwningThread 
	{public: ThreadStack^ get(){return r_owningThread;}}

	property WaitObject^  CurrentObject
	{public: WaitObject^ get(){return r_object;}}

	property WaitBlock^  CurrentWaitBlock
	{public: WaitBlock^ get(){return r_waitBlock;}}

};
}
#pragma unmanaged
