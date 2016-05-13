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
    public partial class Form_Psychrometric_Calculator : Form
    {
        public Form_Psychrometric_Calculator()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {

            //this is where the actual calculation begins ..
            //lets defin the constants..

            try
            {

                //lets grab from the textbox input.
                double     temperature = Double.Parse(textBox1.Text.Trim());
                double    humidity = Double.Parse(textBox2.Text.Trim());


                double Patm = 1013;
                try
                {
                    Patm = double.Parse(tb_atm_pressure.Text);
                     
                }catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                tb_atm_pressure.Text = Patm.ToString(); 

                double      A = 6.116441;
                double m = 7.591386;
                double Tn = 240.7263;
                double B = 621.9907;

                //lets set the values..
                tb_atm_pressure.Text = Patm.ToString();
                textBox5.Text = A.ToString();
                textBox6.Text = m.ToString();
                textBox7.Text = Tn.ToString();
                textBox9.Text = B.ToString();

                //calculating 1.dew point..
                double TDewpoint = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature) / (243.04 + temperature))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature) / (243.04 + temperature)));
                //setting to textbox4 its a dew point.
                textBox4.Text = Convert.ToString(TDewpoint);
                //2.Partial pressure of saturation.

                double Pws = A * Math.Pow(10, (m * TDewpoint) / (TDewpoint + Tn));
                textBox8.Text = Convert.ToString(Pws);

                double X = B * Pws / (Patm - Pws);
                textBox10.Text = Convert.ToString(X);

                double h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
                textBox11.Text = Convert.ToString(h);


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                textBox1.Text = "";
                textBox2.Text = "";
                //lets set the values..
                tb_atm_pressure.Text = "";
                textBox5.Text = "";
                textBox6.Text = "";
                textBox7.Text = "";
                textBox9.Text = "";


            }

            //for the EU unit just check if EU unit was selected of not if selected then convert
            //this values here...




        }
    }
}
