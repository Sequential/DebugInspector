#include "StdAfx.h"
#include "HandleBuffer.h"
#include <stack>

#pragma unmanaged

typedef std::stack<HandleEntry*> BUFFERSTACK;

HandleBuffer::HandleBuffer(void)
{
	_size = 0;
	_offset = -1;
	_pCurrentBuffer = _firstBuffer;
	ZeroMemory(_firstBuffer, sizeof(_firstBuffer));
}

HandleBuffer::~HandleBuffer(void)
{
	HandleEntry* pHeapBuffer = (HandleEntry* )(_firstBuffer+MAX_OFFSET)->Address;
	
	if(pHeapBuffer)
	{
		BUFFERSTACK bufStack;
		
		do
		{
			bufStack.push(pHeapBuffer);
			pHeapBuffer = (HandleEntry* )(pHeapBuffer+MAX_OFFSET)->Address;

		}while(pHeapBuffer);
		
		while(bufStack.size())
		{
			pHeapBuffer=bufStack.top();
			bufStack.pop();
			free(pHeapBuffer);
		}
	}
}

void HandleBuffer::Add(HANDLE  handle, ULONG64 address)
{
	if(++_offset==MAX_OFFSET)
	{
		HandleEntry* pNewBuffer = (HandleEntry*)calloc(BUFFER_SIZE, sizeof(HandleEntry));
		(_pCurrentBuffer+MAX_OFFSET)->Address= (ULONG64)(pNewBuffer);
		_pCurrentBuffer = pNewBuffer;
		_offset = 0;
	}
	_pCurrentBuffer[_offset].Handle  = handle;
	_pCurrentBuffer[_offset].Address = address;

	++_size;
}
