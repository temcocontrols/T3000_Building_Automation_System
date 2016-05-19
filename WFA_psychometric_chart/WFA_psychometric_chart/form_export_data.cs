using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using Microsoft.Office.Interop;
using Excel = Microsoft.Office.Interop.Excel;
using System.Reflection;
using System.Data.SQLite;

namespace WFA_psychometric_chart
{
    public partial class form_export_data : Form
    {
        public form_export_data()
        {
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.form_export_data_Disposed );
        }
        //this is done for storing the data collected form the database
        public class data_type_hum_temp
        {
             public string date { get; set; }
            public string hour{get; set;}
            public string minute { get; set; }
            public string distance_from_building { get; set; }
            public double temp { get; set; }
            public double hum { get; set; }
            public string bar_pressure { get; set; }
            public string wind { get; set; }
            public string direction { get; set; }
            public string station_name { get; set; }

        }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();

        public class building_loc_data_type
        {
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
            public string zip { get; set; }
            public string longitude { get; set; }
            public string latitude { get; set; }
            public string elevation { get; set; }

        }
        List<building_loc_data_type> building_info = new List<building_loc_data_type>();


        public class station_loc_data_type
        {

            public string location { get; set; }
            public string distance_from_building { get; set; }
            public string last_update_date { get; set; }
            public string temp { get; set; }
            public string humidity { get; set; }
            public string bar_pressure { get; set; }
            public string wind { get; set; }
            public string directioin { get; set; }
            public string station_name { get; set; }


        }
        List<station_loc_data_type> stationInfo = new List<station_loc_data_type>();

        //this selected index is used to identify the particular location and data associated with that location
        int index_selected=0;

        //THIS IS  A CONNECTION STRING TO CONNECT TO DATABASE
       // string connString1 = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

        



