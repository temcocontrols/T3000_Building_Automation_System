using System;
using System.Collections.Generic;
using System.Text;

namespace PH_App
{
    public class ChartFunctions
    {
        public ChartFunctions()
        {
        }

        public void Line3D(DataSeries ds, ChartStyle cs)
        {
            cs.XMin = -1f;
            cs.XMax = 1f;
            cs.YMin = -1f;
            cs.YMax = 1f;
            cs.ZMin = 0;
            cs.ZMax = 30;
            cs.XTick = 0.5f;
            cs.YTick = 0.5f;
            cs.ZTick = 5;

            ds.XDataMin = cs.XMin;
            ds.YDataMin = cs.YMin;
            ds.XSpacing = 0.3f;
            ds.YSpacing = 0.3f;
            ds.XNumber = Convert.ToInt16((cs.XMax - cs.XMin) / ds.XSpacing) + 1;
            ds.YNumber = Convert.ToInt16((cs.YMax - cs.YMin) / ds.YSpacing) + 1;
            ds.PointList.Clear();

            for (int i = 0; i < 300; i++)
            {
                float t = 0.1f * i;
                float x = (float)Math.Exp(-t / 30) *
                    (float)Math.Cos(t);
                float y = (float)Math.Exp(-t / 30) *
                    (float)Math.Sin(t);
                float z = t;
                ds.AddPoint(new Point3(x, y, z, 1));
            }
        }

