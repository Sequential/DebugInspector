using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using UnmanagedInspector;
using DebugInspector.Controls.UnManaged.ThreadStacks;

namespace DebugInspector.Controls.UnManaged
{
    public partial class UnmanagedThreadStacksControl : UserControl
    {
        private ThreadStackNodeManager r_nodeview;

        public UnmanagedThreadStacksControl()
        {
            InitializeComponent();

            r_nodeview = new ThreadStackNodeManager(tvwHierarchy, "Application X");
        }

        private void toolStripGo_Click(object sender, EventArgs e)
        {
            try
            {
                Go();
            }
            catch(Exception ex)
            {
                Refresh(ex);
            }
        }


        private void Go()
        {
            string strProcessId = toolStripProcessId.Text;
            int nProcessId = int.Parse(strProcessId);
            Process process = Process.GetProcessById(nProcessId);
            DebugProcess dp = new DebugProcess(process);
            Dictionary<int, String> names = new Dictionary<int, string>();
            ClrWaitSnapshot wss = new ClrWaitSnapshot(process);
            ThreadStackManager tsm = wss.Initialize();

            Refresh(tsm, dp, names);
        }

        public void Refresh(Exception ex)
        {
            r_nodeview.Refresh(ex);
        }

        public void Refresh(ThreadStackManager manager, IDebugProcess process, Dictionary<int, String> threadNames)
        {
            try
            {
                r_nodeview.Refresh(manager, process, threadNames);
            }
            catch (Exception e)
            {
                Refresh(e);
            }
        }

    }
}