        private void button1_Click(object sender, EventArgs e)
        {

            /*
            steps:
            0.get the values of index_selected to identify which location we are selecting.
            1. get validated the date
            2. get the options value.
            3. extract the data base on date 
            4.provide a save as dialog based on the formate to be saved.
            */


            DateTime from = dtp_From.Value;
            DateTime to = dtp_To.Value;


            if (dtp_To.Value> dtp_From.Value)
            {
                //porcess the data

              //  string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //  string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                
                HistoricalDataInfo();


                //now lets pull the station location and building location...
                //first building location 
                BuildingDataInfo();

                //now lets pull the station information...
                StationDataInfo();

                //now lets move ont to pringting the values
                if(rb1_excel.Checked== true)
                {
                    try { 

                    //excel is checked..
                    Excel.Application oApp;
                    Excel.Workbook oBook;
                    Excel.Worksheet oSheet;

                    oApp = new Excel.Application();
                    oBook = oApp.Workbooks.Add();
                    oSheet = (Excel.Worksheet)oBook.Worksheets.get_Item(1);

                    //printing the building information..
                    oSheet.Cells[1, 1] = WFA_psychometric_chart.Properties.Resources.Building_Information;
                    oSheet.Cells[2, 1] = "Country";
                    oSheet.Cells[2, 2] = building_info[0].country.ToString();
                    oSheet.Cells[3, 1] = "State";
                    oSheet.Cells[3, 2] = building_info[0].state.ToString();
                    oSheet.Cells[4, 1] = "City";
                    oSheet.Cells[4, 2] = building_info[0].city.ToString();
                    oSheet.Cells[5, 1] = "Zip";
                    oSheet.Cells[5, 2] = building_info[0].zip.ToString();
                    oSheet.Cells[6, 1] = "Longitude";
                    oSheet.Cells[6, 2] = building_info[0].longitude.ToString();
                    oSheet.Cells[7, 1] = "Latitude";
                    oSheet.Cells[7, 2] = building_info[0].latitude.ToString();
                    oSheet.Cells[8, 1] = "Elevation";
                    oSheet.Cells[8, 2] = building_info[0].elevation.ToString();

                    //for station 
                    oSheet.Cells[9, 1] = "Station Information";
                    oSheet.Cells[10, 1] = "Location";
                    oSheet.Cells[10, 2] = stationInfo[0].location.ToString();
                    oSheet.Cells[11, 1] = "Distance from building";
                    oSheet.Cells[11, 2] = stationInfo[0].distance_from_building.ToString();
                    oSheet.Cells[12, 1] = "Last updated date";
                    oSheet.Cells[12, 2] = stationInfo[0].last_update_date.ToString();
                    oSheet.Cells[13, 1] = "Temperature";
                    oSheet.Cells[13, 2] = stationInfo[0].temp.ToString();
                    oSheet.Cells[14, 1] = "Humidity";
                    oSheet.Cells[14, 2] = stationInfo[0].humidity.ToString();
                    oSheet.Cells[15, 1] = "Bar Pressure";
                    oSheet.Cells[15, 2] = stationInfo[0].bar_pressure.ToString();
                    oSheet.Cells[16, 1] = "Wind";
                    oSheet.Cells[16, 2] = stationInfo[0].wind.ToString();
                    oSheet.Cells[17, 1] = "Direction";
                    oSheet.Cells[17, 2] = stationInfo[0].directioin.ToString();
                    oSheet.Cells[18, 1] = "Station Name";
                    oSheet.Cells[18, 2] = stationInfo[0].station_name.ToString();

                    //now printing the value of historical datas..
                    oSheet.Cells[19, 1] = "Past History";
                    oSheet.Cells[20, 1] = "Date";
                    oSheet.Cells[20, 2] = "Hour";
                    oSheet.Cells[20, 3] = "Minute";
                    oSheet.Cells[20, 4] = "Distance form building";                        
                    oSheet.Cells[20, 5] = "Temperature";
                    oSheet.Cells[20, 6] = "Humidity";
                    oSheet.Cells[20, 7] = "Bar Pressure";
                    oSheet.Cells[20, 8] = "Wind Speed";
                    oSheet.Cells[20, 9] = "Wind Direction";
                    oSheet.Cells[20, 10] = "Station Name";
                        //now lets print the value in loop
                        for (int i = 0; i < hist_temp_hum_list.Count; i++)
                    {
                        oSheet.Cells[21+i, 1] = hist_temp_hum_list[i].date.ToString();
                        oSheet.Cells[21+i, 2] = hist_temp_hum_list[i].hour.ToString();
                        oSheet.Cells[21+i, 3] = hist_temp_hum_list[i].minute.ToString();
                            oSheet.Cells[21 + i, 4] = hist_temp_hum_list[i].distance_from_building.ToString();
                            oSheet.Cells[21+i, 5] = hist_temp_hum_list[i].temp.ToString();
                        oSheet.Cells[21+i, 6] = hist_temp_hum_list[i].hum.ToString();
                            oSheet.Cells[21 + i, 7] = hist_temp_hum_list[i].bar_pressure.ToString();
                            oSheet.Cells[21 + i, 8] = hist_temp_hum_list[i].wind.ToString();
                            oSheet.Cells[21 + i, 9] = hist_temp_hum_list[i].direction.ToString();
                            oSheet.Cells[21 + i, 10] = hist_temp_hum_list[i].station_name.ToString();
                        }

                    //now lets open the save dialog box and the save it there..

                    String fileName = "";

                    saveFD.InitialDirectory ="C:";
                    saveFD.FileName = "Excelfile";
                    saveFD.Title = WFA_psychometric_chart.Properties.Resources.Save_Excel_file_to;
                    saveFD.Filter = "Excel file|*.xls";
                        if (saveFD.ShowDialog() == DialogResult.OK)
                        {
                            //save the file..
                            fileName = saveFD.FileName;
                            oBook.SaveAs(fileName);                        

                        }                        
                     oBook.Close();                      
                    oApp.Quit();                       
                    }
                    catch(Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
                else if (rb2_csv.Checked == true)
                {
                    //csv button is checked..

                    CreatingCsvFiles();

                }
                else
                {
                    //txt button is checked..
                    CreatngTxtFile();

                }

            }
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_select_a_valid_date_To_);
            }
            
            //resetting the values at last...
            hist_temp_hum_list.Clear();
            building_info.Clear();
            stationInfo.Clear();
        }//closing of the button btnExtract


