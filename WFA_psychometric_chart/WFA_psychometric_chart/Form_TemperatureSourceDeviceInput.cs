using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class Form_TemperatureSourceDeviceInput : Form
    {
        public Form_TemperatureSourceDeviceInput()
        {
            InitializeComponent();
        }

        private void Form_TemperatureSourceDeviceInput_Load(object sender, EventArgs e)
        {
            //--This will be load function which should be done loading the files form the sqlite files




        }





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
                //  string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
                // string againDbPath = @"Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
                string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db;Version=3;";
                //  MessageBox.Show("Path to building = " + againDbPath);

                // String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

                string sql = "select * from ALL_NODE  where  Parent_SerialNum = '0'";//This will only show parent controller and not sub devices
                                                                                     //     MessageBox.Show("Connection = " + connection);
                                                                                     //  MessageBox.Show("sql = " + sql);
                                                                                     //DataSet myDataSet = new DataSet();
                                                                                     //try
                                                                                     //{
                using (SQLiteConnection conn = new SQLiteConnection(againDbPath))
                {
                    using (SQLiteCommand cmd = new SQLiteCommand(sql, conn))
                    {
                        conn.Open();
                        SQLiteDataReader reader = cmd.ExecuteReader();
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
                }
            }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
            return mainControllerList;
        }




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
        public class DeviceClass
        {
            public string deviceIP { get; set; }
            public uint deviceInstance { get; set; }//device id 

        }
        List<DeviceClass> device_info = new List<DeviceClass>();


        public void PullDeviceInfoFromAccessDB()
        {

            device_info.Clear();//Resetting the values
            CB_Device.Items.Clear();//Clearing the combobox
            btnSetNode.Enabled = false;

            copyOfMainControllerList.Clear();//tHSI IS RESETTING 
            copyOfMainControllerList = ControllerList();//This returns the miancontroller and copy those in copyof...
                                                        //device_info = copyOfMainControllerList;

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


        }

        public class parameter_class1
        {
            public double presentValue { get; set; }
            public uint indexID { get; set; }
            public string device_object_name { get; set; }
            public string object_identifier_type { get; set; }
        }

        public List<parameter_class1> parameterValFromBacnet = new List<parameter_class1>();
        public int indexSelectedDevice = 0;//We can declar this here now

        int deviceInstanceValuTemp = 0;//This for particular device

        private void CB_Device_SelectedIndexChanged(object sender, EventArgs e)
        {
            //--This is when the controller is selected and value is added to it
            //--This load the data form the device rather then access db
            /*            
            Steps :
            1. Get the data and then pull the value            
            */
                                                   
            groupBox3.Enabled = true;
            //Lets clear the combobox first
            CB_param_temp.Items.Clear();
           // CB_param_hum.Items.Clear();
            parameterValFromBacnet.Clear();//Clearing the list value

            if (CB_Device.SelectedIndex > -1)
            {
                //On this index change the we need to get the values from the 
                indexSelectedDevice = CB_Device.SelectedIndex;
                int instanceId = (int)copyOfMainControllerList[indexSelectedDevice].controllerInstanceId;//Device instance selected.

                deviceInstanceValuTemp = instanceId;

                // MessageBox.Show("instance id = " + instanceId);
                // MessageBox.Show("Device checkonline offline = " + CheckDeviceOnlineOffline(deviceInstanceValuTemp, 0));
                if (CheckDeviceOnlineOffline(deviceInstanceValuTemp, 0) == true)
                {


                    //This uint has to be converted to int because that is what is required...
                    DeviceConnection db = new DeviceConnection();
                    db.ScanForDevice();
                    db.ScanForParameters(instanceId);//This will return the parameters
                                                     //resert first 
                    device_info.Clear();
                    foreach (var bn in BACnetClass.DevicesList)
                    {
                        device_info.Add(new DeviceClass
                        {
                            deviceInstance = bn.device_id,
                            deviceIP = bn.device_id + "." + bn.adr.adr[0] + "." + bn.adr.adr[1] + "." + bn.adr.adr[2] + "." + bn.adr.adr[3]
                            

                        });
                    }

                    //Now we can use the value strored int  the db.parameterList1
                    string s = "";
                    foreach (var bac in db.parameterListValue)
                    {
                        parameterValFromBacnet.Add(new parameter_class1
                        {
                            device_object_name = bac.device_object_name,
                            indexID = bac.indexID,
                            presentValue = bac.presentValue,
                            object_identifier_type = bac.object_identifier_type


                        });
                        //    s += bac.device_object_name + "," + bac.presentValue;
                    }

                    //  MessageBox.Show("value = " + s);
                    //--Here we need to scan form the  T3000 database and then publish the list which is there 
                    /*
                    Task :
                    1.Read the data from the alex db 
                    2.Find only TEMPERATURE VALUE
                    3.Filter and display only temperature values
                    */




                    //Now that we have the parameter list lets display the list in the combobox...
                    // string s = "";
                    foreach (var item in parameterValFromBacnet)
                    {
                        CB_param_temp.Items.Add(item.device_object_name + ":" + item.indexID);
                       // CB_param_hum.Items.Add(item.device_object_name + ":" + item.indexID);
                        //  CB_param_enthalpy.Items.Add(item.device_object_name + ":" + item.indexID);
                        //     s += item.device_object_name + ":" + item.indexID + " value = " + item.presentValue+"\n";

                    }
                }//close of the device is online or offline
                else
                {
                    MessageBox.Show("Device is offline");
                }
                //TEST
                // MessageBox.Show("s = " + s);
            }//Close of if
        }

        public int indexForParameter1 = 0;

        List<bool> dataCheckList = new List<bool>();
        /// <summary>
        /// CHECKS WEATHER THE DEVICE IS ONLINE OR OFFLINE
        /// </summary>
        /// <param name="deviceID">device id value</param>
        /// <param name="Parent_SerialNum">0 always</param>
        /// <returns></returns>
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
               // string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
                string againDbPath = @"Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

                //MessageBox.Show("Path to building = " + againDbPath);


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

        private void CB_param_temp_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
