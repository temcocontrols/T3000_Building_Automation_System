using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;


namespace WFA_PH_CurveDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {


        }

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

        public void PlotPHChart(string fluidName)
        {

            /*
             Lets do it step by step as the way that has been done in the Matlab

             */
            //--1. press_rng = logspace(-2,2,300); % [MPa] pressure (p) range

            PH_ChartClass PH = new PH_ChartClass();
            double[] press_rng = PH.LogSpace(-2, 2, 300,true,10).ToArray();
            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s = null;       
            for (int i = 0; i < press_rng.Length; i++)
            {
                s += press_rng[i].ToString() + "\t \t";
            }
            //MessageBox.Show("press_rng values = \n" + s);


            //--For line spacing values
            //2. temp_rng = 273.15+linspace(1,800,300); 
            double[] temp_rng =  PH.LinSpace(1, 800, 300, true).ToArray();
            

            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s1 = null;
            for (int i = 0; i < temp_rng.Length; i++)
            {
                temp_rng[i] += 273.15;
                s1 += temp_rng[i].ToString() + "\t \t";
            }
            // MessageBox.Show("temp_rng values = \n" + s1);



            //--Now for meshgrid we have created truple

            //MeshGrid_Class ms1 = new MeshGrid_Class();
            //3.[p,T] = meshgrid(press_rng,temp_rng); % [MPa,K] mesh p & T

            MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            Tuple<double[,], double[,]> tuple_x = MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            double[,] p = tuple_x.Item1;
            double[,] T = tuple_x.Item2;


            // MessageBox.Show("Tuple =\n" + tuple_x.Item1.Length);
            // MessageBox.Show("Tuple =\n" + p[1,1]);
            //  MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            // MessageBox.Show("Check row = " + p.GetLength(0) + ",column = " + p.GetLength(1)+"pvalue check= "+p[0,0]+", last = "+p[299,299]);

            //--8 pcrit = 22.064; % [MPa] critical pressure
            double pcrit = CoolProp.Props1SI("Water", "Pcrit")/1000000;//To convert to Pa form MPa //22.064;//CoolProp.Props1SI( "Water", "Pcrit");//PropsSI("Pcrit", "", 0, "", 0, "Water");//PH.IAPWS_IF97_TowParameterEquivalentFxn("Pcrit", "", 0, "",0, fluidName); //CoolProp.Props1SI(fluidName, "Pcrit");// CP.PropsSI("Tcrit","",0,"",0,"Water")
            //MessageBox.Show("Pcrit = " + pcrit);


            //double enthalpyone = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", (100 * 1000000), "T", 1073.2, fluidName);/// 1000000;
            //MessageBox.Show("Value enthalpy = " + enthalpyone);

           
            //--Now lets do the calculation for others 
            //4.h = IAPWS_IF97('h_pT',p,T); % [kJ/kg] enthalpy = f(p,T)
            P_Value = p;//Copy
            T_Value = T;//Copy


            double[,] h = new double[p.GetLength(0), p.GetLength(1)];

            for(int i = 0; i < h.GetLength(0); i++)
            {
                for(int j = 0; j < h.GetLength(1); j++)
                {
                    //if (p[i, j] <= pcrit) // 22.064 for water
                    //{ 
                    //--Now calling is done for the enthalpy function
                    double enthalpy = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", p[i, j]*1000000, "T", T[i, j], fluidName)/1000; //--This multiply is done to convert MPa to Pa and enthlapy is divided to convert J/kg to kJ/Kg
                    h[i, j] = enthalpy;
                    //}

                }
            }
            H_Value = h;//Copy



            string pTest1 = null;
            string hum = null;
            for (int i = 0; i < 10; i++)//Row length 
            {
                // pTest += item + "\t";
                for (int j = 0; j < 10; j++)//column length 
                {
                    pTest1 = "p=" + p[j, i].ToString() + ",T= " + T[j, i] +"H= "+h[j,i]+ "\t";
                    hum += pTest1;
                }
            }

           // MessageBox.Show("Value= " + hum);


            //--5 psat = IAPWS_IF97('psat_T',temp_rng); % [MPa] saturation pressure
            // 6. also trimming is done in this section
            List<double> psatList = new List<double>();
            psatList.Clear();

            //for (int i = 0; i < T.GetLength(0); i++)
            //{
            //    for (int j = 0; j < T.GetLength(1); j++)
            //    {
            for(int i = 0; i < temp_rng.Length; i++) { 
                   if((temp_rng[i]>= 273.06)&& ((temp_rng[i] <= 647.096)))
                    { 
                        //--Now calling is done for the enthalpy function
                        double psat = PH.IAPWS_IF97_TowParameterEquivalentFxn("P", "T", temp_rng[i], "Q", 0, fluidName)/1000000;                   
                        psatList.Add(psat);
                    } //Close of if statement
            }//Close of for
            //    }
            //}


            //-- 6. psat = psat(~isnan(psat)); 
            //--Use dynamic list to trim data which are empty rather than array
            
            //MessageBox.Show("Check,psat count="+ psatList.Count);
            //  MessageBox.Show("return val for pres="+ 274.15   + ",q=0: " + CoolProp.PropsSI("H", "T", 274.15, "Q", 0, fluidName));

            //--6 . hLsat = IAPWS_IF97('hL_p',psat); % [kJ/kg] saturated liquid enthalpy
            List<double> hLsatList = new List<double>();
            hLsatList.Clear();
            string TEXT = null;
            try { 
            foreach(var item in psatList)
            {
                  //TEXT = "i ind = " + item.i_index + ", j_ind = " + item.j_index + ", val= " + item.Value;
                    double hLsat = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", item*1000000, "Q", 0, fluidName)/1000;//Q=0 for liquid
                hLsatList.Add(hLsat);

                  
            }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "text = " + TEXT);
            }

            //--7 . hVsat = IAPWS_IF97('hV_p',psat); % [kJ/kg] saturated vapor enthalpy
            List<double> hVsatList = new List<double>();
            hVsatList.Clear();
            foreach (var item in psatList)
            {
                double hVsat = CoolProp.PropsSI("H", "P", item*1000000, "Q", 1, fluidName)/1000;      // PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", item, "Q", 1.0, fluidName);//Zero for vapor
                hVsatList.Add(hVsat);

            }

          //  MessageBox.Show("check ok");


            //--8. already declared

            //--9 hLcrit = IAPWS_IF97('hL_p',pcrit);
            double hLcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit*1000000, "Q", 0, fluidName)/1000;//Zero for liquid

            //--10 . hVcrit = IAPWS_IF97('hV_p',pcrit);
            double hVcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit * 1000000, "Q", 1, fluidName)/1000;//Zero for liquid
            //--11. Tcrit = IAPWS_IF97('Tsat_p',pcrit);
            double Tcrit = CoolProp.PropsSI("Tcrit", "", 0, "", 0, "Water");//PH.IAPWS_IF97_TowParameterEquivalentFxn("Tcrit", "", 0, "", 0, fluidName);//CoolProp.Props1SI(fluidName, "Tcrit");
            //--12. hcrit = IAPWS_IF97('h_pT',pcrit,Tcrit);
            double hcrit = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pcrit * 1000000, "T", Tcrit, fluidName)/1000;//Zero for liquid
                                                                                                           //--13.hVL = hVsat - hLsat; % [kJ/kg] heat of vaporization
            List<double> hVL_List = new List<double>();
            hVL_List.Clear();            
            for(int i = 0; i < hVsatList.Count; i++)
            {
                double subtracted_value = hVsatList[i] - hLsatList[i];
                hVL_List.Add(subtracted_value);
            }

            //--14. hX = hLsat*ones(1,9) + hVL*(0.1:0.1:0.9); % [kJ/kg] mixture enthalpy

            double[,] hX = new double[hLsatList.Count,9];
            double[,] hVL_LeftSideValue = new double[hLsatList.Count, 9];//stores hVL*(0.1:0.1:0.9);
            double[,] hLsat_RightSideValue = new double[hLsatList.Count, 9];//stores hLsat*ones(1,9) 

            double stepVal = 0.1;
            for(int i = 0; i < 9; i++)
            {
                for(int j =0;j<hVL_List.Count;j++)
                {
                    hVL_LeftSideValue[j,i] = hVL_List[j] * stepVal;

                }
                stepVal += 0.1;

            }

            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < hLsatList.Count; j++)
                {
                    hLsat_RightSideValue[j, i] = hLsatList[j] ;

                }
                //stepVal += 0.1;

            }

            //For final sum
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < hLsatList.Count; j++)
                {
                    hX[j, i] =  hLsat_RightSideValue[j, i]+ hVL_LeftSideValue[j, i];//  hLsatList[j].Value;

                }
                //stepVal += 0.1;

            }

            //TEST ...
            string test1=null;
            for (int i = 0; i < 1; i++)
            {
                for (int j = 0; j < hLsatList.Count; j++)
                {
                    test1 +=  hX[j, i] +"\t";

                }
              
            }
            // MessageBox.Show("Test = \n" + test1);
            //--Now lets move to plotting the things


            //Conture plot and


            /*
             For this function plot 
             plot([hLsat;hLcrit],[psat;pcrit],'b', ...
                    [hVsat;hVcrit],[psat;pcrit],'r', ...
                    hcrit,pcrit,'o', ...
                    hX,psat*ones(1,9),'g') % vapor dome
                    
             */
            // ph_chart.ChartAreas["ChartArea1"].Axes.
            ph_chart.ChartAreas[0].AxisX.Minimum = 1/1000;
            ph_chart.ChartAreas[0].AxisX.Maximum = 4000;

            ph_chart.ChartAreas[0].AxisX.Interval = 500;
            

            ph_chart.ChartAreas[0].AxisY.Minimum = 0.001;
            ph_chart.ChartAreas[0].AxisY.IsLogarithmic = true;
            ph_chart.ChartAreas[0].AxisY.LogarithmBase = 10;
            ph_chart.ChartAreas[0].AxisY.Interval = 1;
            //ph_chart.ChartAreas[0].AxisY.Maximum = 100;
            // ph_chart.ChartAreas[0].AxisY.Interval = 500;

            //--this one is for [hLsat;hLcrit],[psat;pcrit],'b', ...
            // ph_chart.ChartAreas[0].AxisX.Minimum =

            ph_chart.Series.Clear();

            ph_chart.Series.Add("Series01");
            ph_chart.Series["Series01"].ChartType = SeriesChartType.Line;
            for(int i = 0; i < hLsatList.Count; i++)
            {
              ph_chart.Series["Series01"].Points.AddXY(hLsatList[i], psatList[i]);
             }
            ph_chart.Series["Series01"].Points.AddXY(hLcrit,pcrit);
            ph_chart.Series["Series01"].Color = Color.Blue;
            ph_chart.Series["Series01"].ChartArea = "ChartArea1";


            //This one if for  [hVsat;hVcrit],[psat;pcrit],'r', ...
            ph_chart.Series.Add("Series2");
            ph_chart.Series["Series2"].ChartType = SeriesChartType.Line;
            for (int i = 0; i < hVsatList.Count; i++)
            {
                ph_chart.Series["Series2"].Points.AddXY(hVsatList[i], psatList[i]);
            }
            ph_chart.Series["Series2"].Points.AddXY(hVcrit, pcrit);
            ph_chart.Series["Series2"].Color = Color.Red;
            ph_chart.Series["Series2"].ChartArea = "ChartArea1";


            //--For critical point
            //This one if for  hcrit,pcrit,'o', ...
            ph_chart.Series.Add("Series3");
            ph_chart.Series["Series3"].ChartType = SeriesChartType.Point;

            ph_chart.Series["Series3"].Points.AddXY(hcrit, pcrit);
            ph_chart.Series["Series3"].Color = Color.Orange;
            ph_chart.Series["Series3"].ChartArea = "ChartArea1";

            //--This one is the last one
            double[,] psatOneToNine = new double[psatList.Count, 9];
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < psatList.Count; j++)
                {
                    psatOneToNine[j, i] = psatList[j];

                }
                //stepVal += 0.1;
            }

            for (int i = 0; i < hX.GetLength(1); i++)//Row 
            {

                ph_chart.Series.Add("Series4"+i);
                ph_chart.Series["Series4"+i].ChartType = SeriesChartType.Line;



                for (int j= 0;j < hX.GetLength(0); j++)//Column 
                {
                     ph_chart.Series["Series4"+i].Points.AddXY(hX[j,i], psatOneToNine[j,i]);
                }

                ph_chart.Series["Series4"+i].Color = Color.Green;
                ph_chart.Series["Series4"+i].ChartArea = "ChartArea1";

              

               
            }
            //ph_chart.Series["Series4"].Points.AddXY(hVcrit, pcrit);

            // MessageBox.Show("Enthalpy row  = " + h.GetLength(0) + ",col = " + h.GetLength(1) +"Trimmed value row= "+h_trimed.GetLength(0)+",col = "+h_trimed.GetLength(1));

            // MessageBox.Show("end \n");


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
            for(int i = 0; i < 25; i++)
            {
                string name = "ContourSeries" + i;
                alSeries.Add(name);

            }


            Example6_3.ChartFunctions cf = new Example6_3.ChartFunctions();
            Example6_3.DataSeries ds = new Example6_3.DataSeries();
            Example6_3.Form1 f1 = new Example6_3.Form1();
            Example6_3.DrawChart dc = new Example6_3.DrawChart(f1);

            cf.SetDataPoints(ds, P_Value, T_Value, H_Value);
            //==After calling this we need to call the number of contour plots
            dc.numberContours = 25;


            List<Example6_3.DrawChart.DataTypeForPointList> listPoints = new List<Example6_3.DrawChart.DataTypeForPointList>();

            listPoints = dc.AddContour_MyCustomFxn(ds);


            //--Serching z-levels and plotting 
            int initialZ = listPoints[0].zlevel;
            int initalIndex = 0;
            int runningz = 0;
            for(int i = 0; i < 25; i++)
            {
                //==First one is for moving in the list
                string seriesName = alSeries[i].ToString();
                ph_chart.Series.Add(seriesName);
                ph_chart.Series[seriesName].ChartType = SeriesChartType.Line;

                //==For moving in the list
                for (int z= initalIndex; z < listPoints.Count; z++)
                {
                    runningz = z;
                    if (initialZ == listPoints[z].zlevel)
                    {
                        //==Same zlevel then draw on one line
                        ph_chart.Series[seriesName].Points.AddXY(listPoints[z].x1, listPoints[z].y1);
                        ph_chart.Series[seriesName].Points.AddXY(listPoints[z].x2, listPoints[z].y2);


                    }
                    else
                    {
                        initialZ = listPoints[z].zlevel;
                        initalIndex = z;
                        break;
                    }

                }

                if(runningz == listPoints.Count)
                {
                    break;
                }


            }




            //---End of the contour line plot chunk of code---//



        }
       

        public void DataPullFunctionForCoolProp(string fluidName)
        {

            /*
             Lets do it step by step as the way that has been done in the Matlab

             */
            //--1. press_rng = logspace(-2,2,300); % [MPa] pressure (p) range

            PH_ChartClass PH = new PH_ChartClass();
            double[] press_rng = PH.LogSpace(-2, 2, 300, true, 10).ToArray();
            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s = null;
            for (int i = 0; i < press_rng.Length; i++)
            {
                s += press_rng[i].ToString() + "\t \t";
            }
            //MessageBox.Show("press_rng values = \n" + s);


            //--For line spacing values
            //2. temp_rng = 273.15+linspace(1,800,300); 
            double[] temp_rng = PH.LinSpace(1, 800, 300, true).ToArray();


            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s1 = null;
            for (int i = 0; i < temp_rng.Length; i++)
            {
                temp_rng[i] += 273.15;
                s1 += temp_rng[i].ToString() + "\t \t";
            }
            // MessageBox.Show("temp_rng values = \n" + s1);



            //--Now for meshgrid we have created truple

            //MeshGrid_Class ms1 = new MeshGrid_Class();
            //3.[p,T] = meshgrid(press_rng,temp_rng); % [MPa,K] mesh p & T

            MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            Tuple<double[,], double[,]> tuple_x = MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            double[,] p = tuple_x.Item1;
            double[,] T = tuple_x.Item2;


            // MessageBox.Show("Tuple =\n" + tuple_x.Item1.Length);
            // MessageBox.Show("Tuple =\n" + p[1,1]);
            //  MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            // MessageBox.Show("Check row = " + p.GetLength(0) + ",column = " + p.GetLength(1)+"pvalue check= "+p[0,0]+", last = "+p[299,299]);

            string pTest1 = null;
            string hum = null;
            for (int i = 290; i < 300; i++)//Row length 
            {
                // pTest += item + "\t";
                for (int j = 290; j < 300; j++)//column length 
                {
                    pTest1 = "V=" + p[i, j].ToString() + "\t\t";
                    hum += pTest1;
                }
            }

            //--8 pcrit = 22.064; % [MPa] critical pressure
            double pcrit = CoolProp.Props1SI("Water", "Pcrit") / 1000000;//To convert to Pa form MPa //22.064;//CoolProp.Props1SI( "Water", "Pcrit");//PropsSI("Pcrit", "", 0, "", 0, "Water");//PH.IAPWS_IF97_TowParameterEquivalentFxn("Pcrit", "", 0, "",0, fluidName); //CoolProp.Props1SI(fluidName, "Pcrit");// CP.PropsSI("Tcrit","",0,"",0,"Water")


          //  MessageBox.Show("Pcrit = " + pcrit);
            //--Now lets do the calculation for others 
            //4.h = IAPWS_IF97('h_pT',p,T); % [kJ/kg] enthalpy = f(p,T)
            P_Value = p;//Copy
            T_Value = T;//Copy


            double[,] h = new double[p.GetLength(0), p.GetLength(1)];

            for (int i = 0; i < h.GetLength(0); i++)
            {
                for (int j = 0; j < h.GetLength(1); j++)
                {
                    //if (p[i, j] <= pcrit) // 22.064 for water
                    //{ 
                    //--Now calling is done for the enthalpy function
                    double enthalpy = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", p[i, j]*1000000, "T", T[i, j], fluidName) / 1000;
                    h[i, j] = enthalpy;
                    //}

                }
            }
            H_Value = h;//Copy

            //ExcellDataPull ps = new ExcellDataPull();
            //ps.ExcellData();//Calling excel data
            //                //--5 psat = IAPWS_IF97('psat_T',temp_rng); % [MPa] saturation pressure

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Lets call the actual function
            string fluidNAME = "Water";//"1-Butene"; // "Water";//"n-Propane";//
            PlotPHChart(fluidNAME);
        }



        public void PH_CurveData()
        {

        }



    }
}
