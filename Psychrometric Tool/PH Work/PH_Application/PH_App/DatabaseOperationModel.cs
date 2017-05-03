using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Data.SQLite;
using System.Configuration;
using System.Data;
using System.Windows.Forms;
using System.Drawing;

namespace PH_App
{
    public class DatabaseOperationModel :NodeAndLineClass
    {
        public class SelectedBuildingDatatype
        {
            public string Main_BuildingName { get; set; }
            public string Building_Name { get; set; }           
            public string default_subBuilding { get; set; }
            public string building_path { get; set; } //Here is the building Path we need
            public string longitude { get; set; }
            public string latitude { get; set; }
            public string elevation { get; set; }
            public string ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
            public string EngineeringUnits { get; set; }
        }
       public List<SelectedBuildingDatatype> buildingSelected = new List<SelectedBuildingDatatype>();
        public List<SelectedBuildingDatatype> FindPathOfBuildingInT3000()
        {
          
            string againDbPath = @"Data Source=" + Path.GetFullPath(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), @"..\")) + @"Database\T3000.db";
            using (var connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();               
                var command = new SQLiteCommand("SELECT * from Building WHERE Default_SubBuilding = '1' ", connection);
                var reader = command.ExecuteReader();
                while (reader.Read())
                {
                    buildingSelected.Add(new SelectedBuildingDatatype
                    {
                        Main_BuildingName = reader["Main_BuildingName"].ToString(),
                        Building_Name = reader["Building_Name"].ToString(),
                        default_subBuilding = reader["Default_SubBuilding"].ToString(),
                        building_path = reader["Building_Path"].ToString(),
                        longitude = reader["Longitude"].ToString(),
                        latitude = reader["Latitude"].ToString(),
                        elevation = reader["Elevation"].ToString(),
                        ID = reader["ID"].ToString(),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString(),
                        street = reader["street"].ToString(),
                        EngineeringUnits = reader["EngineeringUnits"].ToString()                        
                    });
                }
            }//closes connection and disposes them
            return buildingSelected;
        }//Close of the function

