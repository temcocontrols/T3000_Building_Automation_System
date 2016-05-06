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
    public partial class Form_Input_For_Seriespoint : Form
    {
        private Form1_main F1;
        public Form_Input_For_Seriespoint(Form1_main fm_p_d)
        {
            this.F1 = fm_p_d;
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.Form_Input_For_Seriespoint_Disposed );
        }

        private void button1_Click(object sender, EventArgs e)
        {
            colorDialog1.ShowDialog();
        }

        private void Form_Input_For_Seriespoint_Load(object sender, EventArgs e)
        {
            btnColor.BackColor = Color.Blue;
            comboBox1.SelectedIndex = 0;



        }

        private void btnSetNode_Click(object sender, EventArgs e)
        {
            /*
            This set node should do two task 
            1. Insert a point by calling a function 
            2. It should input the values in menuStripAllValues
            */
            string source = tbSource.Text;
            string name = tbName.Text;
            string label = tbLabel.Text;
            string cbItemSelected = comboBox1.Text;


            //MessageBox.Show("items index " + cbItemSelected);


            F1.SetNode(source, name, label, btnColor.BackColor, cbItemSelected);
            //MessageBox.Show(Properties.Resources.Success0);

            this.Close();

        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnColor_Click(object sender, EventArgs e)
        {
            if(colorDialog1.ShowDialog()== DialogResult.OK)
            {
                btnColor.BackColor = colorDialog1.Color;

            }
            
        }

      public void Form_Input_For_Seriespoint_Disposed ( object sender, System.EventArgs e )
      {
      }
    }
}
