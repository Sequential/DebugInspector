namespace DebugInspector.Controls.UnManaged
{
    partial class UnmanagedThreadStacksForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this._unmanagedThreadStacksControl = new DebugInspector.Controls.UnManaged.UnmanagedThreadStacksControl();
            this.SuspendLayout();
            // 
            // _unmanagedThreadStacksControl
            // 
            this._unmanagedThreadStacksControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this._unmanagedThreadStacksControl.Location = new System.Drawing.Point(0, 0);
            this._unmanagedThreadStacksControl.Name = "_unmanagedThreadStacksControl";
            this._unmanagedThreadStacksControl.Size = new System.Drawing.Size(284, 264);
            this._unmanagedThreadStacksControl.TabIndex = 0;
            // 
            // UnmanagedThreadStacksForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 264);
            this.Controls.Add(this._unmanagedThreadStacksControl);
            this.Name = "UnmanagedThreadStacksForm";
            this.Text = "UnmanagedThreadStacksForm";
            this.ResumeLayout(false);

        }

        #endregion

        private UnmanagedThreadStacksControl _unmanagedThreadStacksControl;
    }
}