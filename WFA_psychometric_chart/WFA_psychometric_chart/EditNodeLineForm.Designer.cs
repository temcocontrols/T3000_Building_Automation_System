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
<<<<<<< HEAD
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditNodeLineForm));
=======
>>>>>>> origin/master
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
<<<<<<< HEAD
            resources.ApplyResources(this.dataGridView1, "dataGridView1");
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersVisible = false;
=======
            this.dataGridView1.Location = new System.Drawing.Point(3, 25);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.Size = new System.Drawing.Size(907, 220);
            this.dataGridView1.TabIndex = 1;
>>>>>>> origin/master
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            // 
            // nodeID
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.nodeID, "nodeID");
            this.nodeID.Name = "nodeID";
            this.nodeID.ReadOnly = true;
            // 
            // temp
            // 
            resources.ApplyResources(this.temp, "temp");
=======
            this.nodeID.HeaderText = "Node ID";
            this.nodeID.Name = "nodeID";
            this.nodeID.ReadOnly = true;
            this.nodeID.Visible = false;
            // 
            // temp
            // 
            this.temp.HeaderText = "Temperature";
>>>>>>> origin/master
            this.temp.Name = "temp";
            // 
            // humidity
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.humidity, "humidity");
=======
            this.humidity.HeaderText = "Humidity";
>>>>>>> origin/master
            this.humidity.Name = "humidity";
            // 
            // equivalentEnthalpy
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.equivalentEnthalpy, "equivalentEnthalpy");
=======
            this.equivalentEnthalpy.HeaderText = "Equivalent Enthalpy";
>>>>>>> origin/master
            this.equivalentEnthalpy.Name = "equivalentEnthalpy";
            this.equivalentEnthalpy.ReadOnly = true;
            // 
            // showText
            // 
            this.showText.DisplayStyle = System.Windows.Forms.DataGridViewComboBoxDisplayStyle.ComboBox;
<<<<<<< HEAD
            resources.ApplyResources(this.showText, "showText");
=======
            this.showText.HeaderText = "Show Text";
>>>>>>> origin/master
            this.showText.Items.AddRange(new object[] {
            "Name",
            "Source",
            "Label"});
            this.showText.Name = "showText";
            // 
            // name
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.name, "name");
=======
            this.name.HeaderText = "Name";
>>>>>>> origin/master
            this.name.Name = "name";
            // 
            // source
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.source, "source");
=======
            this.source.HeaderText = "Source";
>>>>>>> origin/master
            this.source.Name = "source";
            this.source.ReadOnly = true;
            // 
            // label
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.label, "label");
=======
            this.label.HeaderText = "Label";
>>>>>>> origin/master
            this.label.Name = "label";
            // 
            // color
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.color, "color");
=======
            this.color.HeaderText = "Color";
>>>>>>> origin/master
            this.color.Name = "color";
            this.color.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.color.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // nodeSize
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.nodeSize, "nodeSize");
=======
            this.nodeSize.HeaderText = "Node Size";
>>>>>>> origin/master
            this.nodeSize.Name = "nodeSize";
            // 
            // label1
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
=======
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
>>>>>>> origin/master
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
<<<<<<< HEAD
            resources.ApplyResources(this.dataGridView2, "dataGridView2");
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowHeadersVisible = false;
=======
            this.dataGridView2.Location = new System.Drawing.Point(12, 276);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowHeadersVisible = false;
            this.dataGridView2.Size = new System.Drawing.Size(907, 220);
            this.dataGridView2.TabIndex = 3;
>>>>>>> origin/master
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            // 
            // lineName
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.lineName, "lineName");
=======
            this.lineName.HeaderText = "Line Name";
>>>>>>> origin/master
            this.lineName.Name = "lineName";
            this.lineName.ReadOnly = true;
            // 
            // startNodeID
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.startNodeID, "startNodeID");
=======
            this.startNodeID.HeaderText = "Start Node ID";
>>>>>>> origin/master
            this.startNodeID.Name = "startNodeID";
            this.startNodeID.ReadOnly = true;
            // 
            // endNodeID
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.endNodeID, "endNodeID");
=======
            this.endNodeID.HeaderText = "End Node ID";
>>>>>>> origin/master
            this.endNodeID.Name = "endNodeID";
            this.endNodeID.ReadOnly = true;
            // 
            // linecolor
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.linecolor, "linecolor");
=======
            this.linecolor.HeaderText = "Color";
>>>>>>> origin/master
            this.linecolor.Name = "linecolor";
            this.linecolor.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.linecolor.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // thickness
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.thickness, "thickness");
=======
            this.thickness.HeaderText = "Thickness";
>>>>>>> origin/master
            this.thickness.Name = "thickness";
            // 
            // button1
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.button1, "button1");
            this.button1.Name = "button1";
=======
            this.button1.Location = new System.Drawing.Point(777, 502);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(142, 36);
            this.button1.TabIndex = 5;
            this.button1.Text = "Save Edited Value";
>>>>>>> origin/master
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
<<<<<<< HEAD
            resources.ApplyResources(this.button2, "button2");
            this.button2.Name = "button2";
=======
            this.button2.Location = new System.Drawing.Point(629, 502);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(142, 36);
            this.button2.TabIndex = 6;
            this.button2.Text = "Cancel";
>>>>>>> origin/master
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label3
            // 
<<<<<<< HEAD
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
=======
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(294, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(388, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Note : Node ID and Equivalent Enthalpy are non-editable other fields are editable" +
    "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(303, 254);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(435, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Note : Line Name,Start Node ID and End Node ID are non-editable other fields are " +
    "editable";
            // 
            // EditNodeLineForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(936, 538);
>>>>>>> origin/master
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
<<<<<<< HEAD
=======
            this.Text = "Edit Node Line";
>>>>>>> origin/master
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