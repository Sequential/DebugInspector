#include "StdAfx.h"
#include "KStructs.h"
#include "Kernel.h"
#include "Dac.h"
#include "DebugEventCallbacks.h"

#include "stdio.h"

#include "detours.h"


static int Offsets_WIN2K_5_0_2195_7045[KernelOffsetsArraySize];
static int Offsets_WINXP_5_1_2600_0000[KernelOffsetsArraySize];
static int Offsets_WINXP_5_1_2600_2180[KernelOffsetsArraySize];
static int Offsets_WIN03_5_2_3790_0000[KernelOffsetsArraySize];
static int Offsets_WINVT_6_0_6000_16386[KernelOffsetsArraySize];

 

static bool s_OffsetsInitialized = false;

#define SIGN_EXTEND(_x_) (ULONG64)(LONG)(_x_)

Kernel::Kernel(ULONG32 OSProcessId)
{
	_cannotAttachLocalKernel = false;
	_lastError = 0;
	_OSProcessId = OSProcessId;
	if(!s_OffsetsInitialized)
	{
		SetupOffsets();
		s_OffsetsInitialized = true;
	}
}

Kernel::~Kernel(void)
{
}

void Kernel::SetupOffsets()
{
	Offsets_WIN2K_5_0_2195_7045[OffSetId_KProcess_ThreadListHead] = 0x0050;
	Offsets_WINXP_5_1_2600_0000[OffSetId_KProcess_ThreadListHead] = 0x0050;
	Offsets_WINXP_5_1_2600_2180[OffSetId_KProcess_ThreadListHead] = 0x0050;
	Offsets_WIN03_5_2_3790_0000[OffSetId_KProcess_ThreadListHead] = 0x0050;
	Offsets_WINVT_6_0_6000_16386[OffSetId_KProcess_ThreadListHead] = 0x0050;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_KProcess_UniqueProcessId] = 0x009c;
	Offsets_WINXP_5_1_2600_0000[OffsetId_KProcess_UniqueProcessId] = 0x0084;
	Offsets_WINXP_5_1_2600_2180[OffsetId_KProcess_UniqueProcessId] = 0x0084;
	Offsets_WIN03_5_2_3790_0000[OffsetId_KProcess_UniqueProcessId] = 0x0084;
	Offsets_WINVT_6_0_6000_16386[OffsetId_KProcess_UniqueProcessId] = 0x009c;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_KProcess_ActiveProcessLinks] = 0x00a0;
	Offsets_WINXP_5_1_2600_0000[OffsetId_KProcess_ActiveProcessLinks] = 0x0088;
	Offsets_WINXP_5_1_2600_2180[OffsetId_KProcess_ActiveProcessLinks] = 0x0088;
	Offsets_WIN03_5_2_3790_0000[OffsetId_KProcess_ActiveProcessLinks] = 0x0088;
	Offsets_WINVT_6_0_6000_16386[OffsetId_KProcess_ActiveProcessLinks] = 0x00a0;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_KProcess_ObjectTable] = 0x0128;
	Offsets_WINXP_5_1_2600_0000[OffsetId_KProcess_ObjectTable] = 0x00c4;
	Offsets_WINXP_5_1_2600_2180[OffsetId_KProcess_ObjectTable] = 0x00c4;
	Offsets_WIN03_5_2_3790_0000[OffsetId_KProcess_ObjectTable] = 0x00c4;
	Offsets_WINVT_6_0_6000_16386[OffsetId_KProcess_ObjectTable] = 0x00dc;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_WaitBlockList]   = 0x0058;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_WaitBlockList]   = 0x005c;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_WaitBlockList]   = 0x005c;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_WaitBlockList]   = 0x0054;
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_WaitBlockList]   = 0x0064;
	

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_ThreadListEntry] = 0x01a4;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_ThreadListEntry] = 0x01b0;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_ThreadListEntry] = 0x01b0;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_ThreadListEntry] = 0x01ac;	
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_ThreadListEntry] = 0x01c4;	

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_Teb] = 0x0020;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_Teb] = 0x0020;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_Teb] = 0x0020;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_Teb] = 0x0030;	
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_Teb] = 0x0084;	

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_ClientId] = 0x01ec;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_ClientId] = 0x01ec;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_ClientId] = 0x01ec;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_ClientId] = 0x01ec;	
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_ClientId] = 0x020c;	

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_MutantListHead] = 0x0010;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_MutantListHead] = 0x0010;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_MutantListHead] = 0x0010;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_MutantListHead] = 0x0010;
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_MutantListHead] = 0x01cc;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_StartAddress] = 0x0230;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_StartAddress] = 0x0224;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_StartAddress] = 0x0224;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_StartAddress] = 0x022c;
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_StartAddress] = 0x01f8;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_Win32StartAddress] = 0x0234;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_Win32StartAddress] = 0x0228;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_Win32StartAddress] = 0x0228;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_Win32StartAddress] = 0x0230;
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_Win32StartAddress] = 0x0240;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_EThread_Win32Thread] = 0x0124;
	Offsets_WINXP_5_1_2600_0000[OffsetId_EThread_Win32Thread] = 0x0130;
	Offsets_WINXP_5_1_2600_2180[OffsetId_EThread_Win32Thread] = 0x0130;
	Offsets_WIN03_5_2_3790_0000[OffsetId_EThread_Win32Thread] = 0x014c;
	Offsets_WINVT_6_0_6000_16386[OffsetId_EThread_Win32Thread] = 0x0170;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_HANDLE_TABLE_TableCode] = 0x00;
	Offsets_WINXP_5_1_2600_0000[OffsetId_HANDLE_TABLE_TableCode] = 0x00;
	Offsets_WINXP_5_1_2600_2180[OffsetId_HANDLE_TABLE_TableCode] = 0x00;
	Offsets_WIN03_5_2_3790_0000[OffsetId_HANDLE_TABLE_TableCode] = 0x00;
	Offsets_WINVT_6_0_6000_16386[OffsetId_HANDLE_TABLE_TableCode] = 0x00;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_HANDLE_TABLE_QuotaProcess] = 0x0c;
	Offsets_WINXP_5_1_2600_0000[OffsetId_HANDLE_TABLE_QuotaProcess] = 0x04;
	Offsets_WINXP_5_1_2600_2180[OffsetId_HANDLE_TABLE_QuotaProcess] = 0x04;
	Offsets_WIN03_5_2_3790_0000[OffsetId_HANDLE_TABLE_QuotaProcess] = 0x04;
	Offsets_WINVT_6_0_6000_16386[OffsetId_HANDLE_TABLE_QuotaProcess] = 0x04;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_HANDLE_TABLE_UniqueProcessId] = 0x10;
	Offsets_WINXP_5_1_2600_0000[OffsetId_HANDLE_TABLE_UniqueProcessId] = 0x08;
	Offsets_WINXP_5_1_2600_2180[OffsetId_HANDLE_TABLE_UniqueProcessId] = 0x08;
	Offsets_WIN03_5_2_3790_0000[OffsetId_HANDLE_TABLE_UniqueProcessId] = 0x08;
	Offsets_WINVT_6_0_6000_16386[OffsetId_HANDLE_TABLE_UniqueProcessId] = 0x08;

	Offsets_WIN2K_5_0_2195_7045[OffsetId_HANDLE_TABLE_HandleTableList] = 0x54;
	Offsets_WINXP_5_1_2600_0000[OffsetId_HANDLE_TABLE_HandleTableList] = 0x1c;
	Offsets_WINXP_5_1_2600_2180[OffsetId_HANDLE_TABLE_HandleTableList] = 0x1c;
	Offsets_WIN03_5_2_3790_0000[OffsetId_HANDLE_TABLE_HandleTableList] = 0x1c;
	Offsets_WINVT_6_0_6000_16386[OffsetId_HANDLE_TABLE_HandleTableList] = 0x10;


}

