namespace WFA_psychometric_chart
{
    partial class Form_HumiditySourceDeviceInput
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnSetNode = new System.Windows.Forms.Button();
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
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnCancel.Location = new System.Drawing.Point(136, 195);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(61, 25);
            this.btnCancel.TabIndex = 59;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnSetNode
            // 
            this.btnSetNode.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnSetNode.Location = new System.Drawing.Point(214, 195);
            this.btnSetNode.Name = "btnSetNode";
            this.btnSetNode.Size = new System.Drawing.Size(61, 25);
            this.btnSetNode.TabIndex = 60;
            this.btnSetNode.Text = "Ok";
            this.btnSetNode.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.CB_Device);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Location = new System.Drawing.Point(-6, 41);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(296, 148);
            this.groupBox1.TabIndex = 58;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Device Detail Info";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.cb_enthalpy_or_humidity_input);
            this.groupBox3.Controls.Add(this.checkBox2);
            this.groupBox3.Controls.Add(this.checkBox1);
            this.groupBox3.Controls.Add(this.CB_param_hum);
            this.groupBox3.Controls.Add(this.CB_param_temp);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Enabled = false;
            this.groupBox3.Location = new System.Drawing.Point(6, 42);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(289, 100);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Select Parameter (any two)";
            // 
            // cb_enthalpy_or_humidity_input
            // 
            this.cb_enthalpy_or_humidity_input.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_enthalpy_or_humidity_input.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.cb_enthalpy_or_humidity_input.FormattingEnabled = true;
            this.cb_enthalpy_or_humidity_input.Items.AddRange(new object[] {
            "Humidity",
            "Enthalpy"});
            this.cb_enthalpy_or_humidity_input.Location = new System.Drawing.Point(27, 42);
            this.cb_enthalpy_or_humidity_input.Name = "cb_enthalpy_or_humidity_input";
            this.cb_enthalpy_or_humidity_input.Size = new System.Drawing.Size(82, 21);
            this.cb_enthalpy_or_humidity_input.TabIndex = 58;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.checkBox2.Location = new System.Drawing.Point(6, 49);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(15, 14);
            this.checkBox2.TabIndex = 57;
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.checkBox1.Location = new System.Drawing.Point(6, 22);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(15, 14);
            this.checkBox1.TabIndex = 56;
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // CB_param_hum
            // 
            this.CB_param_hum.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CB_param_hum.Enabled = false;
            this.CB_param_hum.FormattingEnabled = true;
            this.CB_param_hum.Location = new System.Drawing.Point(115, 46);
            this.CB_param_hum.Name = "CB_param_hum";
            this.CB_param_hum.Size = new System.Drawing.Size(160, 21);
            this.CB_param_hum.TabIndex = 5;
            // 
            // CB_param_temp
            // 
            this.CB_param_temp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CB_param_temp.Enabled = false;
            this.CB_param_temp.FormattingEnabled = true;
            this.CB_param_temp.Location = new System.Drawing.Point(115, 19);
            this.CB_param_temp.Name = "CB_param_temp";
            this.CB_param_temp.Size = new System.Drawing.Size(160, 21);
            this.CB_param_temp.TabIndex = 3;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label9.Location = new System.Drawing.Point(36, 22);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(73, 13);
            this.label9.TabIndex = 1;
            this.label9.Text = "Temperature :";
            // 
            // CB_Device
            // 
            this.CB_Device.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CB_Device.FormattingEnabled = true;
            this.CB_Device.Location = new System.Drawing.Point(121, 19);
            this.CB_Device.Name = "CB_Device";
            this.CB_Device.Size = new System.Drawing.Size(160, 21);
            this.CB_Device.TabIndex = 2;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label8.Location = new System.Drawing.Point(28, 19);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(87, 13);
            this.label8.TabIndex = 0;
            this.label8.Text = "Select a device :";
            // 
            // Form_HumiditySourceDeviceInput
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSetNode);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form_HumiditySourceDeviceInput";
            this.Text = "Form_HumiditySourceDeviceInput";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnSetNode;
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
    }
}