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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditNodeLineForm));
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.nodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.temp = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.humidity = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.equivalentEnthalpy = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.showText = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.source = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.color = new System.Windows.Forms.DataGridViewButtonColumn();
            this.nodeSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.lineName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.startNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.endNodeID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.linecolor = new System.Windows.Forms.DataGridViewButtonColumn();
            this.thickness = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
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
            this.nodeID,
            this.temp,
            this.humidity,
            this.equivalentEnthalpy,
            this.showText,
            this.name,
            this.source,
            this.label,
            this.color,
            this.nodeSize});
            resources.ApplyResources(this.dataGridView1, "dataGridView1");
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            // 
            // nodeID
            // 
            resources.ApplyResources(this.nodeID, "nodeID");
            this.nodeID.Name = "nodeID";
            this.nodeID.ReadOnly = true;
            // 
            // temp
            // 
            resources.ApplyResources(this.temp, "temp");
            this.temp.Name = "temp";
            // 
            // humidity
            // 
            resources.ApplyResources(this.humidity, "humidity");
            this.humidity.Name = "humidity";
            // 
            // equivalentEnthalpy
            // 
            resources.ApplyResources(this.equivalentEnthalpy, "equivalentEnthalpy");
            this.equivalentEnthalpy.Name = "equivalentEnthalpy";
            this.equivalentEnthalpy.ReadOnly = true;
            // 
            // showText
            // 
            this.showText.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
            resources.ApplyResources(this.showText, "showText");
            this.showText.Items.AddRange(new object[] {
            "Name",
            "Source",
            "Label"});
            this.showText.Name = "showText";
            // 
            // name
            // 
            resources.ApplyResources(this.name, "name");
            this.name.Name = "name";
            // 
            // source
            // 
            resources.ApplyResources(this.source, "source");
            this.source.Name = "source";
            this.source.ReadOnly = true;
            // 
            // label
            // 
            resources.ApplyResources(this.label, "label");
            this.label.Name = "label";
            // 
            // color
            // 
            resources.ApplyResources(this.color, "color");
            this.color.Name = "color";
            this.color.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.color.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // nodeSize
            // 
            resources.ApplyResources(this.nodeSize, "nodeSize");
            this.nodeSize.Name = "nodeSize";
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
            this.lineName,
            this.startNodeID,
            this.endNodeID,
            this.linecolor,
            this.thickness});
            resources.ApplyResources(this.dataGridView2, "dataGridView2");
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowHeadersVisible = false;
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            // 
            // lineName
            // 
            resources.ApplyResources(this.lineName, "lineName");
            this.lineName.Name = "lineName";
            this.lineName.ReadOnly = true;
            // 
            // startNodeID
            // 
            resources.ApplyResources(this.startNodeID, "startNodeID");
            this.startNodeID.Name = "startNodeID";
            this.startNodeID.ReadOnly = true;
            // 
            // endNodeID
            // 
            resources.ApplyResources(this.endNodeID, "endNodeID");
            this.endNodeID.Name = "endNodeID";
            this.endNodeID.ReadOnly = true;
            // 
            // linecolor
            // 
            resources.ApplyResources(this.linecolor, "linecolor");
            this.linecolor.Name = "linecolor";
            this.linecolor.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.linecolor.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // thickness
            // 
            resources.ApplyResources(this.thickness, "thickness");
            this.thickness.Name = "thickness";
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
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EditNodeLineForm";
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
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.DataGridViewTextBoxColumn lineName;
        private System.Windows.Forms.DataGridViewTextBoxColumn startNodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn endNodeID;
        private System.Windows.Forms.DataGridViewButtonColumn linecolor;
        private System.Windows.Forms.DataGridViewTextBoxColumn thickness;
        private System.Windows.Forms.DataGridViewTextBoxColumn nodeID;
        private System.Windows.Forms.DataGridViewTextBoxColumn temp;
        private System.Windows.Forms.DataGridViewTextBoxColumn humidity;
        private System.Windows.Forms.DataGridViewTextBoxColumn equivalentEnthalpy;
        private System.Windows.Forms.DataGridViewComboBoxColumn showText;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.DataGridViewTextBoxColumn source;
        private System.Windows.Forms.DataGridViewTextBoxColumn label;
        private System.Windows.Forms.DataGridViewButtonColumn color;
        private System.Windows.Forms.DataGridViewTextBoxColumn nodeSize;
    }
}