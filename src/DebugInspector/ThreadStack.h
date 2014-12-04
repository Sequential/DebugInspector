#pragma once
#include "ThreadStackFrame.h"
#include "SyncBlock.h"
#include "SyncBlockManager.h"

#pragma managed
namespace ClrInspector
{
ref class ThreadStackManager;

public ref class ThreadStack
{
internal:
	initonly UInt32 r_CorThreadId;
	initonly UInt32 r_OsThreadId;
	initonly UInt64 r_DomainId;
	initonly List<ThreadStackFrame^>^ r_ThreadStackFrames;
	initonly List<UInt64>^ r_SyncBlockObjectsOwned;
	initonly String^ r_DomainName;
	UInt64   _SyncBlockObjectWaitedOn;
	String^  _SyncBlockObjectWaitedOnName;
internal:
	ThreadStack(UInt32 corThreadId, UInt32 osThreadId, UInt64 domainId, String^ domainName);
	void AddFrame(ThreadStackFrame^ frame);
	void AddOwnedSyncBlocks(SyncBlockManager^ sbm, ThreadStackManager^ tsm);
public:
	property UInt32  CorThreadId	{UInt32  get(){return r_CorThreadId;}}
	property UInt32  OsThreadId		{UInt32  get(){return r_OsThreadId;}}
	property UInt64  DomainId		{UInt64  get(){return r_DomainId;}}
	property String^ DomainName		{String^ get(){return r_DomainName;}}

	property List<ThreadStackFrame^>^  Frames					{List<ThreadStackFrame^>^  get(){return r_ThreadStackFrames;}}
	property List<UInt64>^				SyncBlockObjectsOwned	{List<UInt64>^  get(){return r_SyncBlockObjectsOwned;}}

	property UInt64  SyncBlockObjectWaitedOn	{UInt64  get(){return _SyncBlockObjectWaitedOn;}}
	property String^  SyncBlockObjectWaitedOnName	{String^  get(){return _SyncBlockObjectWaitedOnName;}}
};
}
#pragma unmanaged