DWORD (WINAPI *g_fnGetModuleFileNameW)(HMODULE hModule, LPWSTR lpFilename, DWORD nSize) = ::GetModuleFileNameW;
DWORD (WINAPI *g_fnSizeofResource)(HMODULE hModule, HRSRC hResInfo) = ::SizeofResource;
HGLOBAL (WINAPI *g_fnLoadResource)(HMODULE hModule, HRSRC hResInfo) = ::LoadResource;
HRSRC (WINAPI *g_fnFindResource)(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName) = ::FindResource;

HRSRC MyFindResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName)
{
	if((int)lpType == 0x7777 && (int)lpName == 0x4444)
	{
		HMODULE hExe = LoadLibrary(L"kd.exe"); 
		HRSRC hRes = g_fnFindResource(hExe, lpType, lpName);
		return hRes;
	}
	return g_fnFindResource(hModule, lpType, lpName);
}

DWORD WINAPI MyGetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
  DWORD dwSize = g_fnGetModuleFileNameW(hModule, lpFilename, nSize);
  if (!hModule)
  {
    wcscpy_s(wcsrchr(lpFilename, L'\\'), nSize, L"\\kd.exe");
    dwSize = wcslen(lpFilename);
  }
  return dwSize;
}

DWORD MySizeofResource(HMODULE hModule, HRSRC hResInfo) 
{
	DWORD dwSize = g_fnSizeofResource(hModule, hResInfo);
	if (!hModule)
	{
		HMODULE hExe = LoadLibrary(L"kd.exe"); 
		dwSize = g_fnSizeofResource(hExe, hResInfo);
	}
	return dwSize;
}

