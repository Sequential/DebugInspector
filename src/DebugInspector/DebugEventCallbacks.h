#pragma once
#include "Stdafx.h"
#include <dbgeng.h>
class DebugEventCallbacks : public IDebugEventCallbacks 
{
public:
	DebugEventCallbacks(void);
	~DebugEventCallbacks(void);

	// IUnknown.
    STDMETHOD(QueryInterface)(REFIID InterfaceId, PVOID* Interface);
	STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
	
	// IDebugEventCallbacks.

    // The engine calls GetInterestMask once when
    // the event callbacks are set for a client.
    STDMETHOD(GetInterestMask)(/*__out*/ PULONG Mask);

    // A breakpoint event is generated when
    // a breakpoint exception is received and
    // it can be mapped to an existing breakpoint.
    // The callback method is given a reference
    // to the breakpoint and should release it when
    // it is done with it.
    STDMETHOD(Breakpoint)(/*__in*/ PDEBUG_BREAKPOINT Bp);

    // Exceptions include breaks which cannot
    // be mapped to an existing breakpoint
    // instance.
    STDMETHOD(Exception)(/*__in*/ PEXCEPTION_RECORD64 Exception, 
						 /*__in*/ ULONG FirstChance);

    // Any of these values can be zero if they
    // cannot be provided by the engine.
    // Currently the kernel does not return thread
    // or process change events.
    STDMETHOD(CreateThread)(/*__in*/ ULONG64 Handle,
        /*__in*/ ULONG64 DataOffset,
        /*__in*/ ULONG64 StartOffset
        ) ;
    STDMETHOD(ExitThread)(
        /*__in*/ ULONG ExitCode
        ) ;

    // Any of these values can be zero if they
    // cannot be provided by the engine.
    STDMETHOD(CreateProcess)(
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
        ) ;
    STDMETHOD(ExitProcess)(
        /*__in*/ ULONG ExitCode
        ) ;

    // Any of these values may be zero.
    STDMETHOD(LoadModule)(
        /*__in*/ ULONG64 ImageFileHandle,
        /*__in*/ ULONG64 BaseOffset,
        /*__in*/ ULONG ModuleSize,
        /*__in_opt*/ PCSTR ModuleName,
        /*__in_opt*/ PCSTR ImageName,
        /*__in*/ ULONG CheckSum,
        /*__in*/ ULONG TimeDateStamp
        ) ;
    STDMETHOD(UnloadModule)(
        __in_opt PCSTR ImageBaseName,
        /*__in*/ ULONG64 BaseOffset
        ) ;

    STDMETHOD(SystemError)(
        /*__in*/ ULONG Error,
        /*__in*/ ULONG Level
        ) ;

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
    STDMETHOD(SessionStatus)(
        /*__in*/ ULONG Status
        ) ;

    // The following callbacks are informational
    // callbacks notifying the provider about
    // changes in debug state.  The return value
    // of these callbacks is ignored.  Implementations
    // can not call back into the engine.

    // Debuggee state, such as registers or data spaces,
    // has changed.
    STDMETHOD(ChangeDebuggeeState)(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        );
    // Engine state has changed.
    STDMETHOD(ChangeEngineState)(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        );
    // Symbol state has changed.
    STDMETHOD(ChangeSymbolState)(
        /*__in*/ ULONG Flags,
        /*__in*/ ULONG64 Argument
        );
};