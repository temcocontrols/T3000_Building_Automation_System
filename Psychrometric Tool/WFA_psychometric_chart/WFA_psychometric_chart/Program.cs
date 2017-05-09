using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);


           
            //==Before any thign lets run this code because we need language values which is set by this =================//
            Form1_main f1 = new Form1_main();
            f1.FindPathOfBuildingDBNewVersion();
            string selectedBuildingFromT3000 = f1.BuildingSelected[0].Building_Name;//This stores the building name selected in alex part
            
            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";
            DatabaseOperation db_op = new DatabaseOperation();
            if (File.Exists(databaseFile1))
            {
                //file exist so dont create the database 


                //MessageBox.Show("File exist section");
               
                db_op.sqlite_database_creation_For_UpdateCondition(selectedBuildingFromT3000);
                //MessageBox.Show("finish of sqlite_database_creation_For_UpdateCondition ");

                //--This one is for checking if the data is present or not if not presnet then create
                db_op.ReadDataForBuildingSelectedFromPsychrometric(); //--We find selected building in Psychro metric database

                BuildingIdentificationClass bic = new BuildingIdentificationClass();
                if(db_op.listBuildingSelectedInfo[0].BuildingName != "")
                { 
                bic.CheckingAndUpdatingBuildingDataFromT3000ToPsychro(selectedBuildingFromT3000, db_op.listBuildingSelectedInfo[0].BuildingName);
                }else
                {
                    //The previous building selected in Psy is empty 
                    MessageBox.Show("Internal Error no building is selected. Program will run default building settings");
                    db_op.UpdateIDOneSelectBuildingInPsychro();
                    db_op.SelectBuildingInPsychro("Default_Building");//Selecting the default building

                }



            }
            else
            {
                //MessageBox.Show("Internal database not found. Creating fresh database");
                //this.Close();
                if (selectedBuildingFromT3000 != "")
                {
                    //--sqlite new databse creation

                    if (f1.BuildingSelected[0].Building_Name != "" && f1.BuildingSelected[0].country != "" && f1.BuildingSelected[0].city != "")
                    {
                    f1.sqlite_database_creation(selectedBuildingFromT3000);
                    }
                    else
                    {
                       if( MessageBox.Show("Please enter the building location information first in T300 and then restart the application.\n Do you want to continue with the default settings","Chose an option",MessageBoxButtons.YesNo)==DialogResult.Yes)
                        {
                            string buildingName = "Default_Building";
                            //--Seprate code is required because first time when application starts no
                            //--Building info provided so we have to go with default settings
                            f1.sqlite_database_creationWithDefaultSettingsOnly(buildingName);//Passing BuildingName
                            //Previous code doesnot selecte default building so to select default building do this on
                            db_op.SelectBuildingInPsychro(buildingName);

                        }
                        else
                        { 
                        string databasePath133 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                        string databaseFile = databasePath133 + @"\db_psychrometric_project.s3db";

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
           //===================End of this section=======================//

            //Show the language select dialog
            MultiLang.SelectLanguage frmLang = new MultiLang.SelectLanguage();
            //MessageBox.Show("frm lang" + frmLang);
            frmLang.LoadSettingsAndShow();
            frmLang.Dispose();
            frmLang = null;

            Application.Run(new Form1_main());
        }
    }
}
