using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class UnownedObjectNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_unownedObjectsNode;
        private readonly TreeNode r_unownedObjectNode;

        internal UnownedObjectNode(ThreadStackNodeManager manager, TreeNode unownedObjectsNode, WaitObject unownedObject)
        {
            if (unownedObjectsNode == null)
                throw new ArgumentNullException("unownedObjectsNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            if (unownedObject == null)
                throw new ArgumentNullException("unownedObject");

            r_manager = manager;
            r_unownedObjectsNode = unownedObjectsNode;

            String text = GetUnownedObjectText(unownedObject);
            r_unownedObjectNode = r_unownedObjectsNode.Nodes.Add(text);
            r_unownedObjectNode.ImageKey = "arrow_branch.png";
            r_unownedObjectNode.SelectedImageKey = r_unownedObjectNode.ImageKey;
            r_unownedObjectNode.Tag = this;
        }

        public void Remove()
        {
            r_unownedObjectNode.Remove();
        }

        public void Refresh(WaitObject unownedObject)
        {
            String text = GetUnownedObjectText(unownedObject);
            r_unownedObjectNode.Text = text;
        }

        private String GetUnownedObjectText(WaitObject unownedObject)
        {
            return " "          + r_manager.GetValue(unownedObject.Address) + 
                   " Type: "    + r_manager.GetValue(unownedObject.ObjectType) + 
                   " Handle: "  + r_manager.GetValue(unownedObject.Handle) + 
                   " Name: "    + unownedObject.Name;
        }
    }
}
