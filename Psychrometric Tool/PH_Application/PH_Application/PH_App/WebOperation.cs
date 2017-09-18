using System;
using System.Collections.Generic;
using System.Net;
using System.Windows.Forms;
using System.Xml;

namespace PH_App
{
    class WebOperation
    {

      public  class DataTypeForLatLongElevation
        {
            public double latValue { get; set; }
            public double lngValue { get; set; }
            public double elevation { get; set; }
        }

        public List<DataTypeForLatLongElevation> PullDataOnline(string country1, string state1, string city1, string street1)
        {
            //this function pulls the data from online devices...

            var listReturnValue =new List<DataTypeForLatLongElevation>();
            /*
            1.country,state,city,street,latitude,longitude,elev,zip
            */
            string country = country1;
            string state = state1;
            string city = city1;
            string street = street1;
            // string zip = zip1;
            int value;
            //  if (int.TryParse(zip, out value))
            // {

            if (country != "" && city != "")
            {
                string join_string = "";

                //--THIS IS DONE BECAUSE state,and street if present some api do not give response
                //if (state != "" && street != "")
                //{
                //    join_string = country + "," + state + "," + city + "," + street;
                //}
                //else
                //{
                join_string = country + "," + city;
                // }

                //geo location code goes here..
                try
                {

                    var address = join_string;

                    string BingMapsKey = "AgMVAaLqK8vvJe6OTRRu57wu0x2zBX1bUaqSizo0QhE32fqEK5fN8Ek4wWmO4QR4";

                    //Create REST Services geocode request using Locations API
                    string geocodeRequest = "http://dev.virtualearth.net/REST/v1/Locations/" + address + "?o=xml&key=" + BingMapsKey;

                    using (var wc = new WebClient())
                    {
                        string api_url = geocodeRequest;

                        var data = wc.DownloadString(api_url);
                        //  MessageBox.Show("string apic return =" + data);
                        string xml_string = data.ToString();
                        //MessageBox.Show(xml_string);


                        //--Parsing the xml document int the c# application...                     
                        //xml parsing...
                        XmlDocument xml = new XmlDocument();
                        xml.LoadXml(xml_string);

                      var responseValue =    ProcessResponse(xml);
                      double  latPulledValue = responseValue[0].latValue;//latVal.ToString();//--Storing it in this variable to make it a string...
                       double longPulledValue = responseValue[0].lngValue;//longVal.ToString();

                        //MessageBox.Show("lat = " + latPulledValue + "long = " + longPulledValue);

                        //--This is for the elevation part...
                        //string elevationAPI_URL = "http://dev.virtualearth.net/REST/v1/Elevation/List?pts=" + latVal + "," + longVal + "&key=AgMVAaLqK8vvJe6OTRRu57wu0x2zBX1bUaqSizo0QhE32fqEK5fN8Ek4wWmO4QR4&output=xml";
                        string elevationAPI_URL = "http://dev.virtualearth.net/REST/v1/Elevation/List?pts=" + latPulledValue + "," + longPulledValue + "&key=AgMVAaLqK8vvJe6OTRRu57wu0x2zBX1bUaqSizo0QhE32fqEK5fN8Ek4wWmO4QR4&output=xml";

                        var elevationData = wc.DownloadString(elevationAPI_URL);
                        // MessageBox.Show("elev data = " + elevationData);
                        //--Now lets do the parsing...
                        //xml parsing...
                        XmlDocument xmlElevation = new XmlDocument();
                        xmlElevation.LoadXml(elevationData);
                       double elevationPulledValue = elevationProcess(xmlElevation);//--This gives the elevation...

                        listReturnValue.Add(new DataTypeForLatLongElevation { latValue = latPulledValue, lngValue = longPulledValue, elevation = elevationPulledValue });
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }//close of if...
             // }//close of if int try parse.
             //else
             //{
             //    MessageBox.Show(Properties.Resources.Please_enter_a_valid_zip_numbe);
             //}
            return listReturnValue;
        }//close of  function


        public List<DataTypeForLatLongElevation> ProcessResponse(XmlDocument locationsResponse)
        {
            //Create namespace manager
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(locationsResponse.NameTable);
            nsmgr.AddNamespace("rest", "http://schemas.microsoft.com/search/local/ws/rest/v1");
            double latVal = 0, longVal = 0 ;
            
            //Get formatted addresses: Option 1
            //Get all locations in the response and then extract the formatted address for each location
            XmlNodeList locationElements = locationsResponse.SelectNodes("//rest:Location", nsmgr);
            Console.WriteLine("Show all formatted addresses: Option 1");
            foreach (XmlNode location in locationElements)
            {
                //MessageBox.Show("Lat = "+location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                latVal = double.Parse(location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                // MessageBox.Show("lat = " + latVal);
                longVal = double.Parse(location.SelectSingleNode(".//rest:Longitude", nsmgr).InnerText);
                // MessageBox.Show("Long = " + longVal);
            }
            //   Console.WriteLine();
            var value = new List<DataTypeForLatLongElevation>();
            value.Add(new DataTypeForLatLongElevation
            {
                latValue = latVal,
                lngValue = longVal

            });

            return value;
        }//Close of function

      
        public double elevationProcess(XmlDocument locationsResponse)
        {
            double elev=0;
            //Create namespace manager
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(locationsResponse.NameTable);
            nsmgr.AddNamespace("rest", "http://schemas.microsoft.com/search/local/ws/rest/v1");

            //Get formatted addresses: Option 1
            //Get all locations in the response and then extract the formatted address for each location
            XmlNodeList locationElements = locationsResponse.SelectNodes("//rest:Elevations", nsmgr);
            //Console.WriteLine("Show all formatted addresses: Option 1");
            foreach (XmlNode location in locationElements)
            {
                //MessageBox.Show("Lat = "+location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                elev = double.Parse(location.SelectSingleNode(".//rest:int", nsmgr).InnerText);
                //  MessageBox.Show("elev = " + elev);
                //longVal = double.Parse(location.SelectSingleNode(".//rest:Longitude", nsmgr).InnerText);
                //MessageBox.Show("Long = " + longVal);
            }

            return elev;
        }




    }
}
