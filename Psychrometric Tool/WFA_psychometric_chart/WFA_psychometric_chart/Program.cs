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

            if (File.Exists(databaseFile1))
            {
                //file exist so dont create the database      
            }
            else
            {
                //MessageBox.Show("Internal database not found. Creating fresh database");
                //this.Close();
                if (selectedBuildingFromT3000 != "")
                {
                    //--sqlite new databse creation
                    f1.sqlite_database_creation(selectedBuildingFromT3000);
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
