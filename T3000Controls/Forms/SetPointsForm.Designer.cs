namespace T3000Controls.Forms
{
    partial class SetPointsForm
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
            this.setPointsControl1 = new T3000Controls.SetPointsControl();
            this.SuspendLayout();
            // 
            // setPointsControl1
            // 
            this.setPointsControl1.AdditionalText = " C";
            this.setPointsControl1.BackColor = System.Drawing.Color.Transparent;
            this.setPointsControl1.BottomValue = -100F;
            this.setPointsControl1.BottomZoneValue = -30F;
            this.setPointsControl1.CurrentValue = 0F;
            this.setPointsControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.setPointsControl1.Location = new System.Drawing.Point(0, 0);
            this.setPointsControl1.Name = "setPointsControl1";
            this.setPointsControl1.Size = new System.Drawing.Size(846, 505);
            this.setPointsControl1.TabIndex = 0;
            this.setPointsControl1.TopValue = 100F;
            this.setPointsControl1.TopZoneValue = 50F;
            // 
            // SetPointsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(846, 505);
            this.Controls.Add(this.setPointsControl1);
            this.Name = "SetPointsForm";
            this.Text = "SetPointsForm";
            this.ResumeLayout(false);

        }

        #endregion

        private SetPointsControl setPointsControl1;
    }
}