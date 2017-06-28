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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMessageBox));
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btnGoBack = new System.Windows.Forms.Button();
            this.btnRunDefaultSetting = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // richTextBox1
            // 
            resources.ApplyResources(this.richTextBox1, "richTextBox1");
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            // 
            // btnGoBack
            // 
            resources.ApplyResources(this.btnGoBack, "btnGoBack");
            this.btnGoBack.Name = "btnGoBack";
            this.btnGoBack.UseVisualStyleBackColor = true;
            this.btnGoBack.Click += new System.EventHandler(this.btnGoBack_Click);
            // 
            // btnRunDefaultSetting
            // 
            resources.ApplyResources(this.btnRunDefaultSetting, "btnRunDefaultSetting");
            this.btnRunDefaultSetting.Name = "btnRunDefaultSetting";
            this.btnRunDefaultSetting.UseVisualStyleBackColor = true;
            this.btnRunDefaultSetting.Click += new System.EventHandler(this.btnRunDefaultSetting_Click);
            // 
            // FormMessageBox
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnRunDefaultSetting);
            this.Controls.Add(this.btnGoBack);
            this.Controls.Add(this.richTextBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMessageBox";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button btnGoBack;
        private System.Windows.Forms.Button btnRunDefaultSetting;
    }
}