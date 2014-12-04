using System.Security.Permissions;
using System.Windows.Forms;
using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Collections.Generic;
using System.Text;
using ClrInspector;
using DebugInspector.Controls.Managed.ThreadStacks;
using DebugInspector.Controls;

namespace DebugInspector
{
    /// <summary>
    /// Summary description for MyControl.
    /// </summary>
    public partial class ManagedThreadStacksControl : UserControl, IDebugUserControl
    {
        private ContextMenuStrip _contextMenuStrip;
        private ToolStripMenuItem _ctxModules;
        private ToolStripMenuItem _ctxLineNumbers;
        private ToolStripMenuItem _ctxByteOffsets;
        private ToolStripMenuItem _ctxHexDisplay;

        private IDebuggerEvents _debugEvents;
        private IEnvironment _dte;
        private uint _debugEventsCookie;
        private ThreadStackNodeManager r_nodeview;

        public ManagedThreadStacksControl()
        {
            InitializeComponent();
            _contextMenuStrip = new ContextMenuStrip();

            r_nodeview = new ThreadStackNodeManager(tvwHierarchy, "Application X");
            
            InitCtxMenu();
        }

        private void InitCtxMenu()
        {
            _contextMenuStrip.ShowCheckMargin = false;

            _ctxHexDisplay = new ToolStripMenuItem("Hexidecimal Display", null, new EventHandler(OnHexDisplayToggle));
            _ctxHexDisplay.Checked = true;

            _ctxModules = new ToolStripMenuItem("Show Module Names", null, new EventHandler(OnModuleNamesToggle));
            _ctxModules.Checked = true;

            _ctxLineNumbers = new ToolStripMenuItem("Show Line Numbers", null, new EventHandler(OnLineNumbersToggle));
            _ctxLineNumbers.Checked = true;
            _ctxByteOffsets = new ToolStripMenuItem("Show Byte Offsets", null, new EventHandler(OnByteOffsetsToggle));
            _ctxByteOffsets.Checked = true;

            _contextMenuStrip.Items.Add(_ctxHexDisplay);
            _contextMenuStrip.Items.Add("-");
            _contextMenuStrip.Items.Add(_ctxModules);
            _contextMenuStrip.Items.Add(_ctxLineNumbers);
            _contextMenuStrip.Items.Add(_ctxByteOffsets);
        }

        private void OnModuleNamesToggle(object sender, EventArgs args)
        {
            _ctxModules.Checked = !_ctxModules.Checked;
            r_nodeview.ModuleNames = _ctxModules.Checked;
            r_nodeview.Refresh();
        }

        private void OnLineNumbersToggle(object sender, EventArgs args)
        {
            _ctxLineNumbers.Checked = !_ctxLineNumbers.Checked;
            r_nodeview.LineNumbers = _ctxLineNumbers.Checked;
            r_nodeview.Refresh();
        }

        private void OnByteOffsetsToggle(object sender, EventArgs args)
        {
            _ctxByteOffsets.Checked = !_ctxByteOffsets.Checked;
            r_nodeview.Offsets = _ctxByteOffsets.Checked;
            r_nodeview.Refresh();
        }

        private void OnHexDisplayToggle(object sender, EventArgs args)
        {
            _ctxHexDisplay.Checked = !_ctxHexDisplay.Checked;
            r_nodeview.HexValue = _ctxHexDisplay.Checked;
            r_nodeview.Refresh();
        }

        public void Initialize(IEnvironment dte)
        {
            _dte = dte;
            _debugEvents = dte.AdviseDebuggerEvents(this, out _debugEventsCookie);
            this.ContextMenuStrip = _contextMenuStrip;
        }

        public void EnterNonBreakMode()
        {
            r_nodeview.EnterNonBreakMode();
        }

        public void Refresh(Exception ex)
        {
            r_nodeview.Refresh(ex);
        }

        public void Refresh(ExpiredException ex)
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

        /// <summary> 
        /// Let this control process the mnemonics.
        /// </summary>
        [UIPermission(SecurityAction.LinkDemand, Window = UIPermissionWindow.AllWindows)]
        protected override bool ProcessDialogChar(char charCode)
        {
              // If we're the top-level form or control, we need to do the mnemonic handling
              if (charCode != ' ' && ProcessMnemonic(charCode))
              {
                    return true;
              }
              return base.ProcessDialogChar(charCode);
        }

        /// <summary>
        /// Enable the IME status handling for this control.
        /// </summary>
        protected override bool CanEnableIme
        {
            get
            {
                return true;
            }
        }

        private void tvwHierarchy_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            try
            {
                INavItem navItem = e.Node.Tag as INavItem;
                if (navItem != null)
                {
                    navItem.NavigateTo(_dte);
                    this.Focus();
                }
            }
            catch (Exception ex)
            {
                Refresh(ex);
            }
        }

        private void tsHelpSite_Click(object sender, EventArgs e)
        {
            _dte.GoToURL("http://www.debuginspector.com");
        }

        #region IDebugControl Members

        object IDebugUserControl.GetDebugService(Type type)
        {
            return base.GetService(type);
        }

        #endregion
    }
}
