using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    class ChartCreation
    {
       
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
   
    }
}
