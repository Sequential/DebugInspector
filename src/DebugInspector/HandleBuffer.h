#pragma once
#pragma unmanaged

struct HandleEntry
{
	HANDLE  Handle;
	ULONG64 Address;
};

class HandleBuffer
{
public:
		static const int BUFFER_SIZE = 64;
		static const int MAX_OFFSET = BUFFER_SIZE - 1;
		HandleEntry _firstBuffer[BUFFER_SIZE];
		int _size;
private:
		int _offset;
		HandleEntry* _pCurrentBuffer;

public:
	HandleBuffer(void);
	void Add(HANDLE  handle, ULONG64 address);
public:
	~HandleBuffer(void);
};
