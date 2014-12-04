#pragma once
#pragma unmanaged

enum ClrStackFrameStatus
{
	STATUS_OK						= 0,
	STATUS_UNMANAGED				= 1,
	STATUS_UNINIT_STACK				= 2,
	STATUS_GET_FRAME_DATA_FAILED	= 3,
	STATUS_GET_FRAME_CTX_FAILED		= 4,
	STATUS_INTERRUPTED				= 5
};

struct ClrStackFrameLocal
{
	int     Index;
	BOOL	ClrRegister;
	WCHAR*  pName;
	ULONG64 Value;
	ULONG64 Address;

	ClrStackFrameLocal* pNext;

	ClrStackFrameLocal()
	{
		Index = 0;
		ClrRegister = 0;
		pName = NULL;
		pNext = NULL;
		Value = 0;
		Address = 0;
	}

	void SetName(WCHAR* buffer)
	{
		if(pName)
		{
			delete[] pName;
			pName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pName = new WCHAR[len+1];
		::wcscpy_s(pName, len+1, buffer);
	}

	~ClrStackFrameLocal()
	{
		if(pName)
		{
			delete[] pName;
			pName = NULL;
		}

		if(pNext)
		{
			delete pNext;
			pNext = NULL;
		}
	}
};


struct ClrStackFrameArg
{
	int     Index;
	WCHAR*  pName;
	ULONG64 Address;
	ULONG64 Value;

	ClrStackFrameArg* pNext;

	ClrStackFrameArg()
	{
		Index = 0;
		pName = NULL;
		Value = 0;
		pNext = NULL;
	}

	void SetName(WCHAR* buffer)
	{
		if(pName)
		{
			delete[] pName;
			pName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pName = new WCHAR[len+1];
		::wcscpy_s(pName, len+1, buffer);
	}

	~ClrStackFrameArg()
	{
		if(pName)
		{
			delete[] pName;
			pName = NULL;
		}

		if(pNext)
		{
			delete pNext;
			pNext = NULL;
		}
	}
};

struct ClrStackFrame
{
	DWORD	Esp;
	DWORD	Eip;
	ULONG64 Address;
	ULONG64 StackHeaderOffset;
	ULONG64 WaitingOn;
	BOOL    InMonitorEnter;
	BOOL    InThreadJoin;
	DWORD   JoiningThreadId;
	int     ArgCount;
	int     LocalsCount;
	BOOL    Managed;
	WCHAR*  pFrameName;
	WCHAR*  pMethodName;
	WCHAR*  pWatingOnName;
	WCHAR*  pModuleFileName;
	CHAR*   pFileName;
	ULONG   Line;

	ClrStackFrameStatus Status;

	ClrStackFrameArg* pFirstArg;
	ClrStackFrameLocal* pFirstLocal;

	ClrStackFrame* pNextFrame;

	ClrStackFrame()
	{
		Esp = 0;
		Eip = 0;
		Address = 0;
		ArgCount = 0;
		LocalsCount = 0;
		WaitingOn = 0;
		Managed = TRUE;
		InMonitorEnter = FALSE;
		InThreadJoin = FALSE;
		JoiningThreadId = 0;
		pFrameName = NULL;
		pMethodName = NULL;
		pNextFrame = NULL;
		pFirstArg = NULL;
		pFirstLocal = NULL;
		pWatingOnName = NULL;
		pModuleFileName = NULL;

		Line = 0;
		pFileName = NULL;

		StackHeaderOffset = 0;

		Status = STATUS_OK;
	}

	void SetFrameName(WCHAR* buffer)
	{
		if(pFrameName)
		{
			delete[] pFrameName;
			pFrameName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pFrameName = new WCHAR[len+1];
		::wcscpy_s(pFrameName, len+1, buffer);
	}

	void SetMethodName(WCHAR* buffer)
	{
		if(pMethodName)
		{
			delete[] pMethodName;
			pMethodName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pMethodName = new WCHAR[len+1];
		::wcscpy_s(pMethodName, len+1, buffer);
	}

	void SetWaitingOnName(WCHAR* buffer)
	{
		if(pWatingOnName)
		{
			delete[] pWatingOnName;
			pWatingOnName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pWatingOnName = new WCHAR[len+1];
		::wcscpy_s(pWatingOnName, len+1, buffer);
	}

	void SetModuleFileName(WCHAR* buffer)
	{
		if(pModuleFileName)
		{
			delete[] pModuleFileName;
			pModuleFileName = NULL;
		}
		int len = ::wcsnlen(buffer, 1024);
		pModuleFileName = new WCHAR[len+1];
		::wcscpy_s(pModuleFileName, len+1, buffer);
	}

	void SetFileName(CHAR* buffer)
	{
		if(pFileName)
		{
			delete[] pFileName;
			pFileName = NULL;
		}
		int len = ::strnlen(buffer, 1024);
		pFileName = new CHAR[len+1];
		::strcpy_s(pFileName, len+1, buffer);
	}

	~ClrStackFrame()
	{
		if(pFrameName)
		{
			delete[] pFrameName;
			pFrameName = NULL;
		}

		if(pMethodName)
		{
			delete[] pMethodName;
			pMethodName = NULL;
		}

		if(pWatingOnName)
		{
			delete[] pWatingOnName;
			pWatingOnName = NULL;
		}

		if(pModuleFileName)
		{
			delete[] pModuleFileName;
			pModuleFileName = NULL;
		}

		if(pFileName)
		{
			delete[] pFileName;
			pFileName = NULL;
		}

		if(pFirstArg)
		{
			delete pFirstArg;
			pFirstArg = NULL;
		}

		if(pFirstLocal)
		{
			delete pFirstLocal;
			pFirstLocal = NULL;
		}

		if(pNextFrame)
		{
			delete pNextFrame;
			pNextFrame = NULL;
		}
	}
};
