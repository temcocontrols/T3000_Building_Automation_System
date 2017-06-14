using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.BACnet;
using System.Threading;

namespace PH_App
{
    class BACnetClass
    {

        static BacnetClient bacnet_client;

        // All the present Bacnet Device List
        public  static List<BacNode> DevicesList = new List<BacNode>();

        /*********************************************************************************************/

       // public static IEnumerable<BacNode> DevicesList { get; private set; }


        public void StartProgram(int deviceId,uint inputnodeid)
        {
            Trace.Listeners.Add(new ConsoleTraceListener());

            try
            {
                StartActivity();
                //Console.WriteLine("Started");

                Thread.Sleep(3000); // Wait a fiew time for WhoIs responses (managed in handler_OnIam)
             
                //Console.WriteLine("BACnet Devices List");
                //Console.WriteLine("Instance ID\tBACnetIP");
                lock (DevicesList)
                {
                    foreach (BacNode bn in DevicesList)
                    {
                      // Console.WriteLine("{0}\t{1}.{2}.{3}.{4}", bn.device_id, bn.adr.adr[0], bn.adr.adr[1], bn.adr.adr[2], bn.adr.adr[3]);
                    }

                }
                // Console.WriteLine("Input a instance that you want to read that value (input 1)");
                //  string StrBacnetID = Console.ReadLine();//For now it will be commented later it will be fine


                int BacnetID = deviceId;//24649;//int.Parse(StrBacnetID);
                uint inputNodeID =inputnodeid;
                //ReadWriteExample(BacnetID);
                readBacnetDevieParameter(BacnetID, inputNodeID);
            }
            catch { }

            //Console.ReadKey();
        }


        public  void StartProgramForScanHardware(int deviceId, uint inputnodeid,string identifier_type)
        {
            Trace.Listeners.Add(new ConsoleTraceListener());

            try
            {
                StartActivity();
                //Console.WriteLine("Started");

                Thread.Sleep(3000); // Wait a fiew time for WhoIs responses (managed in handler_OnIam)
                //Console.WriteLine("BACnet Devices List");
                //Console.WriteLine("Instance ID\tBACnetIP");
                lock (DevicesList)
                {
                    //foreach (BacNode bn in DevicesList)
                    //{
                    //    // Console.WriteLine("{0}\t{1}.{2}.{3}.{4}", bn.device_id, bn.adr.adr[0], bn.adr.adr[1], bn.adr.adr[2], bn.adr.adr[3]);
                    //}

                }
          
                int BacnetID = deviceId;//24649;//int.Parse(StrBacnetID);
                uint inputNodeID = inputnodeid;
                //ReadWriteExample(BacnetID);
                readBacnetDevieParameterForScanHardware(BacnetID, inputNodeID,identifier_type);//identifier type
            }
            catch { }

            //Console.ReadKey();
        }

        /// <summary>
        /// Without who is request who is confirms device connectivity however it sleeps for 3 second
        /// so while in loop after connection direct call for the values using this function form bacnet
        /// </summary>
        /// <param name="deviceId">Instance ID of device</param>
        /// <param name="inputnodeid">input parameter value </param>
        /// <param name="identifier_type">identifier like OBJECT_ANALOG_VALUE,OBJECT_ANALOG_INPUT,etc</param>
        /// <returns></returns>
        public double ReadBacnetValue(int deviceId, uint inputnodeid, string identifier_type)
        {
            double retValue = 0;
            int BacnetID = deviceId;//24649;//int.Parse(StrBacnetID);
            uint inputNodeID = inputnodeid;

            readBacnetDevieParameterForScanHardware(BacnetID, inputNodeID, identifier_type);//identifier type

            retValue = PresentValueFromBacnet;
            return retValue;
        }

        public bool RequestingBacnetDeviceForCommunication()
        {
            bool deviceConnectedOrNotConnected = false;//false =not connected

            Trace.Listeners.Add(new ConsoleTraceListener());

            try
            {
                StartActivity();
                //Console.WriteLine("Started");

                Thread.Sleep(3000); // Wait a fiew time for WhoIs responses (managed in handler_OnIam)
                //Console.WriteLine("BACnet Devices List");
                //Console.WriteLine("Instance ID\tBACnetIP");
                lock (DevicesList)
                {
                    foreach (BacNode bn in DevicesList)
                    {
                        // Console.WriteLine("{0}\t{1}.{2}.{3}.{4}", bn.device_id, bn.adr.adr[0], bn.adr.adr[1], bn.adr.adr[2], bn.adr.adr[3]);
                        deviceConnectedOrNotConnected = true;//device detected so
                    }

                }
            }
            catch { }
                return deviceConnectedOrNotConnected;
        }



