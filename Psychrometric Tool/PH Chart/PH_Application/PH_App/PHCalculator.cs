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
    public partial class PHCalculator : Form
    {
        public PHCalculator()
        {
            InitializeComponent();
        }

        private void PHCalculator_Load(object sender, EventArgs e)
        {
            radioButton1.Checked = true;
        }

        string initialTemp = "";
        string initialPressure = "";
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            try { 
            if(textBox1.Text == ""|| tb_atm_pressure.Text == "")
            {
                return;
            }
            //--Text change properties
            double input = double.Parse(textBox1.Text.Trim());
            double inputPr = double.Parse(tb_atm_pressure.Text.Trim());
            if(input <= 0 ||inputPr <= 0)
            {
                return;
            }
            if(radioButton1.Checked)
            { 

            if(input > 0 && input < 1000)//Deg C
            {
                    tbEnthalpy.Text = CalculateSI(input, inputPr).ToString();
                    initialTemp = textBox1.Text;
            }
                else
            {
                MessageBox.Show("Temperature can be between 0 to 1000 °C !");
                    textBox1.Text = initialTemp;
            }

            }else if (radioButton2.Checked)
            {
                if (input > 0 && input < 1000)//Deg C
                {
                    tbEnthalpy.Text = CalculateCGS(input, inputPr).ToString();
                    initialTemp = textBox1.Text;
                }
                else
                {
                    MessageBox.Show("Temperature can be between 0 to 1000 °C!");
                    textBox1.Text = initialTemp;
                }
             }
       
                }catch(Exception ex)
                 {
                  MessageBox.Show(ex.Message);
                 }
        }


     

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            unit_pressure.Text = "MPa";
            if(tb_atm_pressure.Text != "")
            {
                tb_atm_pressure.Text =Math.Round((double.Parse(tb_atm_pressure.Text) * 0.00689476),2).ToString();
            }

        }
        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            unit_pressure.Text = "psi";//pressure per square inch
            if (tb_atm_pressure.Text != "")
            {
                tb_atm_pressure.Text = Math.Round((double.Parse(tb_atm_pressure.Text) * 145.038),2).ToString();//MPa
            }
        }

        private void tb_atm_pressure_TextChanged(object sender, EventArgs e)
        {
            //--Text changed 
            try { 
            if (textBox1.Text == "" || tb_atm_pressure.Text == "")
            {
                return;
            }
            //--Text change properties
            double input = double.Parse(textBox1.Text.Trim());

            double pressureInput = double.Parse(tb_atm_pressure.Text.Trim());
            if (input <= 0 || pressureInput <= 0)
            {
                return;
            }
            if (radioButton1.Checked) { 
            if(pressureInput >0 && pressureInput <= 100)
            {
                    //0k
                    tbEnthalpy.Text = CalculateSI(input, pressureInput).ToString();
                    initialPressure = tb_atm_pressure.Text;
                }
                else
            {
                MessageBox.Show("Pressure should be between 0 to 100 MPa");
                    //return;
                    tb_atm_pressure.Text = initialPressure;
            }
            }else if (radioButton2.Checked)
            {
                if (pressureInput > 0 && pressureInput <= 14504 )
                {
                    //0k
                    tbEnthalpy.Text = CalculateCGS(input, pressureInput).ToString();
                    initialPressure = tb_atm_pressure.Text;
                }
                else
                {
                    MessageBox.Show("Pressure should be between 0 to 14504 psi");
                    tb_atm_pressure.Text = initialPressure;
                    //return;
                }
            }
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

       
        public double CalculateSI(double Temperature, double pressure)
        {
            double tempValue = Temperature + 273.15;//Kelving
            double pressureValue = pressure * 1000000;//Pa
            var mth = new MathOperation();
            double enthalpy = Math.Round(mth.IAPWS_IF97_TowParameterEquivalentFxn("H", "T", tempValue, "P", pressureValue, "water") / 1000, 2);
            return enthalpy;
        }

        public double CalculateCGS(double Temperature, double pressure)
        {
            double tempValue = Temperature + 273.15;//Kelving
            double pressureValue = pressure * 6894.76;// 1000000;//Pa
            var mth = new MathOperation();
            double enthalpy = Math.Round(mth.IAPWS_IF97_TowParameterEquivalentFxn("H", "T", tempValue, "P", pressureValue, "water") / 1000, 2);
            return enthalpy;
        }
    }
}
