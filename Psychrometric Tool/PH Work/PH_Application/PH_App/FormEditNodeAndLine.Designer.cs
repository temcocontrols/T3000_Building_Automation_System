namespace PH_App
{
    partial class FormEditNodeAndLine
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
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.label2 = new System.Windows.Forms.Label();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.label1 = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.colorDialog2 = new System.Windows.Forms.ColorDialog();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
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
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CB_DGV_Temperature_Source = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.nodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.temp = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CB_DGV_Pressure_Source = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.humidity = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.equivalentEnthalpy = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.color = new System.Windows.Forms.DataGridViewButtonColumn();
            this.nodeSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // saveFD
            // 
            this.saveFD.RestoreDirectory = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label2.Location = new System.Drawing.Point(16, 270);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 13);
            this.label2.TabIndex = 15;
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
            this.dataGridView2.Location = new System.Drawing.Point(8, 294);
            this.dataGridView2.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.Size = new System.Drawing.Size(543, 246);
            this.dataGridView2.TabIndex = 14;
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            this.dataGridView2.SelectionChanged += new System.EventHandler(this.dataGridView2_SelectionChanged);
            this.dataGridView2.MouseHover += new System.EventHandler(this.dataGridView2_MouseHover);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label1.Location = new System.Drawing.Point(8, 6);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Edit Node : ";
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.name,
            this.CB_DGV_Temperature_Source,
            this.nodeID,
            this.temp,
            this.CB_DGV_Pressure_Source,
            this.humidity,
            this.equivalentEnthalpy,
            this.color,
            this.nodeSize});
            this.dataGridView1.Location = new System.Drawing.Point(11, 22);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(784, 237);
            this.dataGridView1.TabIndex = 12;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            this.dataGridView1.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.dataGridView1_EditingControlShowing_1);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label4.Location = new System.Drawing.Point(243, 270);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(415, 13);
            this.label4.TabIndex = 17;
            this.label4.Text = "Note : Start Node Name and End Node Name are non-editable other fields are editab" +
    "le";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label3.Location = new System.Drawing.Point(290, 6);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(316, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Note : Equivalent Enthalpy is non-editable other fields are editable";
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
            this.lineName.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
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
            this.linecolor.Width = 40;
            // 
            // thickness
            // 
            this.thickness.HeaderText = "Thickness";
            this.thickness.Name = "thickness";
            this.thickness.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.thickness.Width = 60;
            // 
            // prevNodeID
            // 
            this.prevNodeID.HeaderText = "Prev node id";
            this.prevNodeID.Name = "prevNodeID";
            this.prevNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.prevNodeID.Visible = false;
            // 
            // nextNodeID
            // 
            this.nextNodeID.HeaderText = "Next node id";
            this.nextNodeID.Name = "nextNodeID";
            this.nextNodeID.ReadOnly = true;
            this.nextNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.nextNodeID.Visible = false;
            // 
            // lineSeriesValue
            // 
            this.lineSeriesValue.HeaderText = "line series";
            this.lineSeriesValue.Name = "lineSeriesValue";
            this.lineSeriesValue.ReadOnly = true;
            this.lineSeriesValue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.lineSeriesValue.Visible = false;
            // 
            // lineStatus
            // 
            this.lineStatus.FalseValue = "false";
            this.lineStatus.HeaderText = "Show Name";
            this.lineStatus.IndeterminateValue = "0";
            this.lineStatus.Name = "lineStatus";
            this.lineStatus.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.lineStatus.TrueValue = "true";
            this.lineStatus.Width = 80;
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
            this.temp.HeaderText = "Temperature(°C)";
            this.temp.Name = "temp";
            this.temp.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Pressure_Source
            // 
            this.CB_DGV_Pressure_Source.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            this.CB_DGV_Pressure_Source.HeaderText = "Pressure Source";
            this.CB_DGV_Pressure_Source.Name = "CB_DGV_Pressure_Source";
            // 
            // humidity
            // 
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.humidity.DefaultCellStyle = dataGridViewCellStyle4;
            this.humidity.HeaderText = "Pressure[MPa]";
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
            this.color.Width = 40;
            // 
            // nodeSize
            // 
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.nodeSize.DefaultCellStyle = dataGridViewCellStyle7;
            this.nodeSize.HeaderText = "Node Size";
            this.nodeSize.Name = "nodeSize";
            this.nodeSize.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // FormEditNodeAndLine
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(816, 553);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.MaximizeBox = false;
            this.Name = "FormEditNodeAndLine";
            this.Text = "FormEditNodeAndLine";
            this.Load += new System.EventHandler(this.FormEditNodeAndLine_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.SaveFileDialog saveFD;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ColorDialog colorDialog2;
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
        private System.Windows.Forms.DataGridViewComboBoxColumn CB_DGV_Pressure_Source;
        private System.Windows.Forms.DataGridViewTextBoxColumn humidity;
        private System.Windows.Forms.DataGridViewTextBoxColumn equivalentEnthalpy;
        private System.Windows.Forms.DataGridViewButtonColumn color;
        private System.Windows.Forms.DataGridViewTextBoxColumn nodeSize;
    }
}