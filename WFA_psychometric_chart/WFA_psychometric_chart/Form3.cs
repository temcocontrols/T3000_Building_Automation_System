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
using System.Xml;

namespace WFA_psychometric_chart
{
    public partial class Form3 : Form
    {

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
                //string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";        
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
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
            string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";        
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




}

     

        private void btn_update_now_Click(object sender, EventArgs e)
        {
            /*
             steps:
             * 0.pull the lat and long value stored in the database
             * 1.pull the data form web 
             * 2.Parse the xml data
             * 3.insert data
             * 4.display data
             */
            double lat_val=0.0000;
            double lng_val=0.0000;
            //lets declare some variable to store the value...
            string city_name_pulled="";
            string country_name_pulled="";
            string last_update_pulled="";
            string temp_pulled="";
            string hum_pulled="";
            string pressure_pulled="";
            string wind_speed_pulled="";
            string direction_pulled="";
            string lat_pulled="";
            string long_pulled="";
            


            try
            {
                
                //lets pull the vales offline values stored in db...
                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    OleDbDataReader reader = null;
                    OleDbCommand command = new OleDbCommand("SELECT * from tbl_geo_location_value ", connection);
                    //command.Parameters.AddWithValue("@1", userName)
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        
                        lat_val = double.Parse(reader["latitude"].ToString());
                        lng_val = double.Parse(reader["longitude"].ToString());

                    }
                    connection.Close();
                }

