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
using System.Collections;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;


namespace WFA_psychometric_chart
{
    public partial class Form3 : Form
    {

        public Form3()
        {
            InitializeComponent();
        }

        
        int index_selected = 0;
        int index_for_station_selected = 0;
        //private void button3_Click(object sender, EventArgs e)
        //{
        //    //this will pull the following things from database..
        //    /*
        //     1.country,state,city,street,latitude,longitude,elev,zip
        //     */
        //    string country = tb_country.Text;
        //    string state = tb_state.Text;
        //    string city = tb_city.Text;
        //    string street = tb_street.Text;

        //    if (country != "" && city != "")
        //    {
        //        string join_string = "";
        //        if (state != "" && street != "")
        //        {
        //            join_string = country + "," + state + "," + city + "," + street;
        //        }
        //        else
        //        {
        //            join_string = country + "," + city;
        //        }

        //        //geo location code goes here..
        //        try
        //        {

        //            var address = join_string;
        //            var requestUri = string.Format("http://maps.googleapis.com/maps/api/geocode/xml?address={0}&sensor=false", Uri.EscapeDataString(address));

        //            var request = WebRequest.Create(requestUri);
        //            var response = request.GetResponse();

        //            var xdoc = XDocument.Load(response.GetResponseStream());

        //            var result = xdoc.Element("GeocodeResponse").Element("result");                   
        //            var locationElement = result.Element("geometry").Element("location");
        //            var lat = locationElement.Element("lat");
        //            var lng = locationElement.Element("lng");
        //            double lat2 = Double.Parse(lat.Value);
        //            double lng2 = Double.Parse(lng.Value);
        //            tb_latitude.Text = lat2.ToString();
        //            tb_longitude.Text = lng2.ToString();
                     

        //        }
        //        catch (Exception ex)
        //        {
        //            MessageBox.Show(ex.Message);
        //        }






        //    }//close of if...



        //}

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
           
            //just a fix value for now..temp=25,hum=85
            tb_temp_panel_value.Text = "25".ToString();
            tb_hum_panel_value.Text = "85".ToString();


        }

        //this is used for dynamic list creation...
        private class station_data
        {
           public string name { get; set; }
            public double distance { get; set; }
            public double id { get; set; }
            public double lat { get; set; }
            public double lng { get; set; }
        }

