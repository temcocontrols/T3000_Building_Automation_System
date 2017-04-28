namespace T3000Controls.Forms
{
    partial class Form1
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
            this.tCheckBox1 = new T3000Controls.TCheckBox();
            this.sliderControl1 = new T3000Controls.SliderControl();
            this.SuspendLayout();
            // 
            // tCheckBox1
            // 
            this.tCheckBox1.AutoSize = true;
            this.tCheckBox1.BackColor = System.Drawing.Color.White;
            this.tCheckBox1.CheckColor = System.Drawing.Color.Red;
            this.tCheckBox1.Checked = true;
            this.tCheckBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.tCheckBox1.Font = new System.Drawing.Font("Arial", 10F);
            this.tCheckBox1.ForeColor = System.Drawing.Color.Black;
            this.tCheckBox1.Location = new System.Drawing.Point(45, 131);
            this.tCheckBox1.Name = "tCheckBox1";
            this.tCheckBox1.Size = new System.Drawing.Size(96, 20);
            this.tCheckBox1.TabIndex = 0;
            this.tCheckBox1.TBackColor = System.Drawing.Color.White;
            this.tCheckBox1.TBold = false;
            this.tCheckBox1.TCaption = "TCheckBox";
            this.tCheckBox1.TChecked = true;
            this.tCheckBox1.Text = "tCheckBox1";
            this.tCheckBox1.TFontColor = System.Drawing.Color.Black;
            this.tCheckBox1.TFontSize = 10;
            this.tCheckBox1.TItalic = false;
            this.tCheckBox1.TShowPicture = false;
            this.tCheckBox1.TStrikeout = false;
            this.tCheckBox1.TUnderline = false;
            this.tCheckBox1.UseVisualStyleBackColor = true;
            this.tCheckBox1.CheckBoxClick += new System.EventHandler(this.tCheckBox1_CheckBoxClick);
            this.tCheckBox1.Click += new System.EventHandler(this.tCheckBox1_Click);
            // 
            // sliderControl1
            // 
            this.sliderControl1.AdditionalText = " F";
            this.sliderControl1.BackColor = System.Drawing.Color.Transparent;
            this.sliderControl1.BackgroundWidth = 50;
            this.sliderControl1.BorderColor = System.Drawing.Color.Black;
            this.sliderControl1.BottomValue = 100F;
            this.sliderControl1.BottomZone = true;
            this.sliderControl1.BottomZoneColor = System.Drawing.Color.Red;
            this.sliderControl1.BottomZoneValue = 66F;
            this.sliderControl1.CurrentValue = 50F;
            this.sliderControl1.CurrentValueColor = System.Drawing.Color.Black;
            this.sliderControl1.EnableIndicator = true;
            this.sliderControl1.HandlesBorderColor = System.Drawing.Color.White;
            this.sliderControl1.HandlesHeight = 8;
            this.sliderControl1.IndicatorBorderColor = System.Drawing.Color.Black;
            this.sliderControl1.IndicatorColor = System.Drawing.Color.GreenYellow;
            this.sliderControl1.IndicatorHeight = 20;
            this.sliderControl1.IndicatorText = "";
            this.sliderControl1.IndicatorWidth = 20;
            this.sliderControl1.IsSimpleIndicator = true;
            this.sliderControl1.LinesColor = System.Drawing.Color.LightGray;
            this.sliderControl1.Location = new System.Drawing.Point(138, 212);
            this.sliderControl1.LowEventMode = true;
            this.sliderControl1.MiddleHandleColor = System.Drawing.Color.GreenYellow;
            this.sliderControl1.MiddleZoneValue = 50F;
            this.sliderControl1.MiddleZoneValueAsAverage = false;
            this.sliderControl1.Name = "sliderControl1";
            this.sliderControl1.Size = new System.Drawing.Size(120, 100);
            this.sliderControl1.StepValue = 10F;
            this.sliderControl1.TabIndex = 1;
            this.sliderControl1.TopValue = 0F;
            this.sliderControl1.TopZone = true;
            this.sliderControl1.TopZoneColor = System.Drawing.Color.DeepSkyBlue;
            this.sliderControl1.TopZoneValue = 33F;
            this.sliderControl1.TwoSliderMode = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(556, 492);
            this.Controls.Add(this.sliderControl1);
            this.Controls.Add(this.tCheckBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TCheckBox tCheckBox1;
        private SliderControl sliderControl1;
    }
}