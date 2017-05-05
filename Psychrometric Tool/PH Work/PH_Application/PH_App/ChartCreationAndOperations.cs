using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Reflection;
using System.IO;

namespace PH_App
{
   public class ChartCreationAndOperations :Form_Main_PH_Application
    {
        /// <summary>
        /// These are the properties of the chart asixes
        /// </summary>
        public double xAxisMinimum = 0;
        public double xAxisMaximum = 4000;
        public double yAxisMinimum = 1 / 1000;
        public double yAxisMaximum = 100;

        //dotted series when the mouese is moved
        Series addDottedSeries = new Series("newdottedSeries");


        //--Mouse pointer location Info
        double xCoordinateValue = 0;
        double yCoordinateValue = 0;
        Point? prevPosition = null;

        //variables
       public double xAxis1;
       public double yAxis1;
       public int load = 0;//false
       string idOfNodeSelected;//--initially it represents nothing...
       int indexOfLineInTheList = 0; //This holds the id of the line selected.
       string idSelected;
       int FlagForNodeDelete = 0;//0 means node not selected 1 means selected
       string nodeID_ForDeletingNode = "";
       int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..
       int mouseClickAction = 0;
        int incrementIndex = 0;//--Defining the index


        //Current pointer location
        public double currentXAxis = 0.000;
       public double currentYAxis = 0.000;

        //--Flags decleration over here
        int FlagForDisconnectingLineChoice = 0;// 0 means off and 1 means on
        int FlagForDissableLeftAndRightClicksInChart = 0;//==do not dissable /OFF
        int flagForDisconnectClick = 0;//0 means false it is used to see if the disconnect option is clicked or not.
        int flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 0;//-- 0 means off 1 means on for previous node    and 2 means next node
        int flagNodeSelectedForConnect = 0;
        int indexForSeriesNodePoint = 0;

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
         
        public void PlotPHChart(string fluidName,Chart phChart)
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
            ///*
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


            // */
            //=================================End regregration cycle=====================//

        }

        private void Chart_MouseDown(object sender, MouseEventArgs e)
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

                //contextMenuStrip1.Show(MousePosition);//--This is dissabled



                //==Now lets reset the values first,displaying the previous and next node ====//

                if (FlagForDisconnectingLineChoice == 1)
                {
                    //--We need to show the text and weather he wants to delete form first or seconds
                    //First lets enable and change the Text
                    disconnectLineFromAToolStripMenuItem.Enabled = true;
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
                    
                    foreach (var node in dom.listNodeInfoValues)
                    {
                        if (node.ID == dom.indexOfNextNodeForLineMovement)
                        {
                            //--Next noe is identified 
                            nextNodeIDTemp = node.name;//--Store the name of the thing
                        }
                        else if (node.ID == dom.indexOfPrevPointForLineMovement)
                        {
                            //--Previous node is identified 
                            prevNodeIDTemp = node.name;//--Stroing the name of the node
                        }

                    }

                    //--we need lineID
                    //   disconnectLineFromAToolStripMenuItem.Text = "Disconnect line from " + prevNodeIDTemp;
                    // disconnectLineFromBToolStripMenuItem.Text = "Disconnect line from " + nextNodeIDTemp;

                    nodeAToolStripMenuItem.Text = "Node Name: " + prevNodeIDTemp;
                    nodeBToolStripMenuItem.Text = "Node Name: " + nextNodeIDTemp;



                }
                else
                {
                    //--We need to turn off the click option
                    disconnectLineFromAToolStripMenuItem.Enabled = false;
                    // disconnectLineFromAToolStripMenuItem.Visible = false;
                    //disconnectLineFromBToolStripMenuItem.Enabled = false;

                    //--lets dissable the line
                    nodeAToolStripMenuItem.Enabled = false;
                    nodeBToolStripMenuItem.Enabled = false;
                   // addMixNodeToolStripMenuItem.Enabled = false;//For dissabling the add mix node
                                                                //--Lets make it invisible
                                                                //addMixNodeToolStripMenuItem.Visible = false;

                    //--Lets dissbale
                    //nodeAToolStripMenuItem.Enabled = false;
                    //nodeBToolStripMenuItem.Enabled = false;

                }

