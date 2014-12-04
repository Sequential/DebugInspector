using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using ClrInspector;

namespace DebugInspector.Controls.Managed.ThreadStacks
{
    public class ThreadStackNodeManager
    {
        private readonly TreeView r_treeview;
        private String _appName;

        private Boolean _hexValues;
        private Boolean _moduleNames;
        private Boolean _lineNumbers;
        private Boolean _offsets;
        
        private ThreadStackManager _tsm;
        //internal Process2 _procInfo;
        private Dictionary<int, String> _threadNames;

        //private TreeNode _rootNode = null;
        private RootNode _rootNodeNew;

        public ThreadStackNodeManager(TreeView treeview, String appName)
        {
            if (treeview == null)
                throw new ArgumentNullException("treeview");

            if (appName == null)
                throw new ArgumentNullException("appName");

            _appName = appName;
            r_treeview = treeview;
            _hexValues = true;
            _moduleNames = true;
            _lineNumbers = true;
            _offsets = true;
            _tsm = null;

            
        }

        public void Refresh(ThreadStackManager manager, IDebugProcess process, Dictionary<int, String> threadNames)
        {
            _tsm = manager;
            //_procInfo = process;
            _appName = process.Name;
            _threadNames = threadNames; // seems to hang if we iterate the threads object here
            

            if (_rootNodeNew == null)
            {
                _rootNodeNew = new RootNode(this, r_treeview);
            }
            else
            {
                _rootNodeNew.Refresh();
            }
        }

        public void Refresh()
        {
            if (_rootNodeNew == null)
            {
                _rootNodeNew = new RootNode(this, r_treeview);
            }
            else
            {
                _rootNodeNew.Refresh();
            }
        }

        public void EnterNonBreakMode()
        {
            r_treeview.Nodes.Clear();
            r_treeview.Nodes.Add("Cannot view Call Stacks in Run Mode");
            _rootNodeNew = null;
        }

        public String ApplicationName { get { return _appName; } }

        public String GetThreadName(int osThreadId)
        {
            String name;
            _threadNames.TryGetValue(osThreadId, out name);
            if (name == null)
            {
                name = string.Empty;
            }
            return name;
        }

        public String GetOwnedName(UInt64 objectAddress)
        {
            return _tsm.GetOwnedName(objectAddress);
        }

        public ThreadStackManager ThreadStackManager{get{return _tsm;}}

        public Boolean HexValue
        {
            get { return _hexValues; }
            set { _hexValues = value;}
        }

        public Boolean ModuleNames
        {
            get { return _moduleNames; }
            set { _moduleNames = value; }
        }

        public Boolean LineNumbers
        {
            get { return _lineNumbers; }
            set { _lineNumbers = value; }
        }

        public Boolean Offsets
        {
            get { return _offsets; }
            set { _offsets = value; }
        }

