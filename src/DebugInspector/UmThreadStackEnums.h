#pragma once

#pragma managed
namespace UnmanagedInspector
{

public enum class ThreadStackWaitingOn
{
	NotWaiting = 0,
	WaitingForSingleObject = 1,
	WaitingForMultipleObjects = 2,
	WaitingForCriticalSection = 3,
	WaitingForSendMessage = 4
};

public enum class WaitType
{
	WaitAll = 0,
	WaitAny = 1
};


public enum class WaitObjectType
{
	NotificationEvent = 0,
	SynchronizationEvent  = 1,
	Mutant = 2,
	Process = 3,
	Queue = 4,
	Seamphore = 5,
	Thread = 6,
	NotUsed = 7,
	NotificationTimer = 8,
	SynchronizationTimer = 9
};

}
#pragma unmanaged