HGLOBAL MyLoadResource(HMODULE hModule, HRSRC hResInfo) 
{
	if (!hModule)
	{
		HMODULE hExe = LoadLibrary(L"kd.exe"); 
		HGLOBAL hGlobal = g_fnLoadResource(hExe, hResInfo);
		return hGlobal;
	}
	return g_fnLoadResource(hModule, hResInfo);
}

HRESULT ApplyVistaPatch(void)
{
	HMODULE hDetours = ::LoadLibrary(L"detoured.dll");

	if(!hDetours) return E_NOTIMPL;

  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(&(PVOID&)g_fnGetModuleFileNameW, MyGetModuleFileNameW);
  DetourAttach(&(PVOID&)g_fnSizeofResource,     MySizeofResource);
  DetourAttach(&(PVOID&)g_fnLoadResource,		MyLoadResource);
  DetourAttach(&(PVOID&)g_fnFindResource,		MyFindResource);
  DetourTransactionCommit();

  return S_OK;
}

HRESULT UnApplyVistaPatch(void)
{
	HMODULE hDetours = ::LoadLibrary(L"detoured.dll");

	if(!hDetours) return E_NOTIMPL;

  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID&)g_fnGetModuleFileNameW, MyGetModuleFileNameW);
  DetourDetach(&(PVOID&)g_fnSizeofResource,     MySizeofResource);
  DetourDetach(&(PVOID&)g_fnLoadResource,		MyLoadResource);
  DetourDetach(&(PVOID&)g_fnFindResource,		MyFindResource);
  DetourTransactionCommit();

  return S_OK;
}


HRESULT Kernel::Attach()
{
	// Get Our Version
	// If VISTA do extra work
	OSVERSIONINFOEXW ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bval = ::GetVersionEx((LPOSVERSIONINFOW)&ver);
	if(ver.dwMajorVersion>=6)
	{
		ApplyVistaPatch();	
	}

	HRESULT hr = g_ExtClient->AttachKernel(DEBUG_ATTACH_LOCAL_KERNEL, NULL);

	if(ver.dwMajorVersion>=6)
	{
		UnApplyVistaPatch();
	}


	if(FAILED(hr))
	{
		_lastError = ::GetLastError();
		_cannotAttachLocalKernel = true;
		return hr;
	}
	hr = g_ExtControl->WaitForEvent(0, INFINITE);
	
	ULONG Win32Major = 0;
	ULONG Win32Minor = 0;
	ULONG Win32ServicePack = 0;
	hr = GetCommonVersionInfo(Win32Major, Win32Minor, Win32ServicePack);

	if(Win32Major!=5 && Win32Major!=6)
	{
		return E_NOTIMPL;
	}

	if(Win32Major==6)
	{
		_pOffsets = Offsets_WINVT_6_0_6000_16386;
	}
	else
	{
		_pOffsets = NULL;
		switch(Win32Minor)
		{
			case 0:
			{
				_pOffsets = Offsets_WIN2K_5_0_2195_7045;
				break;
			}
			case 1:
			{
				_pOffsets = Win32ServicePack<512? Offsets_WINXP_5_1_2600_0000 : Offsets_WINXP_5_1_2600_2180;
				break;
			}
			case 2:
			{
				_pOffsets = Offsets_WIN03_5_2_3790_0000;
				break;
			}
			default:
				return E_NOTIMPL;
		}
	}

	hr = GetOffsetFromOSProcessId(_OSProcessId, _KProcess);

	hr = g_ExtSystem2->GetImplicitProcessDataOffset(&_KProcessKernel);
	hr = g_ExtSystem2->GetImplicitThreadDataOffset(&_KThreadKernel);

	printf("Process %08x", _KProcess);
	printf(" %04x\n", _OSProcessId);

	return hr;
}

