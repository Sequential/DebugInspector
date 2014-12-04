using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class ThreadStackWaitedOnNode
    {
        private readonly TreeNode r_threadStackNode;
        private readonly ThreadStackNodeManager r_manager;
        private TreeNode _waitedOnNode;

        internal ThreadStackWaitedOnNode(ThreadStackNodeManager manager, TreeNode threadStackNode)
        {
            if(threadStackNode==null)
                throw new ArgumentNullException("r_threadStackNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_threadStackNode = threadStackNode;
        }

        internal void Refresh(ThreadStack threadStack)
        {
            if (threadStack.SyncBlockObjectWaitedOn > 0)
            {
                if (_waitedOnNode == null)
                {
                    _waitedOnNode = r_threadStackNode.Nodes.Insert(0, GetText(threadStack));
                    _waitedOnNode.ImageKey = "hourglass.png";
                    _waitedOnNode.SelectedImageKey = _waitedOnNode.ImageKey;
                    _waitedOnNode.Tag = this;
                }
                else
                {
                    _waitedOnNode.Text = GetText(threadStack);
                }
            }
            else if (_waitedOnNode != null)
            {
                _waitedOnNode.Tag = null;
                _waitedOnNode.Remove();
                _waitedOnNode = null;
            }
        }

        internal int Ordinal { get { return _waitedOnNode == null ? 0 : 1; } }

        private String GetText(ThreadStack threadStack)
        {
            return "Waiting On (" + r_manager.GetValue(threadStack.SyncBlockObjectWaitedOn) + ") " + threadStack.SyncBlockObjectWaitedOnName;
        }
    }
}
