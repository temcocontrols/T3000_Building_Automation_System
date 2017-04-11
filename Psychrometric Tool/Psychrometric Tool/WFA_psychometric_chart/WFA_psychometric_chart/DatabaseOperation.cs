using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace WFA_psychometric_chart
{
    /// <summary>
    /// Does all the database operations related to Psychometric project
    /// </summary>
    class DatabaseOperation
    {

          public void sqlite_database_creation_For_UpdateCondition(string buildingNameSelected)
        {

            //--lets do try catch
            try
            {

                ReadVersionInfo(buildingNameSelected);//Calling the function 
                if(versionValue != "")
                {
                    DateTime dt = DateTime.Parse(versionValue); 
                    if (dt < AssemblyDateGeneration.Value) //--Comparing the datetime value
                    {
                        
                        //--Operations are required
                        CreateRequiredTable(buildingNameSelected);//Those table which are not present are only created

                        ReadDataPresentOrNotThenInsertBuildingValue();
                        //After changes lets update the version info
                        UpdateVersionInfoAfterUpdating(buildingNameSelected, AssemblyDateGeneration.Value.ToShortDateString());
                    }
                }



            }
            catch (Exception ex)
            {
                //MessageBox.Show(ex.Message);
            }         
        }
        string versionValue;
        private void ReadVersionInfo(string buildingName)
        {
            string tableName = "tbl_" + buildingName + "_Database_Version";//currentNodeTableFromDB; 
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sql1 = "SELECT * From  " + tableName;
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                thisConnection.Open();
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    versionValue = reader["version"].ToString();
                }
            }//close of using

        }

        /// <summary>
        /// Updates the database version info
        /// </summary>
        /// <param name="buildingName">Building selected in T3000</param>
        /// <param name="version">version parameter</param>
        private void UpdateVersionInfoAfterUpdating(string buildingName,string version)
        {
            string tableName = "tbl_" + buildingName + "_Database_Version";//currentNodeTableFromDB; 
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sql1 = $"update {tableName} set version = {version} where count='1'";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                cmd.ExecuteNonQuery();
            }//close of using

        }

        /// <summary>
        /// Create all the tables which are not present 
        /// </summary>
        /// <param name="buildingNameSelected">Name of the building selected in T3000</param>
        private void CreateRequiredTable(string buildingNameSelected)
        {

            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";

         
            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection($"Data Source={databaseFile};Version=3;")) { 
                m_dbConnection.Open();

            string sql = "create table IF NOT EXISTS tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255) ,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";
            SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
            command.ExecuteNonQuery();

            //--next table historical data:tbl_historical_data
            string sql2 = "create table IF NOT EXISTS tbl_historical_data (ID INTEGER,date_current datetime,hour_current int,minute_current int,distance_from_building varchar(255),temperature varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
            SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
            command2.ExecuteNonQuery();


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
            string tbl_comfortzoneSetting = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_comfort_zone_setting  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
            string tbl_chart_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_detail(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
            string tbl_comfort_zone_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_comfort_zone_detail  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
            string tbl_device_info_for_node = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_device_info_for_node  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
            string tbl_input_storage_form_T3000 = " CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_input_storage_from_T3000  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
            string tbl_line_value = " CREATE TABLE  IF NOT EXISTS tbl_" + buildingNameSelected + "_line_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
            string tbl_mix_node_info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_mix_node_info  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
            string tbl_node_data_related_T3000 = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_data_related_T3000  (count INTEGER PRIMARY KEY AUTOINCREMENT, nodeID varchar(255), param1_panelID varchar(255), param1_inputIndex varchar(255), param2_panelID varchar(255), param2_inputIndex varchar(255)) ";
            string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
            string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperatureHumiditySourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),HumiditySourceInfo varchar(255))";
            string tbl_Weather_Controller_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
            string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
            string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";
            string tbl_database_version = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Database_Version(count INTEGER PRIMARY KEY AUTOINCREMENT,version varchar(255))";


            //now execute the query
            SQLiteCommand cm1 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
            cm1.ExecuteNonQuery();

            SQLiteCommand cm2 = new SQLiteCommand(tbl_chart_detail, m_dbConnection);
            cm2.ExecuteNonQuery();

            SQLiteCommand cm3 = new SQLiteCommand(tbl_comfort_zone_detail, m_dbConnection);
            cm3.ExecuteNonQuery();

            SQLiteCommand cm4 = new SQLiteCommand(tbl_device_info_for_node, m_dbConnection);
            cm4.ExecuteNonQuery();

            SQLiteCommand cm5 = new SQLiteCommand(tbl_input_storage_form_T3000, m_dbConnection);
            cm5.ExecuteNonQuery();

            SQLiteCommand cm6 = new SQLiteCommand(tbl_line_value, m_dbConnection);
            cm6.ExecuteNonQuery();

            SQLiteCommand cm7 = new SQLiteCommand(tbl_mix_node_info, m_dbConnection);
            cm7.ExecuteNonQuery();

            SQLiteCommand cm8 = new SQLiteCommand(tbl_node_data_related_T3000, m_dbConnection);
            cm8.ExecuteNonQuery();

            SQLiteCommand cm9 = new SQLiteCommand(tbl_node_value, m_dbConnection);
            cm9.ExecuteNonQuery();

            SQLiteCommand cm10 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
            cm10.ExecuteNonQuery();

            SQLiteCommand cm11 = new SQLiteCommand(tbl_Weather_Controller_Restor_Info, m_dbConnection);
            cm11.ExecuteNonQuery();

            SQLiteCommand cm12 = new SQLiteCommand(tbl_Weather_Web_Restor_Info, m_dbConnection);
            cm12.ExecuteNonQuery();

            SQLiteCommand cm13 = new SQLiteCommand(tbl_Weather_HumSelfCalibration_Restor_Info, m_dbConnection);
            cm13.ExecuteNonQuery();


            }//--Close of using statement 
        }//--Close of method

        /// <summary>
        /// it checks weather the data is present or not and if not present
        /// it inserts new data
        /// </summary>
        private void ReadDataPresentOrNotThenInsertBuildingValue()
        {
            if(ReadDataForBuildingSelected() == false)
            {
                //--If there is no data selected we need to insert the new data
                string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";
                Form1_main f1 = new Form1_main();
                //--new database file 
                //SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                using (SQLiteConnection m_dbConnection = new SQLiteConnection($"Data Source={databaseFile};Version=3;")) { 
                    m_dbConnection.Open();
                string sql_stringx = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                SQLiteCommand command9 = new SQLiteCommand(sql_stringx, m_dbConnection);
                command9.CommandType = CommandType.Text;
                command9.Parameters.AddWithValue("@sel", "1");
                command9.Parameters.AddWithValue("@con", f1.BuildingSelected[0].country);
                command9.Parameters.AddWithValue("@state", f1.BuildingSelected[0].state);
                command9.Parameters.AddWithValue("@city", f1.BuildingSelected[0].city);
                command9.Parameters.AddWithValue("@stre", f1.BuildingSelected[0].street);
                command9.Parameters.AddWithValue("@lng", f1.BuildingSelected[0].longitude);
                command9.Parameters.AddWithValue("@lat", f1.BuildingSelected[0].latitude);
                command9.Parameters.AddWithValue("@elev", f1.BuildingSelected[0].elevation);
                command9.Parameters.AddWithValue("@bname", f1.BuildingSelected[0].Building_Name);
                command9.Parameters.AddWithValue("@engUnit", f1.BuildingSelected[0].EngineeringUnits);
                command9.ExecuteNonQuery();
                }
            }

        }

        
        private bool ReadDataForBuildingSelected()
        {

            bool returnVal = false;
            string tableName = "tbl_building_location";//currentNodeTableFromDB; 
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            string sql1 = $"SELECT * From   { tableName } where  selection='1'";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                thisConnection.Open();
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    //versionValue = reader["version"].ToString();
                    returnVal = true;
                }
            }//close of using


            return returnVal; 
        }


    }
}
