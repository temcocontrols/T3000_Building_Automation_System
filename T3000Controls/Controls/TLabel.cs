namespace T3000Controls
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Drawing;
    using System.Data;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;

    [Guid("B1D99C3A-D02B-4AB2-B44C-21CE5F57B7E1")]
    [ClassInterface(ClassInterfaceType.None)]
   
    public partial class TLabel : UserControl, ITLabel
    {
        #region Properties
        private string _Caption;
        [Description("Label Caption"), Category("Label")]
        public string LabelText
        {
            get { return _Caption; }
            set
            {
                _Caption = value;
                //OnChangeLabelCaption(value);
                this.label1.Text = value;
                 Invalidate();
            }
        }

        #endregion

        public TLabel()
        {
            InitializeComponent();
        }

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);


    }
}
