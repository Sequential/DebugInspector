#pragma unmanaged
#include "StdAfx.h"
#include "LiveCLRDataTarget.h"
#include "DebugExtensions.h"

using namespace System;
using namespace System::IO;

namespace Diag = System::Diagnostics;

#undef GetEnvironmentVariable

#pragma managed
LiveCLRDataTarget::LiveCLRDataTarget(Diag::Process^ process)
{
	m_process = process;
}

LiveCLRDataTarget::~LiveCLRDataTarget(void)
{
	DebugExtensions::UnInitialize();
}


STDMETHODIMP
LiveCLRDataTarget::QueryInterface(
    IN REFIID InterfaceId,
    OUT PVOID* Interface
    )
{
    if (InterfaceId == IID_IUnknown ||
        InterfaceId == IID_ICLRDataTarget)
    {
        *Interface = (ICLRDataTarget*)this;
        // No need to refcount as this class is contained.
        return S_OK;
    }
    else
    {
        *Interface = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
LiveCLRDataTarget::AddRef(
    THIS
    )
{
    // No need to refcount as this class is contained.
    return 1;
}

STDMETHODIMP_(ULONG)
LiveCLRDataTarget::Release(
    THIS
    )
{
    // No need to refcount as this class is contained.
    return 0;
}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetMachineType( 
            /* [out] */ ULONG32 *machineType)
{
	String^ arch = Environment::GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");

	HRESULT hr = S_OK;

	if(arch=="x86")
	{
		*machineType = IMAGE_FILE_MACHINE_I386;
	}
	else if(arch == "AMD64")
	{
		*machineType = IMAGE_FILE_MACHINE_AMD64;
	}
	else if(arch = "IA64")
	{
		*machineType = IMAGE_FILE_MACHINE_IA64;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}
    
HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetPointerSize( 
    /* [out] */ ULONG32 *pointerSize)
{
	*pointerSize = sizeof(void*);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetImageBase( 
    /* [string][in] */ LPCWSTR imagePath,
    /* [out] */ CLRDATA_ADDRESS *baseAddress)
{
	*baseAddress = 0;
	
	String ^clrImagePath = gcnew String(imagePath);

	Diag::Process^ process = m_process;

	for each (Diag::ProcessModule^ m in process->Modules)
    {
        FileInfo^ file = gcnew FileInfo(m->FileName);

        if ((file->FullName == clrImagePath) || (file->Name == clrImagePath))
        {
			*baseAddress = (CLRDATA_ADDRESS)m->BaseAddress.ToInt64();
            return S_OK;
        }
    }

	return E_INVALIDARG;
}


HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::ReadVirtual( 
    /* [in] */ CLRDATA_ADDRESS address,
    /* [length_is][size_is][out] */ BYTE *buffer,
    /* [in] */ ULONG32 request,
    /* [out] */ ULONG32 *done)
{

	// ReadProcessMemory will fail if any part of the
	// region to read does not have read access. This
	// routine attempts to read the largest valid prefix
	// so it has to break up reads on page boundries.

	HRESULT status = S_OK;
	ULONG totalDone = 0;
	SIZE_T read;
	ULONG readSize;


	Diag::Process^ process = m_process;

	HANDLE hProcess = process->Handle.ToPointer();

	while(request > 0)
	{
		// Calculate the bytes to read and don't let read crosss
		// a page boundary.
		readSize = DebugExtensions::OSPageSize() - (ULONG)(address & (DebugExtensions::OSPageSize() - 1));
		readSize = min(request, readSize);

		if(!ReadProcessMemory(hProcess, (PVOID)(ULONG_PTR) address,
							  buffer, readSize, &read))
		{
			if(totalDone == 0)
			{
				// If we haven't read anything indicate failure.
				status = E_FAIL;
			}
			break;
		}
		totalDone += (ULONG) read;
		address += read;
		buffer += read;
		request -= (ULONG) read;
	}

	*done = totalDone;
	return status;
}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::WriteVirtual( 
    /* [in] */ CLRDATA_ADDRESS address,
    /* [size_is][in] */ BYTE *buffer,
    /* [in] */ ULONG32 bytesRequested,
    /* [out] */ ULONG32 *bytesWritten)
{return E_NOTIMPL;}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetTLSValue( 
    /* [in] */ ULONG32 threadID,
    /* [in] */ ULONG32 index,
    /* [out] */ CLRDATA_ADDRESS *value)
{return E_NOTIMPL;}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::SetTLSValue( 
    /* [in] */ ULONG32 threadID,
    /* [in] */ ULONG32 index,
    /* [in] */ CLRDATA_ADDRESS value)
{return E_NOTIMPL;}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetCurrentThreadID( 
    /* [out] */ ULONG32 *threadID)
{return E_NOTIMPL;}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::GetThreadContext( 
    /* [in] */ ULONG32 threadID,
    /* [in] */ ULONG32 contextFlags,
    /* [in] */ ULONG32 contextSize,
    /* [size_is][out] */ BYTE *context)
{return g_ExtAdvanced->GetThreadContext(context, contextSize);}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::SetThreadContext( 
    /* [in] */ ULONG32 threadID,
    /* [in] */ ULONG32 contextSize,
    /* [size_is][in] */ BYTE *context)
{return E_NOTIMPL;}

HRESULT STDMETHODCALLTYPE LiveCLRDataTarget::Request( 
    /* [in] */ ULONG32 reqCode,
    /* [in] */ ULONG32 inBufferSize,
    /* [size_is][in] */ BYTE *inBuffer,
    /* [in] */ ULONG32 outBufferSize,
    /* [size_is][out] */ BYTE *outBuffer)
{return E_INVALIDARG;}

#pragma unmanaged