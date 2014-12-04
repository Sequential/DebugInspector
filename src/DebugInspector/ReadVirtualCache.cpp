#pragma unmanaged
#include "StdAfx.h"
#include "ReadVirtualCache.h"
#include "Dac.h"

HRESULT ReadVirtualCache::Read(ULONG64 Offset, PVOID Buffer, ULONG BufferSize, PULONG lpcbBytesRead)
{
	// Needs testing as it doesnt work
	return g_ExtData->ReadVirtual(Offset, Buffer, BufferSize, lpcbBytesRead);


    // Offset can be any random ULONG64, as it can come from VerifyObjectMember(), and this
    // can pass random pointer values in case of GC heap corruption
    HRESULT ret;
    ULONG cbBytesRead = 0;

    if (BufferSize == 0)
        return S_OK;

    if (BufferSize > CACHE_SIZE)
    {
        // Don't even try with the cache
        return g_ExtData->ReadVirtual(Offset, Buffer, BufferSize, lpcbBytesRead);
    }
 
    if ((m_cacheValid)
        && (Offset >= m_startCache) 
        && (Offset <= m_startCache + m_cacheSize - BufferSize))

    {
        // It is within the cache
        memcpy(Buffer,(LPVOID) ((ULONG64)m_cache + (Offset - m_startCache)), BufferSize);

        if (lpcbBytesRead != NULL)
        {
           *lpcbBytesRead = BufferSize;
        }
 
        return S_OK;
    }
 
    m_cacheValid = FALSE;
    m_startCache = Offset;

    // avoid an int overflow
    if (m_startCache + CACHE_SIZE < m_startCache)
        m_startCache = (ULONG64)(-CACHE_SIZE);

    ret = g_ExtData->ReadVirtual(m_startCache, m_cache, CACHE_SIZE, &cbBytesRead);
    if (ret != S_OK)
    {
        return ret;
    }
    
    m_cacheSize = cbBytesRead;     
    m_cacheValid = TRUE;
    memcpy(Buffer, (LPVOID) ((ULONG64)m_cache + (Offset - m_startCache)), BufferSize);

    if (lpcbBytesRead != NULL)
    {
        *lpcbBytesRead = cbBytesRead;
    }

    return S_OK;
}

