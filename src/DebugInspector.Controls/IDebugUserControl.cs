using System;
using System.Collections.Generic;
using System.Text;
using ClrInspector;

namespace DebugInspector.Controls
{
    public interface IDebugUserControl
    {
        void EnterNonBreakMode();
        object GetDebugService(Type type);
        void Refresh(Exception ex);
        void Refresh(ExpiredException ex);
        void Refresh(ThreadStackManager manager, IDebugProcess process, Dictionary<int, String> threadNames);
        
    }
}
