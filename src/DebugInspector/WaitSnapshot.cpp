#include "StdAfx.h"
#include "WaitSnapshot.h"

WaitSnapshot::WaitSnapshot()
{	
}

WaitSnapshot::~WaitSnapshot(void)
{
}

void WaitSnapshot::Initialize(ULONG64 KProcess, ULONG32 OSProcessId)
{
	_KProcess = KProcess;
	_OSProcessId = OSProcessId;
}

void WaitSnapshot::AddObject(ULONG64 address, UCHAR type, UCHAR size, LONG signalState, wchar_t* szName)
{
	_objectBuffer.Add(address, type, size, signalState, szName);
}

void WaitSnapshot::AddHandle(ULONG64 address, HANDLE handle)
{
	_handleBuffer.Add(handle, address);
}

void WaitSnapshot::AddThread(ULONG64 address, HANDLE threadId)
{
	_threadBuffer.Add(threadId, address);
}

void WaitSnapshot::AddOwnedObject(ULONG64 address, ULONG64 threadAddress)
{
	_ownedBuffer.Add(address, threadAddress);
}

void WaitSnapshot::AddWaitBlock(ULONG64 address, ULONG64 threadAddress, ULONG64 objectAddress, SHORT waitKey, SHORT waitType)
{
	_waitBuffer.Add(address, threadAddress, objectAddress, waitKey, waitType);
}

