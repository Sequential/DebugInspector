#pragma once 

#define CACHE_SIZE 4096

struct ReadVirtualCache
{
    BYTE m_cache[CACHE_SIZE];
    ULONG64 m_startCache;
    BOOL m_cacheValid;
    ULONG m_cacheSize;
    
    ReadVirtualCache() { Clear(); }
    HRESULT Read(ULONG64 Offset, PVOID Buffer, ULONG BufferSize, PULONG lpcbBytesRead);    
    void Clear() { m_cacheValid = FALSE; m_cacheSize = CACHE_SIZE; }
};

