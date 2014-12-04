using System;
using System.Collections.Generic;
using System.Text;

namespace DebugInspector.Controls
{
    public interface IEnvironment
    {
        IDebuggerEvents AdviseDebuggerEvents(IDebugUserControl control, out uint cookie);

        void GoToAddress(String function, int offset);

        void GoToLine(String file, int line);

        void GoToURL(String url);
    }
}
