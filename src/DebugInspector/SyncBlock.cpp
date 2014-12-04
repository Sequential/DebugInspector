#include "StdAfx.h"
#include "SyncBlock.h"

#pragma managed
namespace ClrInspector
{

SyncBlock::SyncBlock(UInt64 object, bool free, UInt64 address, UInt64 reserved, 
				UInt32 monitorheld, UInt32 recursion, UInt64 holdingThread, 
				UInt32 additionalThreadCount, UInt64 appdomainAddress, UInt32 syncblockCount,
				UInt64 osThreadId, UInt64 clrThreadId, String^ objectName)
{
	r_object   = object;
	r_free     = free;
	r_address  = address;
	r_reserved = reserved;
	r_monitorheld = monitorheld;
	r_recursion = recursion;
	r_holdingThread = holdingThread;
	r_additionalThreadCount = additionalThreadCount;
	r_appdomainAddress = appdomainAddress;
	r_syncblockCount = syncblockCount;
	r_osThreadId = osThreadId;
	r_clrThreadId = clrThreadId;
	r_objectName = objectName;

}
}
#pragma unmanaged