HRESULT Kernel::GetCommonVersionInfo(ULONG& win32Major, ULONG& win32Minor, ULONG& servicePack)
{
	ULONG PlatformId = 0;
	ULONG Major = 0;
	ULONG Minor = 0;
	CHAR ServicePackString[255];
	ULONG cbServicePackString = 255;
	ULONG cbServicePackStringUsed=0;

	CHAR BuildString[255];
	ULONG cbBuildString = 255;
	ULONG cbBuildStringUsed=0;
	
	HRESULT hr = g_ExtControl->GetSystemVersion(&PlatformId, &Major, &Minor, 
												ServicePackString, cbServicePackString, &cbServicePackStringUsed,
												&servicePack, BuildString, cbBuildString, &cbBuildStringUsed);
	switch(Minor)
	{
		case 1381:{	win32Major = 4;	win32Minor = 0;	break;}
		case 2195:{	win32Major = 5;	win32Minor = 0;	break;}
		case 2600:{	win32Major = 5;	win32Minor = 1;	break;}
		case 3790:{	win32Major = 5;	win32Minor = 2;	break;}
		case 6000:{	win32Major = 6;	win32Minor = 0;	break;}
		case 6001:{	win32Major = 6;	win32Minor = 1;	break;}
	}
	return hr;
}

HRESULT Kernel::GetOffsetFromOSProcessId(ULONG32 OSProcessId, ULONG64& Offset)
{
	ULONG64 CurrentKProcess = 0;
	HRESULT hr = g_ExtSystem->GetCurrentProcessDataOffset(&CurrentKProcess);	

	ULONG32 ObjectTableAddress = 0;
	ULONG32 StartTableAddress  = 0;
	LIST_ENTRY TableList;
	hr = g_ExtData2->ReadVirtual(CurrentKProcess + _pOffsets[OffsetId_KProcess_ObjectTable], &ObjectTableAddress, sizeof(ObjectTableAddress), NULL);
	hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_HandleTableList]), &TableList, sizeof(TableList), NULL);	
	StartTableAddress = ObjectTableAddress;

	do
	{
		ULONG32 UniqueProcessId = 0;
		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_UniqueProcessId]), &UniqueProcessId, sizeof(UniqueProcessId), NULL);

		if(UniqueProcessId==OSProcessId)
		{
			ULONG32 QuotaProcess = 0;
			hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_QuotaProcess]), &QuotaProcess, sizeof(QuotaProcess), NULL);
			Offset = SIGN_EXTEND(QuotaProcess);
			return S_OK;
		}

		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_HandleTableList]), &TableList, sizeof(TableList), NULL);	
		ObjectTableAddress = (ULONG32)TableList.Flink - _pOffsets[OffsetId_HANDLE_TABLE_HandleTableList];

	}
	while(StartTableAddress != ObjectTableAddress);
	Offset = 0;
	return E_INVALIDARG;
}

