namespace PH_App
{
    partial class Form1_jack_xu
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1_jack_xu));
            this.PlotPanel = new System.Windows.Forms.Panel();
            this.trkElevation = new System.Windows.Forms.TrackBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tbElevation = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tbAzimuth = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.trkAzimuth = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.trkElevation)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trkAzimuth)).BeginInit();
            this.SuspendLayout();
            // 
            // PlotPanel
            // 
            resources.ApplyResources(this.PlotPanel, "PlotPanel");
            this.PlotPanel.BackColor = System.Drawing.Color.White;
            this.PlotPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PlotPanel.Name = "PlotPanel";
            // 
            // trkElevation
            // 
            resources.ApplyResources(this.trkElevation, "trkElevation");
            this.trkElevation.Maximum = 90;
            this.trkElevation.Minimum = -90;
            this.trkElevation.Name = "trkElevation";
            this.trkElevation.TickFrequency = 5;
            this.trkElevation.Value = 30;
            this.trkElevation.Scroll += new System.EventHandler(this.trkElevation_Scroll);
            // 
            // groupBox1
            // 
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Controls.Add(this.tbElevation);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.trkElevation);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // tbElevation
            // 
            resources.ApplyResources(this.tbElevation, "tbElevation");
            this.tbElevation.Name = "tbElevation";
            this.tbElevation.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbElevation_KeyUp);
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // groupBox2
            // 
            resources.ApplyResources(this.groupBox2, "groupBox2");
            this.groupBox2.Controls.Add(this.tbAzimuth);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.trkAzimuth);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.TabStop = false;
            // 
            // tbAzimuth
            // 
            resources.ApplyResources(this.tbAzimuth, "tbAzimuth");
            this.tbAzimuth.Name = "tbAzimuth";
            this.tbAzimuth.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbAzimuth_KeyUp);
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // trkAzimuth
            // 
            resources.ApplyResources(this.trkAzimuth, "trkAzimuth");
            this.trkAzimuth.Maximum = 180;
            this.trkAzimuth.Minimum = -180;
            this.trkAzimuth.Name = "trkAzimuth";
            this.trkAzimuth.TickFrequency = 10;
            this.trkAzimuth.Value = -37;
            this.trkAzimuth.Scroll += new System.EventHandler(this.trkAzimuth_Scroll);
            // 
            // Form1_jack_xu
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.PlotPanel);
            this.Name = "Form1_jack_xu";
            ((System.ComponentModel.ISupportInitialize)(this.trkElevation)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trkAzimuth)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.Panel PlotPanel;
        private System.Windows.Forms.TrackBar trkElevation;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar trkAzimuth;
        private System.Windows.Forms.TextBox tbElevation;
        private System.Windows.Forms.TextBox tbAzimuth;
    }
}

