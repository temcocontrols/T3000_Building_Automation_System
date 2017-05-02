using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
namespace T3000Controls
{
    
    [Guid("4E8459AA-8058-45F2-9DB1-F3CE56F38B3A")]
    [ClassInterface(ClassInterfaceType.None)]

    public partial class TLabel : Label,ITLabel
    {
        private Pen drawPen;
        #region All Label Property
        
       

        private string _Caption;
        [DefaultValue("Temco Controls")]
        [Description("Caption"), Category("Text")]

        public string TCaption
        {
            get { return _Caption; }
            set
            {
                _Caption = value;
                SetText(_Caption);

                Invalidate();
            }
        }
        public void SetText(string strText)
        {
            this.Text = strText;
        }
        public string GetText()
        {
            return _Caption;
        }
       
        private Color _TextColor;
        [Description("Text Color"), Category("Font")]
        public Color TTextColor
        {
            get { return _TextColor; }
            set { _TextColor = value;this.ForeColor = value;Invalidate();}
        }
      
        //Text Font;
        private string _FontName;
        [Description("Text Font"), Category("Font")]
        [DefaultValue("Arial")]
        public string TFontName
        {
            get { return _FontName; }   
            set {
                _FontName = value;
                if (_FontName.Length>0)
                {
                    this.OnFontChanged(EventArgs.Empty);
                    Invalidate();
                }
                
                
            }
        }

        private int _FontSize;

        [Description("Font Size"), Category("Font")]
        [DefaultValue(15)]
        public int TFontSize
        {
            get
            {
                return _FontSize;
            } 
            set {
                _FontSize = value;
                if(_FontSize>=0)
                {
                    this.OnFontChanged(EventArgs.Empty);
                    Invalidate();
                }
            }
        }
        private bool _IsBold;
        [Description("Bold"), Category("Font")]
        public bool TIsBold
        {
            get
            {
                return _IsBold;
            }
            set
            {
                _IsBold = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }

        private bool _IsItalic;
        [Description("Italic"), Category("Font")]
        public bool TIsItalic
        {
            get
            {
                return _IsItalic;
            }
            set {
                _IsItalic = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();

            }
        }
        private bool _IsStrikeout;
        [Description("Strikeout"), Category("Font")]
        public  bool TIsStrikeout
        {
            get
            {
                return _IsStrikeout;
            }
            set {
                _IsStrikeout = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }
        private bool _IsUnderline;
        [Description("Underline"), Category("Font")]
        public bool TIsUnderline
        {
            get {return _IsUnderline; }
            set { _IsUnderline = value;

                this.OnFontChanged(EventArgs.Empty);
                Invalidate();
            }
        }
        [Description("Back Color"), Category("TEXT")]
        //Label's background Color
        private Color _BackColor;
        public Color TBackColor
        {
            get {return _BackColor; }
            set { _BackColor = value;this.BackColor = value;Invalidate(); }
        }

        [Description("Image Path"), Category("TEXT")]
        //Label's background Image
        private string _ImgPath;
        public string TImgPath
        {
            get { return _ImgPath; }
            set { _ImgPath = value;this.Image = Image.FromFile(_ImgPath);Invalidate(); }
        }
        [Description("Label Border Size"), Category("TEXT")]
        //Label's Size
        private float _BorderSize;
        public float TBorderSize
        {
            get { return _BorderSize; }
            set { _BorderSize = value;
                this.drawPen.Color = this.TBorderColor;
                this.drawPen.Width = value;
                //Invalidate();
                this.OnTextChanged(EventArgs.Empty);
            }
        }
         

        [DefaultValue(typeof(Color), "Black")]
        [Description("Back Color"), Category("TEXT")]
        //Label's background Color
        private Color _TBorderColor;
        public Color TBorderColor
        {
            get { return _TBorderColor; }
            set { _TBorderColor = value;
                if(this.TBorderSize !=0)
                    this.drawPen.Color = value;
                Invalidate(); }
        }

        [Description("Label Frame Width"), Category("TEXT")]

        private Size _Size;
        public int TWidth
        {
            get { return  _Size.Width; }

            set
            {
                _Size.Width = value; Parent.Size = _Size; Invalidate();
            }
        }
        [Description("Label Frame Height"), Category("TEXT")]
        public int THeight
        {
            get { return  _Size.Height; }

            set
            {
                _Size.Height = value; Parent.Size = _Size; Invalidate();
            }
        }
        [Description("Label Visible"), Category("TEXT")]
        private bool _Visible;
        public bool TVisible
        {
            get { return _Visible; }

            set
            {
                _Visible = value; Parent.Visible = value; Invalidate();
            }
        }
        [Description("Label Enabled"), Category("TEXT")]
        private bool _Enabled;
        public bool TEnabled
        {
            get { return _Enabled; }
            set
            {
                _Enabled = value; Parent.Enabled = value; Invalidate();
            }
        }
        [Description("Label AutoSize"), Category("TEXT")]
        private bool _AutoSize;
        public bool TAutoSize
        {
            get { return _AutoSize; }

            set
            {
                _AutoSize = value;
                this.AutoSize = value;
                Invalidate();
            }
        }
        [Description("Label ShowBorder"), Category("TEXT")]
        private bool _ShowBorder;
        public bool TShowBorder
        {
            get { return _ShowBorder; }

            set
            {
                _ShowBorder = value;
                // 画Frame
                 Invalidate();
            }
        }

        #endregion
        #region Public Methods
         
        #endregion
        public TLabel()
        {
            InitializeComponent();

            _Caption = "Temco Controls";
            _TextColor = Color.Black;
            _BackColor = Color.White;
            _Size.Width = 70;
            _Size.Height = 25;
            _Visible = true;
            _Enabled = true;
            _AutoSize = true;
            _ShowBorder = false;
            _BorderSize = 1;
            _TBorderColor = Color.Black;
            _FontName = "Arial";
            _FontSize = 15;
            _IsBold = false;
            _IsItalic = false;
            _IsStrikeout = false;
            _IsUnderline = false;
            _ImgPath = "";
             OnFontChanged(EventArgs.Empty);
            this.drawPen = new Pen(new SolidBrush(this._TBorderColor), _BorderSize);
        }
        protected override void OnFontChanged(EventArgs e)
        {

           
              Font font = new Font(new FontFamily(TFontName),
                           float.Parse(TFontSize.ToString()),
                           FontStyle.Bold | FontStyle.Italic | FontStyle.Strikeout | FontStyle.Underline);
            if (!_IsItalic)//没有斜体
            {//删除字体中的斜体样式
                font = new Font(font, font.Style & ~FontStyle.Italic);
            }
            if (!_IsBold)//没有粗体
            {//删除字体中的粗体
                font = new Font(font, font.Style & ~FontStyle.Bold);
            }
            if (!_IsStrikeout)//没有删除线
            {//删除字体中的删除线
                font = new Font(font, font.Style & ~FontStyle.Strikeout);
            }
            if (!_IsUnderline)//没有下划线
            {//删除字体中的下划线
                font = new Font(font, font.Style & ~FontStyle.Underline);
            }
            //这样就得到原来保存的字体样式了，有点麻烦……
            this.Font = font;

            base.OnFontChanged(e);
            this.Invalidate();
        }
        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            if(TShowBorder)
            {
                pe.Graphics.DrawRectangle(drawPen, 0, 0, this.Size.Width, this.Size.Height);

            }
        }

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);


    }
}
