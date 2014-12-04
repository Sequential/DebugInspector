using System;
using System.Collections.Generic;
using System.Text;
using DebugInspector.Controls.Managed.ThreadStacks;
using EnvDTE80;
using DebugInspector.Controls;

namespace DebugInspector.Managed
{
    public class VisualStudioProcess : IDebugProcess
    {
        private readonly Process2 r_process;

        public VisualStudioProcess(Process2 process)
        {
            if (process == null)
                throw new ArgumentNullException("process");

            r_process = process;
        }

        #region IDebugProcess Members

        public string Name
        {
            get { return r_process.Name; }
        }

        #endregion
    }
}
