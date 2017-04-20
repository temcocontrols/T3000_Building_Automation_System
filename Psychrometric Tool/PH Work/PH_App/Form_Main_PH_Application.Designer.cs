namespace Example6_3
{
    partial class Form_Main_PH_Application
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.phChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).BeginInit();
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
            this.phChart.Location = new System.Drawing.Point(12, 12);
            this.phChart.Name = "phChart";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.phChart.Series.Add(series1);
            this.phChart.Size = new System.Drawing.Size(813, 520);
            this.phChart.TabIndex = 1;
            this.phChart.Text = "chart1";
            // 
            // Form_Main_PH_Application
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(833, 550);
            this.Controls.Add(this.phChart);
            this.Name = "Form_Main_PH_Application";
            this.Text = "Form_Main_PH_Application";
            this.Load += new System.EventHandler(this.Form_Main_PH_Application_Load);
            this.Shown += new System.EventHandler(this.Form_Main_PH_Application_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.DataVisualization.Charting.Chart phChart;
    }
}