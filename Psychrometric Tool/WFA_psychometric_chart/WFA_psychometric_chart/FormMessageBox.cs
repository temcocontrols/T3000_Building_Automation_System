using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class FormMessageBox : Form
    {
        public FormMessageBox()
        {
            InitializeComponent();
        }

        private void btnGoBack_Click(object sender, EventArgs e)
        {

            Form1_main f1 = new Form1_main();
            f1.Close();
            this.Close();
        }
       // int identifierForButtonClick = 0;
        private void btnRunDefaultSetting_Click(object sender, EventArgs e)
        {
            BuildingIdentificationClass bn = new BuildingIdentificationClass();
            bn.SelectedSettingsForUserOption = 1;
            //identifierForButtonClick = 1;
        }
    }
}
