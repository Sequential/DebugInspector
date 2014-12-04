#include "StdAfx.h"
#include "DebugStackFrameBuffer.h"
#include <stack>

#pragma unmanaged

typedef std::stack<DebugStackFrameEntry*> BUFFERSTACK;

DebugStackFrameBuffer::DebugStackFrameBuffer(void)
{
	_size = 0;
	_offset = -1;
	_pCurrentBuffer = _firstBuffer;
	ZeroMemory(_firstBuffer, sizeof(_firstBuffer));
}

DebugStackFrameBuffer::~DebugStackFrameBuffer(void)
{
	DebugStackFrameEntry* pHeapBuffer = (DebugStackFrameEntry* )(_firstBuffer+MAX_OFFSET)->Address;
	
	if(pHeapBuffer)
	{
		BUFFERSTACK bufStack;
		
		do
		{
			bufStack.push(pHeapBuffer);
			pHeapBuffer = (DebugStackFrameEntry* )(pHeapBuffer+MAX_OFFSET)->Address;

		}while(pHeapBuffer);
		
		while(bufStack.size())
		{
			pHeapBuffer=bufStack.top();
			bufStack.pop();
			free(pHeapBuffer);
		}
	}
}

void DebugStackFrameBuffer::Add(DEBUG_STACK_FRAME& debugFrame, char* functionName, char* fileName, ULONG lineNo)
{
	if(++_offset==MAX_OFFSET)
	{
		DebugStackFrameEntry* pNewBuffer = (DebugStackFrameEntry*)calloc(BUFFER_SIZE, sizeof(DebugStackFrameEntry));
		(_pCurrentBuffer+MAX_OFFSET)->Address= (ULONG64)(pNewBuffer);
		_pCurrentBuffer = pNewBuffer;
		_offset = 0;
	}
	_pCurrentBuffer[_offset].DebugFrame  = debugFrame;
	_pCurrentBuffer[_offset].FunctionName = functionName? _strdup(functionName) : NULL;
	_pCurrentBuffer[_offset].FileName = fileName? _strdup(fileName) : NULL;
	_pCurrentBuffer[_offset].LineNo = lineNo;

	++_size;
}
