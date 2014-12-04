using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class ThreadStackOwnedNode
    {
        private readonly TreeNode r_threadStackNode;
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_ownedNode;

        internal ThreadStackOwnedNode(ThreadStackNodeManager manager, TreeNode threadStackNode, int index, UInt64 objectAddress)
        {
            if(threadStackNode==null)
                throw new ArgumentNullException("r_threadStackNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_threadStackNode = threadStackNode;

            r_ownedNode = r_threadStackNode.Nodes.Insert(index, GetText(objectAddress));
            r_ownedNode.ImageKey = "key.png";
            r_ownedNode.SelectedImageKey = r_ownedNode.ImageKey;
            r_ownedNode.Tag = this;

            
        }

        private String GetText(UInt64 objectAddress)
        {
            return "Owns (" + r_manager.GetValue(objectAddress) + ") " + r_manager.GetOwnedName(objectAddress);
        }

        internal void Remove()
        {
            r_ownedNode.Remove();
        }

        internal void Refresh(UInt64 objectAddress)
        {
            r_ownedNode.Text = GetText(objectAddress);
        }
    }
}
