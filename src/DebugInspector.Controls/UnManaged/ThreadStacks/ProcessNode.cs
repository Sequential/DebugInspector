using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ProcessNode
    {
        private readonly TreeNode r_rootNode;
        private readonly TreeNode r_processNode;
        private readonly ThreadStackNodeManager r_manager;

        private readonly Dictionary<Int64, ThreadStackNode> r_threadStackNodes;

        private Dictionary<UInt64, ThreadStackNode> _threadStacks = new Dictionary<ulong, ThreadStackNode>();

        internal ProcessNode(ThreadStackNodeManager manager, TreeNode rootNode, Dictionary<Int64, ThreadStack> threadStacks)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;

            String text = GetText();
            r_processNode = r_rootNode.Nodes.Add(text);
            r_processNode.ImageKey = "application.png";
            r_processNode.SelectedImageKey = r_processNode.ImageKey;
            r_processNode.Tag = this;

            r_threadStackNodes = new Dictionary<Int64, ThreadStackNode>();
            RefreshChildren(threadStacks);

            r_processNode.Expand();
        }

        private String GetText()
        {
            return "Process " + r_manager.GetValue((int)r_manager.ThreadStackManager.ProcessId) +
                   "(" + r_manager.GetValue(r_manager.ThreadStackManager.KProcess) + ")"; 
        }

        internal void Remove()
        {
            r_processNode.Remove();
        }

        public void Refresh(Dictionary<Int64, ThreadStack> threadStacks)
        {
            RefreshMe(threadStacks);
            RefreshChildren(threadStacks);
        }

        private void RefreshMe(Dictionary<Int64, ThreadStack> threadStacks)
        {
            String text = GetText();
            r_processNode.Text = text;
        }

        private void RefreshChildren(Dictionary<Int64, ThreadStack> threadStacks)
        {
            List<Int64> threadsNoLonger = new List<Int64>();

            foreach (KeyValuePair<Int64, ThreadStackNode> pair in r_threadStackNodes)
            {
                Int64 oldId = pair.Key;
                ThreadStackNode oldThreadStack = pair.Value;
                ThreadStack tStack;
                if (threadStacks.TryGetValue(oldId, out tStack))
                {
                    oldThreadStack.Refresh(tStack);
                }
                else
                {
                    oldThreadStack.Remove();
                    threadsNoLonger.Add(oldId);
                }
            }

            foreach (UInt32 threadId in threadsNoLonger)
            {
                r_threadStackNodes.Remove(threadId);
            }

            foreach (KeyValuePair<Int64, ThreadStack> pair in threadStacks)
            {
                Int64 newId = pair.Key;
                ThreadStackNode threadStackNode;
                if (!r_threadStackNodes.TryGetValue(newId, out threadStackNode))
                {
                    threadStackNode = new ThreadStackNode(r_manager, r_processNode, pair.Value);
                    r_threadStackNodes.Add(newId, threadStackNode);
                }
            }
        }
    }
}
