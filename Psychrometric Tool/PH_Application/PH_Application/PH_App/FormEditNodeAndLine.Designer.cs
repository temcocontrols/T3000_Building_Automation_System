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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormEditNodeAndLine));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle10 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle12 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle13 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle14 = new System.Windows.Forms.DataGridViewCellStyle();
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.label2 = new System.Windows.Forms.Label();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.label1 = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.colorDialog2 = new System.Windows.Forms.ColorDialog();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.label5 = new System.Windows.Forms.Label();
            this.lb_process = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.lb_node_one_name = new System.Windows.Forms.Label();
            this.lb_node_two_name = new System.Windows.Forms.Label();
            this.lb_dbt = new System.Windows.Forms.Label();
            this.lb_unit_degCelcius = new System.Windows.Forms.Label();
            this.lb_dbt_node1_value = new System.Windows.Forms.Label();
            this.lb_dbt_node2_value = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.lb_Pressure_node1 = new System.Windows.Forms.Label();
            this.lb_Pressure_node2 = new System.Windows.Forms.Label();
            this.label38 = new System.Windows.Forms.Label();
            this.label37 = new System.Windows.Forms.Label();
            this.lb_enthalpy_node1_value = new System.Windows.Forms.Label();
            this.lb_enthalpy_node2_value = new System.Windows.Forms.Label();
            this.lb_horizontal_line = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
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
            this.T1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.T2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.P1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.P2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.E1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.E2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.changeInEnergy = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.lbEnthalpyChange = new System.Windows.Forms.Label();
            this.lbEnthalpyChangeUnit = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
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
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveFD
            // 
            this.saveFD.RestoreDirectory = true;
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
            this.lineStatus,
            this.T1,
            this.T2,
            this.P1,
            this.P2,
            this.E1,
            this.E2,
            this.changeInEnergy});
            resources.ApplyResources(this.dataGridView2, "dataGridView2");
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentClick);
            this.dataGridView2.CellContentDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentDoubleClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            this.dataGridView2.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellValueChanged);
            this.dataGridView2.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.dataGridView2_DataError);
            this.dataGridView2.SelectionChanged += new System.EventHandler(this.dataGridView2_SelectionChanged);
            this.dataGridView2.MouseHover += new System.EventHandler(this.dataGridView2_MouseHover);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
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
            resources.ApplyResources(this.dataGridView1, "dataGridView1");
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            this.dataGridView1.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.dataGridView1_EditingControlShowing_1);
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
            // 
            // lb_process
            // 
            resources.ApplyResources(this.lb_process, "lb_process");
            this.lb_process.Name = "lb_process";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // lb_node_one_name
            // 
            resources.ApplyResources(this.lb_node_one_name, "lb_node_one_name");
            this.lb_node_one_name.Name = "lb_node_one_name";
            // 
            // lb_node_two_name
            // 
            resources.ApplyResources(this.lb_node_two_name, "lb_node_two_name");
            this.lb_node_two_name.Name = "lb_node_two_name";
            // 
            // lb_dbt
            // 
            resources.ApplyResources(this.lb_dbt, "lb_dbt");
            this.lb_dbt.Name = "lb_dbt";
            // 
            // lb_unit_degCelcius
            // 
            resources.ApplyResources(this.lb_unit_degCelcius, "lb_unit_degCelcius");
            this.lb_unit_degCelcius.Name = "lb_unit_degCelcius";
            // 
            // lb_dbt_node1_value
            // 
            resources.ApplyResources(this.lb_dbt_node1_value, "lb_dbt_node1_value");
            this.lb_dbt_node1_value.Name = "lb_dbt_node1_value";
            // 
            // lb_dbt_node2_value
            // 
            resources.ApplyResources(this.lb_dbt_node2_value, "lb_dbt_node2_value");
            this.lb_dbt_node2_value.Name = "lb_dbt_node2_value";
            // 
            // label18
            // 
            resources.ApplyResources(this.label18, "label18");
            this.label18.Name = "label18";
            // 
            // label17
            // 
            resources.ApplyResources(this.label17, "label17");
            this.label17.Name = "label17";
            // 
            // lb_Pressure_node1
            // 
            resources.ApplyResources(this.lb_Pressure_node1, "lb_Pressure_node1");
            this.lb_Pressure_node1.Name = "lb_Pressure_node1";
            // 
            // lb_Pressure_node2
            // 
            resources.ApplyResources(this.lb_Pressure_node2, "lb_Pressure_node2");
            this.lb_Pressure_node2.Name = "lb_Pressure_node2";
            // 
            // label38
            // 
            resources.ApplyResources(this.label38, "label38");
            this.label38.Name = "label38";
            // 
            // label37
            // 
            resources.ApplyResources(this.label37, "label37");
            this.label37.Name = "label37";
            // 
            // lb_enthalpy_node1_value
            // 
            resources.ApplyResources(this.lb_enthalpy_node1_value, "lb_enthalpy_node1_value");
            this.lb_enthalpy_node1_value.Name = "lb_enthalpy_node1_value";
            // 
            // lb_enthalpy_node2_value
            // 
            resources.ApplyResources(this.lb_enthalpy_node2_value, "lb_enthalpy_node2_value");
            this.lb_enthalpy_node2_value.Name = "lb_enthalpy_node2_value";
            // 
            // lb_horizontal_line
            // 
            this.lb_horizontal_line.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            resources.ApplyResources(this.lb_horizontal_line, "lb_horizontal_line");
            this.lb_horizontal_line.Name = "lb_horizontal_line";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lbEnthalpyChange);
            this.groupBox1.Controls.Add(this.lbEnthalpyChangeUnit);
            this.groupBox1.Controls.Add(this.label11);
            this.groupBox1.Controls.Add(this.lb_horizontal_line);
            this.groupBox1.Controls.Add(this.lb_enthalpy_node2_value);
            this.groupBox1.Controls.Add(this.lb_enthalpy_node1_value);
            this.groupBox1.Controls.Add(this.label37);
            this.groupBox1.Controls.Add(this.label38);
            this.groupBox1.Controls.Add(this.lb_Pressure_node2);
            this.groupBox1.Controls.Add(this.lb_Pressure_node1);
            this.groupBox1.Controls.Add(this.label17);
            this.groupBox1.Controls.Add(this.label18);
            this.groupBox1.Controls.Add(this.lb_dbt_node2_value);
            this.groupBox1.Controls.Add(this.lb_dbt_node1_value);
            this.groupBox1.Controls.Add(this.lb_unit_degCelcius);
            this.groupBox1.Controls.Add(this.lb_dbt);
            this.groupBox1.Controls.Add(this.lb_node_two_name);
            this.groupBox1.Controls.Add(this.lb_node_one_name);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.lb_process);
            this.groupBox1.Controls.Add(this.label5);
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
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
            this.lineName.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
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
            this.prevNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // nextNodeID
            // 
            resources.ApplyResources(this.nextNodeID, "nextNodeID");
            this.nextNodeID.Name = "nextNodeID";
            this.nextNodeID.ReadOnly = true;
            this.nextNodeID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // lineSeriesValue
            // 
            resources.ApplyResources(this.lineSeriesValue, "lineSeriesValue");
            this.lineSeriesValue.Name = "lineSeriesValue";
            this.lineSeriesValue.ReadOnly = true;
            this.lineSeriesValue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // lineStatus
            // 
            this.lineStatus.FalseValue = "false";
            resources.ApplyResources(this.lineStatus, "lineStatus");
            this.lineStatus.IndeterminateValue = "0";
            this.lineStatus.Name = "lineStatus";
            this.lineStatus.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.lineStatus.TrueValue = "true";
            // 
            // T1
            // 
            resources.ApplyResources(this.T1, "T1");
            this.T1.Name = "T1";
            this.T1.ReadOnly = true;
            // 
            // T2
            // 
            resources.ApplyResources(this.T2, "T2");
            this.T2.Name = "T2";
            this.T2.ReadOnly = true;
            // 
            // P1
            // 
            resources.ApplyResources(this.P1, "P1");
            this.P1.Name = "P1";
            this.P1.ReadOnly = true;
            // 
            // P2
            // 
            resources.ApplyResources(this.P2, "P2");
            this.P2.Name = "P2";
            this.P2.ReadOnly = true;
            // 
            // E1
            // 
            resources.ApplyResources(this.E1, "E1");
            this.E1.Name = "E1";
            this.E1.ReadOnly = true;
            // 
            // E2
            // 
            resources.ApplyResources(this.E2, "E2");
            this.E2.Name = "E2";
            this.E2.ReadOnly = true;
            // 
            // changeInEnergy
            // 
            resources.ApplyResources(this.changeInEnergy, "changeInEnergy");
            this.changeInEnergy.Name = "changeInEnergy";
            this.changeInEnergy.ReadOnly = true;
            // 
            // lbEnthalpyChange
            // 
            resources.ApplyResources(this.lbEnthalpyChange, "lbEnthalpyChange");
            this.lbEnthalpyChange.Name = "lbEnthalpyChange";
            // 
            // lbEnthalpyChangeUnit
            // 
            resources.ApplyResources(this.lbEnthalpyChangeUnit, "lbEnthalpyChangeUnit");
            this.lbEnthalpyChangeUnit.Name = "lbEnthalpyChangeUnit";
            // 
            // label11
            // 
            resources.ApplyResources(this.label11, "label11");
            this.label11.Name = "label11";
            // 
            // name
            // 
            dataGridViewCellStyle8.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.name.DefaultCellStyle = dataGridViewCellStyle8;
            resources.ApplyResources(this.name, "name");
            this.name.Name = "name";
            this.name.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Temperature_Source
            // 
            dataGridViewCellStyle9.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.CB_DGV_Temperature_Source.DefaultCellStyle = dataGridViewCellStyle9;
            this.CB_DGV_Temperature_Source.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            resources.ApplyResources(this.CB_DGV_Temperature_Source, "CB_DGV_Temperature_Source");
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
            dataGridViewCellStyle10.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.temp.DefaultCellStyle = dataGridViewCellStyle10;
            resources.ApplyResources(this.temp, "temp");
            this.temp.Name = "temp";
            this.temp.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // CB_DGV_Pressure_Source
            // 
            this.CB_DGV_Pressure_Source.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            resources.ApplyResources(this.CB_DGV_Pressure_Source, "CB_DGV_Pressure_Source");
            this.CB_DGV_Pressure_Source.Name = "CB_DGV_Pressure_Source";
            // 
            // humidity
            // 
            dataGridViewCellStyle11.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.humidity.DefaultCellStyle = dataGridViewCellStyle11;
            resources.ApplyResources(this.humidity, "humidity");
            this.humidity.Name = "humidity";
            this.humidity.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // equivalentEnthalpy
            // 
            dataGridViewCellStyle12.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.equivalentEnthalpy.DefaultCellStyle = dataGridViewCellStyle12;
            resources.ApplyResources(this.equivalentEnthalpy, "equivalentEnthalpy");
            this.equivalentEnthalpy.Name = "equivalentEnthalpy";
            this.equivalentEnthalpy.ReadOnly = true;
            this.equivalentEnthalpy.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // color
            // 
            dataGridViewCellStyle13.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle13.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.color.DefaultCellStyle = dataGridViewCellStyle13;
            resources.ApplyResources(this.color, "color");
            this.color.Name = "color";
            this.color.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // nodeSize
            // 
            dataGridViewCellStyle14.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            this.nodeSize.DefaultCellStyle = dataGridViewCellStyle14;
            resources.ApplyResources(this.nodeSize, "nodeSize");
            this.nodeSize.Name = "nodeSize";
            this.nodeSize.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // FormEditNodeAndLine
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.MaximizeBox = false;
            this.Name = "FormEditNodeAndLine";
            this.Load += new System.EventHandler(this.FormEditNodeAndLine_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
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
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lb_process;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label lb_node_one_name;
        private System.Windows.Forms.Label lb_node_two_name;
        private System.Windows.Forms.Label lb_dbt;
        private System.Windows.Forms.Label lb_unit_degCelcius;
        private System.Windows.Forms.Label lb_dbt_node1_value;
        private System.Windows.Forms.Label lb_dbt_node2_value;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label lb_Pressure_node1;
        private System.Windows.Forms.Label lb_Pressure_node2;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.Label lb_enthalpy_node1_value;
        private System.Windows.Forms.Label lb_enthalpy_node2_value;
        private System.Windows.Forms.Label lb_horizontal_line;
        private System.Windows.Forms.GroupBox groupBox1;
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
        private System.Windows.Forms.DataGridViewTextBoxColumn T1;
        private System.Windows.Forms.DataGridViewTextBoxColumn T2;
        private System.Windows.Forms.DataGridViewTextBoxColumn P1;
        private System.Windows.Forms.DataGridViewTextBoxColumn P2;
        private System.Windows.Forms.DataGridViewTextBoxColumn E1;
        private System.Windows.Forms.DataGridViewTextBoxColumn E2;
        private System.Windows.Forms.DataGridViewTextBoxColumn changeInEnergy;
        private System.Windows.Forms.Label lbEnthalpyChange;
        private System.Windows.Forms.Label lbEnthalpyChangeUnit;
        private System.Windows.Forms.Label label11;
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