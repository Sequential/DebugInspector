#pragma once
#pragma managed
namespace ClrInspector
{
	using namespace System;
public ref class ThreadStackFrameArg
{
internal:
	initonly Int32    r_Index;
	initonly String^  r_Name;
	initonly UInt64   r_Address;
	initonly UInt64   r_Value;
public:
	ThreadStackFrameArg(Int32 index, String^ name, UInt64 address, UInt64 value);

	property Int32   Index		{Int32   get(){return r_Index;}}
	property String^ Name		{String^ get(){return r_Name;}}
	property UInt64  Address	{UInt64  get(){return r_Address;}}
	property UInt64  Value		{UInt64  get(){return r_Value;}}
};
}