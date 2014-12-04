using System;
using System.Collections.Generic;
using System.Text;
using ClrInspector;
using System.Windows.Forms;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class AppDomainNode
    {
        private readonly TreeNode r_rootNode;
        private readonly TreeNode r_appDomainNode;
        private readonly ThreadStackNodeManager r_manager;

        private readonly Dictionary<UInt32, ThreadStackNode> r_threadStackNodes;

        private Dictionary<UInt64, ThreadStackNode> _threadStacks = new Dictionary<ulong,ThreadStackNode>();

        internal AppDomainNode(ThreadStackNodeManager manager, TreeNode rootNode, Dictionary<UInt32, ThreadStack> threadStacks)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;

            String text = GetText(threadStacks);
            r_appDomainNode = r_rootNode.Nodes.Add(text);
            r_appDomainNode.ImageKey = "application.png";
            r_appDomainNode.SelectedImageKey = r_appDomainNode.ImageKey;
            r_appDomainNode.Tag = this;
            
            r_threadStackNodes = new Dictionary<uint, ThreadStackNode>();
            RefreshChildren(threadStacks);

            r_appDomainNode.Expand();
        }

        private String GetText(Dictionary<UInt32, ThreadStack> threadStacks)
        {
            Dictionary<UInt32, ThreadStack>.Enumerator enumer = threadStacks.GetEnumerator();
            if(enumer.MoveNext())
            {
                ThreadStack threadStack = enumer.Current.Value;
                return "AppDomain (" + r_manager.GetValue(threadStack.DomainId) + ") " + threadStack.DomainName;
            }
            return r_appDomainNode.Text;
        }

        internal void Remove()
        {
            r_appDomainNode.Remove();
        }

        public void Refresh(Dictionary<UInt32, ThreadStack> threadStacks)
        {
            RefreshMe(threadStacks);
            RefreshChildren(threadStacks);
        }

        private void RefreshMe(Dictionary<UInt32, ThreadStack> threadStacks)
        {
            String text = GetText(threadStacks);
            r_appDomainNode.Text = text;
        }

        private void RefreshChildren(Dictionary<UInt32, ThreadStack> threadStacks)
        {
            List<UInt32> threadsNoLonger = new List<UInt32>();

            foreach (KeyValuePair<UInt32, ThreadStackNode> pair in r_threadStackNodes)
            {
                UInt32 oldId = pair.Key;
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

            foreach (KeyValuePair<UInt32, ThreadStack> pair in threadStacks)
            {
                UInt32 newId = pair.Key;
                ThreadStackNode threadStackNode;
                if (!r_threadStackNodes.TryGetValue(newId, out threadStackNode))
                {
                    threadStackNode = new ThreadStackNode(r_manager, r_appDomainNode, pair.Value);
                    r_threadStackNodes.Add(newId, threadStackNode);
                }
            }
        }
    }
}
