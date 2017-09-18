namespace PH_App
{
    partial class PHCalculator
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PHCalculator));
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tb_atm_pressure = new System.Windows.Forms.TextBox();
            this.tbEnthalpy = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.unit_temp = new System.Windows.Forms.Label();
            this.unit_pressure = new System.Windows.Forms.Label();
            this.unit_enthalpy = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.radioButton2.Location = new System.Drawing.Point(89, 5);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(35, 17);
            this.radioButton2.TabIndex = 16;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "IP";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.radioButton1.Location = new System.Drawing.Point(49, 5);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(35, 17);
            this.radioButton1.TabIndex = 15;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "SI";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label12.Location = new System.Drawing.Point(12, 9);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(32, 13);
            this.label12.TabIndex = 14;
            this.label12.Text = "Unit :";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(103, 31);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 0;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // tb_atm_pressure
            // 
            this.tb_atm_pressure.Location = new System.Drawing.Point(104, 65);
            this.tb_atm_pressure.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.tb_atm_pressure.Name = "tb_atm_pressure";
            this.tb_atm_pressure.Size = new System.Drawing.Size(100, 20);
            this.tb_atm_pressure.TabIndex = 2;
            this.tb_atm_pressure.Text = "1";
            this.tb_atm_pressure.TextChanged += new System.EventHandler(this.tb_atm_pressure_TextChanged);
            // 
            // tbEnthalpy
            // 
            this.tbEnthalpy.Location = new System.Drawing.Point(103, 103);
            this.tbEnthalpy.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.tbEnthalpy.Name = "tbEnthalpy";
            this.tbEnthalpy.ReadOnly = true;
            this.tbEnthalpy.Size = new System.Drawing.Size(100, 20);
            this.tbEnthalpy.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.SlateGray;
            this.label1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label1.Location = new System.Drawing.Point(1, 34);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(98, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Temperature(DBT):";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.SlateGray;
            this.label3.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label3.Location = new System.Drawing.Point(35, 68);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = " Pressure:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.ForeColor = System.Drawing.Color.SlateGray;
            this.label11.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.label11.Location = new System.Drawing.Point(32, 107);
            this.label11.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(57, 13);
            this.label11.TabIndex = 21;
            this.label11.Text = "Enthalphy:";
            // 
            // unit_temp
            // 
            this.unit_temp.AutoSize = true;
            this.unit_temp.ForeColor = System.Drawing.Color.SlateGray;
            this.unit_temp.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.unit_temp.Location = new System.Drawing.Point(211, 33);
            this.unit_temp.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.unit_temp.Name = "unit_temp";
            this.unit_temp.Size = new System.Drawing.Size(18, 13);
            this.unit_temp.TabIndex = 22;
            this.unit_temp.Text = "°C";
            // 
            // unit_pressure
            // 
            this.unit_pressure.AutoSize = true;
            this.unit_pressure.ForeColor = System.Drawing.Color.SlateGray;
            this.unit_pressure.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.unit_pressure.Location = new System.Drawing.Point(211, 72);
            this.unit_pressure.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.unit_pressure.Name = "unit_pressure";
            this.unit_pressure.Size = new System.Drawing.Size(29, 13);
            this.unit_pressure.TabIndex = 24;
            this.unit_pressure.Text = "MPa";
            // 
            // unit_enthalpy
            // 
            this.unit_enthalpy.AutoSize = true;
            this.unit_enthalpy.ForeColor = System.Drawing.Color.SlateGray;
            this.unit_enthalpy.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.unit_enthalpy.Location = new System.Drawing.Point(211, 107);
            this.unit_enthalpy.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.unit_enthalpy.Name = "unit_enthalpy";
            this.unit_enthalpy.Size = new System.Drawing.Size(37, 13);
            this.unit_enthalpy.TabIndex = 32;
            this.unit_enthalpy.Text = "KJ/Kg";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.unit_enthalpy);
            this.groupBox1.Controls.Add(this.unit_pressure);
            this.groupBox1.Controls.Add(this.unit_temp);
            this.groupBox1.Controls.Add(this.label11);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.tbEnthalpy);
            this.groupBox1.Controls.Add(this.tb_atm_pressure);
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.ForeColor = System.Drawing.Color.SlateGray;
            this.groupBox1.Location = new System.Drawing.Point(6, 26);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.groupBox1.Size = new System.Drawing.Size(268, 160);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Calculations";
            // 
            // PHCalculator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(289, 224);
            this.Controls.Add(this.radioButton2);
            this.Controls.Add(this.radioButton1);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "PHCalculator";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PHCalculator";
            this.Load += new System.EventHandler(this.PHCalculator_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox tb_atm_pressure;
        private System.Windows.Forms.TextBox tbEnthalpy;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label unit_temp;
        private System.Windows.Forms.Label unit_pressure;
        private System.Windows.Forms.Label unit_enthalpy;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}