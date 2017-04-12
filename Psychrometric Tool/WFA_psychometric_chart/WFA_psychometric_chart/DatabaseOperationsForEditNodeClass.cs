using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace WFA_psychometric_chart
{

    /// <summary>
    /// This class performs all the database operation 
    /// related to edit node values
    /// </summary>
    class DatabaseOperationsForEditNodeClass: Form1_main
    {


        /// <summary>
        /// Updating node info in db
        /// </summary>
        /// <param name="buildingName"></param>
        /// <param name="id"></param>
        /// <param name="xVal"></param>
        /// <param name="yVal"></param>
        /// <param name="temperature_source"></param>
        /// <param name="humidity_source"></param>
        /// <param name="name"></param>
        /// <param name="colorValue"></param>
        /// <param name="nodeSizeValue"></param>
        /// <param name="airFlow"></param>

        public void UpdateDataForNodeInfoInEditNodeSection(string buildingName,string id, double xVal, double yVal, string temperature_source, string humidity_source, string name, Color colorValue, int nodeSizeValue,int airFlow)
        {

            string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@tempsource,  humidity_source=@humsource, name=@name, colorValue=@colorVal, nodeSize=@node_size_value, airFlow=@airflow   where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                // command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@tempsource", temperature_source);
                command.Parameters.AddWithValue("@humsource", humidity_source);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@airflow", airFlow);            
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }                                           
        }

        //======================================end of update info for node==============================//


     //======================Helps to find the path and store it in BuildingSelected list================//

        public class SelectedBuildingDatatype
        {
            public string Main_BuildingName { get; set; }
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
            //string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
            string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db;Version=3;";
            //  MessageBox.Show("New path : " + againDbPath);
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    BuildingSelected.Add(new SelectedBuildingDatatype
                    {
                        Main_BuildingName = reader["Main_BuildingName"].ToString(),
                        Building_Name = reader["Building_Name"].ToString(),
                        Building_Path = reader["Building_Path"].ToString()
                    });
                }
            }

            //  MessageBox.Show("count = " + BuildingSelected.Count);
        }


  /////========================================Close fo path finding=========================//




    }
}
