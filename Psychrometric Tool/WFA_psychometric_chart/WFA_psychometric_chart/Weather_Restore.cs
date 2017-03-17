using System.Data;
using System.Data.SQLite;
using System.IO;
using System.Reflection;

using System.Collections.Generic;

namespace WFA_psychometric_chart
{
    /// <summary>
    /// This class has all the functions and variables for restore function in the weather selection part
    /// Reading data ,writing data, and clicking events will be done here
    /// </summary>
    class Weather_Restore
    {


        /// <summary>
        /// Updates controller inforamtion to db  "tbl_" + buildingName + "_Weather_Controller_Restor_Info"
        /// </summary>
        /// <param name="buildingName">Building Name</param>
        /// <param name="controllerName">Controller Info</param>
        /// <param name="paramTempName">Temperature Info</param>
        /// <param name="paramHumName">Humidity Info</param>
        public void UpdateControllerInfo(string buildingName, string controllerName, string paramTempName,string paramHumName,string TempValue,string HumValue)
        {
            
            string tableName = "tbl_" + buildingName + "_Weather_Controller_Restor_Info";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set ControllerNameInfo = @cont, TemperatureParameterInfo = @temp, HumidityParameterInfo= @hum,TempValue=@TV,HumValue = @HV    where BuildingName = @name";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@cont", controllerName);
                command.Parameters.AddWithValue("@temp", paramTempName);
                command.Parameters.AddWithValue("@hum", paramHumName);
                command.Parameters.AddWithValue("@name", buildingName);
                command.Parameters.AddWithValue("@TV", TempValue);
                command.Parameters.AddWithValue("@HV", HumValue);
                reader = command.ExecuteReader();

            }//Close of using          

        }

        public void UpdateOrInsertControllerInfo(string buildingName, string controllerName, string paramTempName, string paramHumName,string TempValue,string HumValue)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Controller_Restor_Info";


            bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
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
                // DB_TemperatureSourceUpdate(nodeID, TemperaturSourceInfo);
                UpdateControllerInfo(buildingName, controllerName, paramTempName, paramHumName, TempValue, HumValue);
            }
            else
            {
                //InsertValueOfTemperatureHumiditySoure(nodeID, TemperaturSourceInfo, "");
                InsertControllerInfo(buildingName, controllerName, paramTempName, paramHumName,TempValue,HumValue);
            }


        }

        public void InsertControllerInfo(string buildingName, string controllerName, string paramTempName, string paramHumName,string TempValue,string HumValue)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Controller_Restor_Info";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableForTempHum + "(BuildingName,ControllerNameInfo,TemperatureParameterInfo,HumidityParameterInfo,TempValue,HumValue) VALUES(@bname,@con_name,@temp,@hum,@TV,@HV)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@bname", buildingName);
                command.Parameters.AddWithValue("@con_name", controllerName);
                command.Parameters.AddWithValue("@temp", paramTempName);
                command.Parameters.AddWithValue("@hum", paramHumName);
                command.Parameters.AddWithValue("@TV", TempValue);
                command.Parameters.AddWithValue("@HV", HumValue);
                command.ExecuteNonQuery();
            }
        }

        public class ControllerInfoDT
        {
            public string buildingName { get; set; }
            public string ControllerInfo { get; set;}
            public string TempParamInfo { get; set;}
            public string HumParamInfo { get; set;}
            public string TempValue { get; set; }
            public string HumValue { get; set; }

        }
        public List<ControllerInfoDT> ListControllerInfo = new List<ControllerInfoDT>();
        public void ReadControllerDataForRestore(string buildingName)
        {

            ListControllerInfo.Clear();
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Controller_Restor_Info";

            //bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    // dataPresent = true;
                    ListControllerInfo.Add(new ControllerInfoDT
                    {
                        buildingName = buildingName,
                        ControllerInfo = reader["ControllerNameInfo"].ToString(),
                        TempParamInfo = reader["TemperatureParameterInfo"].ToString(),
                        HumParamInfo = reader["HumidityParameterInfo"].ToString(),
                        TempValue = reader["TempValue"].ToString(),
                        HumValue = reader["HumValue"].ToString()
                    });
                }

            }
        }





        //=============================Lets make the same function for web function in Weather form===========================//

        public void UpdateWebInfo(string buildingName, string enableOrDissable, string stationInfo)
        {

            string tableName = "tbl_" + buildingName + "_Weather_Web_Restor_Info";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set Enable_dissable_info = @enableDissable, stationInfo = @stationInfo1 where BuildingName = @name";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@enableDissable", enableOrDissable);
                command.Parameters.AddWithValue("@stationInfo1", stationInfo);
                command.Parameters.AddWithValue("@name", buildingName);
                reader = command.ExecuteReader();

            }//Close of using          

        }

        public void UpdateOrInsertWebInfo(string buildingName, string enableOrDissable, string stationInfo)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Web_Restor_Info";


            bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
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
                // DB_TemperatureSourceUpdate(nodeID, TemperaturSourceInfo);
                UpdateWebInfo(buildingName, enableOrDissable, stationInfo);
            }
            else
            {
                //InsertValueOfTemperatureHumiditySoure(nodeID, TemperaturSourceInfo, "");
                InsertWebInfo(buildingName, enableOrDissable, stationInfo);
            }


        }

        public void InsertWebInfo(string buildingName, string enableOrDissable, string stationInfo)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Web_Restor_Info";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableForTempHum + "(BuildingName,Enable_dissable_info,stationInfo) VALUES(@bname,@con_name,@temp)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@bname", buildingName);
                command.Parameters.AddWithValue("@con_name", enableOrDissable);
                command.Parameters.AddWithValue("@temp", stationInfo);                
                command.ExecuteNonQuery();
            }
        }

        public class WebInfoDT
        {
            public string buildingName { get; set; }
            public string enableDissable { get; set; }
            public string stationInfo { get; set; }
         
        }
        public List<WebInfoDT> ListWebInfo = new List<WebInfoDT>();
        public void ReadWebDataForRestore(string buildingName)
        {

            ListWebInfo.Clear();
            string tableForTempHum = "tbl_" + buildingName + "_Weather_Web_Restor_Info";
            
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    ListWebInfo.Add(new WebInfoDT
                    {
                        buildingName = buildingName,
                        enableDissable = reader["Enable_dissable_info"].ToString(),
                        stationInfo = reader["stationInfo"].ToString()
                     
                    });
                }

            }
        }





        //===================================End fo the web functions=========================================================//

        //================================For humidity self calibration function =============================///





        public void UpdateHumSelfCalibrationInfo(string buildingName, string enableOrDissable, string max_adjustment)
        {

            string tableName = "tbl_" + buildingName + "_Weather_HumSelfCalibration_Restor_Info";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set Enable_dissable_info = @enableDissable, max_adjustment_per_day = @stationInfo1 where BuildingName = @name";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@enableDissable", enableOrDissable);
                command.Parameters.AddWithValue("@stationInfo1", max_adjustment);
                command.Parameters.AddWithValue("@name", buildingName);
                reader = command.ExecuteReader();

            }//Close of using          

        }

        public void UpdateOrInsertHumSelfCalibrationInfo(string buildingName, string enableOrDissable, string MAX_Adjustment)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_HumSelfCalibration_Restor_Info";


            bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
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
                // DB_TemperatureSourceUpdate(nodeID, TemperaturSourceInfo);
                UpdateHumSelfCalibrationInfo(buildingName, enableOrDissable, MAX_Adjustment);
            }
            else
            {
                //InsertValueOfTemperatureHumiditySoure(nodeID, TemperaturSourceInfo, "");
                InsertHumSelfCalibrationInfo(buildingName, enableOrDissable, MAX_Adjustment);
            }


        }

        public void InsertHumSelfCalibrationInfo(string buildingName, string enableOrDissable, string max_adjustment)
        {
            string tableForTempHum = "tbl_" + buildingName + "_Weather_HumSelfCalibration_Restor_Info";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableForTempHum + "(BuildingName,Enable_dissable_info,max_adjustment_per_day) VALUES(@bname,@con_name,@temp)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@bname", buildingName);
                command.Parameters.AddWithValue("@con_name", enableOrDissable);
                command.Parameters.AddWithValue("@temp", max_adjustment);
                command.ExecuteNonQuery();
            }
        }

        public class HumSelfCalibrationInfoDT
        {
            public string buildingName { get; set; }
            public string enableDissable { get; set; }
            public string MAX_Adjustment_Value { get; set; }

        }
        public List<HumSelfCalibrationInfoDT> ListHumSelfCalibrationInfo = new List<HumSelfCalibrationInfoDT>();
        public void ReadHumSelfCalibrationInfoDTDataForRestore(string buildingName)
        {

            ListHumSelfCalibrationInfo.Clear();
            string tableForTempHum = "tbl_" + buildingName + "_Weather_HumSelfCalibration_Restor_Info";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where BuildingName = '" + buildingName + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    ListHumSelfCalibrationInfo.Add(new HumSelfCalibrationInfoDT
                    {
                        buildingName = buildingName,
                        enableDissable = reader["Enable_dissable_info"].ToString(),
                        MAX_Adjustment_Value = reader["max_adjustment_per_day"].ToString()

                    });
                }

            }
        }












        //==============================End of humidity self calibration=======================================//











    }
}
