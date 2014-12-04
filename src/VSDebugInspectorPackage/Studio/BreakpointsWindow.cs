using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using EnvDTE;

namespace DebugInspector.Studio
{
    class BreakpointsWindow
    {
        private const int GW_OWNER = 4;
        private const int GWL_EXSTYLE = -20;
        private const long WS_EX_TOOLWINDOW = 0x00000080L;

        [DllImport("user32")]
        private static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        [DllImport("user32")]
        private static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

        [DllImport("user32")]
        private static extern UInt32 GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32")]
        private static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        [DllImport("user32")]
        private static extern bool EnumChildWindows(IntPtr hwndParent, EnumWindowsProc lpEnumFunc, IntPtr lParam);

        private delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
        private static readonly int sr_procId = System.Diagnostics.Process.GetCurrentProcess().Id;

        private static IntPtr s_hwndMainWindow = IntPtr.Zero;
        private static IntPtr s_hwndBreakpointsWindow = IntPtr.Zero;
        private static IntPtr s_hwndBreakpointsTreeView = IntPtr.Zero;
        private static TreeView s_treeview;

        private static void Initialize(DTE dte)
        {
            if (s_hwndMainWindow == IntPtr.Zero)
            {
                dte.Windows.Item(GuidBreakpoints).Visible = dte.Windows.Item(GuidBreakpoints).Visible;
                dte.Windows.Item(GuidDisassembly).Visible = dte.Windows.Item(GuidDisassembly).Visible;

                EnumChildWindows(IntPtr.Zero, new EnumWindowsProc(EnumWindowsForProcId), IntPtr.Zero);
                if (s_hwndMainWindow != IntPtr.Zero)
                {
                    EnumChildWindows(s_hwndMainWindow, new EnumWindowsProc(EnumWindowsForBreakpoints), IntPtr.Zero);
                }
                if (s_hwndBreakpointsWindow != IntPtr.Zero)
                {
                    EnumChildWindows(s_hwndBreakpointsWindow, new EnumWindowsProc(EnumWindowsForTreeview), IntPtr.Zero);
                }
                if (s_hwndBreakpointsTreeView != IntPtr.Zero)
                {
                    s_treeview = new TreeView(s_hwndBreakpointsTreeView);
                }
            }
        }

        private static bool EnumWindowsForProcId(IntPtr hWnd, IntPtr lParam)
        { 
            uint procId;
            GetWindowThreadProcessId(hWnd, out procId);
            if(sr_procId == procId)
            {
                if (GetWindow(hWnd, GW_OWNER) == IntPtr.Zero)
                {
                    UInt32 style = GetWindowLong(hWnd, GWL_EXSTYLE);
                    if ((style & WS_EX_TOOLWINDOW) == 0)
                    {
                        s_hwndMainWindow = hWnd;
                        return false;
                    }
                }
            }
            return true;
        }

        private static bool EnumWindowsForBreakpoints(IntPtr hWnd, IntPtr lParam)
        { 
            int maxCount = 255;
            StringBuilder sb = new StringBuilder(maxCount);
            GetClassName(hWnd, sb, maxCount);

            if (sb.ToString() == "VsBreakpointsWindowPane")
            {
                s_hwndBreakpointsWindow = hWnd;
                return false;
            }
            return true;
        }

        private static bool EnumWindowsForTreeview(IntPtr hWnd, IntPtr lParam)
        {
            int maxCount = 255;
            StringBuilder sb = new StringBuilder(maxCount);
            GetClassName(hWnd, sb, maxCount);

            if (sb.ToString() == "SysTreeView32")
            {
                s_hwndBreakpointsTreeView = hWnd;
                return false;
            }
            return true;
        }

        private static String GetAddress(String function, int offset)
        {
            StringBuilder sb = new StringBuilder(function);
            sb.Append(" + 0x");
            sb.Append(offset.ToString("x8"));
            return sb.ToString();
        }
        private static readonly String GuidBreakpoints = "{BE4D7042-BA3F-11D2-840E-00C04F9902C1}";
        private static readonly String GuidDisassembly = "{CF577B8C-4134-11D2-83E5-00C04F9902C1}";
        private static readonly String GuidThreadStack = "{fbd48652-c3e2-4b5e-bf94-816f18cf486f}";
        private static readonly String MenuGoToDisassembly = "DebuggerContextMenus.BreakpointsWindow.GoToDisassembly";
        
        public void GoToDisassembly(DTE dte, String method, int offset)
        {
            Initialize(dte);
            if (s_treeview == null)
            {
                return;
            }

            // Make sure the windows are inti.alized
            DTE dteX5 = dte.Windows.Item(GuidBreakpoints).DTE;
            dte.Windows.Item(GuidDisassembly).Visible = dte.Windows.Item(GuidDisassembly).Visible;

            String address = GetAddress(method, offset);

            int countBefore = s_treeview.Count;
            Breakpoints bps = dte.Debugger.Breakpoints.Add("", "", 0, 0, "", dbgBreakpointConditionType.dbgBreakpointConditionTypeWhenTrue,
                "C#", "", 0, address, 0, dbgHitCountType.dbgHitCountTypeNone);
            int countAfter = s_treeview.Count;

            IntPtr selected;
            IntPtr foundItem;

            if(!FindBreakpoint(address, method, offset, out foundItem, out selected))
            {
                return;
            }

            s_treeview.Select(foundItem);

            dte.ExecuteCommand(MenuGoToDisassembly, "");

            if (countAfter > countBefore)
            {
                bps.Item(1).Delete();
            }

            s_treeview.Select(selected);

            dte.Windows.Item(GuidThreadStack).Activate();
            dte.Windows.Item(GuidDisassembly).Activate();
        }

        private bool FindBreakpoint(String address, String method, int offset, out IntPtr foundItem, out IntPtr selectedItem)
        {
            foundItem = IntPtr.Zero;
            selectedItem = IntPtr.Zero;

            // 1. Exact Match
            if (s_treeview.Find(address, out foundItem, out selectedItem))
            {
                return true;
            }

            // 2. No braces menas no luck
            int firstbrace = method.IndexOf('(');
            if (firstbrace <= 0)
            {
                return false;
            }

            // 3. Sometimes empty braces means no braces
            String tempMethod = method.Substring(0, firstbrace);
            address = GetAddress(tempMethod, offset);
            if (s_treeview.Find(address, out foundItem, out selectedItem))
            {
                return true;
            }

            // 4. Try shorthand notation
            String shMethod = MakeShortHand(method);
            address = GetAddress(shMethod, offset);
            if (s_treeview.Find(address, out foundItem, out selectedItem))
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Sometimes the BreakPoints window creates a shorthand notation of the param types
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        private String MakeShortHand(String method)
        {
            int firstbrace = method.IndexOf('(');
            if (firstbrace <= 0)
            {
                return method;
            }
            int lastbrace = method.IndexOf(')');
            if (lastbrace <= 0)
            {
                return method;
            }
            int len = lastbrace-firstbrace-1;
            if(len<=0)
            {
                return method;
            }
            String parameterString = method.Substring(firstbrace + 1, len);
            String[] parameters = parameterString.Split(',');
            String[] shParams = new String[parameters.Length];
            for (int i=0;i<parameters.Length;++i)
            {
                String parameter = parameters[i];
                String newname = TypeNames.GetShorthandName(parameter);
                if (newname == null)
                {
                    newname = parameter;
                }
                shParams[i] = newname;
            }
            String newParams = String.Join(", ", shParams);
            String newMethod = method.Substring(0, firstbrace);
            return newMethod + "(" + newParams + ")";
        }
    }

    
}