HRESULT Kernel::GetThreadInfo(ULONG32 EThreadAddress)
{
	ULONG32 EThreadClientIdAddress = EThreadAddress + _pOffsets[OffsetId_EThread_ClientId];
	CLIENT_ID ClientId;
	HRESULT hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(EThreadClientIdAddress), &ClientId, sizeof(ClientId), NULL);

	printf("Thread %08x, Client Id %04x.%04x \n", EThreadAddress, ClientId.UniqueProcess, ClientId.UniqueThread);

	ULONG32 WinThread32Address;
	hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(EThreadAddress + _pOffsets[OffsetId_EThread_Win32Thread]), &WinThread32Address, sizeof(WinThread32Address), NULL);

	printf("    Win32Thread %x \n", WinThread32Address);

	if(WinThread32Address)
	{
		ULONG32 HwndListAddress	= 0;
		
		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(WinThread32Address + offset_Win32thread_HwndList), &HwndListAddress, sizeof(HwndListAddress), NULL);
		printf("    HwndListAddress %x \n", HwndListAddress);
		if(HwndListAddress)
		{ 
			ULONG32 WinThread32BackAddress = 0;
			HWND hWnd = 0;
			do
			{
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(HwndListAddress + offset_HWNDLIST_Handle), &hWnd, sizeof(hWnd), NULL);
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(HwndListAddress + offset_HWNDLIST_Win32Thread), &WinThread32BackAddress, sizeof(WinThread32BackAddress), NULL);

				printf("  List %x  Hwnd %x \n", HwndListAddress, hWnd);

				_pCurrentSnapshot->AddHandle(SIGN_EXTEND(HwndListAddress), (HANDLE)hWnd);
				_pCurrentSnapshot->AddOwnedObject(SIGN_EXTEND(HwndListAddress), SIGN_EXTEND(EThreadAddress));
				// TODO: Get the Window Name
				_pCurrentSnapshot->AddObject(SIGN_EXTEND(HwndListAddress), 99, 99, 0, L"HWND");

				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(HwndListAddress + offset_HWNDLIST_Next), &HwndListAddress, sizeof(HwndListAddress), NULL);

			}while(HwndListAddress && (WinThread32BackAddress==WinThread32Address));
		}
	}
	_pCurrentSnapshot->AddThread(SIGN_EXTEND(EThreadAddress), ClientId.UniqueThread);
	return hr;
}

HRESULT Kernel::GetDispatcherInfo(ULONG32 ObjectAddress)
{
	DISPATCHER_HEADER WaitObject;
	HRESULT hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectAddress), &WaitObject, sizeof(WaitObject), NULL);


	printf("        Obj %08x Type %d Size %d SignalState %d \n", 
				ObjectAddress,
				WaitObject.Type,
				WaitObject.Size,
				WaitObject.SignalState);

	ULONG32 handle = 0;
	hr = GetHandleFromObjectAddress(ObjectAddress, _KProcess, handle);

	printf("        Obj Handle 0x%x \n", handle);
	
	wchar_t name[1024];
	name[0] = '\0';
	hr = GetObjectName(SIGN_EXTEND(ObjectAddress),name, 1024);

	wprintf(L"        Obj Name   %s \n\n", name);

	_pCurrentSnapshot->AddObject(SIGN_EXTEND(ObjectAddress), 
								 WaitObject.Type,
								 WaitObject.Size,
								 WaitObject.SignalState, 
								 name);


	return hr;

}

