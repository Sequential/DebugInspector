using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    internal class ThreadStackNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_appDomainNode;
        private readonly TreeNode r_threadStackNode;

        private readonly ThreadStackWaitedOnNode r_waitedOnNode;

        private readonly List<ThreadStackOwnedNode> r_ownedNodes = new List<ThreadStackOwnedNode>();
        private readonly List<ThreadStackFrameNode> r_frameNodes = new List<ThreadStackFrameNode>();

        internal ThreadStackNode(ThreadStackNodeManager manager, TreeNode appDomainNode, ThreadStack threadStack)
        {
            if (appDomainNode == null)
                throw new ArgumentNullException("appDomainNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_appDomainNode = appDomainNode;

            String text = GetText(threadStack);
            r_threadStackNode = appDomainNode.Nodes.Add(text);
            r_threadStackNode.ImageKey = "cog.png";
            r_threadStackNode.SelectedImageKey = r_threadStackNode.ImageKey;
            r_threadStackNode.Tag = this;
            r_waitedOnNode = new ThreadStackWaitedOnNode(r_manager, r_threadStackNode);
            r_ownedNodes = new List<ThreadStackOwnedNode>();
            r_frameNodes = new List<ThreadStackFrameNode>();

            RefreshChildren(threadStack);
        }

        private String GetText(ThreadStack threadStack)
        {
            uint osid = threadStack.OsThreadId;
            return "Thread: " + r_manager.GetValue(osid) + " " + r_manager.GetThreadName((int)osid);
        }

        internal void Remove()
        {
            r_threadStackNode.Remove();
        }

        private void RefreshMe(ThreadStack threadStack)
        {
            r_threadStackNode.Text = GetText(threadStack);
        }

        internal void Refresh(ThreadStack threadStack)
        {
            RefreshMe(threadStack);
            RefreshChildren(threadStack);
        }

        private void RefreshChildren(ThreadStack threadStack)
        {
            r_waitedOnNode.Refresh(threadStack);

            RefreshOwnedObjects(threadStack);

            RefreshFrames(threadStack.Frames);

        }

        private void RefreshOwnedObjects(ThreadStack threadStack)
        {
            List<UInt64> newOwnedNodes = threadStack.SyncBlockObjectsOwned;

            int indexDelta = r_waitedOnNode.Ordinal;
            int countOld = r_ownedNodes.Count;
            int countNew = newOwnedNodes.Count;
            int to = countNew;

            if (countNew < countOld)
            {
                for (int i = countNew; i < countOld; ++i)
                {
                    r_ownedNodes[countNew].Remove();
                    r_ownedNodes.RemoveAt(countNew);
                }
            }
            else if (countOld < countNew)
            {
                to = countOld;
                int index = countOld;
            
                for (int i = 0; i < (countNew - countOld); ++i)
                {
                    UInt64 objectAddress = newOwnedNodes[index];
                    ThreadStackOwnedNode ownedNode = new ThreadStackOwnedNode(r_manager, r_threadStackNode, index+indexDelta, objectAddress);
                    r_ownedNodes.Add(ownedNode);
                    ++index;
                }
            }

            for (int i = 0; i < to; ++i)
            {
                r_ownedNodes[i].Refresh(newOwnedNodes[i]);
            }
        }

        private void RefreshFrames(List<ThreadStackFrame> frames)
        {
            int indexDelta = r_waitedOnNode.Ordinal + r_ownedNodes.Count;
            int countOld = r_frameNodes.Count;
            int countNew = frames.Count;
            int to = countNew;

            if (countNew < countOld)
            {
                for (int i = countNew; i < countOld; ++i)
                {
                    r_frameNodes[countNew].Remove();
                    r_frameNodes.RemoveAt(countNew);
                }
            }
            else if (countOld < countNew)
            {
                to = countOld;
                int index = countOld;
            
                for (int i = 0; i < (countNew - countOld); ++i)
                {
                    ThreadStackFrame frame = frames[index];
                    ThreadStackFrameNode frameNode = new ThreadStackFrameNode(r_manager, r_threadStackNode, index+indexDelta, frame);
                    r_frameNodes.Add(frameNode);
                    ++index;
                }
            }

            for (int i = 0; i < to; ++i)
            {
                r_frameNodes[i].Refresh(frames[i]);
            }
        }
    }
}
