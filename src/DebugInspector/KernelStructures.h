#pragma once
#pragma unmanaged

const int VER_WIN_2K_5_0_2195_7045 = 0;
const int VER_WIN_XP_5_1_2600_0000 = 1;
const int VER_WIN_XP_5_1_2600_2180 = 2;
const int VER_WIN_03_5_2_3790_0000 = 3;

const int Offset_KProcess_ThreadListHead = 0;
const int Offset_EThread_WaitBlockList   = 1;
const int Offset_EThread_ThreadListEntry = 2;

const int Offset_EThread_Teb = 3;
const int Offset_EThread_ClientId = 4;
const int Offset_KProcess_UniqueProcessId = 5;
const int Offset_KProcess_ActiveProcessLinks = 6;
const int Offset_EThread_MutantListHead = 7;

const int OffsetArraySize = 8;

int Offsets_WIN_2K_5_0_2195_7045[OffsetArraySize];
int Offsets_WIN_XP_5_1_2600_0000[OffsetArraySize];
int Offsets_WIN_XP_5_1_2600_2180[OffsetArraySize];
int Offsets_WIN_03_5_2_3790_0000[OffsetArraySize];

enum DispatcherObjectType
{
	NotificationEvent = 0,
	SynchronizationEvent = 1,
	Mutant = 2,
	Process = 3,
	Queue = 4,
	Semaphore = 5,
	Thread = 6,
	Undefined = 7,
	NotifiactionTimer = 8,
	SynchronizationTimer = 9
};

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
}CLIENT_ID, *PCLIENT_ID;

typedef struct _DISPATCHER_HEADER
{
	UCHAR Type;
	UCHAR Absolute;
	UCHAR Size;
	UCHAR Inserted;
	LONG  SignalState;
	LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER, *PDISPATCHER_HEADER;

typedef struct _KWAIT_BLOCK
{
	LIST_ENTRY	WaitListEntry;
	LONG		KThread;
	PVOID		Object;
	PVOID		NextWaitblock;
	SHORT		WaitKey;
	SHORT		WaitType;
} KWAIT_BLOCK, *PKWAIT_BLOCK;

typedef struct _KPROCESS_DATA
{
	int * pOffsetData;
	ULONG64 PKPROCESS;
} KPROCESS_DATA,*PKPROCESS_DATA;

struct WaitBlock
{
	ULONG64    Address;
	ULONG32    ThreadAddress;
	ULONG32    ObjectAddress;
	SHORT      WaitKey;
	SHORT      WaitType;
	WaitBlock* pNext;

	WaitBlock()
	{
		Address=0;
		ThreadAddress=0;
		ObjectAddress=0;
		WaitKey=0;
		WaitType=0;
		pNext=NULL;
	}
	~WaitBlock()
	{
		if(pNext)
		{
			delete pNext;
			pNext = NULL;
		}
	}
};

struct DispatcherObject
{
	ULONG64 Address;
	UCHAR   Type;
	UCHAR   Size;
	LONG    SignalState;
	HANDLE  Handle;
	WCHAR*  pName;

	DispatcherObject()
	{
		Address=0;
		Type=0;
		Size=0;
		SignalState=0;
		Handle=0;
		pName=NULL;
	}

	~DispatcherObject()
	{
		if(pName)
		{
			delete pName;
			pName=NULL;
		}
	}
};