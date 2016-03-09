using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace WFA_psychometric_chart
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {


            //lets get the geo values....
            try
            {

                if (tb_city.Text != "")
                {
                    //if not null the perform geo coding..
                    var address = tb_city.Text;
                    var requestUri = string.Format("http://maps.googleapis.com/maps/api/geocode/xml?address={0}&sensor=false", Uri.EscapeDataString(address));

                    var request = WebRequest.Create(requestUri);
                    var response = request.GetResponse();

                    var xdoc = XDocument.Load(response.GetResponseStream());

                    var result = xdoc.Element("GeocodeResponse").Element("result");
                    //MessageBox.Show(result.ToString());
                    var locationElement = result.Element("geometry").Element("location");
                    var lat = locationElement.Element("lat");
                    var lng = locationElement.Element("lng");
                    double lat2 = Double.Parse(lat.Value);
                    double lng2 = Double.Parse(lng.Value);

                   // MessageBox.Show(lat.ToString());
                   // MessageBox.Show(lat2.ToString());
                    // double lat_val = double.Parse(lat.ToString());
                    // double long_val = double.Parse(lng.ToString());

                    tb_lat.Text = lat2.ToString();
                    tb_long.Text = lng2.ToString();


                    //double lat =   GetCoordinatesLat(tb_city_name.Text);
                    //double lng = GetCoordinatesLng(tb_city_name.Text);
                    //tb_lat.Text = lat.ToString();
                    //tb_long.Text = lng.ToString();

                    btn_set_value.Enabled = true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                btn_set_value.Enabled = false;
                tb_city.Text = "";
            }





        }

        private void button2_Click(object sender, EventArgs e)
        {

            //lets store this value in a text file and the pull every time when it is called..
            if (tb_lat.Text != null && tb_long.Text != null)
            {
                string[] line = {tb_lat.Text,tb_long.Text};



                //var pat_test = System.Environment.
                //                 GetFolderPath(
                //                     Environment.SpecialFolder.ProgramFiles
                //                 );
                //string fileName = "long_lat_value.txt";
                string path = AppDomain.CurrentDomain.BaseDirectory + @"long_lat_value.txt";
                //MessageBox.Show("the appdomain path= " + path);
                // MessageBox.Show("the path = " + pat_test);
               // string path1 = System.IO.Path.Combine(pat_test, "long_lat_value.txt");
               // MessageBox.Show("the path = " + pat_test);

                System.IO.File.WriteAllLines(path, line);
                MessageBox.Show("The latitude and longitude value is saved successfully");
                


            }



        }
    }
}
