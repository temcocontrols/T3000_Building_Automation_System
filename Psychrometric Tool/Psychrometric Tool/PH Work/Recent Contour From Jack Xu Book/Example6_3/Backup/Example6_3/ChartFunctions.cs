using System;
using System.Collections.Generic;
using System.Text;

namespace Example6_3
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
            cs.XMin = -3;
            cs.XMax = 3;
            cs.YMin = -3;
            cs.YMax = 3;
            cs.ZMin = -8;
            cs.ZMax = 8;
            cs.XTick = 1;
            cs.YTick = 1;
            cs.ZTick = 4;

            ds.XDataMin = cs.XMin;
            ds.YDataMin = cs.YMin;
            ds.XSpacing = 0.3f;
            ds.YSpacing = 0.3f;
            ds.XNumber = Convert.ToInt16((cs.XMax - cs.XMin) / ds.XSpacing) + 1;
            ds.YNumber = Convert.ToInt16((cs.YMax - cs.YMin) / ds.YSpacing) + 1;

            Point3[,] pts = new Point3[ds.XNumber, ds.YNumber];
            for (int i = 0; i < ds.XNumber; i++)
            {
                for (int j = 0; j < ds.YNumber; j++)
                {
                    float x = ds.XDataMin + i * ds.XSpacing;
                    float y = ds.YDataMin + j * ds.YSpacing;
                    double zz = 3 * Math.Pow((1 - x), 2) * Math.Exp(-x * x -
                        (y + 1) * (y + 1)) - 10 * (0.2 * x - Math.Pow(x, 3) -
                        Math.Pow(y, 5)) * Math.Exp(-x * x - y * y) -
                        1 / 3 * Math.Exp(-(x + 1) * (x + 1) - y * y);
                    float z = (float)zz;
                    pts[i, j] = new Point3(x, y, z, 1);
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
