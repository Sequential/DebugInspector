#include "StdAfx.h"
#include "ClrStackWalker.h"
#include "Dac.h"
#include "Stdio.h"

ClrStackWalker::ClrStackWalker(void)
{

}



HRESULT ClrStackWalker::Create(ClrStackFrame** ppClrFrame)
{
	HRESULT hr = S_OK;
	IXCLRDataStackWalk *pStackWalk = NULL;
	IXCLRDataTask* pTask;
	ULONG ThreadId;

	if ((hr = g_ExtSystem->GetCurrentThreadSystemId(&ThreadId)) != S_OK ||
		(hr = g_clrData->GetTaskByOSThreadID(ThreadId, &pTask)) != S_OK)
	{
		g_ext.Out("Skipping Thread %d as it is likely not a managed thread", ThreadId);
		ClrStackFrame* pBlankFrame  = new ClrStackFrame();
		pBlankFrame->Status = STATUS_UNMANAGED;
		return hr;
	}

	hr = pTask->CreateStackWalk(   CLRDATA_SIMPFRAME_UNRECOGNIZED |
								   CLRDATA_SIMPFRAME_MANAGED_METHOD | 
								   CLRDATA_SIMPFRAME_RUNTIME_MANAGED_CODE |
								   CLRDATA_SIMPFRAME_RUNTIME_UNMANAGED_CODE,
								   &pStackWalk);
	pTask->Release();

	if (hr != S_OK)
	{
		g_ext.Out("Failed to start stack walk: %lx\n",hr);
		ClrStackFrame* pBlankFrame  = new ClrStackFrame();
		pBlankFrame->Status = STATUS_UNINIT_STACK;
		return S_FALSE;
	}

	if(*ppClrFrame)
	{
		delete (*ppClrFrame);
		(*ppClrFrame) = NULL;
	}

	ClrStackFrame* pClrFrame  = NULL;
	/*ULONG64 StackOffset;
	g_ExtRegisters->GetStackOffset (&StackOffset);
	g_ExtRegisters->GetInstructionOffset(&StackOffset);
	g_ExtRegisters->GetFrameOffset(&StackOffset);*/

	while (1)
	{
		ClrStackFrame* pTempFrame  = new ClrStackFrame();
		if(!(*ppClrFrame))
		{
			(*ppClrFrame) = pClrFrame;
		}
		if(pClrFrame)
		{
			pClrFrame->pNextFrame = pTempFrame;
		}
		pClrFrame = pTempFrame;
		
		if (g_ext.IsInterrupt())
		{
			hr = E_ABORT;
			g_ext.Out ("<interrupted>\n");
			pClrFrame->Status = STATUS_INTERRUPTED;
			goto Exit;
		}
        
		DacpFrameData FrameData;
        
		if ((hr = FrameData.Request(pStackWalk)) != S_OK)
		{
			g_ext.Out("Get frame data failed with %lx\n",hr);
			pClrFrame->Status = STATUS_GET_FRAME_DATA_FAILED;
			goto Exit;
		}

		CONTEXT context;
		if ((hr=pStackWalk->GetContext(CONTEXT_ALL, sizeof(CONTEXT),
										   NULL, (BYTE *)&context))!=S_OK)
		{
			g_ext.Out("GetFrameContext failed: %lx\n",hr);
			pClrFrame->Status = STATUS_GET_FRAME_CTX_FAILED;
			goto Exit;
		}

		pClrFrame->Esp = FrameData.frameAddr ? (DWORD)FrameData.frameAddr : context.Esp;
		pClrFrame->Eip = context.Eip;
		pClrFrame->Address = FrameData.frameAddr;
		
		if (FrameData.frameAddr)
		{
			static WCHAR wszNameBuffer[1024]; // should be large enough
			DWORD_PTR vtAddr = NULL;
			Move<DWORD_PTR>(vtAddr, FrameData.frameAddr);
			if (DacpFrameData::GetFrameName (g_clrData, vtAddr, 1024, wszNameBuffer) == S_OK)
			{
				pClrFrame->SetFrameName(wszNameBuffer);
			}
			else
			{          
				pClrFrame->SetFrameName(L"Frame:");
			}
		}

		

		DacpMethodDescData mdescData;

		static char filename[MAX_PATH+1];
		static wchar_t mdname[1024];
        ULONG line;
		CLRDATA_ADDRESS modulePtr;

		
		if (!FrameData.frameAddr)
		{		
			hr = g_ExtSymbols->GetLineByOffset(context.Eip, &line, filename, 1025, NULL, NULL);
			if(SUCCEEDED(hr))
			{
				pClrFrame->SetFileName(filename);
				pClrFrame->Line = line;
			}

			// we may have a method, try to get the methoddesc
			if (mdescData.RequestFromIP(g_clrData,context.Eip)==S_OK)
			{
				DebugExtensions::GetMDInfo(mdname,1024,mdescData.MethodDescPtr);
				pClrFrame->SetMethodName(mdname);
				pClrFrame->Managed = TRUE;
				RetrieveArgsAndLocals (pStackWalk, *pClrFrame, TRUE, TRUE);
				modulePtr = mdescData.ModulePtr;

				pClrFrame->StackHeaderOffset = context.Eip - mdescData.NativeCodeAddr;
			}
		}
		else
		{
			// See if the frame has associated function info.
			if (mdescData.RequestFromFrame(g_clrData,FrameData.frameAddr)==S_OK)
			{
				DebugExtensions::GetMDInfo(mdname,1024,mdescData.MethodDescPtr);
				pClrFrame->SetMethodName(mdname);
				pClrFrame->Managed = FALSE;
				CheckMonitorEnter(pStackWalk, *pClrFrame);
				CheckThreadJoin(pStackWalk, context, *pClrFrame);
				modulePtr = mdescData.ModulePtr;

				pClrFrame->StackHeaderOffset = 0;//context.Eip - mdescData.NativeCodeAddr;
			}
		}

		DacpModuleData module;
		if(module.Request(g_clrData, modulePtr)==S_OK)
		{
			IXCLRDataModule *pModuleDefinition = module.ModuleDefinition;
			ULONG32 moduleNameLen = 0;
			if(pModuleDefinition)
			{
				if(SUCCEEDED(pModuleDefinition->GetFileName(1024, &moduleNameLen, mdname)))
				{
					pClrFrame->SetModuleFileName(mdname);
				}
			}
		}
        
		if (pStackWalk->Next()!=S_OK)
			break;
	}

	Exit:
	if (pStackWalk)
	{
		pStackWalk->Release();
		pStackWalk = NULL;
	}
	return hr == S_FALSE? S_OK: hr;		
}

