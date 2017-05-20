using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PH_App
{
    public partial class Form_PressureInput : Form
    {
        FormEditNodeAndLine f1;
        public Form_PressureInput(FormEditNodeAndLine f)
        {
            f1 = f;
            InitializeComponent();
        }
    }
}
