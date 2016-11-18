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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
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
            this.dataGridView1.Location = new System.Drawing.Point(3, 25);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(907, 220);
            this.dataGridView1.TabIndex = 1;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            this.dataGridView1.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellValueChanged_1);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Edit Node : ";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 254);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Edit Process Line : ";
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
            this.dataGridView2.Location = new System.Drawing.Point(12, 276);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowHeadersVisible = false;
            this.dataGridView2.Size = new System.Drawing.Size(907, 220);
            this.dataGridView2.TabIndex = 3;
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            // 
            // lineID
            // 
            this.lineID.HeaderText = "Line ID";
            this.lineID.Name = "lineID";
            this.lineID.ReadOnly = true;
            this.lineID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.lineID.Visible = false;
            // 
            // lineName
            // 
            this.lineName.HeaderText = "Line Name";
            this.lineName.Name = "lineName";
            // 
            // startNodeID
            // 
            this.startNodeID.HeaderText = "Start Node Name";
            this.startNodeID.Name = "startNodeID";
            this.startNodeID.ReadOnly = true;
            this.startNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // endNodeID
            // 
            this.endNodeID.HeaderText = "End Node Name";
            this.endNodeID.Name = "endNodeID";
            this.endNodeID.ReadOnly = true;
            this.endNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // linecolor
            // 
            this.linecolor.HeaderText = "Color";
            this.linecolor.Name = "linecolor";
            this.linecolor.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // thickness
            // 
            this.thickness.HeaderText = "Thickness";
            this.thickness.Name = "thickness";
            this.thickness.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // prevNodeID
            // 
            this.prevNodeID.HeaderText = "Prev node id";
            this.prevNodeID.Name = "prevNodeID";
            this.prevNodeID.Visible = false;
            // 
            // nextNodeID
            // 
            this.nextNodeID.HeaderText = "Next node id";
            this.nextNodeID.Name = "nextNodeID";
            this.nextNodeID.ReadOnly = true;
            this.nextNodeID.Visible = false;
            // 
            // lineSeriesValue
            // 
            this.lineSeriesValue.HeaderText = "line series";
            this.lineSeriesValue.Name = "lineSeriesValue";
            this.lineSeriesValue.ReadOnly = true;
            this.lineSeriesValue.Visible = false;
            // 
            // lineStatus
            // 
            this.lineStatus.HeaderText = "Show Name";
            this.lineStatus.Name = "lineStatus";
            this.lineStatus.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(294, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(316, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Note : Equivalent Enthalpy is non-editable other fields are editable";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(303, 254);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(415, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Note : Start Node Name and End Node Name are non-editable other fields are editab" +
    "le";
            // 
            // name
            // 
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.name.DefaultCellStyle = dataGridViewCellStyle1;
            this.name.HeaderText = "Name";
            this.name.Name = "name";
            this.name.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Temperature_Source
            // 
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.CB_DGV_Temperature_Source.DefaultCellStyle = dataGridViewCellStyle2;
            this.CB_DGV_Temperature_Source.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            this.CB_DGV_Temperature_Source.HeaderText = "Temperature Source";
            this.CB_DGV_Temperature_Source.Items.AddRange(new object[] {
            "Manual",
            "Web",
            "Device"});
            this.CB_DGV_Temperature_Source.Name = "CB_DGV_Temperature_Source";
            this.CB_DGV_Temperature_Source.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // nodeID
            // 
            this.nodeID.HeaderText = "Node ID";
            this.nodeID.Name = "nodeID";
            this.nodeID.ReadOnly = true;
            this.nodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.nodeID.Visible = false;
            // 
            // temp
            // 
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.temp.DefaultCellStyle = dataGridViewCellStyle3;
            this.temp.HeaderText = "Temperature";
            this.temp.Name = "temp";
            this.temp.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Humidity_Source
            // 
            this.CB_DGV_Humidity_Source.HeaderText = "Humidity Source";
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
            this.humidity.HeaderText = "Humidity";
            this.humidity.Name = "humidity";
            this.humidity.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // equivalentEnthalpy
            // 
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.equivalentEnthalpy.DefaultCellStyle = dataGridViewCellStyle5;
            this.equivalentEnthalpy.HeaderText = "Equivalent Enthalpy";
            this.equivalentEnthalpy.Name = "equivalentEnthalpy";
            this.equivalentEnthalpy.ReadOnly = true;
            this.equivalentEnthalpy.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // color
            // 
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle6.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.color.DefaultCellStyle = dataGridViewCellStyle6;
            this.color.HeaderText = "Color";
            this.color.Name = "color";
            this.color.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // nodeSize
            // 
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.nodeSize.DefaultCellStyle = dataGridViewCellStyle7;
            this.nodeSize.HeaderText = "Node Size";
            this.nodeSize.Name = "nodeSize";
            this.nodeSize.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // airFlow
            // 
            this.airFlow.HeaderText = "Air Flow";
            this.airFlow.Name = "airFlow";
            // 
            // EditNodeLineForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(927, 501);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "EditNodeLineForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Edit Node Line";
            this.Load += new System.EventHandler(this.EditNodeLineForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
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
    }
}