        public void CreatingCsvFiles()
        {
            try { 
            string filePath = ""; //"Your path of the location" + "filename.csv";

            saveFD.Title = WFA_psychometric_chart.Properties.Resources.Chose_a_path_and_file_name;
            saveFD.InitialDirectory = "C:";
            saveFD.Filter = "CSV file|*.csv";
            saveFD.FileName = "CSVResultfile.csv";
            if (saveFD.ShowDialog() == DialogResult.OK)
            {
                filePath = saveFD.FileName;
            }
             else
             {
                filePath = "text.csv";
                    if (File.Exists(filePath))
                    {
                        File.Delete(filePath);
                    }
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Your_file_will_be_saved_as + filePath);
             }

            if (!File.Exists(filePath))
            {
                File.Create(filePath).Close();
            }
            //string delimiter = ",";
            //string[][] output = new string[][]{
            //new string[]{"Value1","Value2","Value3","Value4"} /*add the values that you want inside a csv file. Mostly this function can be used in a foreach loop.*/
            //};
            //int length = output.GetLength(0);
            //StringBuilder sb = new StringBuilder();
            //for (int index = 0; index < length; index++)
            //    sb.AppendLine(string.Join(delimiter, output[index]));
            //File.AppendAllText(filePath, sb.ToString());
            StringBuilder sb = new StringBuilder();
            string locationBuilding = WFA_psychometric_chart.Properties.Resources.Building_Information0;
            string locBuildingTopic = WFA_psychometric_chart.Properties.Resources.country_state_city_street_zip_;
            string locBuildingValues = building_info[0].country + "," + building_info[0].state + "," + building_info[0].city + "," + building_info[0].street + "," + building_info[0].zip + "," + building_info[0].longitude + "," + building_info[0].latitude + "," + building_info[0].elevation;

            string locationStation = WFA_psychometric_chart.Properties.Resources.Station_Information;
            string locStationTopic = WFA_psychometric_chart.Properties.Resources.location_distance_from_buildin;
            string locStationValue = stationInfo[0].location+","+ stationInfo[0].distance_from_building+","+stationInfo[0].last_update_date + "," + stationInfo[0].temp + "," + stationInfo[0].humidity + "," + stationInfo[0].bar_pressure + "," + stationInfo[0].wind + "," + stationInfo[0].directioin + "," + stationInfo[0].station_name+",";

            sb.AppendLine(locationBuilding);
            sb.AppendLine(locBuildingTopic);
            sb.AppendLine(locBuildingValues);
            sb.AppendLine(locationStation);
            sb.AppendLine(locStationTopic);
            sb.AppendLine(locStationValue);

            //now futher porcessing of the obtain data 
            string historicalData = "Historical Data,";
            string historicalTopic = "date,hour,minute,distance form building,temperature,humidity,bar pressure,wind speed,wind direction,station name ";
            sb.AppendLine(historicalData);
            sb.AppendLine(historicalTopic);

            for(int i = 0; i < hist_temp_hum_list.Count; i++)
            {
                string historicalValue = hist_temp_hum_list[i].date + "," + hist_temp_hum_list[i].hour + "," + hist_temp_hum_list[i].minute + ","+hist_temp_hum_list[i].distance_from_building+"," + hist_temp_hum_list[i].temp + "," + hist_temp_hum_list[i].hum+"," + hist_temp_hum_list[i].bar_pressure + "," + hist_temp_hum_list[i].wind+ "," + hist_temp_hum_list[i].direction+ "," + hist_temp_hum_list[i].station_name + ".";
                    sb.AppendLine(historicalValue);

              }

            File.WriteAllText(filePath, sb.ToString());
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            }//close of function CSVfil...

