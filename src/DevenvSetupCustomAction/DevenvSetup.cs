using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;

using Microsoft.Win32;
using System.Diagnostics;



namespace DevenvSetupCustomAction
{
    [RunInstaller(true)]
    public partial class DevenvSetup : Installer
    {
        public DevenvSetup()
        {
            InitializeComponent();
        }

        public override void Install(IDictionary stateSaver)
        {
            base.Install(stateSaver);

            //Debugger.Break();

            try
            {
                using (RegistryKey setupKey = Registry.LocalMachine.OpenSubKey(
                  @"SOFTWARE\Microsoft\VisualStudio\9.0\Setup\VS"))
                {
                    if (setupKey != null)
                    {
                        object oDevEnv = setupKey.GetValue("EnvironmentPath");
                        if (oDevEnv != null)
                        {
                            string devenv = oDevEnv.ToString();
                            if (!string.IsNullOrEmpty(devenv))
                            {
                                // TODO: find out if ver 9 is here, and add
                                //       /nosetupvstemplates
                                Process p = Process.Start(devenv, "/setup ");
                                if (p != null)
                                {
                                    p.WaitForExit();
                                }
                                //Process.Start(devenv, "/setup ").WaitForExit();
                            }
                        }
                    }
                }

                using (RegistryKey setupKey = Registry.LocalMachine.OpenSubKey(
                      @"SOFTWARE\Microsoft\VisualStudio\8.0\Setup\VS"))
                {
                    if (setupKey != null)
                    {
                        object oDevEnv = setupKey.GetValue("EnvironmentPath");
                        if (oDevEnv != null)
                        {
                            string devenv = oDevEnv.ToString();
                            if (!string.IsNullOrEmpty(devenv))
                            {
                                Process p = Process.Start(devenv, "/setup ");
                                if (p != null)
                                {
                                    p.WaitForExit();
                                }
                                //Process.Start(devenv, "/setup ").WaitForExit();
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }


        }
    }
}
