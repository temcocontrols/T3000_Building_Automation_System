namespace WFA_psychometric_chart
{
    partial class form_export_data
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.dtp_To = new System.Windows.Forms.DateTimePicker();
            this.dtp_From = new System.Windows.Forms.DateTimePicker();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.rb3_txt = new System.Windows.Forms.RadioButton();
            this.rb2_csv = new System.Windows.Forms.RadioButton();
            this.rb1_excel = new System.Windows.Forms.RadioButton();
            this.btnExtract = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.cb1_select_data = new System.Windows.Forms.ComboBox();
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "From :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "To :";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.dtp_To);
            this.groupBox1.Controls.Add(this.dtp_From);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Enabled = false;
            this.groupBox1.Location = new System.Drawing.Point(20, 109);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(251, 91);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Date";
            // 
            // dtp_To
            // 
            this.dtp_To.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtp_To.Location = new System.Drawing.Point(64, 51);
            this.dtp_To.Name = "dtp_To";
            this.dtp_To.Size = new System.Drawing.Size(134, 20);
            this.dtp_To.TabIndex = 3;
            // 
            // dtp_From
            // 
            this.dtp_From.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtp_From.Location = new System.Drawing.Point(64, 22);
            this.dtp_From.Name = "dtp_From";
            this.dtp_From.Size = new System.Drawing.Size(134, 20);
            this.dtp_From.TabIndex = 2;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.rb3_txt);
            this.groupBox2.Controls.Add(this.rb2_csv);
            this.groupBox2.Controls.Add(this.rb1_excel);
            this.groupBox2.Enabled = false;
            this.groupBox2.Location = new System.Drawing.Point(20, 206);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(251, 100);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Format";
            // 
            // rb3_txt
            // 
            this.rb3_txt.AutoSize = true;
            this.rb3_txt.Location = new System.Drawing.Point(24, 65);
            this.rb3_txt.Name = "rb3_txt";
            this.rb3_txt.Size = new System.Drawing.Size(36, 17);
            this.rb3_txt.TabIndex = 2;
            this.rb3_txt.Text = "txt";
            this.rb3_txt.UseVisualStyleBackColor = true;
            // 
            // rb2_csv
            // 
            this.rb2_csv.AutoSize = true;
            this.rb2_csv.Location = new System.Drawing.Point(24, 42);
            this.rb2_csv.Name = "rb2_csv";
            this.rb2_csv.Size = new System.Drawing.Size(46, 17);
            this.rb2_csv.TabIndex = 1;
            this.rb2_csv.Text = "CSV";
            this.rb2_csv.UseVisualStyleBackColor = true;
            // 
            // rb1_excel
            // 
            this.rb1_excel.AutoSize = true;
            this.rb1_excel.Checked = true;
            this.rb1_excel.Location = new System.Drawing.Point(24, 19);
            this.rb1_excel.Name = "rb1_excel";
            this.rb1_excel.Size = new System.Drawing.Size(72, 17);
            this.rb1_excel.TabIndex = 0;
            this.rb1_excel.TabStop = true;
            this.rb1_excel.Text = "Excel(.xls)";
            this.rb1_excel.UseVisualStyleBackColor = true;
            // 
            // btnExtract
            // 
            this.btnExtract.Enabled = false;
            this.btnExtract.Location = new System.Drawing.Point(196, 322);
            this.btnExtract.Name = "btnExtract";
            this.btnExtract.Size = new System.Drawing.Size(75, 23);
            this.btnExtract.TabIndex = 4;
            this.btnExtract.Text = "Extract";
            this.btnExtract.UseVisualStyleBackColor = true;
            this.btnExtract.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.cb1_select_data);
            this.groupBox3.Location = new System.Drawing.Point(20, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(251, 79);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Select a building location";
            // 
            // cb1_select_data
            // 
            this.cb1_select_data.FormattingEnabled = true;
            this.cb1_select_data.Location = new System.Drawing.Point(24, 33);
            this.cb1_select_data.Name = "cb1_select_data";
            this.cb1_select_data.Size = new System.Drawing.Size(174, 21);
            this.cb1_select_data.TabIndex = 0;
            this.cb1_select_data.SelectedIndexChanged += new System.EventHandler(this.cb1_select_data_SelectedIndexChanged);
            // 
            // form_export_data
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(303, 357);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.btnExtract);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "form_export_data";
            this.Text = "Export Data";
            this.Load += new System.EventHandler(this.form_extract_data_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DateTimePicker dtp_To;
        private System.Windows.Forms.DateTimePicker dtp_From;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton rb3_txt;
        private System.Windows.Forms.RadioButton rb2_csv;
        private System.Windows.Forms.RadioButton rb1_excel;
        private System.Windows.Forms.Button btnExtract;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox cb1_select_data;
        private System.Windows.Forms.SaveFileDialog saveFD;
    }
}