                //==this one is for delete node to hide
                if (deleteNodeToolStripMenuItem.Enabled == true)
                {
                    deleteNodeToolStripMenuItem.Visible = true;
                }
                else
                {
                    deleteNodeToolStripMenuItem.Visible = false;
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
                if (nodeAToolStripMenuItem.Enabled == false)
                {
                    //disconnecting line invisible
                    disconnectLineFromAToolStripMenuItem.Visible = false;
                }
                else
                {

                    //visible
                    disconnectLineFromAToolStripMenuItem.Visible = true;
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
                this.quickNodeInsertToolStripMenuItem.Image = null;
                if (FlagForDissableLeftAndRightClicksInChart == 1)
                {
                    //FlagForDissableLeftAndRightClicksInChart = 0;
                    CMSinsertNode.Enabled = false;
                    //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                    //string NewDirectory = Path.GetFullPath(Path.Combine(dir, @"..\"));
                    //string file = NewDirectory + @"Database\image\lock.png";

                    //string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    //string file = imagePath + @"\image\lock.png";
                    string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = imagePath + @"\..\Database\image\lock.png";


                    this.quickNodeInsertToolStripMenuItem.Image = Bitmap.FromFile(file);
                    this.quickNodeInsertToolStripMenuItem.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
                    // this.quickNodeInsertToolStripMenuItem.TextAlign = System.Drawing.ContentAlignment.TopCenter;
                    //quickNodeInsertToolStripMenuItem.ImageAlign = 
                    this.quickNodeInsertToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.ImageAndText;

                    CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer

                    //return;//DO not proceed forward
                }
                else
                {
                    //==This shows the contextmenustrip on right click
                    CMSinsertNode.Enabled = true;
                    CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                }

            }

        }

        private void chart1_MouseMove(object sender, MouseEventArgs e,Chart chart1)
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
                    var yVal = result.ChartArea.AxisY.PixelPositionToValue(pos.Y);

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
                        lb_dbt.Text = Math.Round(xVal, 2).ToString();
                        lb_humidity_ratio.Text = Math.Round(yVal, 2).ToString();

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

