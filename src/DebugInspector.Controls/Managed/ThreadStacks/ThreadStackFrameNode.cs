using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    public class ThreadStackFrameNode : INavItem
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_threadStackNode;
        private readonly TreeNode r_frameNode;

        private String _fileName;
        
        private int _line;
        private int _eip;

        private String _managedMethod;
        private int _offset;

        private bool _managed;

        internal ThreadStackFrameNode(ThreadStackNodeManager manager, TreeNode threadStackNode, int index, ThreadStackFrame threadStackframe)
        {
            if (threadStackNode == null)
                throw new ArgumentNullException("threadStackNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            if (threadStackframe == null)
                throw new ArgumentNullException("threadStackframe");

            r_manager = manager;
            r_threadStackNode = threadStackNode;

            String text = GetText(threadStackframe);

            r_frameNode = r_threadStackNode.Nodes.Insert(index, text);
            r_frameNode.ImageKey = "text_list_bullets.png";
            r_frameNode.SelectedImageKey = r_frameNode.ImageKey;
            r_frameNode.Tag = this;
            if (!threadStackframe.Managed)
            {
                r_frameNode.ForeColor = Color.Gray;
            }
            GetInfo(threadStackframe);
        }

        internal void Remove()
        {
            r_frameNode.Remove();
        }

        public void Refresh(ThreadStackFrame threadStackframe)
        {
            String text = GetText(threadStackframe);
            r_frameNode.Text = text;
            r_frameNode.ForeColor = threadStackframe.Managed? Color.Black:Color.Gray;
            GetInfo(threadStackframe);
        }

        private void GetInfo(ThreadStackFrame frame)
        {
            String fn = frame.FileName;
            if (frame.Line > 0 && fn != null && fn.Length > 0)
            {
                _line = (int)frame.Line;
                _fileName = frame.FileName;
            }
            else
            {
                _line = 0;
                _fileName = null;
            }
            _eip = (int)frame.Eip;
            _managedMethod = frame.MethodName;
            _offset = (int)frame.Offset;
            _managed = frame.Managed;
        }

        private String GetText(ThreadStackFrame frame)
        {
            StringBuilder name = new StringBuilder();
            if (frame.FrameName.Length > 0)
            {
                name.Append("[" + frame.FrameName + "] ");
            }
            String moduleName = frame.ModuleName;
            if (r_manager.ModuleNames && moduleName.Length > 0)
            {
                name.Append(moduleName);
                name.Append("!");
            }
            name.Append(frame.MethodName);
            UInt32 line = frame.Line;
            if (r_manager.LineNumbers && line > 0)
            {
                name.Append("- line ");
                name.Append(frame.Line);
            }
            else if (r_manager.Offsets)
            {
                if (frame.Managed)
                {
                    name.Append(" + ");
                    name.Append(r_manager.GetValue(frame.Offset));
                    name.Append(" bytes");
                }
                else
                {
                    name.Append(" ");
                    name.Append(r_manager.GetValue(frame.Eip));
                    name.Append(" EIP");
                }
            }
            return name.ToString();
        }

        #region INavItem Members

        public void NavigateTo(IEnvironment dte)
        {
            if(_line>0)
            {
                dte.GoToLine(_fileName, _line);
            }
            else if (_eip > 0 && _managed)
            {
                dte.GoToAddress(_managedMethod, _offset);
            }
        }

        #endregion
    }
}
