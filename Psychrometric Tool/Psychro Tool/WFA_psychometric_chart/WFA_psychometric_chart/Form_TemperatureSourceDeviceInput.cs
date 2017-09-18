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
        EditNodeLineForm f1;
        public Form_TemperatureSourceDeviceInput(EditNodeLineForm f)
        {
            f1 = f;
            InitializeComponent();
        }

        private void Form_TemperatureSourceDeviceInput_Load(object sender, EventArgs e)
        {
            //--This will be load function which should be done loading the files form the sqlite files



            //Finding building path 
            FindPathOfBuildingDB();



            //MessageBox.Show("BBK tEST : Count = " + BuildingSelected.Count);
            PullDeviceInfoFromAccessDB();

            //copyOfMainControllerList = ControllerList();
            ////--Now lets populate the field
            //foreach(var item in copyOfMainControllerList)
            //{
            //    CB_Device.Items.Add("Controller:" + item.controllerName+"["+item.controllerInstanceId+"]");
            //}

        } 




        public class dataTypeForControllerList
        {
            public string controllerName { get; set; }
            public int controllerInstanceId { get; set; }
            public bool controllerStatus { get; set; }
        }

       public List<dataTypeForControllerList> mainControllerList = new List<dataTypeForControllerList>();
     public   List<dataTypeForControllerList> copyOfMainControllerList = new List<dataTypeForControllerList>();


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
               // MessageBox.Show("Building info found");
                //return null;

                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                //  string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;
                 string againDbPath = @"Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

                //C:\Program Files (x86)\T3000\
                // string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db;Version=3;";
                //string againDbPath = @"Data Source=C:\Program Files (x86)\T3000\Database\T3000.db;Version=3;";
               // MessageBox.Show("Path to building = " + againDbPath);

                // String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

                string sql = "select * from ALL_NODE  where  Parent_SerialNum = '0'";//This will only show parent controller and not sub devices
                   
                //     MessageBox.Show("Connection = " + connection);
               // MessageBox.Show("sql = " + sql);
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
            //MessageBox.Show(ex.Message);
            //}                                          
            return mainControllerList;
        }




        public class SelectedBuildingDatatype
        {   

            public string Main_BuildingName { get; set; }
            public string Building_Name { get; set; }
            public string Building_Path { get; set; }
        }
        public List<SelectedBuildingDatatype> BuildingSelected = new List<SelectedBuildingDatatype>();

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
              //MessageBox.Show("reutn from here : New path : " + againDbPath);

           // return;//This to return
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from Building WHERE Default_SubBuilding = 1 ";//-1 or True  can be used
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

       // public void pullDeviceInfo

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

                   // MessageBox.Show("parameterListValue count option  = " + db.parameterListValue.Count);
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
                      //  s += "\n"+bac.device_object_name + ",["+ bac.indexID + "]"+ bac.presentValue+",TYPE = "+bac.object_identifier_type;
                    }                                                       

                     //MessageBox.Show("Data returned from backnet  value =\n " + s);
                    //--Here we need to scan form th    e  T3000 database and then publish the list which is there 
                    /*
                    Task :
                    1.Read the data from the alex db 
                    2.Find only TEMPERATURE VALUE
                    3.Filter and display only temperature values
                    */                        
                    Form1_main f = new Form1_main();
                    string path_to_alexdb = f.PathToT3000BuildingDB;// PathToT3000BuildingDB;
                    ReadDataFromAlexDatabase( "INPUTable");

                    //--This only filteres temperature inside alex db 
                    FilterOnlyTemperatureAndHumidityFromAlexDBValue();//Returns FilteredTemperatureValueList as a result

                    //--Returns AfterFilteringFromAlexValueListOfValues
                    FilterDataForDisplayInList();//Filteres value form alex db and give new value to insert to list teurns:AfterFilteringFromAlexValueListOfValues


                    foreach (var item in AfterFilteringFromAlexValueListOfValues)
                    {
                        CB_param_temp.Items.Add(item.device_object_name + ":" + item.indexID+ "[Value = " + item.presentValue + "]");
                        // CB_param_hum.Items.Add(item.device_object_name + ":" + item.indexID);
                        //  CB_param_enthalpy.Items.Add(item.device_object_name + ":" + item.indexID);
                        // s += item.device_object_name + ":" + item.indexID + " value = " + item.presentValue+"\n";

                    }

                    CB_param_temp.Enabled = true;
                    //Now that we have the parameter list lets display the list in the combobox...
                    // string s = "";
                    //foreach (var item in parameterValFromBacnet)
                    //{
                    //   CB_param_temp.Items.Add(item.device_object_name + ":" + item.indexID);
                    //   // CB_param_hum.Items.Add(item.device_object_name + ":" + item.indexID);
                    //   //  CB_param_enthalpy.Items.Add(item.device_object_name + ":" + item.indexID);
                    //   // s += item.device_object_name + ":" + item.indexID + " value = " + item.presentValue+"\n";

                    //}
                }//close of the device is online or offline
                else
                {
                    MessageBox.Show("Device is offline");
                }
                //TEST
                // MessageBox.Show("s = " + s);
            }//Close of if
        }


        List<T3000DataType> FilteredTemperatureValueList = new List<T3000DataType>();
      //  List<T3000DataType> FilteredHumidityValueList = new List<T3000DataType>();

        public void FilterOnlyTemperatureAndHumidityFromAlexDBValue()
        {
           // FilteredHumidityValueList.Clear();
            FilteredTemperatureValueList.Clear();
            for (int i = 0; i<InputFromT3000.Count;i++)
            {
                if(InputFromT3000[i].InputUnit == "Deg.C")
                {
                    //--The item is temperature no need to check for humidity
                    //FilteredTemperatureValueList.Add(new T3000DataType (item));
                    //----Her problem arised out of bound error
                    //FilteredTemperatureValueList = InputFromT3000.GetRange(i, 1);  //Copys the items for main list to new list

                    FilteredTemperatureValueList.Add(new T3000DataType
                    {
                        PanelID = InputFromT3000[i].PanelID,
                        InputIndex = InputFromT3000[i].InputIndex,

                        InputDescription = InputFromT3000[i].InputDescription,
                        InputAM = InputFromT3000[i].InputAM,

                        InputValue = InputFromT3000[i].InputValue,
                        InputUnit = InputFromT3000[i].InputUnit,

                        InputRange = InputFromT3000[i].InputRange,
                        InputCalibration = InputFromT3000[i].InputCalibration,


                        InputCalSign = InputFromT3000[i].InputCalSign,
                        InputFilter = InputFromT3000[i].InputFilter,

                        InputDecon = InputFromT3000[i].InputDecon,
                        InputJumper = InputFromT3000[i].InputJumper,
                        InputLabel = InputFromT3000[i].InputLabel

                    });
                }
                //else if (InputFromT3000[i].InputUnit.Contains("%"))
                //{
                //    //--This is humidity  
                //    FilteredHumidityValueList.GetRange(i, 1);
                //}
            }

           // MessageBox.Show("Filtered temperature value form alex db should be 3 , count  = "+FilteredTemperatureValueList.Count);
                                                
        }



       /// <summary>
       /// Stores the value obtained form hardware and filtered form alex database
       /// </summary>

        public List<parameter_class1> AfterFilteringFromAlexValueListOfValues = new List<parameter_class1>();

        /// <summary>
        /// Filters the temperature value form alex database and form 
        /// Hardware pulled 
        /// </summary>

        public void FilterDataForDisplayInList()
        {
            AfterFilteringFromAlexValueListOfValues.Clear();

           
            //--This filters the temperature 
            if (FilteredTemperatureValueList.Count > 0)
            {

                //--Then we need to filter the values from the dbatabse 

                string test = "";
            for(int item =0;item< parameterValFromBacnet.Count;item++)
                {


                    if(parameterValFromBacnet[item].object_identifier_type == "OBJECT_ANALOG_INPUT")
                    { 
                    int indexToCompare = (int)parameterValFromBacnet[item].indexID;
                    for(int v=0; v < FilteredTemperatureValueList.Count ;v++)
                    {
                        if(indexToCompare == int.Parse(FilteredTemperatureValueList[v].InputIndex))
                        {
                            //--Index is mached we need to compare inser it to new list
                            //--Lets add the values
                            //AfterFilteringFromAlexValueListOfValues.GetRange(v, 1);
                            //AfterFilteringFromAlexValueListOfValues.AddRange()
                            AfterFilteringFromAlexValueListOfValues.Add(new parameter_class1
                            {
                                indexID = parameterValFromBacnet[item].indexID,
                                device_object_name = parameterValFromBacnet[item].device_object_name,
                                object_identifier_type = parameterValFromBacnet[item].object_identifier_type,
                                presentValue = parameterValFromBacnet[item].presentValue                               
                            });
                           // test += "\n" + parameterValFromBacnet[item].indexID + ",val" + parameterValFromBacnet[item].presentValue;
                            break;//After the value is found break form this loop
                        }

                        }//Close of if functions
                    } //--Close of for loop 
                }
             //   MessageBox.Show("Test for compared value \n " + test);

            }


          //  MessageBox.Show("value filter form decive and alex tmeperature filter value count = " + AfterFilteringFromAlexValueListOfValues.Count);

        }




        public class T3000DataType
        {
            public string PanelID { get; set; }
            public string InputIndex { get; set; }
            public string InputDescription { get; set; }

            public string InputAM { get; set; }
            public string InputValue { get; set; }
            public string  InputUnit{ get; set; }

            public string InputRange { get; set; }
            public string InputCalibration { get; set; }
            public string InputCalSign { get; set; }

             public string InputFilter { get; set; }
            public string InputDecon { get; set; }
            public string InputJumper { get; set; }
            public string InputLabel { get; set; }

        }
        List<T3000DataType> InputFromT3000 = new List<T3000DataType>();

        /// <summary>
        ///       
        /// Read the data from alex db ..
        /// This will be loaded when first loaded values        
        /// </summary>
        /// <param name="databasePathWithFileName">DB path path to Inputables tableie. [buildingname].db path in alex db</param>
        /// <param name="TableName">name of the table i.2 inputables</param>

        public void ReadDataFromAlexDatabase(string TableName)            
        {
            //--Now lets start reading the data from alex database
            InputFromT3000.Clear();
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
            string againDbPath = @"Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;


           // MessageBox.Show("aLEX DB PATH : INPUTable New path : " + againDbPath+"\n db name  = "+TableName);
            // bool returnValue = false;
            //string latValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from  " + TableName; //Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    InputFromT3000.Add(new  T3000DataType
                    {
                        //Main_BuildingName = reader["Main_BuildingName"].ToString(),
                        //Building_Name = reader["Building_Name"].ToString(),
                        //Building_Path = reader["Building_Path"].ToString()

                        PanelID  = reader["PanelID"].ToString(),
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

           // MessageBox.Show("All data form alex db count = " + InputFromT3000.Count);

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
            //--When this is clicked then we need to put the values into database before that we need to 
            //--Filter the values and put in db
            /*
            Steps:
            1. identify the values
            2.put the values in the database
            3.Finsish the task
            */


            if (CB_param_temp.SelectedIndex > -1)
            {
                //We need to enable set node 
                 btnSetNode.Enabled = true;
                //enableDissableOKButton();

                indexForParameter1 = CB_param_temp.SelectedIndex;
            }




        }

        private void btnSetNode_Click(object sender, EventArgs e)
        {
            //--This one does the actual modification in the box...
            /*
            Steps : Reading the data and then inputing the values  
            1. 
            */
            //--This does all the updating and insertion function
            try { 
            InputDeviceInfoAndValue();
                f1.dataGridView1.EndEdit();

            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

       
            
       

        /// <summary>
        /// This function will input the device information as well as update the node table
        /// </summary>
        public void InputDeviceInfoAndValue()
        {

          //  if (checkBox1.Checked = true && checkBox2.Checked == true)
           // {

                if (CB_param_temp.SelectedIndex >= 0 )
                {
                    //Temp and hum is selected
                    //now param1 will be temperature and param2 wil be humidity
                    //we have id name and present values.
                    string device_instance = copyOfMainControllerList[CB_Device.SelectedIndex].controllerInstanceId.ToString();
                    string deviceIP = "";
                    //For device info form device
                    foreach (var device in device_info)
                    {
                        if (device_instance == device.deviceInstance.ToString())
                        {
                            deviceIP = device.deviceIP;//device_info[CB_Device.SelectedIndex].deviceIP.ToString();
                        }
                    }

                // string temp_param_id = parameterValFromBacnet[CB_param_temp.SelectedIndex].indexID.ToString();//Converting to uint
                string temp_param_id = AfterFilteringFromAlexValueListOfValues[CB_param_temp.SelectedIndex].indexID.ToString();//Converting to uint
               // string temp_present_value = parameterValFromBacnet[CB_param_temp.SelectedIndex].presentValue.ToString();
                string temp_present_value = AfterFilteringFromAlexValueListOfValues[CB_param_temp.SelectedIndex].presentValue.ToString();

                //   string second_param_id = parameterValFromBacnet[CB_param_hum.SelectedIndex].indexID.ToString();
                //  string second_present_value = parameterValFromBacnet[CB_param_hum.SelectedIndex].presentValue.ToString();

                //--This value used to identify weather the data is pulled from
                //1 OBJECT_ANALOG_VALUE, OR OBJECT_ANALOG_INPUT OR OBJECT_ANALOG_OUTPUT

                //string param1_object_identifier_type = parameterValFromBacnet[CB_param_temp.SelectedIndex].object_identifier_type.ToString();
                string param1_object_identifier_type = AfterFilteringFromAlexValueListOfValues[CB_param_temp.SelectedIndex].object_identifier_type.ToString();
                // string param2_object_identifier_type = parameterValFromBacnet[CB_param_hum.SelectedIndex].object_identifier_type.ToString();


              
                //------------------------------------Working here -------------------------------------------------------//
                f1.UpdateNodeInfoForTemperatureDeviceSelection(f1.tempNodeID, device_instance, deviceIP, temp_param_id, "temp", temp_present_value, param1_object_identifier_type);

                //------------------------------------End of working here------------------------------------------------//


                f1.CallFromTemperatureAndHumidtyFormForMixNodeFxn(f1.tempNodeID);

                this.Close();
                }
                else
                {
                    MessageBox.Show("Please select two parameters properly from dropdown box");
                }

           // }  //--close of if checkbox  
        }





        private void btnCancel_Click(object sender, EventArgs e)
        {
            //Before we close we need to refresh as well 

            f1.RefreshChartAndDGVForMixNodeFunction();
            //--If we are closed then before closing we need to rese the values to previous values
            this.Close();//--This form will be closed by this action
        }

        private void Form_TemperatureSourceDeviceInput_FormClosing(object sender, FormClosingEventArgs e)
        {
            f1.RefreshChartAndDGVForMixNodeFunction();
        }
    }
}
