using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ThreadDeadLocksNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_rootNode;
        private List<DeadLockNode> r_deadLockNodes;
        private readonly TreeNode r_deadlocksNode;

        internal ThreadDeadLocksNode(ThreadStackNodeManager manager, TreeNode rootNode)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;
            r_deadLockNodes = new List<DeadLockNode>();

            r_deadlocksNode = rootNode.Nodes.Add("ThreadDeadLocks");
            r_deadlocksNode.ImageKey = "arrow_switch.png";
            r_deadlocksNode.SelectedImageKey = r_deadlocksNode.ImageKey;
            r_deadlocksNode.Tag = this;
            Refresh(r_manager.ThreadStackManager.ThreadDeadLocks);
        }

        public void Refresh(List<WaitDeadLock> deadlocks)
        {
            TreeNodeCollection deadLockNodes = r_deadlocksNode.Nodes;
            int countNew = deadlocks.Count;
            int countOld = r_deadLockNodes.Count;

            if (countNew < countOld)
            {
                for (int i = countNew; i < countOld; ++i)
                {
                    r_deadLockNodes[countNew].Remove();
                    r_deadLockNodes.RemoveAt(countNew);
                }
            }
            else if (countOld < countNew)
            {
                for (int i = 0; i < (countNew - countOld); ++i)
                {
                    DeadLockNode deadLockNode = new DeadLockNode(r_manager, r_deadlocksNode, (countOld + i));
                    r_deadLockNodes.Add(deadLockNode);
                }
            }

            for (int i = 0; i < deadlocks.Count; ++i)
            {
                DeadLockNode deadlockNode = r_deadLockNodes[i];
                deadlockNode.Refresh(deadlocks[i]);
            }
        }
    }
}
