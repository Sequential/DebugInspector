using System;
using System.Collections.Generic;
using System.Text;
using EnvDTE;
using Microsoft.VisualStudio.Shell.Interop;
using DebugInspector.Studio;
using DebugInspector.Controls.Managed.ThreadStacks;

namespace DebugInspector.Managed
{
    class VisualStudioNavHelper 
    {
        public static void GoToAddress(DTE dte, String function, int offset)
        {
            BreakpointsWindow bwin = new BreakpointsWindow();
            bwin.GoToDisassembly(dte, function, offset);
        }

        public static void GoToLine(DTE dte, String file, int line)
        {
            Window win = dte.ItemOperations.OpenFile(file, EnvDTE.Constants.vsViewKindTextView);
            TextSelection selection = dte.ActiveDocument.Selection as TextSelection;
            selection.GotoLine(line, true);
        }

        public static void GoToUrl(DTE dte, String url)
        {
            dte.ItemOperations.Navigate(url, vsNavigateOptions.vsNavigateOptionsDefault);
        }
    }
}
