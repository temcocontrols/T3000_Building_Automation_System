namespace WFA_PH_CurveDemo
{
    partial class formPHChartMain
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.phChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.button1 = new System.Windows.Forms.Button();
            this.lb_fluid_name = new System.Windows.Forms.Label();
            this.CMSOperation = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addNodeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lbCoordinate = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).BeginInit();
            this.CMSOperation.SuspendLayout();
            this.SuspendLayout();
            // 
            // phChart
            // 
            this.phChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.Title = "Enthalpy h[KJ/kg]";
            chartArea1.AxisY.Title = "Pressure P[MPa]";
            chartArea1.Name = "ChartArea1";
            this.phChart.ChartAreas.Add(chartArea1);
            legend1.Enabled = false;
            legend1.Name = "Legend1";
            this.phChart.Legends.Add(legend1);
            this.phChart.Location = new System.Drawing.Point(14, 21);
            this.phChart.Name = "phChart";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.phChart.Series.Add(series1);
            this.phChart.Size = new System.Drawing.Size(809, 495);
            this.phChart.TabIndex = 0;
            this.phChart.Text = "chart1";
            this.phChart.MouseDown += new System.Windows.Forms.MouseEventHandler(this.phChart_MouseDown);
            this.phChart.MouseMove += new System.Windows.Forms.MouseEventHandler(this.phChart_MouseMove);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.button1.Location = new System.Drawing.Point(630, 524);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Plot";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Visible = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lb_fluid_name
            // 
            this.lb_fluid_name.AutoSize = true;
            this.lb_fluid_name.Location = new System.Drawing.Point(361, 9);
            this.lb_fluid_name.Name = "lb_fluid_name";
            this.lb_fluid_name.Size = new System.Drawing.Size(27, 13);
            this.lb_fluid_name.TabIndex = 2;
            this.lb_fluid_name.Text = "Title";
            // 
            // CMSOperation
            // 
            this.CMSOperation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addNodeToolStripMenuItem});
            this.CMSOperation.Name = "CMSOperation";
            this.CMSOperation.Size = new System.Drawing.Size(134, 26);
            // 
            // addNodeToolStripMenuItem
            // 
            this.addNodeToolStripMenuItem.Name = "addNodeToolStripMenuItem";
            this.addNodeToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.addNodeToolStripMenuItem.Text = "Insert node";
            this.addNodeToolStripMenuItem.Click += new System.EventHandler(this.addNodeToolStripMenuItem_Click);
            // 
            // lbCoordinate
            // 
            this.lbCoordinate.AutoSize = true;
            this.lbCoordinate.Location = new System.Drawing.Point(35, 529);
            this.lbCoordinate.Name = "lbCoordinate";
            this.lbCoordinate.Size = new System.Drawing.Size(35, 13);
            this.lbCoordinate.TabIndex = 4;
            this.lbCoordinate.Text = "label1";
            // 
            // formPHChartMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(833, 550);
            this.Controls.Add(this.lbCoordinate);
            this.Controls.Add(this.lb_fluid_name);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.phChart);
            this.Name = "formPHChartMain";
            this.Text = "PH Diagram";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.formPHChartMain_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).EndInit();
            this.CMSOperation.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button button1;
        public System.Windows.Forms.DataVisualization.Charting.Chart phChart;
        private System.Windows.Forms.Label lb_fluid_name;
        private System.Windows.Forms.ContextMenuStrip CMSOperation;
        private System.Windows.Forms.ToolStripMenuItem addNodeToolStripMenuItem;
        public System.Windows.Forms.Label lbCoordinate;
    }
}

