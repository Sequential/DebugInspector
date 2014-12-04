#pragma once

#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
public ref class WaitObject
{
internal:
	initonly Int64		r_address;
	initonly Int16		r_type;
	initonly Int16		r_size;
	initonly Int64		r_signalState;
	initonly String^    r_name;
	Int64				_userHandle;
public:
	WaitObject(Int64 address, Int16 type, Int16 size, Int64 signalState, String^ name);

	property Int64 Address 		{public: Int64 get(){return r_address;}}
	property Int64 ObjectType 	{public: Int64 get(){return r_type;}}
	property Int64 Size 		{public: Int64 get(){return r_size;}}
	property Int64 SignalState	{public: Int64 get(){return r_signalState;}}
	property Int64 Handle   	{public: Int64 get(){return _userHandle;}}

	property String^ Name	{public: String^ get(){return r_name;}}

//	property WaitObjectType^ ObjectType 
//	{public: WaitObjectType^ get(){return (WaitObjectType^)r_type;}}


};
}
#pragma unmanaged
