using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using T3000Controls.Properties;

namespace T3000Controls
{
    [Guid("E21C6EB6-FEC9-4962-B2D5-1E904B33C3F8")]
    [ClassInterface(ClassInterfaceType.None)]
    [ComSourceInterfaces(typeof(ICheckEvents))]

    public partial class TCheckBox :CheckBox, ICheck
    {
        #region All Private
        private string _Caption = "TCheckBox";
        private string _FontName = "Arial";
        private int _FontSize = 10;
        private bool _Bold = false;
        private bool _Italic = false;
        private bool _Strikeout = false;
        private bool _Underline = false;
        private Color _BackColor = Color.White;
        private Color _FontColor = Color.Black;
        private Color _CheckColor = Color.Red;
        private bool _ShowPicture = false;
        #endregion
        #region All Label Property
        [Description("Caption"), Category("Text")]
        public string TCaption
         {
            get{ return _Caption; }
            set{ SetText(value); }
         }
        //Text Font;
    
        [Description("Text Font"), Category("Font")]
        [DefaultValue("Arial")]
        public string TFontName
        {
            get { return _FontName; }
            set
            {
                _FontName = value;
                if (_FontName.Length > 0)
                {
                    this.OnFontChanged(EventArgs.Empty);
                    Invalidate();
                }


            }
        }

    

        [Description("Font Size"), Category("Font")]
        [DefaultValue(15)]
        public int TFontSize
        {
            get
            {
                return _FontSize;
            }
            set
            {
                _FontSize = value;
                if (_FontSize >= 0)
                {
                    this.OnFontChanged(EventArgs.Empty);
                    Invalidate();
                }
            }
        }
        
