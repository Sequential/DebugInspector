#pragma once
#pragma unmanaged

struct OwnedEntry
{
	ULONG64 Address;
	ULONG64 ThreadAddress;
};

class OwnedBuffer
{
public:
		static const int BUFFER_SIZE = 64;
		static const int MAX_OFFSET = BUFFER_SIZE - 1;
		OwnedEntry _firstBuffer[BUFFER_SIZE];
		int _size;
private:
		int _offset;
		OwnedEntry* _pCurrentBuffer;

public:
	OwnedBuffer(void);
	void Add(ULONG64  handle, ULONG64 address);
public:
	~OwnedBuffer(void);
};
