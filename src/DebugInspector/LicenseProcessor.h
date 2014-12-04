#pragma once
#include "Windows.h"

class LicenseProcessor
{
public:

	LicenseProcessor(void)
	{
	}


	~LicenseProcessor(void)
	{
	}

	char* GetLicenseText()
	{
		static char* pOk = "The DebugInspector Component is licensed";
		static char* pFail = "The DebugInspector Component has expired. Please download the latest version from http://www.DebugInspector.com";

		return IsLicenseValid() ? pOk : pFail;
	}

	bool IsLicenseValid()
	{
		SYSTEMTIME systemtime;
		::GetSystemTime(&systemtime);
		if(systemtime.wYear>2014)
			return false;
		if((systemtime.wYear>2013) && (systemtime.wMonth>6))
			return false;
		return true;
	}

	
};
