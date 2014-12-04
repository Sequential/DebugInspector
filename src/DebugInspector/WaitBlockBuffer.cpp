#include "StdAfx.h"
#include "WaitBlockBuffer.h"
#include <stack>

#pragma unmanaged

typedef std::stack<WaitBlockEntry*> BUFFERSTACK;

WaitBlockBuffer::WaitBlockBuffer(void)
{
	_size = 0;
	_offset = -1;
	_pCurrentBuffer = _firstBuffer;
	ZeroMemory(_firstBuffer, sizeof(_firstBuffer));
}

WaitBlockBuffer::~WaitBlockBuffer(void)
{
	WaitBlockEntry* pHeapBuffer = (WaitBlockEntry* )(_firstBuffer+MAX_OFFSET)->Address;
	
	if(pHeapBuffer)
	{
		BUFFERSTACK bufStack;
		
		do
		{
			bufStack.push(pHeapBuffer);
			pHeapBuffer = (WaitBlockEntry* )(pHeapBuffer+MAX_OFFSET)->Address;

		}while(pHeapBuffer);
		
		while(bufStack.size())
		{
			pHeapBuffer=bufStack.top();
			bufStack.pop();
			free(pHeapBuffer);
		}
	}
}

void WaitBlockBuffer::Add(ULONG64 address, ULONG64 threadAddress, ULONG64 objectAddress, SHORT waitKey, SHORT waitType)
{
	if(++_offset==MAX_OFFSET)
	{
		WaitBlockEntry* pNewBuffer = (WaitBlockEntry*)calloc(BUFFER_SIZE, sizeof(WaitBlockEntry));
		(_pCurrentBuffer+MAX_OFFSET)->Address= (ULONG64)(pNewBuffer);
		_pCurrentBuffer = pNewBuffer;
		_offset = 0;
	}
	_pCurrentBuffer[_offset].Address = address;
	_pCurrentBuffer[_offset].ThreadAddress  = threadAddress;
	_pCurrentBuffer[_offset].ObjectAddress  = objectAddress;
	_pCurrentBuffer[_offset].WaitKey		= waitKey;
	_pCurrentBuffer[_offset].WaitType		= waitType;

	++_size;
}
