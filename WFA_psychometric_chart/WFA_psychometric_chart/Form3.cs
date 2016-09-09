using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Forms;
using System.Net;
using System.Xml.Linq;
using System.Xml;
using System.Collections;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Reflection;
using System.Data.SQLite;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Data.OleDb;

namespace WFA_psychometric_chart
{
    public partial class Form3 : Form
    {

        public Form3()
        {
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.Form3_Disposed );
        }

        private void Form3_Disposed(object sender, EventArgs e)
        {
            //throw new NotImplementedException();

            //Disposing the timer when the device is closed.
            timerForDevice.Stop();
            timerForDevice.Tick -= new EventHandler(timer1_Tick_For_Device);
            timerForDevice.Dispose();

        }

        string buildingNameStore;
        int index_selected = 0;
        int index_for_station_selected = 0;
       

        //We need to store the data pulled into a List.
        //--setting variable data...
        public class DataTypeSettingList
        {
            public int id { get; set; }
            public string buildingName { get; set; }
            public int enabled { get; set; }
            public string selectedStationName { get; set; }
            public int indexSelected { get; set; }

        }
        List<DataTypeSettingList> settingList = new List<DataTypeSettingList>();

        //--END OF THE setting variable data

        //Lets initialize a timer 

        private Timer timerForDevice = new Timer();
        public void InitTimerForDevice()
        {
            //update_now_function();//First time calling the function..
            //timer1 = new System.Windows.Forms.Timer();
            timerForDevice.Start();
            timerForDevice.Tick += new EventHandler(timer1_Tick_For_Device);
            timerForDevice.Interval = 1000 * 15; // in miliseconds //this is 5 seconds
            

        }
         public void timer1_Tick_For_Device(object sender , EventArgs e)
        {
           // MessageBox.Show("Test called ");
        //  ReadDataFromDevice();
        }

        public class DeviceClass
        {
            public string deviceIP { get; set; }
            public uint deviceInstance { get; set; }//device id 

        }
        List<DeviceClass> device_info = new List<DeviceClass>();


        private void button4_Click(object sender, EventArgs e)
        {
            //First read the data value 
            //ReadDataFromDevice();

            // initialize timer for constant reading..
            //InitTimerForDevice();

            if (flagForTimer == 1)
            {
                //Timer is already set so dissable it first and then reenable...
                _Timer.Enabled = false;
                _Timer.Elapsed -= HandleTick;//new System.EventHandler(HandleTick);
               // _Timer.Stop();
                _Timer.Dispose();

                flagForTimer = 0;//Timer dissabled
            }

            device_info.Clear();//Resetting the values
            //=========Resetting the two comboboxes first===========

            CB_param_hum.Text = "";
            CB_param_hum.Items.Clear();
            CB_param_hum.Enabled = false;

            CB_param_temp.Text = "";
            CB_param_temp.Items.Clear();
            CB_param_temp.Enabled = false;
            tb_temp_panel_value.Text = "";
            tb_hum_panel_value.Text = "";
            //=======================end of resetting=============


            CB_Device.Items.Clear();//Clearing the combobox
            CB_Device.Text = "";

            //DeviceConnection dc = new DeviceConnection();
            //dc.ScanForDevice();//Scan the device

            ////Now we need to scan the device and fill the values.
            //foreach (var bn in BACnetClass.DevicesList)
            //{
            //    device_info.Add(new DeviceClass
            //    {
            //        deviceInstance = bn.device_id,
            //        deviceIP = bn.device_id + "." + bn.adr.adr[0] + "." + bn.adr.adr[1] + "." + bn.adr.adr[2] + "." + bn.adr.adr[3]

            //    });
            //}
            copyOfMainControllerList.Clear();//tHSI IS RESETTING 
            copyOfMainControllerList = ControllerList();//This returns the miancontroller and copy those in copyof...


            //Test 
            //MessageBox.Show("Device list num =" + BACnetClass.DevicesList.Count+ ",device infor num= "+device_info.Count);

            //if device infor shows 0 item the retunr immediately
            //if (device_info.Count <= 0)
            //{
            //    return;
            //}
            if (copyOfMainControllerList.Count < 0)
            {
                return;
            }
            //MessageBox.Show("success :bacnetclass count= " + BACnetClass.DevicesList.Count);
            //Now lets fill the combobox value
            foreach (var b in copyOfMainControllerList)
            {
                CB_Device.Items.Add("Device: " + b.controllerName);
               // MessageBox.Show("device");
            }

            //This one is for the online/offline status part
            if (CB_Device.Items.Count > 0)
            {
                //if There is item then we say device is online else ofline
                lb_device_status.Text = "connected";
            }
            else
            {
                lb_device_status.Text = "disconnected";
            }

        }
                   /// <summary>
                   /// The main controller will always have Parent_SerialNum = 0;
                   /// </summary>
        public class dataTypeForControllerList
        {
            public string controllerName { get; set; }
            public int controllerInstanceId { get; set; }
            public bool controllerStatus { get; set; }
        }

        List<dataTypeForControllerList> mainControllerList = new List<dataTypeForControllerList>();
        List<dataTypeForControllerList> copyOfMainControllerList = new List<dataTypeForControllerList>();


        /// <summary>
        /// This function return a controller list that is ONLINE only
        /// </summary>
        public List<dataTypeForControllerList> ControllerList()
        {
            mainControllerList.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\Database\Buildings\Default_Building\Default_Building.mdb";
            //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+ databaseFile+ @";Persist Security Info=True ";
            //C:\Program Files (x86)\T3000\Database\Buildings\Default_Building

            if (BuildingSelected.Count > 0)
            {
                //MessageBox.Show("Building infor not found");
                //return null;
            
            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

              //  MessageBox.Show("Path to building = " + againDbPath);

           // String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

            string sql = "select * from ALL_NODE  where  Parent_SerialNum = '0'";//This will only show parent controller and not sub devices
                                                                                 //  MessageBox.Show("Connection = " + connection);
                                                                                 //  MessageBox.Show("sql = " + sql);
                                                                                 //DataSet myDataSet = new DataSet();
                                                                                 //try
                                                                                 //{
            using (OleDbConnection conn = new OleDbConnection(againDbPath))
            {
                using (OleDbCommand cmd = new OleDbCommand(sql, conn))
                {
                    conn.Open();
                    OleDbDataReader reader = cmd.ExecuteReader();
                    while (reader.Read())
                    {
                        //dataCheckList.Add((bool)reader["Online_Status"]);
                        //if true then add the device or list the main controller
                        if ((bool)reader["Online_Status"])
                        {
                            mainControllerList.Add(new dataTypeForControllerList
                            {
                                controllerName = reader["Product_Name"].ToString(), //This is product name
                                controllerInstanceId = int.Parse(reader["Object_Instance"].ToString()),
                                controllerStatus = (bool)reader["Online_Status"]
                            });

                        }


                    }

                }

                //MessageBox.Show("Count"+myDataSet[0])
                //  MessageBox.Show("Count the value = " + dataCheckList.Count+" , value = "+dataCheckList[0]);
            }
        }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
         
        

            return mainControllerList;
        }







        /// <summary>
        /// This is for temporary class for storing the temperature and humidity value
        /// </summary>

        public class HumTempDataType
        {
            public double Temp { get; set; }
            public double Hum { get; set; }
        }
      public  List<HumTempDataType> HumTempList = new List<HumTempDataType>();//list to store the data form the device...
        public void ReadDataFromDevice(int deviceID,uint panel1ID,uint panel2ID)
        {
            //lets do some operation regarding the pannel id and stuff

            uint panelID1 = panel1ID;// tb_temp_panel_ID.Text;
            uint panelID2 = panel2ID;//tb_hum_panel_ID.Text;
            //if ((panelID1 != "") &&  (panelID2 != ""))
            //{
            //then perform this task 
            try
            {
                uint panID_1 =panelID1;

                uint panID_2 =panelID2;
                BACnetClass b = new BACnetClass();

                //for temperature value
           //  b.StartProgram(panID_1);
               string temp = b.PresentValueFromBacnet.ToString();
                tb_temp_panel_value.Text = temp;
                //For humidity value
            // b.StartProgram(panID_2);
               string humidity = b.PresentValueFromBacnet.ToString();
                tb_hum_panel_value.Text = humidity;
                //lets store these two values in a temporary list

                HumTempList.Add(new HumTempDataType
                {
                    Temp = double.Parse(temp),
                    Hum = double.Parse(humidity)

                });
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }

        }

        //this is used for dynamic list creation...
        public class station_data
        {
           public string name { get; set; }
            public double distance { get; set; }
            public double id { get; set; }
            public double lat { get; set; }
            public double lng { get; set; }
        }

        List<station_data> store_station_list = new List<station_data>();
        
