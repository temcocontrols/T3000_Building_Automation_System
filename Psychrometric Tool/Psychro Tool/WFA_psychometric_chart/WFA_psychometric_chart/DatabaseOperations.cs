using System;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Reflection;

namespace WFA_psychometric_chart
{
    /// <summary>
    /// Perfoms the database operations 
    /// used by form1_main
    /// </summary>
    class DatabaseOperations:Form1_main
    {

        /// <summary>
        /// Insert the data to SQLite db 
        /// stores values of nodes
        /// </summary>
        /// <param name="id">node id </param>
        /// <param name="xVal">x axis value</param>
        /// <param name="yVal">y axis value</param>
        /// <param name="temperature_source">Temperature source[Manual/Web/Device]</param>
        /// <param name="humidity_source">Humidity source[Manual/Web/Device]</param>
        /// <param name="name">Node name</param>
        /// <param name="colorValue">Node color</param>
        /// <param name="nodeSizeValue">Node Size</param>
        public void InsertNodeInfoToDBWithoutDeviceInfo(string buildingName,string chart_respective_nodeID,string id, double xVal, double yVal, string temperature_source, string humidity_source, string name,Color colorValue, int nodeSizeValue,string airFlow)
        {
            //This is the name of the table that stores the node information...
            if (buildingName == "")
            {
                return;
            }

            //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  
            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "insert into " + tableName + "(chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,humidity_source,name,colorValue,nodeSize,airFlow,lastUpdatedXValue,lastUpdatedYValue) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@humidity_source,@name,@colorVal,@node_size_value,@airFlow,@lastUpdateDate,@lastUpdateDate2)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chart_respective_nodeID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@temperature_source", temperature_source);
                command.Parameters.AddWithValue("@humidity_source", humidity_source);
                command.Parameters.AddWithValue("@name", name);                
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));              
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@airFlow", airFlow);
                command.Parameters.AddWithValue("@lastUpdateDate", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));//Lets store the date time in the node values
                command.Parameters.AddWithValue("@lastUpdateDate2", DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));
                command.ExecuteNonQuery();
            }
  
        }//--close of insertnodeinfotodb fxn

           /// <summary>
           /// Helps to update the database values
           /// </summary>
           /// <param name="id">Node id</param>
           /// <param name="xVal">x value</param>
           /// <param name="yVal">y value</param>
           /// <param name="temperature_source">temperature source</param>
           /// <param name="humidity_source">humidity source</param>
           /// <param name="name">node name</param>
           /// <param name="colorValue">node color</param>
           /// <param name="nodeSizeValue">node size</param>
           /// <param name="airflow">air flow amount</param>
           /// <param name="lastUpdatedDateValue">date current updated</param>
        public void UpdateNodeInfoToDBWithNewParameters(string buildingName,string id, double xVal, double yVal, string temperature_source, string humidity_source,string name,  Color colorValue,  int nodeSizeValue,int airflow)
        {     //CurrentSelectedBuilding
              //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@temp_source, humidity_source=@hum_source,name=@name, colorValue=@colorVal, nodeSize=@node_size_value,airFlow=@airflowValue where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@temp_source", temperature_source);
                command.Parameters.AddWithValue("@hum_source", humidity_source);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@airflowValue", airflow);
               // command.Parameters.AddWithValue("@lastUpdatedDate", lastUpdatedDateValue);
                command.Parameters.AddWithValue("@id", id);
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn




        public void InsertMixNodeInfo(string buildingName, string chartID,string node_id, string prevNodeID, string nextNodeID)
        {
            //This is the name of the table that stores the node information...
            if (buildingName == "")
            {
                return;
            }

            //string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  
            string tableName = "tbl_" + buildingName + "_mix_node_info";//currentNodeTableFromDB;  
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "insert into " + tableName + "(nodeID,chartID,previousNodeID,nextNodeID) VALUES(@nodeid,@chart_id,@prevnodeid,@nextnodeid)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@nodeid", node_id);
                command.Parameters.AddWithValue("@chart_id", chartID);
                command.Parameters.AddWithValue("@prevnodeid", prevNodeID);
                command.Parameters.AddWithValue("@nextnodeid", nextNodeID);         
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn






    }//Close of  Class
}
