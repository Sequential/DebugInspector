using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class DeadLockItemNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_deadlockNode;
        private readonly TreeNode r_deadlockItemNode;

        internal DeadLockItemNode(ThreadStackNodeManager manager, TreeNode deadlockNode, WaitDeadLockItem deadlockItem)
        {
            if (deadlockNode == null)
                throw new ArgumentNullException("deadlockNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            if (deadlockItem == null)
                throw new ArgumentNullException("deadlockItem");

            r_manager = manager;
            r_deadlockNode = deadlockNode;

            String text = GetDeadLockItemText(deadlockItem);
            r_deadlockItemNode = r_deadlockNode.Nodes.Add(text);
            r_deadlockItemNode.ImageKey = "arrow_branch.png";
            r_deadlockItemNode.SelectedImageKey = r_deadlockItemNode.ImageKey;
            r_deadlockItemNode.Tag = this;
        }

        public void Remove()
        {
            r_deadlockNode.Remove();
        }

        public void Refresh(WaitDeadLockItem deadlockItem)
        {
            String text = GetDeadLockItemText(deadlockItem);
            r_deadlockItemNode.Text = text;
        }

        private String GetDeadLockItemText(WaitDeadLockItem item)
        {
            return "Thread (" + r_manager.GetValue(item.WaitingThread.OsThreadId) + ") waits on " + r_manager.GetValue((ulong)item.CurrentObject.Address) + " owned by " + r_manager.GetValue(item.OwningThread.OsThreadId);
        }
    }
}
