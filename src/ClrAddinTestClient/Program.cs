using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace ClrInspector
{
    class Program
    {

        private class ALock{ }
        private class BLock { }

        private ALock A = new ALock();
        private BLock B = new BLock();

        static void Main(string[] args)
        {
            Console.WriteLine("ClrInspector at work...");

            ProcessStartInfo psi = new ProcessStartInfo(@"C:\Tools\sscli\ClrAddin\ClrTestApp\bin\Debug\ClrTestApp.exe");

            Process process = Process.Start(psi);
            Thread.Sleep(5000);

            Dac dac = new Dac(process);
            ThreadStackManager tsm = dac.ConstructThreadStacks();


            Console.ReadLine();
            process.Kill();

        }
    }
}
