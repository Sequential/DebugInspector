namespace DebugInspector
{
    partial class ManagedThreadStacksControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose( bool disposing )
        {
            if( disposing )
            {
                if(components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose( disposing );
        }


        #region Component Designer generated code
        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ManagedThreadStacksControl));
            this.tvwHierarchy = new System.Windows.Forms.TreeView();
            this._imgListIcons = new System.Windows.Forms.ImageList(this.components);
            this.tsHelpSite = new System.Windows.Forms.ToolStripButton();
            this._toolStrip = new System.Windows.Forms.ToolStrip();
            this._toolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // tvwHierarchy
            // 
            this.tvwHierarchy.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvwHierarchy.ImageIndex = 0;
            this.tvwHierarchy.ImageList = this._imgListIcons;
            this.tvwHierarchy.Location = new System.Drawing.Point(0, 25);
            this.tvwHierarchy.Name = "tvwHierarchy";
            this.tvwHierarchy.SelectedImageIndex = 0;
            this.tvwHierarchy.Size = new System.Drawing.Size(199, 125);
            this.tvwHierarchy.TabIndex = 0;
            this.tvwHierarchy.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.tvwHierarchy_NodeMouseDoubleClick);
            // 
            // _imgListIcons
            // 
            this._imgListIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("_imgListIcons.ImageStream")));
            this._imgListIcons.TransparentColor = System.Drawing.Color.Transparent;
            this._imgListIcons.Images.SetKeyName(0, "application.png");
            this._imgListIcons.Images.SetKeyName(1, "application_cascade.png");
            this._imgListIcons.Images.SetKeyName(2, "cog.png");
            this._imgListIcons.Images.SetKeyName(3, "key.png");
            this._imgListIcons.Images.SetKeyName(4, "hourglass.png");
            this._imgListIcons.Images.SetKeyName(5, "text_list_bullets.png");
            this._imgListIcons.Images.SetKeyName(6, "arrow_branch.png");
            this._imgListIcons.Images.SetKeyName(7, "arrow_refresh.png");
            this._imgListIcons.Images.SetKeyName(8, "arrow_switch.png");
            // 
            // tsHelpSite
            // 
            this.tsHelpSite.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.tsHelpSite.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsHelpSite.Name = "tsHelpSite";
            this.tsHelpSite.Size = new System.Drawing.Size(36, 22);
            this.tsHelpSite.Text = "Help";
            this.tsHelpSite.ToolTipText = "Go to www.debuginspector.com";
            this.tsHelpSite.Click += new System.EventHandler(this.tsHelpSite_Click);
            // 
            // _toolStrip
            // 
            this._toolStrip.CanOverflow = false;
            this._toolStrip.GripMargin = new System.Windows.Forms.Padding(0);
            this._toolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this._toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsHelpSite});
            this._toolStrip.Location = new System.Drawing.Point(0, 0);
            this._toolStrip.Name = "_toolStrip";
            this._toolStrip.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._toolStrip.Size = new System.Drawing.Size(199, 25);
            this._toolStrip.TabIndex = 1;
            // 
            // ManagedThreadStacksControl
            // 
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.tvwHierarchy);
            this.Controls.Add(this._toolStrip);
            this.Name = "ManagedThreadStacksControl";
            this.Size = new System.Drawing.Size(199, 150);
            this._toolStrip.ResumeLayout(false);
            this._toolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.TreeView tvwHierarchy;
        private System.Windows.Forms.ImageList _imgListIcons;
        private System.Windows.Forms.ToolStripButton tsHelpSite;
        private System.Windows.Forms.ToolStrip _toolStrip;

    }
}
