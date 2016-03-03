namespace WFA_psychometric_chart
{
    partial class Form4_insert_data
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
            this.tb_country = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tb_state = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tb_city = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tb_street = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_ZIP = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.btn_insert_data = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tbn_cancel = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.tb_elevation = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tb_longitude = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.tb_latitude = new System.Windows.Forms.TextBox();
            this.btn_pull_online_data = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // tb_country
            // 
            this.tb_country.Location = new System.Drawing.Point(326, 71);
            this.tb_country.Name = "tb_country";
            this.tb_country.Size = new System.Drawing.Size(200, 20);
            this.tb_country.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(259, 77);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Country :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(259, 103);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "State :";
            // 
            // tb_state
            // 
            this.tb_state.Location = new System.Drawing.Point(326, 97);
            this.tb_state.Name = "tb_state";
            this.tb_state.Size = new System.Drawing.Size(200, 20);
            this.tb_state.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(259, 129);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(30, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "City :";
            // 
            // tb_city
            // 
            this.tb_city.Location = new System.Drawing.Point(326, 123);
            this.tb_city.Name = "tb_city";
            this.tb_city.Size = new System.Drawing.Size(200, 20);
            this.tb_city.TabIndex = 4;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(259, 155);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Street :";
            // 
            // tb_street
            // 
            this.tb_street.Location = new System.Drawing.Point(326, 149);
            this.tb_street.Name = "tb_street";
            this.tb_street.Size = new System.Drawing.Size(200, 20);
            this.tb_street.TabIndex = 6;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(259, 181);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(30, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "ZIP :";
            // 
            // tb_ZIP
            // 
            this.tb_ZIP.Location = new System.Drawing.Point(326, 175);
            this.tb_ZIP.Name = "tb_ZIP";
            this.tb_ZIP.Size = new System.Drawing.Size(200, 20);
            this.tb_ZIP.TabIndex = 8;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(539, 78);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(11, 13);
            this.label6.TabIndex = 10;
            this.label6.Text = "*";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(539, 130);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(11, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "*";
            // 
            // btn_insert_data
            // 
            this.btn_insert_data.Enabled = false;
            this.btn_insert_data.Location = new System.Drawing.Point(451, 312);
            this.btn_insert_data.Name = "btn_insert_data";
            this.btn_insert_data.Size = new System.Drawing.Size(89, 23);
            this.btn_insert_data.TabIndex = 12;
            this.btn_insert_data.Text = "Insert Data";
            this.btn_insert_data.UseVisualStyleBackColor = true;
            this.btn_insert_data.Click += new System.EventHandler(this.button1_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(599, 228);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(11, 13);
            this.label8.TabIndex = 13;
            this.label8.Text = "*";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(616, 227);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(85, 13);
            this.label9.TabIndex = 14;
            this.label9.Text = "= Required input";
            // 
            // tbn_cancel
            // 
            this.tbn_cancel.Location = new System.Drawing.Point(262, 312);
            this.tbn_cancel.Name = "tbn_cancel";
            this.tbn_cancel.Size = new System.Drawing.Size(89, 23);
            this.tbn_cancel.TabIndex = 15;
            this.tbn_cancel.Text = "Cancel";
            this.tbn_cancel.UseVisualStyleBackColor = true;
            this.tbn_cancel.Click += new System.EventHandler(this.button2_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(259, 259);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(57, 13);
            this.label10.TabIndex = 21;
            this.label10.Text = "Elevation :";
            // 
            // tb_elevation
            // 
            this.tb_elevation.Location = new System.Drawing.Point(326, 253);
            this.tb_elevation.Name = "tb_elevation";
            this.tb_elevation.ReadOnly = true;
            this.tb_elevation.Size = new System.Drawing.Size(200, 20);
            this.tb_elevation.TabIndex = 20;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(259, 233);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(60, 13);
            this.label11.TabIndex = 19;
            this.label11.Text = "Longitude :";
            // 
            // tb_longitude
            // 
            this.tb_longitude.Location = new System.Drawing.Point(326, 227);
            this.tb_longitude.Name = "tb_longitude";
            this.tb_longitude.ReadOnly = true;
            this.tb_longitude.Size = new System.Drawing.Size(200, 20);
            this.tb_longitude.TabIndex = 18;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(259, 207);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(51, 13);
            this.label12.TabIndex = 17;
            this.label12.Text = "Latitude :";
            // 
            // tb_latitude
            // 
            this.tb_latitude.Location = new System.Drawing.Point(326, 201);
            this.tb_latitude.Name = "tb_latitude";
            this.tb_latitude.ReadOnly = true;
            this.tb_latitude.Size = new System.Drawing.Size(200, 20);
            this.tb_latitude.TabIndex = 16;
            // 
            // btn_pull_online_data
            // 
            this.btn_pull_online_data.Location = new System.Drawing.Point(356, 312);
            this.btn_pull_online_data.Name = "btn_pull_online_data";
            this.btn_pull_online_data.Size = new System.Drawing.Size(89, 23);
            this.btn_pull_online_data.TabIndex = 22;
            this.btn_pull_online_data.Text = "Pull online data";
            this.btn_pull_online_data.UseVisualStyleBackColor = true;
            this.btn_pull_online_data.Click += new System.EventHandler(this.button3_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(539, 182);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(11, 13);
            this.label13.TabIndex = 23;
            this.label13.Text = "*";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(539, 208);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(15, 13);
            this.label14.TabIndex = 24;
            this.label14.Text = "**";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(539, 233);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(15, 13);
            this.label15.TabIndex = 25;
            this.label15.Text = "**";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(539, 259);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(15, 13);
            this.label16.TabIndex = 26;
            this.label16.Text = "**";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(595, 256);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(15, 13);
            this.label17.TabIndex = 27;
            this.label17.Text = "**";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(616, 253);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(95, 13);
            this.label18.TabIndex = 28;
            this.label18.Text = "= Filled by program";
            // 
            // Form4_insert_data
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(859, 449);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.btn_pull_online_data);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.tb_elevation);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.tb_longitude);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.tb_latitude);
            this.Controls.Add(this.tbn_cancel);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.btn_insert_data);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tb_ZIP);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tb_street);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tb_city);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tb_state);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tb_country);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form4_insert_data";
            this.Text = "Insert Data";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tb_country;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_state;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tb_city;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tb_street;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tb_ZIP;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btn_insert_data;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button tbn_cancel;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox tb_elevation;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox tb_longitude;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox tb_latitude;
        private System.Windows.Forms.Button btn_pull_online_data;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
    }
}