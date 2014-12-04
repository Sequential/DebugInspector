#include "StdAfx.h"
#include "DebugStackTrace.h"
#include "Dac.h"

#include "stdio.h"

DebugStackTrace::DebugStackTrace(void)
{
}

DebugStackTrace::~DebugStackTrace(void)
{
}

HRESULT DebugStackTrace::Initialize(ULONG id, ULONG sysId)
{
	DEBUG_STACK_FRAME frames[4096];
	ULONG numframes = 0;
	HRESULT hr = S_OK; 

	_debugThreadId = id;
	_systemThreadId = sysId;
	_waitType = NotWaiting;
	_critSecAddress = 0;
	_timeout = INFINITE;
	_bWaitAll = FALSE;

	hr = g_ExtSystem2->SetCurrentThreadId(id);

	hr = g_ExtControl->GetStackTrace(0, 0, 0, frames, 4096, &numframes);

	for(ULONG frame=0;frame<numframes;++frame)
	{
		char funcName[512];
		unsigned __int64 offset = 0;
		ULONG lineno = 0;
		ZeroMemory(funcName, 512);

		hr = g_ExtSymbols->GetNameByOffset(frames[frame].InstructionOffset, funcName, 511, NULL, &offset);
		if(SUCCEEDED(hr))
		{
			const char * pName = funcName;

			if(_waitType==NotWaiting)
			{
				if(strstr(pName, "WaitForMultipleObjects"))
				{
					_waitType = WaitingForMultipleObjects;
					_bWaitAll = (BOOL)(frames[frame].Params[2]==0)?FALSE:TRUE;
					_timeout  = (DWORD) frames[frame].Params[3];

					ULONG64 count  = frames[frame].Params[0];

					if(count>0)
					{
						HANDLE*  pHandles = (HANDLE*) calloc((ULONG)count, sizeof(HANDLE));
						ZeroMemory(pHandles, sizeof(pHandles));
						
						ULONG64 handleAddress = frames[frame].Params[1];
						hr = g_ExtData2->ReadVirtual(handleAddress, pHandles, sizeof(pHandles), NULL);
						for(int i=0; i<count; ++i)
						{
							_handleBuffer.Add(pHandles[i], 0);
						}
						free(pHandles);
					}
				}
				else if(strstr(pName, "WaitForSingleObject"))
				{
					_waitType = WaitingForSingleObject;
					_handleBuffer.Add((HANDLE)frames[frame].Params[0], 0);
					_timeout  = (DWORD) frames[frame].Params[1];
					_bWaitAll = TRUE;
				}
				
			}

			if(strstr(pName, "SendMessage"))
			{
				_waitType = WaitingForSendMessage;
				_handleBuffer.Add((HANDLE)frames[frame].Params[0], 0);
				_timeout = INFINITE;
				_bWaitAll = TRUE;
			}
			
			// Will have called WaitforSingleObject above
			if (strstr(pName, "RtlpWaitForCriticalSection") ) // XP
			{
				_waitType = WaitingForCriticalSection;
				_bWaitAll = TRUE;
				_critSecAddress  = frames[frame].Params[0];

				HANDLE OwningThreadOsId = 0;
				HANDLE LockSemaphore = 0;
				hr = g_ExtData2->ReadVirtual(_critSecAddress + offset_CritSec_OwningThread, &OwningThreadOsId, sizeof(OwningThreadOsId), NULL);
				hr = g_ExtData2->ReadVirtual(_critSecAddress + offset_CritSec_LockSemaphore, &LockSemaphore, sizeof(LockSemaphore), NULL);

				_handleBuffer.Add(LockSemaphore, 0);
				_owningThreadBuffer.Add(LockSemaphore, (ULONG64)OwningThreadOsId);
			}

			if (strstr(pName, "RtlpWaitOnCriticalSection")) // VISTA
			{
				_waitType = WaitingForCriticalSection;
				_bWaitAll = TRUE;
				_critSecAddress  = frames[frame-1].Params[3];

				HANDLE OwningThreadOsId = 0;
				HANDLE LockSemaphore = 0;
				hr = g_ExtData2->ReadVirtual(_critSecAddress + offset_CritSec_OwningThread, &OwningThreadOsId, sizeof(OwningThreadOsId), NULL);
				hr = g_ExtData2->ReadVirtual(_critSecAddress + offset_CritSec_LockSemaphore, &LockSemaphore, sizeof(LockSemaphore), NULL);

				_handleBuffer.Add(LockSemaphore, 0);
				_owningThreadBuffer.Add(LockSemaphore, (ULONG64)OwningThreadOsId);
			}
		}
		else
		{
			sprintf_s(funcName, 511, "0x%I64X", frames[frame].InstructionOffset);
		}

		// File and Line
		char fileName[512];
		ZeroMemory(fileName, 512);
		hr = g_ExtSymbols->GetLineByOffset(frames[frame].InstructionOffset, &lineno, fileName, 511, NULL, NULL);

		_frameBuffer.Add(frames[frame], funcName, fileName, lineno); 
	}

	return hr;
}