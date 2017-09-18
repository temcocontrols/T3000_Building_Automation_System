/*
 * Project : PH application
 * Author Name : Bhoj bahadur karki
 * Date : 2017-July-4th 
 * Contact : nishantkarki2013@hotmail.com
 */
namespace PH_App
{
    class DeviceAndParameterProcess
    {
     /// <summary>
     /// function to take input of controller and variable name and id and scan the hardware values
     /// </summary>
     /// <param name="controllerInstanceID"></param>
     /// <param name="variableName"></param>
     /// <param name="variableNumber"></param>
        public double InputDeviceInfoAndValue(int controllerInstanceID,string variableName, int variableNumber)
        {                    
            /*
             Task by this function : 
             1. Filter which object to call.
             2. call a function and a value            
             */
            string inputNameForYabee = "";
            if(variableName == "VAR")
            {
                // ANALOG_INPUT_VALUE
                inputNameForYabee = "OBJECT_ANALOG_VALUE";
            }
            else if (variableName == "IN")
            {
                // ANALOG_INPUT_INPUT
                inputNameForYabee = "OBJECT_ANALOG_INPUT";
            }
            else if (variableName == "OUT")
            {
                //variableName == OUT so for this we have
                inputNameForYabee = "OBJECT_ANALOG_OUTPUT";

            }
            double valueFromDevice = 0;
            var db = new DeviceConnection();
            lock (db)
            { 
            db.ScanForDevice();
            //==Now calling yabee for this values
            var bc = new BACnetClass();
            valueFromDevice = bc.ReadSingleParameterUsigBacnetID(controllerInstanceID, inputNameForYabee, variableNumber);
             db.ScanForDevice();

            }
            //db.ScanForDevice();
            return valueFromDevice;
        }

        /// <summary>
        /// Not used
        /// </summary>
        /// <returns></returns>
        public double returnedValueFromYabee()
        {
            return 0;
        }

    }
}
