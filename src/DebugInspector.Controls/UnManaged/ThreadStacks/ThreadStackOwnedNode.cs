using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ThreadStackOwnedNode
    {
        private readonly TreeNode r_threadStackNode;
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_ownedNode;

        internal ThreadStackOwnedNode(ThreadStackNodeManager manager, TreeNode threadStackNode, int index, WaitObject waitObject)
        {
            if (threadStackNode == null)
                throw new ArgumentNullException("r_threadStackNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_threadStackNode = threadStackNode;

            r_ownedNode = r_threadStackNode.Nodes.Insert(index, GetText(waitObject));
            r_ownedNode.ImageKey = "key.png";
            r_ownedNode.SelectedImageKey = r_ownedNode.ImageKey;
            r_ownedNode.Tag = this;
        }

        private String GetText(WaitObject waitObject)
        {
            return " " + r_manager.GetValue(waitObject.Address) +
                   " Type: " + ((WaitObjectType)waitObject.ObjectType).ToString() +
                   " Handle: " + r_manager.GetValue(waitObject.Handle) +
                   " Name: " + waitObject.Name;
        }

        internal void Remove()
        {
            r_ownedNode.Remove();
        }

        internal void Refresh(WaitObject waitObject)
        {
            r_ownedNode.Text = GetText(waitObject);
        }
    }
}
