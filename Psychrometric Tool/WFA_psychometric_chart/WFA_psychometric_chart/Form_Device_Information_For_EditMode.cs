using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class Form_Device_Information_For_EditMode : Form
    {
        EditNodeLineForm f1;
        public Form_Device_Information_For_EditMode(EditNodeLineForm f)
        {
            f1 = f;
            InitializeComponent();
        }

        private void Form_Device_Information_For_EditMode_Load(object sender, EventArgs e)
        {
            //This load method should load the values for the given functions
            cb_enthalpy_or_humidity_input.SelectedIndex = 0;

            FindPathOfBuildingDB();

            //--Now lets load the values
            PullDeviceInfoFromAccessDB();


        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnSetNode_Click(object sender, EventArgs e)
        {
            //--Here we are going to implement the code required...
            //Yo functionality lekda lekda gain6 ke kya ho 
            /*
             --These should do following task:
             --1.Should get the values of the parameter such as instanceID,IPadress,temp,hum/enthalpy [which one is selected ],
             --  param1ID,param2ID,param1_identifier_type,and param2_identifier_type.
             --2.and the update to the data base 
                  1. in device info table 
                  2.main node the selected value to be device  
            */
            //0k now lets start the task

            InputDeviceInfoAndValue();
        }

        /// <summary>
        /// This function will input the device information as well as update the node table
        /// </summary>
        public void InputDeviceInfoAndValue()
        {

            if (checkBox1.Checked = true && checkBox2.Checked == true)
            {

                if (CB_param_temp.SelectedIndex >= 0 && CB_param_hum.SelectedIndex >= 0)
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

                    string temp_param_id = parameterValFromBacnet[CB_param_temp.SelectedIndex].indexID.ToString();//Converting to uint
                    string temp_present_value = parameterValFromBacnet[CB_param_temp.SelectedIndex].presentValue.ToString();

                    string second_param_id = parameterValFromBacnet[CB_param_hum.SelectedIndex].indexID.ToString();
                    string second_present_value = parameterValFromBacnet[CB_param_hum.SelectedIndex].presentValue.ToString();

                    //--This value used to identify weather the data is pulled from
                    //1 OBJECT_ANALOG_VALUE, OR OBJECT_ANALOG_INPUT OR OBJECT_ANALOG_OUTPUT

                    string param1_object_identifier_type = parameterValFromBacnet[CB_param_temp.SelectedIndex].object_identifier_type.ToString();
                    string param2_object_identifier_type = parameterValFromBacnet[CB_param_hum.SelectedIndex].object_identifier_type.ToString();


                    if (cb_enthalpy_or_humidity_input.SelectedIndex == 0)
                    {
                        //info has to be temp and hum
                        //uncomment this after this ............BBK
                        //F1.SetNodeForDeviceUsingTempAndHum(source, name, label, btnColor.BackColor, cbItemSelected, 20, device_instance, deviceIP, temp_param_id, second_param_id, "temp", "hum", temp_present_value, second_present_value, param1_object_identifier_type, param2_object_identifier_type);
                        //--Now lets find the node id
                       // MessageBox.Show("id is " + f1.tempNodeID);
                        f1.UpdateNodeInfoForDeviceSelection(f1.tempNodeID, device_instance, deviceIP, temp_param_id, second_param_id, "temp", "hum", temp_present_value, second_present_value, param1_object_identifier_type, param2_object_identifier_type);

                    }
                    else
                    {
                        //Temperature and enthalpy select bhayeako 6 ....:) :) ;p 
                        ///UNCOMMENT THIS LATERE
                        // F1.SetNodeForDeviceUsingTempAndHum(source, name, label, btnColor.BackColor, cbItemSelected, 20, device_instance, deviceIP, temp_param_id, second_param_id, "temp", "enthalpy", temp_present_value, second_present_value, param1_object_identifier_type, param2_object_identifier_type);
                        f1.UpdateNodeInfoForDeviceSelection(f1.tempNodeID, device_instance, deviceIP, temp_param_id, second_param_id, "temp", "enthalpy", temp_present_value, second_present_value, param1_object_identifier_type, param2_object_identifier_type);
                    }


                    this.Close();
                }
                else
                {
                    MessageBox.Show("Please select two parameters properly from dropdown box");
                }

            }  //--close of if checkbox  
        }



        public class DeviceClass                               
        {
            public string deviceIP { get; set; }
            public uint deviceInstance { get; set; }//device id 

        }
        List<DeviceClass> device_info = new List<DeviceClass>();
        public int indexSelectedDevice = 0;//We can declar this here now

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
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

                //  MessageBox.Show("Path to building = " + againDbPath);

                // String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

                string sql = "select * from ALL_NODE  where  Parent_SerialNum = '0'";//This will only show parent controller and not sub devices
                                                                                     //  MessageBox.Show("Connection = " + connection);
                                                                                     //  MessageBox.Show("sql = " + sql);
                                                                                     //DataSet myDataSet = new DataSet();
                                                                                     //try
                                                                                     //{
                using (OleDbConnection conn = new OleDbConnection(againDbPath))
                {
                    using (OleDbCommand cmd = new OleDbCommand(sql, conn))
                    {
                        conn.Open();
                        OleDbDataReader reader = cmd.ExecuteReader();
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
            string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
            //  MessageBox.Show("New path : " + againDbPath);
            // bool returnValue = false;
            //string latValue = "";
            using (OleDbConnection connection = new OleDbConnection(againDbPath))
            {
                connection.Open();
                OleDbDataReader reader = null;
                string queryString = "SELECT * from Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                OleDbCommand command = new OleDbCommand(queryString, connection);

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
        int deviceInstanceValuTemp = 0;//This for particular device
        private void CB_Device_SelectedIndexChanged(object sender, EventArgs e)
        {
            //--This load the data form the device rather then access db
            /*
            
            Steps :
            1. Get the data and then pull the value
            
            */

            groupBox3.Enabled = true;
            //Lets clear the combobox first
            CB_param_temp.Items.Clear();
            CB_param_hum.Items.Clear();
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

                    //Now that we have the parameter list lets display the list in the combobox...
                    // string s = "";
                    foreach (var item in parameterValFromBacnet)
                    {
                        CB_param_temp.Items.Add(item.device_object_name + ":" + item.indexID);
                        CB_param_hum.Items.Add(item.device_object_name + ":" + item.indexID);
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
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

                //MessageBox.Show("Path to building = " + againDbPath);


                try
                {
                    using (OleDbConnection conn = new OleDbConnection(againDbPath))
                    {
                        using (OleDbCommand cmd = new OleDbCommand(sql, conn))
                        {
                            conn.Open();
                            OleDbDataReader reader = cmd.ExecuteReader();
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

        int countFlag = 0;
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            //on checked enable the parameter
            if (checkBox1.Checked == true)
            {
                //then enable the combobox else dissable and 
                CB_param_temp.Enabled = true;

                countFlag++;//Increment the value

                Checkbox_Enable_Dissable();



            }
            else {
                //dissable the combobox...
                CB_param_temp.Enabled = false;
                countFlag--;//Decrement the count
                Checkbox_Enable_Dissable();
            }

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            //if checked then do else dont
            //on checked enable the parameter
            if (checkBox2.Checked == true)
            {
                //then enable the combobox else dissable and 
                CB_param_hum.Enabled = true;
                countFlag++;
                Checkbox_Enable_Dissable();


            }
            else {
                //dissable the combobox...
                CB_param_hum.Enabled = false;
                countFlag--;//Decrement flag
                Checkbox_Enable_Dissable();

            }

        }

        public void Checkbox_Enable_Dissable()
        {

            if (countFlag == 2)
            {
                if (checkBox1.Checked == true && checkBox2.Checked == true)
                {
              
                }
              
            }
            else
            {
                //This is if the cont is 1
                if (checkBox1.Checked == true)
                {
                    //2nd and 3rd free it
                    checkBox2.Checked = false;
                    checkBox2.Enabled = true;
                }
                else if (checkBox2.Checked == true)
                {
                    checkBox1.Checked = false;
                    checkBox1.Enabled = true;
                }
            

            }
        }     //close of the function

        private void CB_param_temp_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CB_param_temp.SelectedIndex > -1)
            {
                //We need to enable set node 
                // btnSetNode.Enabled = true;
                //enableDissableOKButton();

                indexForParameter1 = CB_param_temp.SelectedIndex;
            }
        }

        public int indexForParameter2 = 0;
        private void CB_param_hum_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (CB_param_hum.SelectedIndex > -1)
            {
                //We need to enable set node 


                 btnSetNode.Enabled = true;
                //enableDissableOKButton();

                indexForParameter2 = CB_param_temp.SelectedIndex;
            }
        }

        public void enableDissableOKButton()
        {
            if (indexForParameter1 > -1 && indexForParameter2 > -1)
            {
                btnSetNode.Enabled = true;
            }
            else
            {
                btnSetNode.Enabled = false;
            }             
        }
    }
}
