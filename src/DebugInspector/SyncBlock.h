#pragma once
#pragma managed

using namespace System;

namespace ClrInspector
{
public ref class SyncBlock
{
internal:
	initonly UInt64 r_object;
	initonly bool   r_free;
	initonly UInt64 r_address; 
	initonly UInt64 r_reserved; 
	initonly UInt32 r_monitorheld; 
	initonly UInt32 r_recursion; 
	initonly UInt64 r_holdingThread; 
	initonly UInt32 r_additionalThreadCount; 
	initonly UInt64 r_appdomainAddress; 
	initonly UInt32 r_syncblockCount;

	initonly UInt64 r_osThreadId; 
	initonly UInt64 r_clrThreadId; 
	initonly String^ r_objectName; 

internal:
	SyncBlock(UInt64 object, bool free, UInt64 address, UInt64 reserved, 
				UInt32 monitorheld, UInt32 recursion, UInt64 holdingThread, 
				UInt32 additionalThreadCount, UInt64 appdomainAddress, UInt32 syncblockCount,
				UInt64 osThreadId, UInt64 clrThreadId, String^ objectName);
public:
};
}
#pragma unmanaged