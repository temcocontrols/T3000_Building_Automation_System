namespace T3000Controls
{
    using System.ComponentModel;
    using System.Drawing;
    using System.Drawing.Drawing2D;
    using System.Windows.Forms;

    internal partial class IndicatorControl : Label
    {
        #region DesignerProperties

        private bool _isSimple = true;
        [Description("Simple version of indicator"), Category("Appearance")]
        public bool IsSimple
        {
            get { return _isSimple; }
            set
            {
                _isSimple = value;

                Invalidate();
            }
        }

        private string _indicatorText = "";
        [Description("Indicator text"), Category("Appearance")]
        public string IndicatorText
        {
            get { return _indicatorText; }
            set
            {
                _indicatorText = value;

                Invalidate();
            }
        }

        private float _value = 0.0F;
        [Description("Value for indicator"), Category("Appearance")]
        public float Value
        {
            get { return _value; }
            set
            {
                _value = value;

                Invalidate();
            }
        }

        private string _additionalText = " F";
        [Description("Additional text for value"), Category("Appearance")]
        public string AdditionalText
        {
            get { return _additionalText; }
            set
            {
                _additionalText = value;

                Invalidate();
            }
        }

        private Color _borderColor = Color.Black;
        [Description("Color for border"), Category("Appearance")]
        public Color BorderColor
        {
            get { return _borderColor; }
            set
            {
                _borderColor = value;

                Invalidate();
            }
        }

        #endregion

        public IndicatorControl()
        {
            InitializeComponent();

            ResizeRedraw = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Text = IsSimple ? "" : $"{IndicatorText} {_value.ToString("F1")}{AdditionalText}";

            var path = new GraphicsPath();
            if (IsSimple)
            {
                path.AddPolygon(new[] {
                    new PointF(0, 0),
                    new PointF(Width - 1, Height / 2.0F),
                    new PointF(0, Height - 1),
                });
            }
            else
            {
                path.AddPolygon(new[] {
                    new PointF(0, 0),
                    new PointF(Width - Height, 0),
                    new PointF(Width - 1, Height / 2.0F),
                    new PointF(Width - Height, Height - 1),
                    new PointF(0, Height - 1),
                });
            }

            using (var pen = new Pen(BorderColor, 1))
            {
                e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
                e.Graphics.DrawPath(pen, path);
            }

            Region = GraphicsUtilities.GetRegionForPath(path);
        }
    }
}