HRESULT Kernel::ProcessHandles()
{
	ULONG32 ObjectTableAddress = 0;
	HRESULT hr = g_ExtData2->ReadVirtual(_KProcess + _pOffsets[OffsetId_KProcess_ObjectTable], &ObjectTableAddress, sizeof(ObjectTableAddress), NULL);

	bool winxpormore = true;
	if(winxpormore)
	{
		ULONG TableCode = 0;
		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_TableCode]), &TableCode, sizeof(TableCode), NULL);	
		ULONG TableLevel = (ULONG)(TableCode & 3);
		TableCode -= TableLevel;
		
		const int TableSize = 0x200;
		const int cbSizeOfHandleTableEntry = 0x8;
		const int offsetObject = 0x18;
		PCHAR Level1, Level2, Level3;
		switch(TableLevel)
		{
			case 0:
			{
				for(int i=0;i<TableSize;++i)
				{
					Level1 = (PCHAR)TableCode;
					ULONG32 Entry = (ULONG32)&Level1[i*cbSizeOfHandleTableEntry];
					ULONG32 CurrentObjectAddress = 0;
					hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
					if(CurrentObjectAddress)
					{
						CurrentObjectAddress &= ~3;
						CurrentObjectAddress += offsetObject;
						printf("%x    %x \n", (i << 0x2), CurrentObjectAddress);
						_pCurrentSnapshot->AddHandle(SIGN_EXTEND(CurrentObjectAddress), (HANDLE)(i << 0x2));
					}
				}
				break;
			}
			case 1:
			{
				ULONG32 Level1Table[TableSize];
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(TableCode) , &Level1Table, sizeof(Level1Table), NULL);
				for(int i=0;i<TableSize;++i)
				{
					ULONG32 Table2Address = Level1Table[i];
					if(Table2Address>0)
					{
						Level2 = (PCHAR)Table2Address;
						for(int j=0;j<TableSize;++j)
						{
							ULONG32 Entry = (ULONG32)&Level2[j*cbSizeOfHandleTableEntry];
							ULONG32 CurrentObjectAddress = 0;
							hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
							if(CurrentObjectAddress)
							{	
								_pCurrentSnapshot->AddHandle(SIGN_EXTEND(CurrentObjectAddress+offsetObject), (HANDLE)((i << 0x2) | (j << 0xA)));
							}
						}
					}

				}
				break;
			}
			case 2:
			{
				ULONG32 Level1Table[TableSize];
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(TableCode) , &Level1Table, sizeof(Level1Table), NULL);
				for(int i=0;i<TableSize;++i)
				{
					ULONG32 Table2Address = Level1Table[i];
					if(Table2Address>0)
					{
						ULONG32 Level2Table[TableSize];
						hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Table2Address) , &Level2Table, sizeof(Level2Table), NULL);
						for(int j=0;j<TableSize;++j)
						{
							ULONG32 Table3Address = Level2Table[i];
							if(Table3Address>0)
							{
								Level3 = (PCHAR)Table3Address;
								for(int k=0;k<TableSize;++k)
								{
									ULONG32 Entry = (ULONG32)&Level3[k*cbSizeOfHandleTableEntry];
									ULONG32 CurrentObjectAddress = 0;
									hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
									if(CurrentObjectAddress)
									{
										_pCurrentSnapshot->AddHandle(SIGN_EXTEND(CurrentObjectAddress+offsetObject), (HANDLE)((i << 0x2) | (j << 0xA) | (k << 0x12)));
									}
								}
							}
						}
					}
				}
				break;
			}
		}
	}
	return hr;
}

