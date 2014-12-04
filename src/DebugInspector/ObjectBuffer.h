#pragma once
#pragma unmanaged

struct ObjectEntry
{
	ULONG64		Address;
	UCHAR		Type;
	UCHAR		Size;
	LONG		SignalState;
	wchar_t*	Name;
};


class ObjectBuffer
{
public:
		static const int BUFFER_SIZE = 64;
		static const int MAX_OFFSET = BUFFER_SIZE - 1;
		ObjectEntry* _firstBuffer;
		int _size;
private:
		int _offset;
		ObjectEntry* _pCurrentBuffer;

public:
	ObjectBuffer(void);
	void Add(ULONG64 address, UCHAR type, UCHAR size, LONG signalState, wchar_t* name);
public:
	~ObjectBuffer(void);
};
