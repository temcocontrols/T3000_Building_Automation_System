namespace WFA_psychometric_chart
{
    partial class TrashBox
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TrashBox));
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Count = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.chartID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ChartName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.RestoreChartCheckBox = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.chart_respective_nodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.chart_respective_lineID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Count,
            this.chartID,
            this.ChartName,
            this.RestoreChartCheckBox,
            this.chart_respective_nodeID,
            this.chart_respective_lineID});
            resources.ApplyResources(this.dataGridView1, "dataGridView1");
            this.dataGridView1.Name = "dataGridView1";
            // 
            // Count
            // 
            resources.ApplyResources(this.Count, "Count");
            this.Count.Name = "Count";
            // 
            // chartID
            // 
            resources.ApplyResources(this.chartID, "chartID");
            this.chartID.Name = "chartID";
            // 
            // ChartName
            // 
            resources.ApplyResources(this.ChartName, "ChartName");
            this.ChartName.Name = "ChartName";
            // 
            // RestoreChartCheckBox
            // 
            resources.ApplyResources(this.RestoreChartCheckBox, "RestoreChartCheckBox");
            this.RestoreChartCheckBox.Name = "RestoreChartCheckBox";
            // 
            // chart_respective_nodeID
            // 
            resources.ApplyResources(this.chart_respective_nodeID, "chart_respective_nodeID");
            this.chart_respective_nodeID.Name = "chart_respective_nodeID";
            // 
            // chart_respective_lineID
            // 
            resources.ApplyResources(this.chart_respective_lineID, "chart_respective_lineID");
            this.chart_respective_lineID.Name = "chart_respective_lineID";
            // 
            // button1
            // 
            resources.ApplyResources(this.button1, "button1");
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            resources.ApplyResources(this.button2, "button2");
            this.button2.Name = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            resources.ApplyResources(this.button3, "button3");
            this.button3.Name = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // TrashBox
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.dataGridView1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "TrashBox";
            this.Load += new System.EventHandler(this.TrashBox_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Count;
        private System.Windows.Forms.DataGridViewTextBoxColumn chartID;
        private System.Windows.Forms.DataGridViewTextBoxColumn ChartName;
        private System.Windows.Forms.DataGridViewCheckBoxColumn RestoreChartCheckBox;
        private System.Windows.Forms.DataGridViewTextBoxColumn chart_respective_nodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn chart_respective_lineID;
    }
}