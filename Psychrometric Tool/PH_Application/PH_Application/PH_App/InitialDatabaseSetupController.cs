/*
 * Project : PH application
 * Author Name : Bhoj bahadur karki
 * Date : 2017-July-4th 
 * Contact : nishantkarki2013@hotmail.com
 */
using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Reflection;
using System.Windows.Forms;


namespace PH_App
{
    /// <summary>
    /// this will perform all the operations related to initital database operations
    /// </summary>
    public class InitialDatabaseSetupController:MainController//DatabaseOperationModel
    {

        //public int SelectedSettingsForUserOption { get; set; }
        public void CheckingAndUpdatingBuildingDataFromT3000ToPsychro(string buildingNameFromT3000, string buildingNameFromPsychro)
        {
            // Form1_main f1 = new Form1_main();
            // var db_o = new DatabaseOperationModel();
            List<SelectedBuildingDatatype> selectedBuildingInT300 = new List<SelectedBuildingDatatype>();
            //Just a check
            if (buildingNameFromT3000 == "" || buildingNameFromPsychro == "")
            {
                //MessageBox.Show($"Values are empty T300building ={buildingNameFromT3000} , and psy = {buildingNameFromPsychro}");
                return;
            }

            if (buildingNameFromPsychro == buildingNameFromT3000)
            {
                //--Matched then select this one
                SelectBuildingInPsychro(buildingNameFromT3000);
                // MessageBox.Show($"Matched db selected  = { buildingNameFromPsychro}");

            }
            else
            {
                //--Not matched so first check data present or not to select
                if (CheckDataPresentInPsychro(buildingNameFromT3000))
                {
                    // MessageBox.Show("Data present in psycho CheckDataPresentInPsychro is true");

                    //--Data present, only selection
                    SelectBuildingInPsychro(buildingNameFromT3000);

                }
                else
                {
                    //--False
                    selectedBuildingInT300 = FindPathOfBuildingInT3000();  //f1.FindPathOfBuildingDBNewVersion();
                    if (selectedBuildingInT300[0].country != "" && selectedBuildingInT300[0].Building_Name != "")
                    {
                        //  MessageBox.Show("country and building name not empty f1.BuildingSelected[0].country !=  f1.BuildingSelected[0].Building_Name != is true");

                        WriteT3000BuildingInfoToPsychoDB("1", selectedBuildingInT300[0].country, selectedBuildingInT300[0].state, selectedBuildingInT300[0].city, selectedBuildingInT300[0].street, selectedBuildingInT300[0].longitude, selectedBuildingInT300[0].latitude, selectedBuildingInT300[0].elevation, selectedBuildingInT300[0].Building_Name, selectedBuildingInT300[0].EngineeringUnits);
                        SelectBuildingInPsychro(selectedBuildingInT300[0].Building_Name);
                    }
                    else
                    {
                        //--Building info is not present
                        string text = "Current selected building location information is not provided. Input the location information in T3000 in building configuration section.\nDo you wish to continue with default settings?";
                        if (MessageBox.Show(text, "System configuration warning", MessageBoxButtons.YesNo) == DialogResult.Yes)
                        {
                            //--Yes is click continue with the default settings.
                            string buildingName = "Default_Building";
                            SelectBuildingInPsychro(buildingName);
                        }
                        else
                        {
                            //MessageBox.Show("Application close ");
                            //--NO selected
                            //f1.Close();//just close the form and the application
                            Environment.Exit(0);
                            //Application.Exit();                         
                        }
                    }
                }
            }

        }

        public bool CheckDataPresentInPsychro(string buildingNameFromT3000)
        {


            var db_o = new DatabaseOperationModel();
            db_o.ReadAllBuildingInfoFromPsychrometric();//Reads all data
            bool retVal = false;

            foreach (var item in db_o.listOfAllBuildingInPsychro)
            {
                if (buildingNameFromT3000 == item.BuildingName)
                {
                    retVal = true;
                    break;
                }
            }


            return retVal;
        }

