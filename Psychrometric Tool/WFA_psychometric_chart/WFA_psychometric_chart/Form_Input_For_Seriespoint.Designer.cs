namespace WFA_psychometric_chart
{
    partial class Form_Input_For_Seriespoint
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Input_For_Seriespoint));
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.btnSetNode = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.btnColor = new System.Windows.Forms.Button();
            this.tbLabel = new System.Windows.Forms.TextBox();
            this.tbName = new System.Windows.Forms.TextBox();
            this.tbSource = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tb_line_name = new System.Windows.Forms.TextBox();
            this.lb_line_name = new System.Windows.Forms.Label();
            this.lb_line_enabled = new System.Windows.Forms.Label();
            this.cb_line_enabled = new System.Windows.Forms.CheckBox();
            this.gb_line_info = new System.Windows.Forms.GroupBox();
            this.gb_line_info.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnSetNode
            // 
            resources.ApplyResources(this.btnSetNode, "btnSetNode");
            this.btnSetNode.Name = "btnSetNode";
            this.btnSetNode.UseVisualStyleBackColor = true;
            this.btnSetNode.Click += new System.EventHandler(this.btnSetNode_Click);
            // 
            // btnCancel
            // 
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            resources.GetString("comboBox1.Items"),
            resources.GetString("comboBox1.Items1"),
            resources.GetString("comboBox1.Items2")});
            resources.ApplyResources(this.comboBox1, "comboBox1");
            this.comboBox1.Name = "comboBox1";
            // 
            // btnColor
            // 
            resources.ApplyResources(this.btnColor, "btnColor");
            this.btnColor.Name = "btnColor";
            this.btnColor.UseVisualStyleBackColor = true;
            this.btnColor.Click += new System.EventHandler(this.btnColor_Click);
            // 
            // tbLabel
            // 
            resources.ApplyResources(this.tbLabel, "tbLabel");
            this.tbLabel.Name = "tbLabel";
            // 
            // tbName
            // 
            resources.ApplyResources(this.tbName, "tbName");
            this.tbName.Name = "tbName";
            // 
            // tbSource
            // 
            resources.ApplyResources(this.tbSource, "tbSource");
            this.tbSource.Name = "tbSource";
            this.tbSource.ReadOnly = true;
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
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
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.Name = "label6";
            // 
            // tb_line_name
            // 
            resources.ApplyResources(this.tb_line_name, "tb_line_name");
            this.tb_line_name.Name = "tb_line_name";
            // 
            // lb_line_name
            // 
            resources.ApplyResources(this.lb_line_name, "lb_line_name");
            this.lb_line_name.Name = "lb_line_name";
            // 
            // lb_line_enabled
            // 
            resources.ApplyResources(this.lb_line_enabled, "lb_line_enabled");
            this.lb_line_enabled.Name = "lb_line_enabled";
            // 
            // cb_line_enabled
            // 
            resources.ApplyResources(this.cb_line_enabled, "cb_line_enabled");
            this.cb_line_enabled.Name = "cb_line_enabled";
            this.cb_line_enabled.UseVisualStyleBackColor = true;
            // 
            // gb_line_info
            // 
            this.gb_line_info.Controls.Add(this.lb_line_name);
            this.gb_line_info.Controls.Add(this.cb_line_enabled);
            this.gb_line_info.Controls.Add(this.tb_line_name);
            this.gb_line_info.Controls.Add(this.lb_line_enabled);
            resources.ApplyResources(this.gb_line_info, "gb_line_info");
            this.gb_line_info.Name = "gb_line_info";
            this.gb_line_info.TabStop = false;
            // 
            // Form_Input_For_Seriespoint
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gb_line_info);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.btnSetNode);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.btnColor);
            this.Controls.Add(this.tbLabel);
            this.Controls.Add(this.tbName);
            this.Controls.Add(this.tbSource);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form_Input_For_Seriespoint";
            this.Load += new System.EventHandler(this.Form_Input_For_Seriespoint_Load);
            this.gb_line_info.ResumeLayout(false);
            this.gb_line_info.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.Button btnSetNode;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button btnColor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbSource;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.TextBox tbLabel;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tb_line_name;
        private System.Windows.Forms.Label lb_line_name;
        private System.Windows.Forms.Label lb_line_enabled;
        private System.Windows.Forms.CheckBox cb_line_enabled;
        private System.Windows.Forms.GroupBox gb_line_info;
    }
}