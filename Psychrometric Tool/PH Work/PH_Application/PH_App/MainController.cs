using System;
using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using Excel = Microsoft.Office.Interop.Excel;


namespace PH_App
{
    /// <summary>
    /// Only contains the methods which will be called from GUI Application
    /// </summary>
   public class MainController :DatabaseOperationModel
    {

      
        
        public void LoadForPH(string fluidName, Form_Main_PH_Application f)
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
            //var ch = new ChartCreationAndOperations();
            PlotPHChart(fluidNAME, f.phChart);

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
        public string BuildingSelectionTableCreationReturnBuildingName(Form_Main_PH_Application f)
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
        public void BuildingConfigurationSetting(Form_Main_PH_Application f)
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

        public void DataGridView_Show_Data(Form_Main_PH_Application f1)
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
        private void fillDataGridView(Form_Main_PH_Application f1)
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

        //***********************************************************************************//
        //======================ChartCreationAndOperation.cs FILE code starts=============================//
        
        /// <summary>
        /// These are the properties of the chart asixes
        /// </summary>
        public double xAxisMinimum = 0;
        public double xAxisMaximum = 4000;
        public double yAxisMinimum = 0;//1 / 10000;
        public double yAxisMaximum = 100;
        double radiusSize = 0.25;//initally 0.25


        public class DataTypeForPH_CurveData
        {

            public int i_index { get; set; }
            public int j_index { get; set; }
            public double Value { get; set; }


        }

        List<DataTypeForPH_CurveData> psatUnfilterList = new List<DataTypeForPH_CurveData>();
        public double[,] P_Value = new double[300, 300];
        public double[,] T_Value = new double[300, 300];
        public double[,] H_Value = new double[300, 300];

        public void PlotPHChart(string fluidName, Chart phChart)
        {

            MathOperation PH = new MathOperation();

            /*
             Lets do it step by step as the way that has been done in the Matlab

             */
            //--1. press_rng = logspace(-2,2,300); % [MPa] pressure (p) range

            // var stopWatch = System.Diagnostics.Stopwatch.StartNew();

            //double[] press_rng = PH.LogSpace(-2, 2, 300,true,10).ToArray();
            double[] press_rng = PH.LogSpace(-3, 2, 300, true, 10).ToArray();// initially it was form -2,2,300 //-4,4,300 is not supported by coolprop
            //--For line spacing values
            //2. temp_rng = 273.15+linspace(1,800,300); 
            //double[] temp_rng =  PH.LinSpace(1, 800, 300, true).ToArray();

            //--Here -4,800,300 doesnot work error by cool prop 0 onwards on negative axis(eg.0,-1,-2,-3,...) does not work
            double[] temp_rng = PH.LinSpace(1, 800, 300, true).ToArray();// changed from 1,800,300 


            for (int i = 0; i < temp_rng.Length; i++)
            {
                temp_rng[i] += 273.15;
            }


            //--Now for meshgrid we have created truple
            //MeshGrid_Class ms1 = new MeshGrid_Class();
            //3.[p,T] = meshgrid(press_rng,temp_rng); % [MPa,K] mesh p & T

            MeshGridClass.MeshGrid(press_rng, temp_rng);

            Tuple<double[,], double[,]> tuple_x = MeshGridClass.MeshGrid(press_rng, temp_rng);

            double[,] p = tuple_x.Item1;
            double[,] T = tuple_x.Item2;

            //--8 pcrit = 22.064; % [MPa] critical pressure
            double pcrit = CoolProp.Props1SI(fluidName, "Pcrit") / 1000000;//To convert to Pa form MPa //22.064;//CoolProp.Props1SI( "Water", "Pcrit");//PropsSI("Pcrit", "", 0, "", 0, "Water");//PH.IAPWS_IF97_TowParameterEquivalentFxn("Pcrit", "", 0, "",0, fluidName); //CoolProp.Props1SI(fluidName, "Pcrit");// CP.PropsSI("Tcrit","",0,"",0,"Water")

            //--Now lets do the calculation for others 
            //4.h = IAPWS_IF97('h_pT',p,T); % [kJ/kg] enthalpy = f(p,T)
            P_Value = p;//Copy
            T_Value = T;//Copy


            double[,] h = new double[p.GetLength(0), p.GetLength(1)];

            for (int i = 0; i < h.GetLength(0); i++)
            {
                for (int j = 0; j < h.GetLength(1); j++)
                {

                    double enthalpy = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", p[i, j] * 1000000, "T", T[i, j], fluidName) / 1000; //--This multiply is done to convert MPa to Pa and enthlapy is divided to convert J/kg to kJ/Kg
                    h[i, j] = enthalpy;

                }
            }
            H_Value = h;//Copy

            //--5 psat = IAPWS_IF97('psat_T',temp_rng); % [MPa] saturation pressure
            // 6. also trimming is done in this section
            List<double> psatList = new List<double>();
            psatList.Clear();

            //--Finding boundary condition i.e maximun temperature range and minimum temperature range for different fluid
            //--eg. For Water min to max is 273.06K - 647.096K [in kelvin]


            double maxCriticalTemperature = 0;
            //--For critical CP.PropsSI("Tcrit","",0,"",0,"Water")
            maxCriticalTemperature = PH.IAPWS_IF97_TowParameterEquivalentFxn("Tcrit", "", 0, "Q", 0, fluidName);

            for (int i = 0; i < temp_rng.Length; i++)
            {

                if (((temp_rng[i] <= maxCriticalTemperature)))
                {

                    double psat = PH.IAPWS_IF97_TowParameterEquivalentFxn("P", "T", temp_rng[i], "Q", 0, fluidName) / 1000000;
                    psatList.Add(psat);
                } //Close of if statement
            }//Close of for


            //-- 6. psat = psat(~isnan(psat)); 
            //--Use dynamic list to trim data which are empty rather than array

            //--6 . hLsat = IAPWS_IF97('hL_p',psat); % [kJ/kg] saturated liquid enthalpy
            List<double> hLsatList = new List<double>();
            hLsatList.Clear();

            foreach (var item in psatList)
            {
                double hLsat = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", item * 1000000, "Q", 0, fluidName) / 1000;//Q=0 for liquid
                hLsatList.Add(hLsat);
            }

            //--7 . hVsat = IAPWS_IF97('hV_p',psat); % [kJ/kg] saturated vapor enthalpy
            List<double> hVsatList = new List<double>();
            hVsatList.Clear();
            foreach (var item in psatList)
            {
                double hVsat = CoolProp.PropsSI("H", "P", item * 1000000, "Q", 1, fluidName) / 1000;      // PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", item, "Q", 1.0, fluidName);//Zero for vapor
                hVsatList.Add(hVsat);

            }

            //--8. already declared

            //--9 hLcrit = IAPWS_IF97('hL_p',pcrit);
            double hLcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit * 1000000, "Q", 0, fluidName) / 1000;//Zero for liquid