        public string ReadVersionInfo(string tableName)
        {
            string versionValue="";
            //string tableName1 = tableName;//"tbl_Database_Version";//currentNodeTableFromDB;             
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";            
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand("SELECT * From  " + tableName, thisConnection);
                thisConnection.Open();
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    versionValue = reader["version"].ToString();
                }
            }//close of using
            return versionValue;
        }

        /// <summary>
        /// Create all the tables which are not present 
        /// </summary>
        /// <param name="buildingNameSelected">Name of the building selected in T3000</param>
        public void CreateRequiredTable(string buildingNameSelected)
        {
          
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection($"Data Source={databaseFile};Version=3;"))
            {
                m_dbConnection.Open();
                string sql = "create table IF NOT EXISTS tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255) ,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
                command.ExecuteNonQuery();

                //--next table historical data:tbl_historical_data
                //string sql2 = "create table IF NOT EXISTS tbl_historical_data (ID INTEGER,date_current datetime,hour_current int,minute_current int,distance_from_building varchar(255),temperature varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                //SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
                //command2.ExecuteNonQuery();


                string sql3 = "create table IF NOT EXISTS tbl_language_option (ID int, language_id int)";
                SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                command3.ExecuteNonQuery();


                ////--next table weather related datas...
                string sql4 = "create table IF NOT EXISTS tbl_weather_related_values (ID INTEGER ,location varchar(255),distance_from_building varchar(255),last_update_date varchar(255),temp varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, m_dbConnection);
                command4.ExecuteNonQuery();


                //==================Lets create some more essential tables  added in jan 26th 2017=========//

                /*
                 Tables to be added in this section 
                 1.tbl_[] _chart_comfort_zone_setting
                 2.tbl_[]_chart_detail
                 3.tbl_[]_comfort_zone_detail
                 4.tbl_[]_device_info_for_node
                 5.tbl_[]_input_storage_from_T3000
                 6.tbl_[]_line_value
                 7.tbl_[]_mix_node_info
                 8.tbl_[]_node_data_related_T300
                 9. tbl_[]_node_value
                 10.tbl_[]_TemperatureHumiditySourceInfo
                 11.tbl_[]_Weather_Controller_Restor_Info
                 12. tbl_[]_Weather_Web_Restor_Info
                 13.tbl_[]_Weather_HumSelfCalibration_Restor_Info
                 14.tbl_[]_Database_Version
                 */
               // string tbl_comfortzoneSetting = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_comfort_zone_setting  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                string tbl_chart_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_detail(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                //string tbl_comfort_zone_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_comfort_zone_detail  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                string tbl_device_info_for_node = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_device_info_for_node  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                string tbl_input_storage_form_T3000 = " CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_input_storage_from_T3000  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                string tbl_line_value = " CREATE TABLE  IF NOT EXISTS tbl_" + buildingNameSelected + "_line_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                //string tbl_mix_node_info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_mix_node_info  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                string tbl_node_data_related_T3000 = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_data_related_T3000  (count INTEGER PRIMARY KEY AUTOINCREMENT, nodeID varchar(255), param1_panelID varchar(255), param1_inputIndex varchar(255), param2_panelID varchar(255), param2_inputIndex varchar(255)) ";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";


                //string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperatureHumiditySourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),HumiditySourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                //MessageBox.Show("Creating tables from update section");

                //now execute the query
                //SQLiteCommand cm101 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                //cm101.ExecuteNonQuery();

                SQLiteCommand cm201 = new SQLiteCommand(tbl_chart_detail, m_dbConnection);
                cm201.ExecuteNonQuery();

                //SQLiteCommand cm301 = new SQLiteCommand(tbl_comfort_zone_detail, m_dbConnection);
                //cm301.ExecuteNonQuery();

                SQLiteCommand cm401 = new SQLiteCommand(tbl_device_info_for_node, m_dbConnection);
                cm401.ExecuteNonQuery();

                SQLiteCommand cm501 = new SQLiteCommand(tbl_input_storage_form_T3000, m_dbConnection);
                cm501.ExecuteNonQuery();

                SQLiteCommand cm601 = new SQLiteCommand(tbl_line_value, m_dbConnection);
                cm601.ExecuteNonQuery();

                //SQLiteCommand cm701 = new SQLiteCommand(tbl_mix_node_info, m_dbConnection);
                //cm701.ExecuteNonQuery();

                SQLiteCommand cm801 = new SQLiteCommand(tbl_node_data_related_T3000, m_dbConnection);
                cm801.ExecuteNonQuery();

                SQLiteCommand cm901 = new SQLiteCommand(tbl_node_value, m_dbConnection);
                cm901.ExecuteNonQuery();

                //SQLiteCommand cm1001 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                //cm1001.ExecuteNonQuery();

                //SQLiteCommand cm1101 = new SQLiteCommand(tbl_Weather_Controller_Restor_Info, m_dbConnection);
                //cm1101.ExecuteNonQuery();

                //SQLiteCommand cm1201 = new SQLiteCommand(tbl_Weather_Web_Restor_Info, m_dbConnection);
                //cm1201.ExecuteNonQuery();

                //SQLiteCommand cm1301 = new SQLiteCommand(tbl_Weather_HumSelfCalibration_Restor_Info, m_dbConnection);
                //cm1301.ExecuteNonQuery();

                SQLiteCommand cm1401 = new SQLiteCommand(tbl_database_version, m_dbConnection);
                cm1401.ExecuteNonQuery();

            }//--Close of using statement 
        }//--Close of method

        public class DataTypeOfBuildingInPsychro
        {
            public int selection { get; set; }
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
            public string longitude { get; set; }
            public string latitude { get; set; }
            public string elevation { get; set; }
            public string BuildingName { get; set; }
            public string EngineeringUnits { get; set; }
        }

       // public List<DataTypeOfBuildingInPsychro> listBuildingSelectedInfoInPsycho  = new List<DataTypeOfBuildingInPsychro>();
        /// <summary>
        /// Function does two task 
        /// 1. it finds which building is selected and its values
        /// 2. it returns true if the building is selected and false if none is selected
        /// </summary>
        /// <returns></returns>

        public bool ReadDataForBuildingSelectedFromPsychrometric()
        {
            selectedBuildingList.Clear();
            bool returnVal = false;
            string tableName = "tbl_building_location";//currentNodeTableFromDB; 
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sql1 = $"SELECT * From   { tableName } where  selection='1'";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                thisConnection.Open();
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    selectedBuildingList.Add(new DataTypeOfBuildingInPsychro
                    {
                        selection = int.Parse(reader["selection"].ToString()),
                        ID = int.Parse(reader["ID"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString(),
                        street = reader["street"].ToString(),
                        longitude = reader["longitude"].ToString(),
                        latitude = reader["latitude"].ToString(),
                        elevation = reader["elevation"].ToString(),
                        BuildingName = reader["BuildingName"].ToString(),
                        EngineeringUnits = reader["EngineeringUnits"].ToString()
                    });
                    returnVal = true;
                }
            }//close of using


            return returnVal;
        }

        public void ReadDataPresentOrNotThenInsertBuildingValue(string country,string state,string city,string street,string longitude,string latitude,string elevation,string buildingName,string EngineeringUnits)
        {
            if (ReadDataForBuildingSelectedFromPsychrometric() == false)
            {
                //--If there is no data selected we need to insert the new data
                //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";
                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];

                //Form1_main f1 = new Form1_main();
                //--new database file 
                //SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                using (SQLiteConnection m_dbConnection = new SQLiteConnection($"Data Source={databaseFile};Version=3;"))
                {
                    m_dbConnection.Open();
                    string sql_stringx = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                    SQLiteCommand command9 = new SQLiteCommand(sql_stringx, m_dbConnection);
                    command9.CommandType = CommandType.Text;
                    command9.Parameters.AddWithValue("@sel", "1");
                    command9.Parameters.AddWithValue("@con", country);
                    command9.Parameters.AddWithValue("@state", state);
                    command9.Parameters.AddWithValue("@city", city);
                    command9.Parameters.AddWithValue("@stre", street);
                    command9.Parameters.AddWithValue("@lng", longitude);
                    command9.Parameters.AddWithValue("@lat", latitude);
                    command9.Parameters.AddWithValue("@elev",elevation);
                    command9.Parameters.AddWithValue("@bname",   buildingName);
                    command9.Parameters.AddWithValue("@engUnit", EngineeringUnits);
                    command9.ExecuteNonQuery();
                }
            }

        }

        public void UpdateVersionInfoAfterUpdating(string version)
        {
            string tableName = "tbl_Database_Version";//currentNodeTableFromDB; 
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sqlQuery = $"update {tableName} set version = '{version}' where count='1'";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                thisConnection.Open();
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, thisConnection);
                cmd.ExecuteNonQuery();
            }//close of using

        }

        public List<DataTypeOfBuildingInPsychro> listOfAllBuildingInPsychro = new List<DataTypeOfBuildingInPsychro>();

        public void ReadAllBuildingInfoFromPsychrometric()
        {
            listOfAllBuildingInPsychro.Clear();
            // bool returnVal = false;
            string tableName = "tbl_building_location";//currentNodeTableFromDB; 
                                                       //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                       // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sqlQuery = $"SELECT * From   { tableName } ";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, thisConnection);
                thisConnection.Open();
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    listOfAllBuildingInPsychro.Add(new DataTypeOfBuildingInPsychro
                    {
                        selection = int.Parse(reader["selection"].ToString()),
                        ID = int.Parse(reader["ID"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString(),
                        street = reader["street"].ToString(),
                        longitude = reader["longitude"].ToString(),
                        latitude = reader["latitude"].ToString(),
                        elevation = reader["elevation"].ToString(),
                        BuildingName = reader["BuildingName"].ToString(),
                        EngineeringUnits = reader["EngineeringUnits"].ToString()
                    });


                }
            }//close of using

        }

        public void SelectBuildingInPsychro(string buildingNameToBeSelected)
        {
            //--Steps unselect all the building and then select the one building 
            string tableName = "tbl_building_location";//currentNodeTableFromDB; 
                                                       //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                       //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            bool buildingPresent = false;
            //---Read data if the building name is present then only do updating else dont do that
            string sql1 = $"SELECT * From   { tableName } where BuildingName='{buildingNameToBeSelected}'";
            using (var thisConnection = new SQLiteConnection(connString1))
            {
                var cmd = new SQLiteCommand(sql1, thisConnection);
                thisConnection.Open();
                var reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    buildingPresent = true;

                }
            }//close of using


            if (buildingPresent == true)
            {

                //--Unselecting all the buildings
                using (var thisConnection = new SQLiteConnection(connString1))
                {

                    thisConnection.Open();
                    string sql_string = $"update  {tableName }  set selection=@sel";
                    var command = new SQLiteCommand(sql_string, thisConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@sel", 0);

                    command.ExecuteNonQuery();
                }//close of using

                //--Now selecting the one building
                using (var thisConnection = new SQLiteConnection(connString1))
                {
                    // SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                    thisConnection.Open();
                    string sql_string = $"update  {tableName }  set selection=@sel where BuildingName=@build";
                    var command = new SQLiteCommand(sql_string, thisConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@sel", 1);
                    command.Parameters.AddWithValue("@build", buildingNameToBeSelected);
                    command.ExecuteNonQuery();
                }//close of using



            }//Close of building present 

        }

        public void WriteT3000BuildingInfoToPsychoDB(string selection, string country, string state, string city, string street, string lng, string lat, string elev, string buildingName, string engUnits)
        {

            //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device";   // currentLineTableFromDB;
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@sel", selection);
                command.Parameters.AddWithValue("@con", country);
                command.Parameters.AddWithValue("@state", state);
                command.Parameters.AddWithValue("@city", city);
                command.Parameters.AddWithValue("@stre", street);
                command.Parameters.AddWithValue("@lng", lng);
                command.Parameters.AddWithValue("@lat", lat);
                command.Parameters.AddWithValue("@elev", elev);
                command.Parameters.AddWithValue("@bname", buildingName);
                command.Parameters.AddWithValue("@engUnit", engUnits);
                command.ExecuteNonQuery();
            }



        }

        public void UpdateIDOneSelectBuildingInPsychro()
        {
            //--Steps unselect all the building and then select the one building 
            string tableName = "tbl_building_location";//currentNodeTableFromDB; 
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            //--Unselecting all the buildings
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {

                thisConnection.Open();
                string sql_string = $"update  {tableName }  set selection=@sel where ID = '1'";
                SQLiteCommand command = new SQLiteCommand(sql_string, thisConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@sel", 1);
                command.ExecuteNonQuery();
            }//close of using

        }

        public void sqlite_database_creation(string buildingNameSelected, string country, string state, string city, string street, string longitude, string latitude, string elevation, string buildingName, string EngineeringUnits)
        {

            //--lets do try catch
            try
            {
                //--This is where we are going to create all the database  and tables of sqlite
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";

                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                //string connString = @"Data Source=" + databaseFile1 + ";Version=3;";

                //  MessageBox.Show("Create table section path Path = " + databaseFile);
                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                m_dbConnection.Open();

                //--building location table : tbl_building_location
                //--This one is with the zip code later zip code is removed
                //string sql = "create table tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255), ZIP int,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";
                string sql = "create table IF NOT EXISTS tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255) ,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";

                SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
                command.ExecuteNonQuery();
                //--next table geo location value : tbl_geo_location_value
                //string sql1 = "create table tbl_geo_location_value (ID int ,longitude varchar(255),latitude varchar(255),elevation varchar(255))";
                //SQLiteCommand command1 = new SQLiteCommand(sql1, m_dbConnection);
                //command1.ExecuteNonQuery();

                //--next table historical data:tbl_historical_data
                //string sql2 = "create table IF NOT EXISTS tbl_historical_data (ID INTEGER,date_current datetime,hour_current int,minute_current int,distance_from_building varchar(255),temperature varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                //SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
                //command2.ExecuteNonQuery();
                //--next table tbl_temp_himidity 
                //string sql3 = "create table tbl_temp_humidity (temp int,humidity int)";
                //SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                //command3.ExecuteNonQuery();

                string sql3 = "create table IF NOT EXISTS tbl_language_option (ID int, language_id int)";
                SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                command3.ExecuteNonQuery();


                ////--next table weather related datas...
                string sql4 = "create table IF NOT EXISTS tbl_weather_related_values (ID INTEGER ,location varchar(255),distance_from_building varchar(255),last_update_date varchar(255),temp varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, m_dbConnection);
                command4.ExecuteNonQuery();


                //==================Lets create some more essential tables  added in jan 26th 2017=========//

                /*
                 Tables to be added in this section 
                 1.tbl_[] _chart_comfort_zone_setting
                 2.tbl_[]_chart_detail
                 3.tbl_[]_comfort_zone_detail
                 4.tbl_[]_device_info_for_node
                 5.tbl_[]_input_storage_from_T3000
                 6.tbl_[]_line_value
                 7.tbl_[]_mix_node_info
                 8.tbl_[]_node_data_related_T300
                 9. tbl_[]_node_value
                 */
                // string tbl_comfortzoneSetting = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_comfort_zone_setting  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                string tbl_chart_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_detail(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                //string tbl_comfort_zone_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_comfort_zone_detail  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                string tbl_device_info_for_node = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_device_info_for_node  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                string tbl_input_storage_form_T3000 = " CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_input_storage_from_T3000  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                string tbl_line_value = " CREATE TABLE  IF NOT EXISTS tbl_" + buildingNameSelected + "_line_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                //string tbl_mix_node_info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_mix_node_info  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                string tbl_node_data_related_T3000 = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_data_related_T3000  (count INTEGER PRIMARY KEY AUTOINCREMENT, nodeID varchar(255), param1_panelID varchar(255), param1_inputIndex varchar(255), param2_panelID varchar(255), param2_inputIndex varchar(255)) ";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";


                //string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperatureHumiditySourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),HumiditySourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                //now execute the query
                //SQLiteCommand cm1 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                //cm1.ExecuteNonQuery();

                SQLiteCommand cm2 = new SQLiteCommand(tbl_chart_detail, m_dbConnection);
                cm2.ExecuteNonQuery();

                //SQLiteCommand cm3 = new SQLiteCommand(tbl_comfort_zone_detail, m_dbConnection);
                //cm3.ExecuteNonQuery();

                SQLiteCommand cm4 = new SQLiteCommand(tbl_device_info_for_node, m_dbConnection);
                cm4.ExecuteNonQuery();

                SQLiteCommand cm5 = new SQLiteCommand(tbl_input_storage_form_T3000, m_dbConnection);
                cm5.ExecuteNonQuery();

                SQLiteCommand cm6 = new SQLiteCommand(tbl_line_value, m_dbConnection);
                cm6.ExecuteNonQuery();

                //SQLiteCommand cm7 = new SQLiteCommand(tbl_mix_node_info, m_dbConnection);
                //cm7.ExecuteNonQuery();

                SQLiteCommand cm8 = new SQLiteCommand(tbl_node_data_related_T3000, m_dbConnection);
                cm8.ExecuteNonQuery();

                SQLiteCommand cm9 = new SQLiteCommand(tbl_node_value, m_dbConnection);
                cm9.ExecuteNonQuery();

                //SQLiteCommand cm10 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                //cm10.ExecuteNonQuery();

                //SQLiteCommand cm11 = new SQLiteCommand(tbl_Weather_Controller_Restor_Info, m_dbConnection);
                //cm11.ExecuteNonQuery();

                //SQLiteCommand cm12 = new SQLiteCommand(tbl_Weather_Web_Restor_Info, m_dbConnection);
                //cm12.ExecuteNonQuery();

                //SQLiteCommand cm13 = new SQLiteCommand(tbl_Weather_HumSelfCalibration_Restor_Info, m_dbConnection);
                //cm13.ExecuteNonQuery();

                SQLiteCommand cm14 = new SQLiteCommand(tbl_database_version, m_dbConnection);
                cm14.ExecuteNonQuery();

                /*
                Now lets read the data form alex database and store it in the our db_psychometric_project.s3db databse                  
                //==Read complete now writing the values to our database                               
                */

                //--At least adding default_building for first time in the database
                string defaultBuildingSQL = "Select * from tbl_building_location where BuildingName = 'Default_Building'";

                bool buildingPresent = false;
                SQLiteCommand cmd = new SQLiteCommand(defaultBuildingSQL, m_dbConnection);

                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    buildingPresent = true;

                }
                if (buildingPresent != true)
                {
                    //--Default buildig setting not present so insert the default building setting
                    string sqlStringDefaultBuilding = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                    SQLiteCommand commandDefaultBuilding = new SQLiteCommand(sqlStringDefaultBuilding, m_dbConnection);
                    commandDefaultBuilding.CommandType = CommandType.Text;
                    commandDefaultBuilding.Parameters.AddWithValue("@sel", "0");
                    commandDefaultBuilding.Parameters.AddWithValue("@con", "china");
                    commandDefaultBuilding.Parameters.AddWithValue("@state", "shanghai");
                    commandDefaultBuilding.Parameters.AddWithValue("@city", "shanghai");
                    commandDefaultBuilding.Parameters.AddWithValue("@stre", "");
                    commandDefaultBuilding.Parameters.AddWithValue("@lng", "121.516799926758");
                    commandDefaultBuilding.Parameters.AddWithValue("@lat", "31.169605255127");
                    commandDefaultBuilding.Parameters.AddWithValue("@elev", "4");
                    commandDefaultBuilding.Parameters.AddWithValue("@bname", "Default_Building");
                    commandDefaultBuilding.Parameters.AddWithValue("@engUnit", "SI");
                    commandDefaultBuilding.ExecuteNonQuery();
                }


                //MessageBox.Show("We have created all tables now insertion is left");
                //This function will write to tbl_bulding_location as well as will make a building selected
                //WriteT3000BuildingInfoToPsychoDB("1", BuildingSelected[0].country, BuildingSelected[0].state, BuildingSelected[0].city, BuildingSelected[0].street, BuildingSelected[0].longitude, BuildingSelected[0].latitude, BuildingSelected[0].elevation, BuildingSelected[0].Building_Name, BuildingSelected[0].EngineeringUnits);

                string sql_stringx = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                SQLiteCommand command9 = new SQLiteCommand(sql_stringx, m_dbConnection);
                command9.CommandType = CommandType.Text;
                command9.Parameters.AddWithValue("@sel", "1");
                command9.Parameters.AddWithValue("@con", country);
                command9.Parameters.AddWithValue("@state", state);
                command9.Parameters.AddWithValue("@city", city);
                command9.Parameters.AddWithValue("@stre", street);
                command9.Parameters.AddWithValue("@lng",longitude);
                command9.Parameters.AddWithValue("@lat",latitude);
                command9.Parameters.AddWithValue("@elev",elevation);
                command9.Parameters.AddWithValue("@bname", buildingNameSelected);
                command9.Parameters.AddWithValue("@engUnit",EngineeringUnits);
                command9.ExecuteNonQuery();

                //==Now lets make a single default comfort zone in our database.
                //--This one is for default value.
                //InitialDatabaseSetupController idb = new InitialDatabaseSetupController();
                //string id = idb.GetGUID();
                //string sql_data_for_comfortzone = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id + "', 'Default_comfort_zone','22','28','60','80','GREEN')";
                //SQLiteCommand cmddata = new SQLiteCommand(sql_data_for_comfortzone, m_dbConnection);
                //cmddata.ExecuteNonQuery();

                //--comfortzone airport.
                //string id_airport = idb.GetGUID();
                //string sql_comfortzone_airport = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_airport + "', 'Airport','22','28','35','65','Blue')";
                //SQLiteCommand cmd_airport = new SQLiteCommand(sql_comfortzone_airport, m_dbConnection);
                //cmd_airport.ExecuteNonQuery();

                ////--comfortzone club.
                //string id_club = idb.GetGUID();
                //string sql_comfortzone_club = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_club + "', 'Club','22','28','35','65','Blueviolet')";
                //SQLiteCommand cmd_club = new SQLiteCommand(sql_comfortzone_club, m_dbConnection);
                //cmd_club.ExecuteNonQuery();

                ////--comfortzone computer.
                //string id_computer = idb.GetGUID();
                //string sql_comfortzone_computer = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_computer + "', 'Computer','22','28','35','65','Turquoise')";
                //SQLiteCommand cmd_computer = new SQLiteCommand(sql_comfortzone_computer, m_dbConnection);
                //cmd_computer.ExecuteNonQuery();

                ////--comfortzone datacenter.
                //string id_datacenter = idb.GetGUID();
                //string sql_comfortzone_datacenter = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_datacenter + "', 'Data Center','22','28','35','65','Lightgreen')";
                //SQLiteCommand cmd_datacenter = new SQLiteCommand(sql_comfortzone_datacenter, m_dbConnection);
                //cmd_datacenter.ExecuteNonQuery();

                ////--comfortzone gym.
                //string id_gym = idb.GetGUID();
                //string sql_comfortzone_gym = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_gym + "', 'Gym','22','28','35','65','Lightgray')";
                //SQLiteCommand cmd_gym = new SQLiteCommand(sql_comfortzone_gym, m_dbConnection);
                //cmd_gym.ExecuteNonQuery();

                ////--comfortzone Hospital.
                //string id_Hospital = GetGUID();
                //string sql_comfortzone_Hospital = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Hospital + "', 'Hospital','22','28','35','65','Red')";
                //SQLiteCommand cmd_Hospital = new SQLiteCommand(sql_comfortzone_Hospital, m_dbConnection);
                //cmd_Hospital.ExecuteNonQuery();

                ////--comfortzone Music.
                //string id_Music = GetGUID();
                //string sql_comfortzone_Music = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Music + "', 'Music','22','28','35','65','Orangered')";
                //SQLiteCommand cmd_Music = new SQLiteCommand(sql_comfortzone_Music, m_dbConnection);
                //cmd_Music.ExecuteNonQuery();

                //--For inserting the version value first time while creating database
                string version = AssemblyDateGeneration.Value.ToShortDateString();
                string sql_database_version = "INSERT INTO  tbl_Database_Version( version)   VALUES('" + version + "')";
                SQLiteCommand cmd_database_version = new SQLiteCommand(sql_database_version, m_dbConnection);
                cmd_database_version.ExecuteNonQuery();

                //===============END OF THE NEW CODE ADDED IN THIS SECTION==========================//




                //--Lets input some values in the tbl_building_location and in tbl_language_option default 

                //string sql_input1 = "INSERT INTO tbl_building_location (selection,country,state,city,street,BuildingName,EngineeringUnits) VALUES(1, 'china','SangHai','SangHai','No.35,yi yuan garden','Default_Building','SI') ";
                //SQLiteCommand commandINput5 = new SQLiteCommand(sql_input1, m_dbConnection);
                //commandINput5.ExecuteNonQuery();

                //MessageBox.Show("Test We reached this part ln 5207");
                //Adding to language option
                string sql_input2 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(1, 1) ";
                string sql_input3 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(2, 0) ";
                string sql_input4 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(3, 0) ";

                SQLiteCommand c2 = new SQLiteCommand(sql_input2, m_dbConnection);
                c2.ExecuteNonQuery();

                SQLiteCommand c3 = new SQLiteCommand(sql_input3, m_dbConnection);
                c3.ExecuteNonQuery();
                SQLiteCommand c4 = new SQLiteCommand(sql_input4, m_dbConnection);
                c4.ExecuteNonQuery();
                m_dbConnection.Close();//--closing the connection

            }
            catch (Exception ex)
            {


                //string databasePath133 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath133 + @"\db_psychrometric_project.s3db";

                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                //try
                //{
                if (File.Exists(databaseFile))
                {
                    File.Delete(databaseFile);
                }

                //}
                //catch { }
                Application.Exit();


            }

        }

        public void sqlite_database_creationWithDefaultSettingsOnly(string buildingNameSelected)
        {
            //--lets do try catch
            try
            {
                //--This is where we are going to create all the database  and tables of sqlite
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";

                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                //string connString = @"Data Source=" + databaseFile1 + ";Version=3;";

                //  MessageBox.Show("Create table section path Path = " + databaseFile);
                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                m_dbConnection.Open();

                //--building location table : tbl_building_location
                //--This one is with the zip code later zip code is removed
                //string sql = "create table tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255), ZIP int,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";
                string sql = "create table IF NOT EXISTS tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255) ,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";

                SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
                command.ExecuteNonQuery();
                //--next table geo location value : tbl_geo_location_value
                //string sql1 = "create table tbl_geo_location_value (ID int ,longitude varchar(255),latitude varchar(255),elevation varchar(255))";
                //SQLiteCommand command1 = new SQLiteCommand(sql1, m_dbConnection);
                //command1.ExecuteNonQuery();

                //--next table historical data:tbl_historical_data
                //string sql2 = "create table IF NOT EXISTS tbl_historical_data (ID INTEGER,date_current datetime,hour_current int,minute_current int,distance_from_building varchar(255),temperature varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                //SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
                //command2.ExecuteNonQuery();
                //--next table tbl_temp_himidity 
                //string sql3 = "create table tbl_temp_humidity (temp int,humidity int)";
                //SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                //command3.ExecuteNonQuery();

                string sql3 = "create table IF NOT EXISTS tbl_language_option (ID int, language_id int)";
                SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                command3.ExecuteNonQuery();


                ////--next table weather related datas...
                string sql4 = "create table IF NOT EXISTS tbl_weather_related_values (ID INTEGER ,location varchar(255),distance_from_building varchar(255),last_update_date varchar(255),temp varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, m_dbConnection);
                command4.ExecuteNonQuery();


                //==================Lets create some more essential tables  added in jan 26th 2017=========//

                /*
                 Tables to be added in this section 
                 1.tbl_[] _chart_comfort_zone_setting
                 2.tbl_[]_chart_detail
                 3.tbl_[]_comfort_zone_detail
                 4.tbl_[]_device_info_for_node
                 5.tbl_[]_input_storage_from_T3000
                 6.tbl_[]_line_value
                 7.tbl_[]_mix_node_info
                 8.tbl_[]_node_data_related_T300
                 9. tbl_[]_node_value
                 */
                // string tbl_comfortzoneSetting = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_comfort_zone_setting  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                string tbl_chart_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_detail(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                //string tbl_comfort_zone_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_comfort_zone_detail  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                string tbl_device_info_for_node = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_device_info_for_node  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                string tbl_input_storage_form_T3000 = " CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_input_storage_from_T3000  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                string tbl_line_value = " CREATE TABLE  IF NOT EXISTS tbl_" + buildingNameSelected + "_line_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                //string tbl_mix_node_info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_mix_node_info  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                string tbl_node_data_related_T3000 = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_data_related_T3000  (count INTEGER PRIMARY KEY AUTOINCREMENT, nodeID varchar(255), param1_panelID varchar(255), param1_inputIndex varchar(255), param2_panelID varchar(255), param2_inputIndex varchar(255)) ";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";


                //string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperatureHumiditySourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),HumiditySourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                //MessageBox.Show("Creating tables from update section");

                //now execute the query
                //SQLiteCommand cm101 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                //cm101.ExecuteNonQuery();

                SQLiteCommand cm201 = new SQLiteCommand(tbl_chart_detail, m_dbConnection);
                cm201.ExecuteNonQuery();

                //SQLiteCommand cm301 = new SQLiteCommand(tbl_comfort_zone_detail, m_dbConnection);
                //cm301.ExecuteNonQuery();

                SQLiteCommand cm401 = new SQLiteCommand(tbl_device_info_for_node, m_dbConnection);
                cm401.ExecuteNonQuery();

                SQLiteCommand cm501 = new SQLiteCommand(tbl_input_storage_form_T3000, m_dbConnection);
                cm501.ExecuteNonQuery();

                SQLiteCommand cm601 = new SQLiteCommand(tbl_line_value, m_dbConnection);
                cm601.ExecuteNonQuery();

                //SQLiteCommand cm701 = new SQLiteCommand(tbl_mix_node_info, m_dbConnection);
                //cm701.ExecuteNonQuery();

                SQLiteCommand cm801 = new SQLiteCommand(tbl_node_data_related_T3000, m_dbConnection);
                cm801.ExecuteNonQuery();

                SQLiteCommand cm901 = new SQLiteCommand(tbl_node_value, m_dbConnection);
                cm901.ExecuteNonQuery();

                //SQLiteCommand cm1001 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                //cm1001.ExecuteNonQuery();

                //SQLiteCommand cm1101 = new SQLiteCommand(tbl_Weather_Controller_Restor_Info, m_dbConnection);
                //cm1101.ExecuteNonQuery();

                //SQLiteCommand cm1201 = new SQLiteCommand(tbl_Weather_Web_Restor_Info, m_dbConnection);
                //cm1201.ExecuteNonQuery();

                //SQLiteCommand cm1301 = new SQLiteCommand(tbl_Weather_HumSelfCalibration_Restor_Info, m_dbConnection);
                //cm1301.ExecuteNonQuery();

                SQLiteCommand cm1401 = new SQLiteCommand(tbl_database_version, m_dbConnection);
                cm1401.ExecuteNonQuery();

                /*
                Now lets read the data form alex database and store it in the our db_psychometric_project.s3db databse                  
                //==Read complete now writing the values to our database                               
                */

                //--At least adding default_building for first time in the database
                string defaultBuildingSQL = "Select * from tbl_building_location where BuildingName = 'Default_Building'";

                bool buildingPresent = false;
                var cmd = new SQLiteCommand(defaultBuildingSQL, m_dbConnection);

                var reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    buildingPresent = true;

                }
                if (buildingPresent != true)
                {
                    //--Default buildig setting not present so insert the default building setting
                    string sqlStringDefaultBuilding = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                    var commandDefaultBuilding = new SQLiteCommand(sqlStringDefaultBuilding, m_dbConnection);
                    commandDefaultBuilding.CommandType = CommandType.Text;
                    commandDefaultBuilding.Parameters.AddWithValue("@sel", "0");
                    commandDefaultBuilding.Parameters.AddWithValue("@con", "china");
                    commandDefaultBuilding.Parameters.AddWithValue("@state", "shanghai");
                    commandDefaultBuilding.Parameters.AddWithValue("@city", "shanghai");
                    commandDefaultBuilding.Parameters.AddWithValue("@stre", "");
                    commandDefaultBuilding.Parameters.AddWithValue("@lng", "121.516799926758");
                    commandDefaultBuilding.Parameters.AddWithValue("@lat", "31.169605255127");
                    commandDefaultBuilding.Parameters.AddWithValue("@elev", "4");
                    commandDefaultBuilding.Parameters.AddWithValue("@bname", buildingNameSelected);//buildingNameSelected //Default_Building
                    commandDefaultBuilding.Parameters.AddWithValue("@engUnit", "SI");
                    commandDefaultBuilding.ExecuteNonQuery();
                }


                //MessageBox.Show("We have created all tables now insertion is left");
                //This function will write to tbl_bulding_location as well as will make a building selected
                //WriteT3000BuildingInfoToPsychoDB("1", BuildingSelected[0].country, BuildingSelected[0].state, BuildingSelected[0].city, BuildingSelected[0].street, BuildingSelected[0].longitude, BuildingSelected[0].latitude, BuildingSelected[0].elevation, BuildingSelected[0].Building_Name, BuildingSelected[0].EngineeringUnits);

                //string sql_stringx = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                //SQLiteCommand command9 = new SQLiteCommand(sql_stringx, m_dbConnection);
                //command9.CommandType = CommandType.Text;
                //command9.Parameters.AddWithValue("@sel", "1");
                //command9.Parameters.AddWithValue("@con", BuildingSelected[0].country);
                //command9.Parameters.AddWithValue("@state", BuildingSelected[0].state);
                //command9.Parameters.AddWithValue("@city", BuildingSelected[0].city);
                //command9.Parameters.AddWithValue("@stre", BuildingSelected[0].street);
                //command9.Parameters.AddWithValue("@lng", BuildingSelected[0].longitude);
                //command9.Parameters.AddWithValue("@lat", BuildingSelected[0].latitude);
                //command9.Parameters.AddWithValue("@elev", BuildingSelected[0].elevation);
                //command9.Parameters.AddWithValue("@bname", BuildingSelected[0].Building_Name);
                //command9.Parameters.AddWithValue("@engUnit", BuildingSelected[0].EngineeringUnits);
                //command9.ExecuteNonQuery();

                //==Now lets make a single default comfort zone in our database.
                //--This one is for default value.
                //string id = GetGUID();
                //string sql_data_for_comfortzone = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id + "', 'Default_comfort_zone','22','28','60','80','GREEN')";
                //SQLiteCommand cmddata = new SQLiteCommand(sql_data_for_comfortzone, m_dbConnection);
                //cmddata.ExecuteNonQuery();

                ////--comfortzone airport.
                //string id_airport = GetGUID();
                //string sql_comfortzone_airport = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_airport + "', 'Airport','22','28','35','65','Blue')";
                //SQLiteCommand cmd_airport = new SQLiteCommand(sql_comfortzone_airport, m_dbConnection);
                //cmd_airport.ExecuteNonQuery();

                ////--comfortzone club.
                //string id_club = GetGUID();
                //string sql_comfortzone_club = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_club + "', 'Club','22','28','35','65','Blueviolet')";
                //SQLiteCommand cmd_club = new SQLiteCommand(sql_comfortzone_club, m_dbConnection);
                //cmd_club.ExecuteNonQuery();

                ////--comfortzone computer.
                //string id_computer = GetGUID();
                //string sql_comfortzone_computer = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_computer + "', 'Computer','22','28','35','65','Turquoise')";
                //SQLiteCommand cmd_computer = new SQLiteCommand(sql_comfortzone_computer, m_dbConnection);
                //cmd_computer.ExecuteNonQuery();

                ////--comfortzone datacenter.
                //string id_datacenter = GetGUID();
                //string sql_comfortzone_datacenter = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_datacenter + "', 'Data Center','22','28','35','65','Lightgreen')";
                //SQLiteCommand cmd_datacenter = new SQLiteCommand(sql_comfortzone_datacenter, m_dbConnection);
                //cmd_datacenter.ExecuteNonQuery();

                ////--comfortzone gym.
                //string id_gym = GetGUID();
                //string sql_comfortzone_gym = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_gym + "', 'Gym','22','28','35','65','Lightgray')";
                //SQLiteCommand cmd_gym = new SQLiteCommand(sql_comfortzone_gym, m_dbConnection);
                //cmd_gym.ExecuteNonQuery();

                ////--comfortzone Hospital.
                //string id_Hospital = GetGUID();
                //string sql_comfortzone_Hospital = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Hospital + "', 'Hospital','22','28','35','65','Red')";
                //SQLiteCommand cmd_Hospital = new SQLiteCommand(sql_comfortzone_Hospital, m_dbConnection);
                //cmd_Hospital.ExecuteNonQuery();

                ////--comfortzone Music.
                //string id_Music = GetGUID();
                //string sql_comfortzone_Music = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Music + "', 'Music','22','28','35','65','Orangered')";
                //SQLiteCommand cmd_Music = new SQLiteCommand(sql_comfortzone_Music, m_dbConnection);
                //cmd_Music.ExecuteNonQuery();

                //--For inserting the version value first time while creating database
                string version = AssemblyDateGeneration.Value.ToShortDateString();
                string sql_database_version = "INSERT INTO tbl_Database_Version( version)   VALUES('" + version + "')";
                SQLiteCommand cmd_database_version = new SQLiteCommand(sql_database_version, m_dbConnection);
                cmd_database_version.ExecuteNonQuery();

                //===============END OF THE NEW CODE ADDED IN THIS SECTION==========================//




                //--Lets input some values in the tbl_building_location and in tbl_language_option default 

                //string sql_input1 = "INSERT INTO tbl_building_location (selection,country,state,city,street,BuildingName,EngineeringUnits) VALUES(1, 'china','SangHai','SangHai','No.35,yi yuan garden','Default_Building','SI') ";
                //SQLiteCommand commandINput5 = new SQLiteCommand(sql_input1, m_dbConnection);
                //commandINput5.ExecuteNonQuery();

                //MessageBox.Show("Test We reached this part ln 5207");
                //Adding to language option
                string sql_input2 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(1, 1) ";
                string sql_input3 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(2, 0) ";
                string sql_input4 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(3, 0) ";

                SQLiteCommand c2 = new SQLiteCommand(sql_input2, m_dbConnection);
                c2.ExecuteNonQuery();

                SQLiteCommand c3 = new SQLiteCommand(sql_input3, m_dbConnection);
                c3.ExecuteNonQuery();
                SQLiteCommand c4 = new SQLiteCommand(sql_input4, m_dbConnection);
                c4.ExecuteNonQuery();
                m_dbConnection.Close();//--closing the connection

            }
            catch (Exception ex)
            {


                //string databasePath133 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath133 + @"\db_psychrometric_project.s3db";

                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                ////try
                //{
                if (File.Exists(databaseFile))
                {
                    File.Delete(databaseFile);
                }

                //}
                //catch { }
                Application.Exit();


            }


        }

        public bool checkForDataInSqlite()
        {
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";

            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            bool returnValue = false;
            string id = "";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);              
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                     id = reader["ID"].ToString();
                }
            }
            if (id != "")
            {
                returnValue = true;
            }
            else
            {
                returnValue = false;
            }
            return returnValue;
        }
        public class DataTypeTempBuildingValue
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
        }
        List<DataTypeTempBuildingValue> temp_building_values = new List<DataTypeTempBuildingValue>();
        int index_selected;
        public void  PullLocationInformation()
        {
            try
            {
                //cb1_select_data.Items.Clear();
                //ArrayList stored_location = new ArrayList();
                temp_building_values.Clear();//we need to clear the values for new items


                //--changing all the database to the sqlite database...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    SQLiteCommand comm = new SQLiteCommand("SELECT * from tbl_building_location where selection = 1", connection);
                    //command.Parameters.AddWithValue("@1", userName)
                    reader = comm.ExecuteReader();
                    while (reader.Read())
                    {

                        temp_building_values.Add(new DataTypeTempBuildingValue
                        {
                            ID = int.Parse(reader["id"].ToString()),
                            country = reader["country"].ToString(),
                            state = reader["state"].ToString(),
                            city = reader["city"].ToString()
                        });

                    }

                    //--This is index_selected is later used for inserting lat , long value to db if not filled already
                    index_selected = temp_building_values[0].ID;
                    // MessageBox.Show("stored place = " + s);
                    comm.Dispose();
                    reader.Dispose();
                    connection.Close();
                }

            }
            catch (Exception ex)
            {
                // MessageBox.Show(ex.Message);
            }
        }

        public bool CheckLatLongAvailable()
        {
            //--Lets do some connection checking and validating the data returned...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            bool returnValue = false;
            string latValue = "";
            string lngValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
      

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    latValue = reader["latitude"].ToString();
                    lngValue = reader["longitude"].ToString();
                }
                //  MessageBox.Show("latitude value " + latValue + ",longvalue = " + lngValue);
            }
            if ((latValue == "" || lngValue == "") || (latValue == 0.ToString() && lngValue == 0.ToString()))
            {
                returnValue = true;
                // MessageBox.Show("value is ture = " + returnValue);

            }

            //--This will be either true or false based on the check value..
            return returnValue;
        }

        public void FillLatLongValueAutomatically()
        {

            string country = null, state = null, city = null, street = null;//, zip = null;
            //--This portion fill the lat,long and elevation value is not present in the database by users..
            //--Lets do some connection checking and validating the data returned...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    country = reader["country"].ToString();
                    state = reader["state"].ToString();
                    city = reader["city"].ToString();
                    street = reader["street"].ToString();
                    // zip = reader["zip"].ToString();
                }
            }

            // MessageBox.Show("Country = " + country + ",city " + city);
             WebOperation wb = new WebOperation();
            var listOnlineValue= new List<WebOperation.DataTypeForLatLongElevation>();
            listOnlineValue = wb.PullDataOnline(country, state, city, street);// pull_data_online(country, state, city, street);//--This will fill the online values form the database
                                                           //--After pulling above we get three values we need to push it to database...
                                                           // MessageBox.Show("inside  FillLatLongValueAutomatically lat pulled value=" + latPulledValue + ",long = " + longPulledValue + ",elev = " + elevationPulledValue);

            //--Upadating the table which has no values ...
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE tbl_building_location SET  latitude=@latitude_value,longitude=@longitude_value,elevation=@elevation  where selection=1;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                //command.Parameters.AddWithValue("@latitude_value", latPulledValue.ToString());
                //command.Parameters.AddWithValue("@longitude_value", longPulledValue.ToString());
                // command.Parameters.AddWithValue("@elevation", elevationPulledValue.ToString());

                command.Parameters.AddWithValue("@latitude_value", listOnlineValue[0].latValue.ToString());
                command.Parameters.AddWithValue("@longitude_value", listOnlineValue[0].lngValue.ToString());
                command.Parameters.AddWithValue("@elevation", listOnlineValue[0].elevation.ToString());
                command.ExecuteNonQuery();
            }


        }

        public List<DataTypeOfBuildingInPsychro> buildingList = new List<DataTypeOfBuildingInPsychro>();

        /// <summary>
        /// it helps to pull the information of the building stored
        /// </summary>
        public void get_stored_data_about_building()
        {
            try
            {
                buildingList.Clear();
                //--changing all the database to the sqlite database...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {

                        buildingList.Add(new DataTypeOfBuildingInPsychro
                        {
                            ID = int.Parse(reader["ID"].ToString()),
                            country = reader["country"].ToString(),
                            state = reader["state"].ToString(),
                            city = reader["city"].ToString(),
                            street = reader["street"].ToString(),
                            //ZIP = int.Parse(reader["ZIP"].ToString()),//--This zip code has be removed in latest version
                            longitude = reader["longitude"].ToString(),
                            latitude = reader["latitude"].ToString(),
                            elevation = reader["elevation"].ToString(),
                            BuildingName = reader["BuildingName"].ToString(),
                            EngineeringUnits = reader["EngineeringUnits"].ToString()

                        });

                    }


                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }
        public List<DataTypeOfBuildingInPsychro> selectedBuildingList = new List<DataTypeOfBuildingInPsychro>();
        public void CheckSelectedBuilding()
        {


            selectedBuildingList.Clear();//Reset the values first ...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from tbl_building_location where selection = 1 ";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
               
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //Now lets add to the list 
                    selectedBuildingList.Add(new DataTypeOfBuildingInPsychro
                    {
                        ID = int.Parse(reader["id"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString(),
                        street = reader["street"].ToString(),
                        longitude = reader["longitude"].ToString(),
                        latitude = reader["latitude"].ToString(),
                        elevation = reader["elevation"].ToString(),
                        BuildingName = reader["BuildingName"].ToString()
                    });
                }
            }//close of using..
        }

        public void CreateRequireTableIfNotPresent(string buildingName)
        {
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string tableForNode = "tbl_" + buildingName + "_node_value";
            string tableForLine = "tbl_" + buildingName + "_line_value";
            string tableFordevice = "tbl_" + buildingName + "_device_info_for_node";
            string tableForInputStorageFromT3000 = "tbl_" + buildingName + "_input_storage_from_T3000";
            string tableForValuesFromT3000 = "tbl_" + buildingName + "_node_data_related_T3000";
           // string tableMixNode = "tbl_" + buildingName + "_mix_node_info";


            //These two tables are for comfort zone 
            //string tableforComfortZoneDetail = "tbl_" + buildingName + "_comfort_zone_detail";
            //string tableForChartComfortZoneSetting = "tbl_" + buildingName + "_chart_comfort_zone_setting";

           //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";



            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

               
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                //--Modified to new: This one was intial query
                //string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),source varchar(255),name varchar(255),label varchar(255),colorValue varchar(255),showTextItem varchar(255),nodeSize varchar(255))";
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                //--Modified by bbk: for pulling information from the device
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                //string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id_from_nodeID varchar(255) ,panelID varchar(255),inputIndex varchar(255),inputDescription varchar(255),inputAM varchar(255), inputValue  varchar(255) ,inputUnit varchar(255),inputRange varchar(255),inputCalibration varchar(255),inputFilter varchar(255),inputJumper varchar(255),inputLabel varchar(255))";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();


                //gives the info about the which value From T3000 is associated with nodes in psychometric
                // string sql_For_nodeInfoFromT3000 = "create table IF NOT EXISTS  " +  + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID varchar(255),IP varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                //string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id_from_nodeID varchar(255) ,panelID varchar(255),inputIndex varchar(255),inputDescription varchar(255),inputAM varchar(255), inputValue  varchar(255) ,inputUnit varchar(255),inputRange varchar(255),inputCalibration varchar(255),inputFilter varchar(255),inputJumper varchar(255),inputLabel varchar(255))";
                string sql_For_nodeInfoFromT3000 = "create table IF NOT EXISTS  " + tableForValuesFromT3000 + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,param1_panelID varchar(255),param1_inputIndex varchar(255),param2_panelID varchar(255),param2_inputIndex varchar(255))";
                SQLiteCommand command_T3000 = new SQLiteCommand(sql_For_nodeInfoFromT3000, connection);
                command_T3000.ExecuteNonQuery();


                ////These tables are for comfort zone...
                //string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                //SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                //command6.ExecuteNonQuery();

                ////--Comfortzonetable
                //string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                //SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                //command7.ExecuteNonQuery();


                //-- NOW table for storing values tableForInputStorageFromT3000
                string sql8 = "create table IF NOT EXISTS  " + tableForInputStorageFromT3000 + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


                //tableMixNode
                //string sql9 = "create table IF NOT EXISTS  " + tableMixNode + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                //SQLiteCommand command9 = new SQLiteCommand(sql9, connection);
                //command9.ExecuteNonQuery();



            }

        }

        public class chartDetailDT
        {
            public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
            public string enableChartStatus { get; set; }
        }
        public List<chartDetailDT> chartDetailList = new List<chartDetailDT>();//This is used for storing the chart detail ids

        public void PullChartList(string buildingName)
        {

            chartDetailList.Clear();//resetting the chart list value..
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "select * from " + tableForChartDetail + " where enableChartStatus ='true' ";

                var command = new SQLiteCommand(sql, connection);

                var reader = command.ExecuteReader();
               // MessageBox.Show("Reader field Count = " + reader.FieldCount+"\nsql="+sql+"\nconstring= "+connString);
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    chartDetailList.Add(new chartDetailDT
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chartID = reader["chartID"].ToString(),
                        chartName = reader["chartName"].ToString(),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                        enableChartStatus = reader["enableChartStatus"].ToString() //either true or false

                    });
                }
            }

        }  //Close of the function  PullChartList

        public string GetGUID()
        {
            return Guid.NewGuid().ToString("N");
        }

        public void insertNewDataFromDGV(string name)
        {
            string buildingNameValue = selectedBuildingList[0].BuildingName;//listBuildingSelectedInfoInPsycho
            //string buildingNameValue = listBuildingSelectedInfoInPsycho[0].BuildingName;
            string tableNameOfChart = "tbl_" + buildingNameValue + "_chart_detail";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string chartIDValue = buildingNameValue + name + GetGUID();//This is the chart id value
            string chart_resp_nodes_group_ID = chartIDValue + "_node_group";
            string chart_resp_lines_group_ID = chartIDValue + "_line_group";
            string enableChartStatusVar = "true";   //--Firs first time insertion its always true
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "insert into  " + tableNameOfChart + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@chartID_value,@nodeName_value,@nodeRespectiveID,@lineRespectiveID,@enableChartStatus1)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartID_value", chartIDValue);
                command.Parameters.AddWithValue("@nodeName_value", name);
                command.Parameters.AddWithValue("@nodeRespectiveID", chart_resp_nodes_group_ID);
                command.Parameters.AddWithValue("@lineRespectiveID", chart_resp_lines_group_ID);
                command.Parameters.AddWithValue("@enableChartStatus1", enableChartStatusVar);

                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();


            }
        }

        public void UpdateNewDataFromDGV(string name, string chartID)
        {
            if (name == "")
            {
                //If name is empty the do not update
                return;
            }
            string buildingNameValue = selectedBuildingList[0].BuildingName;
            string tableNameOfChart = "tbl_" + buildingNameValue + "_chart_detail";

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "update " + tableNameOfChart + "   set  chartName=@chartName_value  where chartID = @chartID_value;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartName_value", name);
                command.Parameters.AddWithValue("@chartID_value", chartID);
                command.ExecuteNonQuery();
            }
        }
        public void LoadNodeAndLineFromDB(int indexValue)
        {
            //Based on this row index we need to update the values and redraw lines..

            try
            {
                // listForDataFromDB.Clear();//Lets clear the node...
                if (indexValue < 0)
                {
                    return;
                }
                if (chartDetailList.Count <= 0)
                {
                    return;
                }

                if (selectedBuildingList.Count <= 0)
                {
                    return;
                }
                //Lets identify the node
                // int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
                //int id = e.RowIndex;//This index is used to identify which cell or chart is clicked.
                int id = indexValue;//This index is used to identify which cell or chart is clicked.
                                    /*
                                    Now lets find the chart line id and chart node id 
                                    */
                string chartNodeGroupID = chartDetailList[id].chart_respective_nodeID;//This is for the node
                string chartLineGroupID = chartDetailList[id].chart_respective_lineID;//This is for the line

                string selectedChartID = chartDetailList[id].chartID;
               // var nc = new NodeAndLineClass();
                //--Reset the context menu stip first..
                listNodeInfoValues.Clear();
                //--Reset the context menu stip first..
                listLineInfoValues.Clear();
                //--Now lets clear the mix node list first and then refill agiain
                //mixNodeInfoList.Clear();


                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";

                /*
                Table name for line and node values...
                ie: 
                "tbl_"+ buildingname+ "_node_value"
                "tbl_"+ buildingname+ "_line_value"
                */
                string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
               // string tableMixNode = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();

                    SQLiteDataReader reader = null;
                    string queryString = "SELECT *  from " + nodeTableName + " WHERE chart_respective_nodeID = @chartnodeID";

                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    command.Parameters.AddWithValue("@chartnodeID", chartNodeGroupID);//This is the group id that is used to identify each node
                    
                    int count = 0;
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        //editied for reading from alex db
                        listNodeInfoValues.Add(new DataTypeForNode
                        {
                            ID = reader["nodeID"].ToString(), //This is just changed code : bbk305
                            xVal = double.Parse(reader["xValue"].ToString()),
                            yVal = double.Parse(reader["yValue"].ToString()),
                            temperature_source = reader["temperature_source"].ToString(),
                            pressure_source = reader["humidity_source"].ToString(),
                            name = reader["name"].ToString(),

                            // label = reader["label"].ToString(),
                            colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                            // showItemText = reader["showTextItem"].ToString(),
                            marker_Size = int.Parse(reader["nodeSize"].ToString()),
                            //airFlow = int.Parse(reader["airFlow"].ToString()),
                            lastUpdatedDate = reader["lastUpdatedDate"].ToString()
                           


                        });

                    }

                    //--Resetting the index value...

                    if (listNodeInfoValues.Count > 0)
                    {
                        count = listNodeInfoValues.Count;//--This is used for udpdating the index values..
                    }
                    else
                    {
                        count = 0;
                    }
                    //--Resetting the actual index value

                    //--HereError
                    //var bo = new BuildingOperation();
                    index = count; //Index is set to the count values of the node

                    //--Now we can find the previous id values as follows:
                    //=====================================Finding previous id value=================================//

                     previousNodeIndexForLineInput = IndexOfPreviousNodeForLineFunction();
                    //==================================End of previous id=======================================//

                    //--Adding data form the line node values...
                    SQLiteDataReader reader2x = null;
                    string queryString2x = "SELECT *  from  " + lineTableName + " WHERE chart_respective_lineID = @lineID";
                    //--Testing..
                    //   MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

                    SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);
                    command2x.Parameters.AddWithValue("@lineID", chartLineGroupID);//This is the group id that is used to identify each node

                    //int count2 = 0;
                    reader2x = command2x.ExecuteReader();
                    while (reader2x.Read())
                    {
                        listLineInfoValues.Add(new DataTypeForLine
                        {
                            ID = reader2x["lineID"].ToString(),//This is just change code :bbk305
                            prevNodeId = reader2x["prevNodeId"].ToString(),
                            nextNodeId = reader2x["nextNodeId"].ToString(),
                            lineColorValue = ColorTranslator.FromHtml(reader2x["lineColorValue"].ToString()),
                            lineSeriesID = new System.Windows.Forms.DataVisualization.Charting.Series(reader2x["lineSeriesId"].ToString()),
                            lineThickness = int.Parse(reader2x["thickness"].ToString()),
                            name = reader2x["name"].ToString(),
                            status = int.Parse(reader2x["status"].ToString())
                        });

                    }
                
                   
                }//close of using..
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }


    }
}
