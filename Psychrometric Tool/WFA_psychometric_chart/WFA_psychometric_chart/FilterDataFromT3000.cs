using System.Collections.Generic;
using System.Data.SQLite;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    /// <summary>
    /// class helps to filter data from alex database(i.e.T3000)
    /// </summary>
   public class FilterDataFromT3000
    {

        public class T3000DataType
        {
            public string PanelID { get; set; }
            public string InputIndex { get; set; }
            public string InputDescription { get; set; }

            public string InputAM { get; set; }
            public string InputValue { get; set; }
            public string InputUnit { get; set; }

            public string InputRange { get; set; }
            public string InputCalibration { get; set; }
            public string InputCalSign { get; set; }

            public string InputFilter { get; set; }
            public string InputDecon { get; set; }
            public string InputJumper { get; set; }
            public string InputLabel { get; set; }

        }
       public List<T3000DataType> InputFromT3000List = new List<T3000DataType>();

        /// <summary>
        ///       
        /// Read the data from alex db ..
        /// This will be loaded when first loaded values        
        /// </summary>
        /// <param name="databasePathWithFileName">DB path path to Inputables tableie. [buildingname].db path in alex db</param>
        /// <param name="TableName">name of the table i.2 inputables</param>

        public void ReadDataFromAlexDatabaseForInput(string inputTableName,string buildingPath)
        {
            //--Now lets start reading the data from alex database
            InputFromT3000List.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
            //string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            ////string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
            //// string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db;Version=3;";
            //string againDbPath = @"Data Source=" + databasePathWithFileName + ";Version=3;";

            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            //  string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
            string againDbPath = @"Data Source=" + newPath + "" + buildingPath; // BuildingSelected[0].Building_Path;


            // MessageBox.Show("aLEX DB PATH : INPUTable New path : " + againDbPath+"\n db name  = "+TableName);
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from  " + inputTableName; //Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    InputFromT3000List.Add(new T3000DataType
                    {
                        
                        PanelID = reader["PanelID"].ToString(),
                        InputIndex = reader["InputIndex"].ToString(),

                        InputDescription = reader["InputDescription"].ToString(),
                        InputAM = reader["InputAM"].ToString(),

                        InputValue = reader["InputValue"].ToString(),
                        InputUnit = reader["InputUnit"].ToString(),

                        InputRange = reader["InputRange"].ToString(),
                        InputCalibration = reader["InputCalibration"].ToString(),


                        InputCalSign = reader["InputCalSign"].ToString(),
                        InputFilter = reader["InputFilter"].ToString(),

                        InputDecon = reader["InputDecon"].ToString(),
                        InputJumper = reader["InputJumper"].ToString(),
                        InputLabel = reader["InputLabel"].ToString()

                    });



                }
            }

             //MessageBox.Show("All data  InputFromT3000List form alex db count = " + InputFromT3000List.Count);

        }

        //==Fot variables
        public class T3000DataTypeForVariable
        {
            public string var_panel { get; set; }
            public string var_index { get; set; }
            public string var_am { get; set; }

            public string var_value { get; set; }
            public string var_units { get; set; }
            public string var_label { get; set; }

        }
        public List<T3000DataTypeForVariable> variableFromT3000List = new List<T3000DataTypeForVariable>();



        public void ReadDataFromAlexDatabaseForVariable(string outputTableName, string buildingPath)
        {
            //--Now lets start reading the data from alex database
            variableFromT3000List.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
        
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            //  string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
            string againDbPath = @"Data Source=" + newPath + "" + buildingPath; // BuildingSelected[0].Building_Path;


            // MessageBox.Show("aLEX DB PATH : INPUTable New path : " + againDbPath+"\n db name  = "+TableName);
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from  " + outputTableName; //Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    variableFromT3000List.Add(new T3000DataTypeForVariable
                    {

                        var_panel = reader["var_panel"].ToString(),
                        var_index = reader["var_index"].ToString(),
                        var_am = reader["var_am"].ToString(),
                        var_value = reader["var_value"].ToString(),
                        var_units = reader["var_units"].ToString(),
                        var_label = reader["var_label"].ToString(),


                    });



                }
            }

            // MessageBox.Show("All data form alex db count = " + InputFromT3000.Count);
        }


        //====================================output form T3000 ==========================
        public class T3000DataTypeForOutput
        {
            public string out_panel { get; set; }
            public string out_index { get; set; }
            public string out_am { get; set; }
            public string out_value { get; set; }
            public string out_units { get; set; }
            public string out_range { get; set; }
            public string out_pwm_period { get; set; }
            public string out_decom { get; set; }
            public string out_label { get; set; }

        }
        public List<T3000DataTypeForOutput> outputFromT3000List= new List<T3000DataTypeForOutput>();

        /// <summary>
        ///       
        /// Read the data from alex db ..
        /// This will be loaded when first loaded values        
        /// </summary>
        /// <param name="databasePathWithFileName">DB path path to Inputables tableie. [buildingname].db path in alex db</param>
        /// <param name="TableName">name of the table i.2 inputables</param>

        public void ReadDataFromAlexDatabaseForOutput(string outputTableName, string buildingPath)
        {
            //--Now lets start reading the data from alex database
            outputFromT3000List.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
            //string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            ////string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
            //// string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db;Version=3;";
            //string againDbPath = @"Data Source=" + databasePathWithFileName + ";Version=3;";

            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            //  string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
            string againDbPath = @"Data Source=" + newPath + "" + buildingPath; // BuildingSelected[0].Building_Path;


            // MessageBox.Show("aLEX DB PATH : INPUTable New path : " + againDbPath+"\n db name  = "+TableName);
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from  " + outputTableName; //Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    outputFromT3000List.Add(new T3000DataTypeForOutput
                    {

                        out_panel = reader["out_panel"].ToString(),
                        out_index = reader["out_index"].ToString(),
                        out_am = reader["out_am"].ToString(),
                        out_value = reader["out_value"].ToString(),
                        out_units = reader["out_units"].ToString(),
                        out_range = reader["out_range"].ToString(),
                        out_pwm_period = reader["out_pwm_period"].ToString(),
                        out_decom = reader["out_decom"].ToString(),
                        out_label = reader["out_label"].ToString()
                   
                    });

                }
            }

            // MessageBox.Show("All data form alex db count = " + InputFromT3000.Count);

        }


        //--This one contains the Controller information from table ALL_NODE
        public class All_node
        {
            public string MainBuilding_Name { get; set; }
            public string Building_Name { get; set; }
            public string Serial_ID { get; set; }
            public string Floor_name { get; set; }
            public string Room_name { get; set; }
            public string Product_name { get; set; }
            public string Product_class_ID { get; set; }
            public string Product_ID { get; set; }
            public string Screen_Name { get; set; }
            public string  Bautrate{ get; set; }
            public string  Background_imgID { get; set; }
            public string  Hardware_Ver{ get; set; }
            public string Software_Ver { get; set; }
            public string  Com_Port{ get; set; }
            public string EPsize { get; set; }
            public string Protocol { get; set; }
            public string Online_Status { get; set; }
            public string  NetworkCard_Address{ get; set; }
            public string Custom { get; set; }
            public string  Parent_SerialNum{ get; set; }
            public string  Panel_Number { get; set; }
            public string  Object_Instance { get; set; }

    }
        public List<All_node> listForControllerInfoFromALL_NODE_Table = new List<All_node>();

        /// <summary>
        /// Gives the controller info for given panel_id and given parent_serialnum present in
        /// BuildingName>ALL_NODE table in T3000 database
        /// </summary>
        /// <param name="outputTableName">ALL_NODE</param>
        /// <param name="buildingPath">T3000 db path</param>
        /// <param name="panel_number">any(e.g 1 for controller 1 and 2 for controller2)</param>
         public void ReadALL_NODE_Information_For_Given_Panel(string outputTableName, string buildingPath,int panel_number)
        {

            //--Now lets start reading the data from alex database
            listForControllerInfoFromALL_NODE_Table.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
          
            string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
          
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            string againDbPath = @"Data Source=" + newPath + "" + buildingPath; // BuildingSelected[0].Building_Path;

            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from  " + outputTableName+" WHERE Panal_Number = '"+panel_number +"' AND Parent_SerialNum = '0'"; 
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    listForControllerInfoFromALL_NODE_Table.Add(new All_node
                    {

                        MainBuilding_Name = reader["MainBuilding_Name"].ToString(),
                        Building_Name = reader["Building_Name"].ToString(),
                        Serial_ID = reader["Serial_ID"].ToString(),
                        Floor_name = reader["Floor_name"].ToString(),
                        Room_name = reader["Room_name"].ToString(),
                        Product_name = reader["Product_name"].ToString(),
                        Product_class_ID = reader["Product_class_ID"].ToString(),
                        Product_ID = reader["Product_ID"].ToString(),
                        Screen_Name = reader["Screen_Name"].ToString(),
                        Bautrate = reader["Bautrate"].ToString(),
                        Background_imgID = reader["Background_imgID"].ToString(),
                        Hardware_Ver = reader["Hardware_Ver"].ToString(),
                        Software_Ver = reader["Software_Ver"].ToString(),
                        Com_Port = reader["Com_Port"].ToString(),
                        EPsize = reader["EPsize"].ToString(),
                        Protocol = reader["Protocol"].ToString(),
                        Online_Status = reader["Online_Status"].ToString(),
                        NetworkCard_Address = reader["NetworkCard_Address"].ToString(),
                        Custom = reader["Custom"].ToString(),
                        Parent_SerialNum = reader["Parent_SerialNum"].ToString(),
                        Panel_Number = reader["Panal_Number"].ToString(),
                        Object_Instance = reader["Object_Instance"].ToString(),

                    });

                }
            }
            

        }



    }
}
