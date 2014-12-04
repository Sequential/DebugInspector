using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using UnmanagedInspector;

namespace DebugInspector.Controls.UnManaged.ThreadStacks
{
    internal class ThreadStackNode
    {
        private readonly ThreadStackNodeManager r_manager;
        private readonly TreeNode r_processNode;
        private readonly TreeNode r_threadStackNode;

        private readonly List<ThreadStackOwnedNode> r_ownedNodes  = new List<ThreadStackOwnedNode>();
        private readonly List<ThreadStackWaitedOnNode> r_waitedNodes = new List<ThreadStackWaitedOnNode>();
        private readonly List<ThreadStackFrameNode> r_frameNodes   = new List<ThreadStackFrameNode>();

        internal ThreadStackNode(ThreadStackNodeManager manager, TreeNode processNode, ThreadStack threadStack)
        {
            if (processNode == null)
                throw new ArgumentNullException("processNode");

            if (manager == null)
                throw new ArgumentNullException("manager");

            r_manager = manager;
            r_processNode = processNode;

            String text = GetText(threadStack);
            r_threadStackNode = processNode.Nodes.Add(text);
            r_threadStackNode.ImageKey = "cog.png";
            r_threadStackNode.SelectedImageKey = r_threadStackNode.ImageKey;
            r_threadStackNode.Tag = this;
            r_waitedNodes = new List<ThreadStackWaitedOnNode>();
            r_ownedNodes  = new List<ThreadStackOwnedNode>();
            r_frameNodes  = new List<ThreadStackFrameNode>();

            RefreshChildren(threadStack);
        }

        private String GetText(ThreadStack threadStack)
        {
            Int32 osid = threadStack.OsThreadId;
            Int64 ktid = threadStack.KTrhead;

            return "Thread: " + r_manager.GetValue(osid) + " (" + r_manager.GetValue(ktid) + ")";
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
            RefreshWaitedObjects(threadStack);

            RefreshOwnedObjects(threadStack);

            RefreshFrames(threadStack.StackFrames);
        }

        private void RefreshWaitedObjects(ThreadStack threadStack)
        {
            List<WaitBlock> newWaitedNodes = new List<WaitBlock>(threadStack.WaitBlocks.Values);

            int countOld = r_waitedNodes.Count;
            int countNew = newWaitedNodes.Count;
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
                    WaitBlock objectAddress = newWaitedNodes[index];
                    ThreadStackWaitedOnNode waitedOnNode = new ThreadStackWaitedOnNode(r_manager, r_threadStackNode, index, objectAddress);
                    r_waitedNodes.Add(waitedOnNode);
                    ++index;
                }
            }

            for (int i = 0; i < to; ++i)
            {
                r_waitedNodes[i].Refresh(newWaitedNodes[i]);
            }
        }

        private void RefreshOwnedObjects(ThreadStack threadStack)
        {
            List<WaitObject> newOwnedNodes = new List<WaitObject>(threadStack.OwnedObjects.Values);

            int indexDelta = r_waitedNodes.Count;
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
                    WaitObject objectAddress = newOwnedNodes[index];
                    ThreadStackOwnedNode ownedNode = new ThreadStackOwnedNode(r_manager, r_threadStackNode, index + indexDelta, objectAddress);
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
            int indexDelta = r_waitedNodes.Count + r_ownedNodes.Count;
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
                    ThreadStackFrameNode frameNode = new ThreadStackFrameNode(r_manager, r_threadStackNode, index + indexDelta, frame);
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
