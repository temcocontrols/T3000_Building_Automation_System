namespace WFA_psychometric_chart
{
    partial class EditNodeLineForm
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditNodeLineForm));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CB_DGV_Temperature_Source = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.nodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.temp = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CB_DGV_Humidity_Source = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.humidity = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.equivalentEnthalpy = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.color = new System.Windows.Forms.DataGridViewButtonColumn();
            this.nodeSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.airFlow = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.lineID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.lineName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.startNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.endNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.linecolor = new System.Windows.Forms.DataGridViewButtonColumn();
            this.thickness = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.prevNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.nextNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.lineSeriesValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.lineStatus = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.name,
            this.CB_DGV_Temperature_Source,
            this.nodeID,
            this.temp,
            this.CB_DGV_Humidity_Source,
            this.humidity,
            this.equivalentEnthalpy,
            this.color,
            this.nodeSize,
            this.airFlow});
            resources.ApplyResources(this.dataGridView1, "dataGridView1");
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            this.dataGridView1.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellValueChanged_1);
            // 
            // name
            // 
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.name.DefaultCellStyle = dataGridViewCellStyle1;
            resources.ApplyResources(this.name, "name");
            this.name.Name = "name";
            this.name.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Temperature_Source
            // 
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.CB_DGV_Temperature_Source.DefaultCellStyle = dataGridViewCellStyle2;
            this.CB_DGV_Temperature_Source.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            resources.ApplyResources(this.CB_DGV_Temperature_Source, "CB_DGV_Temperature_Source");
            this.CB_DGV_Temperature_Source.Items.AddRange(new object[] {
            "Manual",
            "Web",
            "Device"});
            this.CB_DGV_Temperature_Source.Name = "CB_DGV_Temperature_Source";
            this.CB_DGV_Temperature_Source.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // nodeID
            // 
            resources.ApplyResources(this.nodeID, "nodeID");
            this.nodeID.Name = "nodeID";
            this.nodeID.ReadOnly = true;
            this.nodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // temp
            // 
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.temp.DefaultCellStyle = dataGridViewCellStyle3;
            resources.ApplyResources(this.temp, "temp");
            this.temp.Name = "temp";
            this.temp.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Humidity_Source
            // 
            resources.ApplyResources(this.CB_DGV_Humidity_Source, "CB_DGV_Humidity_Source");
            this.CB_DGV_Humidity_Source.Items.AddRange(new object[] {
            "Manual",
            "Web",
            "Device"});
            this.CB_DGV_Humidity_Source.Name = "CB_DGV_Humidity_Source";
            // 
            // humidity
            // 
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.humidity.DefaultCellStyle = dataGridViewCellStyle4;
            resources.ApplyResources(this.humidity, "humidity");
            this.humidity.Name = "humidity";
            this.humidity.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // equivalentEnthalpy
            // 
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.equivalentEnthalpy.DefaultCellStyle = dataGridViewCellStyle5;
            resources.ApplyResources(this.equivalentEnthalpy, "equivalentEnthalpy");
            this.equivalentEnthalpy.Name = "equivalentEnthalpy";
            this.equivalentEnthalpy.ReadOnly = true;
            this.equivalentEnthalpy.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // color
            // 
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle6.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.color.DefaultCellStyle = dataGridViewCellStyle6;
            resources.ApplyResources(this.color, "color");
            this.color.Name = "color";
            this.color.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // nodeSize
            // 
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.nodeSize.DefaultCellStyle = dataGridViewCellStyle7;
            resources.ApplyResources(this.nodeSize, "nodeSize");
            this.nodeSize.Name = "nodeSize";
            this.nodeSize.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // airFlow
            // 
            resources.ApplyResources(this.airFlow, "airFlow");
            this.airFlow.Name = "airFlow";
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
            // dataGridView2
            // 
            this.dataGridView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView2.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.lineID,
            this.lineName,
            this.startNodeID,
            this.endNodeID,
            this.linecolor,
            this.thickness,
            this.prevNodeID,
            this.nextNodeID,
            this.lineSeriesValue,
            this.lineStatus});
            resources.ApplyResources(this.dataGridView2, "dataGridView2");
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowHeadersVisible = false;
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            // 
            // lineID
            // 
            resources.ApplyResources(this.lineID, "lineID");
            this.lineID.Name = "lineID";
            this.lineID.ReadOnly = true;
            this.lineID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // lineName
            // 
            resources.ApplyResources(this.lineName, "lineName");
            this.lineName.Name = "lineName";
            // 
            // startNodeID
            // 
            resources.ApplyResources(this.startNodeID, "startNodeID");
            this.startNodeID.Name = "startNodeID";
            this.startNodeID.ReadOnly = true;
            this.startNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // endNodeID
            // 
            resources.ApplyResources(this.endNodeID, "endNodeID");
            this.endNodeID.Name = "endNodeID";
            this.endNodeID.ReadOnly = true;
            this.endNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // linecolor
            // 
            resources.ApplyResources(this.linecolor, "linecolor");
            this.linecolor.Name = "linecolor";
            this.linecolor.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // thickness
            // 
            resources.ApplyResources(this.thickness, "thickness");
            this.thickness.Name = "thickness";
            this.thickness.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // prevNodeID
            // 
            resources.ApplyResources(this.prevNodeID, "prevNodeID");
            this.prevNodeID.Name = "prevNodeID";
            // 
            // nextNodeID
            // 
            resources.ApplyResources(this.nextNodeID, "nextNodeID");
            this.nextNodeID.Name = "nextNodeID";
            this.nextNodeID.ReadOnly = true;
            // 
            // lineSeriesValue
            // 
            resources.ApplyResources(this.lineSeriesValue, "lineSeriesValue");
            this.lineSeriesValue.Name = "lineSeriesValue";
            this.lineSeriesValue.ReadOnly = true;
            // 
            // lineStatus
            // 
            resources.ApplyResources(this.lineStatus, "lineStatus");
            this.lineStatus.Name = "lineStatus";
            this.lineStatus.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // EditNodeLineForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "EditNodeLineForm";
            this.Load += new System.EventHandler(this.EditNodeLineForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.DataGridViewTextBoxColumn lineID;
        private System.Windows.Forms.DataGridViewTextBoxColumn lineName;
        private System.Windows.Forms.DataGridViewTextBoxColumn startNodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn endNodeID;
        private System.Windows.Forms.DataGridViewButtonColumn linecolor;
        private System.Windows.Forms.DataGridViewTextBoxColumn thickness;
        private System.Windows.Forms.DataGridViewTextBoxColumn prevNodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn nextNodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn lineSeriesValue;
        private System.Windows.Forms.DataGridViewCheckBoxColumn lineStatus;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.DataGridViewComboBoxColumn CB_DGV_Temperature_Source;
        private System.Windows.Forms.DataGridViewTextBoxColumn nodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn temp;
        private System.Windows.Forms.DataGridViewComboBoxColumn CB_DGV_Humidity_Source;
        private System.Windows.Forms.DataGridViewTextBoxColumn humidity;
        private System.Windows.Forms.DataGridViewTextBoxColumn equivalentEnthalpy;
        private System.Windows.Forms.DataGridViewButtonColumn color;
        private System.Windows.Forms.DataGridViewTextBoxColumn nodeSize;
        private System.Windows.Forms.DataGridViewTextBoxColumn airFlow;
        public System.Windows.Forms.DataGridView dataGridView1;
    }
}