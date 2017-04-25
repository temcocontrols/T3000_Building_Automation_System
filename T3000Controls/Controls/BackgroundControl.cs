namespace T3000Controls
{
    using System.ComponentModel;
    using System.Drawing;
    using System.Windows.Forms;
    using System.Drawing.Drawing2D;

    internal partial class BackgroundControl : UserControl
    {
        #region DesignerProperties

        private Color _linesColor = Color.DarkGray;
        [Description("Color for lines"), Category("Appearance")]
        public Color LinesColor
        {
            get { return _linesColor; }
            set
            {
                _linesColor = value;
                Invalidate();
            }
        }

        private Color _borderColor = Color.Black;
        [Description("Color for borders"), Category("Appearance")]
        public Color BorderColor
        {
            get { return _borderColor; }
            set
            {
                _borderColor = value;
                Invalidate();
            }
        }

        private Color _topZoneColor = Color.DeepSkyBlue;
        [Description("Color for top zone"), Category("Appearance")]
        public Color TopZoneColor
        {
            get { return _topZoneColor; }
            set
            {
                _topZoneColor = value;
                Invalidate();
            }
        }

        private Color _bottomZoneColor = Color.Red;
        [Description("Color for bottom zone"), Category("Appearance")]
        public Color BottomZoneColor
        {
            get { return _bottomZoneColor; }
            set
            {
                _bottomZoneColor = value;
                Invalidate();
            }
        }

        private Color _currentValueColor = Color.Black;
        [Description("Color for current value"), Category("Appearance")]
        public Color CurrentValueColor
        {
            get { return _currentValueColor; }
            set
            {
                _currentValueColor = value;
                Invalidate();
            }
        }

        private bool _topZone = true;
        [Description("Top zone"), Category("Data")]
        public bool TopZone
        {
            get { return _topZone; }
            set
            {
                _topZone = value;
                Invalidate();
            }
        }

        private bool _bottomZone = true;
        [Description("Bottom zone"), Category("Data")]
        public bool BottomZone
        {
            get { return _bottomZone; }
            set
            {
                _bottomZone = value;
                Invalidate();
            }
        }

        private float _currentValueY = 50;
        [Description("Current value Y"), Category("Data")]
        public float CurrentValueY
        {
            get { return _currentValueY; }
            set
            {
                _currentValueY = value;
                Invalidate();
            }
        }

        private float _topZoneValueY = 33;
        [Description("Top zone value Y"), Category("Data")]
        public float TopZoneValueY
        {
            get { return _topZoneValueY; }
            set
            {
                _topZoneValueY = value;
                Invalidate();
            }
        }

        private float _bottomZoneValueY = 66;
        [Description("Bottom zone value Y"), Category("Data")]
        public float BottomZoneValueY
        {
            get { return _bottomZoneValueY; }
            set
            {
                _bottomZoneValueY = value;
                Invalidate();
            }
        }

        private float _stepHeight = 10;
        [Description("Step height"), Category("Data")]
        public float StepHeight
        {
            get { return _stepHeight; }
            set
            {
                _stepHeight = value;
                Invalidate();
            }
        }

        private float _smallOffsetY = 5;
        [Description("Small offset Y"), Category("Data")]
        public float SmallOffsetY
        {
            get { return _smallOffsetY; }
            set
            {
                _smallOffsetY = value;
                Invalidate();
            }
        }

        private float _bigOffsetY = 10;
        [Description("Big offset Y"), Category("Data")]
        public float BigOffsetY
        {
            get { return _bigOffsetY; }
            set
            {
                _bigOffsetY = value;
                Invalidate();
            }
        }

        #endregion

        public BackgroundControl()
        {
            InitializeComponent();

            ResizeRedraw = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            var graphics = e.Graphics;
            graphics.SmoothingMode = SmoothingMode.AntiAlias;
            graphics.Clear(BackColor);

            var x1 = 0;
            var x2 = Width - 1;
            if (TopZone)
            {
                using (var brush = new SolidBrush(TopZoneColor))
                {
                    var rect = new RectangleF(x1, 0, x2, TopZoneValueY);
                    graphics.FillRectangle(brush, rect);
                }
            }
            if (BottomZone)
            {
                using (var brush = new SolidBrush(BottomZoneColor))
                {
                    var rect = RectangleF.FromLTRB(x1, BottomZoneValueY, x2, Height - 1);
                    graphics.FillRectangle(brush, rect);
                }
            }

            using (var pen = new Pen(LinesColor))
            {
                for (var height = SmallOffsetY; height < Height; height += StepHeight)
                {
                    graphics.DrawLine(pen, 0.2F * x2, height, 0.8F * x2, height);
                }
            }

            using (var pen = new Pen(LinesColor, 2))
            {
                for (var height = BigOffsetY; height < Height; height += StepHeight)
                {
                    graphics.DrawLine(pen, 0.1F * x2, height, 0.9F * x2, height);
                }
            }

            using (var pen = new Pen(BorderColor))
            {
                graphics.DrawRectangle(pen, x1, 0, x2, Height - 1);
            }

            using (var pen = new Pen(CurrentValueColor))
            {
                graphics.DrawLine(pen, 0, CurrentValueY, Width - 1, CurrentValueY);
            }
        }
    }
}