        internal String GetValue(Int64 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(UInt64 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(UInt32 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }
        internal String GetValue(Int32 value)
        {
            return _hexValues ? "0x" + value.ToString("X") : value.ToString();
        }

        internal void Refresh(Exception ex)
        {
            r_treeview.Nodes.Clear();
            r_treeview.Nodes.Add(ex.ToString());
            _rootNodeNew = null;
        }

        internal void Refresh(ExpiredException ex)
        {
            r_treeview.Nodes.Clear();
            r_treeview.Nodes.Add(ex.Message);
            _rootNodeNew = null;
        }


        /*
        private void ProcessRoot(String rootName)
        {
            if (_rootNode == null)
            {
                _rootNode = r_treeview.Nodes.Add(rootName);
                _rootNode.ImageKey = "application_cascade.png";
                _rootNode.SelectedImageKey = _rootNode.ImageKey;
            }
            else if(_rootNode.Text != rootName)
            {
                _rootNode.Text = rootName;            
            }
        }

       

        
        private String GetThreadText(ThreadStack threadTasck)
        {
            return "Thread " + GetValue(threadStack.OsThreadId);
        }

        private void ProcessAppDomains(List<ThreadStack> threadStacks)
        { 
            Dictionary<UInt64, TreeNode> appDomainNodesNew = new Dictionary<UInt64, TreeNode>();
            Dictionary<UInt64, TreeNode> appDomainNodesAll = _appDomainNodes;
            Dictionary<UInt64, TreeNode> appDomainNodesRem = new Dictionary<UInt64, TreeNode>(_appDomainNodes);
            _appDomainNodes = appDomainNodesNew;

            Dictionary<UInt64, Dictionary<UInt64, TreeNode>> appDomainThreadsNew = new Dictionary<UInt64, Dictionary<UInt64, TreeNode>>();
            Dictionary<UInt64, Dictionary<UInt64, TreeNode>> appDomainThreadsAll = _appDomainThreadMap;
            Dictionary<UInt64, Dictionary<UInt64, TreeNode>> appDomainThreadsRem = new Dictionary<UInt64, Dictionary<UInt64, TreeNode>>(_appDomainThreadMap);
            _appDomainThreadMap = appDomainThreadsNew;

            foreach (ThreadStack threadStack in threadStacks)
            {
                TreeNode appDomainNode;
                UInt64 id = threadStack.DomainId;

                Dictionary<UInt64, TreeNode> appDomainThreads;

                if (!appDomainNodesAll.ContainsKey(id))
                {
                    String text = GetAppDomainText(threadStack);
                    appDomainNode = _rootNode.Nodes.Add(text);
                    appDomainNode.ImageKey = "application.png";
                    appDomainNode.SelectedImageKey = appDomainNode.ImageKey;
                    appDomainNodesAll.Add(id, appDomainNode);
                    appDomainNodesNew.Add(id, appDomainNode);

                    appDomainThreads = new Dictionary<UInt64, TreeNode>();
                    appDomainThreadsAll.Add(id, appDomainThreads);
                    appDomainThreadsNew.Add(id, appDomainThreads);
                }
                else
                {
                    appDomainNode = appDomainNodesAll[id];
                    appDomainNodesRem.Remove(id);
                    appDomainNodesNew.Add(id, appDomainNode);
                    appDomainThreads = _appDomainThreadMap[id];

                    appDomainThreadsRem.Remove(id);
                    
                }
                ProcessThreadStack(appDomainNode, threadStack, appDomainThreadsNew, appDomainThreadsAll, appDomainThreadsRem);
            }

            foreach (TreeNode tn in appDomainNodesRem.Values)
            {
                _rootNode.Nodes.Remove(tn);
            }
        }

        Dictionary<UInt64, List<TreeNode>> GetThreadMap(UInt64 appDomainId)
        { 
            
        }


        
    

        
        internal void RefreshInPlace(ThreadStackManager manager)
        {
            ThreadStackManager oldValue = _tsm;
            _tsm = manager;

            if (_tsm == null)
            {
                this.r_treeview.Nodes.Clear();
            }
            else
            {
                ProcessRoot("Application x.exe");
                ProcessDeadLocks(_tsm.DeadLocks);
                ProcessAppDomains(_tsm.ThreadStacks);
            }    
        }
         
         

        internal void RefreshOld(ThreadStackManager manager)
        {
            ThreadStackManager oldValue = _tsm;
            _tsm = manager;

            if (_tsm == null)
            {
                this.r_treeview.Nodes.Clear();
            }
            else
            {
                this.r_treeview.Nodes.Clear();
                TreeNode rootNode = r_treeview.Nodes.Add("Application X");
                rootNode.ImageKey = "application_cascade.png";
                rootNode.SelectedImageKey = rootNode.ImageKey;

                TreeNode dlsNode = rootNode.Nodes.Add("DeadLocks");
                dlsNode.ImageKey = "arrow_switch.png";
                dlsNode.SelectedImageKey = dlsNode.ImageKey;
                int i = 1;
                foreach (DeadLock deadlock in manager.DeadLocks)
                {
                    TreeNode deadlockNode = dlsNode.Nodes.Add("DeadLock " + i++);
                    deadlockNode.ImageKey = "arrow_refresh.png";
                    deadlockNode.SelectedImageKey = deadlockNode.ImageKey;
                    foreach (DeadLockItem item in deadlock.Items)
                    {
                        TreeNode dliNode = deadlockNode.Nodes.Add("Thread (" + item.Stack.OsThreadId + ") owns " + GetValue(item.ObjectOwned) + " waits " + GetValue(item.ObjectWaitingOn));
                        dliNode.ImageKey = "arrow_branch.png";
                        dliNode.SelectedImageKey = dliNode.ImageKey;
                    }
                }

                Dictionary<UInt64, TreeNode> appDomainNodes = new Dictionary<UInt64, TreeNode>();

                foreach (ThreadStack threadStack in manager.ThreadStacks)
                {
                    TreeNode appDomainNode;
                    if (!appDomainNodes.ContainsKey(threadStack.DomainId))
                    {
                        appDomainNode = rootNode.Nodes.Add("AppDomain (" + GetValue(threadStack.DomainId) + ") " + threadStack.DomainName);
                        appDomainNodes.Add(threadStack.DomainId, appDomainNode);
                    }
                    else
                    {
                        appDomainNode = appDomainNodes[threadStack.DomainId];
                    }
                    appDomainNode.ImageKey = "application.png";
                    appDomainNode.SelectedImageKey = appDomainNode.ImageKey;

                    TreeNode stackNode = appDomainNode.Nodes.Add("Thread:" + threadStack.OsThreadId);
                    stackNode.ImageKey = "cog.png";
                    stackNode.SelectedImageKey = stackNode.ImageKey;
                    if (threadStack.SyncBlockObjectWaitedOn > 0)
                    {
                        TreeNode monWaitingNode = stackNode.Nodes.Add("Waiting On (" + GetValue(threadStack.SyncBlockObjectWaitedOn) + ") " + threadStack.SyncBlockObjectWaitedOnName);
                        monWaitingNode.ImageKey = "hourglass.png";
                        monWaitingNode.SelectedImageKey = monWaitingNode.ImageKey;
                    }

                    foreach (UInt64 mon in threadStack.SyncBlockObjectsOwned)
                    {
                        TreeNode monOwnedNode = stackNode.Nodes.Add("Owns (" + GetValue(mon) + ")");
                        monOwnedNode.ImageKey = "key.png";
                        monOwnedNode.SelectedImageKey = monOwnedNode.ImageKey;
                    }
                    foreach (ThreadStackFrame frame in threadStack.Frames)
                    {
                        StringBuilder name = new StringBuilder();
                        if (frame.FrameName.Length > 0)
                        {
                            name.Append("[" + frame.FrameName + "] ");
                        }
                        String moduleName = frame.ModuleName;
                        if (moduleName.Length > 0)
                        {
                            name.Append(moduleName);
                            name.Append("!");
                        }
                        name.Append(frame.MethodName);
                        UInt32 line = frame.Line;
                        if (line > 0)
                        {
                            name.Append("- line ");
                            name.Append(frame.Line);
                        }

                        TreeNode frameNode = stackNode.Nodes.Add(name.ToString());
                        frameNode.ImageKey = "text_list_bullets.png";
                        frameNode.SelectedImageKey = frameNode.ImageKey;
                        if (!frame.Managed)
                        {
                            frameNode.ForeColor = Color.Gray;
                        }
                    }
                }
            }
        }
        * */
    }
         
}