                using (var wc = new WebClient())
                {
                    // var json = await httpClient.GetStringAsync(api_url);


                    //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                    string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat="+lat_val+"&lon="+lng_val+"&appid=615afd606af791f572a1f92b27a68bcd";
                    var data = wc.DownloadString(api_url);
                    //MessageBox.Show("string apic return =" + data);
                    string xml_string = data.ToString();
                    //xml parsing...
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(xml_string);
                    
                    XmlNodeList elem_city = xml.GetElementsByTagName("city");                   
                    foreach (XmlNode x in elem_city)
                    {
                        city_name_pulled = x.Attributes["name"].Value;
                        MessageBox.Show("city name = " + city_name_pulled);
                    }
                    //for temperature
                    XmlNodeList temp_list = xml.GetElementsByTagName("temperature");                   
                    foreach (XmlNode x in temp_list)
                    {
                        temp_pulled = x.Attributes["value"].Value;
                        MessageBox.Show("temp  = " + temp_pulled);
                    }
                    //for humidity
                    XmlNodeList hum_list = xml.GetElementsByTagName("humidity");                    
                    foreach (XmlNode x in hum_list)
                    {
                        hum_pulled = x.Attributes["value"].Value;
                        MessageBox.Show("hum  = " + hum_pulled);
                    }
                    //for pressure..
                    XmlNodeList pressure_list = xml.GetElementsByTagName("pressure");                    
                    foreach (XmlNode x in pressure_list)
                    {
                        pressure_pulled = x.Attributes["value"].Value;
                        MessageBox.Show("press = " + pressure_pulled);
                    }
                    //for wind 

                    XmlNodeList wind_list = xml.GetElementsByTagName("speed");
                    foreach (XmlNode x in wind_list)
                    {
                        wind_speed_pulled = x.Attributes["value"].Value;
                        MessageBox.Show("wind speed = " + wind_speed_pulled);
                    }
                    //for direction..
                    XmlNodeList direction_list = xml.GetElementsByTagName("direction");
                    foreach (XmlNode x in direction_list)
                    {
                        direction_pulled = x.Attributes["name"].Value;
                        MessageBox.Show("direction name = " + direction_pulled);
                    }
                    //for lat and long of station...
                    XmlNodeList coord_list = xml.GetElementsByTagName("coord");
                    foreach (XmlNode x in coord_list)
                    {
                        lat_pulled = x.Attributes["lat"].Value;
                        long_pulled = x.Attributes["lon"].Value;

                        MessageBox.Show("lat = " + lat_pulled +"long" +long_pulled);
                    }
                    //for last date update time 
                    XmlNodeList last_update_list = xml.GetElementsByTagName("lastupdate");
                    foreach (XmlNode x in last_update_list)
                    {
                        last_update_pulled = x.Attributes["value"].Value;
                        MessageBox.Show("last update date = " + last_update_pulled);
                    }
                    
                    //for country..
                    XmlNodeList country_list = xml.GetElementsByTagName("country");
                    foreach (XmlNode x in country_list)
                    {
                        country_name_pulled = x.InnerText;
                        MessageBox.Show("country name = " + country_name_pulled);
                    }

                    //step3. insert the values pulled into a database..

                    //fist calculate lets calculate the distance...
                    /*
                        dlon = lon2 - lon1
                        dlat = lat2 - lat1
                        a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
                        c = 2 * atan2( sqrt(a), sqrt(1-a) )
                        d = R * c (where R is the radius of the Earth) 
                     */
                    string loc_value="";
                    double d=0.0000;
                    double temp_adjust = double.Parse(temp_pulled.ToString()) - 273.15;
                    try{

                    double R = 6371.0;//radius of earth in Km
                    double dlon = double.Parse(long_pulled.ToString()) - lng_val;
                    double dlat=  double.Parse(lat_pulled.ToString()) -lat_val;
                    double a = Math.Pow((Math.Sin(dlat / 2)), 2) + Math.Cos(lat_val) * Math.Cos(double.Parse(lat_pulled.ToString())) * Math.Pow((Math.Sin(dlon / 2)), 2);
                    double c = 2 * Math.Atan2(Math.Sqrt(a), Math.Sqrt(1 - a));
                           d = R * c;//This is the distance
                          loc_value = country_name_pulled + "," + city_name_pulled;
                    using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    string sql_string = "update tbl_weather_related_values set  location=@location_value,distance_from_building=@distance_value,last_update_date=@last_value,temp=@temp_value,humidity=@hum_value,bar_pressure=@pressure_value,wind=@wind_value,direction=@direction_value   where ID = 1;";
                    OleDbCommand command = new OleDbCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                    command.Parameters.AddWithValue("@distance_value", d.ToString());
                    command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                    command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                    command.Parameters.AddWithValue("@hum_value",hum_pulled.ToString());
                    command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                    command.Parameters.AddWithValue("@wind_value",wind_speed_pulled.ToString());
                    command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());

                    command.ExecuteNonQuery();
                    //MessageBox.Show("sql string = " + sql_string);
                    //MessageBox.Show("value updated successfully!");
                   connection.Close();
                
                
            }//close o using..
                    }catch(Exception ex){
                        MessageBox.Show(ex.Message);
                    }

                    //step 4:display the value..
                    tb_location.Text = loc_value;
                    tb_last_updated.Text = last_update_pulled;
                    tb_distance_from_build.Text = d.ToString();
                    tb_cw_temp.Text = temp_adjust.ToString();
                    tb_cw_hum.Text = hum_pulled.ToString();
                    tb_cw_barometer_value.Text = pressure_pulled;
                    tb_cw_wind.Text = wind_speed_pulled.ToString();
                    tb_cw_direction.Text = direction_pulled.ToString();

                    


                    MessageBox.Show("end");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }





        }

        private void button5_Click(object sender, EventArgs e)
        {   
            //storing the value in the database...

            if((tb_longitude.Text != "") && (tb_latitude.Text != ""))
            try
            {
                string lat_value = tb_latitude.Text;
                string long_value = tb_longitude.Text;
                MessageBox.Show("lat= " + lat_value + " lng= " + long_value);
                //string elev_value = tb_elev.Text;
                //lets pull the vales offline values stored in db...
                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";                
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    string sql_string = "update tbl_geo_location_value set   latitude=@latitude_value,longitude=@longitude_value where ID = 1;";
                    OleDbCommand command = new OleDbCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@latitude_value", lat_value);
                    command.Parameters.AddWithValue("@longitude_value", long_value);
                    command.ExecuteNonQuery();
                    //MessageBox.Show("sql string = " + sql_string);
                    MessageBox.Show("value updated successfully!");
                   
                }
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        private void button1_Click(object sender, EventArgs e)
        {
            //This will help to insert the values...

            Form4_insert_data f4 = new Form4_insert_data();
            f4.Show();


        }//close of void update_value_now



      
    }
}
