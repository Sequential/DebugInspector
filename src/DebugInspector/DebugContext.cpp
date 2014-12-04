#pragma unmanaged

#include "StdAfx.h"
#include "DebugContext.h"

extern IXCLRDataProcess* g_clrData;
DWORD WINAPI MyThreadFunction( LPVOID lpParam );

DebugContext::DebugContext(void)
{
}

DebugContext::~DebugContext(void)
{
}

void DebugContext::Initialize()
{
	// Secondary check for Licence Expiry here !!!!
	HANDLE hThread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunction,       // thread function name
            NULL,					// argument to thread function 
            0,                      // use default creation flags 
            NULL);					// returns the thread identifier 

	CloseHandle(hThread);
}

DWORD WINAPI MyThreadFunction( LPVOID lpParam ) 
{ 	
	SYSTEMTIME st;
	::GetSystemTime(&st);

	bool f1 = ((float)(st.wYear)  * 33) > (2014 * 33);
	bool f2 = ((float)(st.wYear)  * 33) > (2013 * 33);
	bool f3 = f2 && ( ( (float)(st.wMonth) * 36) > (06 * 36) ); 

	bool flags = f1 || f3;

	/*bool f1 = (float)(st.wYear-1)/(float)2+(float)20 > (float)1024;
	bool f2 = (float)(st.wYear-0)/(float)2+(float)20 > (float)1024;
	bool f3 = f2 && st.wMonth > 6;
	bool flags = true;*/
	
	if(flags)
	{
		Sleep(60000 + rand());
		delete g_clrData;			
	}
	return 0;
}