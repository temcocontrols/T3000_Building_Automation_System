namespace T3000.Controls
{
    partial class MultipleMonthCalendar
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
            this.nextButton = new System.Windows.Forms.Button();
            this.prevButton = new System.Windows.Forms.Button();
            this.monthsControl = new T3000.Controls.MonthsControl();
            this.SuspendLayout();
            // 
            // nextButton
            // 
            this.nextButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nextButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(219)))), ((int)(((byte)(238)))), ((int)(((byte)(244)))));
            this.nextButton.FlatAppearance.BorderSize = 0;
            this.nextButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.nextButton.Location = new System.Drawing.Point(272, 4);
            this.nextButton.Name = "nextButton";
            this.nextButton.Size = new System.Drawing.Size(24, 24);
            this.nextButton.TabIndex = 1;
            this.nextButton.Text = ">";
            this.nextButton.UseVisualStyleBackColor = false;
            // 
            // prevButton
            // 
            this.prevButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(219)))), ((int)(((byte)(238)))), ((int)(((byte)(244)))));
            this.prevButton.FlatAppearance.BorderSize = 0;
            this.prevButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.prevButton.Location = new System.Drawing.Point(4, 4);
            this.prevButton.Name = "prevButton";
            this.prevButton.Size = new System.Drawing.Size(24, 24);
            this.prevButton.TabIndex = 2;
            this.prevButton.Text = "<";
            this.prevButton.UseVisualStyleBackColor = false;
            // 
            // monthsControl
            // 
            this.monthsControl.DimensionX = 1;
            this.monthsControl.DimensionY = 1;
            this.monthsControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.monthsControl.Location = new System.Drawing.Point(0, 0);
            this.monthsControl.Name = "monthsControl";
            this.monthsControl.Size = new System.Drawing.Size(300, 300);
            this.monthsControl.StartDate = new System.DateTime(2017, 4, 8, 0, 0, 0, 0);
            this.monthsControl.TabIndex = 0;
            // 
            // MultipleMonthCalendar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.prevButton);
            this.Controls.Add(this.nextButton);
            this.Controls.Add(this.monthsControl);
            this.Name = "MultipleMonthCalendar";
            this.Size = new System.Drawing.Size(300, 300);
            this.ResumeLayout(false);

        }

        #endregion

        private MonthsControl monthsControl;
        private System.Windows.Forms.Button nextButton;
        private System.Windows.Forms.Button prevButton;
    }
}
