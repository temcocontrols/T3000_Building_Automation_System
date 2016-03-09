namespace WFA_psychometric_chart
{
    partial class Form2
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btn_set_value = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.tb_long = new System.Windows.Forms.TextBox();
            this.tb_lat = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_city = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btn_set_value);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.tb_long);
            this.groupBox1.Controls.Add(this.tb_lat);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.tb_city);
            this.groupBox1.Location = new System.Drawing.Point(12, 53);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(386, 129);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "For Update Data Constantly";
            // 
            // btn_set_value
            // 
            this.btn_set_value.Enabled = false;
            this.btn_set_value.Location = new System.Drawing.Point(250, 91);
            this.btn_set_value.Name = "btn_set_value";
            this.btn_set_value.Size = new System.Drawing.Size(89, 23);
            this.btn_set_value.TabIndex = 7;
            this.btn_set_value.Text = "Set Values";
            this.btn_set_value.UseVisualStyleBackColor = true;
            this.btn_set_value.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(140, 91);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(89, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Get Geo values";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // tb_long
            // 
            this.tb_long.Location = new System.Drawing.Point(291, 53);
            this.tb_long.Name = "tb_long";
            this.tb_long.ReadOnly = true;
            this.tb_long.Size = new System.Drawing.Size(89, 20);
            this.tb_long.TabIndex = 5;
            // 
            // tb_lat
            // 
            this.tb_lat.Location = new System.Drawing.Point(140, 53);
            this.tb_lat.Name = "tb_lat";
            this.tb_lat.ReadOnly = true;
            this.tb_lat.Size = new System.Drawing.Size(89, 20);
            this.tb_lat.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(235, 56);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Longitude:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(85, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Latitude :";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(114, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Enter city [city,country]";
            // 
            // tb_city
            // 
            this.tb_city.Location = new System.Drawing.Point(140, 19);
            this.tb_city.Name = "tb_city";
            this.tb_city.Size = new System.Drawing.Size(240, 20);
            this.tb_city.TabIndex = 0;
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(449, 211);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form2";
            this.Text = "New Feature";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btn_set_value;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox tb_long;
        private System.Windows.Forms.TextBox tb_lat;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_city;
    }
}