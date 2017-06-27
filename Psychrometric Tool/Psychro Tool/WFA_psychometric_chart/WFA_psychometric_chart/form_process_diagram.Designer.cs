namespace WFA_psychometric_chart
{
    partial class form_process_diagram
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(form_process_diagram));
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.CMSinsertNode = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.insertNodeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tb1_Instruction = new System.Windows.Forms.TextBox();
            this.sourceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripTextBoxSource = new System.Windows.Forms.ToolStripTextBox();
            this.nameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripTextBoxName = new System.Windows.Forms.ToolStripTextBox();
            this.labelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripTextBoxLabel = new System.Windows.Forms.ToolStripTextBox();
            this.showToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripComboBoxShow = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.colorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripTextBoxColor = new System.Windows.Forms.ToolStripTextBox();
            this.setDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.CMSinsertNode.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // chart1
            // 
            this.chart1.AllowDrop = true;
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            this.chart1.ContextMenuStrip = this.CMSinsertNode;
            legend1.Enabled = false;
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            resources.ApplyResources(this.chart1, "chart1");
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Click += new System.EventHandler(this.chart1_Click);
            this.chart1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.mouse_pressed_event);
            this.chart1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MouseMoveEvent);
            // 
            // CMSinsertNode
            // 
            this.CMSinsertNode.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.insertNodeToolStripMenuItem});
            this.CMSinsertNode.Name = "contextMenuStrip2";
            resources.ApplyResources(this.CMSinsertNode, "CMSinsertNode");
            // 
            // insertNodeToolStripMenuItem
            // 
            this.insertNodeToolStripMenuItem.Name = "insertNodeToolStripMenuItem";
            resources.ApplyResources(this.insertNodeToolStripMenuItem, "insertNodeToolStripMenuItem");
            this.insertNodeToolStripMenuItem.Click += new System.EventHandler(this.insertNodeToolStripMenuItem_Click);
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
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.Name = "label6";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tb1_Instruction);
            resources.ApplyResources(this.groupBox2, "groupBox2");
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.TabStop = false;
            // 
            // tb1_Instruction
            // 
            resources.ApplyResources(this.tb1_Instruction, "tb1_Instruction");
            this.tb1_Instruction.Name = "tb1_Instruction";
            this.tb1_Instruction.ReadOnly = true;
            // 
            // sourceToolStripMenuItem
            // 
            resources.ApplyResources(this.sourceToolStripMenuItem, "sourceToolStripMenuItem");
            this.sourceToolStripMenuItem.Name = "sourceToolStripMenuItem";
            // 
            // toolStripTextBoxSource
            // 
            resources.ApplyResources(this.toolStripTextBoxSource, "toolStripTextBoxSource");
            this.toolStripTextBoxSource.Name = "toolStripTextBoxSource";
            this.toolStripTextBoxSource.Tag = "Enter source";
            // 
            // nameToolStripMenuItem
            // 
            resources.ApplyResources(this.nameToolStripMenuItem, "nameToolStripMenuItem");
            this.nameToolStripMenuItem.Name = "nameToolStripMenuItem";
            // 
            // toolStripTextBoxName
            // 
            resources.ApplyResources(this.toolStripTextBoxName, "toolStripTextBoxName");
            this.toolStripTextBoxName.Name = "toolStripTextBoxName";
            // 
            // labelToolStripMenuItem
            // 
            resources.ApplyResources(this.labelToolStripMenuItem, "labelToolStripMenuItem");
            this.labelToolStripMenuItem.Name = "labelToolStripMenuItem";
            // 
            // toolStripTextBoxLabel
            // 
            resources.ApplyResources(this.toolStripTextBoxLabel, "toolStripTextBoxLabel");
            this.toolStripTextBoxLabel.Name = "toolStripTextBoxLabel";
            // 
            // showToolStripMenuItem
            // 
            resources.ApplyResources(this.showToolStripMenuItem, "showToolStripMenuItem");
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            // 
            // toolStripComboBoxShow
            // 
            this.toolStripComboBoxShow.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            resources.ApplyResources(this.toolStripComboBoxShow, "toolStripComboBoxShow");
            this.toolStripComboBoxShow.Items.AddRange(new object[] {
            resources.GetString("toolStripComboBoxShow.Items"),
            resources.GetString("toolStripComboBoxShow.Items1"),
            resources.GetString("toolStripComboBoxShow.Items2")});
            this.toolStripComboBoxShow.Name = "toolStripComboBoxShow";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            resources.ApplyResources(this.toolStripSeparator1, "toolStripSeparator1");
            // 
            // colorToolStripMenuItem
            // 
            resources.ApplyResources(this.colorToolStripMenuItem, "colorToolStripMenuItem");
            this.colorToolStripMenuItem.Name = "colorToolStripMenuItem";
            this.colorToolStripMenuItem.Click += new System.EventHandler(this.colorToolStripMenuItem_Click);
            // 
            // toolStripTextBoxColor
            // 
            this.toolStripTextBoxColor.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            resources.ApplyResources(this.toolStripTextBoxColor, "toolStripTextBoxColor");
            this.toolStripTextBoxColor.Name = "toolStripTextBoxColor";
            // 
            // setDataToolStripMenuItem
            // 
            this.setDataToolStripMenuItem.BackColor = System.Drawing.SystemColors.ControlDark;
            resources.ApplyResources(this.setDataToolStripMenuItem, "setDataToolStripMenuItem");
            this.setDataToolStripMenuItem.Name = "setDataToolStripMenuItem";
            this.setDataToolStripMenuItem.Click += new System.EventHandler(this.setDataToolStripMenuItem_Click);
            this.setDataToolStripMenuItem.MouseLeave += new System.EventHandler(this.SetDataMouseLeaveEvent);
            this.setDataToolStripMenuItem.MouseMove += new System.Windows.Forms.MouseEventHandler(this.SetDataMouseMoveEvent);
            // 
            // contextMenuStrip1
            // 
            resources.ApplyResources(this.contextMenuStrip1, "contextMenuStrip1");
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sourceToolStripMenuItem,
            this.toolStripTextBoxSource,
            this.nameToolStripMenuItem,
            this.toolStripTextBoxName,
            this.labelToolStripMenuItem,
            this.toolStripTextBoxLabel,
            this.showToolStripMenuItem,
            this.toolStripComboBoxShow,
            this.toolStripSeparator1,
            this.colorToolStripMenuItem,
            this.toolStripTextBoxColor,
            this.setDataToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            // 
            // form_process_diagram
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.chart1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "form_process_diagram";
            this.Load += new System.EventHandler(this.form_process_diagram_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.CMSinsertNode.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.contextMenuStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ContextMenuStrip CMSinsertNode;
        private System.Windows.Forms.ToolStripMenuItem insertNodeToolStripMenuItem;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tb1_Instruction;
        private System.Windows.Forms.ToolStripMenuItem sourceToolStripMenuItem;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxSource;
        private System.Windows.Forms.ToolStripMenuItem nameToolStripMenuItem;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxName;
        private System.Windows.Forms.ToolStripMenuItem labelToolStripMenuItem;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxLabel;
        private System.Windows.Forms.ToolStripMenuItem showToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBoxShow;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem colorToolStripMenuItem;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxColor;
        private System.Windows.Forms.ToolStripMenuItem setDataToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
    }
}