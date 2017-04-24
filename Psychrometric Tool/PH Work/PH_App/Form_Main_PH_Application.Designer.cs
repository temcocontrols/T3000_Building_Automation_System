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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
            this.phChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.lb_db_name = new System.Windows.Forms.Label();
            this.lb_buildingselectedShow = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.groupBox4ttt = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelline1 = new System.Windows.Forms.Label();
            this.lb_DP = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.lb_RH = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.lb_humidity_ratio = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.lb_dbt = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.gb_showstatus = new System.Windows.Forms.GroupBox();
            this.lb_vertical_line2 = new System.Windows.Forms.Label();
            this.lb_for_line_vertical1 = new System.Windows.Forms.Label();
            this.lb_numberOfHandler = new System.Windows.Forms.Label();
            this.lb_number_of_chart_label = new System.Windows.Forms.Label();
            this.lb_device_status = new System.Windows.Forms.Label();
            this.lb_devicestatusshow = new System.Windows.Forms.Label();
            this.lb_realtimeshow = new System.Windows.Forms.Label();
            this.lb_modeshow = new System.Windows.Forms.Label();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveConfigurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadConfigurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.simulationMode = new System.Windows.Forms.ToolStripMenuItem();
            this.exportDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.historyPlotToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.trashBoxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gridViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.excelExportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpPsychometricChartToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.groupBox4ttt.SuspendLayout();
            this.gb_showstatus.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // phChart
            // 
            this.phChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea3.AxisX.Title = "Enthalpy h[KJ/kg]";
            chartArea3.AxisY.Title = "Pressure P[MPa]";
            chartArea3.Name = "ChartArea1";
            this.phChart.ChartAreas.Add(chartArea3);
            legend3.Enabled = false;
            legend3.Name = "Legend1";
            this.phChart.Legends.Add(legend3);
            this.phChart.Location = new System.Drawing.Point(225, 38);
            this.phChart.Name = "phChart";
            series3.ChartArea = "ChartArea1";
            series3.Legend = "Legend1";
            series3.Name = "Series1";
            this.phChart.Series.Add(series3);
            this.phChart.Size = new System.Drawing.Size(858, 564);
            this.phChart.TabIndex = 1;
            this.phChart.Text = "chart1";
            // 
            // lb_db_name
            // 
            this.lb_db_name.AutoSize = true;
            this.lb_db_name.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.lb_db_name.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_db_name.Location = new System.Drawing.Point(140, 39);
            this.lb_db_name.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_db_name.Name = "lb_db_name";
            this.lb_db_name.Size = new System.Drawing.Size(52, 14);
            this.lb_db_name.TabIndex = 53;
            this.lb_db_name.Text = "dbName";
            // 
            // lb_buildingselectedShow
            // 
            this.lb_buildingselectedShow.AutoSize = true;
            this.lb_buildingselectedShow.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_buildingselectedShow.Location = new System.Drawing.Point(48, 39);
            this.lb_buildingselectedShow.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_buildingselectedShow.Name = "lb_buildingselectedShow";
            this.lb_buildingselectedShow.Size = new System.Drawing.Size(87, 13);
            this.lb_buildingselectedShow.TabIndex = 52;
            this.lb_buildingselectedShow.Text = "Current Building: ";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle7.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle7.Font = new System.Drawing.Font("Arial", 8.25F);
            dataGridViewCellStyle7.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.ControlDark;
            dataGridViewCellStyle7.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle7;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.id,
            this.name});
            this.dataGridView1.Location = new System.Drawing.Point(51, 56);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.Size = new System.Drawing.Size(153, 546);
            this.dataGridView1.TabIndex = 51;
            this.dataGridView1.TabStop = false;
            // 
            // id
            // 
            dataGridViewCellStyle8.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle8.SelectionBackColor = System.Drawing.Color.SteelBlue;
            dataGridViewCellStyle8.SelectionForeColor = System.Drawing.Color.WhiteSmoke;
            this.id.DefaultCellStyle = dataGridViewCellStyle8;
            this.id.HeaderText = "ID";
            this.id.Name = "id";
            this.id.ReadOnly = true;
            this.id.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.id.Width = 20;
            // 
            // name
            // 
            dataGridViewCellStyle9.SelectionBackColor = System.Drawing.Color.SteelBlue;
            dataGridViewCellStyle9.SelectionForeColor = System.Drawing.Color.White;
            this.name.DefaultCellStyle = dataGridViewCellStyle9;
            this.name.HeaderText = "System Name";
            this.name.Name = "name";
            this.name.ReadOnly = true;
            this.name.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.name.Width = 130;
            // 
            // groupBox4ttt
            // 
            this.groupBox4ttt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox4ttt.Controls.Add(this.label5);
            this.groupBox4ttt.Controls.Add(this.label4);
            this.groupBox4ttt.Controls.Add(this.label3);
            this.groupBox4ttt.Controls.Add(this.labelline1);
            this.groupBox4ttt.Controls.Add(this.lb_DP);
            this.groupBox4ttt.Controls.Add(this.label25);
            this.groupBox4ttt.Controls.Add(this.lb_RH);
            this.groupBox4ttt.Controls.Add(this.label24);
            this.groupBox4ttt.Controls.Add(this.lb_humidity_ratio);
            this.groupBox4ttt.Controls.Add(this.label23);
            this.groupBox4ttt.Controls.Add(this.lb_dbt);
            this.groupBox4ttt.Controls.Add(this.label21);
            this.groupBox4ttt.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.groupBox4ttt.Location = new System.Drawing.Point(9, 37);
            this.groupBox4ttt.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.groupBox4ttt.Name = "groupBox4ttt";
            this.groupBox4ttt.Padding = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.groupBox4ttt.Size = new System.Drawing.Size(40, 603);
            this.groupBox4ttt.TabIndex = 50;
            this.groupBox4ttt.TabStop = false;
            // 
            // label5
            // 
            this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label5.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label5.Location = new System.Drawing.Point(0, 101);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 1);
            this.label5.TabIndex = 36;
            // 
            // label4
            // 
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label4.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label4.Location = new System.Drawing.Point(0, 153);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(45, 1);
            this.label4.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label3.Location = new System.Drawing.Point(0, 206);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 1);
            this.label3.TabIndex = 11;
            // 
            // labelline1
            // 
            this.labelline1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelline1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelline1.Location = new System.Drawing.Point(1, 54);
            this.labelline1.Name = "labelline1";
            this.labelline1.Size = new System.Drawing.Size(45, 1);
            this.labelline1.TabIndex = 10;
            // 
            // lb_DP
            // 
            this.lb_DP.AutoSize = true;
            this.lb_DP.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.lb_DP.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_DP.Location = new System.Drawing.Point(4, 181);
            this.lb_DP.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_DP.Name = "lb_DP";
            this.lb_DP.Size = new System.Drawing.Size(27, 14);
            this.lb_DP.TabIndex = 7;
            this.lb_DP.Text = "null";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.label25.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label25.Location = new System.Drawing.Point(4, 167);
            this.label25.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(14, 14);
            this.label25.TabIndex = 6;
            this.label25.Text = "S";
            // 
            // lb_RH
            // 
            this.lb_RH.AutoSize = true;
            this.lb_RH.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.lb_RH.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_RH.Location = new System.Drawing.Point(4, 126);
            this.lb_RH.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_RH.Name = "lb_RH";
            this.lb_RH.Size = new System.Drawing.Size(27, 14);
            this.lb_RH.TabIndex = 5;
            this.lb_RH.Text = "null";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.label24.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label24.Location = new System.Drawing.Point(4, 112);
            this.label24.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(14, 14);
            this.label24.TabIndex = 4;
            this.label24.Text = "T";
            // 
            // lb_humidity_ratio
            // 
            this.lb_humidity_ratio.AutoSize = true;
            this.lb_humidity_ratio.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.lb_humidity_ratio.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_humidity_ratio.Location = new System.Drawing.Point(4, 78);
            this.lb_humidity_ratio.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_humidity_ratio.Name = "lb_humidity_ratio";
            this.lb_humidity_ratio.Size = new System.Drawing.Size(27, 14);
            this.lb_humidity_ratio.TabIndex = 3;
            this.lb_humidity_ratio.Text = "null";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.label23.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label23.Location = new System.Drawing.Point(2, 64);
            this.label23.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(14, 14);
            this.label23.TabIndex = 2;
            this.label23.Text = "H";
            // 
            // lb_dbt
            // 
            this.lb_dbt.AutoSize = true;
            this.lb_dbt.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.lb_dbt.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_dbt.Location = new System.Drawing.Point(4, 27);
            this.lb_dbt.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_dbt.Name = "lb_dbt";
            this.lb_dbt.Size = new System.Drawing.Size(27, 14);
            this.lb_dbt.TabIndex = 1;
            this.lb_dbt.Text = "null";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            this.label21.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label21.Location = new System.Drawing.Point(4, 13);
            this.label21.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(14, 14);
            this.label21.TabIndex = 0;
            this.label21.Text = "P";
            // 
            // gb_showstatus
            // 
            this.gb_showstatus.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gb_showstatus.Controls.Add(this.lb_realtimeshow);
            this.gb_showstatus.Controls.Add(this.lb_modeshow);
            this.gb_showstatus.Controls.Add(this.lb_vertical_line2);
            this.gb_showstatus.Controls.Add(this.lb_for_line_vertical1);
            this.gb_showstatus.Controls.Add(this.lb_numberOfHandler);
            this.gb_showstatus.Controls.Add(this.lb_number_of_chart_label);
            this.gb_showstatus.Controls.Add(this.lb_device_status);
            this.gb_showstatus.Controls.Add(this.lb_devicestatusshow);
            this.gb_showstatus.Location = new System.Drawing.Point(51, 608);
            this.gb_showstatus.Name = "gb_showstatus";
            this.gb_showstatus.Size = new System.Drawing.Size(1032, 32);
            this.gb_showstatus.TabIndex = 54;
            this.gb_showstatus.TabStop = false;
            // 
            // lb_vertical_line2
            // 
            this.lb_vertical_line2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lb_vertical_line2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_vertical_line2.Location = new System.Drawing.Point(568, 10);
            this.lb_vertical_line2.Name = "lb_vertical_line2";
            this.lb_vertical_line2.Size = new System.Drawing.Size(1, 20);
            this.lb_vertical_line2.TabIndex = 59;
            // 
            // lb_for_line_vertical1
            // 
            this.lb_for_line_vertical1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lb_for_line_vertical1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_for_line_vertical1.Location = new System.Drawing.Point(425, 10);
            this.lb_for_line_vertical1.Name = "lb_for_line_vertical1";
            this.lb_for_line_vertical1.Size = new System.Drawing.Size(1, 20);
            this.lb_for_line_vertical1.TabIndex = 58;
            // 
            // lb_numberOfHandler
            // 
            this.lb_numberOfHandler.AutoSize = true;
            this.lb_numberOfHandler.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_numberOfHandler.Location = new System.Drawing.Point(97, 10);
            this.lb_numberOfHandler.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_numberOfHandler.Name = "lb_numberOfHandler";
            this.lb_numberOfHandler.Size = new System.Drawing.Size(13, 13);
            this.lb_numberOfHandler.TabIndex = 51;
            this.lb_numberOfHandler.Text = "0";
            // 
            // lb_number_of_chart_label
            // 
            this.lb_number_of_chart_label.AutoSize = true;
            this.lb_number_of_chart_label.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_number_of_chart_label.Location = new System.Drawing.Point(5, 11);
            this.lb_number_of_chart_label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lb_number_of_chart_label.Name = "lb_number_of_chart_label";
            this.lb_number_of_chart_label.Size = new System.Drawing.Size(95, 13);
            this.lb_number_of_chart_label.TabIndex = 50;
            this.lb_number_of_chart_label.Text = "Number of chart  : ";
            // 
            // lb_device_status
            // 
            this.lb_device_status.AutoSize = true;
            this.lb_device_status.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_device_status.Location = new System.Drawing.Point(346, 11);
            this.lb_device_status.Name = "lb_device_status";
            this.lb_device_status.Size = new System.Drawing.Size(71, 13);
            this.lb_device_status.TabIndex = 1;
            this.lb_device_status.Text = "disconnected";
            // 
            // lb_devicestatusshow
            // 
            this.lb_devicestatusshow.AutoSize = true;
            this.lb_devicestatusshow.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_devicestatusshow.Location = new System.Drawing.Point(273, 11);
            this.lb_devicestatusshow.Name = "lb_devicestatusshow";
            this.lb_devicestatusshow.Size = new System.Drawing.Size(77, 13);
            this.lb_devicestatusshow.TabIndex = 0;
            this.lb_devicestatusshow.Text = "Device Status:";
            // 
            // lb_realtimeshow
            // 
            this.lb_realtimeshow.AutoSize = true;
            this.lb_realtimeshow.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_realtimeshow.Location = new System.Drawing.Point(494, 12);
            this.lb_realtimeshow.Name = "lb_realtimeshow";
            this.lb_realtimeshow.Size = new System.Drawing.Size(55, 13);
            this.lb_realtimeshow.TabIndex = 61;
            this.lb_realtimeshow.Text = "Real Time";
            // 
            // lb_modeshow
            // 
            this.lb_modeshow.AutoSize = true;
            this.lb_modeshow.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.lb_modeshow.Location = new System.Drawing.Point(432, 13);
            this.lb_modeshow.Name = "lb_modeshow";
            this.lb_modeshow.Size = new System.Drawing.Size(43, 13);
            this.lb_modeshow.TabIndex = 60;
            this.lb_modeshow.Text = "Mode : ";
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1095, 24);
            this.menuStrip1.TabIndex = 55;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.printToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.saveConfigurationToolStripMenuItem,
            this.loadConfigurationToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // printToolStripMenuItem
            // 
            this.printToolStripMenuItem.Name = "printToolStripMenuItem";
            this.printToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.printToolStripMenuItem.Text = "Print";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.saveAsToolStripMenuItem.Text = "Save As";
            // 
            // saveConfigurationToolStripMenuItem
            // 
            this.saveConfigurationToolStripMenuItem.Name = "saveConfigurationToolStripMenuItem";
            this.saveConfigurationToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.saveConfigurationToolStripMenuItem.Text = "Save Configuration";
            // 
            // loadConfigurationToolStripMenuItem
            // 
            this.loadConfigurationToolStripMenuItem.Name = "loadConfigurationToolStripMenuItem";
            this.loadConfigurationToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.loadConfigurationToolStripMenuItem.Text = "Load Configuration";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // optionToolStripMenuItem
            // 
            this.optionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.simulationMode,
            this.exportDataToolStripMenuItem,
            this.historyPlotToolStripMenuItem1,
            this.trashBoxToolStripMenuItem,
            this.gridViewToolStripMenuItem,
            this.excelExportToolStripMenuItem});
            this.optionToolStripMenuItem.Name = "optionToolStripMenuItem";
            this.optionToolStripMenuItem.Size = new System.Drawing.Size(56, 20);
            this.optionToolStripMenuItem.Text = "Option";
            // 
            // simulationMode
            // 
            this.simulationMode.Name = "simulationMode";
            this.simulationMode.Size = new System.Drawing.Size(152, 22);
            this.simulationMode.Text = "PH Calculator";
            // 
            // exportDataToolStripMenuItem
            // 
            this.exportDataToolStripMenuItem.Name = "exportDataToolStripMenuItem";
            this.exportDataToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exportDataToolStripMenuItem.Text = "Export Data";
            // 
            // historyPlotToolStripMenuItem1
            // 
            this.historyPlotToolStripMenuItem1.Name = "historyPlotToolStripMenuItem1";
            this.historyPlotToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.historyPlotToolStripMenuItem1.Text = "History Plot";
            this.historyPlotToolStripMenuItem1.Visible = false;
            // 
            // trashBoxToolStripMenuItem
            // 
            this.trashBoxToolStripMenuItem.Name = "trashBoxToolStripMenuItem";
            this.trashBoxToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.trashBoxToolStripMenuItem.Text = "Trash Box";
            // 
            // gridViewToolStripMenuItem
            // 
            this.gridViewToolStripMenuItem.Name = "gridViewToolStripMenuItem";
            this.gridViewToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.gridViewToolStripMenuItem.Text = "Grid View";
            // 
            // excelExportToolStripMenuItem
            // 
            this.excelExportToolStripMenuItem.Name = "excelExportToolStripMenuItem";
            this.excelExportToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.excelExportToolStripMenuItem.Text = "Excel Export";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpPsychometricChartToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // helpPsychometricChartToolStripMenuItem
            // 
            this.helpPsychometricChartToolStripMenuItem.Name = "helpPsychometricChartToolStripMenuItem";
            this.helpPsychometricChartToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.helpPsychometricChartToolStripMenuItem.Text = "PH chart";
            // 
            // Form_Main_PH_Application
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1095, 643);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.gb_showstatus);
            this.Controls.Add(this.lb_db_name);
            this.Controls.Add(this.lb_buildingselectedShow);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.groupBox4ttt);
            this.Controls.Add(this.phChart);
            this.Name = "Form_Main_PH_Application";
            this.Text = "PH Chart";
            this.Load += new System.EventHandler(this.Form_Main_PH_Application_Load);
            this.Shown += new System.EventHandler(this.Form_Main_PH_Application_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.phChart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.groupBox4ttt.ResumeLayout(false);
            this.groupBox4ttt.PerformLayout();
            this.gb_showstatus.ResumeLayout(false);
            this.gb_showstatus.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.DataVisualization.Charting.Chart phChart;
        private System.Windows.Forms.Label lb_db_name;
        private System.Windows.Forms.Label lb_buildingselectedShow;
        public System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn id;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.GroupBox groupBox4ttt;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelline1;
        private System.Windows.Forms.Label lb_DP;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label lb_RH;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label lb_humidity_ratio;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label lb_dbt;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.GroupBox gb_showstatus;
        private System.Windows.Forms.Label lb_realtimeshow;
        private System.Windows.Forms.Label lb_modeshow;
        private System.Windows.Forms.Label lb_vertical_line2;
        private System.Windows.Forms.Label lb_for_line_vertical1;
        private System.Windows.Forms.Label lb_numberOfHandler;
        private System.Windows.Forms.Label lb_number_of_chart_label;
        private System.Windows.Forms.Label lb_device_status;
        private System.Windows.Forms.Label lb_devicestatusshow;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveConfigurationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadConfigurationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem simulationMode;
        private System.Windows.Forms.ToolStripMenuItem exportDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem historyPlotToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem trashBoxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem gridViewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem excelExportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpPsychometricChartToolStripMenuItem;
    }
}