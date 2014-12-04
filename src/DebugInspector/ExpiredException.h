#pragma once
using namespace System;
#pragma managed
namespace ClrInspector
{
public ref class ExpiredException :
public Exception
{
public:

	ExpiredException(String^ message):Exception(message)
	{
	}
};
}
#pragma unmanaged