HRESULT ClrStackWalker::CheckMonitorEnter(IXCLRDataStackWalk *pStackWalk, ClrStackFrame& frame)
{
	int cmp = ::wcscmp(frame.pMethodName, L"System.Threading.Monitor.Enter(System.Object)");
	if(cmp==0)
	{
		frame.InMonitorEnter = TRUE;
		frame.ArgCount = 1;
		frame.LocalsCount = 0;

		HRESULT hr = S_OK;
		DWORD_PTR StackTop = (DWORD_PTR)frame.Address;
		DWORD_PTR ptr = StackTop & ~3;  // make certain dword aligned

		DWORD_PTR StackBottom = StackTop + 0xFFFF;
		for (;ptr < StackBottom; ptr += sizeof(DWORD_PTR))
		{		
			DWORD_PTR objAddr = NULL;
			Move<DWORD_PTR>(objAddr, ptr);
			//if(FAILED(hr))
			//	return hr;

			// rule out misidentification of a stack address as an object.
			if (objAddr>=StackTop && objAddr<=StackBottom)
				continue;

			DacpObjectData objectData;			
			if ((hr = objectData.Request(g_clrData,(CLRDATA_ADDRESS)objAddr)) != S_OK)
			{
				continue;
			}
			frame.WaitingOn = (CLRDATA_ADDRESS)objAddr;

			if ((hr = DacpObjectData::GetObjectClassName(g_clrData,(CLRDATA_ADDRESS)objAddr,mdNameLen,g_mdName))==S_OK)
			{
				frame.SetWaitingOnName(g_mdName);
				return S_OK;
			}
			frame.SetWaitingOnName(L"Unknown");
			return hr;
		}
		return E_UNEXPECTED;
	}
	else
	{
		return S_OK;
	}
}