        public static void StartActivity()
        {
            // Bacnet on UDP/IP/Ethernet
            bacnet_client = new BacnetClient(new BacnetIpUdpProtocolTransport(0xBAC0, false), 3000, 3);
            // or Bacnet Mstp on COM4 à 38400 bps, own master id 8
            // m_bacnet_client = new BacnetClient(new BacnetMstpProtocolTransport("COM4", 38400, 8);
            // Or Bacnet Ethernet
            // bacnet_client = new BacnetClient(new BacnetEthernetProtocolTransport("Connexion au réseau local"));          

            bacnet_client.Start();//go
            // Send WhoIs in order to get back all the Iam responses :  
            bacnet_client.OnIam += new BacnetClient.IamHandler(handler_OnIam);
            bacnet_client.WhoIs();

        }

        /*****************************************************************************************************/
                       
            public double PresentValueFromBacnet=0;
          public void readBacnetDevieParameter(int bacnetID,uint bacnetInputPort)
        {
            BacnetValue Value;
            //BacnetValue ValueX;
            bool ret;
            // Read Present_Value property on the object ANALOG_INPUT:0 provided by the device 12345
            // Scalar value only
            //   Console.WriteLine("Read ID = %d input0 value = %d");
            ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, bacnetInputPort), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            //bool t = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, bacnetInputPort), BacnetPropertyIds.PROP_USER_NAME, out ValueX);
            //if (t == true)
            //{

            //}
            if (ret == true)
            {
                // Console.WriteLine("Read id = {0},input1 value : {1}", bacnetID, Value.Value.ToString());

                PresentValueFromBacnet =Math.Round( double.Parse(Value.Value.ToString()),2);
                // Write Present_Value property on the object ANALOG_OUTPUT:0 provided by the device 4000
                //  BacnetValue newValue = new BacnetValue(Convert.ToSingle(Value.Value));   // expect it's a float
                //  ret = WriteScalarValue(4000, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, 0), BacnetPropertyIds.PROP_PRESENT_VALUE, newValue);
                //  Console.WriteLine("Write feedback : " + ret.ToString());
            }
          //  else
                //  MessageBox.Show("Error somewhere !");
        }

