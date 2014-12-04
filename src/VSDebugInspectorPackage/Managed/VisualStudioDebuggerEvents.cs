using System;
using System.Collections.Generic;
using System.Text;
using DebugInspector.Controls.Managed.ThreadStacks;
using Microsoft.VisualStudio.Shell.Interop;
using EnvDTE;
using EnvDTE80;
using ClrInspector;
using DebugInspector.Controls;

namespace DebugInspector.Managed
{
    class VisualStudioDebuggerEvents: IVsDebuggerEvents, IDebuggerEvents
    {
        private readonly DTE r_dte;
        private readonly IDebugUserControl r_myControl;

        internal VisualStudioDebuggerEvents(DTE dte, IDebugUserControl myControl)
        {
            if (dte == null)
                throw new ArgumentNullException("dte");

            if (myControl == null)
                throw new ArgumentNullException("myControl");

            r_myControl = myControl;

            r_dte = dte;
        }

       
        #region IVsDebuggerEvents Members

        public int OnModeChange(DBGMODE dbgmodeNew)
        {
            if (dbgmodeNew == DBGMODE.DBGMODE_Break)
            {
                ProcessBreakMode();
            }
            else
            {
                ProcessRunMode();
            }
            return 0;
        }

        private void ProcessRunMode()
        {
            this.r_myControl.EnterNonBreakMode();
        }

        private void ProcessBreakMode()
        {
            try
            {
                Process2 process = (Process2)r_dte.Debugger.DebuggedProcesses.Item(1);

                Dictionary<int, String> threadNames = new Dictionary<int, string>();
                Threads threads = process.Threads;
                int count = threads.Count;
                for (int i = 1; i <= count; ++i)
                {
                    Thread thread = threads.Item(i);
                    int id = thread.ID;
                    String name = thread.Name;
                    threadNames.Add(id, name);
                }

                int procid = process.ProcessID;
                System.Diagnostics.Process proc = System.Diagnostics.Process.GetProcessById(procid);

                using (Dac dac = new Dac(proc))
                {
                    ThreadStackManager man = dac.ConstructThreadStacks();
                    VisualStudioProcess vsProc = new VisualStudioProcess(process);
                    r_myControl.Refresh(man, vsProc, threadNames);
                }
            }
            catch (ExpiredException ex)
            {
                r_myControl.Refresh(ex);
            }
            catch (Exception ex)
            {
                r_myControl.Refresh(ex);
            }
        }

        #endregion
    }
}
