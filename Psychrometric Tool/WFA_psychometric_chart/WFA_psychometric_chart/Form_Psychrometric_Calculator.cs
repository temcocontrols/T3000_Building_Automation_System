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

        public void calculate()
        {

            //this is where the actual calculation begins ..
            //lets defin the constants..

            try
            {
                if(textBox1.Text != "" && textBox2.Text != "") { 

                if (radioButton1.Checked == true)
                {

                    //lets grab from the textbox input.

                    if (textBox1.Text == "" || textBox2.Text == "" || tb_atm_pressure.Text == "")
                    {
                        MessageBox.Show("Please enter all parameter properly");
                        return;
                    }

                    double temperature = Double.Parse(textBox1.Text.Trim());

                    double humidity = Double.Parse(textBox2.Text.Trim());
                    double Patm = double.Parse(tb_atm_pressure.Text);


                    // tb_atm_pressure.Text = Patm.ToString();

                    CalculatePsymetric(temperature, humidity, Patm);


                    //This one is dew point
                    textBox4.Text = Math.Round(output_dew_point, 4).ToString();
                    textBox8.Text = Math.Round(output_partial_pressure_of_water, 4).ToString();
                    textBox10.Text = Math.Round(output_mixing_ratio, 4).ToString();
                    textBox11.Text = Math.Round(output_enthalpy, 4).ToString();
                    textBox9.Text = Math.Round(output_mol_mass_of_water, 4).ToString();




                }
                else if (radioButton2.Checked == true)
                {

                    if (textBox1.Text == "" || textBox2.Text == "" || tb_atm_pressure.Text == "")
                    {
                        MessageBox.Show("Please enter all parameter properly");
                        return;
                    }

                    //lets grab from the textbox input.
                    double temperature = Double.Parse(textBox1.Text.Trim()); // in deg F
                    double humidity = Double.Parse(textBox2.Text.Trim());    // in % no change


                    double Patm = 29.913872;    //is in In Hg      need to be converted to Hpa
                    try
                    {
                        Patm = double.Parse(tb_atm_pressure.Text);

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }

                    tb_atm_pressure.Text = Patm.ToString();

                    //Thes value needs to be converted first
                    double IP_temp = (temperature - 32) / 1.8;// in deg C now
                    double IP_hum = humidity;//Same no change
                    double IP_pressure = 33.8639 * Patm;//in hpa now

                    CalculatePsymetric(IP_temp, IP_hum, IP_pressure);



                    //now output need  to be converted too.
                    double ip_dew_point = output_dew_point * 1.8 + 32;//in Deg F
                    double ip_partial_pressure = output_partial_pressure_of_water * 0.001;//in lb/lb
                    double ip_mixing_ratio = output_mixing_ratio * 0.001; //in lb/lb
                    double ip_enthalpy = output_enthalpy * 0.429923;// in btu/lb
                    double ip_mol_mass = output_mol_mass_of_water * 0.001;
                    //This one is dew point
                    textBox4.Text = Math.Round(ip_dew_point, 4).ToString();
                    textBox8.Text = Math.Round(ip_partial_pressure, 4).ToString();
                    textBox10.Text = Math.Round(ip_mixing_ratio, 4).ToString();
                    textBox11.Text = Math.Round(ip_enthalpy, 4).ToString();
                    textBox9.Text = Math.Round(ip_mol_mass, 4).ToString();


                }
                }//close of if
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                textBox1.Text = "";
                textBox2.Text = "";
                //lets set the values..
                tb_atm_pressure.Text = "";
                //textBox5.Text = "";
                //textBox6.Text = "";
                //textBox7.Text = "";
                textBox9.Text = "";


            }

            //for the EU unit just check if EU unit was selected of not if selected then convert
            //this values here...


        }
        private void Form_Psychrometric_Calculator_Load(object sender, EventArgs e)
        {
            radioButton1.Checked = true;
        }

        int flagWhenFirstRadioClick = 1;
        int flagForSecondRadioClick = 0;
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            //SI clicked 
            unit_temp.Text = "Deg.C";
            unit_ap.Text = "HPa";
            unit_dp.Text = "Deg.C";
            unit_Pws.Text = "HPa";
            unit_mol_mass_over_mol_wt.Text = "g/kg";
            unit_mixed_gas_ratio.Text = "g/kg";
            unit_enthalpy.Text = "KJ/Kg";

            textBox1.Text = "";
            textBox2.Text = "";

            flagWhenFirstRadioClick = 1;
            if(flagForSecondRadioClick == 1)
            {
                //First second is clicked now need to be converted 
                double pressureValue = double.Parse(tb_atm_pressure.Text);
                // double IP_pressure = 33.8639 * Patm;//in hpa now
                pressureValue =  pressureValue* 33.8639;// now this in in Hpa
                tb_atm_pressure.Text = pressureValue.ToString();
                flagForSecondRadioClick = 0;
            }

            if(tb_atm_pressure.Text == "")
            {
                //if empty then
                tb_atm_pressure.Text = "1013";
            }

            textBox1.Text = "";
            textBox2.Text = "";
            textBox4.Text = "";
            textBox8.Text = "";
            textBox10.Text = "";
            textBox11.Text = "";
            textBox9.Text = "";

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            //Imperial unit selected 
            unit_temp.Text = "Deg.F";
            unit_ap.Text = "In. Hg";
            unit_dp.Text = "Deg.F";
            unit_Pws.Text = "In. Hg";
            unit_mol_mass_over_mol_wt.Text = "lb/lb";
            unit_mixed_gas_ratio.Text = "lb/lb";
            unit_enthalpy.Text = "Btu/lb";

            try {
                if(flagWhenFirstRadioClick == 1)
                { 
                    //We have just come form SI to IP so converstion required

                     double pressureValue = double.Parse(tb_atm_pressure.Text);
                    // double IP_pressure = 33.8639 * Patm;//in hpa now
                    pressureValue =  pressureValue/ 33.8639 ;  // now this is in in Hg
                    tb_atm_pressure.Text = pressureValue.ToString();

                    flagWhenFirstRadioClick = 0;
                    flagForSecondRadioClick = 1;
                }
                 if (tb_atm_pressure.Text == "")
                {
                    tb_atm_pressure.Text = "29.913872";
                }

                textBox1.Text = "";
                textBox2.Text = "";
                textBox4.Text = "";
                textBox8.Text = "";
                textBox10.Text = "";
                textBox11.Text = "";
                textBox9.Text = "";

           
            }
            catch(Exception ex)
            {

            }

        }


        //input variable decleration .
        double input_temperature = 0;
        double input_relative_humidity = 0;
        double input_pressure = 0;

        //outputs...
        double output_dew_point = 0;
        double output_partial_pressure_of_water = 0;
        double output_mol_mass_of_water = 0;
        double output_mixing_ratio = 0;
        double output_enthalpy = 0;

        public void CalculatePsymetric(double temp, double hum,double pressure)
        {
            //Contains the formula of the converstion 
            //lets grab from the textbox input.
            double temperature = Double.Parse(textBox1.Text.Trim());
            double humidity = Double.Parse(textBox2.Text.Trim());


            double Patm = pressure;
           
            double A = 6.116441;
            double m = 7.591386;
            double Tn = 240.7263;
            double B = 621.9907;
             
            //calculating 1.dew point..
            double TDewpoint = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature) / (243.04 + temperature))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature) / (243.04 + temperature)));
            //setting to textbox4 its a dew point.
    // textBox4.Text = Convert.ToString(TDewpoint);
            //2.Partial pressure of saturation.

            double Pws = A * Math.Pow(10, (m * TDewpoint) / (TDewpoint + Tn));
   //textBox8.Text = Convert.ToString(Pws);

            double X = B * Pws / (Patm - Pws);
  //   textBox10.Text = Convert.ToString(X);

            double h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
            // textBox11.Text = Convert.ToString(h);

            output_dew_point = TDewpoint;
            output_partial_pressure_of_water = Pws;
            output_mixing_ratio = X;
            output_enthalpy = h;
            output_mol_mass_of_water = B;//this value is in g/kg


        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            //we need to call something on text change....
            if(textBox1.Text != "")
            {
                //if not empty then only do this function 
                calculate();
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            //On text change in 2 do this function
            if(textBox2.Text != "")
            {
                //DO this
                calculate();
            }
        }
    }
}
