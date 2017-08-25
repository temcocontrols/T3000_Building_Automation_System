/*
 * Project : PH application
 * Author Name : Bhoj bahadur karki
 * Date : 2017-July-4th 
 * Contact : nishantkarki2013@hotmail.com
 */
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Data.SQLite;
using System.Configuration;
using System.Data;
using System.Windows.Forms;
using System.Drawing;
using System.Windows.Forms.DataVisualization.Charting;
using System.Globalization;

namespace PH_App
{
    /// <summary>
    /// Does all the database operations here 
    /// </summary>
    public class DatabaseOperationModel:PHChartGlobalVariables //:NodeAndLineClass
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
        public List<SelectedBuildingDatatype> buildingSelectedInT3000 = new List<SelectedBuildingDatatype>();
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

        public class fluidProperty
        {
            public int selected { get; set; }
            public string fluidName { get; set; }
            public double Xmin { get; set; }
            public double Xmax { get; set; }
            public double Ymin { get; set; }
            public double Ymax { get; set; }
        }
        public List<fluidProperty> fluidInfo = new List<fluidProperty>();

        public void PullDataOfFluidInfo(string buildingName)
        {
            fluidInfo.Clear();
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection($"Data Source={databaseFile};Version=3;"))
            {
                m_dbConnection.Open();
                //string slqFluid = "Select * from tbl_" + buildingName + "_FluidInfo where selected='1'";
                string slqFluid = "Select * from tbl_" + buildingName + "_FluidInfo ";

                //  bool fluidPresent = false;
                SQLiteCommand cmdFluid = new SQLiteCommand(slqFluid, m_dbConnection);

                SQLiteDataReader readerFluid = cmdFluid.ExecuteReader();
                while (readerFluid.Read())
                {
                    //fluidPresent = true;
                    fluidInfo.Add(new fluidProperty
                    {
                        selected = int.Parse(readerFluid["selected"].ToString()),
                        fluidName = readerFluid["fluidName"].ToString(),
                        Xmin = double.Parse(readerFluid["Xmin"].ToString()),
                        Xmax = double.Parse(readerFluid["Xmax"].ToString()),
                        Ymin = double.Parse(readerFluid["Ymin"].ToString()),
                        Ymax = double.Parse(readerFluid["Ymax"].ToString()),
                    });
                }

            }

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
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),pressure_source varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";


                string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperaturePressureSourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),PressureSourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                string tbl_FluidInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_FluidInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,selected INTEGER, fluidName varchar(255) ,Xmin VARCHAR(255),Xmax varchar(255),Ymin VARCHAR(255),Ymax varchar(255))";

                //MessageBox.Show("Creating tables from update section");

                ////--tbl_[]_chart_fluid_info
                string sqlChartFluidInfo = "create table IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_fluid_info(count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255),fluidName varchar(255))";
                SQLiteCommand cmdChartFluidInfo = new SQLiteCommand(sqlChartFluidInfo, m_dbConnection);
                cmdChartFluidInfo.ExecuteNonQuery();

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

                SQLiteCommand cm1001 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                cm1001.ExecuteNonQuery();

               
                SQLiteCommand cm1401 = new SQLiteCommand(tbl_database_version, m_dbConnection);
                cm1401.ExecuteNonQuery();

                //For fluidInfo
                SQLiteCommand cmdFluidInfo = new SQLiteCommand(tbl_FluidInfo, m_dbConnection);
                cmdFluidInfo.ExecuteNonQuery();


                string slqFluid = "Select * from tbl_" + buildingNameSelected + "_FluidInfo";

                bool fluidPresent = false;
                SQLiteCommand cmdFluid = new SQLiteCommand(slqFluid, m_dbConnection);

                SQLiteDataReader readerFluid = cmdFluid.ExecuteReader();
                while (readerFluid.Read())
                {
                    fluidPresent = true;

                }

                if (fluidPresent != true)
                {

                    /*
                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','2000','0.03','100')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','4000','0.5','100')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','2800','0.05','100')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','1400','0.5','100')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','2400','0.5','100')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();
                    */


                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','1500','0.03','30')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','2500','0.5','50')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','1000','0.05','10')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','600','0.5','10')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','800','0.5','10')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();

                }


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
                //string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),pressure_source varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";//,lastUpdatedDate varchar(255))";

                string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperaturePressureSourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),PressureSourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                //now execute the query
                //SQLiteCommand cm1 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                //cm1.ExecuteNonQuery();
                string tbl_FluidInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_FluidInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,selected INTEGER, fluidName varchar(255) ,Xmin VARCHAR(255),Xmax varchar(255),Ymin VARCHAR(255),Ymax varchar(255))";


                ////--tbl_[]_chart_fluid_info
                string sqlChartFluidInfo = "create table IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_fluid_info(count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255),fluidName varchar(255))";
                SQLiteCommand cmdChartFluidInfo = new SQLiteCommand(sqlChartFluidInfo, m_dbConnection);
                cmdChartFluidInfo.ExecuteNonQuery();


                //For fluidInfo
                SQLiteCommand cmdFluidInfo = new SQLiteCommand(tbl_FluidInfo, m_dbConnection);
                cmdFluidInfo.ExecuteNonQuery();

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

                SQLiteCommand cm10 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                cm10.ExecuteNonQuery();

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

                //--First check the data then 
                string slqFluid = "Select * from tbl_"+ buildingNameSelected + "_FluidInfo";

                bool fluidPresent = false;
                SQLiteCommand cmdFluid = new SQLiteCommand(slqFluid, m_dbConnection);

                SQLiteDataReader readerFluid = cmdFluid.ExecuteReader();
                while (readerFluid.Read())
                {
                    fluidPresent = true;

                }

                if (fluidPresent != true)
                {

                    /*
                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','2000','0.03','100')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','4000','0.5','100')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','2800','0.05','100')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','1400','0.5','100')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','2400','0.5','100')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();

                    */

                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','1500','0.03','30')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','2500','0.5','50')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','1000','0.05','10')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','600','0.5','10')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','800','0.5','10')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();
                }


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
                // string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),pressure_source varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";//,lastUpdatedDate varchar(255))";

                string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperaturePressureSourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),PressureSourceInfo varchar(255))";
                //string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                //string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                //string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
                //string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";
                string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";

                //MessageBox.Show("Creating tables from update section");

                //now execute the query
                //SQLiteCommand cm101 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                //cm101.ExecuteNonQuery();

                string tbl_FluidInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_FluidInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,selected INTEGER, fluidName varchar(255) ,Xmin VARCHAR(255),Xmax varchar(255),Ymin VARCHAR(255),Ymax varchar(255))";

                ////--tbl_[]_chart_fluid_info
                string sqlChartFluidInfo = "create table IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_fluid_info(count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255),fluidName varchar(255))";
                SQLiteCommand cmdChartFluidInfo = new SQLiteCommand(sqlChartFluidInfo, m_dbConnection);
                cmdChartFluidInfo.ExecuteNonQuery();


                //For fluidInfo
                SQLiteCommand cmdFluidInfo = new SQLiteCommand(tbl_FluidInfo, m_dbConnection);
                cmdFluidInfo.ExecuteNonQuery();

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

                SQLiteCommand cm1001 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                cm1001.ExecuteNonQuery();

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
                string slqFluid = "Select * from tbl_" + buildingNameSelected + "_FluidInfo";

                bool fluidPresent = false;
                SQLiteCommand cmdFluid = new SQLiteCommand(slqFluid, m_dbConnection);

                SQLiteDataReader readerFluid = cmdFluid.ExecuteReader();
                while (readerFluid.Read())
                {
                    fluidPresent = true;
                }

                if (fluidPresent != true)
                {
                    /*
                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','2000','0.03','100')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','4000','0.5','100')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','2800','0.05','100')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','1400','0.5','100')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','2400','0.5','100')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();
                    */


                    //water
                    string sqlWater = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '1','Water','0.001','4000','0.001','100')";
                    SQLiteCommand cmdWater = new SQLiteCommand(sqlWater, m_dbConnection);
                    cmdWater.ExecuteNonQuery();
                    //acetone
                    string sqlAcetone = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Acetone','0.001','1500','0.03','30')";
                    SQLiteCommand cmdAcetone = new SQLiteCommand(sqlAcetone, m_dbConnection);
                    cmdAcetone.ExecuteNonQuery();
                    //ammonia
                    string sqlAmmonia = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','Ammonia','0.5','2500','0.5','50')";
                    SQLiteCommand cmdAmmonia = new SQLiteCommand(sqlAmmonia, m_dbConnection);
                    cmdAmmonia.ExecuteNonQuery();

                    //n-Propane
                    string sqlPropane = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','n-Propane','0.001','1000','0.05','10')";
                    SQLiteCommand cmdPropane = new SQLiteCommand(sqlPropane, m_dbConnection);
                    cmdPropane.ExecuteNonQuery();

                    //R134a
                    string sqlR134a = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','R134a','0.5','600','0.5','10')";
                    SQLiteCommand cmdR134a = new SQLiteCommand(sqlR134a, m_dbConnection);
                    cmdR134a.ExecuteNonQuery();

                    //1-Butene
                    string sqlButene = "INSERT INTO tbl_" + buildingNameSelected + "_FluidInfo(selected,fluidName,Xmin,Xmax,Ymin,Ymax )   VALUES( '0','1-Butene','0.5','800','0.5','10')";
                    SQLiteCommand cmdButene = new SQLiteCommand(sqlButene, m_dbConnection);
                    cmdButene.ExecuteNonQuery();
                }




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
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),pressure_source varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";//,lastUpdatedDate varchar(255))";
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
          //  public string fluidName { get; set; }
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
                        enableChartStatus = reader["enableChartStatus"].ToString() 
                       //fluidName = reader["fluidName"].ToString()
                    });
                }
            }
        }  //Close of the function  PullChartList

        public class fluidChartType
        {
            public int count { get; set; }
            public string chartID { get; set; }
            public string fluidName { get; set; }
            
        }
        public List<fluidChartType> fluidForChartsList = new List<fluidChartType>();//This is used for storing the chart detail ids

        public void PullFluidForChartList(string buildingName)
        {

            fluidForChartsList.Clear();//resetting the chart list value..
            string tableName = "tbl_" + buildingName + "_chart_fluid_info";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql = "select * from  " + tableName ;
                var command = new SQLiteCommand(sql, connection);
                var reader = command.ExecuteReader();
                while (reader.Read())
                {
                    fluidForChartsList.Add(new fluidChartType
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chartID = reader["chartID"].ToString(),
                        fluidName = reader["fluidName"].ToString()                        
                    });
                }
            }
        }

        public string GetGUID()
        {
            return Guid.NewGuid().ToString("N");
        }

        public void insertNewDataFromDGV(string name)
        {
            string buildingNameValue = selectedBuildingList[0].BuildingName;//listBuildingSelectedInfoInPsycho
            //string buildingNameValue = listBuildingSelectedInfoInPsycho[0].BuildingName;
            string tableNameOfChart = "tbl_" + buildingNameValue + "_chart_detail";
            string chartFluidTable = "tbl_" + buildingNameValue + "_chart_fluid_info";

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string chartIDValue =  name + GetGUID();//This is the chart id value
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
               // command.Parameters.AddWithValue("@fName","Water");

                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();


                //--Inserting fluidName Water in tbl_[]_chart_fluid_info

                string sqlQuery = "insert into  " + chartFluidTable + " (chartID,fluidName) VALUES(@chartID,@fluidName)";
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, connection);
                cmd.CommandType = CommandType.Text;
                cmd.Parameters.AddWithValue("@chartID", chartIDValue);
                cmd.Parameters.AddWithValue("@fluidName", "Water");
                cmd.ExecuteNonQuery();


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
        //public class TemperatureHumiditySourceInfo
        //{
        //    public string chartID { get; set; }
        //    public string nodeID { get; set; }
        //    public string TemepratureSoureString { get; set; }
        //    public string HumiditySourceString { get; set; }
        //}
        public List<TemperaturePressureSourceInfo> listTemperatureAndHumidtySourceInfoWhenDeviceSelected = new List<TemperaturePressureSourceInfo>();

        public void LoadNodeAndLineFromDB(int indexValue)
        {
            //Based on this row index we need to update the values and redraw lines..
            //try
            //{
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
                //for device and humidity info
                listTemperatureAndHumidtySourceInfoWhenDeviceSelected.Clear();

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
                //--Table for input instring value for device
                string tableInputStringOfNodeForDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";

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
                            pressure_source = reader["pressure_source"].ToString(),
                            name = reader["name"].ToString(),

                            // label = reader["label"].ToString(),
                            colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                            // showItemText = reader["showTextItem"].ToString(),
                            marker_Size = int.Parse(reader["nodeSize"].ToString()),
                            //airFlow = int.Parse(reader["airFlow"].ToString()),
                            //lastUpdatedDate = reader["lastUpdatedDate"].ToString()
                            lastUpdatedXValue = reader["lastUpdatedXValue"].ToString(),
                            lastUpdatedYValue = reader["lastUpdatedYValue"].ToString()


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
                    // index = count; //Index is set to the count values of the node

                    //--Now we can find the previous id values as follows:
                    //=====================================Finding previous id value=================================//
                    //var chartOptrn = new ChartCreationAndOperations();
                    var main = new MainController();
                     previousNodeIndexForLineInput = main.IndexOfPreviousNodeForLineFunction();
                    //==================================End of previous id=======================================//

                    //--Adding data form the line node values...
                    
                    string queryString2x = "SELECT *  from  " + lineTableName + " WHERE chart_respective_lineID = @lineID";
                    //--Testing..
                    //MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

                    SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);
                    command2x.Parameters.AddWithValue("@lineID", chartLineGroupID);//This is the group id that is used to identify each node

                //int count2 = 0;
                  SQLiteDataReader reader2x  = command2x.ExecuteReader();
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

                    
                    //--For temperature and humidity source info
                    string querySQLString = "select * from   " + tableInputStringOfNodeForDevice;
                    var command12 = new SQLiteCommand(querySQLString, connection);
                    var reader1 = command12.ExecuteReader();
                    while (reader1.Read())
                    {
                        listTemperatureAndHumidtySourceInfoWhenDeviceSelected.Add(new TemperaturePressureSourceInfo
                        {
                            chartID = reader1["chartID"].ToString(),
                            nodeID = reader1["NodeID"].ToString(),
                            TemepratureSoureString = reader1["TemperatureSourceInfo"].ToString(),
                            PressureSourceString = reader1["PressureSourceInfo"].ToString(),

                        });

                    }

                   // */

                }//close of using..
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

        }

        public void InsertNodeInfoToDBWithoutDeviceInfo(string buildingName, string chart_respective_nodeID, string id, double xVal, double yVal, string temperature_source, string pressure_source, string name, Color colorValue, int nodeSizeValue)
        {
            //This is the name of the table that stores the node information...
            if (buildingName == "")
            {
                return;
            }

            //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  
            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  
                                                                     //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                                     //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //string sql_string = "insert into " + tableName + "(chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedDate) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@pressure_source,@name,@colorVal,@node_size_value,@lastUpdateDate)";
                string sql_string = "insert into " + tableName + "(chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedXValue,lastUpdatedYValue) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@pressure_source,@name,@colorVal,@node_size_value,@lastUpdateDateX,@lastUpdateDateY)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chart_respective_nodeID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@temperature_source", temperature_source);
                command.Parameters.AddWithValue("@pressure_source", pressure_source);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                //command.Parameters.AddWithValue("@airFlow", airFlow);
                //command.Parameters.AddWithValue("@lastUpdateDateX", DateTime.Now.ToString("G",DateTimeFormatInfo.InvariantInfo));
                //command.Parameters.AddWithValue("@lastUpdateDateY", DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
                command.Parameters.AddWithValue("@lastUpdateDateX", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));//Lets store the date time in the node values
                command.Parameters.AddWithValue("@lastUpdateDateY", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn

        public void InsertLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, string lineSeriesVal, int linethickness,string chart_respective_lineID, string lineName, int lineStatus_ON_OFF)
        {
            //--Note for series the series.name property will be stored in databse and later it will be converted.
            string lineSeriesName = lineSeriesVal;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";   // currentLineTableFromDB;
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "insert into " + tableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@nameVal,@statusVal)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chart_respective_lineID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@thicknessValue", linethickness);
                command.Parameters.AddWithValue("@nameVal", lineName);
                command.Parameters.AddWithValue("@statusVal", lineStatus_ON_OFF.ToString());
                command.ExecuteNonQuery();
            }


        }

        public void UpdateLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, Series lineSeriesVal, int lineThicknessValue, string line_name, int line_status)
        {
            string lineSeriesName = lineSeriesVal.Name;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set prevNodeId=@pn,nextNodeId=@nn,lineColorValue=@lc,lineSeriesId=@ls ,thickness = @linethickness ,name = @lnName,status = @lnStatus where   lineID=@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@linethickness", lineThicknessValue);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@lnName", line_name);
                command.Parameters.AddWithValue("@lnStatus", line_status);
                command.ExecuteNonQuery();
            }
        }

        public void UpdateNodeInfoToDBWithNewParameters(string buildingName, string id, double xVal, double yVal, string temperature_source, string pressure_source, string name, Color colorValue, int nodeSizeValue)
        {   //CurrentSelectedBuilding
            //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  
            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@temp_source, pressure_source=@pressure_source,name=@name, colorValue=@colorVal, nodeSize=@node_size_value where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@temp_source", temperature_source);
                command.Parameters.AddWithValue("@pressure_source", pressure_source);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                //command.Parameters.AddWithValue("@airflowValue", airflow);
                //command.Parameters.AddWithValue("@lastUpdatedDate", lastUpdatedDateValue);
                command.Parameters.AddWithValue("@id", id);
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn

        public List<DataTypeForNode> nodeInfoContainingDevice = new List<DataTypeForNode>();
        /// <summary>
        /// This helps to delete node,line and device info for node,form db to clear the chart,does not delete chart only content 
        /// </summary>
        public void DeleteChartContent(string chartID, string chart_resp_nodeID, string chart_resp_lineID)
        {
            /*
            Steps : 
            1.Read the node and line respective chart id info
            2. using node respective chart id read the node having source="Device" [This is used for deleting device info of each selected node]
            3. Delete the line info first using chart_respective_lineID form tbl_..._line_value
            4.Delete the device info form stored source   form tbl_..._device_info_for_node
            5.Delete the node info form tbl_..._node_value
            6.Delete the chart info now from tbl_..._chart_detail
            */

            // int selectedItemIndex = int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

            //we need to find the corresponding tables for deletion.

            //int id = selectedItemIndex;
            // string tableName = "tbl_"+
            nodeInfoContainingDevice.Clear();//Resetting list
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableComfortZone = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";//currentNodeTableFromDB; 
           // string tableTempHumSourceInfo = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";
            //lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            string sql1 = "SELECT * From  " + nodeTableName + "  where chart_respective_nodeID ='" + chart_resp_nodeID + "'";
            //  MessageBox.Show("sql1=" + sql1);

            //(nodeID,device_instanceID,IP,param1ID_X,param2ID_Y) VALUES(@id,@instanceID,@IP,@param1,@param2)";

            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);

                thisConnection.Open();

                SQLiteDataReader reader = cmd.ExecuteReader();

                // MessageBox.Show("cmd error= " + cmd.CommandText);

                while (reader.Read())
                {
                    if ((reader["temperature_source"].ToString() == "Device") || (reader["pressure_source"].ToString() == "Device"))
                    {
                        nodeInfoContainingDevice.Add(new DataTypeForNode
                        {
                            ID = reader["nodeID"].ToString(), //This is just changed code : bbk305
                            xVal = double.Parse(reader["xValue"].ToString()),
                            yVal = double.Parse(reader["yValue"].ToString()),
                            // temperature_source = reader["temperature_source"].ToString(),
                            // humidity_source = reader["humidity_source"].ToString(),


                            name = reader["name"].ToString(),
                            // label = reader["label"].ToString(),
                            colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                            //showItemText = reader["showTextItem"].ToString(),
                            marker_Size = int.Parse(reader["nodeSize"].ToString()),
                            temperature_source = reader["temperature_source"].ToString(),
                            pressure_source = reader["pressure_source"].ToString(),
                            // airFlow = double.Parse(reader["airFlow"].ToString()),
                            //lastUpdatedDate = reader["lastUpdatedDate"].ToString()
                            lastUpdatedXValue = reader["lastUpdatedDateXValue"].ToString(),
                            lastUpdatedYValue = reader["lastUpdatedDateYValue"].ToString()


                        });
                    }  //close of if

                }

            }//close of using...

            //Deleting the node info from device  ie all the device information of node

            if (nodeInfoContainingDevice.Count > 0)
            {

                for (int i = 0; i < nodeInfoContainingDevice.Count; i++)
                {
                    using (SQLiteConnection connection = new SQLiteConnection(connString1))
                    {
                        connection.Open();
                        string queryString = "delete from " + tableNameDevice + " where nodeID = @id_value";
                        SQLiteCommand command = new SQLiteCommand(queryString, connection);
                        command.Parameters.AddWithValue("@id_value", nodeInfoContainingDevice[i].ID);
                        command.ExecuteNonQuery();

                    }//Close of using

                } //close of for

            }//close of if

            //-----Now deleting the line

            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + lineTableName + " where chart_respective_lineID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chart_resp_lineID);
                command.ExecuteNonQuery();

            }//Close of using


            //===Deleting comfort zone setting for clear chart
            //using (SQLiteConnection connection = new SQLiteConnection(connString1))
            //{
            //    connection.Open();
            //    string queryString = "delete from " + tableComfortZone + " where chartID = @id_value";
            //    SQLiteCommand command = new SQLiteCommand(queryString, connection);
            //    command.Parameters.AddWithValue("@id_value", chartID);
            //    command.ExecuteNonQuery();

            //}//Close of using



            //--now deleting the node info from the main node info
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + nodeTableName + " where chart_respective_nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chart_resp_nodeID);
                command.ExecuteNonQuery();

            }//Close of using


        }


        public void DeleteSingleNodeAndAssociatedLinesInDB(string nodeID)
        {
            DeleteNodeUsingNodeID(nodeID);
            DeleteLineUsingNodeID(nodeID);


        }
        public void DeleteNodeUsingNodeID(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();


            }//Close of using 


        }

        public void DeleteLineUsingNodeID(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";


            //==Deleting if previous nodeid is matched
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where prevNodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();


            }//Close of using 


            //Deleting if nextNodeISmatched

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where nextNodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();

            }//Close of using 


        }

        public void SQLiteSaveConfigurationSetting(Form_Main_PH_Application f1)
        {


            //--Path and table names
            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            string tableForFluidInfo = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_fluid_info";

            //lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile_x = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile_x = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            //Reading the data form SQLite DB
            ReadDataForSavingConfiguration(databaseFile_x, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForFluidInfo);//This reads all the data

            string fileName = "";
            f1.saveFD.InitialDirectory = "C:";
            f1.saveFD.FileName = "SavePsySetting";
            f1.saveFD.Filter = "s3db(.s3db) |*.s3db";
            //ImageFormat format = ImageFormat.Png;
            if (f1.saveFD.ShowDialog() == DialogResult.OK)
            {
                //  Cursor.Current = Cursors.WaitCursor;
                fileName = f1.saveFD.FileName;

                // Cursor.current = Cursors.WaitCursor;

                //saving the file in that path
                string databaseFile = fileName;//path + fileName; //databasePath1 + @"\db_psychrometric_project.s3db";

                //--Creating file and table details 

                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                //SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                //m_dbConnection.Open();

                CreateTableForSavingConfiguration(databaseFile);

                //After the table has been created we can write to the database 
                //--InsertDataForSavingtoSQLiteDB(string pathWithFileName);

                //--Reading data will be done before this so 

                //---Writing to this db ==>insertion


                string chartTableNamex = "tbl_chart_detail";
                string nodeTableNamex = "tbl_node_value";
                string lineTableNamex = "tbl_line_value";
                string tableNameDevicex = "tbl_device_info_for_node";//currentNodeTableFromDB; 
                //string tableForComfortZoneSettingx = "tbl_chart_comfort_zone_setting";
                //string tableForCF_Detailx = "tbl_comfort_zone_detail";
                string tableForfluidinfo = "tbl_chart_fluid_info";


                InsertForSavingConfiguration(databaseFile, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForfluidinfo);

                // SQLiteConnection.ClearPool();//This helps in 
            }

        }

        
        public class nodeDataTypeForSaving
        {
            //  public int count { get; set; }
            public string chart_respective_nodeID { get; set; } //This is the chartID for grouping the node for single node values.
            public string nodeID { get; set; }  //This is the individual node id for each node 
            public double xValue { get; set; }
            public double yValue { get; set; }
            // public string source { get; set; }
            public string temperature_source { get; set; }
            public string pressure_source { get; set; }
            public string name { get; set; }
            // public string label { get; set; }
            public string colorValue { get; set; }
            //public string showTextItem { get; set; }
            public int nodeSize { get; set; }
           // public int airFlow { get; set; }
            //public string lastUpdatedDate { get; set; }
            public string lastUpdatedXValue { get; set; }
            public string lastUpdatedYValue { get; set; }
        }
        
        public class chartDetailDT_X
        {
            // public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
            public string enableChartStatus { get; set; }
        }
        
        public class lineDataType_X
        {
            //  public int count { get; set; }
            public string chart_respective_lineID { get; set; }
            public string lineID { get; set; }
            public string prevNodeID { get; set; }
            public string nextNodeID { get; set; }
            public string lineColorValue { get; set; }
            public string lineSeriesID { get; set; }
            public string thickness { get; set; }
            public string name { get; set; }
            public string status { get; set; }
        }
        public class dt_for_device_info   //datatype for device info
        {

            public string nodeID { get; set; } //--for identifying which point is selected..
            public string device_instance_id_for_param1 { get; set; }//--this is the values that represent the point in a chart
            public string ip_for_param1 { get; set; }
            public string device_instance_id_for_param2 { get; set; }//--this is the values that represent the point in a chart
            public string ip_for_param2 { get; set; }
            public string param1id { get; set; }
            public string param2id { get; set; }
            public string param1info { get; set; }
            public string param2info { get; set; }
            public string param1_id_type { get; set; }
            public string param2_id_type { get; set; }

        }
        //Lets have a list for chartDetailList
        public List<chartDetailDT_X> chartInfoPulledForSaving = new List<chartDetailDT_X>();//This is used for storing the chart detail ids

        //Nowlets have the node information ==>> all node information
        public List<nodeDataTypeForSaving> nodeInfoPulledForSaving = new List<nodeDataTypeForSaving>();

        //NOw list for line info
        public List<lineDataType_X> lineInfoPulledForSaving = new List<lineDataType_X>();

        //Now  list for pulling the device info
        public List<dt_for_device_info> deviceInfoPulledForSaving = new List<dt_for_device_info>();

        //For fluid info 
        public List<fluidChartType> listfluidInfoForChart = new List<fluidChartType>();

        //Now for storing the comfortzone setting...
        // public List<datatype_for_comfortzone> comfortZoneInforForEachChartForSaving = new List<datatype_for_comfortzone>();//which chart is associated with which comfortzone

        //Now for storing the actual confort zone values
        //  public List<dataTypeForCF> ComfortZonesDetailForSaving = new List<dataTypeForCF>();  //This one contains the comfortzone setting

        //public class dataTypeFor_mix_node_info
        //{
        //    public string ChartID { get; set; }

        //    public string nodeID { get; set; }
        //    //public string min_temp { get; set; }
        //    // public string max_temp { get; set; }
        //    public string previousNodeID { get; set; }
        //    public string nextNodeID { get; set; }
        //    //public Color colorValue { get; set; }
        //}

        // public List<dataTypeFor_mix_node_info> mixNodeInfoListForSaveConfiguration = new List<dataTypeFor_mix_node_info>();  //This one contains the comfortzone setting

        /// <summary>
        /// Read data form db to save as SQLite file
        /// </summary>

        public void ReadDataForSavingConfiguration(string path, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice,string tableForFluidInfo)
        {

            //--Resetting values first
            chartInfoPulledForSaving.Clear();
            nodeInfoPulledForSaving.Clear();
            lineInfoPulledForSaving.Clear();
            deviceInfoPulledForSaving.Clear();
            listfluidInfoForChart.Clear();
            //comfortZoneInforForEachChartForSaving.Clear();
            //ComfortZonesDetailForSaving.Clear();
            //mixNodeInfoListForSaveConfiguration.Clear();

            ////--This reads the value of data form db lets use list for storing the data
            //string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            //string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            //string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            //string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";

            ////lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connectionString = @"Data Source=" + path + ";Version=3;";


            string sql_for_chart_detail = "SELECT * From  " + chartTableName;
            string sql_for_node = "SELECT * From  " + nodeTableName;
            string sql_for_line = "SELECT * From  " + lineTableName;
            string sql_forDevice = "SELECT * From  " + tableNameDevice;
            //string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            //string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
           string sqlforfluidinfo = "SELECT * From  " + tableForFluidInfo;
            //;



            //  MessageBox.Show("sql1=" + sql1);         
            using (SQLiteConnection conx = new SQLiteConnection(connectionString))
            {

                conx.Close();
                conx.Open();

                SQLiteCommand cmd = new SQLiteCommand(sql_for_chart_detail, conx);
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {

                    if (reader["chartID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        chartInfoPulledForSaving.Add(new chartDetailDT_X
                        {

                            //  count = int.Parse(reader["count"].ToString()),
                            chartID = reader["chartID"].ToString(),
                            chartName = reader["chartName"].ToString(),
                            chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                            chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                            enableChartStatus = reader["enableChartStatus"].ToString()

                        });
                    }
                }

                //----Reading the node information 

                SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conx);
                SQLiteDataReader reader1 = cmd1.ExecuteReader();
                while (reader1.Read())
                {

                    if (reader1["chart_respective_nodeID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        nodeInfoPulledForSaving.Add(new nodeDataTypeForSaving
                        {

                            //count = int.Parse(reader1["count"].ToString()),
                            chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                            nodeID = reader1["nodeID"].ToString(),
                            xValue = double.Parse(reader1["xValue"].ToString()),
                            yValue = double.Parse(reader1["yValue"].ToString()),
                            temperature_source = reader1["temperature_source"].ToString(),
                            pressure_source = reader1["pressure_source"].ToString(),
                            name = reader1["name"].ToString(),
                            // label = reader1["label"].ToString(),
                            colorValue = reader1["colorValue"].ToString(),
                            // showTextItem = reader1["showTextItem"].ToString(),
                            nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                            // airFlow = int.Parse(reader1["airFlow"].ToString()),
                            //lastUpdatedDate = reader1["lastUpdatedDate"].ToString()
                            lastUpdatedXValue = reader1["lastUpdatedXValue"].ToString(),
                            lastUpdatedYValue = reader1["lastUpdatedYValue"].ToString()
                        });
                    }

                }

                //--Reading the line information
                SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conx);
                SQLiteDataReader reader2 = cmd2.ExecuteReader();
                while (reader2.Read())
                {


                    if (reader2["chart_respective_lineID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        lineInfoPulledForSaving.Add(new lineDataType_X
                        {
                            // count = int.Parse(reader2["count"].ToString()),
                            chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                            lineID = reader2["lineID"].ToString(),
                            prevNodeID = reader2["prevNodeID"].ToString(),
                            nextNodeID = reader2["nextNodeID"].ToString(),
                            lineColorValue = reader2["lineColorValue"].ToString(),
                            lineSeriesID = reader2["lineSeriesID"].ToString(),
                            thickness = reader2["thickness"].ToString(),
                            name = reader2["name"].ToString(),
                            status = reader2["status"].ToString()


                        });

                    }
                }


                //--Pulling data for device info

                SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conx);
                SQLiteDataReader reader3 = cmd3.ExecuteReader();
                while (reader3.Read())
                {

                    if (reader3["nodeID"].ToString() != "")
                    {
                        deviceInfoPulledForSaving.Add(new dt_for_device_info
                        {
                            nodeID = reader3["nodeID"].ToString(),
                            device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                            ip_for_param1 = reader3["IP_for_param1"].ToString(),

                            device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                            ip_for_param2 = reader3["IP_for_param2"].ToString(),

                            param1id = reader3["param1ID"].ToString(),
                            param2id = reader3["param2ID"].ToString(),
                            param1info = reader3["param1_info"].ToString(),
                            param2info = reader3["param2_info"].ToString(),
                            param1_id_type = reader3["param1_identifier_type"].ToString(),
                            param2_id_type = reader3["param2_identifier_type"].ToString()

                        });
                    }
                }


                //string sqlForFluidInfo = "SELECT * From  " + tableFluidInfo;


                //--Now reading the last part associated comfortzone with each chart
                SQLiteCommand cmd5 = new SQLiteCommand(sqlforfluidinfo, conx);
                SQLiteDataReader reader5 = cmd5.ExecuteReader();

                while (reader5.Read())
                {
                    if (reader5["chartID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        listfluidInfoForChart.Add(new fluidChartType
                        {
                            chartID = reader5["chartID"].ToString(),//reader["chartID"].ToString(),
                            fluidName = reader5["fluidName"].ToString(),
                            count = int.Parse(reader5["count"].ToString())
                        });
                    }
                }
                
            } //close of using statement 




            //} //close of using statement 



        }

        public void CreateTableForSavingConfiguration(string filePathWithName)
        {
            string tableForChartDetail = "tbl_chart_detail";
            string tableForNode = "tbl_node_value";
            string tableForLine = "tbl_line_value";
            string tableFordevice = "tbl_device_info_for_node";
           string tableForfluidinfo = "tbl_chart_fluid_info";


            //These two tables are for comfort zone 
            //string tableforComfortZoneDetail = "tbl_comfort_zone_detail";
            //string tableForChartComfortZoneSetting = "tbl_chart_comfort_zone_setting";

            // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + filePathWithName + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),temperature_source varchar(255),pressure_source varchar(255),name varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";//,lastUpdatedDate varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255),status varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),IP_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();

                //These tables are for comfort zone...
                //string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                //SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                //command6.ExecuteNonQuery();


                //string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                //SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                //command7.ExecuteNonQuery();

                string sql8 = "create table IF NOT EXISTS  " + tableForfluidinfo + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,fluidName varchar(255))";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


            }

        }

        public void InsertForSavingConfiguration(string fileWithPath, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice,string tableForFluidInfo)
        {

            //--This is where we are going to create all the database  and tables of sqlite
            //   string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";




            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + fileWithPath + ";Version=3;"))
            {
                m_dbConnection.Open();

                //Lets input chart detail id...if we are inserting we need to clear first
                string sql_input1 = null;
                foreach (var ch in chartInfoPulledForSaving)
                {
                    sql_input1 = "INSERT INTO " + chartTableName + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@id,@chartname,@cnid,@clid,@enableChartSt) ";
                    SQLiteCommand cmd = new SQLiteCommand(sql_input1, m_dbConnection);
                    cmd.Parameters.AddWithValue("@id", ch.chartID);
                    cmd.Parameters.AddWithValue("@chartname", ch.chartName);
                    cmd.Parameters.AddWithValue("@cnid", ch.chart_respective_nodeID);
                    cmd.Parameters.AddWithValue("@clid", ch.chart_respective_lineID);
                    cmd.Parameters.AddWithValue("@enableChartSt", ch.enableChartStatus);
                    cmd.ExecuteNonQuery();
                }


                //--Now lest input the node id
                foreach (var ch in nodeInfoPulledForSaving)
                {
                    //string sql_string = "insert into " + nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedDate) VALUES(@chartid,@id,@xVal,@yVal,@tempsource,@prsource,@name,@colorVal,@node_size_value,@lastUpdatedValue)";
                    string sql_string = "insert into " + nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedXValue,lastUpdatedYValue) VALUES(@chartid,@id,@xVal,@yVal,@tempsource,@prsource,@name,@colorVal,@node_size_value,@lastUpdatedX,@lastUpdatedY)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chart_respective_nodeID);
                    command.Parameters.AddWithValue("@id", ch.nodeID);
                    command.Parameters.AddWithValue("@xVal", ch.xValue.ToString());
                    command.Parameters.AddWithValue("@yVal", ch.yValue.ToString());
                    command.Parameters.AddWithValue("@tempsource", ch.temperature_source);
                    command.Parameters.AddWithValue("@prsource", ch.pressure_source);
                    command.Parameters.AddWithValue("@name", ch.name);
                    //command.Parameters.AddWithValue("@label", ch.label);
                    command.Parameters.AddWithValue("@colorVal", ch.colorValue);
                    //command.Parameters.AddWithValue("@text", ch.showTextItem);
                    command.Parameters.AddWithValue("@node_size_value", ch.nodeSize);
                    // command.Parameters.AddWithValue("@airflow", ch.airFlow);
                    command.Parameters.AddWithValue("@lastUpdatedX", ch.lastUpdatedXValue);
                    command.Parameters.AddWithValue("@lastUpdatedY", ch.lastUpdatedYValue);
                    command.ExecuteNonQuery();
                }


                //--Now lets input the line value

                foreach (var ch in lineInfoPulledForSaving)
                {
                    string sql_string = "insert into  " + lineTableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@lnName,@lnStatus)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chart_respective_lineID);
                    command.Parameters.AddWithValue("@id", ch.lineID);
                    command.Parameters.AddWithValue("@pn", ch.prevNodeID);
                    command.Parameters.AddWithValue("@nn", ch.nextNodeID);
                    command.Parameters.AddWithValue("@lc", ch.lineColorValue);
                    command.Parameters.AddWithValue("@ls", ch.lineSeriesID);
                    command.Parameters.AddWithValue("@thicknessValue", ch.thickness);
                    command.Parameters.AddWithValue("@lnName", ch.name);
                    command.Parameters.AddWithValue("@lnStatus", ch.status);
                    command.ExecuteNonQuery();
                    // command.ExecuteNonQuery();
                }


                //--This one is for device input  
                foreach (var ch in deviceInfoPulledForSaving)
                {
                    string sql_string = "insert into  " + tableNameDevice + " ( nodeID,device_instanceID_for_param1,IP_for_param1,device_instanceID_for_param2,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID_param1,@IP_param1,@instanceID_param2,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", ch.nodeID);
                    command.Parameters.AddWithValue("@instanceID_param1", ch.device_instance_id_for_param1);
                    command.Parameters.AddWithValue("@IP_param1", ch.ip_for_param1);
                    command.Parameters.AddWithValue("@instanceID_param2", ch.device_instance_id_for_param2);
                    command.Parameters.AddWithValue("@IP_param2", ch.ip_for_param2);
                    command.Parameters.AddWithValue("@param1", ch.param1id);
                    command.Parameters.AddWithValue("@param2", ch.param2id);
                    command.Parameters.AddWithValue("@param1info", ch.param1info);
                    command.Parameters.AddWithValue("@param2info", ch.param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", ch.param1_id_type);
                    command.Parameters.AddWithValue("@param2_iden_type", ch.param2_id_type);
                    command.ExecuteNonQuery();
                }



                //---Now for the comfort zone setting i guess... :)
                foreach (var ch in listfluidInfoForChart)
                {
                    string sql_string = "insert into " + tableForFluidInfo + "(chartID,fluidName) VALUES(@chartid,@comfortzoneid)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chartID);
                    command.Parameters.AddWithValue("@comfortzoneid", ch.fluidName);
                    //command.Parameters.AddWithValue("@status", ch.status);
                    command.ExecuteNonQuery();
                }
                

            } //Close of using




        }

        public void LoadDataFromFileToDBConfiguration()
        {
            //--This helps in loading function 

            //--Now required function and tables 

            //--nothing just clearing the values
            //--Resetting values first
            //chartInfoPulledForSaving.Clear();
            //nodeInfoPulledForSaving.Clear();
            //lineInfoPulledForSaving.Clear();
            //deviceInfoPulledForSaving.Clear();
            //comfortZoneInforForEachChartForSaving.Clear();
            //ComfortZonesDetailForSaving.Clear();


            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_chart_detail";
            string nodeTableName = "tbl_node_value";
            string lineTableName = "tbl_line_value";
            string tableNameDevice = "tbl_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_comfort_zone_detail";
            string tableForfluidInfo = "tbl_chart_fluid_info";


            OpenFileDialog theDialog = new OpenFileDialog();
            theDialog.Title = "Open s3db File";
            theDialog.Filter = "s3db files|*.s3db";
            theDialog.InitialDirectory = @"C:\";
            if (theDialog.ShowDialog() == DialogResult.OK)
            {
                // MessageBox.Show(theDialog.FileName.ToString());
                //here we are going to read the data and all the function 

                //lets get the id values...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = theDialog.FileName;
                string pathToFile = @"Data Source=" + databaseFile + ";Version=3;";

                // MessageBox.Show("BBK TEST Hellow,PATH = "+pathToFile);


                //Test(pathToFile);
                //return;

                //--This function reads the data
                //####### TASK1 :Read data
                // try {
                // MessageBox.Show("read data"); 
                ReadDataForSavingConfiguration_For_Load(pathToFile, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForfluidInfo);//This reads all the data

                // MessageBox.Show("finish read data");
                //}catch(Exception ex)
                //{
                //    MessageBox.Show("Exception data : " + ex.Data + ",\nexp mess:" + ex.Message + "\nexp source" + ex.Source + "exp " + ex);
                //}
                // MessageBox.Show("Hellow END");

                //######### TASK2: Delete data
                //--Now removing the table datas form db 
                // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
                //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFileName1 = databasePath1 + @"\db_psychrometric_project.s3db";
                string databaseFileName1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

                //--Table names
                string chartTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevice1 = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                //string tableForComfortZoneSetting1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                //string tableForCF_Detail1 = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableforfluidinfo = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_fluid_info";

                DeleteAllDataFromTable(databaseFileName1, chartTableName1);
                DeleteAllDataFromTable(databaseFileName1, nodeTableName1);
                DeleteAllDataFromTable(databaseFileName1, lineTableName1);
                DeleteAllDataFromTable(databaseFileName1, tableNameDevice1);
                DeleteAllDataFromTable(databaseFileName1, tableforfluidinfo);
                //DeleteAllDataFromTable(databaseFileName1, tableForComfortZoneSetting1);
                //DeleteAllDataFromTable(databaseFileName1, tableForCF_Detail1);
                //DeleteAllDataFromTable(databaseFileName1, tableformixnode);

                //####### TASK3 : Writing to db 
                //Now writing the data to db

                string chartTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevicex = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                //string tableForComfortZoneSettingx = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                //string tableForCF_Detailx = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableForfluidInfox = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_fluid_info";

                // MessageBox.Show("Loading Here mix table "+ tableformixnodex+"dbfILE NAME"+ databaseFileName1);

                InsertForSavingConfiguration_For_Load(databaseFileName1, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForfluidInfox);


                // MessageBox.Show("InsertioncOMPLETE");
                //--Pulling data and plotting values--
                //DataGridView_Show_Data();
                //Now reloading the data 
                //PullDataAndPlot();

            }
        }


        public List<chartDetailDT_X> chartInfoPulledForSaving_For_Load = new List<chartDetailDT_X>();//This is used for storing the chart detail ids

        //Nowlets have the node information ==>> all node information
        public List<nodeDataTypeForSaving> nodeInfoPulledForSaving_For_Load = new List<nodeDataTypeForSaving>();

        //NOw list for line info
        public List<lineDataType_X> lineInfoPulledForSaving_For_Load = new List<lineDataType_X>();

        //Now  list for pulling the device info
        public List<dt_for_device_info> deviceInfoPulledForSaving_For_Load = new List<dt_for_device_info>();

        public List<fluidChartType> listFluidInfo_For_Load = new List<fluidChartType>();
        //Now for storing the comfortzone setting...
        //public List<datatype_for_comfortzone> comfortZoneInforForEachChartForSaving_For_Load = new List<datatype_for_comfortzone>();//which chart is associated with which comfortzone

        ////Now for storing the actual confort zone values
        //public List<dataTypeForCF> ComfortZonesDetailForSaving_For_Load = new List<dataTypeForCF>();  //This one contains the comfortzone setting

        //public List<dataTypeFor_mix_node_info> mixNodeList_For_Load = new List<dataTypeFor_mix_node_info>();  //This one contains the comfortzone setting


        public void ReadDataForSavingConfiguration_For_Load(string DB_String_path, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice,string tableForFluidInfo)
        {

            //--Resetting values first
            chartInfoPulledForSaving_For_Load.Clear();
            nodeInfoPulledForSaving_For_Load.Clear();
            lineInfoPulledForSaving_For_Load.Clear();
            deviceInfoPulledForSaving_For_Load.Clear();
            listFluidInfo_For_Load.Clear();
            //comfortZoneInforForEachChartForSaving_For_Load.Clear();
            //ComfortZonesDetailForSaving_For_Load.Clear();
            //mixNodeList_For_Load.Clear();

            string connectionString = DB_String_path;// @"Data Source=" + path + ";Version=3;";

            string sql_for_chart_detail = "SELECT * From  " + chartTableName;
            string sql_for_node = "SELECT * From  " + nodeTableName;
            string sql_for_line = "SELECT * From  " + lineTableName;
            string sql_forDevice = "SELECT * From  " + tableNameDevice;
            string sql_forfluidInfo = "SELECT * From  " + tableForFluidInfo;

            //string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            //string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
            //string sql_for_mix_node = "SELECT * From  " + tableForMixNode;



            // MessageBox.Show("inside sqlite path =" + path);         
            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {

                //MessageBox.Show("BHIRA");
                //conx.Close();
                conName.Open();
                using (SQLiteCommand cmdx = new SQLiteCommand(sql_for_chart_detail, conName))
                {
                    SQLiteDataReader reader = cmdx.ExecuteReader();
                    while (reader.Read())
                    {

                        if (reader["chartID"].ToString() != "")
                        {
                            //--Reading and inserting in a List
                            // if(!chartInfoPulledForSaving_For_Load.Contains())

                            // if(chartInfoPulledForSaving_For_Load)
                            foreach (var items in chartInfoPulledForSaving_For_Load)
                            {
                                if (items.chartID == reader["chartID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }

                            chartInfoPulledForSaving_For_Load.Add(new chartDetailDT_X
                            {
                                chartID = reader["chartID"].ToString(),
                                chartName = reader["chartName"].ToString(),
                                chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                                chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                                enableChartStatus = reader["enableChartStatus"].ToString()
                            });
                        }
                    }

                    reader.Close();
                }
            }

            // MessageBox.Show("chart count value = " + chartInfoPulledForSaving_For_Load.Count);

            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //----Reading the node information 

                using (SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conName))
                {
                    SQLiteDataReader reader1 = cmd1.ExecuteReader();
                    while (reader1.Read())
                    {

                        if (reader1["chart_respective_nodeID"].ToString() != "")
                        {

                            //if items present then do not add

                            foreach (var items in nodeInfoPulledForSaving_For_Load)
                            {
                                if (items.nodeID == reader1["nodeID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }


                            //--Reading and inserting in a List
                            nodeInfoPulledForSaving_For_Load.Add(new nodeDataTypeForSaving
                            {

                                ////--count = int.Parse(reader1["count"].ToString()),


                                chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                                nodeID = reader1["nodeID"].ToString(),
                                xValue = double.Parse(reader1["xValue"].ToString()),
                                yValue = double.Parse(reader1["yValue"].ToString()),
                                //source = reader1["source"].ToString(),
                                temperature_source = reader1["temperature_source"].ToString(),
                                pressure_source = reader1["pressure_source"].ToString(),
                                name = reader1["name"].ToString(),
                                //label = reader1["label"].ToString(),
                                colorValue = reader1["colorValue"].ToString(),
                                //showTextItem = reader1["showTextItem"].ToString(),
                                nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                                // airFlow = int.Parse(reader1["airFlow"].ToString()),
                                // lastUpdatedDate = reader1["lastUpdatedDate"].ToString()
                                lastUpdatedXValue = reader1["lastUpdatedXValue"].ToString(),
                                lastUpdatedYValue = reader1["lastUpdatedYValue"].ToString()

                            });
                        }

                    }
                    reader1.Close();

                }
            }

            // MessageBox.Show("Node count value = " + nodeInfoPulledForSaving_For_Load.Count);

            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Reading the line information
                using (SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conName))
                {
                    SQLiteDataReader reader2 = cmd2.ExecuteReader();
                    while (reader2.Read())
                    {


                        if (reader2["chart_respective_lineID"].ToString() != "")
                        {


                            foreach (var items in lineInfoPulledForSaving_For_Load)
                            {
                                if (items.lineID == reader2["lineID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }


                            //This is the reading part of the data...
                            lineInfoPulledForSaving_For_Load.Add(new lineDataType_X
                            {
                                // count = int.Parse(reader2["count"].ToString()),
                                chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                                lineID = reader2["lineID"].ToString(),
                                prevNodeID = reader2["prevNodeID"].ToString(),
                                nextNodeID = reader2["nextNodeID"].ToString(),
                                lineColorValue = reader2["lineColorValue"].ToString(),
                                lineSeriesID = reader2["lineSeriesID"].ToString(),
                                thickness = reader2["thickness"].ToString(),
                                name = reader2["name"].ToString(),
                                status = reader2["status"].ToString()

                            });

                        }
                    }
                    reader2.Close();
                }
            }

            //  MessageBox.Show("Line count value = " + lineInfoPulledForSaving_For_Load.Count);


            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Pulling data for device info

                using (SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conName))
                {
                    SQLiteDataReader reader3 = cmd3.ExecuteReader();
                    while (reader3.Read())
                    {

                        if (reader3["nodeID"].ToString() != "")
                        {

                            foreach (var items in deviceInfoPulledForSaving_For_Load)
                            {
                                if (items.nodeID == reader3["nodeID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }



                            deviceInfoPulledForSaving_For_Load.Add(new dt_for_device_info
                            {
                                nodeID = reader3["nodeID"].ToString(),
                                device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                                ip_for_param1 = reader3["IP_for_param1"].ToString(),
                                device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                                ip_for_param2 = reader3["IP_for_param2"].ToString(),
                                param1id = reader3["param1ID"].ToString(),
                                param2id = reader3["param2ID"].ToString(),
                                param1info = reader3["param1_info"].ToString(),
                                param2info = reader3["param2_info"].ToString(),
                                param1_id_type = reader3["param1_identifier_type"].ToString(),
                                param2_id_type = reader3["param2_identifier_type"].ToString()

                            });
                        }
                    }

                    reader3.Close();
                }

            }
            // MessageBox.Show("device count value = " + deviceInfoPulledForSaving_For_Load.Count);

            //For fluid info
            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Now reading the chart info
                using (SQLiteCommand cmd4 = new SQLiteCommand(sql_forfluidInfo, conName))
                {
                    SQLiteDataReader reader4 = cmd4.ExecuteReader();
                    while (reader4.Read())
                    {

                       

                            listFluidInfo_For_Load.Add(new fluidChartType
                            {
                                chartID = reader4["chartID"].ToString(),
                                fluidName = reader4["fluidName"].ToString(),
                                count = int.Parse(reader4["count"].ToString())
                                
                            });
                       
                    }
                    reader4.Close();
                }
            }
            ////  MessageBox.Show("cf detail count value = " + ComfortZonesDetailForSaving_For_Load.Count);

            //using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            //{
            //    conName.Open();
            //    //--Now reading the last part associated comfortzone with each chart
            //    using (SQLiteCommand cmdFive = new SQLiteCommand(sql_for_CF_Setting, conName))
            //    {
            //        SQLiteDataReader readerFive = cmdFive.ExecuteReader();

            //        while (readerFive.Read())
            //        {
            //            if (readerFive["chartID"].ToString() != "")
            //            {
            //                foreach (var items in comfortZoneInforForEachChartForSaving_For_Load)
            //                {
            //                    if (items.chartid == readerFive["chartID"].ToString())
            //                    {
            //                        //if contain do not add
            //                        return;
            //                    }
            //                }

            //                //This is the reading part of the data...
            //                comfortZoneInforForEachChartForSaving_For_Load.Add(new datatype_for_comfortzone
            //                {

            //                    chartid = readerFive["chartID"].ToString(),//reader["chartID"].ToString(),
            //                    comfortzoneid = readerFive["comfort_zone_ID"].ToString(),
            //                    status = readerFive["status"].ToString()
            //                });
            //            }
            //        }
            //        readerFive.Close();
            //    } //close of using statement
            //      //  MessageBox.Show("cf single for chart count value = " + comfortZoneInforForEachChartForSaving_For_Load.Count);
            //}



            ////=============For MXING PART====================//




            //using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            //{
            //    conName.Open();
            //    //--Now reading the chart info
            //    using (SQLiteCommand cmd5 = new SQLiteCommand(sql_for_mix_node, conName))
            //    {
            //        SQLiteDataReader reader5 = cmd5.ExecuteReader();
            //        while (reader5.Read())
            //        {

            //            if (reader5["chartID"].ToString() != "")
            //            {
            //                foreach (var items in mixNodeList_For_Load)
            //                {
            //                    if (items.ChartID == reader5["chartID"].ToString())
            //                    {
            //                        //if contain do not add
            //                        return;
            //                    }
            //                }

            //                mixNodeList_For_Load.Add(new dataTypeFor_mix_node_info
            //                {
            //                    ChartID = reader5["chartID"].ToString(),
            //                    nodeID = reader5["nodeID"].ToString(),
            //                    previousNodeID = reader5["previousNodeID"].ToString(),
            //                    nextNodeID = reader5["nextNodeID"].ToString()

            //                });
            //            }
            //        }
            //        reader5.Close();
            //    }
            //}

            //=======================END MIXING=========================//



        }


        public void DeleteAllDataFromTable(string db_path, string tableName)
        {
            // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
            // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + db_path + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName;
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.ExecuteNonQuery();

            }//Close of using 
        }

        public void InsertForSavingConfiguration_For_Load(string fileWithPath, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice,string tablefluidinfo)
        {

            //--This is where we are going to create all the database  and tables of sqlite
            //   string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";




            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + fileWithPath + ";Version=3;"))
            {
                m_dbConnection.Open();

                //Lets input chart detail id...if we are inserting we need to clear first
                string sql_input1 = null;
                foreach (var ch in chartInfoPulledForSaving_For_Load)
                {
                    sql_input1 = "INSERT INTO " + chartTableName + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@id,@chartname,@cnid,@clid,@enablestatus) ";
                    SQLiteCommand cmd = new SQLiteCommand(sql_input1, m_dbConnection);
                    cmd.Parameters.AddWithValue("@id", ch.chartID);
                    cmd.Parameters.AddWithValue("@chartname", ch.chartName);
                    cmd.Parameters.AddWithValue("@cnid", ch.chart_respective_nodeID);
                    cmd.Parameters.AddWithValue("@clid", ch.chart_respective_lineID);
                    cmd.Parameters.AddWithValue("@enablestatus", ch.enableChartStatus);
                    cmd.ExecuteNonQuery();
                }


                //--Now lest input the node id
                foreach (var ch in nodeInfoPulledForSaving_For_Load)
                {
                    // string sql_string = "insert into " + nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedDate) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@pressure_source,@name,@colorVal,@node_size_value,@lastupdateddate)";
                    string sql_string = "insert into " + nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,pressure_source,name,colorValue,nodeSize,lastUpdatedXValue,lastUpdatedYValue) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@pressure_source,@name,@colorVal,@node_size_value,@lastupdatedX,@lastupdatedY)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chart_respective_nodeID);
                    command.Parameters.AddWithValue("@id", ch.nodeID);
                    command.Parameters.AddWithValue("@xVal", ch.xValue.ToString());
                    command.Parameters.AddWithValue("@yVal", ch.yValue.ToString());
                    //command.Parameters.AddWithValue("@source", ch.source);
                    command.Parameters.AddWithValue("@temperature_source", ch.temperature_source);
                    command.Parameters.AddWithValue("@pressure_source", ch.pressure_source);
                    command.Parameters.AddWithValue("@name", ch.name);
                    //command.Parameters.AddWithValue("@label", ch.label);
                    command.Parameters.AddWithValue("@colorVal", ch.colorValue);
                    //command.Parameters.AddWithValue("@text", ch.showTextItem);
                    command.Parameters.AddWithValue("@node_size_value", ch.nodeSize);
                   // command.Parameters.AddWithValue("@airflow", ch.airFlow);
                    command.Parameters.AddWithValue("@lastupdatedX", ch.lastUpdatedXValue);
                    command.Parameters.AddWithValue("@lastupdatedY", ch.lastUpdatedYValue);
                    command.ExecuteNonQuery();
                }


                //--Now lets input the line value

                foreach (var ch in lineInfoPulledForSaving_For_Load)
                {
                    string sql_string = "insert into  " + lineTableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@lnName,@lnStatus)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chart_respective_lineID);
                    command.Parameters.AddWithValue("@id", ch.lineID);
                    command.Parameters.AddWithValue("@pn", ch.prevNodeID);
                    command.Parameters.AddWithValue("@nn", ch.nextNodeID);
                    command.Parameters.AddWithValue("@lc", ch.lineColorValue);
                    command.Parameters.AddWithValue("@ls", ch.lineSeriesID);
                    command.Parameters.AddWithValue("@thicknessValue", ch.thickness);
                    command.Parameters.AddWithValue("@lnName", ch.name);
                    command.Parameters.AddWithValue("@lnStatus", ch.status);
                    command.ExecuteNonQuery();

                    // command.ExecuteNonQuery();
                }


                //--This one is for device input  
                foreach (var ch in deviceInfoPulledForSaving_For_Load)
                {
                    string sql_string = "insert into  " + tableNameDevice + "(nodeID,device_instanceID_for_param1,IP_for_param1,device_instanceID_for_param2,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID1,@IP1,@instanceID2,@IP2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", ch.nodeID);
                    command.Parameters.AddWithValue("@instanceID1", ch.device_instance_id_for_param1);
                    command.Parameters.AddWithValue("@IP1", ch.ip_for_param1);
                    command.Parameters.AddWithValue("@instanceID2", ch.device_instance_id_for_param2);
                    command.Parameters.AddWithValue("@IP2", ch.ip_for_param2);
                    command.Parameters.AddWithValue("@param1", ch.param1id);
                    command.Parameters.AddWithValue("@param2", ch.param2id);
                    command.Parameters.AddWithValue("@param1info", ch.param1info);
                    command.Parameters.AddWithValue("@param2info", ch.param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", ch.param1_id_type);
                    command.Parameters.AddWithValue("@param2_iden_type", ch.param2_id_type);
                    command.ExecuteNonQuery();
                }



                //---fluidinfo
                foreach (var ch in listFluidInfo_For_Load)
                {
                    string sql_string = "insert into " + tablefluidinfo + "(chartID,fluidName) VALUES(@chartid,@comfortzoneid)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@chartid", ch.chartID);
                    command.Parameters.AddWithValue("@comfortzoneid", ch.fluidName);
                    //command.Parameters.AddWithValue("@status", ch.status);
                    command.ExecuteNonQuery();
                }


                ////--Information about detail comfortzone

                //foreach (var ch in ComfortZonesDetailForSaving_For_Load)
                //{

                //    string sql_string = "insert into " + tableForCF_Detail + "(id,name,min_temp,max_temp,min_hum,max_hum,colorValue) VALUES(@id,@name,@min_t,@max_t,@min_h,@max_h,@color)";
                //    SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                //    command.CommandType = CommandType.Text;
                //    command.Parameters.AddWithValue("@id", ch.id);
                //    command.Parameters.AddWithValue("@name", ch.name);
                //    command.Parameters.AddWithValue("@min_t", ch.min_temp.ToString());
                //    command.Parameters.AddWithValue("@max_t", ch.max_temp.ToString());
                //    command.Parameters.AddWithValue("@min_h", ch.min_hum.ToString());
                //    command.Parameters.AddWithValue("@max_h", ch.max_hum.ToString());
                //    command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(ch.colorValue));
                //    command.ExecuteNonQuery();

                //}


                //foreach (var ch in mixNodeList_For_Load)
                //{
                //    string sql_input1x = "insert into  " + tableMixNode + " (chartID,nodeID,previousNodeID,nextNodeID) VALUES(@chartid,@nodeid,@cnid,@clid) ";
                //    SQLiteCommand cmdx = new SQLiteCommand(sql_input1x, m_dbConnection);
                //    cmdx.Parameters.AddWithValue("@chartid", ch.ChartID);
                //    cmdx.Parameters.AddWithValue("@nodeid", ch.nodeID);
                //    cmdx.Parameters.AddWithValue("@cnid", ch.previousNodeID);
                //    cmdx.Parameters.AddWithValue("@clid", ch.nextNodeID);
                //    cmdx.ExecuteNonQuery();
                //}

            }//cLOSE OF USING

        }

        public void DisableChart(string chartID)
        {
            /*
            Steps :1. Frist read line info,nodeinfo,comfortzone info for particular chart id
                    2.Then delete line value using lineID,
                    3.Delete node values using nodeID
                    4.Delete comfortzone value using chartid
                    5.delete the chart info using chart id value
            */

            //string chartID =   
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";// "tbl_" ++"_node_value";
           //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
           //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set enableChartStatus = @status    where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", "false");
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using          
            
        }//Close of dissable chart

        public void ReadDataForSavingTemplateFile(string chartId, string path, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice,string tableFluidInfo)
        {

            //--Resetting values first
            chartInfoPulledForSaving.Clear();
            nodeInfoPulledForSaving.Clear();
            lineInfoPulledForSaving.Clear();
            deviceInfoPulledForSaving.Clear();
            listfluidInfoForChart.Clear();
            //comfortZoneInforForEachChartForSaving.Clear();
            //ComfortZonesDetailForSaving.Clear();
            //mixNodeInfoListForSaveConfiguration.Clear();

            ////--This reads the value of data form db lets use list for storing the data
            //string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            //string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            //string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            //string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";

            ////lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connectionString = @"Data Source=" + path + ";Version=3;";


            string sql_for_chart_detail = "SELECT * From  " + chartTableName + "  where chartID = '" + chartId + "'";
            //string sql_for_node = "SELECT * From  " + nodeTableName;
            //string sql_for_line = "SELECT * From  " + lineTableName;
            //string sql_forDevice = "SELECT * From  " + tableNameDevice;
            //string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            //string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
            //string sql_for_mix_node_info = "SELECT * From  " + mixnodeInfoTable;
            //;



            //  MessageBox.Show("sql1=" + sql1);         
            using (SQLiteConnection conx = new SQLiteConnection(connectionString))
            {

                conx.Close();
                conx.Open();

                SQLiteCommand cmd = new SQLiteCommand(sql_for_chart_detail, conx);
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {

                    if (reader["chartID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        chartInfoPulledForSaving.Add(new chartDetailDT_X
                        {

                            //  count = int.Parse(reader["count"].ToString()),
                            chartID = reader["chartID"].ToString(),
                            chartName = reader["chartName"].ToString(),
                            chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                            chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                            enableChartStatus = reader["enableChartStatus"].ToString()

                        });
                    }
                }

                string sql_for_node = "SELECT * From  " + nodeTableName + "  where chart_respective_nodeID = '" + chartInfoPulledForSaving[0].chart_respective_nodeID + "'";

                //----Reading the node information 

                SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conx);
                SQLiteDataReader reader1 = cmd1.ExecuteReader();
                while (reader1.Read())
                {

                    if (reader1["chart_respective_nodeID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        nodeInfoPulledForSaving.Add(new nodeDataTypeForSaving
                        {

                            //count = int.Parse(reader1["count"].ToString()),
                            chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                            nodeID = reader1["nodeID"].ToString(),
                            xValue = double.Parse(reader1["xValue"].ToString()),
                            yValue = double.Parse(reader1["yValue"].ToString()),
                            temperature_source = reader1["temperature_source"].ToString(),
                            pressure_source = reader1["pressure_source"].ToString(),
                            name = reader1["name"].ToString(),
                            // label = reader1["label"].ToString(),
                            colorValue = reader1["colorValue"].ToString(),
                            // showTextItem = reader1["showTextItem"].ToString(),
                            nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                            //airFlow = int.Parse(reader1["airFlow"].ToString()),
                            // lastUpdatedDate = reader1["lastUpdatedDate"].ToString()
                            lastUpdatedXValue = reader1["lastUpdatedXValue"].ToString(),
                            lastUpdatedYValue = reader1["lastUpdatedYValue"].ToString()

                        });
                    }

                }

                string sql_for_line = "SELECT * From  " + lineTableName + "  where chart_respective_lineID = '" + chartInfoPulledForSaving[0].chart_respective_lineID + "'";


                //--Reading the line information
                SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conx);
                SQLiteDataReader reader2 = cmd2.ExecuteReader();
                while (reader2.Read())
                {


                    if (reader2["chart_respective_lineID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        lineInfoPulledForSaving.Add(new lineDataType_X
                        {
                            // count = int.Parse(reader2["count"].ToString()),
                            chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                            lineID = reader2["lineID"].ToString(),
                            prevNodeID = reader2["prevNodeID"].ToString(),
                            nextNodeID = reader2["nextNodeID"].ToString(),
                            lineColorValue = reader2["lineColorValue"].ToString(),
                            lineSeriesID = reader2["lineSeriesID"].ToString(),
                            thickness = reader2["thickness"].ToString(),
                            name = reader2["name"].ToString(),
                            status = reader2["status"].ToString()


                        });

                    }
                }


                for (int i = 0; i < nodeInfoPulledForSaving.Count; i++)
                {


                    if (nodeInfoPulledForSaving[i].temperature_source == "Device" || nodeInfoPulledForSaving[i].pressure_source == "Device")
                    {
                        string sql_forDevice = "SELECT * From  " + tableNameDevice + "  where nodeID = '" + nodeInfoPulledForSaving[i].nodeID + "'";

                        //--Pulling data for device info
                        SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conx);
                        SQLiteDataReader reader3 = cmd3.ExecuteReader();
                        while (reader3.Read())
                        {

                            if (reader3["nodeID"].ToString() != "")
                            {
                                deviceInfoPulledForSaving.Add(new dt_for_device_info
                                {
                                    nodeID = reader3["nodeID"].ToString(),
                                    device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                                    ip_for_param1 = reader3["IP_for_param1"].ToString(),
                                    device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                                    ip_for_param2 = reader3["IP_for_param2"].ToString(),
                                    param1id = reader3["param1ID"].ToString(),
                                    param2id = reader3["param2ID"].ToString(),
                                    param1info = reader3["param1_info"].ToString(),
                                    param2info = reader3["param2_info"].ToString(),
                                    param1_id_type = reader3["param1_identifier_type"].ToString(),
                                    param2_id_type = reader3["param2_identifier_type"].ToString()

                                });
                            }
                        }

                    }//--Close of if loop


                }//Close of the for loop


                string sqlForFluidInfo = "SELECT * From  " + tableFluidInfo + "  where chartID = '" + chartId + "'";


                //--Now reading the last part associated comfortzone with each chart
                SQLiteCommand cmd5 = new SQLiteCommand(sqlForFluidInfo, conx);
                SQLiteDataReader reader5 = cmd5.ExecuteReader();

                while (reader5.Read())
                {
                    if (reader5["chartID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        listfluidInfoForChart.Add(new fluidChartType
                        {
                            chartID = reader5["chartID"].ToString(),//reader["chartID"].ToString(),
                            fluidName = reader5["fluidName"].ToString(),
                            count = int.Parse(reader5["count"].ToString())
                        });
                    }
                }


                //if (comfortZoneInforForEachChartForSaving.Count > 0)
                //{
                //    string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail + " where id = '" + comfortZoneInforForEachChartForSaving[0].comfortzoneid + "'";


                //    //--Now reading the chart info
                //    SQLiteCommand cmd4 = new SQLiteCommand(sql_for_CF_Detail, conx);
                //    SQLiteDataReader reader4 = cmd4.ExecuteReader();
                //    while (reader4.Read())
                //    {

                //        if (reader4["id"].ToString() != "")
                //        {
                //            ComfortZonesDetailForSaving.Add(new dataTypeForCF
                //            {
                //                id = reader4["id"].ToString(),
                //                name = reader4["name"].ToString(),
                //                min_temp = reader4["min_temp"].ToString(), //this is in string formate
                //                max_temp = reader4["max_temp"].ToString(),
                //                min_hum = reader4["min_hum"].ToString(),
                //                max_hum = reader4["max_hum"].ToString(),
                //                colorValue = ColorTranslator.FromHtml(reader4["colorValue"].ToString())
                //            });
                //        }
                //    }
                //}//IF THE VALUE EXIST CLOSE


                //string sql_for_mix_node_info = "SELECT * From  " + mixnodeInfoTable + " where chartID  = '" + chartId + "'";



                ////==For mix node loading up 
                //SQLiteCommand cmd6 = new SQLiteCommand(sql_for_mix_node_info, conx);
                //SQLiteDataReader reader6 = cmd6.ExecuteReader();

                //while (reader6.Read())
                //{
                //    if (reader6["chartID"].ToString() != "")
                //    {
                //        //This is the reading part of the data...
                //        mixNodeInfoListForSaveConfiguration.Add(new dataTypeFor_mix_node_info
                //        {
                //            ChartID = reader6["chartID"].ToString(),//reader["chartID"].ToString(),
                //            nodeID = reader6["nodeID"].ToString(),
                //            previousNodeID = reader6["previousNodeID"].ToString(),
                //            nextNodeID = reader6["nextNodeID"].ToString(),
                //        });
                //    }
                //}



            } //close of using statement 


            //} //close of using statement 



        }

        public void SaveChartAsTemplate(string chartID,Form_Main_PH_Application f1)
        {
            //--Path and table names
            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            //string tableFor_mix_node_info = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";
            string tableFluidInfo = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_fluid_info";
            //lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile_x = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile_x = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";


            //Reading the data form SQLite DB
            ReadDataForSavingTemplateFile(chartID, databaseFile_x, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableFluidInfo);//This reads all the data

            string fileName = "";
            f1.saveFD.InitialDirectory = "C:";
            f1.saveFD.FileName = "SavePsyTemplateFile";
            f1.saveFD.Filter = "s3db(.s3db) |*.s3db";
            //ImageFormat format = ImageFormat.Png;
            if (f1.saveFD.ShowDialog() == DialogResult.OK)
            {
                //  Cursor.Current = Cursors.WaitCursor;
                fileName = f1.saveFD.FileName;

                // Cursor.current = Cursors.WaitCursor;

                //saving the file in that path
                string databaseFile = fileName;//path + fileName; //databasePath1 + @"\db_psychrometric_project.s3db";

                //--Creating file and table details 

                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                //SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                //m_dbConnection.Open();

                CreateTableForSavingTemplateFile(databaseFile); //Creates a file and saves in the files

                //After the table has been created we can write to the database 
                //--InsertDataForSavingtoSQLiteDB(string pathWithFileName);

                //--Reading data will be done before this so 

                //---Writing to this db ==>insertion


                string chartTableNamex = "tbl_chart_detail";
                string nodeTableNamex = "tbl_node_value";
                string lineTableNamex = "tbl_line_value";
                string tableNameDevicex = "tbl_device_info_for_node";//currentNodeTableFromDB; 
                //string tableForComfortZoneSettingx = "tbl_chart_comfort_zone_setting";
                //string tableForCF_Detailx = "tbl_comfort_zone_detail";
                //string tableForMixNode = "tbl_mix_node_info";
                string tableForFluidInfo = "tbl_chart_fluid_info";


                //--This will work same as for saving the entire files ok ok
                InsertForSavingConfiguration(databaseFile, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForFluidInfo);

                // SQLiteConnection.ClearPool();//This helps in 
            }


        }// close of fxn

        public void CreateTableForSavingTemplateFile(string filePathWithName)
        {
            string signaturetableForCheck = "signature_table";
            string tableForChartDetail = "tbl_chart_detail";
            string tableForNode = "tbl_node_value";
            string tableForLine = "tbl_line_value";
            string tableFordevice = "tbl_device_info_for_node";
           // string tableFormixNodeInfo = "tbl_mix_node_info";


            //These two tables are for comfort zone 
            string tableforComfortZoneDetail = "tbl_comfort_zone_detail";
            string tableForChartComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            string tableFluidInfo = "tbl_chart_fluid_info";

            // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + filePathWithName + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..
                string sql3x = "create table IF NOT EXISTS " + signaturetableForCheck + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartName VARCHAR(255))";
                SQLiteCommand command3x = new SQLiteCommand(sql3x, connection);
                command3x.ExecuteNonQuery();


                //--We also need to create table for particular data added..
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                //string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),temperature_source varchar(255),pressure_source varchar(255),name varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(255),lastUpdatedDate varchar(255))";
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),temperature_source varchar(255),pressure_source varchar(255),name varchar(255),colorValue varchar(255),nodeSize varchar(255),lastUpdatedXValue varchar(255),lastUpdatedYValue varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255),status varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),IP_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();

                //These tables are for comfort zone...
                //string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                //SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                //command6.ExecuteNonQuery();


                //string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                //SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                //command7.ExecuteNonQuery();

                string sql8 = "create table IF NOT EXISTS  " + tableFluidInfo + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,fluidName varchar(255) )";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


            }

        }

        public void LoadDataFromTemplateFileToDBActionTemplateFileUpload(string chartId, string chartName)
        {
            //--This helps in loading function 

            //--Now required function and tables 



            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_chart_detail";
            string nodeTableName = "tbl_node_value";
            string lineTableName = "tbl_line_value";
            string tableNameDevice = "tbl_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_comfort_zone_detail";
            //string tableForMixNode = "tbl_mix_node_info";
            string tableForFluidInfo = "tbl_chart_fluid_info";


            OpenFileDialog theDialog = new OpenFileDialog();
            theDialog.Title = "Open s3db File";
            theDialog.Filter = "s3db files|*.s3db";
            theDialog.InitialDirectory = @"C:\";
            if (theDialog.ShowDialog() == DialogResult.OK)
            {
                // MessageBox.Show(theDialog.FileName.ToString());
                //here we are going to read the data and all the function 

                //lets get the id values...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = theDialog.FileName;
                string pathToFile = @"Data Source=" + databaseFile + ";Version=3;";

                // MessageBox.Show("BBK TEST Hellow,PATH = "+pathToFile);


                //Test(pathToFile);
                //return;

                //--This function reads the data
                //####### TASK1 :Read data
                // try {

                // MessageBox.Show("read data"); 

                //--Checking the validity of the template file

                if (!TableExistOrNotCheckTemplateFile(pathToFile))
                {
                    MessageBox.Show("Please load a valid template file");
                    return;

                }


                ReadDataForSavingConfiguration_For_Load(pathToFile, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForFluidInfo);//This reads all the data

                // MessageBox.Show("finish read data");
                //}catch(Exception ex)
                //{
                //    MessageBox.Show("Exception data : " + ex.Data + ",\nexp mess:" + ex.Message + "\nexp source" + ex.Source + "exp " + ex);
                //}
                // MessageBox.Show("Hellow END");

                //######### TASK2: Delete data
                //--Now removing the table datas form db 
                // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
                //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFileName1 = databasePath1 + @"\db_psychrometric_project.s3db";
                string databaseFileName1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";


                //--Table names
                string chartTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevice1 = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                string tablefluidinfo = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_fluid_info";//currentNodeTableFromDB; 
                //string tableForComfortZoneSetting1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                //string tableForCF_Detail1 = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                //string tableformixnode = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                //DeleteAllDataFromTable(databaseFileName1, chartTableName1);
                //DeleteAllDataFromTable(databaseFileName1, nodeTableName1);
                //DeleteAllDataFromTable(databaseFileName1, lineTableName1);
                //DeleteAllDataFromTable(databaseFileName1, tableNameDevice1);
                //DeleteAllDataFromTable(databaseFileName1, tableForComfortZoneSetting1);
                //DeleteAllDataFromTable(databaseFileName1, tableForCF_Detail1);
                //DeleteAllDataFromTable(databaseFileName1, tableformixnode);

                //####### TASK3 : Writing to db 
                //Now writing the data to db

                string chartTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevicex = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                //string tableForComfortZoneSettingx = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                //string tableForCF_Detailx = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                //string tableformixnodex = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                // MessageBox.Show("Loading Here mix table "+ tableformixnodex+"dbfILE NAME"+ databaseFileName1);



                for (int i = 0; i < chartInfoPulledForSaving_For_Load.Count; i++)
                {
                    if (chartInfoPulledForSaving_For_Load[i].chartID == chartId)
                    {
                        MessageBox.Show("Chart with same ID already present. Could not load the template file");
                        return;

                    }
                    else if (chartInfoPulledForSaving_For_Load[i].chartName == chartName)
                    {
                        MessageBox.Show("Chart with same name already present. Could not load the template file");
                        return;

                    }
                }


                InsertForSavingConfiguration_For_Load(databaseFileName1, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tablefluidinfo);


                // MessageBox.Show("InsertioncOMPLETE");
                //--Pulling data and plotting values--
                //DataGridView_Show_Data();
                //Now reloading the data 

               // PullDataAndPlot();

            }
        }


        public bool TableExistOrNotCheckTemplateFile(string DB_String_path)
        {

            bool returnValue = false;
            string connectionString = DB_String_path;// @"Data Source=" + path + ";Version=3;";

            //string sql_for_chart_detail = "SELECT * From  " + chartTableName;
            string dbCheckSql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'signature_table'";

            // MessageBox.Show("inside sqlite path =" + path);         
            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {

                //MessageBox.Show("BHIRA");
                //conx.Close();
                conName.Open();
                using (SQLiteCommand cmdx = new SQLiteCommand(dbCheckSql, conName))
                {
                    SQLiteDataReader reader = cmdx.ExecuteReader();
                    while (reader.Read())
                    {

                        returnValue = true;//Present 

                        // reader.Close();
                    }
                }

            }


            return returnValue;
        }


        public class TemperaturePressureSourceInfo
        {
            public string chartID { get; set; }
            public string nodeID { get; set; }
            public string TemepratureSoureString { get; set; }
            public string PressureSourceString { get; set; }
        }
        public List<TemperaturePressureSourceInfo> listTempPressureSourceInfo = new List<TemperaturePressureSourceInfo>();

        public void PullDataForTemperaturePressureSource(string nodeID)
        {

            listTempPressureSourceInfo.Clear();
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    listTempPressureSourceInfo.Add(new TemperaturePressureSourceInfo
                    {
                        chartID = reader["chartID"].ToString(),
                        nodeID = nodeID,
                        TemepratureSoureString = reader["TemperatureSourceInfo"].ToString(),
                        PressureSourceString = reader["PressureSourceInfo"].ToString(),

                    });

                }

            }//Close of using statement
        }


        //============================This one is for Edit Node and line section=========//

        /// <summary>
        /// Updating node info in db
        /// </summary>
        /// <param name="buildingName"></param>
        /// <param name="id"></param>
        /// <param name="xVal"></param>
        /// <param name="yVal"></param>
        /// <param name="temperature_source"></param>
        /// <param name="pressure_source"></param>
        /// <param name="name"></param>
        /// <param name="colorValue"></param>
        /// <param name="nodeSizeValue"></param>
       
        public void UpdateDataForNodeInfoInEditNodeSection(string buildingName, string id, double xVal, double yVal, string temperature_source, string pressure_source, string name, Color colorValue, int nodeSizeValue)
        {

            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@tempsource,  pressure_source=@pressuresource, name=@name, colorValue=@colorVal, nodeSize=@node_size_value where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                // command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@tempsource", temperature_source);
                command.Parameters.AddWithValue("@pressuresource",pressure_source);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
               // command.Parameters.AddWithValue("@airflow", airFlow);
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }
        }

        public void UpdateNodeInfoToDBFromTemperatureDeviceSource(string id, double xVal, double yVal, string TemperatureSource, string pressureSource, string name, Color colorValue, int nodeSizeValue)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";


            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@tempSource,pressure_source=@pressureSource ,name=@name,  colorValue=@colorVal, nodeSize=@node_size_value ,lastUpdatedDate= @date  where nodeID  =@id";
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@tempSource,pressure_source=@pressureSource ,name=@name,  colorValue=@colorVal, nodeSize=@node_size_value ,lastUpdatedXValue= @date,lastUpdatedYValue= @dateY  where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                // command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@tempSource", TemperatureSource);
                command.Parameters.AddWithValue("@pressureSource", pressureSource);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
               // command.Parameters.AddWithValue("@airflow", airFlow.ToString());
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@date", DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo)); //This is the date presented at what time the update  was performed
                command.Parameters.AddWithValue("@dateY", DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn


        public void InsertUpdateFromTemperatureDeviceSelectionToDBOnlyTempertureUpdate(string nodeid, string temp_deviceInstanceID, string temp_ip, string temp_param1id, string temp_param1info, string temp_param1type)
        {

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 

            //checking if the data is present or not
            SQLiteDataReader reader = null;
            string queryString = "SELECT *  from " + tableNameDevice + " WHERE nodeID = @id";

            bool flag = false;
            using (SQLiteConnection cxn = new SQLiteConnection(connString))
            {
                cxn.Open();
                SQLiteCommand command = new SQLiteCommand(queryString, cxn);

                command.Parameters.AddWithValue("@id", nodeid);//This is the group id that is used to identify each node
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    if (reader["nodeID"].ToString() != "")
                    {
                        flag = true;
                    }
                }   //Close of while

            } //Close of using



            if (flag == false)
            {
                //insert the value 

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Insert the node value flag = false wala and id  = "+nodeid);
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID_for_param1,device_instanceID_for_param2,IP_for_param1,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceIDparam1,@deviceInstanceParam2,@IP_param1,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceIDparam1", temp_deviceInstanceID);
                    command.Parameters.AddWithValue("@deviceInstanceParam2", "");//Insert empty value

                    command.Parameters.AddWithValue("@IP_param1", temp_ip);
                    command.Parameters.AddWithValue("@IP_param2", "");//Insert empty value

                    command.Parameters.AddWithValue("@param1", temp_param1id);
                    command.Parameters.AddWithValue("@param2", "");
                    command.Parameters.AddWithValue("@param1info", temp_param1info);
                    command.Parameters.AddWithValue("@param2info", "");
                    command.Parameters.AddWithValue("@param1_iden_type", temp_param1type);
                    command.Parameters.AddWithValue("@param2_iden_type", "");
                    command.ExecuteNonQuery();
                }
            }
            else
            {
                //Update the value
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Update mechanism the node value flag = true  wala,id = "+nodeid);

                    string sql_string = "UPDATE  " + tableNameDevice + "  SET device_instanceID_for_param1 = @instanceID,IP_for_param1 =@IP ,param1ID = @param1 ,param1_info = @param1info,param1_identifier_type= @param1_iden_type  where nodeID = @id ";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", temp_deviceInstanceID);
                    command.Parameters.AddWithValue("@IP", temp_ip);
                    command.Parameters.AddWithValue("@param1", temp_param1id);
                    // command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param1info", temp_param1info);
                    //  command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", temp_param1type);
                    // command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }

            }


        }

        public void InsertValueOfTemperatureHumiditySoure(string nodeID, string TemperatureSource, string pressureSource)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableForTempHum + "(NodeID,chartID,TemperatureSourceInfo,PressureSourceInfo) VALUES(@id,@chartID,@t,@h)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@id", nodeID);
                command.Parameters.AddWithValue("@chartID", chartDetailList[indexForWhichChartIsSelected].chartID);
                command.Parameters.AddWithValue("@t", TemperatureSource);
                command.Parameters.AddWithValue("@h", pressureSource);
                command.ExecuteNonQuery();
            }
        }

        public void UpdateOrInsertHumiditySourceInfo(string nodeID, string pressureSource)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";


            bool dataPresent = false;
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    dataPresent = true;
                }

            }

            //Data is present then update else insert
            if (dataPresent == true)
            {
                DB_HumiditySourceUpdate(nodeID, pressureSource);
            }
            else
            {
                InsertValueOfTemperatureHumiditySoure(nodeID, "", pressureSource);
            }


        }



        /// <summary>
        /// Function helps to update the humidity source value into the database
        /// </summary>
        /// <param name="nodeID"></param>
        /// <param name="pressureSourceValue"></param>
        public void DB_HumiditySourceUpdate(string nodeID, string pressureSourceValue)
        {
            /*
           //This function helps to update the temperature source detail such as 1-VAR1, 2-var2 etc.
           or even label name from alex db
            */


            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";// "tbl_" ++"_node_value";
                                                                                                                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                                                                                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set PressureSourceInfo = @status    where NodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", pressureSourceValue);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();
            }//Close of using          

        }
        public void DB_TemperatureSourceUpdate(string nodeID, string TemperatureSourceValue)
        {
            /*
           //This function helps to update the temperature source detail such as 1-VAR1, 2-var2 etc.
           or even label name from alex db
            */


            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set TemperatureSourceInfo = @status    where NodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", TemperatureSourceValue);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using          

        }

        public void UpdateOrInsertTemperatureSourceInfo(string nodeID, string TemperaturSourceInfo)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperaturePressureSourceInfo";


            bool dataPresent = false;
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    dataPresent = true;
                }

            }

            //Data is present then update else insert
            if (dataPresent == true)
            {
                DB_TemperatureSourceUpdate(nodeID, TemperaturSourceInfo);
            }
            else
            {
                InsertValueOfTemperatureHumiditySoure(nodeID, TemperaturSourceInfo, "");
            }


        }

        public void InsertUpdateFromHumidityDeviceSelectionToDBOnlyTempertureUpdate(string nodeid, string Humidity_deviceInstanceID, string Humidity_ip, string Humidity_param1id, string Humidity_param1info, string Humidity_param1type)
        {
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 

            //checking if the data is present or not
            SQLiteDataReader reader = null;
            string queryString = "SELECT *  from " + tableNameDevice + " WHERE nodeID = @id";

            bool flag = false;
            using (SQLiteConnection cxn = new SQLiteConnection(connString))
            {
                cxn.Open();
                SQLiteCommand command = new SQLiteCommand(queryString, cxn);

                command.Parameters.AddWithValue("@id", nodeid);//This is the group id that is used to identify each node
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    if (reader["nodeID"].ToString() != "")
                    {
                        flag = true;
                    }
                }   //Close of while

            } //Close of using



            if (flag == false)
            {
                //insert the value 

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Insert the node value flag = false wala and id  = "+nodeid);
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID_for_param1,device_instanceID_for_param2,IP_for_param1,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceIDparam1,@deviceInstanceParam2,@IP_param1,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceIDparam1", "");
                    command.Parameters.AddWithValue("@deviceInstanceParam2", Humidity_deviceInstanceID);//Insert empty value

                    command.Parameters.AddWithValue("@IP_param1", "");
                    command.Parameters.AddWithValue("@IP_param2", Humidity_ip);//Insert empty value

                    command.Parameters.AddWithValue("@param1", "");
                    command.Parameters.AddWithValue("@param2", Humidity_param1id);
                    command.Parameters.AddWithValue("@param1info", "");
                    command.Parameters.AddWithValue("@param2info", Humidity_param1info);
                    command.Parameters.AddWithValue("@param1_iden_type", "");
                    command.Parameters.AddWithValue("@param2_iden_type", Humidity_param1type);
                    command.ExecuteNonQuery();
                }
            }
            else
            {
                //Update the value
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Update mechanism the node value flag = true  wala,id = "+nodeid);

                    string sql_string = "UPDATE  " + tableNameDevice + "  SET device_instanceID_for_param2 = @instanceID,IP_for_param2 =@IP ,param2ID = @param2 ,param2_info = @param2info,param2_identifier_type= @param2_iden_type  where nodeID = @id ";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", Humidity_deviceInstanceID);
                    command.Parameters.AddWithValue("@IP", Humidity_ip);
                    command.Parameters.AddWithValue("@param2", Humidity_param1id);
                    // command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param2info", Humidity_param1info);
                    //  command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param2_iden_type", Humidity_param1type);
                    // command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }

            }


        }

        //=============================End of edit node and line section===============//


        //-======================Backgroundworker Task -==========================//
        public class device_info_class
        {
            public string nodeID { get; set; }
            public string device_instance_id_for_param1 { get; set; }
            public string device_IP_for_param1 { get; set; }

            public string device_instance_id_for_param2 { get; set; }
            public string device_IP_for_param2 { get; set; }
            public string param1_id { get; set; }
            public string param2_id { get; set; }
            public string param1_info { get; set; }
            public string param2_info { get; set; }

            public string param1_identifier_type { get; set; }
            public string param2_identifier_type { get; set; }

        }
       public List<device_info_class> device_info_list = new List<device_info_class>();//This list will store the values of device info

        /// <summary>
        /// This get the device info values form the database 
        /// </summary>
        /// <param name="nodeID1">the node id selected to search for </param>
        public void ReadDeviceInfoForNode(string nodeID1)
        {
            device_info_list.Clear();//Resetting list
            //string deviceTableName = "tbl_"+
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                                                                                                             //lets get the id values...
                                                                                                             //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            string sql1 = "SELECT * From  " + tableNameDevice + "  where nodeID='" + nodeID1 + "'";
            //  MessageBox.Show("sql1=" + sql1);

            //(nodeID,device_instanceID,IP,param1ID_X,param2ID_Y) VALUES(@id,@instanceID,@IP,@param1,@param2)";

            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);

                thisConnection.Open();

                SQLiteDataReader reader = cmd.ExecuteReader();

                //    MessageBox.Show("cmd error= " + cmd.CommandText);

                while (reader.Read())
                {
                    // AL_nodeId.Add(reader["nodeID"].ToString());
                    device_info_list.Add(new device_info_class
                    {
                        nodeID = nodeID1,
                        device_instance_id_for_param1 = reader["device_instanceID_for_param1"].ToString(),
                        device_IP_for_param1 = reader["IP_for_param1"].ToString(),
                        device_instance_id_for_param2 = reader["device_instanceID_for_param2"].ToString(),
                        device_IP_for_param2 = reader["IP_for_param2"].ToString(),
                        param1_id = reader["param1ID"].ToString(),
                        param2_id = reader["param2ID"].ToString(),
                        param1_info = reader["param1_info"].ToString(),
                        param2_info = reader["param2_info"].ToString(),
                        param1_identifier_type = reader["param1_identifier_type"].ToString(),
                        param2_identifier_type = reader["param2_identifier_type"].ToString()

                    });
                }


            }

        }

        List<bool> dataCheckList = new List<bool>();

        public bool CheckDeviceOnlineOffline(int deviceID, int Parent_SerialNum)
        {
            //Resetting
            dataCheckList.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //   string databaseFile = databasePath + @"\Database\Buildings\Default_Building\Default_Building.mdb";
            //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+ databaseFile+ @";Persist Security Info=True ";
            //C:\Program Files (x86)\T3000\Database\Buildings\Default_Building
            //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

            string sql = "select * from ALL_NODE  where Object_Instance = '" + deviceID + "' and Parent_SerialNum = '" + Parent_SerialNum + "'";
            bool status = false;
            //MessageBox.Show("Outside the path,"+buildingSelected.Count );
            if (buildingSelectedInT3000.Count > 0)
            {
                /*
                This path is dynamic path we calculated based on this 
                <installed directory of T3000 (not psycho)>\... where ... = Database\..value returned by files
                */
                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                //string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].building_path;
                string againDbPath = @"Data Source=" + newPath + "" + buildingSelectedInT3000[0].building_path;
                // MessageBox.Show("Path = \n" + againDbPath);
                try
                {
                    using (SQLiteConnection conn = new SQLiteConnection(againDbPath))
                    {
                        using (SQLiteCommand cmd = new SQLiteCommand(sql, conn))
                        {
                            conn.Open();
                            SQLiteDataReader reader = cmd.ExecuteReader();
                            while (reader.Read())
                            {
                                dataCheckList.Add((bool)reader["Online_Status"]);
                            }

                        }

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
                //MessageBox.Show( "datacheckcount= " + dataCheckList.Count + " Status = " + status);
            }
            return status;
        }
        public void UpdateNodeInfoToDBForTemeperatureFromHardware(string id, double xVal)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";


            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal , lastUpdatedDate=@date   where nodeID  =@id";
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal , lastUpdatedXValue=@date   where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                //command.Parameters.AddWithValue("@date", DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
                command.Parameters.AddWithValue("@date", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn
        public void UpdateNodeInfoToDBForPressureFromHardware(string id, double yVal)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //string sql_string = "UPDATE " + tableName + "   set   yValue=@yVal, lastUpdatedDate=@date  where nodeID  =@id";
                string sql_string = "UPDATE " + tableName + "   set   yValue=@yVal, lastUpdatedYValue=@date  where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                // command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());

                //command.Parameters.AddWithValue("@date", DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
                command.Parameters.AddWithValue("@date", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn


        //====================End of background worker task====================//


        //===================For TrashBox db opartion========================//

        //This list holds the detail of the chart to be deleted
        public List<DataTypeForNode> deleteNodeDetailList = new List<DataTypeForNode>();

        //Read node Values
        /// <summary>
        ///    help in reading all the node info for a particular chart
        /// </summary>
        /// <param name="chartID">char id</param>
        public void ReadNodeInfoToDelete(string chartNodeID,string buildingName)
        {
            deleteNodeDetailList.Clear();
            string tableName = "tbl_" + buildingName + "_node_value";// "tbl_" ++"_node_value";
                                                                                             //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                                                             //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from  " + tableName + "  where chart_respective_nodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartNodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    deleteNodeDetailList.Add(new DataTypeForNode
                    {
                     
                        ID = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        temperature_source = reader["temperature_source"].ToString(),
                        pressure_source = reader["pressure_source"].ToString(),
                        name = reader["name"].ToString(),

                        // label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        // showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString()),
                        //airFlow = int.Parse(reader["airFlow"].ToString()),
                        // lastUpdatedDate = reader["lastUpdatedDate"].ToString()
                        lastUpdatedXValue = reader["lastUpdatedXValue"].ToString(),
                        lastUpdatedYValue = reader["lastUpdatedYValue"].ToString()

                    });
                }
            }//Close of using 
        }
        public void DeleteNodeDeviceInfo(string nodeID, string buildingName)
        {
            string tableName = "tbl_" + buildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
                                                                                                       //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                                                                       //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using 
        }



        //Same for the line 
        public void DeleteLine(string lineID,string buildingName)
        {
            string tableName = "tbl_" + buildingName + "_line_value";// "tbl_" ++"_node_value";
                                                                                             //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                                                                                             //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chart_respective_lineID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", lineID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();


            }//Close of using 
        }



        public void DeleteNode(string nodeID,string buildingName)
        {
            string tableName = "tbl_" + buildingName + "_node_value";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chart_respective_nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();


            }//Close of using 
        }




        public void DeleteChart(string chartID,string buildingName)
        {
            /*
            Steps :1. Frist read line info,nodeinfo,comfortzone info for particular chart id
                    2.Then delete line value using lineID,
                    3.Delete node values using nodeID
                    4.Delete comfortzone value using chartid
                    5.delete the chart info using chart id value
            */

            //string chartID =   
            string tableName = "tbl_" + buildingName + "_chart_detail";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using

        }

        public void DeleteFluidInfoForChart(string chartID, string buildingName)
        {
            /*
            Steps :1. Frist read line info,nodeinfo,comfortzone info for particular chart id
                    2.Then delete line value using lineID,
                    3.Delete node values using nodeID
                    4.Delete comfortzone value using chartid
                    5.delete the chart info using chart id value
            */

            //string chartID =   
            string tableName = "tbl_" + buildingName + "_chart_fluid_info";// "tbl_" ++"_node_value";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using

        }

        public void updateValueInDatabaseToTrue(string buildingName, string chartID)
        {
            //--Now lets make the check state to be true
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "UPDATE " + tableForChartDetail + " SET  enableChartStatus ='true' where chartID = '" + chartID + "'";


                SQLiteCommand command = new SQLiteCommand(connection);
                command.CommandText = sql;
              
                command.ExecuteNonQuery();
              
            }

        }

        public List<chartDetailDT> chartDetailListForDissabledValue = new List<chartDetailDT>();//This is used for storing the chart detail ids

        public void PullChartListForTrashBox(string buildingName)
        {

            chartDetailListForDissabledValue.Clear();//resetting the chart list value..
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            //  MessageBox.Show("connection string " + connString);

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "select * from " + tableForChartDetail + " where enableChartStatus ='false' ";

                SQLiteCommand command = new SQLiteCommand(sql, connection);

                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    chartDetailListForDissabledValue.Add(new chartDetailDT
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

        }  //Close of the function
        //====================End of trash box db oparation================//
        
    }
}
