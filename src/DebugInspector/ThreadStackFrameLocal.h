#pragma once
#pragma managed
namespace ClrInspector
{
	using namespace System;
public ref class ThreadStackFrameLocal
{
internal:
	initonly Int32		r_Index;
	initonly bool		r_IsClrRegister;
	initonly String^	r_Name;
	initonly UInt64		r_Value;
	initonly UInt64		r_Address;
public:
	ThreadStackFrameLocal(Int32 index, bool isClrRegister, String^ name, UInt64 value, UInt64 address);
};
}
