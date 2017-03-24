using System.Windows.Forms;

namespace T3000Controls
{
    partial class HandleControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.valueLabel = new T3000Controls.TransparentLabel();
            this.SuspendLayout();
            // 
            // valueLabel
            // 
            this.valueLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.valueLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.valueLabel.Location = new System.Drawing.Point(54, 0);
            this.valueLabel.Name = "valueLabel";
            this.valueLabel.Size = new System.Drawing.Size(39, 13);
            this.valueLabel.TabIndex = 1;
            this.valueLabel.Text = "Text";
            this.valueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // HandleControl
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.valueLabel);
            this.DoubleBuffered = true;
            this.Name = "HandleControl";
            this.Size = new System.Drawing.Size(94, 16);
            this.ResumeLayout(false);

        }

        #endregion

        private TransparentLabel valueLabel;
    }
}