HRESULT ClrStackWalker::CheckThreadJoin(IXCLRDataStackWalk *pStackWalk, CONTEXT& context, ClrStackFrame& frame)
{
	//return S_OK;
	int cmp = ::wcscmp(frame.pMethodName, L"System.Threading.Thread.JoinInternal()");
	if(cmp==0)
	{
		frame.InThreadJoin = TRUE;
		frame.ArgCount = 1;
		frame.LocalsCount = 0;

		HRESULT hr = S_OK;
		DWORD_PTR StackTop = (DWORD_PTR)frame.Address;
		DWORD_PTR ptr = StackTop & ~3;  // make certain dword aligned

		DWORD_PTR StackBottom = StackTop + 0xFFFF;
		for (;ptr < StackBottom; ptr += sizeof(DWORD_PTR))
		{		
			DWORD_PTR objAddr = NULL;
			Move<DWORD_PTR>(objAddr, ptr);
			//if(FAILED(hr))
			//	return hr;

			// rule out misidentification of a stack address as an object.
			if (objAddr>=StackTop && objAddr<=StackBottom)
				continue;

			DacpObjectData objectData;			
			if ((hr = objectData.Request(g_clrData,(CLRDATA_ADDRESS)objAddr)) != S_OK)
			{
				continue;
			}

			if ((hr = DacpObjectData::GetObjectClassName(g_clrData,(CLRDATA_ADDRESS)objAddr,mdNameLen,g_mdName))==S_OK)
			{
				frame.SetWaitingOnName(g_mdName);
			}
			else
			{
				frame.SetWaitingOnName(L"Unknown");
				return hr;
			}

			//////////// get member ////////////////

			DacpEEClassData EECls;
			if ((hr=EECls.Request(g_clrData,objectData.EEClass))!=S_OK)
			{
				g_ext.Out("Invalid EEClass address\n");
				return hr;
			}  

			// Get the module name
			DacpModuleData module;
			if ((hr=module.Request(g_clrData,EECls.Module))!=S_OK)
			{
				return hr;    
			}
			IMetaDataImport *pImport = MDImportForModule(&module);

			DacpMethodTableData vMethTable;
			if ((hr = vMethTable.Request(g_clrData,EECls.MethodTable))!=S_OK)
			{
				return hr;    
			}
			BOOL fIsShared = vMethTable.bIsShared;

			
			CLRDATA_ADDRESS dwStartAddr = objAddr;
			CLRDATA_ADDRESS dwAddr = EECls.FirstField;
			DacpFieldDescData vFieldDesc;
			int numInstanceFields = 0;

			
			while(numInstanceFields < EECls.wNumInstanceFields)
			{
				if ((vFieldDesc.Request (g_clrData, (CLRDATA_ADDRESS) dwAddr)!=S_OK) || (vFieldDesc.Type >= ELEMENT_TYPE_MAX))
				{
					g_ext.Out("Unable to display fields\n");
					return E_FAIL;
				}


				dwAddr = vFieldDesc.NextField;
				DWORD offset = vFieldDesc.dwOffset;
				BOOL bValueClass = FALSE;

				if(!((vFieldDesc.bIsThreadLocal || vFieldDesc.bIsContextLocal || fIsShared) && vFieldDesc.bIsStatic))
				{
					if (!bValueClass)
					{
						offset += sizeof(BaseObject);
					}
				}

				if(!vFieldDesc.bIsStatic)
				{
					numInstanceFields ++;
		            
					NameForToken_s (TokenFromRid(vFieldDesc.mb, mdtFieldDef), pImport, g_mdName, mdNameLen, false);
					int cmp = ::wcscmp(g_mdName, L"m_ManagedThreadId");

					if(cmp==0)
					{
						if (dwStartAddr > 0)
						{
							DWORD_PTR dwTmp = dwStartAddr + vFieldDesc.dwOffset
											+ (bValueClass ? 0 : sizeof(BaseObject));
							
							union Value
							{
								char ch;
								short Short;
								DWORD_PTR ptr;
								int Int;
								unsigned int UInt;
								__int64 Int64;
								unsigned __int64 UInt64;
								float Float;
								double Double;
							} value;

							ZeroMemory(&value, sizeof(value));							
							MoveBlock<Value>(value, (ULONG64)dwTmp, sizeof(int));

							int threadId = value.Int;
							frame.JoiningThreadId = threadId;
							frame.WaitingOn = (CLRDATA_ADDRESS)threadId;
							return S_OK;
						}
					}
				}
				
			}
			
			return hr;
		}
		
		return E_UNEXPECTED;
	}
	else
	{
		return S_OK;
	}
}

