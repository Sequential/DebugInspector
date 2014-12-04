#include "StdAfx.h"
#include "ThreadStackFrameArg.h"

using namespace System;
#pragma managed
namespace ClrInspector
{

ThreadStackFrameArg::ThreadStackFrameArg(Int32 index, String^ name, UInt64 address, UInt64 value)
{
	r_Index = index;
	r_Name = name;
	r_Address = address;
	r_Value = value;
}

}