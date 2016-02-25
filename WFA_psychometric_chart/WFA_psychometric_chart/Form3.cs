using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.OleDb;
using System.Net;
using System.Xml.Linq;

namespace WFA_psychometric_chart
{
    public partial class Form3 : Form
    {
        //OleDbCommand cmd = new OleDbCommand();
        //OleDbConnection con = new OleDbConnection();

        public Form3()
        {
            InitializeComponent();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //this will pull the following things from database..
            /*
             1.country,state,city,street,latitude,longitude,elev,zip
             */
            string country = tb_country.Text;
            string state = tb_state.Text;
            string city = tb_city.Text;
            string street = tb_street.Text;

            if (country != "" && city != "")
            {
                string join_string = "";
                if (state != "" && street != "")
                {
                    join_string = country + "," + state + "," + city + "," + street;
                }
                else
                {
                    join_string = country + "," + city;
                }

                //geo location code goes here..
                try
                {

                    var address = join_string;
                    var requestUri = string.Format("http://maps.googleapis.com/maps/api/geocode/xml?address={0}&sensor=false", Uri.EscapeDataString(address));

                    var request = WebRequest.Create(requestUri);
                    var response = request.GetResponse();

                    var xdoc = XDocument.Load(response.GetResponseStream());

                    var result = xdoc.Element("GeocodeResponse").Element("result");                   
                    var locationElement = result.Element("geometry").Element("location");
                    var lat = locationElement.Element("lat");
                    var lng = locationElement.Element("lng");
                    double lat2 = Double.Parse(lat.Value);
                    double lng2 = Double.Parse(lng.Value);
                    tb_latitude.Text = lat2.ToString();
                    tb_longitude.Text = lng2.ToString();
                     

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }






            }//close of if...



        }

        private void button4_Click(object sender, EventArgs e)
        {
            //lets do some operation regarding the pannel id and stuff

            string panelID1 = tb_temp_panel_ID.Text;
            string panelID2 = tb_hum_panel_ID.Text;
            if ((panelID1 != "") &&  (panelID2 != ""))
            {
                //then perform this task 



            }
            else
            {
                MessageBox.Show("Please Enter proper pannel id field");
            }
           

        }

        private void cb_enable_disable_CheckedChanged(object sender, EventArgs e)
        {

            if (cb_enable_disable.Checked == true)
            {
                //enable all the text boxex..
                tb_location.Enabled = true;
                tb_distance_from_build.Enabled = true;
                tb_last_updated.Enabled = true;
                btn_pull_offline_data.Enabled = true;
                btn_update_now.Enabled = true;
                tb_cw_barometer_value.Enabled = true;
                tb_cw_direction.Enabled = true;
                tb_cw_hum.Enabled = true;
                tb_cw_temp.Enabled = true;
                tb_cw_wind.Enabled = true;
                cb_hum_self_calib.Enabled = true;


            }
            else
            {
                //dissable..
                tb_location.Enabled = false;
                tb_distance_from_build.Enabled = false;
                tb_last_updated.Enabled = false;
                btn_pull_offline_data.Enabled = false;
                btn_update_now.Enabled = false;
                tb_cw_barometer_value.Enabled = false;
                tb_cw_direction.Enabled = false;
                tb_cw_hum.Enabled = false;
                tb_cw_temp.Enabled = false;
                tb_cw_wind.Enabled = false;
                cb_hum_self_calib.Enabled =false;
                //dissable the second check box...
                cb_hum_self_calib.Checked = false;

            }


        }

        private void cb_hum_self_calib_CheckedChanged(object sender, EventArgs e)
        {
            //if the self calibration is enabled or dissabled then 
            if (cb_hum_self_calib.Checked == true)
            {

                btn_help.Enabled = true;
                tb_max_adjust.Enabled = true;
                tb_current_offset.Enabled = true;
                btn_set_value.Enabled = true;

            }
            else
            {

                btn_help.Enabled = false;
                tb_max_adjust.Enabled = false;
                tb_current_offset.Enabled = false;
                btn_set_value.Enabled = false;
            }



        }

        private void btn_pull_offline_data_Click(object sender, EventArgs e)
        {
            //here we need to pull the off line data that will be pulled form the web and has been stored in database.
            /*
             steps:
             * 1.pull the data from database.
             * 2.display it in the text box             
             */

            try
            {
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    OleDbDataReader reader = null;
                    OleDbCommand command = new OleDbCommand("SELECT * from tbl_weather_related_values", connection);
                    //command.Parameters.AddWithValue("@1", userName)
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                        tb_location.Text = reader["location"].ToString();
                        tb_distance_from_build.Text = reader["distance_from_building"].ToString();
                        tb_last_updated.Text = reader["last_update_date"].ToString();
                        tb_cw_temp.Text = reader["temp"].ToString();
                        tb_cw_hum.Text = reader["humidity"].ToString();
                        tb_cw_barometer_value.Text = reader["bar_pressure"].ToString();
                        tb_cw_wind.Text = reader["wind"].ToString();
                        tb_cw_direction.Text = reader["direction"].ToString();


                    }
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        private void button6_Click(object sender, EventArgs e)
        {

            //lets pull the vales offline values stored in db...


         string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
        using (OleDbConnection connection = new OleDbConnection(connString))
           {
        connection.Open();
        OleDbDataReader reader = null;
        OleDbCommand command = new OleDbCommand("SELECT * from tbl_building_location", connection);            
        //command.Parameters.AddWithValue("@1", userName)
        reader = command.ExecuteReader();    
        while (reader.Read())
        {
            //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
            tb_country.Text = reader["country"].ToString();
            tb_state.Text = reader["state"].ToString();
            tb_city.Text = reader["city"].ToString();
            tb_street.Text = reader["street"].ToString();
            tb_ZIP.Text = reader["ZIP"].ToString();



        }    
   }




}//close of void btn6



      
    }
}