void ClrStackWalker::RetrieveArgsAndLocals(IXCLRDataStackWalk *pStackWalk, ClrStackFrame& frame, BOOL bArgs, BOOL bLocals)
{
    // Okay, what can I get for arguments?
    IXCLRDataFrame *pFramey = NULL;
    IXCLRDataValue *pVal = NULL;
    
    if (FAILED(pStackWalk->GetFrame(&pFramey)))
    {
        goto TheExit;
    }
    
    ULONG32 argy;
    ULONG32 localy;
    if (FAILED(pFramey->GetNumArguments(&argy)))
    {
        goto TheExit;
    }

    if (FAILED(pFramey->GetNumLocalVariables(&localy)))
    {
        goto TheExit;
    }

	frame.ArgCount = argy;
	frame.LocalsCount = localy;

    if (bArgs)
    {        
		if(frame.pFirstArg)
		{
			delete frame.pFirstArg;
			frame.pFirstArg = NULL;
		}

        WCHAR argName[mdNameLen];
		ClrStackFrameArg* pArg = NULL;

        for (ULONG32 i=0; i < argy; i++)
        {               
			ClrStackFrameArg* pTempArg = new ClrStackFrameArg();
			if(!frame.pFirstArg)
			{
				frame.pFirstArg = pArg;
			}
			if(pArg)
			{
				pArg->pNext = pTempArg;
			}
			pArg = pTempArg;

            ULONG32 realNameLen;
            HRESULT hr;
            hr = pFramey->GetArgumentByIndex(i,
                                   &pVal,
                                   sizeof(argName) / sizeof(WCHAR),
                                   &realNameLen,
                                   argName);
            if (FAILED(hr))
            {
                goto TheExit;
            }
			CLRDATA_ADDRESS address = 0;
			hr = pVal->GetAddress(&address);
			pArg->Address = address;
			pArg->SetName(argName);

            ULONG32 dwSize = 0;
            hr = pVal->GetBytes(0,&dwSize,NULL);
            if (HRESULT_CODE(hr) == ERROR_BUFFER_OVERFLOW)
            {
                BYTE *pByte = new BYTE[dwSize];
                if (pByte == NULL)
                {
                    goto TheExit;
                }
                hr = pVal->GetBytes(dwSize,&dwSize,pByte);
                if (FAILED(hr))
                {
                    g_ext.Out ("<unable to retrieve data>\n");
                }
                else
                {
                    ULONG64 outVar = 0;
                    switch(dwSize)
                    {
                        case 1: outVar = *((BYTE *) pByte); break;
                        case 2: outVar = *((short *) pByte); break;
                        case 4: outVar = *((DWORD *) pByte); break;
                        case 8: outVar = *((ULONG64 *) pByte); break;
                        default: outVar = 0;
                    }
					pArg->Value = (ULONG64) outVar;
                }
                delete [] pByte;
            }
            pVal->Release();
            pVal = NULL;
        }
    }    

    if (bLocals)
    {  
		if(frame.pFirstLocal)
		{
			delete frame.pFirstLocal;
			frame.pFirstLocal = NULL;
		}

		WCHAR localName[mdNameLen];

		ClrStackFrameLocal* pLocal = NULL;

        for (ULONG32 i=0; i < localy; i++)
		{   

			ClrStackFrameLocal* pTempLocal = new ClrStackFrameLocal();
			if(!frame.pFirstLocal)
			{
				frame.pFirstLocal = pLocal;
			}
			if(pLocal)
			{
				pLocal->pNext = pTempLocal;
			}
			pLocal = pTempLocal;
           
            HRESULT hr;
            ULONG32 namelen = 0;

            hr = pFramey->GetLocalVariableByIndex(i,
                                   &pVal,
                                   mdNameLen,&namelen,localName);
            if (FAILED(hr))
            {
                goto TheExit;
            }
			pLocal->SetName(localName);

            ULONG32 numLocations;
            if (SUCCEEDED(pVal->GetNumLocations(&numLocations)) &&
                numLocations == 1)
            {
                ULONG32 flags;
                CLRDATA_ADDRESS addr;
                if (SUCCEEDED(pVal->GetLocationByIndex(0, &flags, &addr)))
                {
                    if (flags == CLRDATA_VLOC_REGISTER)
                    {
                        pLocal->ClrRegister = TRUE;
                    }
                    else
                    {
                        pLocal->Address = (ULONG64)addr;
                    }
                }
            }
            ULONG32 dwSize = 0;
            hr = pVal->GetBytes(0,&dwSize,NULL);
            if (HRESULT_CODE(hr) == ERROR_BUFFER_OVERFLOW)
            {
                BYTE *pByte = new BYTE[dwSize];
                if (pByte == NULL)
                {
                    goto TheExit;
                }
                hr = pVal->GetBytes(dwSize,&dwSize,pByte);
                if (FAILED(hr))
                {
                    g_ext.Out ("<unable to retrieve data>\n");
                }
                else
                {
                    ULONG64 outVar = 0;
                    switch(dwSize)
                    {
                        case 1: outVar = *((BYTE *) pByte); break;
                        case 2: outVar = *((short *) pByte); break;
                        case 4: outVar = *((DWORD *) pByte); break;
                        case 8: outVar = *((ULONG64 *) pByte); break;
                        default: outVar = 0;
                    }
					pLocal->Value = (ULONG64) outVar;
                }
                delete [] pByte;
            }
            
            pVal->Release();
            pVal = NULL;
        }        
    }
    
TheExit:
    if (pVal)
        pVal->Release();
    if (pFramey)
        pFramey->Release();
}

ClrStackWalker::~ClrStackWalker(void)
{
}
