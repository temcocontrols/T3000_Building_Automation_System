using System;
using System.Configuration;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace PH_App
{
    class MainController :DatabaseOperationModel
    {
       // public double AirPressureFromDB = 0;
        public double AirPressureFromDB { get; set; }
        public string CurrentSelectedBuilding { get; set; }
        public string PathToT3000BuildingDB { get; set; }

        public void LoadForPH(string fluidName, PH_App.Form_Main_PH_Application f)
        {
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];//databasePath1 + @"\db_psychrometric_project.s3db";

           // PH_Application.Form_Main_PH_Application f = new PH_Application.Form_Main_PH_Application();

            if (checkForDataInSqlite() != true)
            {
                MessageBox.Show("Please Configure the building location in T3000 first and restart the application again");              
                Application.Exit();
                return;
            }//Close of if
            //--Web calibration
            //WebInfoCalibration();
            //--Pressure calibration
           // PressureCalibrationForBuilding();

            //--Plotting of the chart Here --------//
            string fluidNAME = fluidName;//"Water";//"1-Butene"; // "Water";//"n-Propane";//Acetone//Ammonia//Krypton//Nitrogen //Note for Air not working //Argon//CarbonDioxide// not working p-Xylene//R134a
            ChartCreation ch = new ChartCreation();
            ch.PlotPHChart(fluidNAME, f.phChart);

            //--End of the plotting chart here-----//
            //--Other task here
           
           string buildingNameValue= BuildingSelectionTableCreationReturnBuildingName(f);
            //=========Building click and settings====//
            BuildingConfigurationSetting(f);
            //--Assing the location path to alex database 
            PathToT3000BuildingDB = CalcPathToT300Building(buildingNameValue);

            //== For date generation
            f.Text = "Psychometric Chart [" + AssemblyDateGeneration.Value.ToShortDateString() + "]";
        }

        public void WebInfoCalibration()
        {
            PullLocationInformation();

            if (CheckLatLongAvailable() == true)
            {
                FillLatLongValueAutomatically();//--Fill the lat long values...
                                                //--MessageBox.Show("show filllat");
            }
        }

        public void PressureCalibrationForBuilding()
        {
            get_stored_data_about_building();
            //We have formula for altitude and pressure calculation
            /*
            #formula:
            P= 101325(1-2.25577*10^(-5)*h)^5.25588
            where p = air pressure in pa 
             h = altitude in meteres
            */
            if (buildingList[0].elevation != "")
            {
                double altitue = double.Parse(buildingList[0].elevation);
                double P = 101325 * Math.Pow((1 - (2.25577 * Math.Pow(10, -5) * altitue)), 5.25588);
                if (P == 0 || P.ToString() == "")
                {
                    AirPressureFromDB = 101325;//in terms of pa
                }
                else
                {
                    AirPressureFromDB = P;
                }
            }
            else
            {
                AirPressureFromDB = 101325;//in terms of pa }
            }
        }
        public string BuildingSelectionTableCreationReturnBuildingName(PH_App.Form_Main_PH_Application f)
        {
            CheckSelectedBuilding();
            string buildingNameValue = selectedBuildingList[0].BuildingName;
            if (buildingList.Count > 0) { 
            f.lb_unit_chosen_display.Text = "Unit : " + buildingList[0].EngineeringUnits;
            }
            f.lb_db_name.Text = buildingNameValue;
            if (selectedBuildingList.Count > 0) { 
            CurrentSelectedBuilding = selectedBuildingList[0].BuildingName;
            }
            CreateRequireTableIfNotPresent(buildingNameValue);
            return buildingNameValue;
        }

        public string CalcPathToT300Building(string buildingNameValue)
        {
            string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);           
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            string againNewPath = newPath + @"Database\Buildings\" + buildingNameValue + @"\" + buildingNameValue + ".db";  //psychopath+ database\Buildings\"BuildingName"\"BuildingName.db"           
            return againNewPath;
        }
        public void BuildingConfigurationSetting(PH_App.Form_Main_PH_Application f)
        {
            DataGridView_Show_Data(f);
            f.dataGridView1.Rows.Add();
            f.phChart.Enabled = false;
         
            if (f.dataGridView1.Rows.Count > 0 && chartDetailList.Count >0)  //If there is data then only do this one
            {

                if (f.dataGridView1.CurrentCell.RowIndex >= 0 && f.dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                {
                    var eventArgs = new DataGridViewCellEventArgs(f.dataGridView1.CurrentCell.ColumnIndex, f.dataGridView1.CurrentCell.RowIndex);
                    f.dataGridView1.Rows[f.dataGridView1.CurrentCell.RowIndex].Cells[f.dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                    //f.dataGridView1_CellClick(sender, eventArgs);
                    //MessageBox.Show("selecteion change clicked");
                }
                else
                {
                    var eventArgs = new DataGridViewCellEventArgs(1, 0);
                    // or setting the selected cells manually before executing the function
                    f.dataGridView1.Rows[0].Cells[1].Selected = true;
                    //f.dataGridView1_CellClick(sender, eventArgs);
                   
                }
            }

        }

        public void DataGridView_Show_Data(PH_App.Form_Main_PH_Application f1)
        {
            if (selectedBuildingList.Count <= 0)
            {
                //We no need to proceed futher 
                return;
            }
            //Pulling the data form the data grid view...
            PullChartList(selectedBuildingList[0].BuildingName);//This is the list of the building present ...
            
            fillDataGridView(f1);
        

        }
        private void fillDataGridView(PH_App.Form_Main_PH_Application f1)
        {

            //dataGridView1.Rows.Clear();
            int xCount = 0;
            if (chartDetailList.Count > 0)
            {
                for (int i = 0; i < chartDetailList.Count; i++)
                {
                    if (chartDetailList[i].enableChartStatus == "true") //when enable then only display
                    {
                        f1.dataGridView1.Rows.Add();
                        f1.dataGridView1.Rows[i].Cells["ID"].Value = ++xCount;//chartDetailList[i].;
                        f1.dataGridView1.Rows[i].Cells["Name"].Value = chartDetailList[i].chartName;
                    }
                }
                //dataGridView1.DataBindingComplete += new DataGridViewBindingCompleteEventHandler(dataGridView1.DataBindingComplete);

            }//--if close
            //dataGridView1.ClearSelection();

            f1.lb_numberOfHandler.Text = chartDetailList.Count.ToString();


        }//--close fo filldatagridview


    }
}
