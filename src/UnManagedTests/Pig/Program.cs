using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using TestWin;
using Microsoft.Win32.SafeHandles;

namespace Test
{
    class Program
    {
        private static Object l1 = new object();
        private static Object l2 = new object();

        private static Thread t0;
        private static Thread t1 = new Thread(new ThreadStart(m1));
        private static Thread t2 = new Thread(new ThreadStart(m2));

        private static Mutex s_mutex;
        private static Mutex s_mutex2;

        private static Form1 s_frm1;

        static void Main(string[] args)
        {
            t0 = Thread.CurrentThread;

            CritsFromClr.Class1 class1 = new CritsFromClr.Class1();

            Int32 cs1=0;
            Int32 cs2=0;
            class1.CreatCrits(ref cs1, ref cs2);

            

            s_mutex = new Mutex(true, "Mutex from C# Code");
            s_mutex2 = new Mutex(true, "Mutex2 from C# Code");

            t1.Start();
            t2.Start();


            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            s_frm1 = new Form1();
            s_frm1.AddLine("CRITSECADDR==" + cs1);
            s_frm1.AddLine("CRITSECADDR==" + cs2);
            Application.Run(s_frm1);

            Thread.Sleep(1000);

            t1.Join();
        }

        static void m1()
        {
            lock (l1)
            { 
                Thread.Sleep(1000);
                t2.Join();
            }
        }

        static void m2()
        {
            while (true)
            {
                lock (l1)
                {
                    Thread.Sleep(5000);
                    s_frm1.AddLine("MTID==" + Thread.CurrentThread.ManagedThreadId);
                    t0.Join();            
                }
            }
        }
    }
}
