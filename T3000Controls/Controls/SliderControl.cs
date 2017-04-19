namespace T3000Controls
{
    using System;
    using System.ComponentModel;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;
    using System.Drawing;

    [Guid("ABA068FC-6B49-3031-B74A-1C51A3C8833A")]
    [ClassInterface(ClassInterfaceType.None)]
    [ComSourceInterfaces(typeof(ISliderControlEvents))]
    public partial class SliderControl : UserControl, ISliderControl
    {
        #region Properties

        private float _topValue = 0;
        [Description("Top value"), Category("Slider")]
        public float TopValue
        {
            get { return _topValue; }
            set
            {
                _topValue = value;
                OnTopValueChanged(value);

                Invalidate();
            }
        }

        private float _bottomValue = 100;
        [Description("Bottom value"), Category("Slider")]
        public float BottomValue
        {
            get { return _bottomValue; }
            set
            {
                _bottomValue = value;
                OnBottomValueChanged(value);

                Invalidate();
            }
        }

        private float _currentValue = 50;
        [Description("Current value"), Category("Slider")]
        public float CurrentValue
        {
            get { return _currentValue; }
            set 
            {
                _currentValue = value;
                indicator.Value = value;
                OnCurrentValueChanged(value);

                Invalidate();
            }
        }

        [Description("Top zone"), Category("Slider")]
        public bool TopZone
        {
            get { return backgroundControl.TopZone; }
            set
            {
                backgroundControl.TopZone = value;
                topHandle.Visible = value;
                middleHandle.Visible = OneSliderMode;
            }
        }

        [Description("Bottom zone"), Category("Slider")]
        public bool BottomZone
        {
            get { return backgroundControl.BottomZone; }
            set
            {
                backgroundControl.BottomZone = value;
                bottomHandle.Visible = value;
                middleHandle.Visible = OneSliderMode;
            }
        }

        private float _topZoneValue = 33;
        [Description("Top zone value"), Category("Slider")]
        public float TopZoneValue
        {
            get { return _topZoneValue; }
            set
            {
                _topZoneValue = value;
                if (TopZone)
                {
                    OnTopZoneValueChanged(value);
                }
                topHandle.Value = value;
                if (MiddleZoneValueAsAverage)
                    MiddleZoneValue = AverageZoneValue;

                Invalidate();
            }
        }

        private float _bottomZoneValue = 66;
        [Description("Bottom zone value"), Category("Slider")]
        public float BottomZoneValue
        {
            get { return _bottomZoneValue; }
            set
            {
                _bottomZoneValue = value;
                if (BottomZone)
                {
                    OnBottomZoneValueChanged(value);
                }
                bottomHandle.Value = value;
                if (MiddleZoneValueAsAverage)
                    MiddleZoneValue = AverageZoneValue;

                Invalidate();
            }
        }

        private float _middleZoneValue = 50.0F;
        [Description("Bottom zone value"), Category("Slider")]
        public float MiddleZoneValue
        {
            get { return _middleZoneValue; }
            set
            {
                _middleZoneValue = value;
                if (middleHandle.Visible)
                {
                    OnMiddleZoneValueChanged(value);
                }
                middleHandle.Value = value;

                Invalidate();
            }
        }

        private float _stepValue = 10;
        [Description("Step value"), Category("Slider")]
        public float StepValue
        {
            get { return _stepValue; }
            set
            {
                _stepValue = value;
                Invalidate();
            }
        }

        private bool _twoSliderMode = false;
        [Description("Two handle control mode"), Category("Slider")]
        public bool TwoSliderMode
        {
            get { return _twoSliderMode; }
            set
            {
                _twoSliderMode = value;
                middleHandle.Visible = OneSliderMode;

                Invalidate();
            }
        }

        private string _additionalText = " F";
        [Description("Additional text for handles and for indicator"), Category("Slider")]
        public string AdditionalText
        {
            get { return _additionalText; }
            set
            {
                _additionalText = value;
                topHandle.AdditionalText = value;
                middleHandle.AdditionalText = value;
                bottomHandle.AdditionalText = value;
                indicator.AdditionalText = value;
                
                Invalidate();
            }
        }

        [Description("Color for lines"), Category("Background")]
        public Color LinesColor
        {
            get { return backgroundControl.LinesColor; }
            set { backgroundControl.LinesColor = value; }
        }

        [Description("Color for borders"), Category("Background")]
        public Color BorderColor
        {
            get { return backgroundControl.BorderColor; }
            set { backgroundControl.BorderColor = value; }
        }

        [Description("Color for top zone"), Category("Background")]
        public Color TopZoneColor
        {
            get { return backgroundControl.TopZoneColor; }
            set
            {
                backgroundControl.TopZoneColor = value;
                topHandle.BackColor = value;
            }
        }

        [Description("Color for bottom zone"), Category("Background")]
        public Color BottomZoneColor
        {
            get { return backgroundControl.BottomZoneColor; }
            set
            {
                backgroundControl.BottomZoneColor = value;
                bottomHandle.BackColor = value;
            }
        }

        [Description("Color for current value"), Category("Background")]
        public Color CurrentValueColor
        {
            get { return backgroundControl.CurrentValueColor; }
            set { backgroundControl.CurrentValueColor = value; }
        }

        [Description("Width for background panel"), Category("Background")]
        public int BackgroundWidth
        {
            get { return backgroundControl.Width; }
            set
            {
                backgroundControl.Width = value;
                topHandle.HandleWidth = value;
                middleHandle.HandleWidth = value;
                bottomHandle.HandleWidth = value;

                Invalidate();
            }
        }

        [Description("Height for handles"), Category("Handles")]
        public int HandlesHeight
        {
            get { return topHandle.HandleHeight; }
            set
            {
                topHandle.HandleHeight = value;
                middleHandle.HandleHeight = value;
                bottomHandle.HandleHeight = value;
                topHandle.Height = value * 2;
                middleHandle.Height = value * 2;
                bottomHandle.Height = value * 2;

                Invalidate();
            }
        }

        [Description("Color for handles border"), Category("Handles")]
        public Color HandlesBorderColor
        {
            get { return topHandle.BorderColor; }
            set
            {
                topHandle.BorderColor = value;
                middleHandle.BorderColor = value;
                bottomHandle.BorderColor = value;
            }
        }

        [Description("Color for handles border"), Category("Handles")]
        public Color MiddleHandleColor
        {
            get { return middleHandle.BackColor; }
            set { middleHandle.BackColor = value; }
        }

        [Description("Low event mode for handles"), Category("Handles")]
        public bool LowEventMode { get; set; } = true;

        [Description("Automatical set middle handle value as average"), Category("Handles")]
        public bool MiddleZoneValueAsAverage { get; set; }

        [Description("Color for indicator"), Category("Indicator")]
        public Color IndicatorColor
        {
            get { return indicator.BackColor; }
            set { indicator.BackColor = value; }
        }

        [Description("Color for indicator border"), Category("Indicator")]
        public Color IndicatorBorderColor
        {
            get { return indicator.BorderColor; }
            set { indicator.BorderColor = value; }
        }

        [Description("Width for indicator"), Category("Indicator")]
        public int IndicatorWidth
        {
            get { return indicator.Width; }
            set
            {
                indicator.Width = value;

                var x = EnableIndicator ? value : 0;
                backgroundControl.Left = x;
                topHandle.Left = x;
                middleHandle.Left = x;
                bottomHandle.Left = x;

                var handleWidth = EnableIndicator ? Width - value : Width;
                topHandle.Width = handleWidth;
                middleHandle.Width = handleWidth;
                bottomHandle.Width = handleWidth;
            }
        }

        [Description("Height for indicator"), Category("Indicator")]
        public int IndicatorHeight {
            get { return indicator.Height; }
            set { indicator.Height = value; }
        }

        private bool _enableIndicator = true;
        [Description("Enable/disable indicator for current value"), Category("Indicator")]
        public bool EnableIndicator
        {
            get { return _enableIndicator; }
            set
            {
                _enableIndicator = value;
                indicator.Visible = value;
                //Update positions and widths
                IndicatorWidth = IndicatorWidth;
            }
        }

        [Description("Indicator is simple"), Category("Indicator")]
        public bool IsSimpleIndicator
        {
            get { return indicator.IsSimple; }
            set
            {
                indicator.IsSimple = value;
                
                Invalidate();
            }
        }

        [Description("Indicator text"), Category("Indicator")]
        public string IndicatorText {
            get { return indicator.IndicatorText; }
            set { indicator.IndicatorText = value; }
        }

        #endregion

        #region PublicMethods

        [Browsable(false)]
        public bool IsInverse => TopValue > BottomValue;

        [Browsable(false)]
        public bool InTopZone => IsInverse ? CurrentValue >= TopZoneValue : CurrentValue <= TopZoneValue;

        [Browsable(false)]
        public bool InBottomZone => IsInverse ? CurrentValue <= BottomZoneValue : CurrentValue >= BottomZoneValue;

        [Browsable(false)]
        public string ValueToText(float value) => value.ToString("F1") + AdditionalText;

        [Browsable(false)]
        public string UnknownValueText = "-";

        [Browsable(false)]
        public string TopZoneText => TopZone ? ValueToText(TopZoneValue) : UnknownValueText;

        [Browsable(false)]
        public string BottomZoneText => BottomZone ? ValueToText(BottomZoneValue) : UnknownValueText;

        [Browsable(false)]
        public string MiddleZoneText => BottomZone && TopZone ? ValueToText(MiddleZoneValue) : UnknownValueText;

        [Browsable(false)]
        public string TopValueText => ValueToText(TopValue);

        [Browsable(false)]
        public string BottomValueText => ValueToText(BottomValue);

        [Browsable(false)]
        public string CurrentValueText => ValueToText(CurrentValue);

        [Browsable(false)]
        public float AverageZoneValue => (TopZoneValue + BottomZoneValue) / 2;

        [Browsable(false)]
        public bool OneSliderMode => !TwoSliderMode && TopZone && BottomZone;

        [Browsable(false)]
        public void SetRange(float topValue, float bottomValue)
        {
            TopValue = topValue;
            BottomValue = bottomValue;
        }

        [Browsable(false)]
        public void SetZoneValues(float topZoneValue, float bottomZoneValue)
        {
            TopZoneValue = topZoneValue;
            BottomZoneValue = bottomZoneValue;
        }

        [Browsable(false)]
        public void SetZoneValues(float topZoneValue, float middleZoneValue, float bottomZoneValue)
        {
            MiddleZoneValueAsAverage = false;

            TopZoneValue = topZoneValue;
            MiddleZoneValue = middleZoneValue;
            BottomZoneValue = bottomZoneValue;
        }

        #endregion

        #region Events

        [Description("Causes if top zone value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler TopZoneValueChanged;

        [Description("Causes if bottom zone value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler BottomZoneValueChanged;

        [Description("Causes if current value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler CurrentValueChanged;

        [Description("Causes if top value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler TopValueChanged;

        [Description("Causes if bottom value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler BottomValueChanged;

        [Description("Causes if middle zone value is changed"), Category("Slider")]
        public virtual event ValueChangedEventHandler MiddleZoneValueChanged;

        [Description("Causes if top handle is moved. Only in TwoSliderMode"), Category("Slider")]
        public virtual event ValueChangedEventHandler TopHandleMoved;

        [Description("Causes if middle handle is moved. Only in OneSliderMode"), Category("Slider")]
        public virtual event ValueChangedEventHandler MiddleHandleMoved;

        [Description("Causes if bottom handle is moved. Only in TwoSliderMode"), Category("Slider")]
        public virtual event ValueChangedEventHandler BottomHandleMoved;

        protected void OnTopZoneValueChanged(float value) =>
            TopZoneValueChanged?.Invoke(this, value);

        protected void OnBottomZoneValueChanged(float value) =>
            BottomZoneValueChanged?.Invoke(this, value);

        protected void OnCurrentValueChanged(float value) =>
            CurrentValueChanged?.Invoke(this, value);

        protected void OnTopValueChanged(float value) =>
            TopValueChanged?.Invoke(this, value);

        protected void OnBottomValueChanged(float value) =>
            BottomValueChanged?.Invoke(this, value);

        protected void OnMiddleZoneValueChanged(float value) =>
            MiddleZoneValueChanged?.Invoke(this, value);

        protected void OnTopHandleMoved(float value) =>
            TopHandleMoved?.Invoke(this, value);

        protected void OnMiddleHandleMoved(float value) =>
            MiddleHandleMoved?.Invoke(this, value);

        protected void OnBottomHandleMoved(float value) =>
            BottomHandleMoved?.Invoke(this, value);

        #endregion

        private MouseMover Mover { get; }

        public SliderControl()
        {
            InitializeComponent();
            
            ResizeRedraw = true;
            Mover = new MouseMover(this);
        }

        public float YToValue(float y)
        {
            return SliderUtilities.YToValue(y, TopValue, BottomValue, Height);
        }

        public float ValueToY(float value)
        {
            return SliderUtilities.ValueToY(value, TopValue, BottomValue, Height);
        }

        public float ValueToHeight(float value)
        {
            return SliderUtilities.ValueToHeight(value, TopValue, BottomValue, Height);
        }

        private float GetOffsetForValue(float value)
        {
            return SliderUtilities.GetOffsetForValue(value, TopValue, BottomValue, Height);
        }

        public float Clamp(float value)
        {
            var maxValue = Math.Max(BottomValue, TopValue);
            var minValue = Math.Min(BottomValue, TopValue);

            return value.Clamp(minValue, maxValue);
        }

        private void handle_MouseDown(object sender, MouseEventArgs e)
        {
            Mover.Start(sender, e);
        }

        private void handle_MouseUp(object sender, MouseEventArgs e)
        {
            Mover.End();
        }

        private void topHandle_MouseUp(object sender, MouseEventArgs e)
        {
            if (!TwoSliderMode)
            {
                middleHandle_MouseUp(sender, e);
                return;
            }

            handle_MouseUp(sender, e);
            TopZoneValue = topHandle.Value;
            OnTopHandleMoved(topHandle.Value);
        }

        private void middleHandle_MouseUp(object sender, MouseEventArgs e)
        {
            handle_MouseUp(sender, e);
            UpdateZoneValuesFromHandles();
            OnMiddleHandleMoved(middleHandle.Value);
        }

        private void bottomHandle_MouseUp(object sender, MouseEventArgs e)
        {
            if (!TwoSliderMode)
            {
                middleHandle_MouseUp(sender, e);
                return;
            }

            handle_MouseUp(sender, e);
            BottomZoneValue = bottomHandle.Value;
            OnBottomHandleMoved(bottomHandle.Value);
        }

        private void handle_MouseMove(object sender, MouseEventArgs e)
        {
            if (!Mover.IsMoved)
            {
                return;
            }

            var handle = sender as HandleControl;
            if (handle == null)
            {
                return;
            }

            var point = Mover.GetPoint(e);
            var value = YToValue(point.Y + handle.Height / 2.0F);

            handle.Value = Clamp(value);
        }

        private void topHandle_MouseMove(object sender, MouseEventArgs e)
        {
            if (!Mover.IsMoved)
            {
                return;
            }
            if (!TwoSliderMode)
            {
                oneSliderMode_MouseMove(sender, e);
                return;
            }
            handle_MouseMove(sender, e);

            //Restricts value from bottom handle value
            topHandle.Value = TopValue > BottomValue ?
                Math.Max(bottomHandle.Value, topHandle.Value) :
                Math.Min(bottomHandle.Value, topHandle.Value);

            if (!LowEventMode)
            {
                OnTopHandleMoved(topHandle.Value);
                TopZoneValue = topHandle.Value;
            }
            Refresh();
        }

        private void bottomHandle_MouseMove(object sender, MouseEventArgs e)
        {
            if (!Mover.IsMoved)
            {
                return;
            }
            if (!TwoSliderMode)
            {
                oneSliderMode_MouseMove(sender, e);
                return;
            }
            handle_MouseMove(sender, e);

            //Restricts value from top handle value
            bottomHandle.Value = TopValue > BottomValue ?
                Math.Min(bottomHandle.Value, topHandle.Value) :
                Math.Max(bottomHandle.Value, topHandle.Value);

            if (!LowEventMode)
            {
                OnBottomHandleMoved(bottomHandle.Value);
                BottomZoneValue = bottomHandle.Value;
            }
            Refresh();
        }

        private void oneSliderMode_MouseMove(object sender, MouseEventArgs e)
        {
            if (TwoSliderMode || !Mover.IsMoved)
            {
                return;
            }

            var handle = sender as HandleControl;
            if (handle == null)
            {
                return;
            }

            var prevValue = handle.Value;
            handle_MouseMove(sender, e);
            var delta = handle.Value - prevValue;
            handle.Value -= delta;

            topHandle.Value += delta;
            middleHandle.Value += delta;
            bottomHandle.Value += delta;

            if (!LowEventMode)
            {
                OnMiddleHandleMoved(middleHandle.Value);
                UpdateZoneValuesFromHandles();
            }

            Refresh();
        }

        private void UpdateZoneValuesFromHandles()
        {
            var temp = MiddleZoneValueAsAverage;
            MiddleZoneValueAsAverage = false;
            TopZoneValue = topHandle.Value;
            BottomZoneValue = bottomHandle.Value;
            MiddleZoneValueAsAverage = temp;
            MiddleZoneValue = MiddleZoneValueAsAverage ? AverageZoneValue : middleHandle.Value;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            
            //Update background control properties
            backgroundControl.TopZoneValueY = ValueToY(topHandle.Value);
            backgroundControl.BottomZoneValueY = ValueToY(bottomHandle.Value);
            backgroundControl.CurrentValueY = ValueToY(CurrentValue);
            backgroundControl.StepHeight = ValueToHeight(StepValue);
            backgroundControl.BigOffsetY = GetOffsetForValue(StepValue);
            backgroundControl.SmallOffsetY = backgroundControl.BigOffsetY + ValueToHeight(StepValue / 2);
            
            Func<float, Control, int> getYForHandleFromValue = (value, control) =>
                Convert.ToInt32(ValueToY(Clamp(value)) - control.Height / 2.0F);

            //Update indicator control Y position
            indicator.Top = getYForHandleFromValue(indicator.Value, indicator);

            //Update handles Y positions
            topHandle.Top = getYForHandleFromValue(topHandle.Value, topHandle);
            bottomHandle.Top = getYForHandleFromValue(bottomHandle.Value, bottomHandle);
            middleHandle.Top = getYForHandleFromValue(middleHandle.Value, middleHandle);

            backgroundControl.Refresh();
        }

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);
    }
}