        private void CreatngTxtFile()
        {

            try
            {
                string filePath = ""; //"Your path of the location" + "filename.csv";

                saveFD.Title = WFA_psychometric_chart.Properties.Resources.Chose_a_path_and_file_name;
                saveFD.InitialDirectory = "C:";
                saveFD.Filter = "Text file|*.txt";
                saveFD.FileName = "TextResultfile.txt";
                if (saveFD.ShowDialog() == DialogResult.OK)
                {
                    filePath = saveFD.FileName;
                }
                else
                {
                    filePath = "text.txt";
                    if (File.Exists(filePath))
                    {
                        File.Delete(filePath);
                    }
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Your_file_will_be_saved_as + filePath);
                }

                if (!File.Exists(filePath))
                {
                    File.Create(filePath).Close();
                }
                StringBuilder sb = new StringBuilder();
                string locationBuilding = WFA_psychometric_chart.Properties.Resources.Building_Information0;
                string locBuildingTopic = WFA_psychometric_chart.Properties.Resources.country_state_city_street_zip_;
                string locBuildingValues = building_info[0].country + "," + building_info[0].state + "," + building_info[0].city + "," + building_info[0].street + "," + building_info[0].zip + "," + building_info[0].longitude + "," + building_info[0].latitude + "," + building_info[0].elevation;

                string locationStation = WFA_psychometric_chart.Properties.Resources.Station_Information;
                string locStationTopic = WFA_psychometric_chart.Properties.Resources.location_distance_from_buildin;
                string locStationValue = stationInfo[0].location + "," + stationInfo[0].distance_from_building + "," + stationInfo[0].last_update_date + "," + stationInfo[0].temp + "," + stationInfo[0].humidity + "," + stationInfo[0].bar_pressure + "," + stationInfo[0].wind + "," + stationInfo[0].directioin + "," + stationInfo[0].station_name + ",";

                sb.AppendLine(locationBuilding);
                sb.AppendLine(locBuildingTopic);
                sb.AppendLine(locBuildingValues);
                sb.AppendLine(locationStation);
                sb.AppendLine(locStationTopic);
                sb.AppendLine(locStationValue);

                //now futher porcessing of the obtain data 
                string historicalData = WFA_psychometric_chart.Properties.Resources.Historical_Data;
                string historicalTopic = "date,hour,minute,distance form building,temperature,humidity,bar pressure,wind speed,wind direction,station name ";
                sb.AppendLine(historicalData);
                sb.AppendLine(historicalTopic);

                for (int i = 0; i < hist_temp_hum_list.Count; i++)
                {
                    string historicalValue = hist_temp_hum_list[i].date + "," + hist_temp_hum_list[i].hour + "," + hist_temp_hum_list[i].minute + "," + hist_temp_hum_list[i].distance_from_building + "," + hist_temp_hum_list[i].temp + "," + hist_temp_hum_list[i].hum + "," + hist_temp_hum_list[i].bar_pressure + "," + hist_temp_hum_list[i].wind + "," + hist_temp_hum_list[i].direction + "," + hist_temp_hum_list[i].station_name + ".";
                    sb.AppendLine(historicalValue);

                }

                File.WriteAllText(filePath, sb.ToString());
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        public void HistoricalDataInfo()
        {

            //--changing all the database to the sqlite database...
            string databasePath11 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath11 + @"\db_psychrometric_project.s3db";

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";



            using (SQLiteConnection connection1 = new SQLiteConnection(connString1))
            {
                connection1.Open();



                //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                //lets pass this string to a query which does the pulling part.
                SQLiteDataReader reader1 = null;
                SQLiteCommand command1 = new SQLiteCommand("Select * from tbl_historical_data WHERE date_current BETWEEN @date_first AND @date_second AND ID=@id_value", connection1);
                command1.Parameters.AddWithValue("@date_first", dtp_From.Value);
                command1.Parameters.AddWithValue("@date_second", dtp_To.Value);
                command1.Parameters.AddWithValue("@id_value", index_selected);
                //command1.Parameters.AddWithValue("@station_name", station_name);
                reader1 = command1.ExecuteReader();
                while (reader1.Read())
                {
                    //station_name = reader["station_name"].ToString();
                    hist_temp_hum_list.Add(
                        new data_type_hum_temp
                        {
                            date = reader1["date_current"].ToString(),
                            hour = reader1["hour_current"].ToString(),
                            minute = reader1["minute_current"].ToString(),
                            distance_from_building = reader1["distance_from_building"].ToString(),
                            temp = double.Parse(reader1["temperature"].ToString()),
                            hum = double.Parse(reader1["humidity"].ToString()),
                            bar_pressure = reader1["bar_pressure"].ToString(),
                            wind = reader1["wind"].ToString(),
                            direction = reader1["direction"].ToString(),
                            station_name = reader1["station_name"].ToString()
                        });
                }//close of while loop       
                 // connection1.Close();
            }//close of database using statement 

            //test..printing.
            //string s = null;
            //for (int i = 0; i < hist_temp_hum_list.Count; i++)
            //{
            //    s += hist_temp_hum_list[i].date + ", h =" + hist_temp_hum_list[i].hour + ",temp =" + hist_temp_hum_list[i].temp + ",station = " + hist_temp_hum_list[i].station_name + "\n";
            //}
            //MessageBox.Show("value of s = "+s);



        }
        public void BuildingDataInfo()
        {

            //--changing all the database to the sqlite database...
            string databasePath11 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath11 + @"\db_psychrometric_project.s3db";

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection11 = new SQLiteConnection(connString1))
            {
                connection11.Open();



                //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                //lets pass this string to a query which does the pulling part.
                SQLiteDataReader reader1 = null;
                SQLiteCommand command1 = new SQLiteCommand("Select * from tbl_building_location WHERE ID=@id_value", connection11);
                command1.Parameters.AddWithValue("@id_value", index_selected);
                //command1.Parameters.AddWithValue("@station_name", station_name);
                reader1 = command1.ExecuteReader();
                while (reader1.Read())
                {
                    //station_name = reader["station_name"].ToString();
                    building_info.Add(
                        new building_loc_data_type
                        {
                            country = reader1["country"].ToString(),
                            state = reader1["state"].ToString(),
                            city = reader1["city"].ToString(),
                            zip = reader1["zip"].ToString(),
                            longitude = reader1["longitude"].ToString(),
                            latitude = reader1["latitude"].ToString(),
                            elevation = reader1["elevation"].ToString()

                        });
                }//close of while loop       
                 // connection1.Close();
            }//close of database using statement 

            //testing...
            //string s = null;
            //for (int i = 0; i < building_info.Count; i++)
            //{
            //    s += building_info[i].country + ", state =" + building_info[i].state + ",city =" + building_info[i].city + "\n";
            //}
            //MessageBox.Show(s);


        }

