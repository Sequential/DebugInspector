using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Shell;
using DebugInspector.Controls.Managed.ThreadStacks;
using EnvDTE;
using DebugInspector.Debug;
using DebugInspector.Controls;

namespace DebugInspector
{
    /// <summary>
    /// This class implements the tool window exposed by this package and hosts a user control.
    ///
    /// In Visual Studio tool windows are composed of a frame (implemented by the shell) and a pane, 
    /// usually implemented by the package implementer.
    ///
    /// This class derives from the ToolWindowPane class provided from the MPF in order to use its 
    /// implementation of the IVsWindowPane interface.
    /// </summary>
    [Guid("fbd48652-c3e2-4b5e-bf94-816f18cf486f")]
    public class ManagedThreadStacksToolWindow : ToolWindowPane
    {
        // This is the user control hosted by the tool window; it is exposed to the base class 
        // using the Window property. Note that, even if this class implements IDispose, we are
        // not calling Dispose on this object. This is because ToolWindowPane calls Dispose on 
        // the object returned by the Window property.
        private ManagedThreadStacksControl control;

        /// <summary>
        /// Standard constructor for the tool window.
        /// </summary>
        public ManagedThreadStacksToolWindow() :
            base(null)
        {
            // Set the window title reading it from the resources.
            this.Caption = Resources.ToolWindowTitle;
            // Set the image that will appear on the tab of the window frame
            // when docked with an other window
            // The resource ID correspond to the one defined in the resx file
            // while the Index is the offset in the bitmap strip. Each image in
            // the strip being 16x16.
            this.BitmapResourceID = 399;
            this.BitmapIndex = 0;

            control = new ManagedThreadStacksControl();
        }

        /// <summary>
        /// This property returns the handle to the user control that should
        /// be hosted in the Tool Window.
        /// </summary>
        override public IWin32Window Window
        {
            get
            {
                return (IWin32Window)control;
            }
        }

        public override void OnToolWindowCreated()
        {
            base.OnToolWindowCreated();

            try
            {
                IDebugUserControl dbc = control;
                DTE dte = dbc.GetDebugService(typeof(DTE)) as DTE;
                VisualStudioEnvironment env = new VisualStudioEnvironment(dte);
                control.Initialize(env as IEnvironment);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error in DebugInspector Creation");
                throw;
            }
        }

    }
}
