#pragma once
#include "ThreadStack.h"
#pragma managed
namespace ClrInspector
{
public ref class DeadLockItem
{
internal:
	initonly ThreadStack^	r_threadStack;
	initonly UInt64			r_objectOwned;
	initonly UInt64			r_objectWaitingOn;
public:
	DeadLockItem(ThreadStack^ threadStack, UInt64 objectOwned, UInt64 objectWaitingOn);

	property ThreadStack^	Stack			{ThreadStack^  get(){return r_threadStack;}}
	property UInt64			ObjectOwned		{UInt64  get(){return r_objectOwned;}}
	property UInt64			ObjectWaitingOn	{UInt64  get(){return r_objectWaitingOn;}}
};
}
#pragma unmanaged