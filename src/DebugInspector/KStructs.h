#pragma once
#include "stdafx.h"
#pragma unmanaged

const int VER_WIN2K_5_0_2195_7045 = 1;
const int VER_WINXP_5_1_2600_0000 = 2;
const int VER_WINXP_5_1_2600_2180 = 3;
const int VER_WIN03_5_2_3790_0000 = 4;

const int OffSetId_KProcess_ThreadListHead = 0;
const int OffsetId_EThread_WaitBlockList   = 1;
const int OffsetId_EThread_ThreadListEntry = 2;
const int OffsetId_EThread_Teb = 3;
const int OffsetId_EThread_ClientId = 4;
const int OffsetId_KProcess_UniqueProcessId = 5;
const int OffsetId_KProcess_ActiveProcessLinks = 6;
const int OffsetId_EThread_MutantListHead = 7;
const int OffsetId_KProcess_ObjectTable = 8;
const int OffsetId_EThread_StartAddress = 9; 
const int OffsetId_EThread_Win32StartAddress = 10;
const int OffsetId_EThread_Win32Thread = 11;
const int OffsetId_HANDLE_TABLE_TableCode       = 12;
const int OffsetId_HANDLE_TABLE_QuotaProcess    = 13;
const int OffsetId_HANDLE_TABLE_UniqueProcessId = 14;
const int OffsetId_HANDLE_TABLE_HandleTableList = 15;


const int KernelOffsetsArraySize = 16;

extern int Offsets_WIN2K_5_0_2195_7045[KernelOffsetsArraySize];
extern int Offsets_WINXP_5_1_2600_0000[KernelOffsetsArraySize];
extern int Offsets_WINXP_5_1_2600_2180[KernelOffsetsArraySize];
extern int Offsets_WIN03_5_2_3790_0000[KernelOffsetsArraySize];
extern int Offsets_WINVT_6_0_6000_16386[KernelOffsetsArraySize];

// TODO: 

// OBJECTS

	const int offset_OBJECT_Header				  = - 0x018;
	const int offset_OBJECT_HEADER_NameInfoOffset =   0x00c;

// HANDLES
	//const int offset_HANDLE_TABLE_TableCode       = 0x000;//Win2k:Doesn't exist, VISTA =0
	//const int offset_HANDLE_TABLE_QuotaProcess    = 0x004;//Win2k:ULONG+LONG+POINTER			== 12, VISTA==4
	//const int offset_HANDLE_TABLE_UniqueProcessId = 0x008;//Win2k:ULONG+LONG+POINTER+POINTER	== 16, VISTA=8
	//const int offset_HANDLE_TABLE_HandleTableList = 0x010;//Win2k:16+LONG+LONG+ERESOURCE(38?) == xx, XP==1c, VISTA=10

// STACK TRACE
	//const int offset_ETHREAD_StartAddress         = 0x224; //vist==1f8, 5.1.0==224, 5.1.1==224, 5.2==22c
	//const int offset_ETHREAD_Win32StartAddress    = 0x228; //vist==240, 5.1.0==228, 5.1.1==228, 5.2==230

	const int offset_CritSec_OwningThread  = 0x0c;
	const int offset_CritSec_LockSemaphore = 0x10;

// GUI Thread (Win32Thread)
	//const int offset_ETHREAD_Win32Thread  = 0x130; // vist==170, Win2k == 0x124; 5.1.0==130, 5.1.1==130, 5.2==14c

	const int offset_Win32thread_HwndList = 0x0c0; // Win2k == 0x0b8;
	const int offset_HWNDLIST_Handle      = 0x000; // Win2k == 0x000;
	const int offset_HWNDLIST_Next        = 0x02c; // Win2k == 0x02c;
	const int offset_HWNDLIST_Win32Thread = 0x008;


typedef struct _DISPATCHER_HEADER {
  UCHAR  Type;
  UCHAR  Absolute;
  UCHAR  Size;
  UCHAR  Inserted;
  LONG  SignalState;
  LIST_ENTRY  WaitListHead;
} DISPATCHER_HEADER, *PDISPATCHER_HEADER;

typedef struct _KWAIT_BLOCK
{
	LIST_ENTRY		WaitListEntry;
	LONG			KThread;
	PVOID			Object;
	PVOID           NextWaitBlock;
	SHORT			WaitKey;
	SHORT			WaitType;
} KWAIT_BLOCK, *PKWAIT_BLOCK;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

enum DispatcherObjectType
{
	NotificationEvent = 0,
	SynchronizationEvent  = 1,
	Mutant = 2,
	Process = 3,
	Queue = 4,
	Seamphore = 5,
	Thread = 6,
	NotUsed = 7,
	NotificationTimer = 8,
	SynchronizationTimer = 9
};

enum WaitType
{
	WaitAll = 0,
	WaitAny = 1
};

enum WaitingFor
{
	NotWaiting = 0,
	WaitingForSingleObject = 1,
	WaitingForMultipleObjects = 2,
	WaitingForCriticalSection = 3,
	WaitingForSendMessage = 4
};