        public void Peak3D(DataSeries ds, ChartStyle cs)
        {


            /*

                   cs.XMin = -1;
                   cs.XMax = 1;
                   cs.YMin = -1;
                   cs.YMax = 1;
                   cs.ZMin = -1;
                   cs.ZMax = 1;
                   cs.XTick = 1;
                   cs.YTick = 1;
                   cs.ZTick = 1;

                   //cs.XMin = -1;
                   //cs.XMax = 1;
                   //cs.YMin = -1;
                   //cs.YMax = 1;
                   //cs.ZMin = -1;
                   //cs.ZMax = 1;
                   //cs.XTick = 1;
                   //cs.YTick = 1;
                   //cs.ZTick = 1;

                   ds.XDataMin = cs.XMin;
                   ds.YDataMin = cs.YMin;
            ds.XSpacing = 1;// 0.3f;
            ds.YSpacing = 1;// 0.3f;
                   ds.XNumber = Convert.ToInt16((cs.XMax - cs.XMin) / ds.XSpacing) + 1;
                   ds.YNumber = Convert.ToInt16((cs.YMax - cs.YMin) / ds.YSpacing) + 1;

                   Point3[,] pts = new Point3[ds.XNumber, ds.YNumber];
                   for (int i = 0; i < ds.XNumber; i++)
                   {
                       for (int j = 0; j < ds.YNumber; j++)
                       {
                           float x = ds.XDataMin + i * ds.XSpacing;
                           float y = ds.YDataMin + j * ds.YSpacing;
                          /*
                             double zz = 3 * Math.Pow((1 - x), 2) * Math.Exp(-x * x -
                                (y + 1) * (y + 1)) - 10 * (0.2 * x - Math.Pow(x, 3) -
                                Math.Pow(y, 5)) * Math.Exp(-x * x - y * y) -
                                1 / 3 * Math.Exp(-(x + 1) * (x + 1) - y * y);
                            */ /* 
                           double zz = Math.Pow(x, 2) + Math.Pow(y, 2);
                      float z = (float)zz;


                           pts[i, j] = new Point3(x, y, z, 1);
                       }
                   }

            //==========================Contour demo code added====================//
          //  */  /* 

             int[,] X = new int[6, 4];
             int[,] Y = new int[6, 4];
             int[,] Z = new int[6, 4];

            cs.XMin = -4;
            cs.XMax = 2;
            cs.YMin = -2;
            cs.YMax = 3;
            //cs.ZMin = -1;
            //cs.ZMax = 1;
            cs.XTick = 1;
            cs.YTick = 1;
           // cs.ZTick = 1;

            X[0, 0] = -4;
             X[1, 0] = -4;
             X[2, 0] = -4;
             X[3, 0] = -4;
             X[4, 0] = -4;
             X[5, 0] = -4;

            X[0, 1] = -2;
             X[1, 1] = -2;
            X[2, 1] = -2;
            X[3, 1] = -2;
            X[4, 1] = -2;
            X[5, 1] = -2;

             X[0, 2] = 0;
             X[1, 2] = 0;
             X[2, 2] = 0;
            X[3, 2] = 0;
            X[4, 2] = 0;
            X[5, 2] = 0;

            X[0, 3] = 2;
            X[1, 3] = 2;
            X[2, 3] = 2;
            X[3, 3] = 2;
            X[4, 3] = 2;
            X[5, 3] = 2;


             Y[0, 0] = -2;
             Y[1, 0] = -1;
            Y[2, 0] = 0;
            Y[3, 0] = 1;
            Y[4, 0] = 2;
            Y[5, 0] = 3;



            Y[0, 1] = -2;
             Y[1, 1] = 1;
             Y[2, 1] = 0;
            Y[3, 1] = 1;
            Y[4, 1] = 2;
            Y[5, 1] = 3;


            Y[0, 2] = -2;
            Y[1, 2] = 1;
            Y[2, 2] = 0;
            Y[3, 2] = 1;
            Y[4, 2] = 2;
            Y[5, 2] = 3;

            Y[0, 3] = -2;
            Y[1, 3] = 1;
            Y[2, 3] = 0;
            Y[3, 3] = 1;
            Y[4, 3] = 2;
            Y[5, 3] = 3;



             Z[0, 0] = 20;
             Z[1, 0] = 17;
             Z[2, 0] = 16;
            Z[3, 0] = 17;
            Z[4, 0] = 20;
            Z[5, 0] = 25;

            Z[0, 1] = 8;
             Z[1, 1] = 5;
             Z[2, 1] = 4;
            Z[3, 1] = 5;
            Z[4, 1] = 8;
            Z[5, 1] = 13;


            Z[0, 2] = 4;
             Z[1, 2] = 1;
             Z[2, 2] = 0;
            Z[3, 2] = 1;
            Z[4, 2] = 4;
            Z[5, 2] = 9;

            Z[0, 3] = 8;
            Z[1, 3] = 5;
            Z[2, 3] = 4;
            Z[3, 3] = 5;
            Z[4, 3] = 8;
            Z[5, 3] = 13;


            Point3[,] pts = new Point3[4, 6];

             for (int i = 0; i <4; i++)
             {
                 for (int j = 0; j <6; j++)
                 {
                     //pts[i, j].X = X[i, j];
                     //pts[i, j].Y = Y[i, j];
                     //pts[i, j].Z = Z[i, j];
                     pts[i, j] = new Point3(X[j,i], Y[j,i], Z[j,i], 1);

                 }
             }

             */

            //  if (pts.Length == 0) { }
            //=====================End of the contour demo code==================//



            //==First calling the data in WFA_PH_Demo Project
            //WFA_PH_Demo 

            ///*


            cs.XMin = 0;
            cs.XMax = 4000;
            cs.YMin = 0;
            cs.YMax = 100;
            cs.ZMin = 0;
            cs.ZMax = 1;
            cs.XTick = 500;
            cs.YTick = 10;
            cs.ZTick = 1;

            ds.XDataMin = cs.XMin;
            ds.YDataMin = cs.YMin;
            ds.XSpacing = 1;// 0.3f;
            ds.YSpacing = 1;// 0.3f;

            // WFA_PH_CurveDemo.Form1 f1 = new WFA_PH_CurveDemo.Form1();
            // f1.PlotPHChart("Water");//This will call the function required

            // MessageBox.Show("We are here inside peak 3d so ");

            // MessageBox.Show("Read finish excel file");
            // f1.DataPullFunctionForCoolProp("water");//n-Propane

            //ExcellDataPull ex = new ExcellDataPull();
            //ex.ExcellData();

            Point3[,] pts = new Point3[300, 300];

            /*
                 for (int i = 0; i < f1.P_Value.GetLength(0); i++)
                 {
                     for (int j = 0; j <f1.P_Value.GetLength(1); j++)
                     {
                                //pts[i, j].X = X[i, j];
                                //pts[i, j].Y = Y[i, j];
                                //pts[i, j].Z = Z[i, j];
                                 pts[i, j] = new Point3((int)f1.H_Value[j,i], (int)f1.P_Value[j, i], (int)f1.T_Value[j, i], 1);
                                //pts[i, j] = new Point3((int)f1.H_Value[i ,j], (int)f1.P_Value[i, j], (int)f1.T_Value[i, j], 1);
                                //pts[i, j] = new Point3( (float)f1.P_Value[i, j], (float)ex.h_list_Value[i,j], (float)f1.T_Value[i, j], 1);
                                //pts[i, j] = new Point3( (float)ex.h_list_Value[i, j], (float)f1.P_Value[j, i], (float)f1.T_Value[j, i], 1);

                            }
                 }
                        int lastValue =(int) f1.P_Value[299, 299];

           

            */
            ds.PointArray = pts;
        }
        public void SetDataPoints(DataSeries ds, double[,] P_Value, double[,] T_Value, double[,] H_Value)
        {
            ds.XSpacing =  1;// 0.3f;
            ds.YSpacing =  1;// 0.3f;

            Point3[,] pts = new Point3[300, 300];

            for (int i = 0; i < P_Value.GetLength(0); i++)
            {
                for (int j = 0; j < P_Value.GetLength(1); j++)
                {
                    pts[i, j] = new Point3((float)H_Value[j, i], (float)P_Value[j, i], (float)T_Value[j, i], 1);                   
                }
            }
           
            ds.PointArray = pts;

        }

