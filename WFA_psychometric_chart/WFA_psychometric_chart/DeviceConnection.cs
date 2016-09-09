using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace WFA_psychometric_chart
{
    class DeviceConnection
    {
        //Now here we are going to get the connected device value

        ////Lets build the function we need to do the task 
        //public class ca{
        //      public  string deviceName { get; set; }
        //    public string deviceInstance { get; set; }
        //    public string ConnectedPort { get; set; }
        //}
       
        /// <summary>
        /// This function is used to scan bacnet devices  connected 
        /// </summary>
        /// <returns>returns the list of devices</returns>
      public  List<BacNode> ScanForDevice()
        {
            //List<BacNode> DevicesList = new List<BacNode>();

            try
            {
            BACnetClass.StartActivity();
                //Console.WriteLine("Started");

                Thread.Sleep(3000); // Wait a fiew time for WhoIs responses (managed in handler_OnIam)
               
             

            }
            catch { }

            return BACnetClass.DevicesList;
                
        }


        //========================================Device return value of the and id of the selected ====================//
 
        public List<BACnetClass.parameter_class> parameterListValue = new List<BACnetClass.parameter_class>();
        /// <summary>
        /// This used to get the values form the device id
        /// </summary>
        /// <param name="deviceID"></param>
        public List<BACnetClass.parameter_class> ScanForParameters(int deviceID)
        {
            parameterListValue.Clear();//Resetting the values
            parameterListValue = BACnetClass.ReadParameterUsingBacnetID(deviceID);

            return parameterListValue;     
        }

        //========================================End of the device retrun....=========================================//

        

    }
}