        public void InitialProcessingForDatabaseAndApp()
        {
            //==Before any thign lets run this code because we need language values which is set by this =================//
            //Form1_main f1 = new Form1_main();
            // var idb = new DatabaseOperationModel();
            //List<SelectedBuildingDatatype> buildingSelected = new List<SelectedBuildingDatatype>();

            buildingSelected = FindPathOfBuildingInT3000();
            string selectedBuildingFromT3000 = buildingSelected[0].Building_Name;//This stores the building name selected in alex part

            //string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";

            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

            //var db_op = new DatabaseOperation();
            if (File.Exists(databaseFile))
            {
                //file exist so dont create the database 

                //MessageBox.Show("File exist section");

                sqlite_database_creation_For_UpdateCondition(selectedBuildingFromT3000, buildingSelected[0].country, buildingSelected[0].state, buildingSelected[0].city, buildingSelected[0].street, buildingSelected[0].longitude, buildingSelected[0].latitude, buildingSelected[0].elevation, buildingSelected[0].EngineeringUnits);
                //MessageBox.Show("finish of sqlite_database_creation_For_UpdateCondition ");

                //--This one is for checking if the data is present or not if not presnet then create
                ReadDataForBuildingSelectedFromPsychrometric(); //--We find selected building in Psychro metric database

                // BuildingIdentificationClass bic = new BuildingIdentificationClass();
                if (selectedBuildingList[0].BuildingName != "")
                {
                    CheckingAndUpdatingBuildingDataFromT3000ToPsychro(selectedBuildingFromT3000, selectedBuildingList[0].BuildingName);
                }
                else
                {
                    //The previous building selected in Psy is empty 
                    MessageBox.Show("Internal Error no building is selected. Program will run default building settings");
                    UpdateIDOneSelectBuildingInPsychro();
                    SelectBuildingInPsychro("Default_Building");//Selecting the default building

                }                
            }
            else
            {
                //MessageBox.Show("Internal database not found. Creating fresh database");
                //this.Close();
                if (selectedBuildingFromT3000 != "")
                {
                    //--sqlite new databse creation
                    if (buildingSelected[0].Building_Name != "" && buildingSelected[0].country != "" && buildingSelected[0].city != "")
                    {
                        sqlite_database_creation(selectedBuildingFromT3000, buildingSelected[0].country, buildingSelected[0].state, buildingSelected[0].city, buildingSelected[0].street, buildingSelected[0].longitude, buildingSelected[0].latitude, buildingSelected[0].elevation, buildingSelected[0].Building_Name, buildingSelected[0].EngineeringUnits);
                    }
                    else
                    {
                        if (MessageBox.Show("Please enter the building location information first in T300 and then restart the application.\n Do you want to continue with the default settings", "Chose an option", MessageBoxButtons.YesNo) == DialogResult.Yes)
                        {
                            string buildingName = "Default_Building";
                            //--Seprate code is required because first time when application starts no
                            //--Building info provided so we have to go with default settings
                            sqlite_database_creationWithDefaultSettingsOnly(buildingName);//Passing BuildingName
                            //Previous code doesnot selecte default building so to select default building do this on
                            SelectBuildingInPsychro(buildingName);
                        }
                        else
                        {
                            //string databasePath133 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                            //string databaseFile = databasePath133 + @"\db_psychrometric_project.s3db";
                            //string databaseFile1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";
                            if (File.Exists(databaseFile))
                            {
                                File.Delete(databaseFile);
                            }
                            Environment.Exit(0);
                            Application.Exit();
                        }//Close of else
                    }
                }
                else
                {
                    //End the application
                    MessageBox.Show("Could not create new databse.Closing!");
                    Application.Exit();//Close the application
                }
            }
        }

        public void sqlite_database_creation_For_UpdateCondition(string buildingNameSelected, string country, string state, string city, string street, string longitude, string latitude, string elevation, string EngineeringUnits)
        {
            //--lets do try catch
            //try
            //{
            //var idb = new DatabaseOperationModel();
            string tableName = "tbl_Database_Version";//"tbl_" + buildingNameSelected + "_Database_Version";
            string versionValue = ReadVersionInfo(tableName);//Calling the function 
            if (versionValue != "")
            {
                DateTime dt = DateTime.Parse(versionValue);
                if (dt < AssemblyDateGeneration.Value) //--Comparing the datetime value
                {
                    // MessageBox.Show("Here we are inisde new creations");
                    //--Operations are required
                    CreateRequiredTable(buildingNameSelected);//Those table which are not present are only created

                    ReadDataPresentOrNotThenInsertBuildingValue(country, state, city, street, longitude, latitude, elevation, buildingNameSelected, EngineeringUnits);
                    //After changes lets update the version info
                    UpdateVersionInfoAfterUpdating(AssemblyDateGeneration.Value.ToShortDateString());
                }
            }
            //}
            //catch (Exception ex)
            //{
            // MessageBox.Show(ex.Message);
            //}         
        }
        
    }
}
