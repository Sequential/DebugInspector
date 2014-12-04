#include "StdAfx.h"
#include "UmWaitObject.h"

#pragma managed
namespace UnmanagedInspector
{
WaitObject::WaitObject(Int64 address, Int16 type, Int16 size, Int64 signalState, String^ name)
{
	r_address = address;
	r_type    = type;
	r_size    = size;
	r_signalState = signalState;
	r_name = name;
	_userHandle = 0;
}
}
#pragma unmanaged
