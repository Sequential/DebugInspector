#include "StdAfx.h"
#include "ObjectBuffer.h"
#include <stack>

#pragma unmanaged

typedef std::stack<ObjectEntry*> BUFFEROBJECT;

ObjectBuffer::ObjectBuffer(void)
{
	_size = 0;
	_offset = -1;
	_firstBuffer =  (ObjectEntry*)calloc(BUFFER_SIZE, sizeof(ObjectEntry));
	ZeroMemory(_firstBuffer, sizeof(_firstBuffer));
	_pCurrentBuffer = _firstBuffer;
}

ObjectBuffer::~ObjectBuffer(void)
{
	ObjectEntry* pHeapBuffer = _firstBuffer;
	BUFFEROBJECT bufStack;

	while(pHeapBuffer)
	{
		for(int i=0;i<=MAX_OFFSET;++i)
		{
			free(pHeapBuffer[i].Name);
		}
		bufStack.push(pHeapBuffer);
		pHeapBuffer = (ObjectEntry* )(pHeapBuffer+MAX_OFFSET)->Address;
	}

	while(bufStack.size())
	{
		pHeapBuffer=bufStack.top();
		bufStack.pop();
		free(pHeapBuffer);
	}
	
}

void ObjectBuffer::Add(ULONG64 address, UCHAR type, UCHAR size, LONG signalState, wchar_t* name)
{
	if(++_offset==MAX_OFFSET)
	{
		ObjectEntry* pNewBuffer = (ObjectEntry*)calloc(BUFFER_SIZE, sizeof(ObjectEntry));
		(_pCurrentBuffer+MAX_OFFSET)->Address= (ULONG64)(pNewBuffer);
		_pCurrentBuffer = pNewBuffer;
		_offset = 0;
	}

	_pCurrentBuffer[_offset].Address = address;
	_pCurrentBuffer[_offset].Type = type;
	_pCurrentBuffer[_offset].Size = size;
	_pCurrentBuffer[_offset].SignalState = signalState;
	_pCurrentBuffer[_offset].Name = name? _wcsdup(name) : NULL;

	++_size;
}



