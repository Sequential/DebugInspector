using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class UnownedObjectsNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_rootNode;
        private List<UnownedObjectNode> r_objectNodes;
        private readonly TreeNode r_unownedObjectsNode;

        internal UnownedObjectsNode(ThreadStackNodeManager manager, TreeNode rootNode)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;
            r_objectNodes = new List<UnownedObjectNode>();

            r_unownedObjectsNode = rootNode.Nodes.Add("Unowned Objects");
            r_unownedObjectsNode.ImageKey = "arrow_switch.png";
            r_unownedObjectsNode.SelectedImageKey = r_unownedObjectsNode.ImageKey;
            r_unownedObjectsNode.Tag = this;

            Refresh(r_manager.ThreadStackManager.UnownedObjects);
        }

        public void Refresh(List<WaitObject> unownedObjects)
        {
            int countNew = unownedObjects.Count;
            int countOld = r_unownedObjectsNode.Nodes.Count;

            if (countNew < countOld)
            {
                for (int i = countNew; i < countOld; ++i)
                {
                    r_unownedObjectsNode.Nodes[countNew].Remove();
                }
            }
            else if (countOld < countNew)
            {
                for (int i = 0; i < (countNew - countOld); ++i)
                {

                    UnownedObjectNode unownedNode = new UnownedObjectNode(r_manager, r_unownedObjectsNode, unownedObjects[countOld + i]);
                    r_objectNodes.Add(unownedNode);
                }
            }

            for (int i = 0; i < unownedObjects.Count; ++i)
            {
                UnownedObjectNode unownedNode = r_objectNodes[i];
                unownedNode.Refresh(unownedObjects[i]);
            }
        }
    }
}