        public void SinROverR3D(DataSeries ds, ChartStyle cs)
        {
            cs.XMin = -8;
            cs.XMax = 8;
            cs.YMin = -8;
            cs.YMax = 8;
            cs.ZMin = -0.5f;
            cs.ZMax = 1;
            cs.XTick = 4;
            cs.YTick = 4;
            cs.ZTick = 0.5f;
            
            ds.XDataMin = cs.XMin;
            ds.YDataMin = cs.YMin;
            ds.XSpacing = 0.5f;
            ds.YSpacing = 0.5f;
            ds.XNumber = Convert.ToInt16((cs.XMax - cs.XMin) / ds.XSpacing) + 1;
            ds.YNumber = Convert.ToInt16((cs.YMax - cs.YMin) / ds.YSpacing) + 1;

            Point3[,] pts = new Point3[ds.XNumber, ds.YNumber];
            for (int i = 0; i < ds.XNumber; i++)
            {
                for (int j = 0; j < ds.YNumber; j++)
                {
                    float x = ds.XDataMin + i * ds.XSpacing;
                    float y = ds.YDataMin + j * ds.YSpacing;
                    float r = (float)Math.Sqrt(x * x + y * y) + 0.000001f;
                    float z = (float)Math.Sin(r) / r;
                    pts[i, j] = new Point3(x, y, z, 1);
                }
            }
            ds.PointArray = pts;
        }

        public void Exp4D(DataSeries ds, ChartStyle cs)
        {
            cs.XMin = -2;
            cs.XMax = 2;
            cs.YMin = -2;
            cs.YMax = 2;
            cs.ZMin = -2;
            cs.ZMax = 2;
            cs.XTick = 1;
            cs.YTick = 1;
            cs.ZTick = 1;

            ds.XDataMin = cs.XMin;
            ds.YDataMin = cs.YMin;
            ds.ZZDataMin = cs.ZMin;
            ds.XSpacing = 0.1f;
            ds.YSpacing = 0.1f;
            ds.ZSpacing = 0.1f;
            ds.XNumber = Convert.ToInt16((cs.XMax - cs.XMin) / ds.XSpacing) + 1;
            ds.YNumber = Convert.ToInt16((cs.YMax - cs.YMin) / ds.YSpacing) + 1;
            ds.ZNumber = Convert.ToInt16((cs.ZMax - cs.ZMin) / ds.ZSpacing) + 1;

            Point4[, ,] pts = new Point4[ds.XNumber, ds.YNumber, ds.ZNumber];
            for (int i = 0; i < ds.XNumber; i++)
            {
                for (int j = 0; j < ds.YNumber; j++)
                {
                    for (int k = 0; k < ds.ZNumber; k++)
                    {
                        float x = ds.XDataMin + i * ds.XSpacing;
                        float y = ds.YDataMin + j * ds.YSpacing;
                        float z = cs.ZMin + k * ds.ZSpacing;
                        float v = z * (float)Math.Exp(-x * x - y * y - z * z);
                        pts[i, j, k] = new Point4(new Point3(x, y, z, 1), v);
                    }
                }
            }
            ds.Point4Array = pts;
        }

    }
}
