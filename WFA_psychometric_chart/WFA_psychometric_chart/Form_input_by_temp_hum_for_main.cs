using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class Form_input_by_temp_hum_for_main : Form
    {
        Form1_main F1;
        public Form_input_by_temp_hum_for_main(Form1_main f)
        {
            F1 = f;
            InitializeComponent();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (tb_temperature_input.Text != "")
            {
                string pattern = @"^[0-9]*(?:\.[0-9]*)?$";//@"^[0 - 9]([.][0 - 9])?$"; //@"^[0-9]+$"; //for integers only
                Regex rgx = new Regex(pattern);
                string text = tb_temperature_input.Text;
                //string sentence = "Who writes these notes?";
                if (rgx.IsMatch(text) == true)
                {
                    //matched no need to edit 
                    double x = double.Parse(tb_temperature_input.Text);
                    if (x >= 0 && x <= 50)
                    {

                    }
                    else
                    {
                        //show error message.
                        MessageBox.Show("Value out of range");
                        tb_temperature_input.Text = 0.ToString();
                    }
                }
                else
                {
                    MessageBox.Show("You can only input integer values between 0-50 deg C");
                    // dataGridView1.CurrentCell.Value = initialNodeSize;
                    tb_temperature_input.Text = 0.ToString();
                }
            }
            else
            {
                tb_temperature_input.Text = 0.ToString();
            }
        }

        private void tb_humidity_input_TextChanged(object sender, EventArgs e)
        {
            if(tb_humidity_input.Text != "") { 
            string pattern = @"^[0-9]*(?:\.[0-9]*)?$";//@"^[0 - 9]([.][0 - 9])?$"; //@"^[0-9]+$"; //for integers only
            Regex rgx = new Regex(pattern);
            string text = tb_humidity_input.Text;
            //string sentence = "Who writes these notes?";
            if (rgx.IsMatch(text) == true)
            {
                //matched no need to edit 
                double x = double.Parse(tb_humidity_input.Text);
                if (x >= 0 && x <= 100)
                {

                }
                else
                {
                    //show error message.
                    MessageBox.Show("Value out of range");
                    tb_humidity_input.Text = 0.ToString();
                }
            }
            else
            {
                MessageBox.Show("You can only input integer values between 0-100%");
                // dataGridView1.CurrentCell.Value = initialNodeSize;
                tb_humidity_input.Text = 0.ToString();
            }

            }
            else
            {
                tb_humidity_input.Text = 0.ToString();
            }
        }

        private void btnSetNode_Click(object sender, EventArgs e)
        {
            //Set node values

            /*
    This set node should do two task 
    1. Insert a point by calling a function 
    2. It should input the values in menuStripNodeInfoValues
    */
            string source = tbSource.Text;
            string name = tbName.Text;
            string label = tbLabel.Text;
            string cbItemSelected = comboBox1.Text;
            double temp = double.Parse(tb_temperature_input.Text);
            double hum = double.Parse(tb_humidity_input.Text);


            //MessageBox.Show("items index " + cbItemSelected);


            F1.SetNodeWithValues(source, name, label, btnColor.BackColor, cbItemSelected,temp,hum);
            //MessageBox.Show(Properties.Resources.Success0);
            this.Close();
        }

        private void Form_input_by_temp_hum_for_main_Load(object sender, EventArgs e)
        {
            btnColor.BackColor = Color.MediumVioletRed;

            comboBox1.SelectedIndex = 0;
        }

        private void btnColor_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                btnColor.BackColor = colorDialog1.Color;

            }
        }
    }
}