HRESULT Kernel::GetHandleFromObjectAddress(ULONG32 ObjectAddress, ULONG64 KProcess, ULONG32& Handle)
{
	ULONG32 ObjectTableAddress = 0;
	HRESULT hr = g_ExtData2->ReadVirtual(KProcess + _pOffsets[OffsetId_KProcess_ObjectTable], &ObjectTableAddress, sizeof(ObjectTableAddress), NULL);

	bool winxpormore = true;
	if(winxpormore)
	{
		ULONG TableCode = 0;
		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ObjectTableAddress + _pOffsets[OffsetId_HANDLE_TABLE_TableCode]), &TableCode, sizeof(TableCode), NULL);	
		ULONG TableLevel = (ULONG)(TableCode & 3);
		TableCode -= TableLevel;
		
		const int TableSize = 0x200;
		const int cbSizeOfHandleTableEntry = 0x8;
		PCHAR Level1, Level2, Level3;
		switch(TableLevel)
		{
			case 0:
			{
				for(int i=0;i<TableSize;++i)
				{
					Level1 = (PCHAR)TableCode;
					ULONG32 Entry = (ULONG32)&Level1[i*cbSizeOfHandleTableEntry];
					ULONG32 CurrentObjectAddress = 0;
					hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
					if(CurrentObjectAddress-1+0x18 == ObjectAddress)
					{
						Handle = i << 0x2;
						return S_OK;
					}
				}
				break;
			}
			case 1:
			{
				ULONG32 Level1Table[TableSize];
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(TableCode) , &Level1Table, sizeof(Level1Table), NULL);
				for(int i=0;i<TableSize;++i)
				{
					ULONG32 Table2Address = Level1Table[i];
					if(Table2Address>0)
					{
						Level2 = (PCHAR)Table2Address;
						for(int j=0;j<TableSize;++j)
						{
							ULONG32 Entry = (ULONG32)&Level2[j*cbSizeOfHandleTableEntry];
							ULONG32 CurrentObjectAddress = 0;
							hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
							if(CurrentObjectAddress-1+0x18 == ObjectAddress)
							{
								Handle = (i << 0x2) | (j << 0xA); 
								return S_OK;
							}
						}
					}
				}
				break;
			}
			case 2:
			{
				ULONG32 Level1Table[TableSize];
				hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(TableCode) , &Level1Table, sizeof(Level1Table), NULL);
				for(int i=0;i<TableSize;++i)
				{
					ULONG32 Table2Address = Level1Table[i];
					if(Table2Address>0)
					{
						ULONG32 Level2Table[TableSize];
						hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Table2Address) , &Level2Table, sizeof(Level2Table), NULL);
						for(int j=0;j<TableSize;++j)
						{
							ULONG32 Table3Address = Level2Table[i];
							if(Table3Address>0)
							{
								Level3 = (PCHAR)Table3Address;
								for(int k=0;k<TableSize;++k)
								{
									ULONG32 Entry = (ULONG32)&Level3[k*cbSizeOfHandleTableEntry];
									ULONG32 CurrentObjectAddress = 0;
									hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(Entry) , &CurrentObjectAddress, sizeof(CurrentObjectAddress), NULL);	
									if(CurrentObjectAddress-1+0x18 == ObjectAddress)
									{
										Handle = (i << 0x2) | (j << 0xA) | (k << 0x12) ;
										return S_OK;
									}
								}
							}
						}
					}
				}
				break;
			}
		}
	}
	return hr;
}

// TODO: Directory Names
HRESULT Kernel::GetObjectName(ULONG64 ObjectAddress, WCHAR* buffer, int size)
{
	BYTE NameOffset = 0;
	
	ULONG64 NameOffsetAddress = ObjectAddress + offset_OBJECT_Header + offset_OBJECT_HEADER_NameInfoOffset;
	HRESULT hr = g_ExtData2->ReadVirtual(NameOffsetAddress, &NameOffset, sizeof(NameOffset), NULL);

	if(NameOffset !=0)
	{
		ULONG64 ObjectNameAddress = ObjectAddress + offset_OBJECT_Header - NameOffset;
		ULONG32 DirectoryAddress = 0;
		hr = g_ExtData2->ReadVirtual(ObjectNameAddress, &DirectoryAddress, sizeof(DirectoryAddress), NULL);

		ULONG64 UnicodeStringAddress = ObjectNameAddress + 0x4;
		ULONG64 StringLengthAddress  = UnicodeStringAddress;
		ULONG64 MaxStringLengthAddress  = UnicodeStringAddress + 0x2;
		ULONG64 BufferPointerAddress  = UnicodeStringAddress + 0x4;

		short len = 0;
		short maxLen = 0;
		DWORD bufferPtr = 0;

		hr = g_ExtData2->ReadVirtual(StringLengthAddress, &len, sizeof(len), NULL);
		hr = g_ExtData2->ReadVirtual(MaxStringLengthAddress, &maxLen, sizeof(maxLen), NULL);
		hr = g_ExtData2->ReadVirtual(BufferPointerAddress, &bufferPtr, sizeof(bufferPtr), NULL);

		if(size<len)
		{
			len = size - 2;
		}
		if(len>0)
		{
			hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(bufferPtr), buffer, len  + 2, NULL);
		}
		else if(DirectoryAddress !=0)
		{
			//itow(DirectoryAddress, buffer, 10);
		}
	}
	return hr;
}

