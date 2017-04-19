namespace T3000Controls
{
    partial class SliderControl
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
            this.indicator = new T3000Controls.IndicatorControl();
            this.middleHandle = new T3000Controls.HandleControl();
            this.bottomHandle = new T3000Controls.HandleControl();
            this.topHandle = new T3000Controls.HandleControl();
            this.backgroundControl = new T3000Controls.BackgroundControl();
            this.SuspendLayout();
            // 
            // indicator
            // 
            this.indicator.AdditionalText = " F";
            this.indicator.BackColor = System.Drawing.Color.GreenYellow;
            this.indicator.BorderColor = System.Drawing.Color.Black;
            this.indicator.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.indicator.IndicatorText = "";
            this.indicator.IsSimple = true;
            this.indicator.Location = new System.Drawing.Point(0, 40);
            this.indicator.Name = "indicator";
            this.indicator.Size = new System.Drawing.Size(20, 20);
            this.indicator.TabIndex = 5;
            this.indicator.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.indicator.Value = 50F;
            // 
            // middleHandle
            // 
            this.middleHandle.AdditionalText = " F";
            this.middleHandle.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.middleHandle.BackColor = System.Drawing.Color.GreenYellow;
            this.middleHandle.BorderColor = System.Drawing.Color.White;
            this.middleHandle.HandleHeight = 8;
            this.middleHandle.HandleWidth = 50;
            this.middleHandle.Location = new System.Drawing.Point(20, 42);
            this.middleHandle.Name = "middleHandle";
            this.middleHandle.Size = new System.Drawing.Size(100, 16);
            this.middleHandle.TabIndex = 4;
            this.middleHandle.Value = 50F;
            this.middleHandle.MouseDown += new System.Windows.Forms.MouseEventHandler(this.handle_MouseDown);
            this.middleHandle.MouseMove += new System.Windows.Forms.MouseEventHandler(this.oneSliderMode_MouseMove);
            this.middleHandle.MouseUp += new System.Windows.Forms.MouseEventHandler(this.middleHandle_MouseUp);
            // 
            // bottomHandle
            // 
            this.bottomHandle.AdditionalText = " F";
            this.bottomHandle.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.bottomHandle.BackColor = System.Drawing.Color.Red;
            this.bottomHandle.BorderColor = System.Drawing.Color.White;
            this.bottomHandle.HandleHeight = 8;
            this.bottomHandle.HandleWidth = 50;
            this.bottomHandle.Location = new System.Drawing.Point(20, 58);
            this.bottomHandle.Name = "bottomHandle";
            this.bottomHandle.Size = new System.Drawing.Size(100, 16);
            this.bottomHandle.TabIndex = 1;
            this.bottomHandle.Value = 66F;
            this.bottomHandle.MouseDown += new System.Windows.Forms.MouseEventHandler(this.handle_MouseDown);
            this.bottomHandle.MouseMove += new System.Windows.Forms.MouseEventHandler(this.bottomHandle_MouseMove);
            this.bottomHandle.MouseUp += new System.Windows.Forms.MouseEventHandler(this.bottomHandle_MouseUp);
            // 
            // topHandle
            // 
            this.topHandle.AdditionalText = " F";
            this.topHandle.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.topHandle.BackColor = System.Drawing.Color.DeepSkyBlue;
            this.topHandle.BorderColor = System.Drawing.Color.White;
            this.topHandle.HandleHeight = 8;
            this.topHandle.HandleWidth = 50;
            this.topHandle.Location = new System.Drawing.Point(20, 25);
            this.topHandle.Name = "topHandle";
            this.topHandle.Size = new System.Drawing.Size(100, 16);
            this.topHandle.TabIndex = 3;
            this.topHandle.Value = 33F;
            this.topHandle.MouseDown += new System.Windows.Forms.MouseEventHandler(this.handle_MouseDown);
            this.topHandle.MouseMove += new System.Windows.Forms.MouseEventHandler(this.topHandle_MouseMove);
            this.topHandle.MouseUp += new System.Windows.Forms.MouseEventHandler(this.topHandle_MouseUp);
            // 
            // backgroundControl
            // 
            this.backgroundControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.backgroundControl.BackColor = System.Drawing.Color.GreenYellow;
            this.backgroundControl.BigOffsetY = 10F;
            this.backgroundControl.BorderColor = System.Drawing.Color.Black;
            this.backgroundControl.BottomZone = true;
            this.backgroundControl.BottomZoneColor = System.Drawing.Color.Red;
            this.backgroundControl.BottomZoneValueY = 66F;
            this.backgroundControl.CurrentValueColor = System.Drawing.Color.Black;
            this.backgroundControl.CurrentValueY = 50F;
            this.backgroundControl.LinesColor = System.Drawing.Color.LightGray;
            this.backgroundControl.Location = new System.Drawing.Point(20, 0);
            this.backgroundControl.Name = "backgroundControl";
            this.backgroundControl.Size = new System.Drawing.Size(50, 100);
            this.backgroundControl.SmallOffsetY = 5F;
            this.backgroundControl.StepHeight = 10F;
            this.backgroundControl.TabIndex = 2;
            this.backgroundControl.TopZone = true;
            this.backgroundControl.TopZoneColor = System.Drawing.Color.DeepSkyBlue;
            this.backgroundControl.TopZoneValueY = 33F;
            // 
            // SliderControl
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.indicator);
            this.Controls.Add(this.middleHandle);
            this.Controls.Add(this.bottomHandle);
            this.Controls.Add(this.topHandle);
            this.Controls.Add(this.backgroundControl);
            this.DoubleBuffered = true;
            this.Name = "SliderControl";
            this.Size = new System.Drawing.Size(120, 100);
            this.ResumeLayout(false);

        }

        #endregion

        private HandleControl topHandle;
        private HandleControl bottomHandle;
        private BackgroundControl backgroundControl;
        private HandleControl middleHandle;
        private IndicatorControl indicator;
    }
}
