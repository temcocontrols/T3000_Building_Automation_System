namespace WFA_psychometric_chart
{
    partial class form_heat_map
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(form_heat_map));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.dtp_From = new System.Windows.Forms.DateTimePicker();
            this.dtp_To = new System.Windows.Forms.DateTimePicker();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.chartOptionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.printMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.gb_select_date = new System.Windows.Forms.GroupBox();
            this.menuStrip1.SuspendLayout();
            this.gb_select_date.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // button1
            // 
            resources.ApplyResources(this.button1, "button1");
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // dtp_From
            // 
            this.dtp_From.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            resources.ApplyResources(this.dtp_From, "dtp_From");
            this.dtp_From.Name = "dtp_From";
            this.dtp_From.Value = new System.DateTime(2016, 1, 1, 0, 0, 0, 0);
            // 
            // dtp_To
            // 
            this.dtp_To.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            resources.ApplyResources(this.dtp_To, "dtp_To");
            this.dtp_To.Name = "dtp_To";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chartOptionToolStripMenuItem});
            resources.ApplyResources(this.menuStrip1, "menuStrip1");
            this.menuStrip1.Name = "menuStrip1";
            // 
            // chartOptionToolStripMenuItem
            // 
            this.chartOptionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.printMapToolStripMenuItem,
            this.saveAsImageToolStripMenuItem});
            this.chartOptionToolStripMenuItem.Name = "chartOptionToolStripMenuItem";
            resources.ApplyResources(this.chartOptionToolStripMenuItem, "chartOptionToolStripMenuItem");
            // 
            // printMapToolStripMenuItem
            // 
            this.printMapToolStripMenuItem.Name = "printMapToolStripMenuItem";
            resources.ApplyResources(this.printMapToolStripMenuItem, "printMapToolStripMenuItem");
            this.printMapToolStripMenuItem.Click += new System.EventHandler(this.printMapToolStripMenuItem_Click);
            // 
            // saveAsImageToolStripMenuItem
            // 
            this.saveAsImageToolStripMenuItem.Name = "saveAsImageToolStripMenuItem";
            resources.ApplyResources(this.saveAsImageToolStripMenuItem, "saveAsImageToolStripMenuItem");
            this.saveAsImageToolStripMenuItem.Click += new System.EventHandler(this.saveAsImageToolStripMenuItem_Click);
            // 
            // gb_select_date
            // 
            this.gb_select_date.Controls.Add(this.dtp_From);
            this.gb_select_date.Controls.Add(this.label1);
            this.gb_select_date.Controls.Add(this.dtp_To);
            this.gb_select_date.Controls.Add(this.label2);
            this.gb_select_date.Controls.Add(this.button1);
            resources.ApplyResources(this.gb_select_date, "gb_select_date");
            this.gb_select_date.Name = "gb_select_date";
            this.gb_select_date.TabStop = false;
            // 
            // form_heat_map
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gb_select_date);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "form_heat_map";
            this.Load += new System.EventHandler(this.form_heat_map_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.gb_select_date.ResumeLayout(false);
            this.gb_select_date.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.DateTimePicker dtp_From;
        private System.Windows.Forms.DateTimePicker dtp_To;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem chartOptionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem printMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsImageToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFD;
        private System.Windows.Forms.GroupBox gb_select_date;
    }
}