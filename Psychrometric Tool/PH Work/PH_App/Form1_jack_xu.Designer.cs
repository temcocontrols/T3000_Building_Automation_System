namespace Example6_3
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
            this.PlotPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PlotPanel.BackColor = System.Drawing.Color.White;
            this.PlotPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PlotPanel.Location = new System.Drawing.Point(8, 8);
            this.PlotPanel.Name = "PlotPanel";
            this.PlotPanel.Size = new System.Drawing.Size(330, 248);
            this.PlotPanel.TabIndex = 1;
            // 
            // trkElevation
            // 
            this.trkElevation.Location = new System.Drawing.Point(32, 16);
            this.trkElevation.Maximum = 90;
            this.trkElevation.Minimum = -90;
            this.trkElevation.Name = "trkElevation";
            this.trkElevation.Size = new System.Drawing.Size(216, 45);
            this.trkElevation.TabIndex = 2;
            this.trkElevation.TickFrequency = 5;
            this.trkElevation.Value = 30;
            this.trkElevation.Scroll += new System.EventHandler(this.trkElevation_Scroll);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.tbElevation);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.trkElevation);
            this.groupBox1.Location = new System.Drawing.Point(8, 264);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(328, 64);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Elevation (degree)";
            // 
            // tbElevation
            // 
            this.tbElevation.Location = new System.Drawing.Point(272, 24);
            this.tbElevation.Name = "tbElevation";
            this.tbElevation.Size = new System.Drawing.Size(48, 20);
            this.tbElevation.TabIndex = 5;
            this.tbElevation.Text = "30";
            this.tbElevation.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbElevation.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbElevation_KeyUp);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(248, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(19, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "90";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(22, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "-90";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(240, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "180";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.tbAzimuth);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.trkAzimuth);
            this.groupBox2.Location = new System.Drawing.Point(8, 336);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(328, 64);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Azimuth (degree)";
            // 
            // tbAzimuth
            // 
            this.tbAzimuth.Location = new System.Drawing.Point(272, 24);
            this.tbAzimuth.Name = "tbAzimuth";
            this.tbAzimuth.Size = new System.Drawing.Size(48, 20);
            this.tbAzimuth.TabIndex = 6;
            this.tbAzimuth.Text = "-37";
            this.tbAzimuth.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbAzimuth.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbAzimuth_KeyUp);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 24);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "-180";
            // 
            // trkAzimuth
            // 
            this.trkAzimuth.Location = new System.Drawing.Point(32, 16);
            this.trkAzimuth.Maximum = 180;
            this.trkAzimuth.Minimum = -180;
            this.trkAzimuth.Name = "trkAzimuth";
            this.trkAzimuth.Size = new System.Drawing.Size(216, 45);
            this.trkAzimuth.TabIndex = 2;
            this.trkAzimuth.TickFrequency = 10;
            this.trkAzimuth.Value = -37;
            this.trkAzimuth.Scroll += new System.EventHandler(this.trkAzimuth_Scroll);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 405);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.PlotPanel);
            this.Name = "Form1";
            this.Text = "Form1";
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

