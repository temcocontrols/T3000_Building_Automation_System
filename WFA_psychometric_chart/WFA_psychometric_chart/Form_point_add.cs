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
    public partial class Form_point_add : Form
    {
        private Form_handler F1;
        public Form_point_add(Form_handler f)
        {
            this.F1 = f;
            InitializeComponent();
        }

        private void btnColor_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                btnColor.BackColor = colorDialog1.Color;

            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private static String HexConverter(System.Drawing.Color c)
        {
            return "#" + c.R.ToString("X2") + c.G.ToString("X2") + c.B.ToString("X2");
        }
        private void btnSetNode_Click(object sender, EventArgs e)
        {
            /*
            This set node should do two task 
            1. Insert a point by calling a function 
            2. It should input the values in menuStripNodeInfoValues
            */
            string source = tbSource.Text;
            string name = tbName.Text;
            string label = tbLabel.Text;
            string cbItemSelected = comboBox1.Text;


            //MessageBox.Show("items index " + cbItemSelected);

          //  Color c = HexConverter(btnColor.BackColor);

            F1.SetNode(source, name, label, btnColor.BackColor, cbItemSelected);
            //MessageBox.Show(Properties.Resources.Success0);

            this.Close();

        }

        private void Form_point_add_Load(object sender, EventArgs e)
        {
            btnColor.BackColor = Color.Orange;
            comboBox1.SelectedIndex = 0;
        }
    }
}
