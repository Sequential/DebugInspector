#pragma once
#include "ClrSyncBlock.h"


#pragma unmanaged
class ClrSyncBlockManager
{
public:
	ClrSyncBlockManager(void);
	~ClrSyncBlockManager(void);

	HRESULT SyncBlockCount(unsigned long& count);
	HRESULT RetrieveSyncBlocks(DWORD startIndex, ClrSyncBlock* buffer, int bufferize, int& readSize, BOOL& bMore);

};
