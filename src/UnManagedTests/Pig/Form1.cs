using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;


namespace TestWin
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int val = 1;
            int.TryParse(button1.Text, out val);
           
             Process p = Process.GetProcessById(3016);


           

             button1.Text = "" + (val+1);

            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int id = Process.GetCurrentProcess().Id;
            this.Text = id.ToString();
        }

        public delegate void AddLineDel(String s);
        public void AddLine(String s)
        {
            if (InvokeRequired)
            {
                Invoke(new AddLineDel(AddLine), new object[] { s });
            }
            textBox1.Text += s + Environment.NewLine;
        }

    }
}