            //--10 . hVcrit = IAPWS_IF97('hV_p',pcrit);
            double hVcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit * 1000000, "Q", 1, fluidName) / 1000;//Zero for liquid
            //--11. Tcrit = IAPWS_IF97('Tsat_p',pcrit);
            double Tcrit = maxCriticalTemperature; // CoolProp.PropsSI("Tcrit", "", 0, "", 0, "Water");//PH.IAPWS_IF97_TowParameterEquivalentFxn("Tcrit", "", 0, "", 0, fluidName);//CoolProp.Props1SI(fluidName, "Tcrit");
            //--12. hcrit = IAPWS_IF97('h_pT',pcrit,Tcrit);
            double hcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit * 1000000, "T", Tcrit, fluidName) / 1000;//Zero for liquid
                                                                                                                            //--13.hVL = hVsat - hLsat; % [kJ/kg] heat of vaporization
            List<double> hVL_List = new List<double>();
            hVL_List.Clear();
            for (int i = 0; i < hVsatList.Count; i++)
            {
                double subtracted_value = hVsatList[i] - hLsatList[i];
                hVL_List.Add(subtracted_value);
            }

            //--14. hX = hLsat*ones(1,9) + hVL*(0.1:0.1:0.9); % [kJ/kg] mixture enthalpy

            double[,] hX = new double[hLsatList.Count, 9];
            double[,] hVL_LeftSideValue = new double[hLsatList.Count, 9];//stores hVL*(0.1:0.1:0.9);
            double[,] hLsat_RightSideValue = new double[hLsatList.Count, 9];//stores hLsat*ones(1,9) 

            double stepVal = 0.1;
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < hVL_List.Count; j++)
                {
                    hVL_LeftSideValue[j, i] = hVL_List[j] * stepVal;

                }
                stepVal += 0.1;

            }

            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < hLsatList.Count; j++)
                {
                    hLsat_RightSideValue[j, i] = hLsatList[j];

                }


            }

            //For final sum
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < hLsatList.Count; j++)
                {
                    hX[j, i] = hLsat_RightSideValue[j, i] + hVL_LeftSideValue[j, i];//  hLsatList[j].Value;

                }

            }

            //--Now lets move to plotting the things
            //Conture plot and
            /*
             For this function plot 
             plot([hLsat;hLcrit],[psat;pcrit],'b', ...
                    [hVsat;hVcrit],[psat;pcrit],'r', ...
                    hcrit,pcrit,'o', ...
                    hX,psat*ones(1,9),'g') % vapor dome                    
             */

            phChart.Series.Clear();
            phChart.ChartAreas[0].AxisX.Minimum = 1 / 1000; //--This was 1/1000
            phChart.ChartAreas[0].AxisX.Maximum = 4000; //4000;
            phChart.ChartAreas[0].AxisX.Interval = 500;

            phChart.ChartAreas[0].AxisY.Minimum = 0.01; //--This was 0.001
            phChart.ChartAreas[0].AxisY.IsLogarithmic = true;
            phChart.ChartAreas[0].AxisY.LogarithmBase = 10;
            phChart.ChartAreas[0].AxisY.Interval = 1;
            // phChart.ChartAreas[0].AxisY.Maximum = 50;

            //--this one is for [hLsat;hLcrit],[psat;pcrit],'b', ...
            // ph_chart.ChartAreas[0].AxisX.Minimum =

            phChart.Series.Clear();

            phChart.Series.Add("Series01");
            phChart.Series["Series01"].ChartType = SeriesChartType.Line;
            for (int i = 0; i < hLsatList.Count; i++)
            {
                phChart.Series["Series01"].Points.AddXY(hLsatList[i], psatList[i]);
            }
            phChart.Series["Series01"].Points.AddXY(hLcrit, pcrit);
            phChart.Series["Series01"].Color = Color.Blue;
            phChart.Series["Series01"].Font = new Font(FontFamily.GenericSansSerif, 8, FontStyle.Bold);
            //phChart.Series["Series01"].Font.

            // /*
            phChart.Series["Series01"].Points[12].Label = "S";
            phChart.Series["Series01"].Points[15].Label = "a";
            phChart.Series["Series01"].Points[18].Label = "t";
            phChart.Series["Series01"].Points[21].Label = "u";
            phChart.Series["Series01"].Points[24].Label = "r";
            phChart.Series["Series01"].Points[27].Label = "a";
            phChart.Series["Series01"].Points[30].Label = "t";
            phChart.Series["Series01"].Points[33].Label = "i";
            phChart.Series["Series01"].Points[36].Label = "o";
            phChart.Series["Series01"].Points[40].Label = "n";

            phChart.Series["Series01"].Points[50].Label = "L";
            phChart.Series["Series01"].Points[55].Label = "i";
            phChart.Series["Series01"].Points[63].Label = "q";
            phChart.Series["Series01"].Points[69].Label = "u";
            phChart.Series["Series01"].Points[75].Label = "i";
            phChart.Series["Series01"].Points[80].Label = "d";

            // */


            phChart.Series["Series01"].ChartArea = "ChartArea1";

            //This one if for  [hVsat;hVcrit],[psat;pcrit],'r', ...
            phChart.Series.Add("Series2");
            phChart.Series["Series2"].ChartType = SeriesChartType.Line;
            for (int i = 0; i < hVsatList.Count; i++)
            {
                phChart.Series["Series2"].Points.AddXY(hVsatList[i], psatList[i]);
            }
            phChart.Series["Series2"].Points.AddXY(hVcrit, pcrit);
            phChart.Series["Series2"].Color = Color.Red;
            //Chart1.Series(0).Font = New Font(Me.Font.Name, 5, FontStyle.Regular)
            phChart.Series["Series2"].Font = new Font(FontFamily.GenericSansSerif, 8, FontStyle.Bold);
            // phChart.Series["Series2"].Points[12].Label = $"Saturation Vapour";

            // /*
            phChart.Series["Series2"].Points[12].Label = "S";
            phChart.Series["Series2"].Points[15].Label = "a";
            phChart.Series["Series2"].Points[18].Label = "t";
            phChart.Series["Series2"].Points[21].Label = "u";
            phChart.Series["Series2"].Points[24].Label = "r";
            phChart.Series["Series2"].Points[27].Label = "a";
            phChart.Series["Series2"].Points[31].Label = "t";
            phChart.Series["Series2"].Points[35].Label = "i";
            phChart.Series["Series2"].Points[39].Label = "o";
            phChart.Series["Series2"].Points[44].Label = "n";

            phChart.Series["Series2"].Points[50].Label = "V";
            phChart.Series["Series2"].Points[55].Label = "a";
            phChart.Series["Series2"].Points[60].Label = "p";
            phChart.Series["Series2"].Points[67].Label = "o";
            phChart.Series["Series2"].Points[74].Label = "u";
            phChart.Series["Series2"].Points[80].Label = "r";
            // */

            phChart.Series["Series2"].ChartArea = "ChartArea1";

            //--For critical point
            //This one if for  hcrit,pcrit,'o', ...
            phChart.Series.Add("Series3");
            phChart.Series["Series3"].ChartType = SeriesChartType.Point;

            phChart.Series["Series3"].Points.AddXY(hcrit, pcrit);
            phChart.Series["Series3"].Color = Color.Orange;
            phChart.Series["Series3"].ChartArea = "ChartArea1";

            //--This one is the last one
            double[,] psatOneToNine = new double[psatList.Count, 9];
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < psatList.Count; j++)
                {
                    psatOneToNine[j, i] = psatList[j];
                }
            }

            for (int i = 0; i < hX.GetLength(1); i++)//Row 
            {

                phChart.Series.Add("Series4" + i);
                phChart.Series["Series4" + i].ChartType = SeriesChartType.Line;



                for (int j = 0; j < hX.GetLength(0); j++)//Column 
                {
                    phChart.Series["Series4" + i].Points.AddXY(hX[j, i], psatOneToNine[j, i]);
                }

                phChart.Series["Series4" + i].Color = Color.Green;
                phChart.Series["Series4" + i].Points[12].Label = $"{(i + 1) * 10} %";//initially no. 12
                phChart.Series["Series4" + i].Font = new Font(FontFamily.GenericSansSerif, 8, FontStyle.Bold);

                phChart.Series["Series4" + i].ChartArea = "ChartArea1";

            }

            //---Contour line plotting chunk of code -----------//
            /*
             Steps : 
             1. number of contour
             2. series decleration
             3. ds the data setting for Jack xu book[data is already calculated so]
             4. calling the plot function in Jack xu code
             */

            int numberOfContour = 25;
            ArrayList alSeries = new ArrayList();
            for (int i = 0; i < numberOfContour; i++)
            {
                string name = "ContourSeries" + i;
                alSeries.Add(name);


            }

            ChartFunctions cf = new ChartFunctions();
            DataSeries ds = new DataSeries();
            Form1_jack_xu f1 = new Form1_jack_xu();
            DrawChart dc = new DrawChart(f1);

            cf.SetDataPoints(ds, P_Value, T_Value, H_Value);
            //==After calling this we need to call the number of contour plots
            /* numberContours values = 
             28 gives 20 lines,
             30 gives 20 lines,
             25 gives 17 lines
             35 gives 25 lines [max limit]
             20 gives 14 lines
             27 gives 19 lines
             13 gives 8 lines
             */
            dc.numberContours = 35;

            List<DrawChart.DataTypeForPointList> listPoints = new List<DrawChart.DataTypeForPointList>();
            listPoints = dc.AddContour_MyCustomFxn(ds);

            //-------------For indicator of Temperature---------//

            string seriesName1 = "TemperaturePoints";
            phChart.Series.Add(seriesName1);
            phChart.Series[seriesName1].ChartType = SeriesChartType.Point;
            int ind = 0;
            int flagSingleTemperatureIndicator = 1;//on first
            double zlevelValueForTempIndicator = 0;
            int dataPointCounter = 0;
            // int enthalpyValueForIndiator = 2000;

            //---------------End of indicator of temperature---//



            //--Serching z-levels and plotting 
            int initialZ = listPoints[0].zlevel;
            int initalIndex = 0;
            int runningz = 0;
            for (int i = 0; i < numberOfContour; i++)
            {
                //==First one is for moving in the list
                string seriesName = alSeries[i].ToString();
                Series s1 = new Series(seriesName);
                //s1.MarkerSize = 15;//--This does not work so
                s1.ChartType = SeriesChartType.Line;
                phChart.Series.Add(s1);
                //phChart.Series.Add(seriesName);
                //phChart.Series[seriesName].ChartType = SeriesChartType.Line;
                //phChart.Series[seriesName].MarkerSize = 60;
                //==For moving in the list
                for (int z = initalIndex; z < listPoints.Count; z++)
                {
                    runningz = z;




                    if (initialZ == listPoints[z].zlevel)
                    {
                        //==Same zlevel then draw on one line
                        phChart.Series[seriesName].Points.AddXY(listPoints[z].x1, listPoints[z].y1);
                        phChart.Series[seriesName].Points.AddXY(listPoints[z].x2, listPoints[z].y2);

                        dataPointCounter++;


                        // /*
                        //===========Temperature indicator================//
                        if (flagSingleTemperatureIndicator == 1 && dataPointCounter > (listPoints[z].zlevel) && (listPoints[z].x1 > 200 && listPoints[z].x1 < 2500))//(flagSingleTemperatureIndicator == 1 && zlevelValueForTempIndicator == listPoints[z].zlevel) //(listPoints[z].x1 == 2000)
                        {
                            double temperature = PH.IAPWS_IF97_TowParameterEquivalentFxn("T", "H", listPoints[z].x1 * 1000, "P", listPoints[z].y1 * 1000000, fluidName); //--This multiply is done to convert MPa to Pa and enthlapy is divided to convert J/kg to kJ/Kg
                            phChart.Series[seriesName1].Points.AddXY(listPoints[z].x1, listPoints[z].y1);
                            phChart.Series[seriesName1].Points[ind++].Label = $"{Math.Round(temperature - 273.15, 0)}°C";
                            flagSingleTemperatureIndicator = 0;//off

                            dataPointCounter = 0;
                        }

                        if (listPoints[z].zlevel > zlevelValueForTempIndicator)
                        {
                            flagSingleTemperatureIndicator = 1;//on
                            ////zlevelValueForTempIndicator = listPoints[z].zlevel;
                        }
                        zlevelValueForTempIndicator = listPoints[z].zlevel;
                        //===========Temper indiactor end==============//
                        // */


                    }
                    else
                    {
                        initialZ = listPoints[z].zlevel;
                        initalIndex = z;

                        //flagSingleTemperatureIndicator = 1;//on
                        break;
                    }

                }

                if (runningz == listPoints.Count)
                {
                    break;
                }

            }

            //stopWatch.Stop();
            //MessageBox.Show($"Elapsed millisecond = { stopWatch.ElapsedMilliseconds} ms");

            //---End of the contour line plot chunk of code---//
            ArrayList aListTemperature = new ArrayList();
            //string seriesName1 = "TemperaturePoints";
            //phChart.Series.Add(seriesName1);
            //phChart.Series[seriesName1].ChartType = SeriesChartType.Point;
            //int ind = 0;

            //for (double y = 0.005; y < 40; y *= 2)
            //{
            //    double temperature = PH.IAPWS_IF97_TowParameterEquivalentFxn("T", "H", 400 * 1000, "P", y * 1000000, fluidName); //--This multiply is done to convert MPa to Pa and enthlapy is divided to convert J/kg to kJ/Kg
            //    aListTemperature.Add(temperature);
            //    phChart.Series[seriesName1].Points.AddXY(400, y);
            //    phChart.Series[seriesName1].Points[ind++].Label = $"{Math.Round(temperature - 273.15, 2)} DegC";
            //}

            //=================================This part is for regregration cycle=====================//
            /*
            //--We need to convert temp to kelvine
            double T1 = 106 + 273.15;//Deg C+273.15 = kelving temperature ==>TEMPERATURE // changed from 10
            double P1 = 0.05;//MPa ==>Y-Axis[Pressure] //changed form 0.05mpa
            double T2 = 200 + 273.15;
            double P2 = 2;//MPa //changed form 1 MPa
            double T3 = 200 + 273.15;
            double P3 = P2;
            double T4 = T1;
            double P4 = P1;

            //--Lets calculate enthalpy 
            //double enthalpy1 = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", P1 * 1000000, "T", T1, fluidName) / 1000;
            //double enthalpy2 = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", P2 * 1000000, "T", T2, fluidName) / 1000;
            //double enthalpy3 = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", P3 * 1000000, "T", T3, fluidName) / 1000;
            //double enthalpy4 = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", P4 * 1000000, "T", T4, fluidName) / 1000;

            double enthalpy1 = 2650;
            double enthalpy2 = 2900;
            double enthalpy3 = 900;
            double enthalpy4 = 900;


            // MessageBox.Show($"Different enthalpy values en1[same input as en4]= {enthalpy1} , en2 = {enthalpy2},en3 = {enthalpy3} , en4  ={enthalpy4}");
            string sName1 = "RefregrationCyclePoints";
            phChart.Series.Add(sName1);
            phChart.Series[sName1].ChartType = SeriesChartType.Point;
            phChart.Series[sName1].MarkerStyle = MarkerStyle.Circle;
            phChart.Series[sName1].MarkerSize = 8;

            phChart.Series[sName1].Points.AddXY(enthalpy1, P1);
            phChart.Series[sName1].Points.AddXY(enthalpy2, P2);
            phChart.Series[sName1].Points.AddXY(enthalpy3, P3);
            phChart.Series[sName1].Points.AddXY(enthalpy4, P4);

            string lineCycle1 = "LineRefCycle1";
            phChart.Series.Add(lineCycle1);
            phChart.Series[lineCycle1].ChartType = SeriesChartType.Line;

            phChart.Series[lineCycle1].BorderWidth = 5;
            phChart.Series[lineCycle1].MarkerStyle = MarkerStyle.Circle;
            phChart.Series[lineCycle1].MarkerSize = 5;
            phChart.Series[lineCycle1].Color = Color.Green;
            phChart.Series[lineCycle1].Points.AddXY(enthalpy1, P1);
            phChart.Series[lineCycle1].Points.AddXY((enthalpy1 + enthalpy2) / 2, 0.3);//1.2 is pressure it is manually given as scale is in log
            phChart.Series[lineCycle1].Points.AddXY(enthalpy2, P2);
            phChart.Series[lineCycle1].Points[1].Label = "Compression";


            string lineCycle2 = "LineRefCycle2";
            phChart.Series.Add(lineCycle2);
            phChart.Series[lineCycle2].ChartType = SeriesChartType.Line;
            phChart.Series[lineCycle2].MarkerStyle = MarkerStyle.Circle;

            phChart.Series[lineCycle2].BorderWidth = 5;
            phChart.Series[lineCycle2].MarkerSize = 5;
            phChart.Series[lineCycle2].Color = Color.Green;
            phChart.Series[lineCycle2].Points.AddXY(enthalpy2, P2);
            phChart.Series[lineCycle2].Points.AddXY((enthalpy2 + enthalpy3) / 2, (P2 + P3) / 2);
            phChart.Series[lineCycle2].Points.AddXY(enthalpy3, P3);
            phChart.Series[lineCycle2].Points[1].Label = "Condensor";

            string lineCycle3 = "LineRefCycle3";
            phChart.Series.Add(lineCycle3);
            phChart.Series[lineCycle3].ChartType = SeriesChartType.Line;
            phChart.Series[lineCycle3].MarkerStyle = MarkerStyle.Circle;

            phChart.Series[lineCycle3].BorderWidth = 5;
            phChart.Series[lineCycle3].MarkerSize = 5;
            phChart.Series[lineCycle3].Color = Color.Green;
            phChart.Series[lineCycle3].Points.AddXY(enthalpy3, P3);
            phChart.Series[lineCycle3].Points.AddXY((enthalpy3 + enthalpy4) / 2, 0.3);//0.3 is a pressure ie 0.3mpa
            phChart.Series[lineCycle3].Points.AddXY(enthalpy4, P4);
            phChart.Series[lineCycle3].Points[1].Label = "Expansion Valve";


            string lineCycle4 = "LineRefCycle4";
            phChart.Series.Add(lineCycle4);
            phChart.Series[lineCycle4].ChartType = SeriesChartType.Line;
            phChart.Series[lineCycle4].MarkerStyle = MarkerStyle.Circle;

            phChart.Series[lineCycle4].BorderWidth = 5;
            phChart.Series[lineCycle4].MarkerSize = 5;
            phChart.Series[lineCycle4].Color = Color.Green;
            phChart.Series[lineCycle4].Points.AddXY(enthalpy4, P4);
            phChart.Series[lineCycle4].Points.AddXY((enthalpy4 + enthalpy1) / 2, (P4 + P1) / 2);
            phChart.Series[lineCycle4].Points.AddXY(enthalpy1, P1);
            phChart.Series[lineCycle4].Points[1].Label = "Evaporator";


          */
            //=================================End regregration cycle=====================//

        }
        public void Chart_MouseDown(object sender, MouseEventArgs e, Form_Main_PH_Application f1, Point position)
        {

            if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            {

                //--Check of enable or dissable the cell click
                //if (FlagForDissableLeftAndRightClicksInChart == 1)
                //{
                //    //FlagForDissableLeftAndRightClicksInChart = 0;
                //    return;//DO not proceed forward
                //}

                //we need to show context menu strip

                //MessageBox.Show("Right pressed");    
                //--this is calculated based on this location the graphics will be plotted..
                xCoordinateValue = e.Location.X;
                yCoordinateValue = e.Location.Y;

                //f1.phChart.ChartAreas[0].CursorX.SetCursorPixelPosition(new Point(e.X, e.Y), true);
                //f1.phChart.ChartAreas[0].CursorY.SetCursorPixelPosition(new Point(e.X, e.Y), true);

                //double pX = f1.phChart.ChartAreas[0].CursorX.Position; //X Axis Coordinate of your mouse cursor
                //double pY = f1.phChart.ChartAreas[0].CursorY.Position; //Y Axis Coordinate of your mouse cursor

                //xCoordinateValue = pX;
                //yCoordinateValue = pY;
                //contextMenuStrip1.Show(MousePosition);//--This is dissabled



                //==Now lets reset the values first,displaying the previous and next node ====//

                if (FlagForDisconnectingLineChoice == 1)
                {
                    //--We need to show the text and weather he wants to delete form first or seconds
                    //First lets enable and change the Text
                    f1.disconnectLineFromAToolStripMenuItem.Enabled = true;
                    //disconnectLineFromAToolStripMenuItem.Visible = true;
                    //disconnectLineFromBToolStripMenuItem.Enabled = true;

                    //if(addMixNodeToolStripMenuItem.Enabled == true)
                    //{ 
                    //addMixNodeToolStripMenuItem.Visible = true;
                    //}

                    //--Now lets make the text 
                    //steps :
                    /*
                       1.Detect previous and next node id 
                       2.Detect corresponding name value for the corresponding  id
                       3.Display the node Name  
                    */
                    string prevNodeIDTemp = "";
                    string nextNodeIDTemp = "";

                    foreach (var node in listNodeInfoValues)
                    {
                        if (node.ID == indexOfNextNodeForLineMovement)
                        {
                            //--Next noe is identified 
                            nextNodeIDTemp = node.name;//--Store the name of the thing
                        }
                        else if (node.ID == indexOfPrevPointForLineMovement)
                        {
                            //--Previous node is identified 
                            prevNodeIDTemp = node.name;//--Stroing the name of the node
                        }

                    }

                    //--we need lineID
                    //   disconnectLineFromAToolStripMenuItem.Text = "Disconnect line from " + prevNodeIDTemp;
                    // disconnectLineFromBToolStripMenuItem.Text = "Disconnect line from " + nextNodeIDTemp;

                    f1.nodeAToolStripMenuItem.Text = "Node Name: " + prevNodeIDTemp;
                    f1.nodeBToolStripMenuItem.Text = "Node Name: " + nextNodeIDTemp;



                }
                else
                {
                    //--We need to turn off the click option
                    f1.disconnectLineFromAToolStripMenuItem.Enabled = false;
                    // disconnectLineFromAToolStripMenuItem.Visible = false;
                    //disconnectLineFromBToolStripMenuItem.Enabled = false;

                    //--lets dissable the line
                    f1.nodeAToolStripMenuItem.Enabled = false;
                    f1.nodeBToolStripMenuItem.Enabled = false;
                    // addMixNodeToolStripMenuItem.Enabled = false;//For dissabling the add mix node
                    //--Lets make it invisible
                    //addMixNodeToolStripMenuItem.Visible = false;

                    //--Lets dissbale
                    //nodeAToolStripMenuItem.Enabled = false;
                    //nodeBToolStripMenuItem.Enabled = false;

                }

                //==this one is for delete node to hide
                if (f1.deleteNodeToolStripMenuItem.Enabled == true)
                {
                    f1.deleteNodeToolStripMenuItem.Visible = true;
                }
                else
                {
                    f1.deleteNodeToolStripMenuItem.Visible = false;
                }

                //--------This one is for mix node to show or hide
                //if (addMixNodeToolStripMenuItem.Enabled == false)
                //{
                //    addMixNodeToolStripMenuItem.Visible = false;
                //}
                //else
                //{
                //    addMixNodeToolStripMenuItem.Visible = true;
                //}

                //--This one is for disconnecting line 
                if (f1.nodeAToolStripMenuItem.Enabled == false)
                {
                    //disconnecting line invisible
                    f1.disconnectLineFromAToolStripMenuItem.Visible = false;
                }
                else
                {

                    //visible
                    f1.disconnectLineFromAToolStripMenuItem.Visible = true;
                }

                //--Now lets do for the comfort zone parts...

                //--now lets check some things
                //InfoForComfortZoneOfChart(chartDetailList[indexForWhichChartIsSelected].chartID);
                ////MessageBox.Show("here = " + default_comfort_zone_of_chart[0].status + " count = "+ default_comfort_zone_of_chart.Count);
                //if (default_comfort_zone_of_chart.Count > 0)
                //{
                //    if (default_comfort_zone_of_chart[0].status == "enable")
                //    {
                //        //now lets calculate the humidity because we can only make comparision with the humidity values....
                //        HumTempCalcByCoordinate();//This will return humidityValue AND TemperatureValue by mouse coordinate
                //                                  //MessageBox.Show("check min temp = " + listchartComfortZoneInfoSingle[0].min_temp + "\n max temp = " + listchartComfortZoneInfoSingle[0].max_temp + "\n min hum= " + listchartComfortZoneInfoSingle[0].min_hum + "\n max hum=" + listchartComfortZoneInfoSingle[0].max_hum);

                //        if (listchartComfortZoneInfoSingle.Count > 0)
                //        {
                //            if ((temperatureValue > double.Parse(listchartComfortZoneInfoSingle[0].min_temp) && temperatureValue < double.Parse(listchartComfortZoneInfoSingle[0].max_temp)) && (humidityValue > double.Parse(listchartComfortZoneInfoSingle[0].min_hum) && humidityValue < double.Parse(listchartComfortZoneInfoSingle[0].max_hum)))
                //            {
                //                if (showComfortZoneToolStripMenuItem.Enabled == true)
                //                {
                //                    showComfortZoneToolStripMenuItem.Visible = true;
                //                }
                //                else
                //                {
                //                    showComfortZoneToolStripMenuItem.Visible = false;
                //                }
                //            }
                //            else
                //            {
                //                showComfortZoneToolStripMenuItem.Visible = false;
                //            }
                //            //--Chart is enable futher processing is needed
                //        }//close of if listchartComfortZoneInfoSingle
                //    }
                //    else
                //    {
                //        //--Hide the comfort zone
                //        showComfortZoneToolStripMenuItem.Visible = false;
                //    }
                //}
                //else
                //{
                //    showComfortZoneToolStripMenuItem.Visible = false;
                //}



                //===End of the reset the values=========// 
                f1.quickNodeInsertToolStripMenuItem.Image = null;
                if (FlagForDissableLeftAndRightClicksInChart == 1)
                {
                    //FlagForDissableLeftAndRightClicksInChart = 0;
                    f1.CMSinsertNode.Enabled = false;
                    //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                    //string NewDirectory = Path.GetFullPath(Path.Combine(dir, @"..\"));
                    //string file = NewDirectory + @"Database\image\lock.png";

                    string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = imagePath + @"\image\lock.png";
                    //string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    //string file = imagePath + @"\..\Database\image\lock.png";


                    f1.quickNodeInsertToolStripMenuItem.Image = Bitmap.FromFile(file);
                    f1.quickNodeInsertToolStripMenuItem.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
                    // this.quickNodeInsertToolStripMenuItem.TextAlign = System.Drawing.ContentAlignment.TopCenter;
                    //quickNodeInsertToolStripMenuItem.ImageAlign = 
                    f1.quickNodeInsertToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.ImageAndText;

                    //f1.CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                    //f1.CMSinsertNode.Show(e.X, e.Y);
                    f1.CMSinsertNode.Show(position);
                    //return;//DO not proceed forward
                }
                else
                {
                    //==This shows the contextmenustrip on right click
                    f1.CMSinsertNode.Enabled = true;
                    //f1.CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                    // f1.CMSinsertNode.Show(e.X,e.Y);
                    f1.CMSinsertNode.Show(position);
                }
                //f1.CMSinsertNode.Show(e.X, e.Y);
                // MessageBox.Show("OK");

            }

        }
        public void chart_MouseMove(object sender, MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {
            //this part helps to get the x  and the y coordinate 
            //this coordinate finding is based on the plotting part of chart element type..
            var pos = e.Location;
            if (prevPosition.HasValue && pos == prevPosition.Value)
                return;
            // tooltip.RemoveAll();
            prevPosition = pos;
            var results = chart1.HitTest(pos.X, pos.Y, false, ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(pos.X);
                    var yVal = Math.Pow(10, result.ChartArea.AxisY.PixelPositionToValue(pos.Y));

                    // if((currentXAxis>=0 && currentXAxis<=50)&&(currentYAxis>=0 && currentYAxis <= 30)) { 

                    if (((double)xVal >= xAxisMinimum && (double)xVal <= xAxisMaximum) && ((double)yVal >= yAxisMinimum && (double)yVal <= yAxisMaximum))
                    {
                        //--These currentXAxis and currentYAxis is used for showing the dotted line 
                        currentXAxis = (double)xVal;
                        currentYAxis = (double)yVal;

                        //lb_test.Text = "x = " + currentXAxis + ",y  = " + currentYAxis;
                        // if ((currentXAxis >= 0 && currentXAxis <= 50) && (currentYAxis >= 0 && currentYAxis <= 30))
                        //{

                        //now lets move on to making other part 
                        /*1.find dbt value => this is x axis value 
                         * 2.find sp.ratio value => this is yaxis value
                         */
                        f1.lbEnthalpy.Text = Math.Round(xVal, 2).ToString();
                        f1.lbPressure.Text = Math.Round(yVal, 2).ToString();
                        //lb_humidity_ratio.Text = Math.Round(yVal, 2).ToString();

                        //now lets move towards printing the relative humidity at that position and dew point and enthalpy also wbt
                        //first Relative humidity...
                        //first we need to see equation w = 622*phi*pg./(patm-phi*pg);
                        /*
                         we need to calc phi value given by ycord/30 as the max value is 30..
                         * second pg which is calculated by temperature pulled from the text file we need to fist 
                         * calculate the round up value of x coord to an integer...
                         */

                        //this part is not correct yet we need to do this again....
                        /*
                        double phi = 0.00000;
                        //double y_axis = yVal;
                        //now for pg..
                        ArrayList temperature_value = new ArrayList();
                        ArrayList pg_value_from_txtfile = new ArrayList();

                        //--Copying the ref temp and humidity values..
                        temperature_value = t;
                        pg_value_from_txtfile = pg;

                        double temperature = Math.Round(xVal);
                        double corres_pg_value = 0.000000;
                        for (int i = 0; i < temperature_value.Count; i++)
                        {
                            if (temperature == Double.Parse(temperature_value[i].ToString()))
                            {
                                corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                                break;
                            }
                        }//close of for

                        double patm = AirPressureFromDB * 0.001; // this is in terms of kpa //101.325;//this is constant... //101.325;//this is constant...
                                                                 //Lets check the patm value
                                                                 //  lb_test1.Text = "patm = " + patm;
                        double w = yVal;
                        phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                        phi = phi * 100;//changing into percent..
                                        //now display in label...
                        lb_RH.Text = Math.Round(phi, 2).ToString();

                        //lb_test1.Text = " Humidity = " + phi;

                        //now lets calculate the dew point...
                        double humidity = phi;
                        double temperature1 = xVal;
                        double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
                        //now lets print this value..
                        lb_DP.Text = Math.Round(TD, 2).ToString();


                        //now lets move towards enthalpy...

                        Patm = patm * 10;  // 1013;
                        A = 6.116441;
                        m = 7.591386;
                        Tn = 240.7263;
                        B = 621.9907;

                        double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

                        double X = B * Pws / (Patm - Pws);   //This one is formula 

                        h = temperature1 * (1.01 + (0.00189 * X)) + 2.5 * X; //This one is the enthalpy
                        //now lets display this value ..
                        lb_enthalpy.Text = Math.Round(h, 2).ToString();
                        */
                    }


                }//Closing of currentxval= 0-50 and 0-30 currentyval
            }

            //if (flagForEditComfortZoneGraphically == 1)
            //{

            //    if (flagForBorderLineSelectedForMoveForEditCF == 1)
            //    {
            //        //--Perform redraw function 


            //        CursorFunctionForComfortZoneEditMode(e);

            //    }
            //    else
            //    {
            //        //--IF edit comfort zone is enabled dont do other task just do the 
            //        //Task of comfort zone only and let other task hault for now
            //        ComfortZoneBorderLineDetectForEdit(e); // --This methods detects line movement
            //    }




            //}
            //else
            //{

            //--IF the line is selected/disconnected and then we need to connect to a node
            if (flagForDisconnectClick == 1)  //Disconnect is clicked then they talk
            {
                f1.lbPrintTest.Text = "FlagForDisConnect=1";
                //--Creating temporary line..
                //--then redraw it again...
                addTemporarySeries(addDottedSeries, f1);
                //--Now lets move on the showing the hand when hover over the Node lets do it bro...
                addCursorFunctionForLineDisconnectConnect(e, f1.phChart, f1);
                // lb_where.Text = "me : discon =1";

            }
            else
            {
                f1.lbPrintTest.Text = "FlagForDisConnect=0";
                // lb_where.Text = "me : else line detect on";
               // f1.disconnectLineToolStripMenuItem.Enabled = false;

                //--This one is for two side of line disconnection -------//
                f1.disconnectLineFromAToolStripMenuItem.Enabled = false;
                //disconnectLineFromBToolStripMenuItem.Enabled = false;

                f1.nodeAToolStripMenuItem.Enabled = false;
                f1.nodeBToolStripMenuItem.Enabled = false;
                // addMixNodeToolStripMenuItem.Enabled = false;//Dissable add mix node
                //---End of two side of line disconnection section

                //--This is for the weather the line is moverover or not...
                LineDetectOnMouseMove(e, f1.phChart, f1);

                //--Lets add a function for the process diagram drawing..

                ProcessDiagramMouseMoveFunction(e, f1.phChart, f1);//--This does the adding and removing part             
            }

            // }//close of else

        }//close of the main private void...

        /// <summary>
        /// add a dotted series when the mouse is moved but not we need to add to
        /// chart first in order to get the desired effect
        /// </summary>
        /// <param name="addDottedSeries"></param>
        public void addTemporarySeries(Series addDottedSeries, Form_Main_PH_Application f1)
        {

            //--then redraw it again...
            addDottedSeries.Points.Clear();
            addDottedSeries.ChartType = SeriesChartType.FastLine;
            addDottedSeries.BorderDashStyle = ChartDashStyle.Dash;//--This gives the dashed style
            addDottedSeries.Color = Color.Black;
            addDottedSeries.BorderWidth = 3;

            double xAxisValue = 0.00;
            double yAxisvalue = 0.00;

            if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 0)
            {
                //--If the line is not reseted then return form here
                return;
            }
            //We need to find the previous point
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                //1 means prev node is selected 
                if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 2) //This is done because we are detaching from node1
                {
                    if (listNodeInfoValues[i].ID == indexOfPrevPointForLineMovement)//indexOfPrevPointForLineMovement variable is in NodeAndLineClass 
                    {
                        //This is the node of previous point we need to find the x and y coordinate of
                        xAxisValue = listNodeInfoValues[i].xVal;
                        yAxisvalue = listNodeInfoValues[i].yVal;
                        break;//If value found no need to search all exit of loop
                    }

                } //2 means next node is selected 0 means off
                else if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 1)//--This is done we are detaching form node2
                {
                    if (listNodeInfoValues[i].ID == indexOfNextNodeForLineMovement)
                    {
                        //This is the node of previous point we need to find the x and y coordinate of
                        xAxisValue = listNodeInfoValues[i].xVal;
                        yAxisvalue = listNodeInfoValues[i].yVal;
                        break;//If value found no need to search all exit of loop
                    }
                }
            }
            //addDottedSeries.Points.AddXY(menuStripNodeInfoValues[indexOfPrevPointForLineMovement].xVal, menuStripNodeInfoValues[indexOfPrevPointForLineMovement].yVal);
            addDottedSeries.Points.AddXY(xAxisValue, yAxisvalue);//--this is the new changed code:bbk305
            addDottedSeries.Points.AddXY(currentXAxis, currentYAxis);

        }

        public void addCursorFunctionForLineDisconnectConnect(MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {//--This function helps to draw a mouse move event..
            //--This is done to prevent mouse event e.x called before chart is loaded other wise the program will crash
            if (!chart1.IsAccessible && load == 0)
            {
                load = 1;
                return;

            }

            //this event occurs and compares the values in the list first and identifies if the values
            if ((e.X > chart1.ChartAreas[0].Position.X && e.Y > chart1.ChartAreas[0].Position.Y) && (e.X < chart1.Width && e.Y < chart1.Height))
            {
                try
                {
                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue =Math.Pow(10, chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y));// This is done because its logscale

                    xAxis1 = xValue;
                    yAxis1 = yValue;
                    if ((xAxis1 >= xAxisMinimum && xAxis1 <= xAxisMaximum) && (yAxis1 >= yAxisMinimum && yAxis1 <= yAxisMaximum))
                    {
                        //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                        if (listNodeInfoValues.Count > 0)
                        {
                            //foreach(var values in menuStripNodeInfoValues)

                            for (int i = 0; i < listNodeInfoValues.Count; i++)
                            {

                                if ((xValue > listNodeInfoValues[i].xVal - radiusSize && xValue < listNodeInfoValues[i].xVal + radiusSize) && (yValue > listNodeInfoValues[i].yVal - radiusSize && yValue < listNodeInfoValues[i].yVal + radiusSize))
                                {

                                    idOfNodeSelected =listNodeInfoValues[i].ID;
                                    if (f1.Cursor == Cursors.Cross)
                                    {
                                        f1.Cursor = Cursors.Hand;
                                    }

                                    //--Whenever this occurs lets move on to attaching the the node or say refreshing and replotting....
                                    //--For this as well lets rise a flag..
                                    flagNodeSelectedForConnect = 1;
                                    break;//this break is for if found the value no longer loop increases the perfomances..
                                }
                                else
                                {
                                    if (f1.Cursor != Cursors.Cross)
                                    {
                                        f1.Cursor = Cursors.Cross;
                                        // readyForMouseClick = 0;//dissable on click event.
                                        flagNodeSelectedForConnect = 0;
                                    }

                                }
                            }
                        }//close of if menuStripAllValue>0
                    }//close of if
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }//--close of the if..

        }//close of the actual function...public void

        Color storeColor;
        int flagForColor = 0;

        //--Lets store the series for futher processing...
        Series tempSeries;
        /// <summary>
        /// Detects the line which connects the two nodes
        /// when hovered over it 
        /// </summary>
        /// <param name="e">mouse event argument an event simply</param>
        private void LineDetectOnMouseMove(MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {
            HitTestResult hit = chart1.HitTest(e.X, e.Y);
            // Text = "Element: " + hit.ChartElementType;
            DataPoint dp = null;
            if (hit.ChartElementType == ChartElementType.DataPoint)
                dp = hit.Series.Points[hit.PointIndex];

            //lb_test.Text = "nothing ";

            if (dp != null)
            {
                //  Text += " Point #" + hit.PointIndex  + " x-value:" + dp.XValue + " y-value: " + dp.YValues[0]+" series name = "+hit.Series.Name;

                if (listLineInfoValues.Count > 0)
                {
                    for (int i = 0; i < listLineInfoValues.Count; i++)
                    {

                        if (hit.Series.Name != null)
                        {
                            if ((string)hit.Series.Name == (string)listLineInfoValues[i].lineSeriesID.Name)
                            {
                                //--lets store previous color first
                                storeColor = listLineInfoValues[i].lineColorValue;
                                flagForColor = 1;
                                tempSeries = hit.Series;

                                //  idOfLineSelecteForDisconnect = menuStripNodeLineInfoValues[i].ID;
                                indexOfLineInTheList = i; // updating the index 

                                //--These two variables are in NodeAndLineClass.cs files
                                //--Logging the index so that it could be used for futher processing later...
                                indexOfPrevPointForLineMovement = listLineInfoValues[i].prevNodeId;//This gets the previous node id value...

                                indexOfNextNodeForLineMovement = listLineInfoValues[i].nextNodeId;//==This gets the next node id

                                hit.Series.Color = Color.Black;
                                //f1.disconnectLineToolStripMenuItem.Enabled = true;
                                // lb_test.Text = Text;

                                //---This code is for detecting form A and form B point in line detachment---//

                                /*
                                Steps : here we do following task
                                1. Enable both the click option for now just enable.
                                2. We have node info in this->indexOfPrevPointForLineMovement and  this->indexOfNextNodeForLineMovement
                                */
                                f1.disconnectLineFromAToolStripMenuItem.Enabled = true;
                                //disconnectLineFromBToolStripMenuItem.Enabled = true;

                                f1.nodeAToolStripMenuItem.Enabled = true;
                                f1.nodeBToolStripMenuItem.Enabled = true;
                                //addMixNodeToolStripMenuItem.Enabled = true;//For adding mix node

                                FlagForDisconnectingLineChoice = 1;
                                //---End of line deteachment section --------------------//
                            }
                        }
                    }
                }//CLOSE OF IF MENUSTRIP

            }//CLOSE of if dp 
            else
            {
                if (flagForColor == 1)
                    tempSeries.Color = storeColor;

            }
        }

        private void ProcessDiagramMouseMoveFunction(MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {
            //--This function helps to draw a mouse move event..
            //--This is done to prevent mouse event e.x called before chart is loaded other wise the program will crash
            if (!chart1.IsAccessible && load == 0)
            {
                load = 1;
                return;

            }

            //this event occurs and compares the values in the list first and identifies if the values
            if ((e.X > chart1.ChartAreas[0].Position.X && e.Y > chart1.ChartAreas[0].Position.Y) && (e.X < chart1.Width && e.Y < chart1.Height))
            {
                try
                {
                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue =Math.Pow(10, chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y));
                    if ((xValue >= xAxisMinimum && xValue <= xAxisMaximum) && (yValue >= yAxisMinimum && yValue <= yAxisMaximum))
                    {

                        xAxis1 = xValue;
                        yAxis1 = yValue;
                        //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                        if (listNodeInfoValues.Count > 0)
                        {
                            //foreach(var values in menuStripNodeInfoValues)
                           

                            for (int i = 0; i < listNodeInfoValues.Count; i++)
                            {

                                if (listNodeInfoValues[i].temperature_source != "Mix")
                                {
                                   

                                    if ((xValue > listNodeInfoValues[i].xVal - radiusSize  && xValue < listNodeInfoValues[i].xVal + radiusSize) && (yValue > listNodeInfoValues[i].yVal - radiusSize && yValue < listNodeInfoValues[i].yVal + radiusSize))
                                    {

                                        //--This is changed from int to string  code bbk305
                                        idSelected = listNodeInfoValues[i].ID; //Now this is a string 
                                        tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                        if (f1.Cursor != Cursors.Cross)
                                        {
                                            f1.Cursor = Cursors.Hand;
                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 1;//flag is ready on Node selected
                                            nodeID_ForDeletingNode = idSelected;
                                            f1.deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
                                                                                           //=============end of flag for deleting===========//
                                        }
                                        //this.Cursor = Cursors.Hand;
                                        //now this works so lets move forward.
                                        readyForMouseClick = 1;//enable on click event


                                        break;//this break is for if found the value no longer loop increases the perfomances..
                                    }
                                    else
                                    {
                                        if (f1.Cursor != Cursors.Cross)
                                        {
                                            f1.Cursor = Cursors.Arrow;
                                            readyForMouseClick = 0;//dissable on click event.

                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                            f1.deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
                                                                                           //nodeID_ForDeletingNode = idSelected;
                                                                                           //=============end of flag for deleting===========//

                                        }

                                    }
                                    //--Lets filter out the mix nodes---

                                }//Close of if section Mix section

                                //==This one is special case for mix node only
                                else if (listNodeInfoValues[i].temperature_source == "Mix")
                                {
                                    if ((xValue > listNodeInfoValues[i].xVal - radiusSize && xValue < listNodeInfoValues[i].xVal + radiusSize) && (yValue > listNodeInfoValues[i].yVal - radiusSize && yValue < listNodeInfoValues[i].yVal + radiusSize))
                                    {

                                        //--This is changed from int to string  code bbk305
                                        idSelected = listNodeInfoValues[i].ID; //Now this is a string 
                                        tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                        if (f1.Cursor != Cursors.Cross)
                                        {
                                            f1.Cursor = Cursors.Hand;
                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 1;//flag is ready on Node selected
                                            nodeID_ForDeletingNode = idSelected;
                                            f1.deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
                                                                                           //=============end of flag for deleting===========//
                                        }
                                        //this.Cursor = Cursors.Hand;
                                        //now this works so lets move forward.
                                        //===This should be dissabled
                                        // readyForMouseClick = 1;//enable on click event


                                        break;//this break is for if found the value no longer loop increases the perfomances..
                                    }
                                    else
                                    {
                                        if (f1.Cursor != Cursors.Cross)
                                        {
                                            f1.Cursor = Cursors.Arrow;
                                            /// readyForMouseClick = 0;//dissable on click event.

                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                            f1.deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
                                            //nodeID_ForDeletingNode = idSelected;
                                            //=============end of flag for deleting===========//

                                        }
                                    }
                                }
                            }//Close of for loop
                        }//close of if menuStripAllValue>0


                        if (mouseClickAction == 1)
                        {

                            if (Control.ModifierKeys == Keys.Alt)
                            {
                                //--This alter key is for moving along constant x-axis ...
                                // MessageBox.Show(" alt is pressed for x axis constant");


                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                 listNodeInfoValues[tempIndexForNode].yVal = yAxis1;//This value is changed...
                                //====================For mix node movement = ===============//

                                //UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//

                                // label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    // chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);//--removing line series that joins node..
                                    listLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}

                                    //--this is changed as well code :bbk305
                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    //f1.dom.ReDrawPoints(f1.dom.series1, f1.dom.listNodeInfoValues[x].xVal, f1.dom.listNodeInfoValues[x].yVal, f1.dom.listNodeInfoValues[x].colorValue, f1.dom.listNodeInfoValues[x].temperature_source, f1.dom.listNodeInfoValues[x].pressure_source, f1.dom.listNodeInfoValues[x].name, labelValue, f1.dom.listNodeInfoValues[x].marker_Size);
                                    ReDrawPoints(chart1,series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);
                                    //Updating values in database
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //   UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;//Incrementing the index values
                                                              //--Every time it redraws the point we need to update to database the node values
                                }
                                //--resetting incrementIndex
                                //incrementIndex = 0;
                                if (listLineInfoValues.Count > 0)
                                {


                                    for (int x = 0; x < listLineInfoValues.Count; x++)
                                    {
                                        // incrementIndex++;

                                        //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                        //f1.dom.ReDrawLines(f1.dom.listLineInfoValues[x].ID, f1.dom.listLineInfoValues[x].prevNodeId, f1.dom.listLineInfoValues[x].nextNodeId, f1.dom.listLineInfoValues[x].lineSeriesID, f1.dom.listLineInfoValues[x].lineColorValue, f1.dom.listLineInfoValues[x].lineThickness, f1.dom.listLineInfoValues[x].name, f1.dom.listLineInfoValues[x].status);
                                        ReDrawLines(chart1,listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);

                                    }

                                }

                                chart1.Invalidate();
                                // incrementIndex = 0;//reset the values again..
                                //  indexForSeriesNodePoint = 0;//Resetting the index value BBK305A

                            }
                            else if (Control.ModifierKeys == Keys.Shift)
                            {
                                //--This ctrl key is for moving along the y-  axis...

                                //--THis function basically evolve when the shift key is pressed and mouse move.
                                // MessageBox.Show("shift  is pressed for y  axis constant");

                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                listNodeInfoValues[tempIndexForNode].xVal = xAxis1;//--This value is just changed 
                                                                                          //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                                //====================For mix node movement = ===============//

                                //UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//
                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    listLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    //dom.ReDrawPoints(dom.series1, dom.listNodeInfoValues[x].xVal, dom.listNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                     ReDrawPoints(chart1,series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);

                                    //Updating values in database...
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }

                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                incrementIndex = 0;
                                if (listLineInfoValues.Count > 0)
                                {
                                    for (int x = 0; x < listLineInfoValues.Count; x++)
                                    {
                                        incrementIndex++;

                                        // dom.ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                        ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);
                                    }

                                }

                                chart1.Invalidate();
                                // incrementIndex = 0;//reset the values again..
                                // indexForSeriesNodePoint = 0;

                            }
                            else
                            {

                                //--Show indicator
                                ////--Lets clear the indicator point first.
                                //seriesLineIndicator.Points.Clear();

                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                                listNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                                listNodeInfoValues[tempIndexForNode].yVal = yAxis1;


                                //***********************For mix node section functions******************//

                                //Function helps in updating the mix point x and y value when other node are moved
                                // UpdateMixPointOnNodeMovement();
                                //************************End of the mix node ************************//
                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    listLineInfoValues[i].lineSeriesID.Points.Clear();
                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    ReDrawPoints(chart1, series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);

                                    //Updating values in database...
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                // incrementIndex = 0;
                                if (listLineInfoValues.Count > 0)
                                {
                                    for (int x = 0; x < listLineInfoValues.Count; x++)
                                    {
                                        //incrementIndex++;

                                        // dom.ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                        ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);
                                    }

                                }


                                chart1.Invalidate();
                                //incrementIndex = 0;//reset the values again..
                                //   indexForSeriesNodePoint = 0;

                                //indexForSeriesNodePoint = 0;

                            }//closing of key else part
                        }

                        //Need to add here
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }//close of if chart1.ChartAreas[0]


        }

        public void chart_MouseClick(object sender, MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {
            //--Check of enable or dissable the MMcell click
            if (FlagForDissableLeftAndRightClicksInChart == 1)
            {
                //FlagForDissableLeftAndRightClicksInChart = 0;
                return;//DO not proceed forward
            }

            //if (flagForEditComfortZoneGraphically == 1)
            //{

            //    //First click then 1 second click then 2
            //    if ((Cursor == Cursors.SizeWE || Cursor == Cursors.SizeNS))
            //    {
            //        if (selectedSeriesForMoveForEditComfortZone != "")
            //        {
            //            // Cursor = Cursors.SizeWE;
            //            // selectedSeriesForMoveForEditComfortZone = "Vertical_Temp_Left_Side";
            //            flagForBorderLineSelectedForMoveForEditCF += 1;
            //            comfortZoneBorderSeriesCurrentlySelected = selectedSeriesForMoveForEditComfortZone;
            //        }

            //    }

            //if (flagForBorderLineSelectedForMoveForEditCF == 0)
            //{
            //    ////If the button is clicked outside then we have flagForBorderLineSelectedForMoveForEditCF = 3
            //    //this should be done only when clicked outside the chart so use condition
            //    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
            //    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
            //    double hum = CalculateHumidityFromXandYCoordinate(xValue, yValue);//This gets the humidity 

            //    if ((hum < int.Parse(listchartComfortZoneInfoSingle[0].min_hum) || hum > int.Parse(listchartComfortZoneInfoSingle[0].max_hum)) || (xValue < int.Parse(listchartComfortZoneInfoSingle[0].min_temp) || xValue > int.Parse(listchartComfortZoneInfoSingle[0].max_temp)))
            //    {
            //        flagForBorderLineSelectedForMoveForEditCF = 3;
            //    }

            //}

            //if (flagForBorderLineSelectedForMoveForEditCF == 3)
            //{
            //    /*
            //   Means create new chart if the data is updated to new values  
            //   */

            //    //Checking if current value matches with previous values or not if so then do not do anything
            //    if ((minTemperatureForNewComfortZoneCreate != 0) && (maxTemperatureForNewComfortZoneCreate != 0) && (minHumidityForNewComfortZoneCreate != 0) && (maxHumidityForNewComfortZoneCreate != 0))
            //    {
            //        //If the values has changed then default then only change else no change 
            //        //Lets get the previous chart value to make new chart 
            //        string name = listchartComfortZoneInfoSingle[0].name;
            //        int minTempOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].min_temp);
            //        int maxTempOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
            //        int minHumidityOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
            //        int maxHumidityOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);
            //        Color prevComfortzoneColor = listchartComfortZoneInfoSingle[0].colorValue;

            //        if (((minTemperatureForNewComfortZoneCreate == minTempOfPrevComfortzone) && (maxTemperatureForNewComfortZoneCreate == maxTempOfPrevComfortzone) && (minHumidityForNewComfortZoneCreate == minHumidityOfPrevComfortzone) && (maxHumidityForNewComfortZoneCreate == maxHumidityOfPrevComfortzone)))
            //        {
            //            //If equal do not do any thing 
            //            //Refreshing the chart
            //            if (dataGridView1.Rows.Count > 0)  //If there is data then only do this one
            //            {
            //                //set parameters of your event args
            //                var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex);
            //                // or setting the selected cells manually before executing the function
            //                dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
            //                dataGridView1_CellClick(sender, eventArgs);
            //            }

            //        }
            //        else
            //        {
            //            //If they do not match then only create new comfort zone and assign them the value
            //            /*Steps "
            //            1.Clear the existing chart first
            //            2.Create the new chart with new values and new name 
            //            3.Update the chart list 
            //            */
            //            // MessageBox.Show("Chart creating and updating");

            //            //Clearing the border lines 
            //            ClearSeriesInChart("Vertical_Temp_Left_Side");
            //            ClearSeriesInChart("Vertical_Temp_Right_Side");
            //            ClearSeriesInChart("Horizontal_Hum_Down_Side");
            //            ClearSeriesInChart("Horizontal_Hum_Upper_Side");


            //            //Clearing the chart 
            //            ClearComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum));

            //            //We need to generate name
            //            //First splite if underscore is present
            //            string[] word = name.Split('_');


            //            string newComfortZoneName = "";
            //            do
            //            {
            //                newComfortZoneName = word[0] + "_" + countForComfortZone++;
            //            } while (newComfortZoneName == name);


            //            PlotComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, prevComfortzoneColor, newComfortZoneName);

            //            //Now we need to insert new comfort zone now  
            //            InsertComfortZoneValue(newComfortZoneName, minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, prevComfortzoneColor);

            //            //After inserting updating the comfort zone info as well
            //            insertOrUpdateComfortChartSetting(chartDetailList[indexOfChartSelected].chartID, temporaryComfortZoneID_Store);


            //            //--Refresh the comfort zone

            //            if (dataGridView1.Rows.Count > 0)  //If there is data then only do this one
            //            {
            //                //set parameters of your event args
            //                var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex);
            //                // or setting the selected cells manually before executing the function
            //                dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
            //                dataGridView1_CellClick(sender, eventArgs);
            //            }

            //        }
            //        flagForEditComfortZoneGraphically = 0;//Resetting flag


            //    }
            //}




            //Now we do others task

            //This one is last one when released go back to 0 initial state not selected
            //if (flagForBorderLineSelectedForMoveForEditCF == 2)
            //{
            //    flagForBorderLineSelectedForMoveForEditCF = 0;
            //}





            //}
            //else
            //{

            //This flagForDisconnectClick is for the line not node
            if (flagForDisconnectClick == 1)
            {
                //--This one is for line
                if (flagNodeSelectedForConnect == 1)  //Node selected for connect 
                {
                    //--Here we need to do the resetting of the datas in the arraylist and replotting it ....
                    //--This function does the resetting the line properties...
                    ResettingLines(f1);//--Calling the resetting the lines..

                    ReDrawingLineAndNode(chart1);

                    //--Again resetting the values as well ..
                    chart1.Series.Remove(addDottedSeries);//--lets remove the indicator if present 
                    flagForDisconnectClick = 0;
                    flagNodeSelectedForConnect = 0;

                }
            }
            else
            {
                //--This function is used for nodeSelection and releasing node to desired place 
                //--This gets triggered based on mouse select and release..
                //Updating values in database  //This part is of node movement and update not line
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    for (int x = 0; x < listNodeInfoValues.Count; x++)
                    {
                        // UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                        //DatabaseOperations Obj1 = new DatabaseOperations();
                        //Obj1.UpdateNodeInfoToDBWithNewParameters(CurrentSelectedBuilding, menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].marker_Size, (int)menuStripNodeInfoValues[x].airFlow);
                        UpdateNodeInfoToDBWithNewParameters(CurrentSelectedBuilding, listNodeInfoValues[x].ID, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].temperature_source,  listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name,listNodeInfoValues[x].colorValue, listNodeInfoValues[x].marker_Size);
                    }
                }
                NodeSelectionAndRelease(e, chart1, f1); //chart1 is phChart 
                                                        //The node update function should be placed here and the calling 
                                                        //to the redrawing function has to be done  after that.

            }

            //}//Close of else of edit mode
        }//Close of mouse click event 

        //int indexOfLineInTheList = 0; //This holds the id of the line selected.
        public void ResettingLines(Form_Main_PH_Application f1)
        {
            if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 0)
            {
                //No setting is done so return
                //MessageBox.Show("nO UPDATE"); 
                return;
            }

            if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 1) //From prevNode
            {
                //  menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected; //Id of the node selected and

                listLineInfoValues[indexOfLineInTheList].prevNodeId = idOfNodeSelected; //Id of the node selected and
                                                                                               //--Lets update the data here...
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    //Testing...
                    //MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                    //--This is the update function for line values..
                    // UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, idOfNodeSelected, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness);
                    // UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId , menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness, menuStripNodeLineInfoValues[indexOfLineInTheList].name, menuStripNodeLineInfoValues[indexOfLineInTheList].status);
                    UpdateLineInfoToDB(listLineInfoValues[indexOfLineInTheList].ID, listLineInfoValues[indexOfLineInTheList].prevNodeId, listLineInfoValues[indexOfLineInTheList].nextNodeId, listLineInfoValues[indexOfLineInTheList].lineColorValue,listLineInfoValues[indexOfLineInTheList].lineSeriesID, listLineInfoValues[indexOfLineInTheList].lineThickness, listLineInfoValues[indexOfLineInTheList].name, listLineInfoValues[indexOfLineInTheList].status);

                }
            }
            else if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 2)
            {
                listLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected;
                //--Lets update the data here...
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    //Testing...
                    // MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                    //--This is the update function for line values..
                    // UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness, menuStripNodeLineInfoValues[indexOfLineInTheList].name, menuStripNodeLineInfoValues[indexOfLineInTheList].status);
                    UpdateLineInfoToDB(listLineInfoValues[indexOfLineInTheList].ID, listLineInfoValues[indexOfLineInTheList].prevNodeId, listLineInfoValues[indexOfLineInTheList].nextNodeId, listLineInfoValues[indexOfLineInTheList].lineColorValue, listLineInfoValues[indexOfLineInTheList].lineSeriesID, listLineInfoValues[indexOfLineInTheList].lineThickness, listLineInfoValues[indexOfLineInTheList].name, listLineInfoValues[indexOfLineInTheList].status);
                }

            }

        }


        public void NodeSelectionAndRelease(MouseEventArgs e, Chart chart1, Form_Main_PH_Application f1)
        {//this is used to select the partciular id values..


            if (readyForMouseClick == 1)
            {

                if (oneTimeClick == 1)
                {

                    //this is for dissabling insert node when a node is selected
                    f1.CMSinsertNode.Enabled = false;

                    //One tiem click then select the mouse
                    //This is changed to string bbk305
                    setItemSelectedID = idSelected;
                    //  MessageBox.Show("Node grabbed - id=" + setItemSelectedID);
                    f1.Cursor = Cursors.Cross;
                    oneTimeClick = 0;
                    //MessageBox.Show("one time click");
                    mouseClickAction = 1;
                }

                else
                {
                    //reenabling... the insert node 
                    f1.CMSinsertNode.Enabled = true;

                    mouseClickAction = 0;
                    //two time click 
                    oneTimeClick = 1;//again reset to oneTimeClick
                    f1.Cursor = Cursors.Arrow;
                    //MessageBox.Show("Node released by second click");



                    if (Control.ModifierKeys == Keys.Alt)
                    {
                        //--This alter key is for moving along constant x-axis ...
                        //MessageBox.Show(" alt is pressed for x axis constant");


                        //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        listNodeInfoValues[tempIndexForNode].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            listLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}


                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                            labelValue = listNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}


                            //   ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            //ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            ReDrawPoints(chart1, series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);
                            //incrementIndex++;
                            indexForSeriesNodePoint++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (listLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < listLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;
                                // ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);
                            }

                        }

                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting the values...




                    }
                    else if (Control.ModifierKeys == Keys.Shift)
                    {
                        //--This ctrl key is for moving along the y-  axis...

                        //MessageBox.Show("shift  is pressed for y  axis constant");

                        listNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            listLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}


                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                            labelValue = listNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}


                            //   ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            //ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            
                            ReDrawPoints(chart1, series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);
                            //incrementIndex++;
                            indexForSeriesNodePoint++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (listLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < listLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;
                                // ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);
                            }

                        }

                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting the values...


                    }
                    else
                    {

                        //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;
                        listNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                        listNodeInfoValues[tempIndexForNode].yVal = yAxis1;
                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;
                        series1.Points.Clear();
                        for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            listLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}
                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                            labelValue = listNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}
                            //   ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            //ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            ReDrawPoints(chart1, series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);
                            //incrementIndex++;
                            indexForSeriesNodePoint++;
                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (listLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < listLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;
                                // ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);
                            }

                        }
                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting the values...


                    }//closing of key else part
                }//closing of second click

            }//closing of else block


        }//Close of nodeselectionandrelease

        public void ClearChart(object sender, EventArgs e, Form_Main_PH_Application f1)
        {


            try
            {

                if (MessageBox.Show("Are you sure you want to clear this chart?", "Clear chart", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
                //if (dialogResult == DialogResult.Yes)
                {

                    try
                    {
                        f1.Enabled = false;//optional, better target a panel or specific controls
                        f1.UseWaitCursor = true;//from the Form/Window instance

                        //mc.InsertNodeAndLine(phChart, xCoord, yCoord);

                  

                    //--This function should clear the chart 
                    /*
                    Steps :
                    0.Find which chart is selected.
                    1. delete the information from the chart of both node and line
                    2. Re plot the chart function 
                    */

                    string chartidsel = chartDetailList[indexOfChartSelected].chartID;
                    string chart_resp_nodeIDX = chartDetailList[indexOfChartSelected].chart_respective_nodeID;
                    string chart_resp_lineIDX = chartDetailList[indexOfChartSelected].chart_respective_lineID;

                    flagResistingForDGVChangeSelection = 1;//ENABLE;



                    //MessageBox.Show("DELETE CHART CONT..");
                    //Deleting the content of the chart 
                    DeleteChartContent(chartidsel, chart_resp_nodeIDX, chart_resp_lineIDX);//This deletes the content of chart

                    // MessageBox.Show("CLEAR CHART..");
                    ///This function need to be written not written 
                    ClearChartData();
                    /*
                    clear chart will erase the the menustrip info of line and node so replotting it again
                    */
                    // MessageBox.Show("row clear..");
                    //dataGridView1.DataBindingComplete += this.dataGridView1_DataBindingComplete;
                    //MessageBox.Show("");
                    // MessageBox.Show("Before clear  =" + flagResistingForDGVChangeSelection);

                    f1.dataGridView1.Rows.Clear();
                    // MessageBox.Show("DataGridView_Show_Data..");

                    DataGridView_Show_Data(f1);
                    //MessageBox.Show("row add..");

                    f1.dataGridView1.Rows.Add();
                    //if (dataGridView1.Rows.Count > 0)
                    //{
                    //    dataGridView1.Rows[0].Selected = false;
                    //}
                    //dataGridView1_DataBindingComplete(sender,(DataGridViewBindingCompleteEventArgs)e);
                    ////dataGridView1.ClearSelection();
                    ////dataGridView1.ClearSelection();
                    //MessageBox.Show("IF >SELECTED..");
                    //Select the chart with was selected
                    f1.dataGridView1.CurrentCell.Selected = false;

                    if (chartDetailList.Count > indexOfChartSelected)
                    {
                        //We need to select the particular index 
                        //--I hope this will do the replotting thing as well
                        // dataGridView1.Rows[indexOfChartSelected].Selected = true;//The row is selected 
                        //   MessageBox.Show("indexOfChartSelected= " + indexOfChartSelected);
                        f1.dataGridView1.Rows[indexOfChartSelected].Selected = true;//The row is selected 

                    }

                    flagResistingForDGVChangeSelection = 0;//DISENABLE;

                    if (f1.dataGridView1.CurrentCell.RowIndex.ToString() != "")
                    {
                        //set parameters of your event args
                        var eventArgs = new DataGridViewCellEventArgs(1, f1.dataGridView1.CurrentCell.RowIndex);
                        //or setting the selected cells manually before executing the function
                        //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[dataGridView2.CurrentCell.ColumnIndex].Selected = true;
                        f1.dataGridView1.Rows[f1.dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                       f1.dataGridView1_CellClick(sender, eventArgs);
                    }


                    }
                    finally
                    {
                        f1.Enabled = true;//optional
                        f1.UseWaitCursor = false;
                    }

                }//Close of clear chart

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
}

        void ClearChartData()
        {
            //this.Invalidate();
            //chart1.Invalidate();
            //// chart1.Dispose();//--Releases all the resources used by the chart...
            //plot_new_graph();
            //lb_title_display.Text = "";
            ////--Reseting the menustrip values for new plotting....
            //menuStripNodeLineInfoValues.Clear();
            //menuStripNodeInfoValues.Clear();
            //index = 0;
            //incrementIndex = 0;
            //insertNodeToolStripMenuItem.Enabled = true;/*insert node will be dissable with historical plot so reenabling it*/

            /*
             We need to reinvent this function , 
             we need to save the node and line information and then clear it when clicked 
             We do not want to refresh the entire chart as it was done before
             */


        }


        //=======================This one if for node and line operation===========//



        public void ReDrawingLineAndNode(Chart chart1)
        {

            try
            {
                lock (this)
                {
                    if (listNodeInfoValues.Count > 0)
                    {
                        //--This is for replotting all the things again...
                        if (chart1.InvokeRequired)
                        {
                            chart1.Invoke(new Action(() => series1.Points.Clear()));
                        }
                        else
                        {
                            series1.Points.Clear();
                        }

                        //series1.Points.Clear();
                        for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            listLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //--Resetting the index value
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            string labelValue;

                            labelValue = listNodeInfoValues[x].name;

                            //--Redefined code bbk305
                            ReDrawPoints(chart1, series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue, listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);

                            //CODE : BBK305A
                            //--incrementIndex++;
                            indexForSeriesNodePoint++;

                        }
                        //--resetting incrementIndex
                        if (listLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < listLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;
                                //--tHIS IS REDEFINED code bbk305
                                ReDrawLines(chart1, listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);

                            }

                        }

                        if (chart1.InvokeRequired)
                        {
                            chart1.Invoke(new Action(() => chart1.Invalidate()));//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.
                        }
                        else
                        {
                            chart1.Invalidate();//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.
                        }
                        //CODE :BBK305A \\
                        //incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting the value
                    }//Close of if menustripnodeinfovalues

                }//==Close of the lock
            }
            catch (Exception ex)
            {
                // MessageBox.Show(ex.Message);
            }

        }//--Close of the actual function....


        public void ReDrawPoints( Chart chart1, Series s1, double x, double y, Color c, string source_temperature, string pressure_source, string name1, string labelValueText, int marker_size_value)
        {
            // lock (this) { 

            //s1.ChartType = SeriesChartType.Point;

            //string s = "source => temp:" + source_temperature+",hum : "+source_humidity + "\n Name : " + name1 + "\nLable : " + label1x;// + "\nindex=" + indexForSeriesNodePoint;
            string s = "source \t\n temperature: " + source_temperature + ",\t\n Pressure  :  " + pressure_source + "\n Name        :" + name1;// + "\nindex=" + indexForSeriesNodePoint;
            if (chart1.InvokeRequired)
            {
                chart1.Invoke(new Action(() => s1.ChartType = SeriesChartType.Point));
                chart1.Invoke(new Action(() => s1.MarkerSize = marker_size_value));//= 20;
                chart1.Invoke(new Action(() => s1.MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points.AddXY(x, y)));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].ToolTip = s)); // chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].Label = labelValueText));
                //chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].Color = c));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].Color = c));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].MarkerSize = marker_size_value));
                //--This one is for storing the series
               // chart1.Invoke(new Action(() => listNodeSeriesPlotted.Add(s1.Name)));

            }
            else
            {
                s1.ChartType = SeriesChartType.Point;
                s1.MarkerSize = marker_size_value;//= 20;
                s1.MarkerStyle = MarkerStyle.Circle;
                s1.Points.AddXY(x, y);
                //chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s;
                s1.Points[indexForSeriesNodePoint].ToolTip = s;
                s1.Points[indexForSeriesNodePoint].Label = labelValueText;
                s1.Points[indexForSeriesNodePoint].Color = c;
                s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle;
                s1.Points[indexForSeriesNodePoint].Color = c;
                s1.Points[indexForSeriesNodePoint].MarkerSize = marker_size_value;
                //==ADDING THE SERIES TO THE LIST==//
                //code123
               // listNodeSeriesPlotted.Add(s1.Name);

            }

            //  }//--Close of lock 

        } //Close of redraw point 
        List<DataTypeForNode> temporaryNodeValueStoreForRedrawLine = new List<DataTypeForNode>();
        //--Redraw line function
        public void ReDrawLines(Chart chart1,string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color c, int thickness_value, string name, int status)
        {
            // lock (this) { 

            // if (incrementIndex > 0)
            //  if(indexForSeriesNodePoint>0) //This index is resetted later
            //   {
            temporaryNodeValueStoreForRedrawLine.Clear();//Clearing the values of the list
                                                         // MessageBox.Show("ReDrawLines FRIST LINE");


            if (chart1.InvokeRequired)
            {
                //now lets plot lines between tow points...
                chart1.Invoke(new Action(() => newLineSeries = lineSeriesID));//new Series("LineSeries" + incrementIndex); //lineSeriesID; 


                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Invoke(new Action(() => chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name))));
                }

                //}
                chart1.Invoke(new Action(() => newLineSeries.MarkerSize = 1));
                chart1.Invoke(new Action(() => newLineSeries.ChartType = SeriesChartType.Line));
                if (thickness_value <= 0)
                {
                    chart1.Invoke(new Action(() => newLineSeries.BorderWidth = 3));
                }
                else
                {
                    chart1.Invoke(new Action(() => newLineSeries.BorderWidth = thickness_value));//This is the thickness of lines
                }
                //newSeries.ToolTip = 
                chart1.Invoke(new Action(() => newLineSeries.Color = c));
            }
            else
            {

                newLineSeries = lineSeriesID;//new Series("LineSeries" + incrementIndex); //lineSeriesID; 
                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name));
                }
                newLineSeries.MarkerSize = 1;
                newLineSeries.ChartType = SeriesChartType.Line;
                if (thickness_value <= 0)
                {
                    newLineSeries.BorderWidth = 3;
                }
                else
                {
                    newLineSeries.BorderWidth = thickness_value;//This is the thickness of lines
                }
                newLineSeries.Color = c;

            }


            /*
            We need to calculate the previous node id values and the next node id values.
            */
            //var nc = new NodeAndLineClass();
            //First for previous node id
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                if (prevNodeID == listNodeInfoValues[i].ID)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore =  menuStripNodeInfoValues.GetRange(i,1);  //This is for copying the value.
                    //Copying the values on index 0 assumption 
                    temporaryNodeValueStoreForRedrawLine.Add(new DataTypeForNode
                    {
                        ID = listNodeInfoValues[i].ID,
                        xVal = listNodeInfoValues[i].xVal,
                        yVal = listNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = listNodeInfoValues[i].temperature_source,
                        pressure_source = listNodeInfoValues[i].pressure_source,
                        name = listNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = listNodeInfoValues[i].colorValue,
                        marker_Size = listNodeInfoValues[i].marker_Size,
                        // airFlow = menuStripNodeInfoValues[i].airFlow
                        lastUpdatedDate = listNodeInfoValues[i].lastUpdatedDate

                    });

                    break;//Break form loop
                }
            }

            //Second for the next node id
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                if (nextNodeID == listNodeInfoValues[i].ID)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore[1].Equals(menuStripNodeInfoValues[i]);

                    //temporaryNodeValueStore = menuStripNodeInfoValues.GetRange(i, 1);
                    //The index of this values will be temporaryNodeValueStore[1] ==> 1
                    temporaryNodeValueStoreForRedrawLine.Add(new DataTypeForNode
                    {
                        ID = listNodeInfoValues[i].ID,
                        xVal = listNodeInfoValues[i].xVal,
                        yVal = listNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = listNodeInfoValues[i].temperature_source,
                        pressure_source = listNodeInfoValues[i].pressure_source,
                        name = listNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = listNodeInfoValues[i].colorValue,
                        marker_Size = listNodeInfoValues[i].marker_Size,
                        //airFlow = menuStripNodeInfoValues[i].airFlow
                        lastUpdatedDate = listNodeInfoValues[i].lastUpdatedDate

                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStoreForRedrawLine.Count > 0)
            {

                //--this sets the initial values of humidity and enthalpy
                string tooltipString = "";
                ///*

                string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

                string ZeroLine = "Process:      " + name + " ";
                string dottedLine = "------------------------------------------------------------------";
                string FirstLine = @"Parameters               " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
                string SecondLine = @"enthalpy                " + "KJ/KG              " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                  " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
                //string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                string FourthLine = @"Pressure                " + "MPa                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                     " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);
                //string FifthLine = "Volume Flow Rate           " + "m\xB3/s                " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2) + "                      " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2);

                tooltipString = ZeroLine +"\n"+dottedLine +"\n" + FirstLine + "\n" + SecondLine + "\n" + FourthLine;


                //*/

                //===============This one is for datatable======================================================//
                //DataTable table = new DataTable();
                //table.Columns.Add("Parameters", typeof(string));
                //table.Columns.Add("Units", typeof(string));
                //table.Columns.Add(temporaryNodeValueStoreForRedrawLine[0].name.ToString(), typeof(string));
                //table.Columns.Add(temporaryNodeValueStoreForRedrawLine[1].name.ToString(), typeof(string));

                //table.Rows.Add("DBT", "\x00B0 C", Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2).ToString());
                //table.Rows.Add("Relative Humidity", "%  ", startHumidity1.ToString(), endHumidity1.ToString());
                //table.Rows.Add("Humidity Ratio", "Kg/Kg dryair", Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2).ToString());
                //table.Rows.Add("Volume flow rate", "m\xB3/s ", Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2).ToString());
                //table.Rows.Add("Sp. Volume", "m\xB3/Kg", startSpecificVolume1.ToString(), endSpecificVolume1.ToString());
                //table.Rows.Add("Mass Flow rate(dry air)", "Kg(dry air)/s", Math.Round(massFlowRate1, 2).ToString(), Math.Round(massFlowRate2, 2).ToString());
                //table.Rows.Add("Enthalpy", "KJ/Kg ", startEnthalpy1.ToString(), endEnthalpy1.ToString());
                //table.Rows.Add("Total Energy Flow", "KJ/s", Math.Round(totalEnthalpyFlow1, 2).ToString(), Math.Round(totalEnthalpyFlow2, 2).ToString());
                //table.Rows.Add("Heat Change", "KW ", Math.Round(heatChange, 2).ToString(), "");
                // MessageBox.Show("Table \n" + table.Columns[2].Rows[3].ToString());
                //tooltipString = table.ToString();
                //================================datatable close==============================================//
                if (chart1.InvokeRequired)
                {

                    chart1.Invoke(new Action(() => newLineSeries.ToolTip = tooltipString));
                    //newSeries.MarkerStyle = MarkerStyle.Circle;
                    //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);

                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal))));   //for prevnodeid
                    double mid_point_XValue = (temporaryNodeValueStoreForRedrawLine[0].xVal + temporaryNodeValueStoreForRedrawLine[1].xVal) / 2;
                    double mid_point_YValue = ((temporaryNodeValueStoreForRedrawLine[0].yVal)/2 + (temporaryNodeValueStoreForRedrawLine[1].yVal) / 2);
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue))));   //Middle point for plotting the Label
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[1].xVal, temporaryNodeValueStoreForRedrawLine[1].yVal))));   //for nextnodeid

                    if (status == 1)
                    {
                        //Then show the label in the chart
                        chart1.Invoke(new Action(() => newLineSeries.Points[1].Label = name));
                    }

                    chart1.Invoke(new Action(() => chart1.Series.Add(newLineSeries)));

                }
                else
                {
                    newLineSeries.ToolTip = tooltipString;
                    newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal));   //for prevnodeid
                    double mid_point_XValue = (temporaryNodeValueStoreForRedrawLine[0].xVal + temporaryNodeValueStoreForRedrawLine[1].xVal) / 2;

                    //This is done to calculate the log mid point 
                    /*
                     * steps: calulate anti log of log values so it becomes linear
                     * then calculate mid value of linear value and
                     * then again calculage log value
                     */
                    //double mid_point_YValue = Math.Log((Math.Pow(10, temporaryNodeValueStoreForRedrawLine[0].yVal) + Math.Pow(10, temporaryNodeValueStoreForRedrawLine[1].yVal)) / 2);
                    // double r = Math.Sqrt(temporaryNodeValueStoreForRedrawLine[0].yVal + temporaryNodeValueStoreForRedrawLine[1].yVal);

                    //Gemetric mean is calculated because the y axis is in lograthmic value which is geometric progression
                    double mid_point_YValue = Math.Sqrt(temporaryNodeValueStoreForRedrawLine[0].yVal * temporaryNodeValueStoreForRedrawLine[1].yVal);// / r;//10;
                    newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue));   //Middle point for plotting the Label

                    newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[1].xVal, temporaryNodeValueStoreForRedrawLine[1].yVal));   //for nextnodeid
                    if (status == 1)
                    {
                        //Then show the label in the chart
                        newLineSeries.Points[1].Label = name;
                    }

                    chart1.Series.Add(newLineSeries);
                }

               // temporaryNodeValueStoreForRedrawLine.Clear();//--Lets reset the value CODE:BBK305A

                //   }  //close of if incrementindex


            }//close of temporary node value

            // }//--Close of LOCK
        }

        public int IndexOfPreviousNodeForLineFunction()
        {
            //int indCountForPrevIdOfLine = 0;
            string idValue = "";
            int indexX = 0;
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                //--Now we need to count the valus
                //--No more mix value is present now 
                //if (listNodeInfoValues[i].temperature_source == "Mix")
                //{
                //    //Do not count 
                //}
                //else
                //{
                //Its not mix node so count every other nodes
                //indCountForPrevIdOfLine++;
                idValue = listNodeInfoValues[i].ID;
                //}
            }
            //--Now lets identify the node value
            for (int i = 0; i < listNodeInfoValues.Count; i++)

            {
                /*
               indCountForPrevIdOfLine-1 this -1 is done because 
                          indCountForPrevIdOfLine 1 means index is 0 
               */
                if (idValue == listNodeInfoValues[i].ID)
                {
                    indexX = i;
                    break;
                }
            }
            return indexX;
        }

        public void InsertNodeAndLine(Chart chart1,double xAxis,double yAxis)
        {
           // chart1 = chart1X;
            //This will help in quick insertion           
            int t = listNodeInfoValues.Count;
            indexRun = 1;
            if (t == 0)
            {
                indexRun = 1;
            }
            else
            {
                foreach (var node in listLineInfoValues)
                {
                    indexRun++;
                }
            }
            //We also need to check if the value is present or not after setting 
            int indNodeRun = indexRun;
            string nodeName = "Node" + indNodeRun;//This is the node name
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                if (listNodeInfoValues[i].name == nodeName)
                {
                    indNodeRun++;
                    nodeName = "Node" + indNodeRun;
                    i = 0;
                }
            }
            //--For line
            int indLineRun = indexRun;//- 1;
            //This one is for line Name
            string lineName = "Line" + indLineRun;
            for (int i = 0; i < listLineInfoValues.Count; i++)
            {
                if (listLineInfoValues[i].name == lineName)
                {
                    indLineRun++;
                    lineName = "Line" + indLineRun;
                    i = 0;
                }

            }

           // MessageBox.Show("X =" + xCoordinateValue + ",Y = " + yCoordinateValue);
            //return;

            //var results = chart1.HitTest((int)xCoordinateValue, (int)yCoordinateValue, false, ChartElementType.PlottingArea);
            //foreach (var result in results)
            //{
            //    if (result.ChartElementType == ChartElementType.PlottingArea)
            //    {
            //        // double xVal = xCoord - chart1.ChartAreas[0].Position.X ;
            //        //double yVal = yCoord-chart1.ChartAreas[0].Position.Y;
                    
            //        var xVal = result.ChartArea.AxisX.PixelPositionToValue(xCoordinateValue);
            //        var yVal =Math.Abs( result.ChartArea.AxisY.PixelPositionToValue(yCoordinateValue));

            //        //MessageBox.Show("aFTER CONVERTING THE VALUES=" + xVal + ",YVAL" + yVal);
            //        //return;
                    SetNodeAndLine(chart1,xAxis, yAxis, "Manual", "Manual", nodeName, Color.Blue, nodeDefaultThickness, lineName, 1, lineDefaultThickness);

            //        break;
            //    }
            //}


        }//Close ofinsetnode

        //--SetNode Function
        public void SetNodeAndLine(Chart chart1,double xAxisValue, double yAxisValue, string temperatureSource, string pressureSource, string name, Color c1, int markerSize, string lineNameVal, int lineStatusVal, int lineThickness)
        {


            //ChartCreation ch = new ChartCreation();
            double xMinRangeValue = xAxisMinimum;
            double xMaxRangeValue = xAxisMaximum;
            double yMinRnageValue = yAxisMinimum;
            double yMaxRangeValue = yAxisMaximum;


            ////now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if (((xAxisValue > xMinRangeValue && xAxisValue <= xMaxRangeValue) && (yAxisValue > yMinRnageValue && yAxisValue <= yMaxRangeValue)))
            {
                //now lets plot the values....
                // plot_by_DBT_HR_process_diagram((double)(int)temperatureValue, (double)humidityValue / 100);
                PlotOnGraphAndInsertToDB(chart1, xAxisValue, yAxisValue, temperatureSource, pressureSource, name, c1, markerSize, lineNameVal, lineStatusVal, lineThickness);
            }
            else
            {
                MessageBox.Show("Please select a proper region");

            }

        }//Close of SetNode

        //==We need to do some callings or so...
        public void PlotOnGraphAndInsertToDB(Chart chart1,double xval, double yval, string temperatureSource, string pressureSource, string name, Color c1, int markerSize, string lineNameVal, int lineStatusVal, int linethickness)
        {
            //chart1.Series.Clear();


            //try
            //{


            series1.ChartType = SeriesChartType.Point;
            //int r, g, b;

            //series1.MarkerSize = 20;
            //series1.MarkerStyle = MarkerStyle.Circle;
            //series1.Points.AddXY(xval, yval);
            //string s = "source :\nTemperature Source " + temperature_sourceGlobal + "\nHumidity Source" + humidity_sourceGlobal + "\n Name : " + tbName;
            //series1.Points[index].Color = colorValue;
            //series1.Points[index].ToolTip = s;
            int countNumberOfNodes = listNodeInfoValues.Count;//for no data 0 for 1 index =1 and so on
            //--we faced problem here code123
            SetNode(series1, countNumberOfNodes, xval, yval, name, c1, markerSize, temperatureSource, pressureSource);

            //string labelStringValue = null;
            //labeling part
            //if (comboboxItemText == "Label")
            //{
            //    //label is selected
            //    labelStringValue = tbLabel;
            //}
            //else if (comboboxItemText == "Name")
            //{
            //Name is selected
            // labelStringValue = tbName;
            //}
            //else
            //{
            //    //Source is selected
            //    labelStringValue = tbSource;
            //}

            //  series1.Points[index].Label = labelStringValue;

            //  MessageBox.Show("value xval =" + xval + ",yval = " + yval);
            //series1.Points[index_series++].Color = colorValue;//blue
            //    MessageBox.Show("end re");
            //index_series++;
            //series1.Enabled = true;
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

            //now lets move on to storing those values and futher porcessing it...


            //we need to get this a unique id 

            string unique_id_for_node =selectedBuildingList[0].BuildingName + GetGUID();//buildingList[0].BuildingName + GetGUID();//This is the unique id 


            //the value is added...
            listNodeInfoValues.Add(new DataTypeForNode
            {
                ID = unique_id_for_node,
                xVal = xval,
                yVal = yval,
                // source = tbSource,
                temperature_source = temperatureSource,
                pressure_source = pressureSource,
                name = name,
                // label = tbLabel,
                colorValue = c1,
                // showItemText = comboboxItemText,
                marker_Size = markerSize,
                //airFlow = airFlowValueGlobal
                lastUpdatedDate = DateTime.Now.ToString()

            });

            //--Inserting the node values in the database sqlite


            if (flagForInsertOrUpdateDataToDB == 1)
            {
                //do only if the  flag is raised.

                // InsertNodeInfoToDB(unique_id_for_node, xval, yval, tbSource, tbName, tbLabel, colorValue, comboboxItemText, markerSize, deviceInstanceValue, deviceIP, deviceParam1ID, deviceParam2ID, device_param1_info_for_node, device_param2_info_for_node, object_param1_identifier_type, object_param2_identifier_type);
                //--New code is added to here
                // var objDbOperation = dom;//new DatabaseOperations();
                //  MessageBox.Show("Building Name= " + selectedBuildingList[0].BuildingName);
                // MessageBox.Show(unique_id_for_node+"xval = "+ xval+" yval"+ yval+"temp "+ temperature_sourceGlobal+"hum="+ humidity_sourceGlobal+",name="+ tbName+",col="+ colorValue+",size=" +markerSize +",air="+airFlowValueGlobal.ToString());
                //objDbOperation.InsertNodeInfoToDBWithoutDeviceInfo(dom.buildingList[0].BuildingName, dom.chartDetailList[bo.indexForWhichChartIsSelected].chart_respective_nodeID, unique_id_for_node, xval, yval, temperatureSource, pressureSource, name, c1, markerSize);
                InsertNodeInfoToDBWithoutDeviceInfo(selectedBuildingList[0].BuildingName, chartDetailList[indexForWhichChartIsSelected].chart_respective_nodeID, unique_id_for_node, xval, yval, temperatureSource, pressureSource, name, c1, markerSize);
                // MessageBox.Show("Operation Complete Test");
                //InsertNodeInfoToDBWithoutDeviceInfo(unique_id_for_node,)
            }



            //the liine plot part is only done when ther is two points or more
            //if (index > 0)
            if (countNumberOfNodes > 0)
            {
                //MessageBox.Show("Here we are inside line");
                //double startHumidity1 = 0;
                //double startEnthalpy1 = 0;
                //double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
                //double endEnthalpy1 = 0;

                //now lets plot lines between tow points...
                Series newLineSeries = new Series("LineSeries" + countNumberOfNodes);
                //string nameSeries = newLineSeries.Name;

                //--If the series already present lets remove from the chart ok ol :)
                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //MessageBox.Show("Series exits");
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name));
                }

                string unique_id_for_line = selectedBuildingList[0].BuildingName + GetGUID();//This is the unique id  //buildingList[0].BuildingName + GetGUID();//selectedBuildingList[0].BuildingName + GetGUID();//This is the unique id 



                //--Lets store the nodeline info as well
                listLineInfoValues.Add(new DataTypeForLine
                {
                    //--Id of this ..
                    ID = unique_id_for_line,
                    // prevNodeId = menuStripNodeInfoValues[index - 1].id, //previousNodeIndexForLineInput
                    prevNodeId = listNodeInfoValues[countNumberOfNodes - 1].ID,
                    nextNodeId = listNodeInfoValues[countNumberOfNodes].ID,
                    lineColorValue = listNodeInfoValues[countNumberOfNodes - 1].colorValue,
                    lineSeriesID = newLineSeries,
                    lineThickness = 3, //default thickness is 3
                    name = lineNameVal,
                    status = lineStatusVal

                });


                //--Adding to db
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                   // MessageBox.Show("Flag forupdate or insert insde line should be 1= " + flagForInsertOrUpdateDataToDB);
                    //--Insert the values when the flag is raised.
                    //InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[index - 1].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries, menuStripNodeLineInfoValues[index - 1].lineThickness);
                    // InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[index - 1].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries,3);

                    InsertLineInfoToDB(unique_id_for_line, listNodeInfoValues[countNumberOfNodes - 1].ID, listNodeInfoValues[countNumberOfNodes].ID, listNodeInfoValues[countNumberOfNodes].colorValue, newLineSeries.Name.ToString(), linethickness, chartDetailList[indexForWhichChartIsSelected].chart_respective_lineID, lineNameVal, lineStatusVal);
                }

                ReDrawLines(chart1, unique_id_for_line, listNodeInfoValues[countNumberOfNodes - 1].ID, listNodeInfoValues[countNumberOfNodes].ID, newLineSeries, listNodeInfoValues[countNumberOfNodes].colorValue, linethickness, lineNameVal, lineStatusVal);

                /*
                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newLineSeries.ChartType = SeriesChartType.Line;
                //newLineSeries.MarkerBorderWidth.Equals(15);
                newLineSeries.MarkerSize.Equals(linethickness);
                //newLineSeries.BorderWidth.Equals(15);
                // newLineSeries.SetCustomProperty(newLineSeries.MarkerSize.ToString(),newLineSeries.MarkerSize.Equals(25).ToString());
                newLineSeries.Color = listNodeInfoValues[countNumberOfNodes].colorValue;

                //=====================================THisi used========================//

                string tooltipString = "";

                // double enthalpyChange = endEnthalpy1 - startEnthalpy1;
                string sequenceDetected = listNodeInfoValues[countNumberOfNodes - 1].name + " to " + listNodeInfoValues[countNumberOfNodes].name;


                string ZeroLine = "Process:  " + name + " ";
                string FirstLine = @"Parameters                      " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
                string SecondLine = @"enthalpy                                   " + "KJ/KG                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                           " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
                //string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                string FourthLine = @"Pressure               " + "MPa  " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                       " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);

                tooltipString = sequenceDetected + "\n" + ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + FourthLine;


                newLineSeries.ToolTip = tooltipString;

                //=============================end of this is used======================//
                newLineSeries.Points.Add(new DataPoint(listNodeInfoValues[countNumberOfNodes - 1].xVal, listNodeInfoValues[countNumberOfNodes - 1].yVal));
                //double mid_point_XValue = (menuStripNodeInfoValues[index - 1].xVal + menuStripNodeInfoValues[index].xVal)/ 2;
                //double mid_point_YValue = (menuStripNodeInfoValues[index - 1].yVal + menuStripNodeInfoValues[index].yVal) / 2;
                double mid_point_XValue = (listNodeInfoValues[countNumberOfNodes - 1].xVal + listNodeInfoValues[countNumberOfNodes].xVal) / 2;
                double mid_point_YValue = (listNodeInfoValues[countNumberOfNodes - 1].yVal + listNodeInfoValues[countNumberOfNodes].yVal) / 2;

                newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue));
                newLineSeries.Points.Add(new DataPoint(listNodeInfoValues[countNumberOfNodes].xVal, listNodeInfoValues[countNumberOfNodes].yVal));

                if (lineStatusVal == 1)
                {
                    newLineSeries.Points[1].Color = c1;
                    newLineSeries.Points[1].Label = lineNameVal;
                }
                chart1.Series.Add(newLineSeries);
                chart1.Series[newLineSeries.Name].BorderWidth = 3;
                */
                

            }// close of redrawing...



            //index++;
            //--Why is this done ,, don't know // lets see later
            previousNodeIndexForLineInput = IndexOfPreviousNodeForLineFunction();


        }//close of buttons

        public void SetLine(Series newSeries, int nodePointCount, double xAxisValue, double yAxisValue, string lineName, Color colorValue, int markerSize, string temperatureSource, string pressureSource)
        {
            newSeries.ChartType = SeriesChartType.Line;
            newSeries.MarkerStyle = MarkerStyle.Circle;
            newSeries.MarkerSize = markerSize;
            newSeries.Points.AddXY(xAxisValue, yAxisValue);
            //string s = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";
            newSeries.Points[nodePointCount].Color = colorValue;
            //newSeries.Points[nodePointCount].ToolTip = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {lineName}";//s;
            //newSeries.Points[nodePointCount].Label = lineName;
            string tooltipString = "";

            string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

            string ZeroLine = "Process:  " + lineName + " ";
            string FirstLine = @"Parameters                      " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
            string SecondLine = @"enthalpy                                   " + "KJ/KG                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                           " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
            //string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
            string FourthLine = @"Pressure               " + "MPa  " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                       " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);
            tooltipString = ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + FourthLine;

        }

        /// <summary>
        /// Sets the node to Chart 
        /// </summary>
        /// <param name="series1"></param>
        /// <param name="xAxisValue"></param>
        /// <param name="yAxisValue"></param>
        /// <param name="nodeName"></param>
        /// <param name="c1"></param>
        /// <param name="markerSize"></param>
        public void SetNode(Series newSeries, int nodePointCount, double xAxisValue, double yAxisValue, string nodeName, Color colorValue, int markerSize, string temperatureSource, string pressureSource)
        {
            newSeries.ChartType = SeriesChartType.Point;
            newSeries.MarkerStyle = MarkerStyle.Circle;
            newSeries.MarkerSize = markerSize;
            newSeries.Points.AddXY(xAxisValue, yAxisValue);
            //string nodeTips = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";
            newSeries.Points[nodePointCount].Color = colorValue;
            newSeries.Points[nodePointCount].ToolTip = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";//nodeTips;
            newSeries.Points[nodePointCount].Label = nodeName;
        }

       
        public void DeleteNode(object sender, EventArgs e,Form_Main_PH_Application f1,Chart chart1)
        {
            //--Delete the node currently selected 
            /*Steps  : 
            1. Find node id first 
            2. delete the node from node list 
            3. Delete the corresponding line id
            4.Refresh the button            
            */


            if (FlagForNodeDelete == 1)//Node has been selected
            {

                if (nodeID_ForDeletingNode != "")
                {
                    //--Then we want to perform some operation
                    DeleteSingleNodeAndAssociatedLinesInDB(nodeID_ForDeletingNode);


                    //--Replotting the data again
                    RefreshGraph(chart1,f1);

                    LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

                    ReDrawingLineAndNode(chart1);



                }
            }
        }


        public void DisconnectLineFromNodeA(object sender, EventArgs e,Form_Main_PH_Application f1,Chart chart1)
        {

            //Deleting form A

            //--This one is for  detaching line form A
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 1;//now prev node is ON

            //---------This code is from Disconnect Line menustipe-------//


            //--This one is for disconnect lines
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (f1.Cursor != Cursors.Cross)
            {
               f1.Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option-----------//

        }

        public void DisconnectLineFromNodeB(object sender, EventArgs e, Form_Main_PH_Application f1, Chart chart1)
        {
            //---This one is for detaching line form B
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 2; //Now next node is ON

            //---------This code is from Disconnect Line menustipe-------//
            //--This one is for disconnect lines
            /*--Steps :
            1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
            2.Then lets show a + cursor to indicate a line has been selected 
            3. When ever the line goes near to a node lets show a hand to drop the line.
            4. When clicked drop then lets connect it to different node which has been dropped.
            */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (f1.Cursor != Cursors.Cross)
            {
                f1.Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option--------//

        }

        //====================End of node and line operations====================//
        //*********************************************************************************************//        
        //======================End of the chart creation and operation FILE=================//


        //================== BuildingOperation.cs File code=====================//



        public void DGVSelectionChanged(object sender, EventArgs e, Form_Main_PH_Application f)
        {
            //--This need to fire an event when the selection is done 

            if (flagResistingForDGVChangeSelection == 1)
            {
                //MessageBox.Show("Test");
                return;
            }

            if (chartDetailList.Count > 0)
            {
                //try { 
                var cellCopy = f.dataGridView1.CurrentCell;

                if (cellCopy != null)
                {
                    if (f.dataGridView1.CurrentCell.ColumnIndex.ToString() != "" && f.dataGridView1.CurrentCell.RowIndex.ToString() != "")
                    {

                        if (f.dataGridView1.CurrentCell.RowIndex >= 0 && f.dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                        {
                            var eventArgs = new DataGridViewCellEventArgs(f.dataGridView1.CurrentCell.ColumnIndex, f.dataGridView1.CurrentCell.RowIndex);
                            //or setting the selected cells manually before executing the function
                            f.dataGridView1.Rows[f.dataGridView1.CurrentCell.RowIndex].Cells[f.dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                            f.dataGridView1_CellClick(sender, eventArgs);
                            //MessageBox.Show("selecteion change clicked");
                        }
                    }

                }
                //}catch(Exception ex)
                //{
                //    MessageBox.Show(ex.Message);
                //}
            }
        }//Close of SelectionChange
        public void DGVCellBeginEdit(object sender, DataGridViewCellCancelEventArgs e, Form_Main_PH_Application f)
        {

            //--This start the editing of the cells
            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                f.dataGridView1.Tag = f.dataGridView1.CurrentCell.Value;
                if (f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
                {
                    beginEditText = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();

                }
            }
        }

        public void DGVCellEndEdit(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {


            // if(Control.ModifierKeys == Keys.Tab) { 
            //--Here we need to add datato the database...

            //try
            //{

            //MessageBox.Show("end edit");
            //--This section helps in validation..
            if (f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
            {
                var regex = new Regex(@"^[a-zA-Z0-9_]+$");
                string name = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                // MessageBox.Show("name= " + name);
                if (regex.IsMatch(name))
                {
                    int length = name.Length;
                    if (length > 30)
                    {
                        //}
                        //else
                        //{

                        MessageBox.Show("You can only have name upto 30 characters");
                        f.dataGridView1.Refresh();
                        if (beginEditText == "")
                        {
                            f.dataGridView1.CurrentCell.Value = "";
                        }
                        else
                        {
                            f.dataGridView1.CurrentCell.Value = beginEditText;
                        }
                        return;
                        // }

                    }
                    //--Testing ..
                    //  MessageBox.Show(" name = " + name);
                    //--we need to check that the table name enter doesnot matches previous values

                    if (name != beginEditText)
                    {
                        for (int i = 0; i < chartDetailList.Count; i++)
                        {
                            if (name == chartDetailList[i].chartName)
                            {
                                MessageBox.Show("chart can not have same name");
                                f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;

                                return;
                            }
                        }

                        //--The value is ok 
                        //--Now lets do the insert opeation for the data...



                        AddOrUpdateNewRecord(name, e, f);

                        //  MessageBox.Show("regx name");
                        //--now lets add new rows..
                        f.dataGridView1.Refresh();
                        //dataGridView1.Rows.Clear();//Remove the rows
                        // BeginInvoke(new MethodInvoker(PopulateControl));
                        //This flag is raised so that first coloumn is not selected everytime we do update
                        flagResistingForDGVChangeSelection = 1;//enable cell edit
                        f.dataGridView1.Rows.Clear();

                        //--Lets get the data and refill the data in datagridview.
                        // PullDataFromDB();


                        //LETS FILL THE DATA
                        // fillDataGridView();
                        // MainController mc = new MainController();
                        DataGridView_Show_Data(f);//This will do both pulling data and filing the data...

                        f.dataGridView1.Rows.Add();
                        f.dataGridView1.CurrentCell.Selected = false;

                        //===============This one for refreshing the chart and selecting new created chart===//

                        //if (chartDetailList.Count > 0)
                        //{

                        if (f.dataGridView1.Rows.Count > 0)
                        {
                            //We need to select the particular index 
                            //--I hope this will do the replotting thing as well
                            if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                            {
                                f.dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;//The row is selected 
                            }

                        }


                        if (f.dataGridView1.Rows.Count > 0) //If there is data in datagridview then only
                        {
                            //MessageBox.Show("Clicked here row index: " + e.RowIndex);
                            var eventArgs = new DataGridViewCellEventArgs(1, e.RowIndex);
                            // or setting the selected cells manually before executing the function
                            // MessageBox.Show("END edit = " + e.RowIndex);
                            f.dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;
                            DGVCellClick(sender, eventArgs, f, f.phChart);
                            // }
                        }

                        flagResistingForDGVChangeSelection = 0;//close enable of dgv_selectionChange event 

                        //===================end of selection of new created chart =======================//
                    }
                    else
                    {
                        //  MessageBox.Show("No change in name");
                    }


                }
                else
                {
                    MessageBox.Show("You can only have letters, numbers and underscores");
                    f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = string.Empty;

                }


            }


            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show("Edit and then press tab to commit changes " + ex.Message);

            //    f.dataGridView1.CurrentCell.Value = beginEditText;
            //    // }
            //}
        }

        public void AddOrUpdateNewRecord(string name, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {
            /*
            //--We need to count the index of the current datagridview if the row index 
                is greater then the data in the list of data             
            */
            if (e.RowIndex + 1 > chartDetailList.Count)
            {
                //insert the data...
                string nameValue = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                insertNewDataFromDGV(nameValue);

            }
            else
            {
                //--Update the data
                string chartIDValue = chartDetailList[e.RowIndex].chartID;
                string nameValue = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                UpdateNewDataFromDGV(nameValue, chartIDValue);

            }
        }
        public void DGVCellDoubleClick(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {
            if (f.dataGridView1.CurrentCell.ColumnIndex == 1)
            {
                f.dataGridView1.CurrentCell.ReadOnly = false;
                f.dataGridView1.BeginEdit(true);

            }

        }

        int indexSelectedForDeletion = 0;

        public void DGVCellMouseClick(object sender, DataGridViewCellMouseEventArgs e, Form_Main_PH_Application f, ContextMenuStrip cms)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.RowIndex >= 0 && e.ColumnIndex == 0)
                {
                    if (f.dataGridView1.Rows[e.RowIndex].Cells[0].Value != null)
                    {
                        Point p = new Point(e.X, e.Y);
                        indexSelectedForDeletion = e.RowIndex;

                        int xAxis = (int)e.X;
                        //--now alsow count the row index so that yavis = e.y+ (rowIndex*constantMultiple)
                        int rowIndex = f.dataGridView1.CurrentCell.RowIndex;
                        int yAxis = (int)e.Y + (rowIndex * 22);

                        //new Point((int)e.X, (int)e.Y)
                        //contextMenuStrip1.Show(dataGridView1, new Point(xAxis, yAxis));
                        cms.Show(f.dataGridView1, new Point(xAxis, yAxis));
                        //contextMenuStrip1.Show(dataGridView1.PointToScreen(e.Location));
                    }
                }


            }
        }
        public void DGVCellClick(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f, System.Windows.Forms.DataVisualization.Charting.Chart chart1)
        {

            if (f.dataGridView1.Rows.Count <= 0)
            {
                return;
            }

            if (flagForTimer == 1)
            {
                if (atimer.Enabled) // Check if the timer is running
                {
                    //Return error.........
                    atimer.Enabled = false;
                    atimer.Dispose();
                    flagForTimer = 0;

                }
            }//close of flagfortimer

           
            //--Lets check if the datagridview1 is empty or chartDetailList is empty
            if (chartDetailList.Count <= 0)
            {
                //--This flag is for OFFLINE mode Or ONLINE mode
                //--Now you are going to turn OFF the offline mode and go into realtime mode
                FlagForCntdBG_Update = 0;//currently in realtime mode so 1


                //--also make the radio button to be OFF and other to be ON
                // rb_ON.Checked = true;

                //--IFno chart is present thMMen make edit mode on
                FlagForLockUnlock = 1;//   This means edit mode ON 
                LockAndUnlock(f);//This method will make things lock

                return;
            }

            //--Showing the data on cell selected...
            // MessageBox.Show("CELL SELECT " );
            //When dgv is click it clicks twice this if is written to stop those twice click.

            if (flagSinglCellClick == 1)
            {
                flagSinglCellClick = 0;
                return;
            }
            if ((e.RowIndex >= 0) && (e.ColumnIndex >= 0))
            {

                if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                {
                    // MessageBox.Show("CELL SELECT ROW= " + e.RowIndex+",columns = "+e.ColumnIndex);
                    //The row is selected ..
                    for (int i = 0; i < chartDetailList.Count; i++)
                    {
                        //--Checking for evey item
                        if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString() == chartDetailList[i].chartName)
                        {
                            //if match found load
                            //data_load();
                            //lb_title_display.Text = "";//--Historical plot needs to be cleared
                            frm1 = f;//Copying the object for further use

                            indexRun = 0;//For inserting nodeName and lineName like node1,line1 etc
                            chart1.Enabled = true;

                            //--This flag is for OFFLINE mode Or ONLINE mode
                            //--Now you are going to turn OFF the offline mode and go into realtime mode
                            FlagForCntdBG_Update = 1;//currently in realtime mode so 1

                            //--also make the radio button to be OFF and other to be ON
                            // rb_OFF.Checked = true;

                            FlagForLockUnlock = 0;//   This means edit mode OFF 
                            LockAndUnlock(f);//This method will make things lock

                            //This one(the signal) is for stoping the background running process instantly 
                            FlagForStopingRunningProcessInstantly = 1;


                            RefreshGraph(chart1, f);
                            /*
                            We need to identify which item in the list was clicked .
                              //Before we load line we need to identify the id of the chart to load the data..
                              */
                            //chartSelectedIDValue = chartDetailList[i].chartID;//--tHIS CONTAINS THE ID OF THE CHART
                            indexForWhichChartIsSelected = i;
                            //This has been changed
                            indexOfChartSelected = e.RowIndex;    //This value is changed 
                            LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

                            flagForInsertOrUpdateDataToDB = 1;
                            //--This is also completed..
                            //var ch = new ChartOperation();
                            //--This needs to be removed or somethings 
                            //var ch = new ChartCreationAndOperations();
                           // MessageBox.Show("Near RedrawingNodeAandLine(),\n node count"+listNodeInfoValues.Count +",line count"+listLineInfoValues.Count);
                             ReDrawingLineAndNode(chart1);  //Done checking bbk modif: changing to alex db 


                            flagSinglCellClick = 1;


                            //==============This part is for comfort zone=====================

                            //====================end of comfort zone=========================

                            //---Here we are going to call scan hardware function ----------------//

                            //StartHardwareScan();
                            //---End of call for scan hardware-----------------------------------//

                            //--Lets make the status of the device and web to be dissabled 
                            //lb_web_status.Text = "inactive";
                            f.lb_device_status.Text = "disconnected";

                            break;
                        }
                        else
                        {
                            flagForInsertOrUpdateDataToDB = 0;
                            RefreshGraph(chart1, f);
                        }
                    }


                }  //close of != null if
                else
                {
                    flagForInsertOrUpdateDataToDB = 0; //==insert or update OFF
                }


                //This is for cell formating and editing...
                //  if (dataGridView1.CurrentCell.ColumnIndex >= 0) { 
                //try
                //{
                    if (f.dataGridView1.CurrentCell.ColumnIndex == 1)
                    {
                        // this.dataGridView1.BeginEdit(false);
                        f.dataGridView1.CurrentCell.ReadOnly = true;
                    }
                //}
                //catch { }

                if (f.backgroundWorker1.IsBusy)
                {
                    // MessageBox.Show("background asyc dissabled");
                    f.backgroundWorker1.CancelAsync(); //==First dissabling background worker

                }

                if (FlagForCntdBG_Update == 1)
                {
                    // MessageBox.Show("Entered");//--test
                    // chart1.Enabled = false;//should be dissabled 
                    //If the chart is in online mode then only do updating task
                    //--***********************Uncomment later********************//


                      //InitTimerForDevice(); //uncomment later

                    //--*************************end*****************************//
                }
                else
                {
                    // chart1.Enabled = true;
                }
            }//close of columnindex if
            else if (e.RowIndex >= 0 && (e.ColumnIndex >= 0 && e.ColumnIndex <= 1))
            {
                //we need to return and does not proceed forwared
                //select all 
                return;
            }

        }

        public void LockAndUnlock(Form_Main_PH_Application f)
        {
            //--We need to load and unload the image 
            if (FlagForLockUnlock == 0)
            {
                //Edit mode is dissabled //Edit OFF section
                //--Load image lock image
                // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);


                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string newPath = Path.GetFullPath(Path.Combine(imagePath, @"..\"));
                string file = newPath + @"Database\image\lock.png";
                // MessageBox.Show(file);

                Bitmap SOME = new Bitmap(Image.FromFile(file));

                Bitmap bp = new Bitmap(SOME, f.pb_lock_unlock.Width, f.pb_lock_unlock.Height);
                f.pb_lock_unlock.Image = bp;//Image.FromFile(file);

                //==Code form edit OFF
                FlagForCntdBG_Update = 1;//On currently
                FlagForDissableLeftAndRightClicksInChart = 1;//turn on 
                                                             //==End of edit OFF
                FlagForLockUnlock = 1;
                f.lb_realtimeshow.Text = "Real Time Mode";
            }
            else
            {
                //Edit mode is enable
                //--Load image lock image
                // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string newPath = Path.GetFullPath(Path.Combine(imagePath, @"..\"));
                string file = newPath + @"Database\image\unlock.png";

                Bitmap SOME = new Bitmap(Image.FromFile(file));
                Bitmap bp = new Bitmap(SOME, f.pb_lock_unlock.Width, f.pb_lock_unlock.Height);

                f.pb_lock_unlock.Image = bp;//Image.FromFile(file);
                //==============Edit ON section these flags make the Edit on=====//
                FlagForCntdBG_Update = 0;//OFF currently
                FlagForDissableLeftAndRightClicksInChart = 0;//==TURN OFF
                //==============End of edit on flag decleration =========//

                FlagForLockUnlock = 0;
                f.lb_realtimeshow.Text = "Edit Mode";
            }
        }

        public void InitTimerForDevice()
        {

            // MessageBox.Show("inside timer");
            flagForTimer = 1;
            atimer = new System.Timers.Timer();
            atimer.Enabled = true;
            atimer.Elapsed += timer1_Tick_For_Device;
            atimer.Interval = 1000 * 5; //x seconds[ 1000 ms * x  =  x seconds]

        }

        //public void timer1_Tick_For_Device(object sender, EventArgs e)
        public void timer1_Tick_For_Device(object sender, EventArgs e)
        {

            try
            {
                if (FlagForCntdBG_Update == 0)
                {
                    return;
                }
                else
                {
                    //var f = new Form_Main_PH_Application();
                    if (!frm1.backgroundWorker1.IsBusy)
                    {

                        frm1.backgroundWorker1.RunWorkerAsync();//--Running the worker async

                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        public void RefreshGraph(Chart chart1, Form_Main_PH_Application f)
        {

            f.Invalidate();
            chart1.Invalidate();
            //plot_new_graph(); //--This one needs to be reinvented

            //===Removing the node series value
            //foreach(var item in listNodeSeriesPlotted)
            //{
            //    // RemoveSeriesFromChart(chart1, item);
            //    RemovingSeriesPointFromChart(chart1, item);
            //}
            RemovingSeriesPointFromChart(chart1, series1.Name);//This contains the node points

            //--Remove the line series value plotted
            foreach (var item in listLineInfoValues)
            {
                RemoveSeriesFromChart(chart1, item.lineSeriesID.Name);//Removing the line series

            }

            //==Removing the dotteed series
            RemovingSeriesPointFromChart(chart1, addDottedSeries.Name);//--Removing the dotted series

            listLineInfoValues.Clear();
            listNodeInfoValues.Clear();
            index = 0;  //This is resetting the index values
            incrementIndex = 0;
            // ReloadComfortZoneForBackGroundWorker();

        }


        void RemoveSeriesFromChart(Chart chart1, string seriesName)
        {
            if (chart1.Series.IndexOf(seriesName) != -1)
            {
                //--This  means the series is present....
                chart1.Series.RemoveAt(chart1.Series.IndexOf(seriesName));
            }
        }

        void RemovingSeriesPointFromChart(Chart chart1, string seriesName)
        {
            if (chart1.Series.IndexOf(seriesName) != -1)
            {
                //--This  means the series is present....
                //chart1.Series.RemoveAt(chart1.Series.IndexOf(seriesName));
                chart1.Series[seriesName].Points.Clear();
            }
        }

        public void dataGridView1_DataBindingComplete(object sender, DataGridViewBindingCompleteEventArgs e, Form_Main_PH_Application f)
        {
            //MessageBox.Show("ok");
            f.dataGridView1.ClearSelection();
            //MessageBox.Show("Databinding");
            f.dataGridView1.SelectionChanged -= f.dataGridView1_SelectionChanged;

            // flagDataBindingCompleteForSelection = 1;
        }




        ///==============BuildingOperation.cs File end========================//

        //=====================Top ContextMenus related codes ===========================//

        public void printHeatMap(Chart chart1)
        {

            try
            {
                //this when click prints the chart.
                // Chart chart1 = form1.chart1;
                System.Drawing.Printing.PrintDocument pd = new System.Drawing.Printing.PrintDocument();
                chart1.Printing.PrintPaint(chart1.CreateGraphics(), chart1.DisplayRectangle);
                PrintDialog pdi = new PrintDialog();
                pdi.Document = pd;
                if (pdi.ShowDialog() == DialogResult.OK)
                    pdi.Document.Print();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        public void saveAsImageHeatMap(Chart chart1,Form_Main_PH_Application f1)
        {
            string fileName = "";
           f1.saveFD.InitialDirectory = "C:";
            f1.saveFD.FileName = "ChartImage";
            f1.saveFD.Filter = "PNG(.png) |*.png|Bitmap(.bmp) |*.bmp|JPEG |*.jpeg";
            ImageFormat format = ImageFormat.Png;
            if (f1.saveFD.ShowDialog() == DialogResult.OK)
            {
                fileName = f1.saveFD.FileName;
                string ext = System.IO.Path.GetExtension(f1.saveFD.FileName);
                switch (ext)
                {
                    case ".bmp":
                        format = ImageFormat.Bmp;
                        break;
                    case ".jpeg":
                        format = ImageFormat.Jpeg;
                        break;
                }

                chart1.SaveImage(fileName, format);
            }
            


        }//Close of save asd

        public void SaveConfiguration(Form_Main_PH_Application f1)
        {
            SQLiteSaveConfigurationSetting(f1);
        }

        public void LoadConfiguration(Form_Main_PH_Application f1,Chart chart1)
        {
            LoadDataFromFileToDBConfiguration();
            //Now lets refresh
            PullDataAndPlot(f1, chart1);
        }

        public void PullDataAndPlot(Form_Main_PH_Application f1,Chart chart1)
        {
            //--Refreshing the tables for all the datas..

           f1.dataGridView1.Rows.Clear();
            DataGridView_Show_Data(f1);
            f1.dataGridView1.Rows.Add();
            //If there is no rows in datagridview...
            //if(dataGridView1.Rows.Count < 1) { 

            RefreshGraph(chart1,f1);

            //after deletion lets select the its corresponding upper chart
            //Select the chart with was selected
            if (chartDetailList.Count > 0)
            {
                //We need to select the particular index 
                //--I hope this will do the replotting thing as well
                f1.dataGridView1.Rows[0].Selected = true;//The row is selected 

            }
        }

        public void ExcelExportFunction(object sender, EventArgs e,Form_Main_PH_Application f1)
        {

            //--Lets create a function which helps to export the data 

            try
            {
                ExcelExport(f1);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        public void ExcelExport(Form_Main_PH_Application f1)
        {
            /*
            2 steps : 
            1. pull the data 
            2. convert the data 
            3. place it in excel files

            */
            try
            {


                String fileName = "";

                f1.saveFD.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);// "C:";
                f1.saveFD.FileName = "ExcelSave";
                f1.saveFD.Title = "Save to excel file";//Properties.Resources.Save_Excel_file_to;
                f1.saveFD.Filter = "Excel file|*.xls";
                if (f1.saveFD.ShowDialog() == DialogResult.OK)
                {



                    if (listNodeInfoValues.Count <= 0)
                    {
                        MessageBox.Show("No data present to export");
                    }

                    if (chartDetailList.Count <= 0)
                    {
                        //Do not proceed futher 
                        MessageBox.Show("No data present to save!");
                        return;
                    }

                    f1.Cursor = Cursors.WaitCursor;
                    //excel is checked..
                    Excel.Application oApp;
                    Excel.Workbook oBook;
                    Excel.Worksheet oSheet;

                    oApp = new Excel.Application();
                    oBook = oApp.Workbooks.Add();
                    oSheet = (Excel.Worksheet)oBook.Worksheets.get_Item(1);


                    //printing the building information..
                    oSheet.Cells[1, 1] = "Node Information";//WFA_psychometric_chart.Properties.Resources.Building_Information;
                    oSheet.Cells[2, 1] = "Name";
                    oSheet.Cells[2, 2] = "Temperature Source";
                    oSheet.Cells[2, 3] = "Temperature";
                    oSheet.Cells[2, 4] = "Pressure Source";
                    oSheet.Cells[2, 5] = "Pressure";
                    oSheet.Cells[2, 6] = "Equivalent Enthalpy";
                    oSheet.Cells[2, 7] = "Color";
                    oSheet.Cells[2, 8] = "Node Size";// building_info[0].zip.ToString();  //zip doesnot exist now
                   // oSheet.Cells[2, 9] = "Air Flow";

                    int count = 3;

                    for (int i = 0; i < listNodeInfoValues.Count; i++)
                    {



                        //for station 
                        //--This function return humDataGridValue and enthalpyDataGridValue
                       // enthalpyHumidityCalculatorForXYvalue(menuStripNodeInfoValues[i].xVal, menuStripNodeInfoValues[i].yVal);

                        //double humValReturn  =  
                        //oSheet.Cells[count+i, 1] = "Node Information";//WFA_psychometric_chart.Properties.Resources.Building_Information;
                        oSheet.Cells[count + i, 1] = listNodeInfoValues[i].name;//dataGridView1.Rows[i].Cells[0].Value.ToString();
                        oSheet.Cells[count + i, 2] = listNodeInfoValues[i].temperature_source;//dataGridView1.Rows[i].Cells[1].Value.ToString();//"Temperature Source";
                        oSheet.Cells[count + i, 3] = ""; //listNodeInfoValues[i].xVal;//dataGridView1.Rows[i].Cells[3].Value.ToString();//"Temperature";

                        oSheet.Cells[count + i, 4] = listNodeInfoValues[i].pressure_source;//dataGridView1.Rows[i].Cells[4].Value.ToString();//"Humidity Source";
                        oSheet.Cells[count + i, 5] = listNodeInfoValues[i].yVal; //Math.Round(humDataGridValue, 2);//dataGridView1.Rows[i].Cells[5].Value.ToString(); //"Humidity";
                        oSheet.Cells[count + i, 6] = listNodeInfoValues[i].xVal;//Math.Round(enthalpyDataGridView, 2); //dataGridView1.Rows[i].Cells[6].Value.ToString(); //"Equivalent Enthalpy";
                                                                                          //--For color value
                                                                                          //DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)menuStripNodeInfoValues[i].colorValue;//dataGridView1.Rows[i].Cells[7];
                                                                                          //buttonCell.FlatStyle = FlatStyle.Popup;
                                                                                          //Color c = buttonCell.Style.BackColor;//= colorDialog1.Color;//System.Drawing.Color.Red;

                        oSheet.Cells[count + i, 7] = listNodeInfoValues[i].colorValue;//c.ToString();
                        oSheet.Cells[count + i, 8] = listNodeInfoValues[i].marker_Size;//dataGridView1.Rows[i].Cells[8].Value.ToString(); // building_info[0].zip.ToString();  //zip doesnot exist now
                        //oSheet.Cells[count + i, 9] = listNodeInfoValues[i].airFlow;// dataGridView1.Rows[i].Cells[9].Value.ToString(); ;

                    } //Close of for loop

                    int NewLineCount = count + f1.dataGridView1.RowCount + 2;


                    //now printing the value of historical datas..
                    oSheet.Cells[NewLineCount, 1] = "Line Information";

                    oSheet.Cells[NewLineCount + 1, 1] = "Line Name";
                    oSheet.Cells[NewLineCount + 1, 2] = "Start Node Name";
                    oSheet.Cells[NewLineCount + 1, 3] = "End Node Name";
                    oSheet.Cells[NewLineCount + 1, 4] = "Color";
                    oSheet.Cells[NewLineCount + 1, 5] = "Thickness";
                    oSheet.Cells[NewLineCount + 1, 6] = "Show Name";
                    //oSheet.Cells[NewLineCount + 1, 7] = "DBT1";
                    //oSheet.Cells[NewLineCount + 1, 8] = "RH1";
                    //oSheet.Cells[NewLineCount + 1, 9] = "HR1";
                    //oSheet.Cells[NewLineCount + 1, 10] = "SV1";

                    //oSheet.Cells[NewLineCount + 1, 11] = "MFR1";
                    //oSheet.Cells[NewLineCount + 1, 12] = "enthalpy1";
                    //oSheet.Cells[NewLineCount + 1, 13] = "TEF1";
                    //oSheet.Cells[NewLineCount + 1, 14] = "DBT2";
                    //oSheet.Cells[NewLineCount + 1, 15] = "RH2";
                    //oSheet.Cells[NewLineCount + 1, 16] = "HR2";
                    //oSheet.Cells[NewLineCount + 1, 17] = "SV2";
                    //oSheet.Cells[NewLineCount + 1, 18] = "MFR2";
                    //oSheet.Cells[NewLineCount + 1, 19] = "Enthalpy2";
                    //oSheet.Cells[NewLineCount + 1, 20] = "TEF2";
                    //oSheet.Cells[NewLineCount + 1, 21] = "heat change";




                    //now lets print the value in loop
                    for (int i1 = 0; i1 < listLineInfoValues.Count; i1++)
                    {




                        //===========================Copied here of the code=========================//


                        string idNode1 = "", lastUpdatedDateNode1 = "", humiditySourceNode1 = "", temperatureSourceNode1 = "", nameNode1 = "";
                        string idNode2 = "", lastUpdatedDateNode2 = "", humiditySourceNode2 = "", temperatureSourceNode2 = "", nameNode2 = "";
                        double xValueNode1 = 0, yValueNode1 = 0;
                        double xValueNode2 = 0, yValueNode2 = 0;
                        int airFlowNode1 = 0, airFlowNode2 = 0;

                        //--Scanning for the values

                        string startNodeName = "";
                        string endNodeName = "";
                        //Now lets calculate the startNodeName and endNodeName
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            if (listLineInfoValues[i1].prevNodeId == listNodeInfoValues[x].ID)
                            {
                                nameNode1 = startNodeName = listNodeInfoValues[x].name;
                                idNode1 = listNodeInfoValues[x].ID;
                                lastUpdatedDateNode1 = listNodeInfoValues[x].lastUpdatedDate;
                                humiditySourceNode1 = listNodeInfoValues[x].pressure_source;
                                temperatureSourceNode1 = listNodeInfoValues[x].temperature_source;
                                //nameNode1 =
                                xValueNode1 = listNodeInfoValues[x].xVal;
                                yValueNode1 = listNodeInfoValues[x].yVal;
                               // airFlowNode1 = (int)menuStripNodeInfoValues[x].airFlow;
                                break;
                            }

                        }

                        //--This one is for end nodename
                        for (int v = 0; v < listNodeInfoValues.Count; v++)
                        {
                            if (listLineInfoValues[i1].nextNodeId == listNodeInfoValues[v].ID)
                            {
                                nameNode2 = endNodeName = listNodeInfoValues[v].name;
                                idNode2 = listNodeInfoValues[v].ID;
                                lastUpdatedDateNode2 = listNodeInfoValues[v].lastUpdatedDate;
                                humiditySourceNode2 = listNodeInfoValues[v].pressure_source;
                                temperatureSourceNode2 = listNodeInfoValues[v].temperature_source;
                                //nameNode1 =
                                xValueNode2 = listNodeInfoValues[v].xVal;
                                yValueNode2 = listNodeInfoValues[v].yVal;
                                //airFlowNode2 = (int)menuStripNodeInfoValues[v].airFlow;
                                break;
                            }

                        }

                        //--Lets make a function which returns all the other values
                        //EditNodeLineForm ed_form = new EditNodeLineForm(this);
                       // ed_form.EnergyParameterCalculationForTwoNodes(xValueNode1, yValueNode1, airFlowNode1, xValueNode2, yValueNode2, airFlowNode2);

                        //===========================End : Copied here of the code=========================//


                        oSheet.Cells[NewLineCount + 2 + i1, 1] = listLineInfoValues[i1].name;//dataGridView2.Rows[i].Cells[1].Value.ToString();
                        oSheet.Cells[NewLineCount + 2 + i1, 2] = //dataGridView2.Rows[i].Cells[2].Value.ToString();//"Start Node Name";

                        oSheet.Cells[NewLineCount + 2 + i1, 3] = startNodeName;//dataGridView2.Rows[i].Cells[3].Value.ToString();//"End Node Name";

                        oSheet.Cells[NewLineCount + 2 + i1, 4] = listLineInfoValues[i1].lineColorValue;// col.ToString();
                        oSheet.Cells[NewLineCount + 2 + i1, 5] = listLineInfoValues[i1].lineThickness;//dataGridView2.Rows[i].Cells[5].Value.ToString(); //"Thickness";
                                                                                                               //DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[i].Cells[9];

                        oSheet.Cells[NewLineCount + 2 + i1, 6] = listLineInfoValues[i1].status;//status; //"Show Name";
                        //oSheet.Cells[NewLineCount + 2 + i1, 7] = xValueNode1; //dataGridView2.Rows[i].Cells[10].Value.ToString(); //"DBT1";
                        //oSheet.Cells[NewLineCount + 2 + i1, 8] = ed_form.relativeHumidity1;//dataGridView2.Rows[i].Cells[11].Value.ToString();//"RH1";
                        //oSheet.Cells[NewLineCount + 2 + i1, 9] = yValueNode1;//dataGridView2.Rows[i].Cells[12].Value.ToString(); //"HR1";
                        //oSheet.Cells[NewLineCount + 2 + i1, 10] = ed_form.spVol1; //dataGridView2.Rows[i].Cells[13].Value.ToString(); //"SV1";


                        //oSheet.Cells[NewLineCount + 2 + i1, 11] = ed_form.massFlowRate1;//dataGridView2.Rows[i].Cells[14].Value.ToString(); //"MFR1";
                        //oSheet.Cells[NewLineCount + 2 + i1, 12] = ed_form.enthalpy1;//dataGridView2.Rows[i].Cells[15].Value.ToString(); //"enthalpy1";

                        //oSheet.Cells[NewLineCount + 2 + i1, 13] = ed_form.totalEnergyFlow1;// dataGridView2.Rows[i].Cells[16].Value.ToString(); //"TEF1";
                        //oSheet.Cells[NewLineCount + 2 + i1, 14] = xValueNode2;//dataGridView2.Rows[i].Cells[17].Value.ToString(); //"DBT2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 15] = ed_form.relativeHumidity2;//dataGridView2.Rows[i].Cells[18].Value.ToString();// "RH2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 16] = yValueNode2;//dataGridView2.Rows[i].Cells[19].Value.ToString(); //"HR2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 17] = ed_form.spVol2;//dataGridView2.Rows[i].Cells[20].Value.ToString(); //"SV2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 18] = ed_form.massFlowRate2;//  dataGridView2.Rows[i].Cells[21].Value.ToString(); //"MFR2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 19] = ed_form.enthalpy2; //dataGridView2.Rows[i].Cells[22].Value.ToString(); //"Enthalpy2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 20] = ed_form.totalEnergyFlow2;// dataGridView2.Rows[i].Cells[23].Value.ToString();// "TEF2";
                        //oSheet.Cells[NewLineCount + 2 + i1, 21] = ed_form.heatChangeForBoth;//dataGridView2.Rows[i].Cells[24].Value.ToString(); //"heat change";


                    }

                    //now lets open the save dialog box and the save it there..

                    f1.Cursor = Cursors.Default;
                    //save the file..
                    fileName = f1.saveFD.FileName;
                    oBook.SaveAs(fileName);


                    oBook.Close();
                    oApp.Quit();
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        //====================End  Top ContextMenus related codes ===========================//


        //====================Chart and template operations=================================//

        public void DeleteIndividualChart(object sender, EventArgs e,Form_Main_PH_Application f1,Chart chart1)
        {
            //  DialogResult dialogResult = MessageBox.Show( "Are you sure you want to delete this chart?", "Delete chart", MessageBoxButtons.YesNo,MessageBoxIcon.Exclamation);

            if (MessageBox.Show("Are you sure you want to delete this chart?", "Delete chart", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
            //if (dialogResult == DialogResult.Yes)
            {
                //--Delete function 
                //--This is the delete operation for the handler.....
                /*
                Steps: 1. delete the row  of the table using the id portion.
                2. Delete the corresponding tables related to the row.             
                */

              


                //===New added code==========================================//
                int selectedItemIndex = f1.dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

                string chartID = chartDetailList[selectedItemIndex].chartID;

                DisableChart(chartID);
                //===END of NEW ADDED CODE===========================//



                f1.dataGridView1.Rows.Clear();//--This one is for clearing the data
                f1.dataGridView1.Refresh();//--Release the previously selectecd items
                f1.dataGridView1.Rows.Add();
                //   MessageBox.Show("Reached before refreshGraph() and after datagv ref");
                DataGridView_Show_Data(f1);

                //If there is no rows in datagridview...
                //if(dataGridView1.Rows.Count < 1) { 



                RefreshGraph(chart1,f1);

                //after deletion lets select the its corresponding upper chart
                //Select the chart with was selected
                if (chartDetailList.Count > 0)
                {
                    //We need to select the particular index 
                    //--I hope this will do the replotting thing as well
                    if (f1.dataGridView1.Rows[0].Cells[1].Value != null)
                    {
                        f1.dataGridView1.Rows[0].Cells[1].Selected = true;//The row is selected 
                    }

                }


                if (chartDetailList.Count > 0)
                {
                    //if (dataGridView1.CurrentCell.RowIndex >= 1)
                    //{
                    //    //set parameters of your event args
                    //    var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex-1);
                    //    // or setting the selected cells manually before executing the function
                    //    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex - 1].Cells[1].Selected = true;
                    //    dataGridView1_CellClick(sender, eventArgs);
                    //} else
                    //{ 
                    //set parameters of your event args


                    var eventArgs = new DataGridViewCellEventArgs(1, 0);
                    // or setting the selected cells manually before executing the function
                    f1.dataGridView1.Rows[0].Cells[1].Selected = true;
                    f1.dataGridView1_CellClick(sender, eventArgs);



                    // }
                }


                //dataGridView2.Rows.Clear();
                // MessageBox.Show("Chart deleted successfully !");
            }


        }

        public void SaveAsTemplate(object sender, EventArgs e,Form_Main_PH_Application f1)
        {

            /*
             This functions helps in saving the chart as template             
             */

            try
            {

                int selectedItemIndex = f1.dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());
                string chart_ID = chartDetailList[selectedItemIndex].chartID;


                SaveChartAsTemplate(chart_ID,f1);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        public void LoadTemplate(object sender, EventArgs e,Form_Main_PH_Application f1,Chart chart1)
        {

            /*
             This function helps to load the template file save earlier 
             */

            try
            {
                int selectedItemIndex = f1.dataGridView1.CurrentCell.RowIndex;
                if (selectedItemIndex < chartDetailList.Count)
                {
                    //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());
                    string chart_ID = chartDetailList[selectedItemIndex].chartID;
                    string chart_Name = chartDetailList[selectedItemIndex].chartName;


                    LoadDataFromTemplateFileToDBActionTemplateFileUpload(chart_ID, chart_Name);
                    PullDataAndPlot(f1, chart1);//Refreshing the data
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }


        //====================CHart and template operations===============================//


   


    }
}
