using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ThreadStackWaitedOnNode
    {
        private readonly TreeNode r_threadStackNode;
        private readonly ThreadStackNodeManager r_manager;
        private TreeNode r_waitedOnNode;

        internal ThreadStackWaitedOnNode(ThreadStackNodeManager manager, TreeNode threadStackNode, int index, WaitBlock waitblock)
        {
            if (threadStackNode == null)
                throw new ArgumentNullException("r_threadStackNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_threadStackNode = threadStackNode;

            r_waitedOnNode = r_threadStackNode.Nodes.Insert(index, GetText(waitblock));
            r_waitedOnNode.ImageKey = "hourglass.png";
            r_waitedOnNode.SelectedImageKey = r_waitedOnNode.ImageKey;
            r_waitedOnNode.Tag = this;
        }

        private String GetText(WaitBlock waitblock)
        {
            return "Waiting (" + waitblock.ObjectWaitType.ToString() + 
                   ") on " + r_manager.GetValue(waitblock.ObjectAddress) + 
                   " with Key " + waitblock.WaitKey;
        }

        internal void Remove()
        {
            r_waitedOnNode.Remove();
        }

        internal void Refresh(WaitBlock waitblock)
        {
            r_waitedOnNode.Text = GetText(waitblock);
        }
    }
}
