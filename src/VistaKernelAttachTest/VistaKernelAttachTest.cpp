// VistaKernelAttachTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DbgEng.h"
#include "DebugOutputCallbacks.h"
#include "detours.h"


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
    wcscpy(wcsrchr(lpFilename, L'\\'), L"\\kd.exe");
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

void installPatch(void)
{
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(&(PVOID&)g_fnGetModuleFileNameW, MyGetModuleFileNameW);
  DetourAttach(&(PVOID&)g_fnSizeofResource,     MySizeofResource);
  DetourAttach(&(PVOID&)g_fnLoadResource,		MyLoadResource);
  DetourAttach(&(PVOID&)g_fnFindResource,		MyFindResource);
  DetourTransactionCommit();
}

void uninstallPatch(void)
{
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID&)g_fnGetModuleFileNameW, MyGetModuleFileNameW);
  DetourDetach(&(PVOID&)g_fnSizeofResource,     MySizeofResource);
  DetourDetach(&(PVOID&)g_fnLoadResource,		MyLoadResource);
  DetourDetach(&(PVOID&)g_fnFindResource,		MyFindResource);
  DetourTransactionCommit();
}


int _tmain(int argc, _TCHAR* argv[])
{

	OSVERSIONINFOEXW ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	BOOL bval = ::GetVersionEx((LPOSVERSIONINFOW)&ver);

	LPTSTR lpMsgBuf;

	FormatMessage( 

    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,

    NULL,

    GetLastError(),

    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language

    (LPTSTR) &lpMsgBuf,

    0,

    NULL 

);



	DWORD err = ::GetLastError();

	::CoInitialize(NULL);

	IDebugClient *g_ExtClient;
	IDebugSymbols2 * g_ExtSymbols2;
	HRESULT hr = DebugCreate(__uuidof(IDebugClient), (void **)&g_ExtClient);

	//IDebugInputCallbacks* pDICB = new DebugInputCallbacks();
	IDebugOutputCallbacks* pDOCB = new DebugOutputCallbacks();
	//hr = g_ExtClient->SetInputCallbacks(pDICB);
	hr = g_ExtClient->SetOutputCallbacks(pDOCB);

	if(SUCCEEDED(hr))
	{
		hr = g_ExtClient->QueryInterface(__uuidof(IDebugSymbols2), (void **)&g_ExtSymbols2);
		hr = g_ExtSymbols2->SetSymbolPath("srv*c:\windows\symbols*http://msdl.microsoft.com/download/symbols");


		installPatch();

		hr = g_ExtClient->AttachKernel(DEBUG_ATTACH_LOCAL_KERNEL, NULL);

		uninstallPatch();

		if(SUCCEEDED(hr))
		{
			g_ExtClient->EndSession(0);
		}
	}   

	::CoUninitialize();
	return 0;
}