        public void enabledClicked()
        {

            if (cb_enable_disable.Checked == true)
            {
                try
               {
                    store_station_list.Clear();
                    //enable all the text boxex..
                    tb_location.Enabled = true;
                    // tb_distance_from_build.Enabled = true;
                    tb_last_updated.Enabled = true;
                    // btn_pull_offline_data.Enabled = true;
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


                        //MessageBox.Show("lat = " + lat + "lng = " + lng);
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
                                    id = double.Parse(station_id),
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

                        if (cb_station_names.Items.Count > 0)
                        {
                            //There is item means we are online 
                            lb_web_status.Text = "active";
                        }
                        else
                        {
                            lb_web_status.Text = "inactive";
                        }
                        // MessageBox.Show("values  = " + s);

                    }//close of using webclient


                    //--This is for the setting ie if the building is selected and closed

                    //CheckDataForSettings();
               
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message + WFA_psychometric_chart.Properties.Resources._Please_make_sure_you_are_conn);
                    //cb_enable_disable.Enabled = false;
                    tb_location.Enabled = false;
                    // tb_distance_from_build.Enabled = false;
                    tb_last_updated.Enabled = false;
                    //  btn_pull_offline_data.Enabled = false;
                    //btn_update_now.Enabled = false;
                    tb_cw_barometer_value.Enabled = false;
                    tb_cw_direction.Enabled = false;
                    tb_cw_hum.Enabled = false;
                    tb_cw_temp.Enabled = false;
                    tb_cw_wind.Enabled = false;
                    cb_hum_self_calib.Enabled = false;
                    //  btn_update_constantly.Enabled = false;
                    //dissable the second check box...
                    cb_hum_self_calib.Checked = false;
                    cb_station_names.Enabled = false;
                    tb_station_distance.Enabled = false;
                    cb_enable_disable.Checked = false;
                    lbConnectionIssue.Text = "GOOD";
                    btnShowLogFile.Enabled = false;

                }



            }
            else
            {

                cb_station_names.Text = "";

                //dissable..
                tb_location.Enabled = false;
                // tb_distance_from_build.Enabled = false;
                tb_last_updated.Enabled = false;
                // btn_pull_offline_data.Enabled = false;
                //btn_update_now.Enabled = false;
                tb_cw_barometer_value.Enabled = false;
                tb_cw_direction.Enabled = false;
                tb_cw_hum.Enabled = false;
                tb_cw_temp.Enabled = false;
                tb_cw_wind.Enabled = false;
                cb_hum_self_calib.Enabled = false;
                //  btn_update_constantly.Enabled = false;
                //dissable the second check box...
                cb_hum_self_calib.Checked = false;
                cb_station_names.Enabled = false;
                tb_station_distance.Enabled = false;



                tb_location.Text = "";
                // tb_distance_from_build.Text= "";
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
                lbConnectionIssue.Text = "GOOD";
                btnShowLogFile.Enabled = false;
               
                //--Disposing the timer1 and timer2 
                timer1.Stop();
                timer1.Tick -= new EventHandler(timer1_Tick);

                timer2.Stop();
                timer2.Tick -= new EventHandler(timer2_Tick);

                timer2.Dispose();
                timer1.Dispose();

                //WE need to disable the connection   
                //This one is for online/Offline status
                lb_web_status.Text = "inactive";


            }


        }

        /// <summary>
        /// This is the weather serivice pulling part now currently it has setting as well saved when ever the user closes the application and reopens it user can start for the previous settings.
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cb_enable_disable_CheckedChanged(object sender, EventArgs e)
        {

            enabledClicked();

            ////--Later this will enabled when we have to add settings....
            ////--If uncecked then we need to remove the settings or just make the setting null 
            //if(cb_enable_disable.Enabled == false)
            //{
            //    //if this is false then we need to disable the added setting or update the settings..
            //    //UpdateSettingIfPresent
            //    //--If the data is present
            //    if (UpdateSettingIfPresent() > 0)
            //    {

            //        //--We need to update the data...
            //        updateSettingData(settingList[0].buildingName, 0, settingList[0].selectedStationName, settingList[0].indexSelected);

            //    }



            //}


        }



        public void updateSettingData(string buildingName,int enable,string selectedStName,int indexSelectedBuilding)
        {
            string bName = buildingName;
            int en = enable;
            string sSelName = selectedStName;
            int ind = indexSelectedBuilding;


            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                string sql_string = "update tbl_weather_settings set  buildingName=@buildValue,enabled=@enableValue,selectedStationName=@selValue,indexSelected=@indValue  where id = @idSelected;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@buildValue", buildingName.ToString());
                command.Parameters.AddWithValue("@enableValue", enable);
                command.Parameters.AddWithValue("@selValue", selectedStName.ToString());
                command.Parameters.AddWithValue("@indValue", indexSelectedBuilding);

                command.Parameters.AddWithValue("@idSelected", settingList[0].id);

                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());

                command.ExecuteNonQuery();


            }


        }



        int countNumberDbReturn;

        public int UpdateSettingIfPresent()
        {

            string databasePath22 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile22 = databasePath22 + @"\db_psychrometric_project.s3db";

            string connString22 = @"Data Source=" + databaseFile22 + ";Version=3;";

            using (SQLiteConnection connection22 = new SQLiteConnection(connString22))
            {
                connection22.Open();
                SQLiteDataReader reader = null;
                SQLiteCommand command22 = new SQLiteCommand("select * from tbl_weather_settings where buildingName=@buildingName", connection22);
                command22.Parameters.AddWithValue("@buildingName", lb_building_name.Text);
                reader = command22.ExecuteReader();
                while (reader.Read())
                {
                   
                    countNumberDbReturn = 1;

                }
            }


            return countNumberDbReturn;

        }


        /// <summary>
        /// This is used to check the data is present or not if present means that we need to select the settings..
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        //variable is deleared in the top.
        public void CheckSettingData()
            {
            string databasePathFFF = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFileFFF = databasePathFFF + @"\db_psychrometric_project.s3db";

            string connStringFFF = @"Data Source=" + databaseFileFFF + ";Version=3;";

            using (SQLiteConnection connectionFFF = new SQLiteConnection(connStringFFF))
            {
                connectionFFF.Open();
                SQLiteDataReader reader = null;

                SQLiteCommand command = new SQLiteCommand("select * from tbl_weather_settings WHERE buildingName='@buildingname'", connectionFFF);
                command.Parameters.AddWithValue("@buildingname", buildingNameStore.ToString());
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    settingList.Add(new DataTypeSettingList
                    {
                        id = int.Parse( reader["id"].ToString()),
                        buildingName = reader["buildingName"].ToString(),
                        enabled = int.Parse(reader["enabled"].ToString()),
                        selectedStationName = reader["selectedStationName"].ToString(),
                        indexSelected = int.Parse( reader["indexSelected"].ToString())
                        

                    });
                    MessageBox.Show("Entereeeeee");
                  
                }
                MessageBox.Show("value counted = " + settingList.Count);
            }


        }


        private void cb_hum_self_calib_CheckedChanged(object sender, EventArgs e)
        {
            //if the self calibration is enabled or dissabled then 
            if (cb_hum_self_calib.Checked == true)
            {

                btn_help.Enabled = true;
                tb_max_adjust.Enabled = true;
               // tb_current_offset.Enabled = true;
                //btn_set_value.Enabled = true;

            }
            else
            {

                tb_max_adjust.Text = "";
                tb_current_offset_percent.Text = "";
                btn_help.Enabled = false;
                tb_max_adjust.Enabled = false;
                // tb_current_offset.Enabled = false;
                //btn_set_value.Enabled = false;

                //Disposing the events
                timer2.Stop();
                timer2.Tick -= new EventHandler(timer2_Tick);

                timer2.Dispose();
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

                    string error_string = WFA_psychometric_chart.Properties.Resources.Please_select_a_location;
                    MessageBox.Show(error_string, "error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    // MessageBox.Show("Please perform the follwing step first.\n 1.select a location \n 2.pull stored building data \n 3. get geo value(we need geo value)");
                }
                else
                {



                    //string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                   // string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                   // string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                   // string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


                    //--changing all the database to the sqlite database...
                    string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                    string connString = @"Data Source=" + databaseFile + ";Version=3;";



                    using (SQLiteConnection connection = new SQLiteConnection(connString))
                    {
                        connection.Open();
                        SQLiteDataReader reader = null;
                        SQLiteCommand command = new SQLiteCommand("SELECT * from tbl_weather_related_values where ID=@id", connection);
                        command.Parameters.AddWithValue("@id", index_selected);
                        reader = command.ExecuteReader();
                        while (reader.Read())
                        {
                            //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                            tb_location.Text = reader["location"].ToString();
                           // tb_distance_from_build.Text = Math.Round(double.Parse(reader["distance_from_building"].ToString()), 2).ToString();
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
            //pull_stored_weather_data();
        }

        private void get_stored_data()
        {
            //try
            //{
           //f (cb1_select_data.SelectedIndex > -1)
           //
                //lets get the index parameter form the table...
                //index_selected = cb1_select_data.SelectedIndex + 1; //
                //MessageBox.Show("index = " + index_selected);
             // index_selected = temp_building_values[cb1_select_data.SelectedIndex].ID;//This is new configuration
                //lets pull the vales offline values stored in db...
           //     string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                //string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
            //    string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";



                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    //command.Parameters.AddWithValue("@index", index);
                    //command.Parameters.Add("@index",DbType.Int32).Value= index_selected;
                   // command.Parameters.AddWithValue("@index", index_selected);
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
                        lb_building_name.Text = reader["BuildingName"].ToString();
                    buildingNameStore = reader["BuildingName"].ToString();//lets store the building name in a variable...
                        index_selected =int.Parse( reader["ID"].ToString()); //--This is added to check the select
                    }


                }

           
        //}//close of if statement
        //    else
        //    {
        //        MessageBox.Show("Please select an item.\n If you dont have a location then please insert first.");
        //    }

            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

        }



        //private void button6_Click(object sender, EventArgs e)
        //{
        //    get_stored_data();
        //}


        //--This si used to check if the internet is gone if gone at what time ie start and end 
        int InternetAvailable = 0;//--not available




        TimeSpan lastUpdateTimeDifference;//--This stores the last updated time..
        string lastUpdateHourOrMinuteOrSecond = null;//--This is for storing only hours or minutes or seconds one parameter.



        public void update_now_function()
        {
            /*
         steps:
         * 0.pull the lat and long value stored in the database
         * 1.pull the data form web 
         * 2.Parse the xml data
         * 3.insert data
         * 4.display data
         */
        //MessageBox.Show("Starting......");

            if (index_for_station_selected > -1)
            {

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
                    //string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                    /// string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                    // string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                    //--changing all the database to the sqlite database...
                    string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                    string connString = @"Data Source=" + databaseFile + ";Version=3;";


                    if (tb_latitude.Text == "" && tb_longitude.Text == "")
                    {

                        string error_string = "Please select a location ";
                        MessageBox.Show(error_string, "error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        // MessageBox.Show("Please perform the follwing step first.\n 1.select a location \n 2.pull stored building data \n 3. get geo value(we need geo value)");
                    }
                    else
                    {

                        try
                        {

                            //enabling the internet connection if available
                            cb_station_names.Enabled = true;

                            lat_val = store_station_list[index_for_station_selected].lat;
                        lng_val = store_station_list[index_for_station_selected].lng;
                        //display lat lng...
                        //MessageBox.Show("lat= "+lat_val+" lng = "+lng_val);
                        using (var wc = new WebClient())
                        {
                            // var json = await httpClient.GetStringAsync(api_url);

                            // double station_id = store_station_list[index_for_station_selected].id;
                            //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                            string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat=" + lat_val + "&lon=" + lng_val + "&appid=615afd606af791f572a1f92b27a68bcd";
                            //string api_url = "http://api.openweathermap.org/data/2.5/station?id="+station_id+"&APPID=615afd606af791f572a1f92b27a68bcd";

                            var data = wc.DownloadString(api_url);
                          //  MessageBox.Show("string apic return =" + data);
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
                          //      MessageBox.Show("temp  = " + temp_pulled);
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
                            string loc_value = "";
                            double d = 0.0000;
                            double temp_adjust = double.Parse(temp_pulled.ToString()) - 273.15;
                       

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



                                //--changing all the database from MSSQL to SQLite database

                                using (SQLiteConnection connection = new SQLiteConnection(connString))
                                {
                                    connection.Open();
                                    SQLiteDataReader reader = null;
                                    int count_col_table = 0;
                                    //lets check the data first...
                                    //step1.
                                    string count_num_column = "select count(id) from tbl_weather_related_values WHERE id='"+index_selected+"'";
                                    SQLiteCommand cmd1 = new SQLiteCommand(count_num_column, connection);
                                    reader = cmd1.ExecuteReader();
                                    while (reader.Read())
                                    {
                                        //lets get the value...
                                        count_col_table = Convert.ToInt32(reader[0].ToString());
                                    }
                                 //   MessageBox.Show("count_col_table  =" + count_col_table);
                                    reader.Close();
                                    //step2. condition
                                    if (count_col_table >0)
                                    {
                                        //update previous value

                                        string sql_string = "update tbl_weather_related_values set  location=@location_value,distance_from_building=@distance_value,last_update_date=@last_value,temp=@temp_value,humidity=@hum_value,bar_pressure=@pressure_value,wind=@wind_value,direction=@direction_value,station_name=@station_name   where ID = @index_selected;";
                                        SQLiteCommand command = new SQLiteCommand(sql_string, connection);
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
                            //            MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                             //           MessageBox.Show("updating...");

                                        command.ExecuteNonQuery();
                                        //MessageBox.Show("sql string = " + sql_string);
                                        //MessageBox.Show("value updated successfully!");

                                    }
                                    else
                                    {
                                        //insert...
                                        string sql_string = "insert into tbl_weather_related_values(ID,location,distance_from_building,last_update_date,temp,humidity,bar_pressure,wind,direction,station_name) VALUES(@id_value,@location_value,@distance_value,@last_value,@temp_value,@hum_value,@pressure_value,@wind_value,@direction_value,@station_name)";
                                        SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                                        command.CommandType = CommandType.Text;

                                        command.Parameters.AddWithValue("@id_value", index_selected);
                                        command.Parameters.AddWithValue("@location_value", loc_value.ToString());
                                        command.Parameters.AddWithValue("@distance_value", d.ToString());
                                        command.Parameters.AddWithValue("@last_value", last_update_pulled.ToString());
                                        command.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                                        command.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                                        command.Parameters.AddWithValue("@pressure_value", pressure_pulled.ToString());
                                        command.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                                        command.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                                        command.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                              //          MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                                        command.ExecuteNonQuery();
                                    }


                                    //here we need to add to another table ie tbl_data_store_temp_hum_one_year which stores all the data...
                                    //first lets seprate the data and time...
                                    string[] value = last_update_pulled.Split('T');
                                    string date = value[0].ToString();
                                    string time = value[1].ToString();
                                    string[] val = time.Split(':');
                                    string hour = val[0].ToString();
                                    string minute = val[1].ToString();








                                    //--Now lets take the current data and time ....

                                    DateTime currentDT = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second);

                                    DateTime serverDateTime = DateTime.Parse(last_update_pulled);

                                    lastUpdateTimeDifference = currentDT - serverDateTime;
                                    // MessageBox.Show( "curr, server time "+currentDT+","+serverDateTime+ "last updated diffe= " + lastUpdateTimeDifference);


                                    string[] datetimeSpanSplit = lastUpdateTimeDifference.ToString().Split(':');


                                    if (double.Parse(datetimeSpanSplit[0].ToString()) != 0.0000)
                                    {
                                        //--Assumes the difference is of hours dont need minute and seconds 
                                        lastUpdateHourOrMinuteOrSecond = datetimeSpanSplit[0].ToString() + " Hour ago";

                                    }
                                    else if (double.Parse(datetimeSpanSplit[1].ToString()) != 0.00)
                                    {
                                        //--Treats as there is minute difference not hours.
                                        lastUpdateHourOrMinuteOrSecond = datetimeSpanSplit[1].ToString() + " Minute ago";

                                    }
                                    else
                                    {
                                        //--There is seconds difference...
                                        lastUpdateHourOrMinuteOrSecond = datetimeSpanSplit[2].ToString() + " Seconds ago";

                                    }
                
                                    // MessageBox.Show("date = " + date + " time = " + time);
                                    /*steps :
                                     1.insert the values in the given database.
                                     * 2.make sure it is in correct formate..
                                     */

                                    //insert...
                                    //    MessageBox.Show("tempe= " + temp_adjust + " hum = " + hum_pulled);
                                    string sql_query = "insert into tbl_historical_data(ID,date_current,hour_current,minute_current,distance_from_building,temperature,humidity,bar_pressure,wind,direction,station_name) VALUES(@id_value,@date_current,@hour_current,@minute_current,@distance_from_building_value,@temp_value,@hum_value,@bar_pressure_value,@wind_value,@direction_value,@station_name)";
                                    SQLiteCommand cmdx = new SQLiteCommand(sql_query, connection);
                                    cmdx.CommandType = CommandType.Text;
                                    cmdx.Parameters.AddWithValue("@id_value", index_selected);//this index selected identifies the location or building info...
                                    cmdx.Parameters.AddWithValue("@date_current", date.ToString());
                                    cmdx.Parameters.AddWithValue("@hour_current", hour.ToString());
                                    cmdx.Parameters.AddWithValue("@minute_current", minute.ToString());
                                    cmdx.Parameters.AddWithValue("@distance_from_building_value", d.ToString());
                                    cmdx.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                                    cmdx.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                                    cmdx.Parameters.AddWithValue("@bar_pressure_value", pressure_pulled.ToString());
                                    cmdx.Parameters.AddWithValue("@wind_value", wind_speed_pulled.ToString());
                                    cmdx.Parameters.AddWithValue("@direction_value", direction_pulled.ToString());
                                    cmdx.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                                    cmdx.ExecuteNonQuery();

                                    //finally close the connection.
                                    connection.Close();

                                    //This one is for the online/offline 
                                    lb_web_status.Text = "active";

                                }//close o using..

                



                         
                            //step 4:display the value..
                            /* instead of reading the values lets pull the data...*/
                            pull_stored_weather_data();


                            tb_last_updated.Text = lastUpdateHourOrMinuteOrSecond.ToString();



                            // MessageBox.Show("success !");
                        } //This is the close of the using statement

                        }
                        catch (Exception ex)
                        {
                            //   MessageBox.Show("Connect to internet please : " + ex.Message);

                            //Here this occurs when the internet connection is lost so

                            lb_web_status.Text = "inactive";
                            cb_station_names.Enabled = false;

                        }



                    }//close of else...block for tb_latitude and tb_longitude..

                    //--IF the intenetavailable = 2 means that internt has gone for some time and we need to recored it..
                    if (InternetAvailable == 2)
                    {
                        //This means the error has occured and ended we need to log to the file                                            
                        //--Present to write to it
                        string logFilePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                        string logFile = logFilePath + @"\ErrorLogFile.txt";

                        if (File.Exists(logFile)) { 
                        
                        string logLine = WFA_psychometric_chart.Properties.Resources.Error_Ended + DateTime.Now + WFA_psychometric_chart.Properties.Resources._Internet_Connection_Available;
                        using (StreamWriter sw = File.AppendText(logFile))
                        {
                            sw.WriteLine(logLine);


                        }

                        }//close of if..


                    }



                    InternetAvailable = 1;//Available



                }
                catch (Exception ex)
                {

                    //--This is the exception for the internet not available..
                    //MessageBox.Show(ex.Message);

                    InternetAvailable = 2;//gone for some time....

                    //--We need to log the information to a file...

                    string logFilePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string logFile = logFilePath + @"\ErrorLogFile.txt";

                    //--Creating a log file if not present else writing to it
                    if (!File.Exists(logFile))
                    {
                        //--Not present so create the file
                        string logHeader = WFA_psychometric_chart.Properties.Resources.Time_Status_Time_Error_Status;
                        string logLine   = WFA_psychometric_chart.Properties.Resources.Error_Detected+DateTime.Now +WFA_psychometric_chart.Properties.Resources._Internet_Connection_Lost+ex.Message;
                        using (StreamWriter fs = new StreamWriter(logFile))
                        {
                            fs.WriteLine(logHeader);
                            fs.WriteLine(logLine);

                        }



                    }
                    else
                    {
                        //--Present to write to it
                        string logLine = WFA_psychometric_chart.Properties.Resources.Error_Detected + DateTime.Now + WFA_psychometric_chart.Properties.Resources._Internet_Connection_Lost + ex.Message;
                        using (StreamWriter sw = File.AppendText(logFile))
                        {
                            sw.WriteLine(logLine);
                            
                        }


                    }

                    lbConnectionIssue.Text = "FAIL";
                    btnShowLogFile.Enabled = true;

                    //This one is for online offline value

                    //WE need to disable the connection   
                    //This one is for online/Offline status
                    lb_web_status.Text = "inactive";



                }


            }//close of index_for_station_selected>-1
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.please_select_a_station_first);
            }

        }


        private void btn_update_now_Click(object sender, EventArgs e)
        {
            update_now_function();
        
        }//close of btn private void..

       
        private void button1_Click(object sender, EventArgs e)
        {
            //This will help to insert the values...

            //Form4_insert_data f4 = new Form4_insert_data(this);
            //f4.Show();

            //--This is now a refresh button that does the refresh things...
            
            get_stored_data();//--This get the latest changed data form db...



        }

        public class DataTypeTempBuildingValue
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string  state{ get; set; }
            public string city { get; set; }
        }

        //ArrayList temp_building_values = new ArrayList();
        List<DataTypeTempBuildingValue> temp_building_values = new List<DataTypeTempBuildingValue>();


        public void fill_combobox()
        {
           //b1_select_data.Items.Clear();
            ArrayList stored_location = new ArrayList();
            temp_building_values.Clear();//we need to clear the values for new items
            //while loading it should populate the field...
            //lets pull the vales offline values stored in db...
            //string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

            //string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


            //--changing all the database to the sqlite database...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";


            // MessageBox.Show("connection string = " + connString);


            SQLiteConnection connection = new SQLiteConnection(connString);
                connection.Open();
                SQLiteDataReader reader = null;
                SQLiteCommand comm = new SQLiteCommand("SELECT * from tbl_building_location where selection = 1", connection);
                //command.Parameters.AddWithValue("@1", userName)
                reader = comm.ExecuteReader();
                while (reader.Read())
                {
                   
                    //string selecte_location = reader["id"].ToString()+","+reader["country"].ToString() + "," + reader["state"].ToString() + "," + reader["city"].ToString();
                //stored_location.Add(selecte_location);

                temp_building_values.Add(new DataTypeTempBuildingValue {
                    ID = int.Parse(reader["id"].ToString()),
                    country = reader["country"].ToString(),
                    state = reader["state"].ToString(),
                    city = reader["city"].ToString()
                });                

                }
                //string s = "";
                for (int i = 0; i < temp_building_values.Count; i++)
                {

                string tempValue = temp_building_values[i].ID+","+temp_building_values[i].country + "," + temp_building_values[i].state + "," + temp_building_values[i].city;
                //b1_select_data.Items.Add(tempValue);
               //b1_select_data.Items[0].Equals(tempValue;
                    //s += stored_location[i] + " , \n";
                }
               // MessageBox.Show("stored place = " + s);
                comm.Dispose();    
            reader.Dispose();    
            connection.Close();

            



        }

        private void Form3_Load(object sender, EventArgs e)
        {
            try { 

            button1.Text = WFA_psychometric_chart.Properties.Resources.Refresh_Building;
                label36.Text = WFA_psychometric_chart.Properties.Resources.Select_station_name;
                //fill_combobox();

                //--we need to check for the lat long value is present for particular selected items..
                //--Before this we need to check for the available values..
                // MessageBox.Show("enter");
                //This part of the code should not be needed now because it is already present in main form

                if (CheckLatLongAvailable() != true)
                {
                    FillLatLongValueAutomatically();//--Fill the lat long values...
                 //   MessageBox.Show("show filllat");
                }

              get_stored_data();//--This will get the selected data form the database...
              cb_enable_disable.Enabled = true;




                //--This is for checking weather the data is present in the setting or not...
                //MessageBox.Show("Before checksettingdata");
                //MessageBox.Show(" BUILDING NAME STOREED" + buildingNameStore);
                //CheckSettingData();
                //MessageBox.Show("after checksettingdata");
                //if (settingList.Count > 0)
                //{
                //    MessageBox.Show("Settinglist count inside ");
                //    //--this means there is a match..
                //    if (settingList[0].enabled == 1)
                //    {
                //        //--if the check was enabled previously...
                //        //--We need to enable settings..
                //        //--This will also triger an event..
                //        cb_enable_disable.Enabled = true;
                //        cb_enable_disable.Checked = true;

                //        //after this we need to check for the indes selsecte.

                //        int indexCB = settingList[0].indexSelected;
                //        //--This show the item is selected or not..
                //        //--I hope this will trigger the events..
                //        cb_station_names.Enabled = true;
                //        cb_station_names.SelectedIndex = indexCB;

                //        on_select_index_change_event(cb_station_names, new EventArgs());//THis calls the event
                //        MessageBox.Show("Reached here...form load");

                //    }
                //}



                //lets calculate the path to the building database folder

                //--Close of the setting things...
                FindPathOfBuildingDB();


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
          

        }


        public class SelectedBuildingDatatype
        {
           public   string Main_BuildingName { get; set; }
            public string Building_Name { get; set; }
            public string Building_Path { get; set; }
        }
        List<SelectedBuildingDatatype> BuildingSelected = new List<SelectedBuildingDatatype>();

        public void FindPathOfBuildingDB()
        {
            BuildingSelected.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
           // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
          //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
           // MessageBox.Show("New path : " + againDbPath);
            // bool returnValue = false;
            //string latValue = "";
            using (OleDbConnection connection = new OleDbConnection(againDbPath))
            {
                connection.Open();
                OleDbDataReader reader = null;
                string queryString = "SELECT * from Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                OleDbCommand command = new OleDbCommand(queryString, connection);
               
                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    BuildingSelected.Add(new SelectedBuildingDatatype
                    {
                      Main_BuildingName = reader["Main_BuildingName"].ToString(),
                      Building_Name = reader["Building_Name"].ToString(),
                      Building_Path  = reader["Building_Path"].ToString()
                    });
                }
            }

          //  MessageBox.Show("count = " + BuildingSelected.Count);
        }


        //--This is for the checking weather lat and long value is available in database or not.
        public bool CheckLatLongAvailable()
        {
            //--Lets do some connection checking and validating the data returned...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            bool returnValue = false;
            string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //command.Parameters.AddWithValue("@index", index);
                //command.Parameters.Add("@index",DbType.Int32).Value= index_selected;
                // command.Parameters.AddWithValue("@index", index_selected);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    latValue =   reader["latitude"].ToString();
                }
            }
            if (latValue != "")
            {
                returnValue = true;
            }
            else
            {
                returnValue = false;
            }

            //--This will be either true or false based on the check value..
            return returnValue;
        }
        string latPulledValue, longPulledValue, elevationPulledValue;

        public void FillLatLongValueAutomatically()
        {

            string country = null, state=null, city=null, street=null, zip=null;
            //--This portion fill the lat,long and elevation value is not present in the database by users..
            //--Lets do some connection checking and validating the data returned...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //command.Parameters.AddWithValue("@index", index);
                //command.Parameters.Add("@index",DbType.Int32).Value= index_selected;
                // command.Parameters.AddWithValue("@index", index_selected);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                       country = reader["country"].ToString();
                    state = reader["state"].ToString();
                    city = reader["city"].ToString();
                    street = reader["street"].ToString();
                    zip =  reader["zip"].ToString();
                }
            }

            MessageBox.Show("Country = "+country+",city "+city);

            pull_data_online(country,state,city,street,zip);//--This will fill the online values form the database
                                                            //--After pulling above we get three values we need to push it to database...

            //--Upadating the table which has no values ...
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "update tbl_building_location set  latitude=@latitude_value,longitude=@longitude_value,elevation=@elevation  where selection=1;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@latitude_value", latPulledValue.ToString());
                command.Parameters.AddWithValue("@longitude_value", longPulledValue.ToString());
                command.Parameters.AddWithValue("@elevation", elevationPulledValue.ToString());
                command.ExecuteNonQuery();
            }


        }



        private void pull_data_online(string country1,string state1,string city1,string street1,string zip1)
        {
            //this function pulls the data from online devices...


            /*
            1.country,state,city,street,latitude,longitude,elev,zip
            */
            string country = country1;
            string state = state1;
            string city = city1;
            string street = street1;
            string zip = zip1;
            int value;
            if (int.TryParse(zip, out value))
            {

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
                        //MessageBox.Show("result = " + result);
                        var locationElement = result.Element("geometry").Element("location");
                        var lat = locationElement.Element("lat");
                        var lng = locationElement.Element("lng");

                        double lat2 = Double.Parse(lat.Value);
                        double lng2 = Double.Parse(lng.Value);
                        latPulledValue = lat2.ToString();
                        longPulledValue = lng2.ToString();

                        //now lets go for the elevation calculation 
                        //var latlng = lat2 + "," + lng2;
                        //var reqUri = string.Format("https://maps.googleapis.com/maps/api/elevation/xml?locations='"+latlng+"'&key=AIzaSyB27paQGkpwaVGAd7wp0N6rcFkq5JEKeiQ");
                        //MessageBox.Show("requrl= " + reqUri);
                        //var req = WebRequest.Create(reqUri);
                        //var resp = req.GetResponse();

                        //var xdoc1 = XDocument.Load(resp.GetResponseStream());
                        //MessageBox.Show("xdoc1= " + xdoc1);
                        //var result1 = xdoc1.Element("elevation").Value;
                        //MessageBox.Show("result1 = " + result1);
                        double elevation = getElevation(lat2, lng2);
                        //MessageBox.Show("elev= " + elevation);
                       elevationPulledValue = elevation.ToString();
                        //btn_insert_data.Enabled = true;

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                       // btn_insert_data.Enabled = false;
                        //tb_country.Text = "";
                        //tb_city.Text = "";
                        //tb_state.Text = "";
                        //tb_street.Text = "";
                        //tb_ZIP.Text = "";

                    }



                }//close of if...
            }//close of if int try parse.
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_zip_numbe);
            }

        }

        public double getElevation(double lat, double lng)
        {
            //https://developers.google.com/maps/documentation/elevation/intro
            var request = (HttpWebRequest)WebRequest.Create(string.Format("https://maps.googleapis.com/maps/api/elevation/json?locations={0},{1}&key=AIzaSyB27paQGkpwaVGAd7wp0N6rcFkq5JEKeiQ", lat, lng));
            var response = (HttpWebResponse)request.GetResponse();
            var sr = new StreamReader(response.GetResponseStream() ?? new MemoryStream()).ReadToEnd();

            var json = JObject.Parse(sr);
            return (double)json.SelectToken("results[0].elevation");
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
            //string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
               // string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
            // string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";



                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_perform_the_follwing_st);
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



                            using (SQLiteConnection connection = new SQLiteConnection(connString))
                            {
                                connection.Open();
                                int count_col_table = 0;
                                //lets check the data first...
                                //step1.
                                string count_num_column = "select count(id) from tbl_weather_related_values ";
                                SQLiteCommand cmd1 = new SQLiteCommand(count_num_column, connection);
                                SQLiteDataReader reader = cmd1.ExecuteReader();
                                while (reader.Read())
                                {
                                    //lets get the value...
                                    count_col_table = Convert.ToInt32(reader[0].ToString());
                                }
                               // MessageBox.Show("count_col_table  =" + count_col_table);
                                reader.Close();
                                //step2. condition
                                if (count_col_table >= index_selected)
                                {
                                    //update previous value

                                    string sql_string = "update tbl_weather_related_values set  location=@location_value,distance_from_building=@distance_value,last_update_date=@last_value,temp=@temp_value,humidity=@hum_value,bar_pressure=@pressure_value,wind=@wind_value,direction=@direction_value,station_name=@station_name   where ID = @index_selected;";
                                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
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
                                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
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

                                //here we need to add to another table ie tbl_data_store_temp_hum_one_year which stores all the data...
                                //first lets seprate the data and time...
                                string[] value = last_update_pulled.Split('T');
                                string date = value[0].ToString();
                                string time = value[1].ToString();
                                /*steps :
                                 1.insert the values in the given database.
                                 * 2.make sure it is in correct formate..
                                 */

                                //insert...
                                string sql_query = "insert into tbl_data_stored_temp_hum_one_year(date_current,time_current,temperature,humidity,station_name) VALUES(@date_current,@time_current,@temp_value,@hum_value,@station_name)";
                                SQLiteCommand cmdx = new SQLiteCommand(sql_query, connection);
                                cmdx.CommandType = CommandType.Text;
                                cmdx.Parameters.AddWithValue("@date_current", date.ToString());
                                cmdx.Parameters.AddWithValue("@time_current", time.ToString());
                                cmdx.Parameters.AddWithValue("@temp_value", temp_adjust.ToString());
                                cmdx.Parameters.AddWithValue("@hum_value", hum_pulled.ToString());
                                cmdx.Parameters.AddWithValue("@station_name", cb_station_names.SelectedItem.ToString());
                                cmdx.ExecuteNonQuery();

                                //finally close the connection.





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

        private Timer timer1= new Timer();
        public void InitTimer()
        {
            update_now_function();//First time calling the function..
            //timer1 = new System.Windows.Forms.Timer();
            timer1.Start();
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Interval = 1000 * 6; // in miliseconds //2min * 30 = 60 min minute ie every 1 hour
           
            
        }

        /// <summary>
        /// pulling the web values
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timer1_Tick(object sender, EventArgs e)
        {
            //  MessageBox.Show("pulling...");
            //update_data_constantly();

            update_now_function();
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
                    string error_string = WFA_psychometric_chart.Properties.Resources.Please_select_a_location_first;
                    MessageBox.Show(error_string, WFA_psychometric_chart.Properties.Resources.error0, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.pulling);
                    InitTimer();

                }
            }//close of if index_for...
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_select_a_station_first_);
            }

        }

        private void btn_help_Click(object sender, EventArgs e)
        {


            string error_string = WFA_psychometric_chart.Properties.Resources.In_this_section_you_select_the;
            MessageBox.Show(error_string, WFA_psychometric_chart.Properties.Resources.Help, MessageBoxButtons.OK, MessageBoxIcon.Information);


        }
       public int adjusted_value =0;
       double sensor_hum = 0;
       double current_weather_hum = 0;
       //double temp_from_weather = 0;
       double total_percentage_change = 0;

        //--This two values are constants always..
        double initialHumOutDoorSensor = 0;
        double initialHumWeatherSensor = 0;
        double calibrationFactor = 0;

        //--end
        public void adjust_hum_value()
        {

            if (tb_max_adjust.Text != "")
            {


                if (tb_hum_panel_value.Text == "")
                {
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_have_a_proper_value_sel);
                }
                else
                {
                    //we need to do something with this sensor value..
                    if (tb_cw_hum.Text == "")
                    {
                        MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_pull_the_current_weathe);
                    }
                    else
                    {
                    
                        //if (adjusted_value == 0) { 
                        ////we need to compare the two values..
                         
                        // adjusted_value++;
                        //}
                       
                        //double adjust_factor = Double.Parse(tb_max_adjust.Text);
                        ////adjusting by 1 degree up and down value.
                        //if (sensor_hum < current_weather_hum - 1)
                        //{
                        //    //decrease the sensor humidity..
                        //    sensor_hum += sensor_hum * adjust_factor/100;


                        //    total_percentage_change += adjust_factor;
                        //    tb_current_offset_percent.Text = total_percentage_change.ToString();
                        //}
                        //else if (sensor_hum > current_weather_hum + 1)
                        //{
                        //    sensor_hum -= sensor_hum * adjust_factor/100;

                        //    total_percentage_change =(initialTempOutDoorSensor- sensor_hum) /initialTempOutDoorSensor*100;
                        //    tb_current_offset_percent.Text = total_percentage_change.ToString();
                        //}

                       // tb_current_offset.Text = sensor_hum.ToString();
                        /*percentage..*/
                        //double percent = (current_weather_hum - sensor_hum) / current_weather_hum * 100;
                        //tb_current_offset_percent.Text = percent.ToString();
                       
                    //for out door humidity > weather humidity 
                    if(initialHumOutDoorSensor > initialHumWeatherSensor)
                        {
                            //--Do this 

                            sensor_hum -= sensor_hum * calibrationFactor / 100;
                            total_percentage_change = (sensor_hum - initialHumOutDoorSensor) / initialHumOutDoorSensor * 100;
                            tb_current_offset_percent.Text = total_percentage_change.ToString();

//                            label34.Text = sensor_hum.ToString();

                        }
                        else if(initialHumOutDoorSensor < initialHumWeatherSensor)
                        {

                            sensor_hum += sensor_hum * calibrationFactor / 100;
                            total_percentage_change = (sensor_hum - initialHumOutDoorSensor) / initialHumOutDoorSensor * 100;
                            tb_current_offset_percent.Text = total_percentage_change.ToString();

                            //--Test..
                            //label34.Text = sensor_hum.ToString();                           

                        }


                      //--Stop condition
                        if (sensor_hum < initialHumWeatherSensor + 1 && sensor_hum > initialHumWeatherSensor - 1)
                        {
                            //if sensor is around current weather condition..
                           // MessageBox.Show("Stop pos ");
                            timer2.Stop();
                            timer2.Tick -= new EventHandler(timer2_Tick);
                            timer2.Dispose();

                        }






                    }

                }

            }
            else
            {
                string error_string = WFA_psychometric_chart.Properties.Resources.Please_enter_a_value_for_adjus;
                MessageBox.Show(error_string, WFA_psychometric_chart.Properties.Resources.Error, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private System.Windows.Forms.Timer timer2 = new System.Windows.Forms.Timer();
        public void InitTimer2()
        {
            //timer2 = new System.Windows.Forms.Timer();
            timer2.Tick += new EventHandler(timer2_Tick);
            timer2.Interval = 1000 * 86400;//now this is 1 day // in miliseconds //2min * 30 = 60 min minute ie every 1 hour

            if (sensor_hum < initialHumWeatherSensor + 1 && sensor_hum > initialHumWeatherSensor - 1)
            {
                //if sensor is around current weather condition..
               // MessageBox.Show("Stop pos ");
                timer2.Stop();

               
            }
            else {

               // MessageBox.Show("Start portion");
                timer2.Start();
            }
           // MessageBox.Show("eND START");
        }

        
        private void timer2_Tick(object sender, EventArgs e)
        {
            //  MessageBox.Show("pulling...");         
               
            adjust_hum_value();
            //MessageBox.Show("pulled...");
        }

        public void self_calibrate()
        {

            try
            {
                if (tb_hum_panel_value.Text != "")
                {
                    if (tb_cw_hum.Text != "")
                    {
                        if (tb_max_adjust.Text != "")
                        {
                            double x = double.Parse(tb_max_adjust.Text);
                            if (x > 0 && x <= 1)
                            {
                                //temp_from_weather = double.Parse(tb_cw_temp.ToString());
                                sensor_hum = Double.Parse(tb_hum_panel_value.Text);
                                current_weather_hum = Double.Parse(tb_cw_hum.Text);

                                //--Initializing intitials values.
                                initialHumOutDoorSensor = sensor_hum;
                                initialHumWeatherSensor = current_weather_hum;

                                InitTimer2();
                            }
                            else
                            {
                                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_value_between_0_1);
                                tb_max_adjust.Text = "";
                            }
                        }
                        else
                        {
                            MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_adjustmen);
                        }
                    }
                    else
                    {
                        MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_get_and_set_the_current);
                    }
                }
                else
                {
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_set_the_value_from_sens);
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        private void btn_set_value_Click(object sender, EventArgs e)
        {
        }

        private void Form3_ClosingForm(object sender, FormClosingEventArgs e)
        {
            timer1.Stop();
            timer1.Tick -= new EventHandler(timer1_Tick);

            timer2.Stop();
            timer2.Tick -= new EventHandler(timer2_Tick);

            timer2.Dispose();
            timer1.Dispose();
            this.Dispose();

            if (flagForTimer == 1)
            {
                //Timer is already set so dissable it first and then reenable...
                _Timer.Enabled = false;
                _Timer.Elapsed -= HandleTick;//new System.EventHandler(HandleTick);
                _Timer.Dispose();

                flagForTimer = 0;//Timer dissabled
            }

        }

        private void cb_event_on_index_change(object sender, EventArgs e)
        {
           // //fill_combobox();
           // cb_enable_disable.Enabled = false;



           // //dissable..
           // tb_location.Enabled = false;
           // tb_distance_from_build.Enabled = false;
           // tb_last_updated.Enabled = false;
           //// btn_pull_offline_data.Enabled = false;
           // //btn_update_now.Enabled = false;
           // tb_cw_barometer_value.Enabled = false;
           // tb_cw_direction.Enabled = false;
           // tb_cw_hum.Enabled = false;
           // tb_cw_temp.Enabled = false;
           // tb_cw_wind.Enabled = false;
           // cb_hum_self_calib.Enabled = false;
           // //btn_update_constantly.Enabled = false;
           // //dissable the second check box...
           // cb_hum_self_calib.Checked = false;
           // cb_station_names.Enabled = false;
           // tb_station_distance.Enabled = false;



           // tb_location.Text = "";
           // tb_distance_from_build.Text = "";
           // tb_last_updated.Text = "";
           // //btn_pull_offline_data.Enabled = true;
           // //btn_update_now.Enabled = true;//this will be true when user selects a station 
           // tb_cw_barometer_value.Text = "";
           // tb_cw_direction.Text = "";
           // tb_cw_hum.Text = "";
           // tb_cw_temp.Text = "";
           // tb_cw_wind.Text = "";
           // //cb_hum_self_calib.Enabled = true;
           // //btn_update_constantly.Enabled = true;//this will be true when user selects a station 
           // //cb_station_names.Enabled = true;
           // tb_station_distance.Text = "";
           // cb_enable_disable.Checked = false;

            
           // //index_selected = cb1_select_data.SelectedIndex + 1; //
           ////nt cb_index_selected = cb1_select_data.SelectedIndex;
           // index_selected = temp_building_values[cb_index_selected].ID;

            get_stored_data();
            
            
            
            
            
            
          //cb_enable_disable.Enabled = true;







        }

        //--on index we need to update constantly
        public void update_constantly_function()
        {


            //this code basically makes the upadating part constantly...
            //it calls the function UpdateDataConstantly()           
            //this function basically calls every  50 minuest...
            if (index_for_station_selected > -1)
            {

                if (tb_latitude.Text == "" && tb_longitude.Text == "")
                {
                    string error_string = WFA_psychometric_chart.Properties.Resources.Please_select_a_location_first;
                    MessageBox.Show(error_string, WFA_psychometric_chart.Properties.Resources.error0, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {
                    //MessageBox.Show("pulling...");
                    label30.Text = WFA_psychometric_chart.Properties.Resources.Status_Updating_data;

                    InitTimer();

                }
            }//close of if index_for...
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_select_a_station_first_);
            }



        }




        private void on_select_index_change_event(object sender, EventArgs e)
        {
            //this is used to calculate the distance for the station...
            try { 
            index_for_station_selected = cb_station_names.SelectedIndex;
            //MessageBox.Show("index = " + index_for_station_selected);
            tb_station_distance.Text = store_station_list[index_for_station_selected].distance.ToString();
                //btn_update_now.Enabled = true;
                // btn_update_constantly.Enabled = true;


                update_constantly_function();//--This functions updates the values constantly..

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            //Start new form for show trend log...
           // TrendLogForm tf = new TrendLogForm(this);
           // tf.Show();

        }

        List<bool> dataCheckList = new List<bool>();
        /// <summary>
        /// CHECKS WEATHER THE DEVICE IS ONLINE OR OFFLINE
        /// </summary>
        /// <param name="deviceID">device id value</param>
        /// <param name="Parent_SerialNum">0 always</param>
        /// <returns></returns>
        public bool CheckDeviceOnlineOffline(int deviceID,int Parent_SerialNum)
        {           
            //Resetting
            dataCheckList.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
         //   string databaseFile = databasePath + @"\Database\Buildings\Default_Building\Default_Building.mdb";
          //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+ databaseFile+ @";Persist Security Info=True ";
            //C:\Program Files (x86)\T3000\Database\Buildings\Default_Building
         //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

            string sql = "select * from ALL_NODE  where Object_Instance = '"+deviceID+"' and Parent_SerialNum = '"+Parent_SerialNum+"'";
            //  MessageBox.Show("Connection = " + connection);
            //  MessageBox.Show("sql = " + sql);
            //DataSet myDataSet = new DataSet();
            bool status = false;
            if (BuildingSelected.Count > 0)
            {
                //MessageBox.Show("Building infor not found");
                //return null;

                /*
                This path is dynamic path we calculated based on this 
                <installed directory of T3000 (not psycho)>\... where ... = Database\..value returned by files
                */
                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

               // MessageBox.Show("Path to building = " + againDbPath);


                try
                {
                    using (OleDbConnection conn = new OleDbConnection(againDbPath))
                    {
                        using (OleDbCommand cmd = new OleDbCommand(sql, conn))
                        {
                            conn.Open();
                            OleDbDataReader reader = cmd.ExecuteReader();
                            while (reader.Read())
                            {
                                dataCheckList.Add((bool)reader["Online_Status"]);
                            }

                        }

                        //MessageBox.Show("Count"+myDataSet[0])
                        //  MessageBox.Show("Count the value = " + dataCheckList.Count+" , value = "+dataCheckList[0]);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
             
                if (dataCheckList.Count > 0)
                {
                    if (dataCheckList[0] == true)
                    {
                        status = true;
                    }
                }

                // MessageBox.Show( "datacheckcount= " + dataCheckList.Count + " Status = " + status);
            }
            return status;
        }


        public class parameter_class1
        {
            public double presentValue { get; set; }
            public uint indexID { get; set; }
            public string device_object_name { get; set; }
        }
        public List<parameter_class1> parameterValFromBacnet = new List<parameter_class1>();

        private void CB_Device_SelectedIndexChanged(object sender, EventArgs e)
        {
            try { 
            //now on index change
           // groupBox3.Enabled = true;
            //Lets clear the combobox first
            CB_param_temp.Items.Clear();
            CB_param_hum.Items.Clear();
            parameterValFromBacnet.Clear();//Clearing the list value

            if (CB_Device.SelectedIndex > -1)
            {
                //This one is if the timer is running then stop the timer second time 
                if (flagForTimer == 1)
                {
                    //Timer is already set so dissable it first and then reenable...
                    _Timer.Enabled = false;
                    _Timer.Elapsed -= HandleTick; //new System.EventHandler(HandleTick);
                    _Timer.Dispose();

                    flagForTimer = 0;//Timer dissabled
                }

                //On this index change the we need to get the values from the 
              int  indexSelectedDevice = CB_Device.SelectedIndex;
                    int instanceId = copyOfMainControllerList[indexSelectedDevice].controllerInstanceId;  //(int)device_info[indexSelectedDevice].deviceInstance;//Device instance selected.

                 //   MessageBox.Show("Instaneid = " + instanceId);
                //===============For regulare update=============//
                deviceInstanceValuTemp = instanceId;//This one is for regular update
                                                    //================end of for regular update=====//
                                                    //This uint has to be converted to int because that is what is required...

                    //Checking if the device is online or offline
                    if (CheckDeviceOnlineOffline(deviceInstanceValuTemp, 0) == true)
                    {
                      //  MessageBox.Show("Checking online status become true");

                        //return;


                        DeviceConnection db = new DeviceConnection();
                        //We need to scan for the device first and then parameter
                        db.ScanForDevice();
                        //db.ScanForParameters(24649);
                        db.ScanForParameters(instanceId);//This will return the parameters
                       // MessageBox.Show("Count = " + db.parameterListValue.Count);       //Now we can use the value strored int  the db.parameterList1
                        string s = "";
                        foreach (var bac in db.parameterListValue)
                        {
                            parameterValFromBacnet.Add(new parameter_class1
                            {
                                device_object_name = bac.device_object_name,
                                indexID = bac.indexID,
                                presentValue = bac.presentValue

                            });
                          //  s += bac.device_object_name + "," + bac.presentValue + "\n";
                        }
                       

                  //  MessageBox.Show("value = " + s);

                    //Now that we have the parameter list lets display the list in the combobox...
                    // string s = "";
                    foreach (var item in parameterValFromBacnet)
                    {
                        CB_param_temp.Items.Add(item.device_object_name + ":" + item.indexID);
                        CB_param_hum.Items.Add(item.device_object_name + ":" + item.indexID);
                        // CB_param_enthalpy.Items.Add(item.device_object_name + ":" + item.indexID);
                        //     s += item.device_object_name + ":" + item.indexID + " value = " + item.presentValue+"\n";

                    }

                    CB_param_temp.Enabled = true;
                    CB_param_hum.Enabled = true;
                      //MessageBox.Show("ENd of the true");
                    //TEST
                }
                else
                {
                        //Show device is offline
                        lb_device_status.Text = "disconnected";
                }
                
            }//Close of if


            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        //Don't use System.Window.Timer
        private System.Timers.Timer _Timer = null;

        int flagForTimer = 0;//DISSABLE
        /// <summary>
        /// This function stores a timer to watch value constantly
        /// </summary>
        public void temporary_timer_function()
        {
            flagForTimer = 1;//ENABLED
            _Timer = new System.Timers.Timer();
            _Timer.Elapsed += HandleTick;//new System.EventHandler(HandleTick);
            _Timer.Interval = 1000 * 1; //new TimeSpan(0, 0, 0, 2); //Timespan of 2 seconds
            _Timer.Start();
        }
        int deviceInstanceValuTemp = 0;
        public async Task AsyncMethod1ForPullingData()
        {
            // Do asynchronous work.
            await Task.Run(()=> getDataFromParameter(deviceInstanceValuTemp));
        }

        //This two variable will be set when we change the index in of cb_param_temp or cb_param_temp
        //This index help to filter the value selected when displaying the value
        int parameterID_ForTemp = -1;
        int parameterID_ForHum = -1;
        int c = 0;
        private void HandleTick(object sender, System.EventArgs e)
        {
            //MessageBox.Show("hANDLER CALLED");

            //Task.Run(()=>AsyncMethod1ForPullingData()); //This will run an async task
            getDataFromParameter(deviceInstanceValuTemp);
  

            //if (lb_test.InvokeRequired)
            //{
            //    // lb_test.Invoke(new Action(() => CB_Device.Items.Clear()));
            //    // CB_Device.Text = ""; 
            //    lb_test.Invoke(new Action(() => lb_test.Text = "Test = " + c++ + " param count" + parameterValFromBacnet.Count));
            //}
            //else
            //{     //if invoke is not requred the do normal way
            //      // CB_Device.Items.Clear();
            //      // CB_Device.Text = "";
            //    lb_test.Text = "Test = " + c++ + " param count" + parameterValFromBacnet.Count;
            //}



        }

        /// <summary>
        /// This function bull the all the available values for particualar device id
        /// </summary>
        /// <param name="device_instance_id"></param>
        /// <returns></returns>
        public void getDataFromParameter(int device_instance_id)
        {

            if (CheckDeviceOnlineOffline(deviceInstanceValuTemp, 0) == true)
            {


               

                DeviceConnection db = new DeviceConnection();
              //  db.parameterListValue.Clear();//This will scan the paramter list first
                db.ScanForParameters(device_instance_id);//This will return the parameters
                                                         //Now we can use the value strored int  the db.parameterList1

                if (db.parameterListValue.Count > 0)
                {

                    ///WE NEED TO REENABLE BOTH DORPDOWN IE FOR TEMP AND HUM IF THE SYSTEM IS ONLINE
                    //temp
                    //  string s = "";
                    parameterValFromBacnet.Clear();
                    foreach (var bac in db.parameterListValue)
                    {
                        parameterValFromBacnet.Add(new parameter_class1
                        {
                            device_object_name = bac.device_object_name,
                            indexID = bac.indexID,
                            presentValue = bac.presentValue

                        });
                        // s += bac.device_object_name + "," + bac.presentValue;
                    }

                    //MessageBox.Show()
                    // MessageBox.Show("count everytime " + parameterValFromBacnet.Count);
                    if (parameterValFromBacnet.Count < 1)
                    {

                    }
                    else
                    {
                        //This is connected section
                        //===============================Ren enabling both dropdown===============//
                        if (CB_param_temp.InvokeRequired)
                        {
                            CB_param_temp.Invoke(new MethodInvoker(() =>
                            {
                                CB_param_temp.Enabled = true;

                            }));
                        }
                        else
                        {
                            CB_param_temp.Enabled = true;
                        }

                        //hum
                        if (CB_param_hum.InvokeRequired)
                        {
                            CB_param_hum.Invoke(new MethodInvoker(() =>
                            {
                                CB_param_hum.Enabled = true;

                            }));
                        }
                        else
                        {
                            CB_param_hum.Enabled = true;
                        }

                        //============================== End=Ren enabling both dropdown===============//




                        //Device is connected and pulling is success
                        //This means we have values 
                        //Temp is set 
                        //shwo status 
                        //lb_device_status.Text = "connected";
                        if (lb_device_status.InvokeRequired)
                        {
                            lb_device_status.Invoke(new Action(() => lb_device_status.Text = "connected"));

                        }
                        else
                        {     //if invoke is not requred the do normal way

                            lb_device_status.Text = "connected";
                        }


                        if (parameterID_ForTemp >= 0)
                        {
                            //This is done to elimate the cross-thread exception which was occured while simple call
                            if (tb_temp_panel_value.InvokeRequired)
                            {
                                tb_temp_panel_value.Invoke(new MethodInvoker(() =>
                                {
                                    //RefreshDataFromDeviceAndWeb();
                                    //MessageBox.Show("Temp value methodinvoker bhitra");     
                                    try { 
                                                tb_temp_panel_value.Text = parameterValFromBacnet[parameterID_ForTemp].presentValue.ToString();
                                    }
                                    catch { }
                                }));
                            }
                            else
                            {
                                // RefreshDataFromDeviceAndWeb();
                                //tb_temp_panel_value.Text = parameterValFromBacnet[parameterID_ForTemp].presentValue.ToString();
                                try
                                {
                                    tb_temp_panel_value.Text = parameterValFromBacnet[parameterID_ForTemp].presentValue.ToString();
                                }
                                catch { }
                            }
                        }

                        //If hum value is set then do below
                        if (parameterID_ForHum >= 0)
                        {
                            //index is set so
                            //tb_hum_panel_value.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                            //This is done to elimate the cross-thread exception which was occured while simple call
                            if (tb_hum_panel_value.InvokeRequired)
                            {
                                tb_hum_panel_value.Invoke(new MethodInvoker(() =>
                                {
                                    //RefreshDataFromDeviceAndWeb();
                                    try { 
                                                tb_hum_panel_value.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                                    }
                                    catch { }
                                }));
                            }
                            else
                            {
                                // RefreshDataFromDeviceAndWeb();
                                // tb_hum_panel_value.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                                try
                                {
                                    tb_hum_panel_value.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                                }
                                catch { }

                            }
                        }

                    }

                }//This is the close of  parameter valueList >0 this done so that when the device return nothing after 
                //It has been online in status change nothing for eg say if device is gone online in access db but now 
                //Device is turned OFF when the scanner just started to scan then device will return nothing thought 
                //Db shows online so in such case in order to preserve previous setting this is used


            }  //this is the closing of checkOnlineOfline fxn
            else
            {
                //Device is offline 
               // lb_device_status.Text = "Disconnected";


                if (lb_device_status.InvokeRequired)
                {
                    lb_device_status.Invoke(new MethodInvoker(() =>
                    {
                        //RefreshDataFromDeviceAndWeb();
                        //lb_device_status.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                        lb_device_status.Text = "Disconnected";

                    }));
                }
                else
                {
                    // RefreshDataFromDeviceAndWeb();
                    // tb_hum_panel_value.Text = parameterValFromBacnet[parameterID_ForHum].presentValue.ToString();
                    lb_device_status.Text = "Disconnected";
                }

                //===============================disabling both dropdown===============//
                //temp
                if (CB_param_temp.InvokeRequired)
                {
                    CB_param_temp.Invoke(new MethodInvoker(() =>
                    {
                        CB_param_temp.Enabled = false;

                    }));
                }
                else
                {
                    CB_param_temp.Enabled = false;
                }

                //hum
                if (CB_param_hum.InvokeRequired)
                {
                    CB_param_hum.Invoke(new MethodInvoker(() =>
                    {
                        CB_param_hum.Enabled = false;

                    }));
                }
                else
                {
                    CB_param_hum.Enabled = false;
                }
                //===============================Ende disabling both dropdown===============//
                //reSETTING BOTH TEXT BOXES
                //tb_temp_panel_value
                if (tb_temp_panel_value.InvokeRequired)
                {
                    tb_temp_panel_value.Invoke(new MethodInvoker(() =>
                    {
                        tb_temp_panel_value.Text = "";

                    }));
                }
                else
                {
                    tb_temp_panel_value.Text = "";
                }

                //tb_hum_panel_value
                if (tb_hum_panel_value.InvokeRequired)
                {
                    tb_hum_panel_value.Invoke(new MethodInvoker(() =>
                    {
                        tb_hum_panel_value.Text = "";

                    }));
                }
                else
                {
                    tb_hum_panel_value.Text = "";
                }




            }

            // return true;
        }//Close maine fxn getdatafromparameter


        private void CB_param_temp_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(CB_param_temp.SelectedIndex >= 0)
            { 
            //on this index is selected the value should be displayed to text box
            tb_temp_panel_value.Text = parameterValFromBacnet[CB_param_temp.SelectedIndex].presentValue.ToString() ;

                //lets set the timer
                //first if the timer is already set then stop that and start new ...
                parameterID_ForTemp = CB_param_temp.SelectedIndex;//Setting the index value for constant update
           if(flagForTimer == 1)
            {
                //Timer is already set so dissable it first and then reenable...
                _Timer.Enabled = false;
                    _Timer.Elapsed -= HandleTick;//new System.EventHandler(HandleTick);
                _Timer.Dispose();

                flagForTimer = 0;//Timer dissabled
            }

                //Checking if the device is online or offline
                if (CheckDeviceOnlineOffline(deviceInstanceValuTemp, 0) == true)
                {

                    //then reenable again
                    temporary_timer_function();
                }
                else
                {
                    //Device is offline 
                    lb_device_status.Text = "Disconnected";

                }
            }

        }

        private void CB_param_hum_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CB_param_hum.SelectedIndex >= 0)
            {

                tb_hum_panel_value.Text = parameterValFromBacnet[CB_param_hum.SelectedIndex].presentValue.ToString();

                parameterID_ForHum = CB_param_hum.SelectedIndex;//Setting the index value for constant update
                if (flagForTimer == 1)
                {
                    //Timer is already set so dissable it first and then reenable...
                    _Timer.Enabled = false;
                    _Timer.Elapsed -= HandleTick;//new System.EventHandler(HandleTick);
                    _Timer.Dispose();

                    flagForTimer = 0;//Timer dissabled
                }

                //then reenable again
                temporary_timer_function();

            }
       
       
        }

        private void tb_max_adjust_TextChanged(object sender, EventArgs e)
        {
           //this is where we are going to call the function ...
           // string a = tb_max_adjust.Text;
             try
            {
                calibrationFactor = double.Parse(tb_max_adjust.Text);


                if (calibrationFactor <= 1 && calibrationFactor >0 )
                {
                    //double x = double.Parse(a);
                    self_calibrate();
                }
                else
                {
                    //Dispose timer..
                    //tb_max_adjust.Text = "";
                    //tb_current_offset_percent.Text = "";
                    timer2.Stop();
                    timer2.Tick -= new EventHandler(timer2_Tick);

                    timer2.Dispose();
                }



            }
            catch (Exception ex)
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_number+ex.Message);
            }

        }

      

    
      

   

        private void btnShowLogFile_Click(object sender, EventArgs e)
        {


            
            string logFilePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string logFile = logFilePath + @"\ErrorLogFile.txt";
            if (File.Exists(logFile)) { 
            Process.Start("notepad.exe", logFile);
            }

        }
          
    }

       }