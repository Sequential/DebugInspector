#pragma once


#pragma managed
namespace UnmanagedInspector
{
	using namespace System;
	using namespace System::Collections::Generic;
public ref class ThreadStackFrame
{
internal:
	initonly String^ r_functionName;
	initonly String^ r_fileName;
	initonly Int32   r_lineNumber;
	initonly Int64   r_instructionOffset;
	initonly Int64   r_frameOffset;
	initonly Int64   r_stackOffset;
	initonly Int64   r_returnOffset;
	initonly Int64   r_functTableEntry;
	initonly Int32   r_frameNumber;
	initonly bool    r_isVirtual;

	initonly array<Int64>^ r_params;//4
	initonly array<Int64>^ r_reserved;//6

public:
	ThreadStackFrame(String^ functionName, String^ fileName, 
					 Int32 lineNumber, Int64 instructionOffset, 
					 Int64 frameOffset, Int64 stackOffset, Int64 returnOffset,
					 Int64 functTableEntry, array<Int64>^ params, array<Int64>^ reserved, 
					 Int32 frameNumber, bool isVirtual);

	property String^ FunctionName 		{public: String^ get(){return r_functionName;}}
	property String^ FileName 			{public: String^ get(){return r_fileName;}}
	property Int32   Line		 		{public: Int32 get(){return r_lineNumber;}}

	property Int64 InstructionOffset 	{public: Int64 get(){return r_instructionOffset;}}
	property Int64 FrameOffset 			{public: Int64 get(){return r_frameOffset;}}
	property Int64 StackOffset			{public: Int64 get(){return r_stackOffset;}}
	property Int64 ReturnOffset			{public: Int64 get(){return r_returnOffset;}}
	property Int64 FunctTableEntry		{public: Int64 get(){return r_functTableEntry;}}
	property Int32 FrameNumber			{public: Int32 get(){return r_frameNumber;}}
	property bool  IsVirtual			{public: bool  get(){return r_isVirtual;}}

	property array<Int64>^ Params		{public: array<Int64>^ get(){return r_params;}}
	property array<Int64>^ Reserved		{public: array<Int64>^ get(){return r_reserved;}}

	
};
}
#pragma unmanaged
