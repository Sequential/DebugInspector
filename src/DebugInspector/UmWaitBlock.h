#pragma once
#include "UmThreadStackEnums.h"
#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
public ref class WaitBlock
{
internal:
	initonly Int64		r_address;
	initonly Int64		r_objectAddress;
	initonly Int64		r_threadAddress;
	initonly Int16		r_waitKey;
	initonly WaitType	r_waitType;
public:
	WaitBlock(Int64 address, Int64 objectAddress, Int64 threadAddress, Int16 waitKey, WaitType waitType);


	property Int64 Address 			{public: Int64 get(){return r_address;}}
	property Int64 ObjectAddress 	{public: Int64 get(){return r_objectAddress;}}
	property Int64 ThreadAddress 	{public: Int64 get(){return r_threadAddress;}}
	property Int64 WaitKey			{public: Int64 get(){return r_waitKey;}}
	property WaitType ObjectWaitType{public: WaitType get(){return r_waitType;}}
};
}
#pragma unmanaged