        private void StationDataInfo()
        {

            //--station data info 
            //--changing all the database to the sqlite database...
            string databasePath11 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath11 + @"\db_psychrometric_project.s3db";

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection1 = new SQLiteConnection(connString1))
            {
                connection1.Open();



                //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                //lets pass this string to a query which does the pulling part.
                SQLiteDataReader reader1 = null;
                SQLiteCommand command1 = new SQLiteCommand("Select * from tbl_weather_related_values WHERE ID=@id_value", connection1);
                command1.Parameters.AddWithValue("@id_value", index_selected);
                //command1.Parameters.AddWithValue("@station_name", station_name);
                reader1 = command1.ExecuteReader();
                while (reader1.Read())
                {
                    //station_name = reader["station_name"].ToString();
                    stationInfo.Add(
                        new station_loc_data_type
                        {
                            location = reader1["location"].ToString(),
                            distance_from_building = reader1["distance_from_building"].ToString(),
                            last_update_date = reader1["last_update_date"].ToString(),
                            temp = reader1["temp"].ToString(),
                            humidity = reader1["humidity"].ToString(),
                            bar_pressure = reader1["bar_pressure"].ToString(),
                            wind = reader1["wind"].ToString(),
                            directioin = reader1["direction"].ToString(),
                            station_name = reader1["station_name"].ToString()
                        });
                }//close of while loop       
                 // connection1.Close();
            }//close of database using statement 

            //testing...
            //string s = null;
            //for (int i = 0; i < stationInfo.Count; i++)
            //{
            //    s += stationInfo[i].location + ", distance =" + stationInfo[i].distance_from_building + ",direc =" + stationInfo[i].directioin + "\n";
            //}
            //MessageBox.Show("station info s value ="+s);

        }

        private void form_extract_data_Load(object sender, EventArgs e)
        {
            //date time formate option 
            dtp_From.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_From.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_To.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_From.Value = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.Value = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);

            //this method selects location pulls the location address 
            PullLocationInformation();

        }
        public class DataTypeTempBuildingValue
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
        }

        //ArrayList temp_building_values = new ArrayList();
        List<DataTypeTempBuildingValue> temp_building_values = new List<DataTypeTempBuildingValue>();



        private void PullLocationInformation()
        {
            try
            {
                //cb1_select_data.Items.Clear();
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

                    temp_building_values.Add(new DataTypeTempBuildingValue
                    {
                        ID = int.Parse(reader["id"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString()
                    });

                }
                ////string s = "";
                //for (int i = 0; i < temp_building_values.Count; i++)
                //{

                //    string tempValue = temp_building_values[i].ID + "," + temp_building_values[i].country + "," + temp_building_values[i].state + "," + temp_building_values[i].city;
                //  //  cb1_select_data.Items.Add(tempValue);
                //    //s += stored_location[i] + " , \n";
                //}

                index_selected = temp_building_values[0].ID;
                // MessageBox.Show("stored place = " + s);
                comm.Dispose();
                reader.Dispose();
                connection.Close();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

       
        

       

      public void form_export_data_Disposed ( object sender, System.EventArgs e )
      {
      }

     
    }
}
