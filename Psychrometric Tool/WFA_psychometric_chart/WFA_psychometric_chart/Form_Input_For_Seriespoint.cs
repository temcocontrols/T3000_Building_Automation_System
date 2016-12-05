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
        int indexRun = 1;
        private void Form_Input_For_Seriespoint_Load(object sender, EventArgs e)
        {
            btnColor.BackColor = Color.Blue;
            comboBox1.SelectedIndex = 1;

            //This will help in quick insertion
            
            int t = F1.menuStripNodeInfoValues.Count;
            if (t > 0)
            {
                indexRun = 1 + t;
            }

            int indNodeCount = indexRun;
            string nodeName = "Node" + indNodeCount;//This is the node name 

            for (int i = 0; i < F1.menuStripNodeInfoValues.Count; i++)
            {
                if (F1.menuStripNodeInfoValues[i].name == nodeName)
                {
                    indNodeCount++;
                    nodeName = "Node" + indNodeCount;
                    i = 0;
                }

            }


            
            //string nodeLabel = "Label" + indNodeCount;

            //for (int i = 0; i < F1.menuStripNodeInfoValues.Count; i++)
            //{
            //    if (F1.menuStripNodeInfoValues[i].label == nodeLabel)
            //    {
            //        indNodeCount++;
            //        nodeLabel = "Node" + indNodeCount;
            //        i = 0;
            //    }

            //}

            int indLineCount = indexRun-1;
            //This one is for line Name
            string lineName = "Line" + indLineCount;
            for (int i = 0; i < F1.menuStripNodeLineInfoValues.Count; i++)
            {
                if (F1.menuStripNodeLineInfoValues[i].name == lineName)
                {
                    indLineCount++;
                    lineName = "Line" + indLineCount;
                    i = 0;
                }

            }


            tbName.Text = nodeName;
           // tbLabel.Text = nodeLabel;
            gb_line_info.Enabled = false;
            if (F1.menuStripNodeInfoValues.Count > 0)
            {
                gb_line_info.Enabled = true;
                tb_line_name.Text = lineName;
                cb_line_enabled.Checked = true;
               
            }


        }

        private void btnSetNode_Click(object sender, EventArgs e)
        {
            /*
            This set node should do two task 
            1. Insert a point by calling a function 
            2. It should input the values in menuStripNodeInfoValues
            */
            //string source = tbSource.Text;
            //string name = tbName.Text;
            //string label = tbLabel.Text;
            //string cbItemSelected = comboBox1.Text;
            //string lineName = "";
            //int status = 0;//dissabled 
            //if (cb_line_enabled.Checked == true)
            //{
            //    status = 1;
            //}
            //if(tb_line_name.Text == "")
            //{
            //    int t = F1.menuStripNodeInfoValues.Count;
            //    if (t > 0)
            //    {
            //        indexRun = t;
            //    }


            //    int indLineCount = indexRun;
            //    //This one is for line Name
            //    for (int i = 0; i < F1.menuStripNodeLineInfoValues.Count; i++)
            //    {
            //        if (F1.menuStripNodeLineInfoValues[i].name == lineName)
            //        {
            //            indLineCount++;
            //            lineName = "Line" + indLineCount;
            //            i = 0;
            //        }

            //    }

            //}
            //else
            //{
            //    lineName = tb_line_name.Text;
            //}

            ////MessageBox.Show("items index " + cbItemSelected);
            //if (F1.menuStripNodeInfoValues.Count > 0) { 

            //F1.SetNode(source, name, label, btnColor.BackColor, cbItemSelected,20,lineName,status);

            //}
            //else
            //{
            //    //--For first input
            //    F1.SetNode(source, name, label, btnColor.BackColor, cbItemSelected, 20, lineName, status);
            //}
            ////MessageBox.Show(Properties.Resources.Success0);
            //this.Close();

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
