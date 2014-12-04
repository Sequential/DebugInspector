using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class RootNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeView r_root;
        private readonly TreeNode r_rootNode;
        private readonly DeadLocksNode r_deadLocksNode;
        private readonly AppDomainsNode r_appDomainsNode;

        internal RootNode(ThreadStackNodeManager manager, TreeView root)
        {
            if (manager == null)
                throw new ArgumentNullException("manager");

            if (root == null)
                throw new ArgumentNullException("root");

            r_manager = manager;
            r_root = root;

            r_root.Nodes.Clear();
            r_rootNode = r_root.Nodes.Add(r_manager.ApplicationName);
            r_rootNode.ImageKey = "application_cascade.png";
            r_rootNode.SelectedImageKey = r_rootNode.ImageKey;
            r_rootNode.Tag = this;
            
            r_deadLocksNode = new DeadLocksNode(r_manager, r_rootNode);
            r_appDomainsNode = new AppDomainsNode(r_manager, r_rootNode);

            r_rootNode.Expand();
        }

        public void Refresh()
        {
            r_rootNode.Text = r_manager.ApplicationName;
            ThreadStackManager tsm = r_manager.ThreadStackManager;
            r_deadLocksNode.Refresh(tsm.DeadLocks);
            r_appDomainsNode.Refresh(tsm.ThreadStacks);
        }
    }

}
