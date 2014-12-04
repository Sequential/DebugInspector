using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace DebugInspector.Studio
{
    class TreeView
    {
        [DllImport("user32")]
        private static extern UInt32 SendMessage(IntPtr hWnd, int wMsg, UInt32 wParam, UInt32 lParam); 

        [DllImport("user32", EntryPoint = "SendMessageW")] 
        private static extern int SendMessageTVI(IntPtr hWnd, int wMsg, int wParam, ref TVITEM tvi); 

        [DllImport("kernel32.dll")] 
        public static extern IntPtr LocalAlloc(uint flags, uint cb); 
        
        [DllImport("kernel32.dll")] 
        public static extern IntPtr LocalFree(IntPtr p); 

        private struct TVITEM 
        { 
          public uint mask; 
          public IntPtr hItem; 
          public uint state; 
          public uint stateMask; 
          public int pszText; //if string, must be preallocated 
          public int cchTextMax; 
          public int iImage; 
          public int iSelectedImage; 
          public int cChildren; 
          public IntPtr lParam;  
        } 


        private const int TV_FIRST = 0x1100;
        private const int TVM_GETCOUNT = (TV_FIRST + 5);
        private const int TVM_GETITEM  = (TV_FIRST + 62); 
        private const int TVM_GETNEXTITEM = (TV_FIRST + 10);
        private const int TVM_SELECTITEM = (TV_FIRST + 11); 

        private const int TVGN_ROOT = 0x0; 
        private const int TVGN_NEXT = 0x1; 
        private const int TVGN_CHILD = 0x4;
        private const int TVGN_CARET = 0x9;
        private const int TVIF_TEXT = 0x1;
        private const int TVIF_STATE = 0x8;
        private const int MY_MAXLVITEMTEXT = 1024;

        private const int TVIS_SELECTED = 0x0002;



        private readonly IntPtr r_hwnd;

        public TreeView(IntPtr hwnd)
        {
            r_hwnd = hwnd;
        }

        public int Count
        {
            get 
            {
                return (int)SendMessage(r_hwnd, TVM_GETCOUNT, 0, 0);
            }
        }

        public bool Find(String name, out IntPtr foundItem, out IntPtr previousSelected)
        {
            IntPtr item = (IntPtr)SendMessage(r_hwnd, TVM_GETNEXTITEM, TVGN_ROOT, 0);
            foundItem = IntPtr.Zero;
            previousSelected = IntPtr.Zero;
            while (item != IntPtr.Zero)
            {
                TVITEM tvItem = new TVITEM();
                IntPtr pszText = LocalAlloc(0x40, MY_MAXLVITEMTEXT);
                tvItem.hItem = item;
                tvItem.cchTextMax = MY_MAXLVITEMTEXT;
                tvItem.pszText = (int)pszText;
                tvItem.stateMask = TVIS_SELECTED;
                tvItem.mask = TVIF_TEXT | TVIF_STATE;

                int bGetItemPtr = SendMessageTVI(r_hwnd, TVM_GETITEM, 0, ref tvItem);
                String buffer = Marshal.PtrToStringUni((IntPtr)tvItem.pszText, MY_MAXLVITEMTEXT);
                LocalFree(pszText);

                int len = buffer.IndexOf('\0');
                if (len > 0)
                {
                    buffer = buffer.Substring(0, len);
                }
                buffer = buffer.Trim();
                if (buffer == name)
                {
                    foundItem = item;
                }
                else if ((tvItem.state & TVIS_SELECTED) == TVIS_SELECTED)
                {
                    previousSelected = item;
                }
                item = (IntPtr)SendMessage(r_hwnd, TVM_GETNEXTITEM, TVGN_NEXT, (uint)item.ToInt32());
            }
            return foundItem != IntPtr.Zero;
        }

        public void Select(IntPtr item)
        {
            IntPtr flag = new IntPtr(TVGN_CARET);
            SendMessage(r_hwnd, TVM_SELECTITEM, TVGN_CARET, (UInt32)item.ToInt32());
        }
        
        public IntPtr SelectedItem
        {
            get
            {
                return (IntPtr)SendMessage(r_hwnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
            }
            
        }

    }
}
