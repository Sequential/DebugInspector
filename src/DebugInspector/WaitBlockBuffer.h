#pragma once
#pragma unmanaged

struct WaitBlockEntry
{
	ULONG64 Address;
	ULONG64 ThreadAddress;
	ULONG64 ObjectAddress;
	SHORT   WaitKey;
	SHORT   WaitType;
};

class WaitBlockBuffer
{
public:
		static const int BUFFER_SIZE = 64;
		static const int MAX_OFFSET = BUFFER_SIZE - 1;
		WaitBlockEntry _firstBuffer[BUFFER_SIZE];
		int _size;
private:
		int _offset;
		WaitBlockEntry* _pCurrentBuffer;

public:
	WaitBlockBuffer(void);
	void Add(ULONG64 address, ULONG64 threadAddress, ULONG64 objectAddress, SHORT waitKey, SHORT waitType);
public:
	~WaitBlockBuffer(void);
};
