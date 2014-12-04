using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace DebugInspector.Controls.UnManaged
{
    public class DebugProcess : IDebugProcess
    {
        private readonly String r_name;
        public DebugProcess(Process process)
        {
            r_name = process.ProcessName;
        }
        #region IDebugProcess Members

        public string Name
        {
            get { return r_name; }
        }

        #endregion
    }
}
