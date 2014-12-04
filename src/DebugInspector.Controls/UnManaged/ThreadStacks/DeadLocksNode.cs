using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class DeadLocksNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_rootNode;
        private readonly TreeNode r_deadlocksNode;
        private readonly ThreadDeadLocksNode r_threadDeadlocksNode;
        private readonly WaitDeadLocksNode   r_waitDeadlocksNode;
        private readonly SuspectDeadLocksNode r_suspectDeadlocksNode;

        internal DeadLocksNode(ThreadStackNodeManager manager, TreeNode rootNode)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;
            r_deadlocksNode     = rootNode.Nodes.Add("DeadLocks");
            r_deadlocksNode.ImageKey = "arrow_switch.png";
            r_deadlocksNode.SelectedImageKey = r_deadlocksNode.ImageKey;
            r_deadlocksNode.Tag = this;



            r_threadDeadlocksNode = new ThreadDeadLocksNode(r_manager, r_deadlocksNode);
            r_waitDeadlocksNode   = new WaitDeadLocksNode(r_manager, r_deadlocksNode);
            r_suspectDeadlocksNode = new SuspectDeadLocksNode(r_manager, r_deadlocksNode);
           
        }

        public void Refresh()
        {
            r_threadDeadlocksNode.Refresh(r_manager.ThreadStackManager.ThreadDeadLocks);
            r_waitDeadlocksNode.Refresh(r_manager.ThreadStackManager.WaitDeadLocks);
            r_suspectDeadlocksNode.Refresh(r_manager.ThreadStackManager.SuspectWaitBlockChains);
        }
    }
}
