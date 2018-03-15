namespace T3000.Controls.MultipleMonthCalendarControls
{
    partial class MonthControl
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
            this.titleButton = new System.Windows.Forms.Button();
            this.daysOfWeekLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // titleButton
            // 
            this.titleButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(219)))), ((int)(((byte)(238)))), ((int)(((byte)(244)))));
            this.titleButton.Dock = System.Windows.Forms.DockStyle.Top;
            this.titleButton.FlatAppearance.BorderSize = 0;
            this.titleButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.titleButton.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(87)))), ((int)(((byte)(108)))), ((int)(((byte)(113)))));
            this.titleButton.Location = new System.Drawing.Point(0, 0);
            this.titleButton.Name = "titleButton";
            this.titleButton.Size = new System.Drawing.Size(176, 23);
            this.titleButton.TabIndex = 0;
            this.titleButton.Text = "Month Year";
            this.titleButton.UseVisualStyleBackColor = false;
            // 
            // daysOfWeekLabel
            // 
            this.daysOfWeekLabel.AutoSize = true;
            this.daysOfWeekLabel.Location = new System.Drawing.Point(3, 26);
            this.daysOfWeekLabel.Name = "daysOfWeekLabel";
            this.daysOfWeekLabel.Size = new System.Drawing.Size(171, 13);
            this.daysOfWeekLabel.TabIndex = 1;
            this.daysOfWeekLabel.Text = "Sun  Mon  Tue  Wed  Thu  Fri  Sat";
            // 
            // OneCalendar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.daysOfWeekLabel);
            this.Controls.Add(this.titleButton);
            this.Name = "OneCalendar";
            this.Size = new System.Drawing.Size(176, 144);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button titleButton;
        private System.Windows.Forms.Label daysOfWeekLabel;
    }
}
