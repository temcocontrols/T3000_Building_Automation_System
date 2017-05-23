namespace PH_App
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
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Count = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.chartID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ChartName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.RestoreChartCheckBox = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.chart_respective_nodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.chart_respective_lineID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // button3
            // 
            this.button3.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.button3.Location = new System.Drawing.Point(243, 317);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(80, 25);
            this.button3.TabIndex = 6;
            this.button3.Text = "Delete";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.button2.Location = new System.Drawing.Point(157, 317);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(80, 25);
            this.button2.TabIndex = 5;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
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
            this.dataGridView1.Location = new System.Drawing.Point(12, 12);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(394, 299);
            this.dataGridView1.TabIndex = 4;
            // 
            // Count
            // 
            this.Count.HeaderText = "Count";
            this.Count.Name = "Count";
            // 
            // chartID
            // 
            this.chartID.HeaderText = "Chart ID";
            this.chartID.Name = "chartID";
            this.chartID.Visible = false;
            // 
            // ChartName
            // 
            this.ChartName.HeaderText = "Chart Name";
            this.ChartName.Name = "ChartName";
            // 
            // RestoreChartCheckBox
            // 
            this.RestoreChartCheckBox.HeaderText = "Restore ";
            this.RestoreChartCheckBox.Name = "RestoreChartCheckBox";
            // 
            // chart_respective_nodeID
            // 
            this.chart_respective_nodeID.HeaderText = "CHART Respective NodeID";
            this.chart_respective_nodeID.Name = "chart_respective_nodeID";
            this.chart_respective_nodeID.Visible = false;
            // 
            // chart_respective_lineID
            // 
            this.chart_respective_lineID.HeaderText = "Chart Respective line id";
            this.chart_respective_lineID.Name = "chart_respective_lineID";
            this.chart_respective_lineID.Visible = false;
            // 
            // button1
            // 
            this.button1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.button1.Location = new System.Drawing.Point(329, 317);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(80, 25);
            this.button1.TabIndex = 7;
            this.button1.Text = "Restore";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // TrashBox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(430, 370);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.dataGridView1);
            this.Name = "TrashBox";
            this.Text = "TrashBox";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Count;
        private System.Windows.Forms.DataGridViewTextBoxColumn chartID;
        private System.Windows.Forms.DataGridViewTextBoxColumn ChartName;
        private System.Windows.Forms.DataGridViewCheckBoxColumn RestoreChartCheckBox;
        private System.Windows.Forms.DataGridViewTextBoxColumn chart_respective_nodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn chart_respective_lineID;
        private System.Windows.Forms.Button button1;
    }
}