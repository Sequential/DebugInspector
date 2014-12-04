#include "StdAfx.h"
#include "UmThreadStackFrame.h"

#pragma managed
namespace UnmanagedInspector
{
ThreadStackFrame::ThreadStackFrame(String^ functionName, String^ fileName, 
								 Int32 lineNumber, Int64 instructionOffset, 
								 Int64 frameOffset, Int64 stackOffset, Int64 returnOffset, 
								 Int64 functTableEntry, array<Int64>^ params, array<Int64>^ reserved, 
								 Int32 frameNumber, bool isVirtual)
{
	if(params==nullptr)
		throw gcnew ArgumentNullException("params");

	if(reserved==nullptr)
		throw gcnew ArgumentNullException("reserved");

	if(params->Length<4)
		throw gcnew ArgumentException("params must be at least 4");

	if(reserved->Length<6)
		throw gcnew ArgumentException("reserved must be at least 6");

	 r_functionName		= functionName;
	 r_fileName			= fileName;
	 r_lineNumber		= lineNumber;
	 r_instructionOffset= instructionOffset;
	 r_frameOffset		= frameOffset;
	 r_stackOffset		= stackOffset;
	 r_returnOffset		= returnOffset;
	 r_functTableEntry	= functTableEntry;
	 r_frameNumber		= frameNumber;
	 r_isVirtual		= isVirtual;
		
	 r_params   = gcnew array<Int64>(params->Length);
	 r_reserved = gcnew array<Int64>(reserved->Length);

	 for(int i=0;i<params->Length;++i)
	 {
		r_params[i] = params[i];
	 }

	 for(int i=0;i<reserved->Length;++i)
	 {
		r_reserved[i] = reserved[i];
	 }
}
}
#pragma unmanaged