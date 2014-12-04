#include "StdAfx.h"
#include "ClrSyncBlockManager.h"
#include "Dac.h"
#include "DacpStructures.h"

#pragma unmanaged

ClrSyncBlockManager::ClrSyncBlockManager(void)
{
}

ClrSyncBlockManager::~ClrSyncBlockManager(void)
{
}

HRESULT ClrSyncBlockManager::SyncBlockCount(unsigned long& count)
{
	HRESULT hr = S_OK;
	count = 0;

	DacpSyncBlockData syncBlockData;
	hr = syncBlockData.Request(g_clrData,1);

    if (hr != S_OK)
    {
        g_ext.Out("Error requesting SyncBlk data\n");
        return hr;
    }
    count = syncBlockData.SyncBlockCount;
	return S_OK;
}

HRESULT ClrSyncBlockManager::RetrieveSyncBlocks(DWORD startIndex, ClrSyncBlock* buffer, int bufferize, int& readSize, BOOL& bMore)
{
	HRESULT hr = S_OK;
	DacpSyncBlockData syncBlockData;
	UINT syncBlockCount = 0;
	int nb = 0;

    for(; nb < bufferize; nb++)
    {
        if (g_ext.IsInterrupt())
			return E_ABORT;

        if (syncBlockData.Request(g_clrData,startIndex + nb) != S_OK)
        {
            g_ext.Out("SyncBlock %d is invalid, skipping\n", nb);
            continue;
        }

		syncBlockCount = syncBlockData.SyncBlockCount;

		if(startIndex + nb > syncBlockCount)
			break;
                   
        DWORD osThreadId = 0;
		ULONG clrThreadId  = 0;

        if (syncBlockData.HoldingThread != NULL)
        {
            DacpThreadData Thread;
            if ((hr = Thread.Request(g_clrData, syncBlockData.HoldingThread)) != S_OK)
            {
                g_ext.Out("Failed to request Thread at %p\n", syncBlockData.HoldingThread);
                return hr;
            }
			osThreadId = Thread.osThreadId;
			g_ExtSystem->GetThreadIdBySystemId (osThreadId, &clrThreadId);
        }

        if (syncBlockData.bFree) 
		{
			buffer[nb].ObjectName[0] = '\0';
        }
		else 
		{
            g_ext.NameForObject((DWORD_PTR)syncBlockData.Object, buffer[nb].ObjectName, 2048);
        }
		
		buffer[nb].AdditionalThreadCount = syncBlockData.AdditionalThreadCount;
		buffer[nb].AppDomainPtr			 = syncBlockData.appDomainPtr;
		buffer[nb].bFree				 = syncBlockData.bFree;
		buffer[nb].ClrThreadId			 = clrThreadId;
		buffer[nb].HoldingThread		 = syncBlockData.HoldingThread;
		buffer[nb].MonitorHeld			 = syncBlockData.MonitorHeld;
		buffer[nb].OsThreadId			 = osThreadId;
		buffer[nb].Recursion			 = syncBlockData.Recursion;
		buffer[nb].SyncBlockCount		 = syncBlockData.SyncBlockCount;
		buffer[nb].SyncBlockPointer		 = syncBlockData.SyncBlockPointer;
		buffer[nb].Reserved				 = syncBlockData.NewInRtmVersion;
		buffer[nb].Object				 = syncBlockData.Object;
	
		readSize = nb + 1;
	}
	bMore = (syncBlockCount > startIndex + nb);
	return hr;
}