        [Description("Bold"), Category("Font")]
        public bool TBold
        {
            get
            {
                return _Bold;
            }
            set
            {
                _Bold = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }

        
        [Description("Italic"), Category("Font")]
        public bool TItalic
        {
            get
            {
                return _Italic;
            }
            set
            {
                _Italic = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();

            }
        }
     
        [Description("Strikeout"), Category("Font")]
        public bool TStrikeout
        {
            get
            {
                return _Strikeout;
            }
            set
            {
                _Strikeout = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }
     
        [Description("Underline"), Category("Font")]
        public bool TUnderline
        {
            get { return _Underline; }
            set
            {
                _Underline = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }

        [Description("Back Color"), Category("TEXT")]
        public Color TBackColor
        {
            get { return _BackColor; }
            set { _BackColor = value; this.BackColor = value; Invalidate(); }
        }
        [Description("Font Color"), Category("Font")]
        public Color TFontColor
        {
            get { return _FontColor; }
            set { _FontColor = value; this.ForeColor = value; Invalidate(); }
        }

        [Description("Check Color"),Category("CheckSet")]
        public Color CheckColor
        {
            get
            {
                return _CheckColor;
            }
            set
            {
                _CheckColor = value;
                this.Invalidate();
            }
        }
        [Description("Show Picture on the Check"),Category("CheckSet")]
        public bool TShowPicture
        {
            get { return _ShowPicture; }
            set { _ShowPicture = value;
                this.OnCheckedChanged(EventArgs.Empty);
                }
        }
        [Description("Checked"), Category("CheckValue")]
        public bool TChecked
        {
            get { return this.Checked; }
            set
            {
                this.Checked = value;
                this.OnCheckedChanged(EventArgs.Empty);
            }
        }
        #endregion


        #region Interface release
        public void SetText(string strText)
        {
            _Caption = strText;
            this.Text = _Caption;
            this.OnTextChanged(EventArgs.Empty);
        }
        public string GetText()
        {
            return _Caption;
        }
        public void SetFont(string FontName, int FontSize,
                     bool IsBold, bool Italic,
                     bool Strikeout, bool Underline)
        {
            _FontName = FontName;
            _FontSize = FontSize;
            _Bold = IsBold;
            _Italic = Italic;
            _Strikeout = Strikeout;
            _Underline = Underline;
            this.OnFontChanged(EventArgs.Empty);
        }
        #endregion
        #region Events
        [Description("Causes if top zone value is changed"), Category("Slider")]
        public virtual event EventHandler CheckBoxClick;
         
        #endregion
        public TCheckBox()
        {
            InitializeComponent();
            this.AutoSize = false;
            this.Width = 100;
            this.Height = 20;
            this.Text = _Caption;
            this.Paint += new PaintEventHandler(this.PaintHandler);
         
            this.Click += new System.EventHandler(this.OnCheckBoxClick);
        }
        private void PaintHandler(object sender, PaintEventArgs pe)
        {
            if (this.Checked)
            {
                Point pt = new Point();

                if (this.CheckAlign == ContentAlignment.BottomCenter)
                {
                    pt.X = (this.Width / 2) - 4;
                    pt.Y = this.Height - 11;
                }
                if (this.CheckAlign == ContentAlignment.BottomLeft)
                {
                    pt.X = 3;
                    pt.Y = this.Height - 11;
                }
                if (this.CheckAlign == ContentAlignment.BottomRight)
                {
                    pt.X = this.Width - 11;
                    pt.Y = this.Height - 11;
                }
                if (this.CheckAlign == ContentAlignment.MiddleCenter)
                {
                    pt.X = (this.Width / 2) - 4; ;
                    pt.Y = (this.Height / 2) - 4;
                }
                if (this.CheckAlign == ContentAlignment.MiddleLeft)
                {
                    pt.X = 3;
                    pt.Y = (this.Height / 2) - 4;
                }
                if (this.CheckAlign == ContentAlignment.MiddleRight)
                {
                    pt.X = this.Width - 11;
                    pt.Y = (this.Height / 2) - 4;
                }
                if (this.CheckAlign == ContentAlignment.TopCenter)
                {
                    pt.X = (this.Width / 2) - 4;
                    pt.Y = 3;
                }
                if (this.CheckAlign == ContentAlignment.TopLeft)
                {
                    pt.X = 3;
                    pt.Y = 3;
                }
                if (this.CheckAlign == ContentAlignment.TopRight)
                {
                    pt.X = this.Width - 11;
                    pt.Y = 3;
                }

                DrawCheck(pe.Graphics, this._CheckColor, pt);
            }
        }

        public void DrawCheck(Graphics g, Color c, Point pt)
        {
            if(_ShowPicture)
            {
                Image chectimag = (Image)(Resources.CHECKED);
                g.DrawImage(chectimag, pt.X - 2, pt.Y - 2);
            }
            else
            {
                Pen pen = new Pen(this._CheckColor);
                g.DrawLine(pen, pt.X, pt.Y + 2, pt.X + 2, pt.Y + 4);
                g.DrawLine(pen, pt.X, pt.Y + 3, pt.X + 2, pt.Y + 5);
                g.DrawLine(pen, pt.X, pt.Y + 4, pt.X + 2, pt.Y + 6);
                g.DrawLine(pen, pt.X + 3, pt.Y + 3, pt.X + 6, pt.Y);
                g.DrawLine(pen, pt.X + 3, pt.Y + 4, pt.X + 6, pt.Y + 1);
                g.DrawLine(pen, pt.X + 3, pt.Y + 5, pt.X + 6, pt.Y + 2);
            }


           
        }
        protected override void OnFontChanged(EventArgs e)
        {


            Font font = new Font(new FontFamily(_FontName),
                         float.Parse(_FontSize.ToString()),
                         FontStyle.Bold | FontStyle.Italic | FontStyle.Strikeout | FontStyle.Underline);
            if (!_Italic)//没有斜体
            {//删除字体中的斜体样式
                font = new Font(font, font.Style & ~FontStyle.Italic);
            }
            if (!_Bold)//没有粗体
            {//删除字体中的粗体
                font = new Font(font, font.Style & ~FontStyle.Bold);
            }
            if (!_Strikeout)//没有删除线
            {//删除字体中的删除线
                font = new Font(font, font.Style & ~FontStyle.Strikeout);
            }
            if (!_Underline)//没有下划线
            {//删除字体中的下划线
                font = new Font(font, font.Style & ~FontStyle.Underline);
            }
            //这样就得到原来保存的字体样式了，有点麻烦……
            this.Font = font;

            base.OnFontChanged(e);
            this.Invalidate();
        }
        protected void OnCheckBoxClick(object sender, EventArgs e) => CheckBoxClick?.Invoke(this, EventArgs.Empty);
        

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);

    }
}
