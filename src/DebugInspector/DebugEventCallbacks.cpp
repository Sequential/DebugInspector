#include "StdAfx.h"
#include "DebugEventCallbacks.h"

DebugEventCallbacks::DebugEventCallbacks(void)
{
}

DebugEventCallbacks::~DebugEventCallbacks(void)
{
}

// IUnknown.
    STDMETHODIMP DebugEventCallbacks::QueryInterface(REFIID InterfaceId, PVOID* Interface)
	{
		if (InterfaceId == IID_IUnknown || InterfaceId == IID_IDebugEventCallbacks)
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
	STDMETHODIMP_(ULONG) DebugEventCallbacks::AddRef(){return S_OK;}
	
	STDMETHODIMP_(ULONG) DebugEventCallbacks::Release(){return S_OK;}
	
	// IDebugEventCallbacks.

    // The engine calls GetInterestMask once when
    // the event callbacks are set for a client.
	STDMETHODIMP DebugEventCallbacks::GetInterestMask(/*__out*/ PULONG Mask){return DEBUG_STATUS_BREAK;}

    // A breakpoint event is generated when
    // a breakpoint exception is received and
    // it can be mapped to an existing breakpoint.
    // The callback method is given a reference
    // to the breakpoint and should release it when
    // it is done with it.
    STDMETHODIMP DebugEventCallbacks::Breakpoint(/*__in*/ PDEBUG_BREAKPOINT Bp){return DEBUG_STATUS_GO;}

    // Exceptions include breaks which cannot
    // be mapped to an existing breakpoint
    // instance.
    STDMETHODIMP DebugEventCallbacks::Exception(/*__in*/ PEXCEPTION_RECORD64 Exception, 
						 /*__in*/ ULONG FirstChance){return DEBUG_STATUS_GO;}

    // Any of these values can be zero if they
    // cannot be provided by the engine.
    // Currently the kernel does not return thread
    // or process change events.
    STDMETHODIMP DebugEventCallbacks::CreateThread(/*__in*/ ULONG64 Handle,
        /*__in*/ ULONG64 DataOffset,
        /*__in*/ ULONG64 StartOffset
        ) {return DEBUG_STATUS_GO;}
    STDMETHODIMP DebugEventCallbacks::ExitThread(
        /*__in*/ ULONG ExitCode
        ) {return DEBUG_STATUS_GO;}

    // Any of these values can be zero if they
    // cannot be provided by the engine.
    STDMETHODIMP DebugEventCallbacks::CreateProcess(
        /*__in*/ ULONG64 ImageFileHandle,
        /*__in*/ ULONG64 Handle,
        /*__in*/ ULONG64 BaseOffset,
        /*__in*/ ULONG ModuleSize,
        /*__in_opt*/ PCSTR ModuleName,
        /*__in_opt*/ PCSTR ImageName,
        /*__in*/ ULONG CheckSum,
        /*__in*/ ULONG TimeDateStamp,
        /*__in*/ ULONG64 InitialThreadHandle,
        /*__in*/ ULONG64 ThreadDataOffset,
        /*__in*/ ULONG64 StartOffset
        ) {return DEBUG_STATUS_GO;}
    STDMETHODIMP DebugEventCallbacks::ExitProcess(
        /*__in*/ ULONG ExitCode
        ) {return DEBUG_STATUS_GO;}

    // Any of these values may be zero.
    STDMETHODIMP DebugEventCallbacks::LoadModule(
        /*__in*/ ULONG64 ImageFileHandle,
        /*__in*/ ULONG64 BaseOffset,
        /*__in*/ ULONG ModuleSize,
        /*__in_opt*/ PCSTR ModuleName,
        /*__in_opt*/ PCSTR ImageName,
        /*__in*/ ULONG CheckSum,
        /*__in*/ ULONG TimeDateStamp
        ) {return DEBUG_STATUS_GO;}
    STDMETHODIMP DebugEventCallbacks::UnloadModule(
        __in_opt PCSTR ImageBaseName,
        /*__in*/ ULONG64 BaseOffset
        ) {return DEBUG_STATUS_GO;}

    STDMETHODIMP DebugEventCallbacks::SystemError(
        /*__in*/ ULONG Error,
        /*__in*/ ULONG Level
        ) {return DEBUG_STATUS_GO;}

    // Session status is synchronous like the other
    // wait callbacks but it is called as the state
    // of the session is changing rather than at
    // specific events so its return value does not
    // influence waiting.  Implementations should just
    // return DEBUG_STATUS_NO_CHANGE.
    // Also, because some of the status
    // notifications are very early or very
    // late in the session lifetime there may not be
    // current processes or threads when the notification
    // is generated.
    STDMETHODIMP DebugEventCallbacks::SessionStatus(
        /*__in*/ ULONG Status
        ) {return DEBUG_STATUS_NO_CHANGE;}

    // The following callbacks are informational
    // callbacks notifying the provider about
    // changes in debug state.  The return value
    // of these callbacks is ignored.  Implementations
    // can not call back into the engine.

    // Debuggee state, such as registers or data spaces,
    // has changed.
    STDMETHODIMP DebugEventCallbacks::ChangeDebuggeeState(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        ){return DEBUG_STATUS_NO_CHANGE;}
    // Engine state has changed.
    STDMETHODIMP DebugEventCallbacks::ChangeEngineState(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        ){return DEBUG_STATUS_NO_CHANGE;}
    // Symbol state has changed.
    STDMETHODIMP DebugEventCallbacks::ChangeSymbolState(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        ){return DEBUG_STATUS_NO_CHANGE;}
