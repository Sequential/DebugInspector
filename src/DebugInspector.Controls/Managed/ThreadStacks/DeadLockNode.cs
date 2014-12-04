using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    public class DeadLockNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_deadlocksNode;
        private List<DeadLockItemNode> r_deadLockItemNodes;
        private readonly TreeNode r_deadlockNode;

        internal DeadLockNode(ThreadStackNodeManager manager, TreeNode deadlocksNode, int index)
        {
            if (deadlocksNode == null)
                throw new ArgumentNullException("deadlocksNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_deadlocksNode = deadlocksNode;
            r_deadLockItemNodes = new List<DeadLockItemNode>();

            r_deadlockNode = r_deadlocksNode.Nodes.Add("DeadLock " + index);
            r_deadlockNode.ImageKey = "arrow_refresh.png";
            r_deadlockNode.SelectedImageKey = r_deadlockNode.ImageKey;
            r_deadlockNode.Tag = this;
        }

        public void Remove()
        {
            r_deadlockNode.Remove();
        }

        public void Refresh(DeadLock deadlock)
        {
            List<DeadLockItem> deadLockItems = deadlock.Items;
            TreeNodeCollection deadLockItemNodes = r_deadlockNode.Nodes;

            int countNew = deadLockItems.Count;
            int countOld = deadLockItemNodes.Count;
            int to = countNew;

            if (countNew < countOld)
            {
                for (int i = countNew; i < countOld; ++i)
                {
                    r_deadLockItemNodes[countNew].Remove();
                    r_deadLockItemNodes.RemoveAt(countNew);
                }
            }
            else if (countOld < countNew)
            {
                to = countOld;
                for (int i = 0; i < (countNew - countOld); ++i)
                {
                    DeadLockItemNode deadLockItemNode = new DeadLockItemNode(r_manager, r_deadlockNode, deadLockItems[countOld + i]);
                    r_deadLockItemNodes.Add(deadLockItemNode);
                } 
            }

            for (int i = 0; i < to; ++i)
            {
                r_deadLockItemNodes[i].Refresh(deadLockItems[i]);
            }
        }
    }
}
