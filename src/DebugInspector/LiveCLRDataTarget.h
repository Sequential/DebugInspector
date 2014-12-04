#pragma once
#pragma managed
namespace Diag = System::Diagnostics;

class LiveCLRDataTarget : public ICLRDataTarget
{
	gcroot<Diag::Process^> m_process;

public:

	LiveCLRDataTarget(Diag::Process^ process);
	~LiveCLRDataTarget(void);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		THIS_
		IN REFIID InterfaceId,
		OUT PVOID* Interface);

	virtual STDMETHODIMP_(ULONG) AddRef(THIS);

	virtual STDMETHODIMP_(ULONG) Release(THIS);

	virtual HRESULT STDMETHODCALLTYPE GetMachineType( 
            /* [out] */ ULONG32 *machineType);
        
    virtual HRESULT STDMETHODCALLTYPE GetPointerSize( 
        /* [out] */ ULONG32 *pointerSize);
    
    virtual HRESULT STDMETHODCALLTYPE GetImageBase( 
        /* [string][in] */ LPCWSTR imagePath,
        /* [out] */ CLRDATA_ADDRESS *baseAddress);
    
    virtual HRESULT STDMETHODCALLTYPE ReadVirtual( 
        /* [in] */ CLRDATA_ADDRESS address,
        /* [length_is][size_is][out] */ BYTE *buffer,
        /* [in] */ ULONG32 bytesRequested,
        /* [out] */ ULONG32 *bytesRead);
    
    virtual HRESULT STDMETHODCALLTYPE WriteVirtual( 
        /* [in] */ CLRDATA_ADDRESS address,
        /* [size_is][in] */ BYTE *buffer,
        /* [in] */ ULONG32 bytesRequested,
        /* [out] */ ULONG32 *bytesWritten);
    
    virtual HRESULT STDMETHODCALLTYPE GetTLSValue( 
        /* [in] */ ULONG32 threadID,
        /* [in] */ ULONG32 index,
        /* [out] */ CLRDATA_ADDRESS *value);
    
    virtual HRESULT STDMETHODCALLTYPE SetTLSValue( 
        /* [in] */ ULONG32 threadID,
        /* [in] */ ULONG32 index,
        /* [in] */ CLRDATA_ADDRESS value);
    
    virtual HRESULT STDMETHODCALLTYPE GetCurrentThreadID( 
        /* [out] */ ULONG32 *threadID);
    
    virtual HRESULT STDMETHODCALLTYPE GetThreadContext( 
        /* [in] */ ULONG32 threadID,
        /* [in] */ ULONG32 contextFlags,
        /* [in] */ ULONG32 contextSize,
        /* [size_is][out] */ BYTE *context);
    
    virtual HRESULT STDMETHODCALLTYPE SetThreadContext( 
        /* [in] */ ULONG32 threadID,
        /* [in] */ ULONG32 contextSize,
        /* [size_is][in] */ BYTE *context);
    
    virtual HRESULT STDMETHODCALLTYPE Request( 
        /* [in] */ ULONG32 reqCode,
        /* [in] */ ULONG32 inBufferSize,
        /* [size_is][in] */ BYTE *inBuffer,
        /* [in] */ ULONG32 outBufferSize,
        /* [size_is][out] */ BYTE *outBuffer);
};
#pragma unmanaged
