#include "StdAfx.h"
#include "ThreadStackFrameLocal.h"

using namespace System;
#pragma managed
namespace ClrInspector
{

ThreadStackFrameLocal::ThreadStackFrameLocal(Int32 index, bool isClrRegister, String^ name, UInt64 value, UInt64 address)
{
	r_Index = index;
	r_IsClrRegister = isClrRegister;
	r_Name = name;
	r_Value = value;
	r_Address = address;
}
}