                        // lb_test1.Text = " Humidity = " + phi;

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
                    //--Creating temporary line..
                    //--then redraw it again...
                    addTemporarySeries(addDottedSeries);
                    //--Now lets move on the showing the hand when hover over the Node lets do it bro...
                    addCursorFunctionForLineDisconnectConnect(e,phChart);
                    // lb_where.Text = "me : discon =1";

                }
                else
                {
                    // lb_where.Text = "me : else line detect on";
                    disconnectLineToolStripMenuItem.Enabled = false;

                    //--This one is for two side of line disconnection -------//
                    disconnectLineFromAToolStripMenuItem.Enabled = false;
                    //disconnectLineFromBToolStripMenuItem.Enabled = false;

                    nodeAToolStripMenuItem.Enabled = false;
                    nodeBToolStripMenuItem.Enabled = false;
                   // addMixNodeToolStripMenuItem.Enabled = false;//Dissable add mix node
                                                                //---End of two side of line disconnection section

                    //--This is for the weather the line is moverover or not...
                    LineDetectOnMouseMove(e,phChart);

                    //--Lets add a function for the process diagram drawing..

                    ProcessDiagramMouseMoveFunction(e,phChart);//--This does the adding and removing part             
                }

           // }//close of else

        }//close of the main private void...

        /// <summary>
        /// add a dotted series when the mouse is moved but not we need to add to
        /// chart first in order to get the desired effect
        /// </summary>
        /// <param name="addDottedSeries"></param>
        public void addTemporarySeries(Series addDottedSeries)
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
            for (int i = 0; i < dom.listNodeInfoValues.Count; i++)
            {
                //1 means prev node is selected 
                if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 2) //This is done because we are detaching from node1
                {
                    if (dom.listNodeInfoValues[i].ID == dom.indexOfPrevPointForLineMovement)//indexOfPrevPointForLineMovement variable is in NodeAndLineClass 
                    {
                        //This is the node of previous point we need to find the x and y coordinate of
                        xAxisValue = dom.listNodeInfoValues[i].xVal;
                        yAxisvalue = dom.listNodeInfoValues[i].yVal;
                        break;//If value found no need to search all exit of loop
                    }

                } //2 means next node is selected 0 means off
                else if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 1)//--This is done we are detaching form node2
                {
                    if (dom.listNodeInfoValues[i].ID == dom.indexOfNextNodeForLineMovement)
                    {
                        //This is the node of previous point we need to find the x and y coordinate of
                        xAxisValue = dom.listNodeInfoValues[i].xVal;
                        yAxisvalue = dom.listNodeInfoValues[i].yVal;
                        break;//If value found no need to search all exit of loop
                    }
                }
            }
            //addDottedSeries.Points.AddXY(menuStripNodeInfoValues[indexOfPrevPointForLineMovement].xVal, menuStripNodeInfoValues[indexOfPrevPointForLineMovement].yVal);
            addDottedSeries.Points.AddXY(xAxisValue, yAxisvalue);//--this is the new changed code:bbk305
            addDottedSeries.Points.AddXY(currentXAxis, currentYAxis);

        }

        public void addCursorFunctionForLineDisconnectConnect(MouseEventArgs e,Chart chart1)
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
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);

                    xAxis1 = xValue;
                    yAxis1 = yValue;
                    if ((xAxis1 >= 0 && xAxis1 <= 50) && (yAxis1 >= 0 && yAxis1 <= 30))
                    {
                        //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                        if (dom.listNodeInfoValues.Count > 0)
                        {
                            //foreach(var values in menuStripNodeInfoValues)

                            for (int i = 0; i < dom.listNodeInfoValues.Count; i++)
                            {

                                if ((xValue > dom.listNodeInfoValues[i].xVal - 0.25 && xValue < dom.listNodeInfoValues[i].xVal + 0.25) && (yValue > dom.listNodeInfoValues[i].yVal - 0.25 && yValue < dom.listNodeInfoValues[i].yVal + 0.25))
                                {

                                    idOfNodeSelected = dom.listNodeInfoValues[i].ID;
                                    if (Cursor == Cursors.Cross)
                                    {
                                        Cursor = Cursors.Hand;
                                    }

                                    //--Whenever this occurs lets move on to attaching the the node or say refreshing and replotting....
                                    //--For this as well lets rise a flag..
                                    flagNodeSelectedForConnect = 1;
                                    break;//this break is for if found the value no longer loop increases the perfomances..
                                }
                                else
                                {
                                    if (Cursor != Cursors.Cross)
                                    {
                                        this.Cursor = Cursors.Cross;
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
        private void LineDetectOnMouseMove(MouseEventArgs e,Chart chart1)
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

                if (dom.listLineInfoValues.Count > 0)
                {
                    for (int i = 0; i < dom.listLineInfoValues.Count; i++)
                    {

                        if (hit.Series.Name != null)
                        {
                            if ((string)hit.Series.Name == (string)dom.listLineInfoValues[i].lineSeriesID.Name)
                            {
                                //--lets store previous color first
                                storeColor = dom.listLineInfoValues[i].lineColorValue;
                                flagForColor = 1;
                                tempSeries = hit.Series;

                                //  idOfLineSelecteForDisconnect = menuStripNodeLineInfoValues[i].ID;
                                indexOfLineInTheList = i; // updating the index 

                                //--These two variables are in NodeAndLineClass.cs files
                                //--Logging the index so that it could be used for futher processing later...
                                dom.indexOfPrevPointForLineMovement = dom.listLineInfoValues[i].prevNodeId;//This gets the previous node id value...

                                dom.indexOfNextNodeForLineMovement = dom.listLineInfoValues[i].nextNodeId;//==This gets the next node id

                                hit.Series.Color = Color.Black;
                                disconnectLineToolStripMenuItem.Enabled = true;
                                // lb_test.Text = Text;

                                //---This code is for detecting form A and form B point in line detachment---//

                                /*
                                Steps : here we do following task
                                1. Enable both the click option for now just enable.
                                2. We have node info in this->indexOfPrevPointForLineMovement and  this->indexOfNextNodeForLineMovement
                                */
                                disconnectLineFromAToolStripMenuItem.Enabled = true;
                                //disconnectLineFromBToolStripMenuItem.Enabled = true;

                                nodeAToolStripMenuItem.Enabled = true;
                                nodeBToolStripMenuItem.Enabled = true;
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
        int tempIndexForNode = 0;
        private void ProcessDiagramMouseMoveFunction(MouseEventArgs e,Chart chart1)
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
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
                    if ((xValue >= 0 && xValue <= 50) && (yValue >= 0 && yValue <= 30))
                    {

                        xAxis1 = xValue;
                        yAxis1 = yValue;
                        //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                        if (dom.listNodeInfoValues.Count > 0)
                        {
                            //foreach(var values in menuStripNodeInfoValues)

                            for (int i = 0; i < dom.listNodeInfoValues.Count; i++)
                            {

                                if (dom.listNodeInfoValues[i].temperature_source != "Mix")
                                {

                                    if ((xValue > dom.listNodeInfoValues[i].xVal - 0.25 && xValue < dom.listNodeInfoValues[i].xVal + 0.25) && (yValue > dom.listNodeInfoValues[i].yVal - 0.25 && yValue < dom.listNodeInfoValues[i].yVal + 0.25))
                                    {

                                        //--This is changed from int to string  code bbk305
                                        idSelected = dom.listNodeInfoValues[i].ID; //Now this is a string 
                                        tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                        if (Cursor != Cursors.Cross)
                                        {
                                            Cursor = Cursors.Hand;
                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 1;//flag is ready on Node selected
                                            nodeID_ForDeletingNode = idSelected;
                                            deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
                                                                                        //=============end of flag for deleting===========//
                                        }
                                        //this.Cursor = Cursors.Hand;
                                        //now this works so lets move forward.
                                        readyForMouseClick = 1;//enable on click event


                                        break;//this break is for if found the value no longer loop increases the perfomances..
                                    }
                                    else
                                    {
                                        if (Cursor != Cursors.Cross)
                                        {
                                            this.Cursor = Cursors.Arrow;
                                            readyForMouseClick = 0;//dissable on click event.

                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                            deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
                                                                                        //nodeID_ForDeletingNode = idSelected;
                                                                                        //=============end of flag for deleting===========//

                                        }

                                    }
                                    //--Lets filter out the mix nodes---

                                }//Close of if section Mix section

                                //==This one is special case for mix node only
                                else if (dom.listNodeInfoValues[i].temperature_source == "Mix")
                                {
                                    if ((xValue > dom.listNodeInfoValues[i].xVal - 0.25 && xValue < dom.listNodeInfoValues[i].xVal + 0.25) && (yValue > dom.listNodeInfoValues[i].yVal - 0.25 && yValue < dom.listNodeInfoValues[i].yVal + 0.25))
                                    {

                                        //--This is changed from int to string  code bbk305
                                        idSelected = dom.listNodeInfoValues[i].ID; //Now this is a string 
                                        tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                        if (Cursor != Cursors.Cross)
                                        {
                                            Cursor = Cursors.Hand;
                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 1;//flag is ready on Node selected
                                            nodeID_ForDeletingNode = idSelected;
                                            deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
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
                                        if (Cursor != Cursors.Cross)
                                        {
                                            this.Cursor = Cursors.Arrow;
                                            /// readyForMouseClick = 0;//dissable on click event.

                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                            deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
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
                                dom.listNodeInfoValues[tempIndexForNode].yVal = yAxis1;//This value is changed...


                                //====================For mix node movement = ===============//

                                //UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//

                                // label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                dom.series1.Points.Clear();
                                for (int i = 0; i < dom.listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    // chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);//--removing line series that joins node..
                                    dom.listLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < dom.listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = dom.listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}

                                    //--this is changed as well code :bbk305
                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    dom.ReDrawPoints(dom.series1, dom.listNodeInfoValues[x].xVal, dom.listNodeInfoValues[x].yVal, dom.listNodeInfoValues[x].colorValue, dom.listNodeInfoValues[x].temperature_source, dom.listNodeInfoValues[x].pressure_source, dom.listNodeInfoValues[x].name, labelValue, dom.listNodeInfoValues[x].marker_Size);
                                    //Updating values in database
                                    if (bo.flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //   UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;//Incrementing the index values
                                                              //--Every time it redraws the point we need to update to database the node values
                                }
                                //--resetting incrementIndex
                                //incrementIndex = 0;
                                if (dom.listLineInfoValues.Count > 0)
                                {


                                    for (int x = 0; x < dom.listLineInfoValues.Count; x++)
                                    {
                                        // incrementIndex++;

                                        //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                        dom.ReDrawLines(dom.listLineInfoValues[x].ID, dom.listLineInfoValues[x].prevNodeId, dom.listLineInfoValues[x].nextNodeId, dom.listLineInfoValues[x].lineSeriesID, dom.listLineInfoValues[x].lineColorValue, dom.listLineInfoValues[x].lineThickness, dom.listLineInfoValues[x].name, dom.listLineInfoValues[x].status);

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
                                dom.listNodeInfoValues[tempIndexForNode].xVal = xAxis1;//--This value is just changed 
                                                                                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                                //====================For mix node movement = ===============//

                                //UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//
                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                dom.series1.Points.Clear();
                                for (int i = 0; i < dom.listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    dom.listLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < dom.listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = dom.listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    //dom.ReDrawPoints(dom.series1, dom.listNodeInfoValues[x].xVal, dom.listNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    dom.ReDrawPoints(dom.series1, dom.listNodeInfoValues[x].xVal, dom.listNodeInfoValues[x].yVal, dom.listNodeInfoValues[x].colorValue, dom.listNodeInfoValues[x].temperature_source, dom.listNodeInfoValues[x].pressure_source, dom.listNodeInfoValues[x].name, labelValue, dom.listNodeInfoValues[x].marker_Size);

                                    //Updating values in database...
                                    if (bo.flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }

                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                incrementIndex = 0;
                                if (dom.listLineInfoValues.Count > 0)
                                {
                                    for (int x = 0; x < dom.listLineInfoValues.Count; x++)
                                    {
                                        incrementIndex++;

                                        // dom.ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                        dom.ReDrawLines(dom.listLineInfoValues[x].ID, dom.listLineInfoValues[x].prevNodeId, dom.listLineInfoValues[x].nextNodeId, dom.listLineInfoValues[x].lineSeriesID, dom.listLineInfoValues[x].lineColorValue, dom.listLineInfoValues[x].lineThickness, dom.listLineInfoValues[x].name, dom.listLineInfoValues[x].status);
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

                                dom.listNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                                dom.listNodeInfoValues[tempIndexForNode].yVal = yAxis1;


                                //***********************For mix node section functions******************//

                                //Function helps in updating the mix point x and y value when other node are moved
                               // UpdateMixPointOnNodeMovement();
                                //************************End of the mix node ************************//
                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                dom.series1.Points.Clear();
                                for (int i = 0; i < dom.listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    dom.listLineInfoValues[i].lineSeriesID.Points.Clear();
                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < dom.listNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                    labelValue = dom.listNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    dom.ReDrawPoints(dom.series1, dom.listNodeInfoValues[x].xVal, dom.listNodeInfoValues[x].yVal, dom.listNodeInfoValues[x].colorValue, dom.listNodeInfoValues[x].temperature_source, dom.listNodeInfoValues[x].pressure_source, dom.listNodeInfoValues[x].name, labelValue, dom.listNodeInfoValues[x].marker_Size);
                                    //Updating values in database...
                                    if (bo.flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                // incrementIndex = 0;
                                if (dom.listLineInfoValues.Count > 0)
                                {
                                    // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");

                                    for (int x = 0; x < dom.listLineInfoValues.Count; x++)
                                    {
                                        // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");
                                        // incrementIndex++;

                                        //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                        // ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);
                                        dom.ReDrawLines(dom.listLineInfoValues[x].ID, dom.listLineInfoValues[x].prevNodeId, dom.listLineInfoValues[x].nextNodeId, dom.listLineInfoValues[x].lineSeriesID, dom.listLineInfoValues[x].lineColorValue, dom.listLineInfoValues[x].lineThickness, dom.listLineInfoValues[x].name, dom.listLineInfoValues[x].status);                                    }
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



    }
}
