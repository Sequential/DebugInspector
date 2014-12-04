#include "StdAfx.h"
#include "SyncBlockManager.h"
#include "ClrSyncBlockManager.h"
#pragma managed

namespace ClrInspector
{

SyncBlockManager::SyncBlockManager(void)
{
	r_syncBlocks = gcnew List<SyncBlock^>();
}

void SyncBlockManager::RetrieveSyncBlocks()
{
	r_syncBlocks->Clear();
	ClrSyncBlockManager clrSyncMan;

	ClrSyncBlock buffer[10];
	int count = 0;
	int index = 0;
	BOOL bMore = false;

	do
	{
		HRESULT hr = clrSyncMan.RetrieveSyncBlocks(index, buffer, 10, count, bMore);
		if(FAILED(hr))
			throw gcnew COMException("Error retrieving SyncBlocks"+index, hr);

		index+=count;
		for(int i=1;i<=count;++i)
		{
			ClrSyncBlock sb = buffer[i];
			String^ objectName = gcnew String(sb.ObjectName);

			SyncBlock^ pSb = gcnew SyncBlock(sb.Object, sb.bFree!=0, sb.SyncBlockPointer, sb.Reserved, sb.MonitorHeld,
				sb.Recursion, sb.HoldingThread, sb.AdditionalThreadCount, sb.AppDomainPtr, sb.SyncBlockCount,
				sb.OsThreadId, sb.ClrThreadId, objectName);

			r_syncBlocks->Add(pSb);
		}
	}while(bMore);
}
}