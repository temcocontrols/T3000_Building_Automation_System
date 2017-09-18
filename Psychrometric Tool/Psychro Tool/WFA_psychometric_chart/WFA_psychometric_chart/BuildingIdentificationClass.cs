using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    class BuildingIdentificationClass 
    {
        public int SelectedSettingsForUserOption { get; set; }
        public void CheckingAndUpdatingBuildingDataFromT3000ToPsychro(string buildingNameFromT3000,string buildingNameFromPsychro)
        {
            Form1_main f1 = new Form1_main();
            DatabaseOperation db_o = new DatabaseOperation();
            //Just a check
            if(buildingNameFromT3000 == "" || buildingNameFromPsychro == "")
            {
                //MessageBox.Show($"Values are empty T300building ={buildingNameFromT3000} , and psy = {buildingNameFromPsychro}");
                return;
            }

            if(buildingNameFromPsychro == buildingNameFromT3000)
            {
                //--Matched then select this one
                db_o.SelectBuildingInPsychro(buildingNameFromT3000);
               // MessageBox.Show($"Matched db selected  = { buildingNameFromPsychro}");

            }
            else
            {
                //--Not matched so first check data present or not to select
                if (CheckDataPresentInPsychro(buildingNameFromT3000))
                {
                   // MessageBox.Show("Data present in psycho CheckDataPresentInPsychro is true");

                    //--Data present, only selection
                    db_o.SelectBuildingInPsychro(buildingNameFromT3000);

                }else
                {
                    //--False
                    f1.FindPathOfBuildingDBNewVersion();
                    if (f1.BuildingSelected[0].country != "" && f1.BuildingSelected[0].Building_Name !="")
                    {
                      //  MessageBox.Show("country and building name not empty f1.BuildingSelected[0].country !=  f1.BuildingSelected[0].Building_Name != is true");

                        f1.WriteT3000BuildingInfoToPsychoDB("1", f1.BuildingSelected[0].country, f1.BuildingSelected[0].state, f1.BuildingSelected[0].city, f1.BuildingSelected[0].street, f1.BuildingSelected[0].longitude, f1.BuildingSelected[0].latitude, f1.BuildingSelected[0].elevation, f1.BuildingSelected[0].Building_Name, f1.BuildingSelected[0].EngineeringUnits);
                        db_o.SelectBuildingInPsychro(f1.BuildingSelected[0].Building_Name);
                    }
                    else
                    {
                        //--Building info is not present
                        string text = "Current selected building location information is not provided. Input the location information in T3000 in building configuration section.\nDo you wish to continue with default settings?";
                        if (MessageBox.Show(text,"System configuration warning",MessageBoxButtons.YesNo)== DialogResult.Yes)
                        {
                            //--Yes is click continue with the default settings.
                            string buildingName = "Default_Building";
                            db_o.SelectBuildingInPsychro(buildingName);
                        }
                        else
                        {
                            //MessageBox.Show("Application close ");
                            //--NO selected
                            f1.Close();//just close the form and the application
                            Environment.Exit(0);
                            //Application.Exit();                         
                        }
                    }
                }

            }


        }

        public bool CheckDataPresentInPsychro(string buildingNameFromT3000)
        {


            DatabaseOperation db_o = new DatabaseOperation();
            db_o.ReadAllBuildingInfoFromPsychrometric();//Reads all data
            bool retVal = false;

            foreach (var item in db_o.listOfAllBuildingInPsychro)
            {
               if(buildingNameFromT3000 == item.BuildingName)
                {
                    retVal = true;
                    break;
                }
            }


            return retVal;
        }


    }
}