HRESULT Kernel::ProcessWaitList(ULONG32 EThreadAddress)
{
	ULONG32 KThreadWaitBlocListkAddress = EThreadAddress + _pOffsets[OffsetId_EThread_WaitBlockList];
	ULONG32 HeadWaitBlockListAddress = 0;
	HRESULT hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(KThreadWaitBlocListkAddress), &HeadWaitBlockListAddress, sizeof(HeadWaitBlockListAddress), NULL);
	ULONG32 WaitBlockAddress = HeadWaitBlockListAddress;
	KWAIT_BLOCK WaitBlock;
	hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(HeadWaitBlockListAddress), &WaitBlock, sizeof(WaitBlock), NULL);

	printf("\n    Wait Blocks\n\n");
	while(true)
	{
		printf("    WB %08x T %08x Obj %08x Key %d Type %d \n\n", 
			WaitBlockAddress,
			WaitBlock.KThread,
			WaitBlock.Object,
			WaitBlock.WaitKey,
			WaitBlock.WaitType);

		_pCurrentSnapshot->AddWaitBlock(SIGN_EXTEND(WaitBlockAddress),
										WaitBlock.KThread,
										(ULONG64)WaitBlock.Object,
										WaitBlock.WaitKey,
										WaitBlock.WaitType); 

		GetDispatcherInfo((ULONG32)WaitBlock.Object);
		

		WaitBlockAddress = (ULONG32)WaitBlock.NextWaitBlock;
		if(HeadWaitBlockListAddress == WaitBlockAddress)
		{
			break;
		}
		hr =  g_ExtData2->ReadVirtual(SIGN_EXTEND(WaitBlockAddress), &WaitBlock, sizeof(WaitBlock), NULL);
	}
	return hr;
}

HRESULT Kernel::ProcessThreadWaitList()
{
	LIST_ENTRY ThreadListHead;
	ULONG32 ListHeadAddress = (ULONG32)_KProcess + _pOffsets[OffSetId_KProcess_ThreadListHead];
	HRESULT hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(ListHeadAddress), &ThreadListHead, sizeof(ThreadListHead), NULL);

	ULONG32 NextEntryAddress = (ULONG32)ThreadListHead.Flink;
	LIST_ENTRY NextEntry;

	while(NextEntryAddress != ListHeadAddress)
	{
		printf("\n");
		ULONG32 EThreadAddress = NextEntryAddress - _pOffsets[OffsetId_EThread_ThreadListEntry];

		hr = GetDispatcherInfo(EThreadAddress);
		hr = GetThreadInfo(EThreadAddress);
		hr = ProcessWaitList(EThreadAddress);
		hr = ProcessMutantList(EThreadAddress);
		hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(NextEntryAddress), &NextEntry, sizeof(NextEntry), NULL);

		NextEntryAddress = (ULONG32)NextEntry.Flink;
	}
	return hr;
}

HRESULT Kernel::ProcessMutantList(ULONG32 EThreadAddress)
{
	printf("\n    Mutants Owned\n\n");

	ULONG32 MutanListHeadAddress = EThreadAddress + _pOffsets[OffsetId_EThread_MutantListHead];
	ULONG32 MutantAdrress = MutanListHeadAddress;
	LIST_ENTRY MutantEntry;
	HRESULT hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(MutantAdrress), &MutantEntry, sizeof(MutantEntry), NULL);
	MutantAdrress = (ULONG32)MutantEntry.Flink;

	while(MutanListHeadAddress != MutantAdrress)
	{
		ULONG32 ObjectAddress = MutantAdrress - _pOffsets[OffsetId_EThread_MutantListHead];

		_pCurrentSnapshot->AddOwnedObject(SIGN_EXTEND(ObjectAddress), SIGN_EXTEND(EThreadAddress));

		hr = GetDispatcherInfo(ObjectAddress);

		HRESULT Hr = g_ExtData2->ReadVirtual(SIGN_EXTEND(MutantEntry.Flink), &MutantEntry, sizeof(MutantEntry), NULL);
		MutantAdrress = (ULONG32)MutantEntry.Flink;
	}
	return hr;
}

HRESULT Kernel::GetWaitChainInfo(WaitSnapshot& snapshot)
{
	_pCurrentSnapshot = &snapshot;
	HRESULT hr = Attach();
	if(SUCCEEDED(hr))
	{
		snapshot.Initialize(_KProcess, _OSProcessId);
		hr = ProcessHandles();
		hr = ProcessThreadWaitList();
	}
	_pCurrentSnapshot = NULL;
	return hr;
}