        public void readBacnetDevieParameterForScanHardware(int bacnetID, uint bacnetInputPort,string identifier_type)
        {
            BacnetValue Value;
            //BacnetValue ValueX;
            bool ret;

            //   Console.WriteLine("Read ID = %d input0 value = %d");
            if(identifier_type == "OBJECT_ANALOG_VALUE")
            { 
            ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, bacnetInputPort), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }
            else if(identifier_type == "OBJECT_ANALOG_INPUT")
            {
                ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, bacnetInputPort), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }
            else
            {
                //do for output
                ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, bacnetInputPort), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }

            if (ret == true)
            {
                // Console.WriteLine("Read id = {0},input1 value : {1}", bacnetID, Value.Value.ToString());

                PresentValueFromBacnet = Math.Round(double.Parse(Value.Value.ToString()), 2);

            }
            //  else
            //  MessageBox.Show("Error somewhere !");
        }


        public class parameter_class
        {
           public double presentValue { get; set; }
            public uint indexID { get; set; }
            public string device_object_name { get; set; }

            public string object_identifier_type { get; set; }
        }
        
     public   static List<parameter_class> ReadParameterUsigBacnetID(int bacnetid)
        {
          
                List<parameter_class> parameterList = new List<parameter_class>();
                BacnetValue Value;
                BacnetValue Value_Object_Name;
                bool ret;
            // Read Present_Value property on the object ANALOG_INPUT:0 provided by the device 12345
            // Scalar value only
            //   Console.WriteLine("Read ID = %d input0 value = %d");

            //---This one is for OBJECT_ANALOG_VALUE

            for (uint x = 0; x < 10; x++) {
                    //ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, 0), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
                    ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, x), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
                    bool rect2 = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, x), BacnetPropertyIds.PROP_OBJECT_NAME, out Value_Object_Name);
                    if (ret == true)
                    {
                        //  Console.WriteLine("Read id = {0},input1 value : {1}", bacnetid, Value.Value.ToString());

                        if (rect2 == true)
                        {
                        //ADD VALUES ONLY IF THE VALUE SI not empty
                        if(Value_Object_Name.ToString() != "") { 
                            //ADD to the list and return the value...
                            parameterList.Add(new parameter_class
                            {
                                device_object_name = Value_Object_Name.ToString(),
                                indexID = x,
                                presentValue = double.Parse(Value.ToString())  ,
                                object_identifier_type = "OBJECT_ANALOG_VALUE"

                            });
                        }//Close of if value _object_name
                    }

                    }//close of if
                     // else
                     //   Console.WriteLine("Error somewhere !");
                }//Close of for

            //************************************************added code***************************************************************//


            //---This one is for OBJECT_ANALOG_INPUT
            for (uint x = 0; x < 10; x++)
            {

                ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, x), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
                bool rect2 = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, x), BacnetPropertyIds.PROP_OBJECT_NAME, out Value_Object_Name);
                if (ret == true)
                {
                    //  Console.WriteLine("Read id = {0},input1 value : {1}", bacnetid, Value.Value.ToString());

                    if (rect2 == true)
                    {
                        //ADD VALUES ONLY IF THE VALUE SI not empty
                        if (Value_Object_Name.ToString() != "")
                        {
                            //ADD to the list and return the value...
                            parameterList.Add(new parameter_class
                            {
                                device_object_name = Value_Object_Name.ToString(),
                                indexID = x,   //THIS ONE IS THE INSTANCE ID
                                presentValue = double.Parse(Value.ToString()),
                                object_identifier_type = "OBJECT_ANALOG_INPUT"

                            });
                        }//Close of if value _object_name
                    }

                }//close of if

            }//Close of for




            //---This one is for OBJECT_ANALOG_OUTPUT
            for (uint x = 0; x < 10; x++)
            {

                ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, x), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
                bool rect2 = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, x), BacnetPropertyIds.PROP_OBJECT_NAME, out Value_Object_Name);
                if (ret == true)
                {
                    //  Console.WriteLine("Read id = {0},input1 value : {1}", bacnetid, Value.Value.ToString());

                    if (rect2 == true)
                    {
                        //ADD VALUES ONLY IF THE VALUE SI not empty
                        if (Value_Object_Name.ToString() != "")
                        {
                            //ADD to the list and return the value...
                            parameterList.Add(new parameter_class
                            {
                                device_object_name = Value_Object_Name.ToString(),
                                indexID = x,   //THIS ONE IS THE INSTANCE ID
                                presentValue = double.Parse(Value.ToString()),
                                object_identifier_type = "OBJECT_ANALOG_OUTPUT"

                            });
                        }//Close of if value _object_name
                    }

                }//close of if

            }//Close of for


            //********************************************added code end*************************************************************//
            return parameterList;
        }

        public double ReadSingleParameterUsigBacnetID(int bacnetID,string bacnetObjectIdentifierType,int bacnetObjectIDInstance)
        {
            BacnetValue Value;
            //BacnetValue ValueX;
            bool ret;
            double returnValue = 0;
            //   Console.WriteLine("Read ID = %d input0 value = %d");
            if (bacnetObjectIdentifierType == "OBJECT_ANALOG_VALUE")
            {
                ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, (uint)bacnetObjectIDInstance), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }
            else if (bacnetObjectIdentifierType == "OBJECT_ANALOG_INPUT")
            {
                ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, (uint)bacnetObjectIDInstance), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }
            else
            {
                //do for output
                ret = ReadScalarValue(bacnetID, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, (uint)bacnetObjectIDInstance), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
            }

            if (ret == true)
            {
                // Console.WriteLine("Read id = {0},input1 value : {1}", bacnetID, Value.Value.ToString());

                 returnValue = Math.Round(double.Parse(Value.Value.ToString()), 2);

            }


            return returnValue;
        }



        //======================================New Parameter reading task fist ================================//


       public static List<parameter_class> ReadParameterUsingBacnetIDForTemperatureAndHumidityBoth(int bacnetid)
        {

           List<parameter_class> parameterList = new List<parameter_class>();
            BacnetValue Value;
            BacnetValue Value_Object_Name;
            // bool ret;
            // Read Present_Value property on the object ANALOG_INPUT:0 provided by the device 12345
            // Scalar value only
            //   Console.WriteLine("Read ID = %d input0 value = %d");

            //---This one is for OBJECT_ANALOG_VALUE
            for (uint x = 0; x < 10; x++)
            {
                //ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, 0), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
              bool  ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, x), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);
                bool rect2 = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, x), BacnetPropertyIds.PROP_OBJECT_NAME, out Value_Object_Name);
                if (ret == true)
                {
                    //  Console.WriteLine("Read id = {0},input1 value : {1}", bacnetid, Value.Value.ToString());

                    if (rect2 == true)
                    {
                        //ADD VALUES ONLY IF THE VALUE SI not empty
                      
                        if (Value_Object_Name.ToString() != "")
                        {
                            //ADD to the list and return the value...
                            parameterList.Add(new parameter_class
                            {
                                device_object_name = Value_Object_Name.ToString(),
                                indexID = x,
                                presentValue = double.Parse(Value.ToString()),
                                object_identifier_type = "OBJECT_ANALOG_VALUE"

                            });
                        }//Close of if value _object_name
                    }

                }//close of if
                 // else
                 //   Console.WriteLine("Error somewhere !");
            }//Close of for

            
            //************************************************added code***************************************************************//

            //********************************************added code end*************************************************************//
            return parameterList;
        }


        //==================================End of new parameter reading ===================================//



        static void ReadWriteExample(int bacnetid)
        {

            BacnetValue Value;
            bool ret;
            // Read Present_Value property on the object ANALOG_INPUT:0 provided by the device 12345
            // Scalar value only
            //   Console.WriteLine("Read ID = %d input0 value = %d");
            ret = ReadScalarValue(bacnetid, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, 0), BacnetPropertyIds.PROP_PRESENT_VALUE, out Value);

            if (ret == true)
            {
                Console.WriteLine("Read id = {0},input1 value : {1}", bacnetid, Value.Value.ToString());

                // Write Present_Value property on the object ANALOG_OUTPUT:0 provided by the device 4000
                // BacnetValue newValue = new BacnetValue(Convert.ToSingle(Value.Value));   // expect it's a float
                // ret = WriteScalarValue(4000, new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_OUTPUT, 0), BacnetPropertyIds.PROP_PRESENT_VALUE, newValue);
                // Console.WriteLine("Write feedback : " + ret.ToString());
            }
            else
                Console.WriteLine("Error somewhere !");
        }

        /*****************************************************************************************************/
        static void handler_OnIam(BacnetClient sender, BacnetAddress adr, uint device_id, uint max_apdu, BacnetSegmentations segmentation, ushort vendor_id)
        {
            lock (DevicesList)
            {
                // Device already registred ?
                foreach (BacNode bn in DevicesList)
                    if (bn.getAdd(device_id) != null) return;   // Yes

                // Not already in the list
                DevicesList.Add(new BacNode(adr, device_id));   // add it
            }
        }

        /*****************************************************************************************************/
        static bool ReadScalarValue(int device_id, BacnetObjectId BacnetObjet, BacnetPropertyIds Propriete, out BacnetValue Value)
        {
            BacnetAddress adr;
            IList<BacnetValue> NoScalarValue;

            Value = new BacnetValue(null);

            // Looking for the device
            adr = DeviceAddr((uint)device_id);
            if (adr == null) return false;  // not found

            // Property Read
            if (bacnet_client.ReadPropertyRequest(adr, BacnetObjet, Propriete, out NoScalarValue) == false)
                return false;

            Value = NoScalarValue[0];
            return true;
        }

        /*****************************************************************************************************/
        static bool WriteScalarValue(int device_id, BacnetObjectId BacnetObjet, BacnetPropertyIds Propriete, BacnetValue Value)
        {
            BacnetAddress adr;

            // Looking for the device
            adr = DeviceAddr((uint)device_id);
            if (adr == null) return false;  // not found

            // Property Write
            BacnetValue[] NoScalarValue = { Value };
            if (bacnet_client.WritePropertyRequest(adr, BacnetObjet, Propriete, NoScalarValue) == false)
                return false;

            return true;
        }

        /*****************************************************************************************************/
        static BacnetAddress DeviceAddr(uint device_id)
        {
            BacnetAddress ret;

            lock (DevicesList)
            {
                foreach (BacNode bn in DevicesList)
                {
                    ret = bn.getAdd(device_id);
                    if (ret != null) return ret;
                }
                // not in the list
                return null;
            }
        }





    }


    class BacNode
    {

        public BacnetAddress adr;
        public uint device_id;

        public BacNode(BacnetAddress adr, uint device_id)
        {
            this.adr = adr;
            this.device_id = device_id;
        }

        public BacnetAddress getAdd(uint device_id)
        {
            if (this.device_id == device_id)
                return adr;
            else
                return null;
        }
    }
}
