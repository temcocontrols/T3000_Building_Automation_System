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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(form_export_data));
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
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
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
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.dtp_To);
            this.groupBox1.Controls.Add(this.dtp_From);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // dtp_To
            // 
            this.dtp_To.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            resources.ApplyResources(this.dtp_To, "dtp_To");
            this.dtp_To.Name = "dtp_To";
            // 
            // dtp_From
            // 
            this.dtp_From.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            resources.ApplyResources(this.dtp_From, "dtp_From");
            this.dtp_From.Name = "dtp_From";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.rb3_txt);
            this.groupBox2.Controls.Add(this.rb2_csv);
            this.groupBox2.Controls.Add(this.rb1_excel);
            resources.ApplyResources(this.groupBox2, "groupBox2");
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.TabStop = false;
            // 
            // rb3_txt
            // 
            resources.ApplyResources(this.rb3_txt, "rb3_txt");
            this.rb3_txt.Name = "rb3_txt";
            this.rb3_txt.UseVisualStyleBackColor = true;
            // 
            // rb2_csv
            // 
            resources.ApplyResources(this.rb2_csv, "rb2_csv");
            this.rb2_csv.Name = "rb2_csv";
            this.rb2_csv.UseVisualStyleBackColor = true;
            // 
            // rb1_excel
            // 
            resources.ApplyResources(this.rb1_excel, "rb1_excel");
            this.rb1_excel.Checked = true;
            this.rb1_excel.Name = "rb1_excel";
            this.rb1_excel.TabStop = true;
            this.rb1_excel.UseVisualStyleBackColor = true;
            // 
            // btnExtract
            // 
            resources.ApplyResources(this.btnExtract, "btnExtract");
            this.btnExtract.Name = "btnExtract";
            this.btnExtract.UseVisualStyleBackColor = true;
            this.btnExtract.Click += new System.EventHandler(this.button1_Click);
            // 
            // form_export_data
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnExtract);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "form_export_data";
            this.Load += new System.EventHandler(this.form_extract_data_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
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
        private System.Windows.Forms.SaveFileDialog saveFD;
    }
}