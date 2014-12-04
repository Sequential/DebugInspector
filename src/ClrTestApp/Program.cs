using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace ClrTest
{
    class Program
    {

        private class ALock { }
        private class BLock { }
        private class CLock { }

        private ALock A = new ALock();
        private BLock B = new BLock();
        private CLock C = new CLock();

        static void Main(string[] args)
        {
            Program p = new Program();


            Thread t1 = new Thread(new ThreadStart(p.LockAB));
            Thread t2 = new Thread(new ThreadStart(p.LockBC));
            Thread t3 = new Thread(new ThreadStart(p.LockCA));

            t1.Start();
            t2.Start();
            t3.Start();

            Thread.Sleep(5000);

            Console.WriteLine("Press the any key");
            Console.ReadLine();
        }

        private void LockAB()
        {
            while (true)
            {
                lock (A)
                {
                    Thread.Sleep(500);
                    lock (B)
                    {
                        Thread.Sleep(500);
                    }
                }
            }
        }

        private void LockBC()
        {
            while (true)
            {
                lock (B)
                {
                    Thread.Sleep(500);
                    lock (C)
                    {
                        Thread.Sleep(500);
                    }
                }
            }
        }

        private void LockCA()
        {
            while (true)
            {
                lock (C)
                {
                    Thread.Sleep(500);
                    lock (A)
                    {
                        Thread.Sleep(500);
                    }
                }
            }
        }
    }
}
