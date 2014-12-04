#pragma once
#include "SyncBlock.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

namespace ClrInspector
{
public ref class SyncBlockManager
{
internal:
	initonly List<SyncBlock^>^ r_syncBlocks;
public:
	SyncBlockManager(void);

	void RetrieveSyncBlocks();
};
}