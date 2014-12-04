// CritsFromClr.h

#pragma once

#include "windows.h"
#include "Process.h"
#include "stdio.h"

using namespace System;

namespace CritsFromClr {


	class Crits
	{
		public:
		static unsigned ThreadFunc1(void* pArgs)
		{
			CRITICAL_SECTION * pCritSec = (CRITICAL_SECTION*)pArgs;
			EnterCriticalSection(&(pCritSec[0]));
			Sleep(1000);
			EnterCriticalSection(&(pCritSec[1]));
			Sleep(600000000);
			LeaveCriticalSection(&(pCritSec[1]));
			Sleep(1000);
			LeaveCriticalSection(&(pCritSec[0]));
			return 0;
		}

		static unsigned ThreadFunc2(void* pArgs)
		{
			CRITICAL_SECTION * pCritSec = (CRITICAL_SECTION*)pArgs;
			EnterCriticalSection(&(pCritSec[1]));
			Sleep(1000);
			EnterCriticalSection(&(pCritSec[0]));
			Sleep(600000000);
			LeaveCriticalSection(&(pCritSec[0]));
			Sleep(1000);
			LeaveCriticalSection(&(pCritSec[1]));
			return 0;
		}

		void RunCritSec(ULONG32& cs1, ULONG32& cs2)
		{

			CRITICAL_SECTION* pCritSec = new CRITICAL_SECTION[2];
			
			cs1 = (ULONG32)&(pCritSec[0]);
			cs2 = (ULONG32)&(pCritSec[1]);
			printf("CRIT_SEC ADDR == 0x%x \n", cs1);
			printf("CRIT_SEC ADDR == 0x%x \n", cs2);

			InitializeCriticalSection(&(pCritSec[0]));
			InitializeCriticalSection(&(pCritSec[1]));

			unsigned threadid1 = 0;
			unsigned threadid2 = 0;
			unsigned threadid3 = 0;
			unsigned threadid4 = 0;
			unsigned threadid5 = 0;
			

			uintptr_t  hThread1 = _beginthreadex(NULL, 0, ThreadFunc1, (void*)(pCritSec), 0, &threadid1);
			uintptr_t  hThread2 = _beginthreadex(NULL, 0, ThreadFunc2, (void*)(pCritSec), 0, &threadid2);
			uintptr_t  hThread3 = _beginthreadex(NULL, 0, ThreadFunc1, (void*)(pCritSec), 0, &threadid3);
			uintptr_t  hThread4 = _beginthreadex(NULL, 0, ThreadFunc2, (void*)(pCritSec), 0, &threadid4);
			uintptr_t  hThread5 = _beginthreadex(NULL, 0, ThreadFunc1, (void*)(pCritSec), 0, &threadid5);
		}
	};

	public ref class Class1
	{
		// TODO: Add your methods for this class here.

	public:
		void CreatCrits(Int32% cs1, Int32% cs2)
		{
			ULONG32 ucs1=0;
			ULONG32 ucs2=0;
			Crits crits;
			crits.RunCritSec(ucs1, ucs2);

			cs1 = ucs1;
			cs2 = ucs2;
		}
	};
}
