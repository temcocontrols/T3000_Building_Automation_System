namespace WFA_psychometric_chart
{
    partial class FormMessageBox
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
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btnGoBack = new System.Windows.Forms.Button();
            this.btnRunDefaultSetting = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // richTextBox1
            // 
            this.richTextBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.richTextBox1.Location = new System.Drawing.Point(1, 12);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Size = new System.Drawing.Size(286, 87);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "Building location information is not Provided. Please input the building location" +
    " information  in T3000";
            // 
            // btnGoBack
            // 
            this.btnGoBack.Location = new System.Drawing.Point(12, 117);
            this.btnGoBack.Name = "btnGoBack";
            this.btnGoBack.Size = new System.Drawing.Size(126, 36);
            this.btnGoBack.TabIndex = 2;
            this.btnGoBack.Text = "Go back to T300";
            this.btnGoBack.UseVisualStyleBackColor = true;
            this.btnGoBack.Click += new System.EventHandler(this.btnGoBack_Click);
            // 
            // btnRunDefaultSetting
            // 
            this.btnRunDefaultSetting.Location = new System.Drawing.Point(144, 117);
            this.btnRunDefaultSetting.Name = "btnRunDefaultSetting";
            this.btnRunDefaultSetting.Size = new System.Drawing.Size(126, 36);
            this.btnRunDefaultSetting.TabIndex = 3;
            this.btnRunDefaultSetting.Text = "Run with Defult Building";
            this.btnRunDefaultSetting.UseVisualStyleBackColor = true;
            this.btnRunDefaultSetting.Click += new System.EventHandler(this.btnRunDefaultSetting_Click);
            // 
            // FormMessageBox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(298, 192);
            this.Controls.Add(this.btnRunDefaultSetting);
            this.Controls.Add(this.btnGoBack);
            this.Controls.Add(this.richTextBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMessageBox";
            this.Text = "FormMessageBox";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button btnGoBack;
        private System.Windows.Forms.Button btnRunDefaultSetting;
    }
}