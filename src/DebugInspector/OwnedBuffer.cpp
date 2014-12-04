#include "StdAfx.h"
#include "OwnedBuffer.h"
#include <stack>

#pragma unmanaged

typedef std::stack<OwnedEntry*> BUFFERSTACK;

OwnedBuffer::OwnedBuffer(void)
{
	_size = 0;
	_offset = -1;
	_pCurrentBuffer = _firstBuffer;
	ZeroMemory(_firstBuffer, sizeof(_firstBuffer));
}

OwnedBuffer::~OwnedBuffer(void)
{
	OwnedEntry* pHeapBuffer = (OwnedEntry* )(_firstBuffer+MAX_OFFSET)->Address;
	
	if(pHeapBuffer)
	{
		BUFFERSTACK bufStack;
		
		do
		{
			bufStack.push(pHeapBuffer);
			pHeapBuffer = (OwnedEntry* )(pHeapBuffer+MAX_OFFSET)->Address;

		}while(pHeapBuffer);
		
		while(bufStack.size())
		{
			pHeapBuffer=bufStack.top();
			bufStack.pop();
			free(pHeapBuffer);
		}
	}
}

void OwnedBuffer::Add(ULONG64  address, ULONG64 threadAddress)
{
	if(++_offset==MAX_OFFSET)
	{
		OwnedEntry* pNewBuffer = (OwnedEntry*)calloc(BUFFER_SIZE, sizeof(OwnedEntry));
		(_pCurrentBuffer+MAX_OFFSET)->Address= (ULONG64)(pNewBuffer);
		_pCurrentBuffer = pNewBuffer;
		_offset = 0;
	}
	_pCurrentBuffer[_offset].Address  = address;
	_pCurrentBuffer[_offset].ThreadAddress = threadAddress;

	++_size;
}
