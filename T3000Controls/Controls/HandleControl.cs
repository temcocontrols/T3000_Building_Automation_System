namespace T3000Controls
{
    using System.ComponentModel;
    using System.Drawing;
    using System.Drawing.Drawing2D;
    using System.Windows.Forms;
    
    internal partial class HandleControl : UserControl
    {
        #region DesignerProperties

        private float _value = 0.0F;
        [Description("Value for handle"), Category("Data")]
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

        private int _handleWidth = 50;
        [Description("Handle width"), Category("Appearance")]
        public int HandleWidth
        {
            get { return _handleWidth; }
            set
            {
                _handleWidth = value;

                Invalidate();
            }
        }

        private int _handleHeight = 10;
        [Description("Handle height"), Category("Appearance")]
        public int HandleHeight
        {
            get { return _handleHeight; }
            set
            {
                _handleHeight = value;

                Invalidate();
            }
        }

        private Color _borderColor = Color.White;
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

        public HandleControl()
        {
            InitializeComponent();

            ResizeRedraw = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            var handleRectangleRadius = HandleHeight / 4;
            var textRectangleRadius = HandleHeight / 2;

            valueLabel.Text = $"{_value.ToString("F1")}{AdditionalText}";
            valueLabel.Location = new Point(HandleWidth + textRectangleRadius, 1);
            valueLabel.Size = new Size(Width - HandleWidth - 2 * textRectangleRadius, Height - 2);

            var handleRectangle = new RectangleF(0, Height / 2.0F - HandleHeight / 2.0F, HandleWidth, HandleHeight);
            var textRectangle = new Rectangle(HandleWidth, 0, Width - HandleWidth, Height);
            var handlePath = GraphicsUtilities.CreateRoundedRectanglePath(handleRectangle, handleRectangleRadius);
            var textPath = GraphicsUtilities.CreateRoundedRectanglePath(textRectangle, textRectangleRadius);

            var graphics = e.Graphics;
            graphics.SmoothingMode = SmoothingMode.AntiAlias;

            using (var brush = new SolidBrush(BackColor))
            {
                graphics.FillPath(brush, handlePath);
                graphics.FillPath(brush, textPath);
            }
            using (var pen = new Pen(BorderColor))
            {
                graphics.DrawPath(pen, handlePath);
                graphics.DrawPath(pen, textPath);
            }
            using (var brush = new SolidBrush(BackColor))
            {
                var rect = handleRectangle;
                rect.Inflate(2, -1);
                rect.X += 3;
                graphics.FillRectangle(brush, rect);
            }

            var region = GraphicsUtilities.GetRegionForPath(handlePath);
            region.Union(GraphicsUtilities.GetRegionForPath(textPath));
            Region = region;
        }
    }
}
