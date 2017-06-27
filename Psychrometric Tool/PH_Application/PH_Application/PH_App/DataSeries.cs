using System;
using System.Collections;
using System.Drawing;

namespace PH_App
{
    public class DataSeries
    {
        private ArrayList pointList;
        private LineStyle lineStyle;
        private BarStyle barStyle;
        private float xdataMin = -5;
        private float ydataMin = -5;
        private float zzdataMin = -5;
        private float xSpacing = 1;
        private float ySpacing = 1;
        private float zSpacing = 1;
        private int xNumber = 10;
        private int yNumber = 10;
        private int zNumber = 10;
        private Point3[,] pointArray;
        private Point4[, ,] point4Array;
        
        public DataSeries()
        {
            lineStyle = new LineStyle();
            barStyle = new BarStyle();
            pointList = new ArrayList();
        }

        public Point4[, ,] Point4Array
        {
            get { return point4Array; }
            set { point4Array = value; }
        }

        public Point3[,] PointArray
        {
            get { return pointArray; }
            set { pointArray = value; }
        }

        public int XNumber
        {
            get { return xNumber; }
            set { xNumber = value; }
        }

        public int YNumber
        {
            get { return yNumber; }
            set { yNumber = value; }
        }

        public int ZNumber
        {
            get { return zNumber; }
            set { zNumber = value; }
        }

        public float XSpacing
        {
            get { return xSpacing; }
            set { xSpacing = value; }
        }

        public float YSpacing
        {
            get { return ySpacing; }
            set { ySpacing = value; }
        }

        public float ZSpacing
        {
            get { return zSpacing; }
            set { zSpacing = value; }
        }

        public float XDataMin
        {
            get { return xdataMin; }
            set { xdataMin = value; }
        }

        public float YDataMin
        {
            get { return ydataMin; }
            set { ydataMin = value; }
        }

        public float ZZDataMin
        {
            get { return zzdataMin; }
            set { zzdataMin = value; }
        }

        public LineStyle LineStyle
        {
            get { return lineStyle; }
            set { lineStyle = value; }
        }

        public BarStyle BarStyle
        {
            get { return barStyle; }
            set { barStyle = value; }
        }

        public ArrayList PointList
        {
            get { return pointList; }
            set { pointList = value; }
        }

        public void AddPoint(Point3 pt)
        {
            PointList.Add(pt);
        }

        public float ZDataMin()
        {
            float zmin = 0;
            for (int i = 0; i < PointArray.GetLength(0); i++)
            {
                for (int j = 0; j < PointArray.GetLength(1); j++)
                {
                    zmin = Math.Min(zmin, PointArray[i, j].Z);
                }
            }
            return zmin;
        }

        public float ZDataMax()
        {
            float zmax = 0;
            for (int i = 0; i < PointArray.GetLength(0); i++)
            {
                for (int j = 0; j < PointArray.GetLength(1); j++)
                {
                    zmax = Math.Max(zmax, PointArray[i, j].Z);
                }
            }
            return zmax;
        }

        public float VDataMin()
        {
            float vmin = 0;
            for (int i = 0; i < Point4Array.GetLength(0); i++)
            {
                for (int j = 0; j < Point4Array.GetLength(1); j++)
                {
                    for (int k = 0; k < Point4Array.GetLength(2); k++)
                    {
                        vmin = Math.Min(vmin, Point4Array[i, j, k].V);
                    }
                }
            }
            return vmin;
        }

        public float VDataMax()
        {
            float vmax = 0;
            for (int i = 0; i < Point4Array.GetLength(0); i++)
            {
                for (int j = 0; j < Point4Array.GetLength(1); j++)
                {
                    for (int k = 0; k < Point4Array.GetLength(2); k++)
                    {
                        vmax = Math.Max(vmax, Point4Array[i, j, k].V);
                    }
                }
            }
            return vmax;
        }
    }
}

