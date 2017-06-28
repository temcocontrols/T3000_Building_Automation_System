using System;
using System.Collections.Generic;
using System.Text;

namespace PH_App
{
    public class Point3
    {
        public float X;
        public float Y;
        public float Z;
        public float W = 1f;

        public Point3()
        {
        }

        public Point3(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public void Transform(Matrix3 m)
        {
            float[] result = m.VectorMultiply(new float[4] { X, Y, Z, W });
            X = result[0];
            Y = result[1];
            Z = result[2];
            W = result[3];
        }

        public void Transform(Matrix3 m, Form1_jack_xu form1, ChartStyle cs)
        {
            // Normalize the point:
            float x1 = (X - cs.XMin) / (cs.XMax-cs.XMin) - 0.5f;
            float y1 = (Y - cs.YMin) / (cs.YMax - cs.YMin) - 0.5f;
            float z1 = (Z - cs.ZMin) / (cs.ZMax - cs.ZMin) - 0.5f;
            // Perform transformation on the point using matrix m:
            float[] result = m.VectorMultiply(new float[4] { x1, y1, z1, W });
            X = result[0];
            Y = result[1];
            // Coordinate transformation from World to Device system:
            float xShift = 1.05f;
            float xScale = 1;
            float yShift = 1.05f;
            float yScale = 0.9f;
            if (cs.Title == "No Title")
            {
                yShift = 0.95f;
                yScale = 1;
            }
            if (cs.IsColorBar)
            {
                xShift = 0.95f;
                xScale = 0.9f;
            }
            X = (xShift + xScale * X) * form1.PlotPanel.Width / 2;
            Y = (yShift - yScale * Y) * form1.PlotPanel.Height / 2;
        }
    }
}
