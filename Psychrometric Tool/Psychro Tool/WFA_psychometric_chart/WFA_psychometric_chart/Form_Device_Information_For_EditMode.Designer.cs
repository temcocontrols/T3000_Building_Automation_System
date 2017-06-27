namespace WFA_psychometric_chart
{
    partial class Form_Device_Information_For_EditMode
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Device_Information_For_EditMode));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.cb_enthalpy_or_humidity_input = new System.Windows.Forms.ComboBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.CB_param_hum = new System.Windows.Forms.ComboBox();
            this.CB_param_temp = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.CB_Device = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnSetNode = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.CB_Device);
            this.groupBox1.Controls.Add(this.label8);
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.cb_enthalpy_or_humidity_input);
            this.groupBox3.Controls.Add(this.checkBox2);
            this.groupBox3.Controls.Add(this.checkBox1);
            this.groupBox3.Controls.Add(this.CB_param_hum);
            this.groupBox3.Controls.Add(this.CB_param_temp);
            this.groupBox3.Controls.Add(this.label9);
            resources.ApplyResources(this.groupBox3, "groupBox3");
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.TabStop = false;
            // 
            // cb_enthalpy_or_humidity_input
            // 
            this.cb_enthalpy_or_humidity_input.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            resources.ApplyResources(this.cb_enthalpy_or_humidity_input, "cb_enthalpy_or_humidity_input");
            this.cb_enthalpy_or_humidity_input.FormattingEnabled = true;
            this.cb_enthalpy_or_humidity_input.Items.AddRange(new object[] {
            resources.GetString("cb_enthalpy_or_humidity_input.Items"),
            resources.GetString("cb_enthalpy_or_humidity_input.Items1")});
            this.cb_enthalpy_or_humidity_input.Name = "cb_enthalpy_or_humidity_input";
            // 
            // checkBox2
            // 
            resources.ApplyResources(this.checkBox2, "checkBox2");
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.UseVisualStyleBackColor = true;
            this.checkBox2.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // checkBox1
            // 
            resources.ApplyResources(this.checkBox1, "checkBox1");
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // CB_param_hum
            // 
            this.CB_param_hum.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            resources.ApplyResources(this.CB_param_hum, "CB_param_hum");
            this.CB_param_hum.FormattingEnabled = true;
            this.CB_param_hum.Name = "CB_param_hum";
            this.CB_param_hum.SelectedIndexChanged += new System.EventHandler(this.CB_param_hum_SelectedIndexChanged);
            // 
            // CB_param_temp
            // 
            this.CB_param_temp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            resources.ApplyResources(this.CB_param_temp, "CB_param_temp");
            this.CB_param_temp.FormattingEnabled = true;
            this.CB_param_temp.Name = "CB_param_temp";
            this.CB_param_temp.SelectedIndexChanged += new System.EventHandler(this.CB_param_temp_SelectedIndexChanged);
            // 
            // label9
            // 
            resources.ApplyResources(this.label9, "label9");
            this.label9.Name = "label9";
            // 
            // CB_Device
            // 
            this.CB_Device.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CB_Device.FormattingEnabled = true;
            resources.ApplyResources(this.CB_Device, "CB_Device");
            this.CB_Device.Name = "CB_Device";
            this.CB_Device.SelectedIndexChanged += new System.EventHandler(this.CB_Device_SelectedIndexChanged);
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // btnCancel
            // 
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnSetNode
            // 
            resources.ApplyResources(this.btnSetNode, "btnSetNode");
            this.btnSetNode.Name = "btnSetNode";
            this.btnSetNode.UseVisualStyleBackColor = true;
            this.btnSetNode.Click += new System.EventHandler(this.btnSetNode_Click);
            // 
            // Form_Device_Information_For_EditMode
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSetNode);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.Name = "Form_Device_Information_For_EditMode";
            this.Load += new System.EventHandler(this.Form_Device_Information_For_EditMode_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox cb_enthalpy_or_humidity_input;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.ComboBox CB_param_hum;
        private System.Windows.Forms.ComboBox CB_param_temp;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox CB_Device;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnSetNode;
    }
}