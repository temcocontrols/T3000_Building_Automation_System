using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;

namespace WFA_psychometric_chart
{
    public partial class Form5_real_time_plot : Form
    {
        private Form1 form1;
        public Form5_real_time_plot(Form1 form1)
        {
            this.form1 = form1;//defined in top..
            //InitializeComponent();
            InitializeComponent();
        }

        private void btn_plot_Click(object sender, EventArgs e)
        {
            //we need the instance of first form to be passed into this form..

            //steps
            /*
             1.load the data from the test file 
             * 2.parse the data
             * 3.then pass the data to the function of form1.
             * 4.see plot.
             */

            string dir = System.IO.Path.GetDirectoryName(
System.Reflection.Assembly.GetExecutingAssembly().Location);

            string file = dir + @"\oneday_data_test.txt";
            string path1 = file;
            string line1;


            //lets store the data into the arraylist..
            ArrayList al_temp = new ArrayList();
            ArrayList al_hum = new ArrayList();
            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {
                //int i = 0;
                //string s = " ";
                while ((line1 = st.ReadLine()) != null)
                {
                    //line.Skip(1);


                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[1]);
                        double temp2 = Double.Parse(value[3].Trim());
                        //now lets add to temperature and pg array..
                        al_temp.Add(temp1);
                        al_hum.Add(temp2);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

            //now lets test the value..
            try
            {
             //lets plot the values..           
            string s = "";
            for (int i = 0; i < al_temp.Count; i++)
            {
                s += "temp = " + al_temp[i].ToString() + ", hum = " + al_hum[i].ToString() + "\n";
                form1.plot_by_DBT_HR(double.Parse(al_temp[i].ToString()),double.Parse(al_hum[i].ToString())/100);
            }


            MessageBox.Show("data = " + s);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void cb_month_check_all_CheckedChanged(object sender, EventArgs e)
        {

            //if the value is checked the we check all the items of months else none
            if (cb_month_check_all.Checked == true)
            {
                //we gonna check the checkboxes..
                cb1.Checked = true;
                cb2.Checked = true;
                cb3.Checked = true;
                cb4.Checked = true;
                cb5.Checked = true;
                cb6.Checked = true;
                cb7.Checked = true;
                cb8.Checked = true;
                cb9.Checked = true;
                cb10.Checked = true;
                cb11.Checked = true;
                cb12.Checked = true;
            }
            else
            {
                //we gonna uncheck..
                cb1.Checked = false;
                cb2.Checked = false;
                cb3.Checked = false;
                cb4.Checked = false;
                cb5.Checked = false;
                cb6.Checked = false;
                cb7.Checked = false;
                cb8.Checked = false;
                cb9.Checked = false;
                cb10.Checked = false;
                cb11.Checked = false;
                cb12.Checked = false;


            }



        }

        private void cb_hour_check_all_CheckedChanged(object sender, EventArgs e)
        {

            //if the value is checked the we check all the items of months else none
            if (cb_hour_check_all.Checked == true)
            {
                //we gonna check the checkboxes..
                cb20.Checked = true;
                cb21.Checked = true;
                cb22.Checked = true;
                cb23.Checked = true;
                cb24.Checked = true;
                cb25.Checked = true;
                cb26.Checked = true;
                cb27.Checked = true;
                cb28.Checked = true;
                cb29.Checked = true;
                cb30.Checked = true;
                cb31.Checked = true;
                cb32.Checked = true;
                cb33.Checked = true;
                cb34.Checked = true;
                cb35.Checked = true;
                cb36.Checked = true;
                cb37.Checked = true;
                cb38.Checked = true;
                cb39.Checked = true;
                cb40.Checked = true;
                cb41.Checked = true; 
                cb42.Checked = true;
                cb43.Checked = true;

            }
            else
            {
                //we gonna uncheck..
                cb20.Checked = false;
                cb21.Checked = false;
                cb22.Checked = false;
                cb23.Checked = false;
                cb24.Checked = false;
                cb25.Checked = false;
                cb26.Checked = false;
                cb27.Checked = false;
                cb28.Checked = false;
                cb29.Checked = false;
                cb30.Checked = false;
                cb31.Checked = false;
                cb32.Checked = false;
                cb33.Checked = false;
                cb34.Checked = false;
                cb35.Checked = false;
                cb36.Checked = false;
                cb37.Checked = false;
                cb38.Checked = false;
                cb39.Checked = false;
                cb40.Checked = false;
                cb41.Checked = false;
                cb42.Checked = false;
                cb43.Checked = false;


            }



        }

    

        
    }
}