        List<station_data> store_station_list = new List<station_data>();
        private void cb_enable_disable_CheckedChanged(object sender, EventArgs e)
        {


            if (cb_enable_disable.Checked == true)
            {
                try
                {
                store_station_list.Clear();
                //enable all the text boxex..
                tb_location.Enabled = true;
                tb_distance_from_build.Enabled = true;
                tb_last_updated.Enabled = true;
                btn_pull_offline_data.Enabled = true;
                //btn_update_now.Enabled = true;//this will be true when user selects a station 
                tb_cw_barometer_value.Enabled = true;
                tb_cw_direction.Enabled = true;
                tb_cw_hum.Enabled = true;
                tb_cw_temp.Enabled = true;
                tb_cw_wind.Enabled = true;
                cb_hum_self_calib.Enabled = true;
                //btn_update_constantly.Enabled = true;//this will be true when user selects a station 
                cb_station_names.Enabled = true;
                tb_station_distance.Enabled = true;


                /*
                 we should try to pull the data of the station names based on the location selected.
                 * 1. Get lat lng value then pull 5 station name by parsing the json file form the openweathermap.org
                 */

               
                    double lat = Double.Parse(tb_latitude.Text);
                    double lng = Double.Parse(tb_longitude.Text);

                    //after this lets get the web url data 
                    using (var wc = new WebClient())
                    {
                        // var json = await httpClient.GetStringAsync(api_url);


                        //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                        string api_url = "http://api.openweathermap.org/data/2.5/station/find?mode=json&lat=" + lat + "&lon=" + lng + "&cnt=5&APPID=615afd606af791f572a1f92b27a68bcd";
                        //MessageBox.Show("api_url = " + api_url);
                        var data = wc.DownloadString(api_url);
                       //its a test...                       
                      //  MessageBox.Show("data = " + data);
                        //lets parse the data provided...
                        //and store it into dynamic list...
                        //lets reset the list first..
                        
                        var jarray = JArray.Parse(data);

                        foreach (var result in jarray.Children<JObject>())
                        {

                                if ((result["station"]["name"] != null) && (result["distance"] != null))
                                {
                                    string station_name = result["station"]["name"].ToString();

                                    string station_distance = result["distance"].ToString();
                                    string station_id = result["station"]["id"].ToString();
                                    string latitude = result["station"]["coord"]["lat"].ToString();
                                    string longitude = result["station"]["coord"]["lon"].ToString();
                                    store_station_list.Add(new station_data
                                    {
                                        name = station_name,
                                        distance = double.Parse(station_distance),
                                        id= double.Parse(station_id),
                                        lat = double.Parse(latitude),
                                        lng = double.Parse(longitude)


                                    });
                                }



                           
                        }//close of foreach...

                        //lets print the values. if we are rightr
                        string s = null;
                        cb_station_names.Items.Clear();
                        for (int i = 0; i < store_station_list.Count; i++)
                        {
                            s += " name  = " + store_station_list[i].name + " dist = " + store_station_list[i].distance + " \n ";
                            //lets set the value to cb so that it gets the exact value...
                            cb_station_names.Items.Add(store_station_list[i].name);

                        }
                        MessageBox.Show("values  = " + s);

                    }//close of using webclient

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message+"\n Please make sure you are connected to internet!");
                    //cb_enable_disable.Enabled = false;
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
                    cb_hum_self_calib.Enabled = false;
                    btn_update_constantly.Enabled = false;
                    //dissable the second check box...
                    cb_hum_self_calib.Checked = false;
                    cb_station_names.Enabled = false;
                    tb_station_distance.Enabled = false;
                    cb_enable_disable.Checked = false;

                }



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
                btn_update_constantly.Enabled = false;
                //dissable the second check box...
                cb_hum_self_calib.Checked = false;
                cb_station_names.Enabled = false;
                tb_station_distance.Enabled = false;



                tb_location.Text = "";
                tb_distance_from_build.Text= "";
                tb_last_updated.Text = "";
                //btn_pull_offline_data.Enabled = true;
                //btn_update_now.Enabled = true;//this will be true when user selects a station 
                tb_cw_barometer_value.Text = "";
                tb_cw_direction.Text = "";
                tb_cw_hum.Text= "";
                tb_cw_temp.Text = "";
                tb_cw_wind.Text = "";
                //cb_hum_self_calib.Enabled = true;
                //btn_update_constantly.Enabled = true;//this will be true when user selects a station 
                //cb_station_names.Enabled = true;
                tb_station_distance.Text = "";

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


        public void pull_stored_weather_data()
        {
            //this pulls the offline data..



            //here we need to pull the off line data that will be pulled form the web and has been stored in database.
            /*
             steps:
             * 1.pull the data from database.
             * 2.display it in the text box             
             */


            try
            {

                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {

                    string error_string = "Please select a location ";
                    MessageBox.Show(error_string, "error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    // MessageBox.Show("Please perform the follwing step first.\n 1.select a location \n 2.pull stored building data \n 3. get geo value(we need geo value)");
                }
                else
                {



                    //string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                    string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                    using (OleDbConnection connection = new OleDbConnection(connString))
                    {
                        connection.Open();
                        OleDbDataReader reader = null;
                        OleDbCommand command = new OleDbCommand("SELECT * from tbl_weather_related_values where ID=@id", connection);
                        command.Parameters.AddWithValue("@id", index_selected);
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
                            tb_station_name.Text = reader["station_name"].ToString();


                        }
                    }
                }//close of else block..

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }


        private void btn_pull_offline_data_Click(object sender, EventArgs e)
        {
            pull_stored_weather_data();
        }

        private void get_stored_data()
        {
            //try
            //{
            if (cb1_select_data.SelectedIndex > -1)
            {
                //lets get the index parameter form the table...
                index_selected = cb1_select_data.SelectedIndex + 1; //
                //MessageBox.Show("index = " + index_selected);

                //lets pull the vales offline values stored in db...
                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    OleDbDataReader reader = null;
                    string queryString = "SELECT * from tbl_building_location WHERE ID=@index";
                    OleDbCommand command = new OleDbCommand(queryString, connection);
                    //command.Parameters.AddWithValue("@index", index);
                    command.Parameters.Add(new OleDbParameter("@index", OleDbType.Integer)).Value = index_selected;
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                        tb_country.Text = reader["country"].ToString();
                        tb_state.Text = reader["state"].ToString();
                        tb_city.Text = reader["city"].ToString();
                        tb_street.Text = reader["street"].ToString();
                        tb_ZIP.Text = reader["ZIP"].ToString();
                        tb_latitude.Text = reader["latitude"].ToString();
                        tb_longitude.Text = reader["longitude"].ToString();
                        tb_elev.Text = reader["elevation"].ToString();

                    }
                }
            }//close of if statement
            else
            {
                MessageBox.Show("Please select an item.\n If you dont have a location then please insert first.");
            }

            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

        }



