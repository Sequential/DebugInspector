#pragma once
#pragma unmanaged

struct DebugStackFrameEntry
{
	ULONG64				Address;
	DEBUG_STACK_FRAME	DebugFrame;
	char*				FunctionName;
	char*				FileName;
	ULONG				LineNo;
};

class DebugStackFrameBuffer
{
public:
		static const int BUFFER_SIZE = 64;
		static const int MAX_OFFSET = BUFFER_SIZE - 1;
		DebugStackFrameEntry _firstBuffer[BUFFER_SIZE];
		int _size;
private:
		int _offset;
		DebugStackFrameEntry* _pCurrentBuffer;

public:
	DebugStackFrameBuffer(void);
	void Add(DEBUG_STACK_FRAME& debugFrame, char* functionName, char* fileName, ULONG lineNo);
public:
	~DebugStackFrameBuffer(void);
};
