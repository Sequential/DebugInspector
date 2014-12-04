using System;
using System.Collections.Generic;
using System.Text;
using DebugInspector.Controls.Managed.ThreadStacks;
using EnvDTE;
using DebugInspector.Managed;
using Microsoft.VisualStudio.Shell.Interop;
using DebugInspector.Controls;

namespace DebugInspector.Debug
{
    public class VisualStudioEnvironment : IEnvironment
    {
        private readonly DTE r_dte;
        private readonly VisualStudioNavHelper r_navhelper;

        internal VisualStudioEnvironment(DTE dte)
        {
            if (dte == null)
                throw new ArgumentNullException("dte");

            r_dte = dte;
            r_navhelper = new VisualStudioNavHelper();
        }

        #region IEnvironment Members


        public IDebuggerEvents AdviseDebuggerEvents(IDebugUserControl control, out uint cookie)
        {
            VisualStudioDebuggerEvents vsde = new VisualStudioDebuggerEvents(r_dte, control);
            IVsDebugger vsd = control.GetDebugService(typeof(IVsDebugger)) as IVsDebugger;
            int hr = vsd.AdviseDebuggerEvents(vsde, out cookie);
            return vsde;
        }

        #endregion

        #region IEnvironment Members


        public void GoToAddress(string function, int offset)
        {
            VisualStudioNavHelper.GoToAddress(r_dte, function, offset);
        }

        public void GoToLine(string file, int line)
        {
            VisualStudioNavHelper.GoToLine(r_dte, file, line);
        }

        public void GoToURL(string url)
        {
            VisualStudioNavHelper.GoToUrl(r_dte, url);
        }

        #endregion

        
    }
}
