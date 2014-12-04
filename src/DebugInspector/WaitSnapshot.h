#pragma once
#include "KStructs.h"
#include "HandleBuffer.h"
#include "ObjectBuffer.h"
#include "OwnedBuffer.h"
#include "WaitBlockBuffer.h"

class WaitSnapshot
{
public:
	ULONG64			_KProcess;
	ULONG32			_OSProcessId;
	HandleBuffer	_handleBuffer;
	ObjectBuffer	_objectBuffer;
	HandleBuffer	_threadBuffer;
	OwnedBuffer		_ownedBuffer;
	WaitBlockBuffer _waitBuffer;

public:
	WaitSnapshot(void);
	void Initialize(ULONG64 KProcess, ULONG32 OSProcessId);
	void AddObject(ULONG64 address, UCHAR type, UCHAR size, LONG signalState, wchar_t* szName);
	void AddHandle(ULONG64 address, HANDLE handle);
	void AddThread(ULONG64 address, HANDLE threadId);
	void AddOwnedObject(ULONG64 address, ULONG64 threadAddress);
	void AddWaitBlock(ULONG64 address, ULONG64 threadAddress, ULONG64 objectAddress, SHORT waitKey, SHORT waitType);

public:
	~WaitSnapshot(void);
};
