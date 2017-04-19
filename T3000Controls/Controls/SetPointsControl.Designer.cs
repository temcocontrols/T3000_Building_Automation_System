namespace T3000Controls
{
    partial class SetPointsControl
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
            this.panel1TwoSliderModeCheckBox = new System.Windows.Forms.CheckBox();
            this.topValueLabel = new System.Windows.Forms.Label();
            this.bottomValueLabel = new System.Windows.Forms.Label();
            this.panel2 = new T3000Controls.SliderControl();
            this.panel = new T3000Controls.SliderControl();
            this.slider1BottomZoneCheckBox = new System.Windows.Forms.CheckBox();
            this.slider1TopZoneCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // panel1TwoSliderModeCheckBox
            // 
            this.panel1TwoSliderModeCheckBox.Location = new System.Drawing.Point(200, 0);
            this.panel1TwoSliderModeCheckBox.Name = "panel1TwoSliderModeCheckBox";
            this.panel1TwoSliderModeCheckBox.Size = new System.Drawing.Size(100, 20);
            this.panel1TwoSliderModeCheckBox.TabIndex = 5;
            this.panel1TwoSliderModeCheckBox.Text = "TwoSliderMode";
            this.panel1TwoSliderModeCheckBox.UseVisualStyleBackColor = true;
            this.panel1TwoSliderModeCheckBox.CheckedChanged += new System.EventHandler(this.panel1TwoSliderModeCheckBox_CheckedChanged);
            // 
            // topValueLabel
            // 
            this.topValueLabel.Location = new System.Drawing.Point(120, 60);
            this.topValueLabel.Name = "topValueLabel";
            this.topValueLabel.Size = new System.Drawing.Size(50, 20);
            this.topValueLabel.TabIndex = 6;
            // 
            // bottomValueLabel
            // 
            this.bottomValueLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bottomValueLabel.Location = new System.Drawing.Point(120, 480);
            this.bottomValueLabel.Name = "bottomValueLabel";
            this.bottomValueLabel.Size = new System.Drawing.Size(50, 20);
            this.bottomValueLabel.TabIndex = 7;
            // 
            // panel2
            // 
            this.panel2.AdditionalText = " F";
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.panel2.BackColor = System.Drawing.Color.Transparent;
            this.panel2.BackgroundWidth = 20;
            this.panel2.BorderColor = System.Drawing.Color.Black;
            this.panel2.BottomValue = -100F;
            this.panel2.BottomZone = true;
            this.panel2.BottomZoneColor = System.Drawing.Color.Red;
            this.panel2.BottomZoneValue = -50F;
            this.panel2.CurrentValue = -5F;
            this.panel2.CurrentValueColor = System.Drawing.Color.Black;
            this.panel2.EnableIndicator = true;
            this.panel2.HandlesBorderColor = System.Drawing.Color.White;
            this.panel2.HandlesHeight = 8;
            this.panel2.IndicatorBorderColor = System.Drawing.Color.Black;
            this.panel2.IndicatorColor = System.Drawing.Color.GreenYellow;
            this.panel2.IndicatorWidth = 20;
            this.panel2.IndicatorHeight = 20;
            this.panel2.IndicatorText = "";
            this.panel2.IsSimpleIndicator = true;
            this.panel2.LinesColor = System.Drawing.Color.LightGray;
            this.panel2.Location = new System.Drawing.Point(300, 80);
            this.panel2.MiddleHandleColor = System.Drawing.Color.GreenYellow;
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(120, 400);
            this.panel2.StepValue = 10F;
            this.panel2.TabIndex = 4;
            this.panel2.TopValue = 100F;
            this.panel2.TopZone = true;
            this.panel2.TopZoneColor = System.Drawing.Color.DeepSkyBlue;
            this.panel2.TopZoneValue = 50F;
            this.panel2.TwoSliderMode = false;
            // 
            // panel
            // 
            this.panel.AdditionalText = " C";
            this.panel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.panel.BackColor = System.Drawing.Color.Transparent;
            this.panel.BackgroundWidth = 20;
            this.panel.BorderColor = System.Drawing.Color.Black;
            this.panel.BottomValue = -100F;
            this.panel.BottomZone = true;
            this.panel.BottomZoneColor = System.Drawing.Color.Red;
            this.panel.BottomZoneValue = -50F;
            this.panel.CurrentValue = -5F;
            this.panel.CurrentValueColor = System.Drawing.Color.Black;
            this.panel.EnableIndicator = true;
            this.panel.HandlesBorderColor = System.Drawing.Color.White;
            this.panel.HandlesHeight = 8;
            this.panel.IndicatorBorderColor = System.Drawing.Color.Black;
            this.panel.IndicatorColor = System.Drawing.Color.GreenYellow;
            this.panel.IndicatorWidth = 200;
            this.panel.IndicatorHeight = 30;
            this.panel.IndicatorText = "Temp";
            this.panel.IsSimpleIndicator = false;
            this.panel.LinesColor = System.Drawing.Color.LightGray;
            this.panel.Location = new System.Drawing.Point(0, 80);
            this.panel.MiddleHandleColor = System.Drawing.Color.GreenYellow;
            this.panel.Name = "panel";
            this.panel.Size = new System.Drawing.Size(300, 400);
            this.panel.StepValue = 10F;
            this.panel.TabIndex = 2;
            this.panel.TopValue = 100F;
            this.panel.TopZone = true;
            this.panel.TopZoneColor = System.Drawing.Color.DeepSkyBlue;
            this.panel.TopZoneValue = 50F;
            this.panel.TwoSliderMode = false;
            // 
            // slider1BottomZoneCheckBox
            // 
            this.slider1BottomZoneCheckBox.BackColor = System.Drawing.Color.Red;
            this.slider1BottomZoneCheckBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.slider1BottomZoneCheckBox.Checked = true;
            this.slider1BottomZoneCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.slider1BottomZoneCheckBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.slider1BottomZoneCheckBox.Location = new System.Drawing.Point(200, 50);
            this.slider1BottomZoneCheckBox.Name = "slider1BottomZoneCheckBox";
            this.slider1BottomZoneCheckBox.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.slider1BottomZoneCheckBox.Size = new System.Drawing.Size(100, 20);
            this.slider1BottomZoneCheckBox.TabIndex = 10;
            this.slider1BottomZoneCheckBox.Text = "-50.0 C";
            this.slider1BottomZoneCheckBox.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.slider1BottomZoneCheckBox.UseVisualStyleBackColor = false;
            this.slider1BottomZoneCheckBox.CheckedChanged += new System.EventHandler(this.slider1BottomValueCheckBox_CheckedChanged);
            // 
            // slider1TopZoneCheckBox
            // 
            this.slider1TopZoneCheckBox.BackColor = System.Drawing.Color.DeepSkyBlue;
            this.slider1TopZoneCheckBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.slider1TopZoneCheckBox.Checked = true;
            this.slider1TopZoneCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.slider1TopZoneCheckBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.slider1TopZoneCheckBox.Location = new System.Drawing.Point(200, 24);
            this.slider1TopZoneCheckBox.Name = "slider1TopZoneCheckBox";
            this.slider1TopZoneCheckBox.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.slider1TopZoneCheckBox.Size = new System.Drawing.Size(100, 20);
            this.slider1TopZoneCheckBox.TabIndex = 11;
            this.slider1TopZoneCheckBox.Text = "50.0 C";
            this.slider1TopZoneCheckBox.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.slider1TopZoneCheckBox.UseVisualStyleBackColor = false;
            this.slider1TopZoneCheckBox.CheckedChanged += new System.EventHandler(this.slider1TopZoneCheckBox_CheckedChanged);
            // 
            // SetPointsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.slider1TopZoneCheckBox);
            this.Controls.Add(this.slider1BottomZoneCheckBox);
            this.Controls.Add(this.bottomValueLabel);
            this.Controls.Add(this.topValueLabel);
            this.Controls.Add(this.panel1TwoSliderModeCheckBox);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel);
            this.Name = "SetPointsControl";
            this.Size = new System.Drawing.Size(1000, 500);
            this.ResumeLayout(false);

        }

        #endregion
        private SliderControl panel;
        private SliderControl panel2;
        private System.Windows.Forms.CheckBox panel1TwoSliderModeCheckBox;
        private System.Windows.Forms.Label topValueLabel;
        private System.Windows.Forms.Label bottomValueLabel;
        private System.Windows.Forms.CheckBox slider1BottomZoneCheckBox;
        private System.Windows.Forms.CheckBox slider1TopZoneCheckBox;
    }
}
