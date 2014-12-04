#include "StdAfx.h"
#include "UmWaitBlock.h"

#pragma managed
namespace UnmanagedInspector
{

WaitBlock::WaitBlock(Int64 address, Int64 objectAddress, Int64 threadAddress, Int16 waitKey, WaitType waitType)
{
	r_address		= address;
	r_objectAddress = objectAddress;
	r_threadAddress = threadAddress;
	r_waitKey		= waitKey;
	r_waitType		= waitType;
}
}
#pragma unmanaged