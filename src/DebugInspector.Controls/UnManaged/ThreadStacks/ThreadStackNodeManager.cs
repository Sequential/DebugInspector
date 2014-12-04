using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ThreadStackNodeManager
    {
        private readonly TreeView r_treeview;
        private String _appName;

        private Boolean _hexValues;
        private Boolean _moduleNames;
        private Boolean _lineNumbers;
        private Boolean _offsets;

        private ThreadStackManager _tsm;
        
        private Dictionary<int, String> _threadNames;

        
        private RootNode _rootNodeNew;

        public ThreadStackNodeManager(TreeView treeview, String appName)
        {
            if (treeview == null)
                throw new ArgumentNullException("treeview");

            if (appName == null)
                throw new ArgumentNullException("appName");

            _appName = appName;
            r_treeview = treeview;
            _hexValues = true;
            _moduleNames = true;
            _lineNumbers = true;
            _offsets = true;
            _tsm = null;


        }

        public String ApplicationName { get { return _appName; } }

        public ThreadStackManager ThreadStackManager { get { return _tsm; } }

        public Boolean HexValue
        {
            get { return _hexValues; }
            set { _hexValues = value; }
        }

        public Boolean ModuleNames
        {
            get { return _moduleNames; }
            set { _moduleNames = value; }
        }

        public Boolean LineNumbers
        {
            get { return _lineNumbers; }
            set { _lineNumbers = value; }
        }

        public Boolean Offsets
        {
            get { return _offsets; }
            set { _offsets = value; }
        }

        internal String GetValue(Int64 value)
        {
            return GetValue((Int32)value);
            //return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(UInt64 value)
        {
            return GetValue((UInt32)value);
            //return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(UInt32 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(Int32 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }

        internal void Refresh(Exception ex)
        {
            r_treeview.Nodes.Clear();
            r_treeview.Nodes.Add(ex.ToString());
            _rootNodeNew = null;
        }

        public void Refresh(ThreadStackManager manager, IDebugProcess process, Dictionary<int, String> threadNames)
        {
            _tsm = manager;
            //_procInfo = process;
            _appName = process.Name;
            _threadNames = threadNames; // seems to hang if we iterate the threads object here


            if (_rootNodeNew == null)
            {
                _rootNodeNew = new RootNode(this, r_treeview);
            }
            else
            {
                _rootNodeNew.Refresh();
            }
        }
    }
}
