namespace T3000Controls
{
    using System;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;
    using System.ComponentModel;

    [Guid("A7CFC298-FE4E-4819-B868-2C014BA244DE")]
    [ClassInterface(ClassInterfaceType.None)]
    public partial class SetPointsControl : UserControl, ISetPointsControl
    {
        #region DesignerProperties

        private float _topValue = 0;
        [Description("Top value"), Category("SetPointsControl")]
        public float TopValue {
            get { return _topValue; }
            set {
                _topValue = value;

                UpdateTopBottomValues();
            }
        }

        private float _bottomValue = 100;
        [Description("Bottom value"), Category("SetPointsControl")]
        public float BottomValue {
            get { return _bottomValue; }
            set {
                _bottomValue = value;

                UpdateTopBottomValues();
            }
        }

        private float _currentValue = 50;
        [Description("Current value"), Category("SetPointsControl")]
        public float CurrentValue {
            get { return _currentValue; }
            set {
                _currentValue = value;
                panel.CurrentValue = value;
                panel2.CurrentValue = value;
            }
        }

        private float _topZoneValue = 33;
        [Description("Top zone value"), Category("SetPointsControl")]
        public float TopZoneValue {
            get { return _topZoneValue; }
            set {
                _topZoneValue = value;
                panel.TopZoneValue = value;
                panel2.TopZoneValue = value;

                Invalidate();
            }
        }

        private float _bottomZoneValue = 66;
        [Description("Bottom zone value"), Category("SetPointsControl")]
        public float BottomZoneValue {
            get { return _bottomZoneValue; }
            set {
                _bottomZoneValue = value;
                panel.BottomZoneValue = value;
                panel2.BottomZoneValue = value;

                Invalidate();
            }
        }

        private string _additionalText = " F";
        [Description("Additional text"), Category("SetPointsControl")]
        public string AdditionalText
        {
            get { return _additionalText; }
            set 
            {
                _additionalText = value;
                panel.AdditionalText = value;
                panel2.AdditionalText = value;
                slider1TopZoneCheckBox.Text = panel.TopZoneText;
                slider1BottomZoneCheckBox.Text = panel.BottomZoneText;

                Invalidate();
            }
        }

        #endregion

        public SetPointsControl()
        {
            InitializeComponent();
        }

        public void UpdateTopBottomValues()
        {
            var topValue = TopValue;
            var bottomValue = BottomValue;
            var delta = Math.Abs(TopValue - BottomValue); // % of this
            var multiplier = 0.05F; //5%
            var divider = 10; //to .. , 10, 20, .. , 80, 90, ..
            if (TopValue > BottomValue)
            {
                topValue = SliderUtilities.RoundUp(TopValue, delta, multiplier, divider);
                bottomValue = SliderUtilities.RoundDown(BottomValue, delta, multiplier, divider);
            }
            else
            {
                topValue = SliderUtilities.RoundDown(TopValue, delta, multiplier, divider);
                bottomValue = SliderUtilities.RoundUp(BottomValue, delta, multiplier, divider);
            }

            panel.TopValue = topValue;
            panel2.TopValue = topValue;

            panel.BottomValue = bottomValue;
            panel2.BottomValue = bottomValue;

            topValueLabel.Text = panel.TopValueText;
            bottomValueLabel.Text = panel.BottomValueText;
        }

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);

        private void panel1TwoSliderModeCheckBox_CheckedChanged(object sender, System.EventArgs e)
        {
            panel.TwoSliderMode = panel1TwoSliderModeCheckBox.Checked;

            panel.CurrentValue += new Random().Next() % 10 - 5;
        }

        private void panel_CurrentValueChanged(float newValue)
        {
            panel2.CurrentValue = newValue;
        }

        private void panel_BottomZoneValueChanged(float newValue)
        {
            panel2.BottomZoneValue = newValue;
        }

        private void panel_TopZoneValueChanged(float newValue)
        {
            panel2.TopZoneValue = newValue;
        }

        private void slider1BottomValueCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            panel.BottomZone = slider1BottomZoneCheckBox.Checked;
            slider1BottomZoneCheckBox.Text = panel.BottomZoneText;
        }

        private void slider1TopZoneCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            panel.TopZone = slider1TopZoneCheckBox.Checked;
            slider1TopZoneCheckBox.Text = panel.TopZoneText;
        }
    }
}
