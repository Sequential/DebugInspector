using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class AppDomainsNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_rootNode;

        private readonly Dictionary<UInt64, AppDomainNode> r_appDomainNodes;

        internal AppDomainsNode(ThreadStackNodeManager manager, TreeNode rootNode)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_rootNode = rootNode;
            r_appDomainNodes = new Dictionary<UInt64, AppDomainNode>();

            Initialize(manager.ThreadStackManager.ThreadStacks);
        }

        private void Initialize(List<ThreadStack> threadStacks)
        {
            Refresh(threadStacks);
        }

        public void Refresh(List<ThreadStack> threadStacks)
        {
            Dictionary<UInt64, Dictionary<UInt32, ThreadStack>> segregated = new Dictionary<UInt64, Dictionary<UInt32, ThreadStack>>();
            for (int i = 0; i < threadStacks.Count; ++i)
            {
                ThreadStack tStack = threadStacks[i];
                Dictionary<UInt32, ThreadStack> tStacks;
                UInt64 did = tStack.DomainId;
                if (!(segregated.TryGetValue(did, out tStacks)))
                {
                    tStacks = new Dictionary<UInt32, ThreadStack>();
                    segregated.Add(did, tStacks);
                }
                tStacks.Add(tStack.OsThreadId, tStack);
            }

            List<UInt64> appDomainsNoLonger = new List<UInt64>();

            foreach (KeyValuePair<UInt64, AppDomainNode> pair in r_appDomainNodes)
            {
                UInt64 oldId = pair.Key;
                AppDomainNode oldDomain = pair.Value;
                Dictionary<UInt32, ThreadStack> tStacks;
                if (segregated.TryGetValue(oldId, out tStacks))
                {
                    oldDomain.Refresh(tStacks);
                }
                else
                {
                    oldDomain.Remove();
                    appDomainsNoLonger.Add(oldId);
                }
            }

            foreach (UInt64 key in appDomainsNoLonger)
            {
                r_appDomainNodes.Remove(key);
            }

            foreach (KeyValuePair<UInt64, Dictionary<UInt32, ThreadStack>> pair in segregated)
            { 
                UInt64 newId = pair.Key;
                AppDomainNode appDomainNode;
                if (!r_appDomainNodes.TryGetValue(newId, out appDomainNode))
                {
                    appDomainNode = new AppDomainNode(r_manager, r_rootNode, pair.Value);
                    r_appDomainNodes.Add(newId, appDomainNode);
                }
            }
        }
    }
}