        //private void button6_Click(object sender, EventArgs e)
        //{
        //    get_stored_data();
        //}


      
     

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


            if (index_for_station_selected > -1) { 

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

                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {

                    string error_string = "Please select a location ";
                    MessageBox.Show(error_string, "error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                   // MessageBox.Show("Please perform the follwing step first.\n 1.select a location \n 2.pull stored building data \n 3. get geo value(we need geo value)");
                }
                else {

                    lat_val = store_station_list[index_for_station_selected].lat;
                    lng_val = store_station_list[index_for_station_selected].lng;
                //display lat lng...
                //MessageBox.Show("lat= "+lat_val+" lng = "+lng_val);
                using (var wc = new WebClient())
                {
                    // var json = await httpClient.GetStringAsync(api_url);

                   // double station_id = store_station_list[index_for_station_selected].id;
                    //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                    string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat="+lat_val+"&lon="+lng_val+"&appid=615afd606af791f572a1f92b27a68bcd";
                    //string api_url = "http://api.openweathermap.org/data/2.5/station?id="+station_id+"&APPID=615afd606af791f572a1f92b27a68bcd";

                    var data = wc.DownloadString(api_url);
                    MessageBox.Show("string apic return =" + data);
                    string xml_string = data.ToString();
                   
                    
                    
                    
                    
                    //xml parsing...
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(xml_string);
                    
                    XmlNodeList elem_city = xml.GetElementsByTagName("city");                   
                    foreach (XmlNode x in elem_city)
                    {
                        city_name_pulled = x.Attributes["name"].Value;
                       // MessageBox.Show("city name = " + city_name_pulled);
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
                        //MessageBox.Show("hum  = " + hum_pulled);
                    }
                    //for pressure..
                    XmlNodeList pressure_list = xml.GetElementsByTagName("pressure");                    
                    foreach (XmlNode x in pressure_list)
                    {
                        pressure_pulled = x.Attributes["value"].Value;
                        //MessageBox.Show("press = " + pressure_pulled);
                    }
                    //for wind 

                    XmlNodeList wind_list = xml.GetElementsByTagName("speed");
                    foreach (XmlNode x in wind_list)
                    {
                        wind_speed_pulled = x.Attributes["value"].Value;
                     //   MessageBox.Show("wind speed = " + wind_speed_pulled);
                    }
                    //for direction..
                    XmlNodeList direction_list = xml.GetElementsByTagName("direction");
                    foreach (XmlNode x in direction_list)
                    {
                        direction_pulled = x.Attributes["name"].Value;
                       // MessageBox.Show("direction name = " + direction_pulled);
                    }
                    //for lat and long of station...
                    XmlNodeList coord_list = xml.GetElementsByTagName("coord");
                    foreach (XmlNode x in coord_list)
                    {
                        lat_pulled = x.Attributes["lat"].Value;
                        long_pulled = x.Attributes["lon"].Value;

                       // MessageBox.Show("lat = " + lat_pulled +"long" +long_pulled);
                    }
                    //for last date update time 
                    XmlNodeList last_update_list = xml.GetElementsByTagName("lastupdate");
                    foreach (XmlNode x in last_update_list)
                    {
                        last_update_pulled = x.Attributes["value"].Value;
                       // MessageBox.Show("last update date = " + last_update_pulled);
                    }
                    
                    //for country..
                    XmlNodeList country_list = xml.GetElementsByTagName("country");
                    foreach (XmlNode x in country_list)
                    {
                        country_name_pulled = x.InnerText;
                      //  MessageBox.Show("country name = " + country_name_pulled);
                    }


                    //xml parsing closes but we need json parser..







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
                        //lets check a simple thing if the index is pressent then update else insert the data...
                        
                        /*steps.. count the number of items in a column 
                         1.if the count is less than the index_selected then update else insert
                         */



                    using (OleDbConnection connection = new OleDbConnection(connString))
                {
                    connection.Open();
                    int count_col_table=0;
                        //lets check the data first...
                        //step1.
                    string count_num_column = "select count(id) from tbl_weather_related_values ";
                    OleDbCommand cmd1 = new OleDbCommand(count_num_column, connection);
                    OleDbDataReader reader = cmd1.ExecuteReader();
                    while (reader.Read())
                    {
                        //lets get the value...
                        count_col_table = Convert.ToInt32(reader[0].ToString());
                    }
                    MessageBox.Show("count_col_table  =" + count_col_table);
                        //step2. condition
                    if (count_col_table >= index_selected)
                    {
                        //update previous value

                        string sql_string = "update tbl_weather_related_values set  location=@location_value,distance_from_building=@distance_value,last_update_date=@last_value,temp=@temp_value,humidity=@hum_value,bar_pressure=@pressure_value,wind=@wind_value,direction=@direction_value,station_name=@station_name   where ID = @index_selected;";
                        OleDbCommand command = new OleDbCommand(sql_string, connection);
                        command.CommandType = CommandType.Text;
                        command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                        command.Parameters.AddWithValue("@distance_value", d.ToString());
                        command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                        command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                        command.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                        command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                        command.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                        command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                        command.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                        command.Parameters.AddWithValue("@index_selected", index_selected);                        
                        MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                        MessageBox.Show("updating...");

                        command.ExecuteNonQuery();
                        //MessageBox.Show("sql string = " + sql_string);
                        //MessageBox.Show("value updated successfully!");

                    }
                    else
                    {
                        //insert...
                        string sql_string = "insert into tbl_weather_related_values(location,distance_from_building,last_update_date,temp,humidity,bar_pressure,wind,direction,station_name) VALUES(@location_value,@distance_value,@last_value,@temp_value,@hum_value,@pressure_value,@wind_value,@direction_value,@station_name)";
                        OleDbCommand command = new OleDbCommand(sql_string, connection);
                        command.CommandType = CommandType.Text;
                        command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                        command.Parameters.AddWithValue("@distance_value", d.ToString());
                        command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                        command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                        command.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                        command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                        command.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                        command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                        command.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                        MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                        command.ExecuteNonQuery();
                    }
                   connection.Close();
                
                
            }//close o using..
                    }catch(Exception ex){
                        MessageBox.Show(ex.Message);
                    }

                    //step 4:display the value..
                    //tb_location.Text = loc_value;
                    //tb_last_updated.Text = last_update_pulled;
                    //tb_distance_from_build.Text = d.ToString();
                    //tb_cw_temp.Text = temp_adjust.ToString();
                    //tb_cw_hum.Text = hum_pulled.ToString();
                    //tb_cw_barometer_value.Text = pressure_pulled;
                    //tb_cw_wind.Text = wind_speed_pulled.ToString();
                    //tb_cw_direction.Text = direction_pulled.ToString();
                    /* instead of reading the values lets pull the data...*/
                    pull_stored_weather_data();
                    


                    MessageBox.Show("success !");
                }

                }//close of else...block for tb_latitude and tb_longitude..

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


            }//close of index_for_station_selected>-1
            else
            {
                MessageBox.Show("please select a station first.");
            }

        
        }//close of btn private void..

        //private void button5_Click(object sender, EventArgs e)
        //{   
        //    //storing the value in the database...
        //    if (index_selected > 0)
        //    {
             
   
        //    if((tb_longitude.Text != "") && (tb_latitude.Text != ""))
        //    try
        //    {
        //        string lat_value = tb_latitude.Text;
        //        string long_value = tb_longitude.Text;
        //        MessageBox.Show("lat= " + lat_value + " lng= " + long_value);
        //        //string elev_value = tb_elev.Text;
        //        //lets pull the vales offline values stored in db...
        //        string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
        //        //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";                
        //        string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
        //        using (OleDbConnection connection = new OleDbConnection(connString))
        //        {
        //            connection.Open();
        //            string sql_string = "update tbl_building_location set   latitude=@latitude_value,longitude=@longitude_value where ID = @index_selected;";
        //            OleDbCommand command = new OleDbCommand(sql_string, connection);
        //            command.CommandType = CommandType.Text;
        //            command.Parameters.AddWithValue("@latitude_value", lat_value);
        //            command.Parameters.AddWithValue("@longitude_value", long_value);
        //            command.Parameters.AddWithValue("@index_selected", index_selected);

        //            command.ExecuteNonQuery();
        //            //MessageBox.Show("sql string = " + sql_string);
        //            MessageBox.Show("value updated successfully!");
                   
        //        }
                
        //    }
        //    catch (Exception ex)
        //    {
        //        MessageBox.Show(ex.Message);
        //    }

        //    }
        //    else
        //    {
        //        //print error..
        //        MessageBox.Show("Please pull the store data first..");
        //    }

        //}

        private void button1_Click(object sender, EventArgs e)
        {
            //This will help to insert the values...

            Form4_insert_data f4 = new Form4_insert_data(this);
            f4.Show();


        }

        public void fill_combobox()
        {
            cb1_select_data.Items.Clear();
            ArrayList stored_location = new ArrayList();
            //while loading it should populate the field...
            //lets pull the vales offline values stored in db...
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
            string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
           // MessageBox.Show("connection string = " + connString);


            OleDbConnection connection = new OleDbConnection(connString);
                connection.Open();
                OleDbDataReader reader = null;
                OleDbCommand comm = new OleDbCommand("SELECT * from tbl_building_location", connection);
                //command.Parameters.AddWithValue("@1", userName)
                reader = comm.ExecuteReader();
                while (reader.Read())
                {
                   
                    string selecte_location = reader["id"].ToString()+","+reader["country"].ToString() + "," + reader["state"].ToString() + "," + reader["city"].ToString();
                    stored_location.Add(selecte_location);
                }
                string s = "";
                for (int i = 0; i < stored_location.Count; i++)
                {
                    cb1_select_data.Items.Add(stored_location[i]);
                    s += stored_location[i] + " , \n";
                }
               // MessageBox.Show("stored place = " + s);
                comm.Dispose();    
            reader.Dispose();    
            connection.Close();

            



        }

        private void Form3_Load(object sender, EventArgs e)
        {
            fill_combobox();

        }

        //private void button2_Click(object sender, EventArgs e)
        //{
        //    fill_combobox();
        //}

        public void update_data_constantly()
        {
            //this is for updating the data every 1 hours...

            /*
             steps:
             * 0.pull the lat and long value stored in the database
             * 1.pull the data form web 
             * 2.Parse the xml data
             * 3.insert data
             * 4.display data
             */
            double lat_val = 0.0000;
            double lng_val = 0.0000;
            //lets declare some variable to store the value...
            string city_name_pulled = "";
            string country_name_pulled = "";
            string last_update_pulled = "";
            string temp_pulled = "";
            string hum_pulled = "";
            string pressure_pulled = "";
            string wind_speed_pulled = "";
            string direction_pulled = "";
            string lat_pulled = "";
            string long_pulled = "";



            try
            {

                //lets pull the vales offline values stored in db...
                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";

                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {
                    MessageBox.Show("Please perform the follwing step first.\n 1.select a location \n 2.pull stored building data \n 3. get geo value(we need geo value)");
                }
                else
                {

                    //lat_val = Double.Parse(tb_latitude.Text);
                    //lng_val = Double.Parse(tb_longitude.Text);
                    lat_val = store_station_list[index_for_station_selected].lat;
                    lng_val = store_station_list[index_for_station_selected].lng;
                    //display lat lng...
                  //  MessageBox.Show("lat= " + lat_val + " lng = " + lng_val);
                    using (var wc = new WebClient())
                    {
                        // var json = await httpClient.GetStringAsync(api_url);


                        //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                        string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat=" + lat_val + "&lon=" + lng_val + "&appid=615afd606af791f572a1f92b27a68bcd";

                        //double station_id = store_station_list[index_for_station_selected].id;
                        //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                        //string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat="+lat_val+"&lon="+lng_val+"&appid=615afd606af791f572a1f92b27a68bcd";
                        //string api_url = "http://api.openweathermap.org/data/2.5/station?id=" + station_id + "&APPID=615afd606af791f572a1f92b27a68bcd";

                        
                        var data = wc.DownloadString(api_url);
                        MessageBox.Show("string apic return =" + data);
                        string xml_string = data.ToString();
                        //xml parsing...
                        XmlDocument xml = new XmlDocument();
                        xml.LoadXml(xml_string);

                        XmlNodeList elem_city = xml.GetElementsByTagName("city");
                        foreach (XmlNode x in elem_city)
                        {
                            city_name_pulled = x.Attributes["name"].Value;
                        //    MessageBox.Show("city name = " + city_name_pulled);
                        }
                        //for temperature
                        XmlNodeList temp_list = xml.GetElementsByTagName("temperature");
                        foreach (XmlNode x in temp_list)
                        {
                            temp_pulled = x.Attributes["value"].Value;
                          //  MessageBox.Show("temp  = " + temp_pulled);
                        }
                        //for humidity
                        XmlNodeList hum_list = xml.GetElementsByTagName("humidity");
                        foreach (XmlNode x in hum_list)
                        {
                            hum_pulled = x.Attributes["value"].Value;
                           // MessageBox.Show("hum  = " + hum_pulled);
                        }
                        //for pressure..
                        XmlNodeList pressure_list = xml.GetElementsByTagName("pressure");
                        foreach (XmlNode x in pressure_list)
                        {
                            pressure_pulled = x.Attributes["value"].Value;
                           // MessageBox.Show("press = " + pressure_pulled);
                        }
                        //for wind 

                        XmlNodeList wind_list = xml.GetElementsByTagName("speed");
                        foreach (XmlNode x in wind_list)
                        {
                            wind_speed_pulled = x.Attributes["value"].Value;
                           // MessageBox.Show("wind speed = " + wind_speed_pulled);
                        }
                        //for direction..
                        XmlNodeList direction_list = xml.GetElementsByTagName("direction");
                        foreach (XmlNode x in direction_list)
                        {
                            direction_pulled = x.Attributes["name"].Value;
                          //  MessageBox.Show("direction name = " + direction_pulled);
                        }
                        //for lat and long of station...
                        XmlNodeList coord_list = xml.GetElementsByTagName("coord");
                        foreach (XmlNode x in coord_list)
                        {
                            lat_pulled = x.Attributes["lat"].Value;
                            long_pulled = x.Attributes["lon"].Value;

                            //MessageBox.Show("lat = " + lat_pulled + "long" + long_pulled);
                        }
                        //for last date update time 
                        XmlNodeList last_update_list = xml.GetElementsByTagName("lastupdate");
                        foreach (XmlNode x in last_update_list)
                        {
                            last_update_pulled = x.Attributes["value"].Value;
                           // MessageBox.Show("last update date = " + last_update_pulled);
                        }

                        //for country..
                        XmlNodeList country_list = xml.GetElementsByTagName("country");
                        foreach (XmlNode x in country_list)
                        {
                            country_name_pulled = x.InnerText;
                           // MessageBox.Show("country name = " + country_name_pulled);
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
                        string loc_value = "";
                        double d = 0.0000;
                        double temp_adjust = double.Parse(temp_pulled.ToString()) - 273.15;
                        try
                        {

                            double R = 6371.0;//radius of earth in Km
                            double dlon = double.Parse(long_pulled.ToString()) - lng_val;
                            double dlat = double.Parse(lat_pulled.ToString()) - lat_val;
                            double a = Math.Pow((Math.Sin(dlat / 2)), 2) + Math.Cos(lat_val) * Math.Cos(double.Parse(lat_pulled.ToString())) * Math.Pow((Math.Sin(dlon / 2)), 2);
                            double c = 2 * Math.Atan2(Math.Sqrt(a), Math.Sqrt(1 - a));
                            d = R * c;//This is the distance
                            loc_value = country_name_pulled + "," + city_name_pulled;
                            //lets check a simple thing if the index is pressent then update else insert the data...

                            /*steps.. count the number of items in a column 
                             1.if the count is less than the index_selected then update else insert
                             */



                            using (OleDbConnection connection = new OleDbConnection(connString))
                            {
                                connection.Open();
                                int count_col_table = 0;
                                //lets check the data first...
                                //step1.
                                string count_num_column = "select count(id) from tbl_weather_related_values ";
                                OleDbCommand cmd1 = new OleDbCommand(count_num_column, connection);
                                OleDbDataReader reader = cmd1.ExecuteReader();
                                while (reader.Read())
                                {
                                    //lets get the value...
                                    count_col_table = Convert.ToInt32(reader[0].ToString());
                                }
                               // MessageBox.Show("count_col_table  =" + count_col_table);
                                //step2. condition
                                if (count_col_table >= index_selected)
                                {
                                    //update previous value

                                    string sql_string = "update tbl_weather_related_values set  location=@location_value,distance_from_building=@distance_value,last_update_date=@last_value,temp=@temp_value,humidity=@hum_value,bar_pressure=@pressure_value,wind=@wind_value,direction=@direction_value,station_name=@station_name   where ID = @index_selected;";
                                    OleDbCommand command = new OleDbCommand(sql_string, connection);
                                    command.CommandType = CommandType.Text;
                                    command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                                    command.Parameters.AddWithValue("@distance_value", d.ToString());
                                    command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                                    command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                                    command.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                                    command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                                    command.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                                    command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                                    command.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                                    command.Parameters.AddWithValue("@index_selected", index_selected);
                                  
                                    //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());

                                    command.ExecuteNonQuery();
                                    //MessageBox.Show("sql string = " + sql_string);
                                    //MessageBox.Show("value updated successfully!");

                                }
                                else
                                {
                                    //insert...
                                    string sql_string = "insert into tbl_weather_related_values(location,distance_from_building,last_update_date,temp,humidity,bar_pressure,wind,direction,station_name) VALUES(@location_value,@distance_value,@last_value,@temp_value,@hum_value,@pressure_value,@wind_value,@direction_value,@station_name)";
                                    OleDbCommand command = new OleDbCommand(sql_string, connection);
                                    command.CommandType = CommandType.Text;
                                    command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                                    command.Parameters.AddWithValue("@distance_value", d.ToString());
                                    command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                                    command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                                    command.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                                    command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                                    command.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                                    command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                                    command.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                                    //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                                    command.ExecuteNonQuery();
                                }
                                connection.Close();


                            }//close o using..
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                        }

                        //step 4:display the value..
                        //tb_location.Text = loc_value;
                        //tb_last_updated.Text = last_update_pulled;
                        //tb_distance_from_build.Text = d.ToString();
                        //tb_cw_temp.Text = temp_adjust.ToString();
                        //tb_cw_hum.Text = hum_pulled.ToString();
                        //tb_cw_barometer_value.Text = pressure_pulled;
                        //tb_cw_wind.Text = wind_speed_pulled.ToString();
                        //tb_cw_direction.Text = direction_pulled.ToString();

                        pull_stored_weather_data();

                        MessageBox.Show("pulled value =" + "\n loc = " + loc_value + "\nlast updated= " + last_update_pulled + "\ntemp = " + temp_pulled + "\nhum = " + hum_pulled + "\n wind =" + wind_speed_pulled + "\ndirection = " + direction_pulled);

                      //  MessageBox.Show("end");
                    }

                }//close of else...block for tb_latitude and tb_longitude..

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        private System.Windows.Forms.Timer timer1= new System.Windows.Forms.Timer();
        public void InitTimer()
        {
            //timer1 = new System.Windows.Forms.Timer();
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Interval = 1000 * 60; // in miliseconds //2min * 30 = 60 min minute ie every 1 hour
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
          //  MessageBox.Show("pulling...");
              update_data_constantly();
            //MessageBox.Show("pulled...");
        }


        private void button7_Click(object sender, EventArgs e)
        {
            //this code basically makes the upadating part constantly...
            //it calls the function UpdateDataConstantly()           
            //this function basically calls every  50 minuest...
            if (index_for_station_selected > -1)
            {

                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {
                    string error_string = "Please select a location first !";
                    MessageBox.Show(error_string, "error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {
                    MessageBox.Show("pulling...");
                    InitTimer();

                }
            }//close of if index_for...
            else
            {
                MessageBox.Show("Please select a station first to pull data.");
            }

        }

        private void btn_help_Click(object sender, EventArgs e)
        {


            string error_string = "In this section you select the maximum adjustment factor per day.If the value is deviated you could set the value of max adjustment factore as 0.01 and it will adjust your humidity as  per the 0.01% value. The total current offset is used to show by how much percent the value is updated every day and the total changed from last set.";
            MessageBox.Show(error_string, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);


        }
       public int adjusted_value =0;
       double sensor_hum = 0;
       double current_weather_hum = 0;
       //double temp_from_weather = 0;
       double total_percentage_change = 0;
        public void adjust_hum_value()
        {

            if (tb_max_adjust.Text != "")
            {


                if (tb_hum_panel_value.Text == "")
                {
                    MessageBox.Show("Please have a proper value selected from sensor \n Perform get value for humidity from sensor");
                }
                else
                {
                    //we need to do something with this sensor value..
                    if (tb_cw_hum.Text == "")
                    {
                        MessageBox.Show("Please pull the current weather value first");
                    }
                    else
                    {
                    
                        if (adjusted_value == 0) { 
                        //we need to compare the two values..
                         
                         adjusted_value++;
                        }
                       
                        double adjust_factor = Double.Parse(tb_max_adjust.Text);
                        //adjusting by 1 degree up and down value.
                        if (sensor_hum < current_weather_hum - 1)
                        {
                            //decrease the sensor humidity..
                            sensor_hum += sensor_hum * adjust_factor/100;


                            total_percentage_change += adjust_factor;
                            tb_current_offset_percent.Text = total_percentage_change.ToString();
                        }
                        else if (sensor_hum > current_weather_hum + 1)
                        {
                            sensor_hum -= sensor_hum * adjust_factor/100;

                            total_percentage_change -= adjust_factor;
                            tb_current_offset_percent.Text = total_percentage_change.ToString();
                        }

                        tb_current_offset.Text = sensor_hum.ToString();
                        /*percentage..*/
                        //double percent = (current_weather_hum - sensor_hum) / current_weather_hum * 100;
                        //tb_current_offset_percent.Text = percent.ToString();
                       
                    }

                }

            }
            else
            {
                string error_string = "Please enter a value for adjustment factor";
                MessageBox.Show(error_string, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private System.Windows.Forms.Timer timer2 = new System.Windows.Forms.Timer();
        public void InitTimer2()
        {
            //timer2 = new System.Windows.Forms.Timer();
            timer2.Tick += new EventHandler(timer2_Tick);
            timer2.Interval = 1000 * 3; // in miliseconds //2min * 30 = 60 min minute ie every 1 hour

            if (sensor_hum > current_weather_hum + 1 && sensor_hum < current_weather_hum - 1)
            {
                //if sensor is around current weather condition..
                timer2.Stop();
            }
            else { 
                timer2.Start();
            }
          
        }

        
        private void timer2_Tick(object sender, EventArgs e)
        {
            //  MessageBox.Show("pulling...");            
            adjust_hum_value();
            //MessageBox.Show("pulled...");
        }


        private void btn_set_value_Click(object sender, EventArgs e)
        {
            try
            {
                if (tb_hum_panel_value.Text != "")
                {
                    if (tb_cw_hum.Text != "")
                    {
                        if (tb_max_adjust.Text != "")
                        { double x = double.Parse(tb_max_adjust.Text);
                        if (x >= 0 && x <= 2) { 
                            //temp_from_weather = double.Parse(tb_cw_temp.ToString());
                            sensor_hum = Double.Parse(tb_hum_panel_value.Text);
                            current_weather_hum = Double.Parse(tb_cw_hum.Text);
                            InitTimer2();
                        }
                        else
                        {
                            MessageBox.Show("Please enter value between 0-2%");
                            tb_max_adjust.Text = "";
                        }
                        }
                        else
                        {
                            MessageBox.Show("Please enter a valid adjustment number eg. 1% would be 0.01");
                        }
                    }
                    else
                    {
                        MessageBox.Show("Please get and set the current weather data");
                    }
                }
                else
                {
                    MessageBox.Show("Please set the value from sensor");
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void Form3_ClosingForm(object sender, FormClosingEventArgs e)
        {
            timer2.Dispose();
            timer1.Dispose();
            this.Dispose();
        }

        private void cb_event_on_index_change(object sender, EventArgs e)
        {
            //fill_combobox();
            cb_enable_disable.Enabled = false;



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
            cb_hum_self_calib.Enabled = false;
            btn_update_constantly.Enabled = false;
            //dissable the second check box...
            cb_hum_self_calib.Checked = false;
            cb_station_names.Enabled = false;
            tb_station_distance.Enabled = false;



            tb_location.Text = "";
            tb_distance_from_build.Text = "";
            tb_last_updated.Text = "";
            //btn_pull_offline_data.Enabled = true;
            //btn_update_now.Enabled = true;//this will be true when user selects a station 
            tb_cw_barometer_value.Text = "";
            tb_cw_direction.Text = "";
            tb_cw_hum.Text = "";
            tb_cw_temp.Text = "";
            tb_cw_wind.Text = "";
            //cb_hum_self_calib.Enabled = true;
            //btn_update_constantly.Enabled = true;//this will be true when user selects a station 
            //cb_station_names.Enabled = true;
            tb_station_distance.Text = "";
            cb_enable_disable.Checked = false;
            
            
            
            
            
            
            
            
            
            
            
            index_selected = cb1_select_data.SelectedIndex + 1; //
            get_stored_data();
            
            
            
            
            
            
            cb_enable_disable.Enabled = true;







        }

        private void on_select_index_change_event(object sender, EventArgs e)
        {
            //this is used to calculate the distance for the station...
            try { 
            index_for_station_selected = cb_station_names.SelectedIndex;
            //MessageBox.Show("index = " + index_for_station_selected);
            tb_station_distance.Text = store_station_list[index_for_station_selected].distance.ToString();
            btn_update_now.Enabled = true;
            btn_update_constantly.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

      

    
      
    }
}
