using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;

namespace PH_App
{
    public class ChartStyle
    {
        private Form1_jack_xu form1;
        private float xMax = 5f;
        private float xMin = -5f;
        private float yMax = 3f;
        private float yMin = -3f;
        private float zMax = 6f;
        private float zMin = -6f;
        private float xTick = 1f;
        private float yTick = 1f;
        private float zTick = 3f;
        private Font tickFont = new Font("Arial Narrow",8, FontStyle.Regular);
        private Color tickColor = Color.Black;
        private string title = "My 3D Chart";
        private Font titleFont = new Font("Arial Narrow", 14, FontStyle.Regular);
        private Color titleColor = Color.Black;
        private string xLabel = "X Axis";
        private string yLabel = "Y Axis";
        private string zLabel = "Z Axis";
        private Font labelFont = new Font("Arial Narrow", 10, FontStyle.Regular);
        private Color labelColor = Color.Black;
        private float elevation = 30;
        private float azimuth = -37.5f;
        private bool isXGrid = true;
        private bool isYGrid = true;
        private bool isZGrid = true;
        LineStyle gridStyle;
        LineStyle axisStyle;
        private bool isColorBar = false;

        public ChartStyle(Form1_jack_xu fm1)
        {
            form1 = fm1;
            gridStyle = new LineStyle();
            axisStyle = new LineStyle();
        }

        public bool IsColorBar
        {
            get { return isColorBar; }
            set { isColorBar = value; }
        }

        public LineStyle AxisStyle
        {
            get { return axisStyle; }
            set { axisStyle = value; }
        }

        public LineStyle GridStyle
        {
            get { return gridStyle; }
            set { gridStyle = value; }
        }

        public Font LabelFont
        {
            get { return labelFont; }
            set { labelFont = value; }
        }

        public Color LabelColor
        {
            get { return labelColor; }
            set { labelColor = value; }
        }

        public Font TitleFont
        {
            get { return titleFont; }
            set { titleFont = value; }
        }

        public Color TitleColor
        {
            get { return titleColor; }
            set { titleColor = value; }
        }

        public Font TickFont
        {
            get { return tickFont; }
            set { tickFont = value; }
        }

        public Color TickColor
        {
            get { return tickColor; }
            set { tickColor = value; }
        }

        public bool IsXGrid
        {
            get { return isXGrid; }
            set { isXGrid = value; }
        }

        public bool IsYGrid
        {
            get { return isYGrid; }
            set { isYGrid = value; }
        }

        public bool IsZGrid
        {
            get { return isZGrid; }
            set { isZGrid = value; }
        }

        public string Title
        {
            get { return title; }
            set { title = value; }
        }

        public string XLabel
        {
            get { return xLabel; }
            set { xLabel = value; }
        }

        public string YLabel
        {
            get { return yLabel; }
            set { yLabel = value; }
        }

        public string ZLabel
        {
            get { return zLabel; }
            set { zLabel = value; }
        }

        public float Elevation
        {
            get { return elevation; }
            set { elevation = value; }
        }

        public float Azimuth
        {
            get { return azimuth; }
            set { azimuth = value; }
        }

        public float XMax
        {
            get { return xMax; }
            set { xMax = value; }
        } 

        public float XMin
        {
            get { return xMin; }
            set { xMin = value; }
        }

        public float YMax
        {
            get { return yMax; }
            set { yMax = value; }
        }

        public float YMin
        {
            get { return yMin; }
            set { yMin = value; }
        }

        public float ZMax
        {
            get { return zMax; }
            set { zMax = value; }
        }

        public float ZMin
        {
            get { return zMin; }
            set { zMin = value; }
        }

        public float XTick
        {
            get { return xTick; }
            set { xTick = value; }
        }

        public float YTick
        {
            get { return yTick; }
            set { yTick = value; }
        }

        public float ZTick
        {
            get { return zTick; }
            set { zTick = value; }
        }

        private Point3[] CoordinatesOfChartBox()
        {
            // Create coordinate of the axes:
            Point3[] pta = new Point3[8];
            pta[0] = new Point3(XMax, YMin, ZMin, 1);
            pta[1] = new Point3(XMin, YMin, ZMin, 1);
            pta[2] = new Point3(XMin, YMax, ZMin, 1);
            pta[3] = new Point3(XMin, YMax, ZMax, 1);
            pta[4] = new Point3(XMin, YMin, ZMax, 1);
            pta[5] = new Point3(XMax, YMin, ZMax, 1);
            pta[6] = new Point3(XMax, YMax, ZMax, 1);
            pta[7] = new Point3(XMax, YMax, ZMin, 1);

            Point3[] pts = new Point3[4];
            int[] npts = new int[4] { 0, 1, 2, 3 };
            if (elevation >= 0)
            {
                if (azimuth >= -180 && azimuth < -90)
                {
                    npts = new int[4] { 1,2,7,6 };
                }
                else if (azimuth >= -90 && azimuth < 0)
                {
                    npts = new int[4] { 0, 1, 2, 3 };
                }
                else if (azimuth >= 0 && azimuth < 90)
                {
                    npts = new int[4] { 7, 0, 1, 4 };
                }
                else if (azimuth >= 90 && azimuth <= 180)
                {
                    npts = new int[4] { 2, 7, 0, 5 };
                }
            }
            else if (elevation < 0)
            {
                if (azimuth >= -180 && azimuth < -90)
                {
                    npts = new int[4] { 1, 0, 7, 6 };
                }
                else if (azimuth >= -90 && azimuth < 0)
                {
                    npts = new int[4] { 0, 7, 2, 3 };
                }
                else if (azimuth >= 0 && azimuth < 90)
                {
                    npts = new int[4] { 7, 2, 1, 4 };
                }
                else if (azimuth >= 90 && azimuth <= 180)
                {
                    npts = new int[4] { 2, 1, 0, 5 };
                }

            }

            for (int i = 0; i < 4; i++)
            {
                pts[i] = pta[npts[i]];
            }
            return pts;
        }

        public void AddChartStyle(Graphics g)
        {
            AddTicks(g);
            AddGrids(g);
            AddAxes(g);
            AddLabels(g);
        }

        private void AddAxes(Graphics g)
        {
            Matrix3 m = Matrix3.AzimuthElevation(Elevation, Azimuth);
            Point3[] pts = CoordinatesOfChartBox();
            Pen aPen = new Pen(AxisStyle.LineColor, AxisStyle.Thickness);
            aPen.DashStyle = AxisStyle.Pattern;
            for (int i = 0; i < pts.Length; i++)
            {
                pts[i].Transform(m, form1, this);
            }
            g.DrawLine(aPen, pts[0].X, pts[0].Y, pts[1].X, pts[1].Y);
            g.DrawLine(aPen, pts[1].X, pts[1].Y, pts[2].X, pts[2].Y);
            g.DrawLine(aPen, pts[2].X, pts[2].Y, pts[3].X, pts[3].Y);
            aPen.Dispose();
        }

        private void AddTicks(Graphics g)
        {
            Matrix3 m = Matrix3.AzimuthElevation(Elevation, Azimuth);
            Point3[] pta = new Point3[2];
            Point3[] pts = CoordinatesOfChartBox();
            Pen aPen = new Pen(AxisStyle.LineColor, AxisStyle.Thickness);
            aPen.DashStyle = AxisStyle.Pattern;

            // Add x ticks:
            float offset = (YMax - YMin) / 30.0f;
            float ticklength = offset;
            for (float x = XMin; x <= XMax; x = x + XTick)
            {
                if (elevation >= 0)
                {
                    if (azimuth >= -90 && azimuth < 90)
                        ticklength = -offset;
                }
                else if (elevation < 0)
                {
                    if ((azimuth >= -180 && azimuth < -90) ||
                        azimuth >= 90 && azimuth <= 180)
                        ticklength = -(YMax - YMin) / 30;
                } 
                pta[0] = new Point3(x, pts[1].Y + ticklength, pts[1].Z, pts[1].W);
                pta[1] = new Point3(x, pts[1].Y, pts[1].Z, pts[1].W);
                for (int i = 0; i < pta.Length; i++)
                {
                    pta[i].Transform(m, form1, this);
                }
                g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
            }

            // Add y ticks:
            offset = (XMax - XMin) / 30.0f;
            ticklength = offset;
            for (float y = YMin; y <= YMax; y = y + YTick)
            {
                pts = CoordinatesOfChartBox();
                if (elevation >= 0)
                {
                    if (azimuth >= -180 && azimuth < 0)
                        ticklength = -offset;
                }
                else if (elevation < 0)
                {
                    if (azimuth >= 0 && azimuth < 180)
                        ticklength = -offset;
                }
                pta[0] = new Point3(pts[1].X + ticklength, y, pts[1].Z, pts[1].W);
                pta[1] = new Point3(pts[1].X, y, pts[1].Z, pts[1].W);
                for (int i = 0; i < pta.Length; i++)
                {
                    pta[i].Transform(m, form1, this);
                }
                g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
            }

            float xoffset = (XMax - XMin) / 45.0f;
            float yoffset = (YMax - YMin) / 20.0f;
            float xticklength = xoffset;
            float yticklength = yoffset;
            for (float z = ZMin; z <= ZMax; z = z + ZTick)
            {
                if (elevation >= 0)
                {
                    if (azimuth >= -180 && azimuth < -90)
                    {
                        xticklength = 0;
                        yticklength = yoffset;
                    }
                    else if (azimuth >= -90 && azimuth < 0)
                    {
                        xticklength = xoffset;
                        yticklength = 0;
                    }
                    else if (azimuth >= 0 && azimuth < 90)
                    {
                        xticklength = 0;
                        yticklength = -yoffset;
                    }
                    else if (azimuth >= 90 && azimuth <= 180)
                    {
                        xticklength = -xoffset;
                        yticklength = 0;
                    }
                }
                else if (elevation <0)
                {
                     if (azimuth >= -180 && azimuth < -90)
                    {
                        yticklength = 0;
                        xticklength = xoffset;
                    }
                    else if (azimuth >= -90 && azimuth < 0)
                    {
                        yticklength = -yoffset;
                        xticklength = 0;
                    }
                    else if (azimuth >= 0 && azimuth < 90)
                    {
                        yticklength = 0;
                        xticklength = -xoffset;
                    }
                    else if (azimuth >= 90 && azimuth <= 180)
                    {
                        yticklength = yoffset;
                        xticklength = 0;
                    }
                }
                pta[0] = new Point3(pts[2].X, pts[2].Y, z, pts[2].W);
                pta[1] = new Point3(pts[2].X + yticklength , 
                    pts[2].Y + xticklength, z, pts[2].W);
                for (int i = 0; i < pta.Length; i++)
                {
                    pta[i].Transform(m, form1, this);
                }
                g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
            }
            aPen.Dispose();
        }

        private void AddGrids(Graphics g)
        {
            Matrix3 m = Matrix3.AzimuthElevation(Elevation, Azimuth);
            Point3[] pta = new Point3[3];
            Point3[] pts = CoordinatesOfChartBox();
            Pen aPen = new Pen(GridStyle.LineColor, GridStyle.Thickness);
            aPen.DashStyle = GridStyle.Pattern;

            // Draw x gridlines:
            if (IsXGrid)
            {
                for (float x = XMin; x <= XMax; x = x + XTick)
                {
                    pts = CoordinatesOfChartBox();
                    pta[0] = new Point3(x, pts[1].Y, pts[1].Z, pts[1].W);
                    if (elevation >= 0)
                    {
                        if ((azimuth >= -180 && azimuth < -90) ||
                            (azimuth >= 0 && azimuth < 90))
                        {
                            pta[1] = new Point3(x, pts[0].Y, pts[1].Z, pts[1].W);
                            pta[2] = new Point3(x, pts[0].Y, pts[3].Z, pts[1].W);
                        }
                        else
                        {
                            pta[1] = new Point3(x, pts[2].Y, pts[1].Z, pts[1].W);
                            pta[2] = new Point3(x, pts[2].Y, pts[3].Z, pts[1].W);

                        }
                    } 
                    else if (elevation < 0)
                    {
                        if ((azimuth >= -180 && azimuth < -90) ||
                            (azimuth >= 0 && azimuth < 90))
                        {
                            pta[1] = new Point3(x, pts[2].Y, pts[1].Z, pts[1].W);
                            pta[2] = new Point3(x, pts[2].Y, pts[3].Z, pts[1].W);

                        }
                        else
                        {
                            pta[1] = new Point3(x, pts[0].Y, pts[1].Z, pts[1].W);
                            pta[2] = new Point3(x, pts[0].Y, pts[3].Z, pts[1].W);
                        }
                    }
                    for (int i = 0; i < pta.Length; i++)
                    {
                        pta[i].Transform(m, form1, this);
                    }
                    g.DrawLine(aPen, pta[0].X,pta[0].Y,pta[1].X,pta[1].Y);
                    g.DrawLine(aPen, pta[1].X,pta[1].Y,pta[2].X,pta[2].Y);
                }

                // Draw y gridlines:
                if (IsYGrid)
                {
                    for (float y = YMin; y <= YMax; y = y + YTick)
                    {
                        pts = CoordinatesOfChartBox();
                        pta[0] = new Point3(pts[1].X, y, pts[1].Z, pts[1].W);
                        if (elevation >= 0)
                        {
                            if ((azimuth >= -180 && azimuth < -90) ||
                                (azimuth >= 0 && azimuth < 90))
                            {
                                pta[1] = new Point3(pts[2].X, y, pts[1].Z, pts[1].W);
                                pta[2] = new Point3(pts[2].X, y, pts[3].Z, pts[1].W);
                            }
                            else
                            {
                                pta[1] = new Point3(pts[0].X, y, pts[1].Z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, y, pts[3].Z, pts[1].W);
                            }
                        }
                        if (elevation < 0)
                        {
                            if ((azimuth >= -180 && azimuth < -90) ||
                                (azimuth >= 0 && azimuth < 90))
                            {
                                pta[1] = new Point3(pts[0].X, y, pts[1].Z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, y, pts[3].Z, pts[1].W);

                            }
                            else
                            {
                                pta[1] = new Point3(pts[2].X, y, pts[1].Z, pts[1].W);
                                pta[2] = new Point3(pts[2].X, y, pts[3].Z, pts[1].W);
                            }
                        }
                        for (int i = 0; i < pta.Length; i++)
                        {
                            pta[i].Transform(m, form1, this);
                        }
                        g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        g.DrawLine(aPen, pta[1].X, pta[1].Y, pta[2].X, pta[2].Y);
                    }
                }

                // Draw Z gridlines:
                if (IsZGrid)
                {
                    for (float z = ZMin; z <= ZMax; z = z + ZTick)
                    {
                        pts = CoordinatesOfChartBox();
                        pta[0] = new Point3(pts[2].X, pts[2].Y, z, pts[2].W);
                        if (elevation >= 0)
                        {
                            if ((azimuth >= -180 && azimuth < -90) ||
                                (azimuth >= 0 && azimuth < 90))
                            {
                                pta[1] = new Point3(pts[2].X, pts[0].Y, z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, pts[0].Y, z, pts[1].W);
                            }
                            else
                            {
                                pta[1] = new Point3(pts[0].X, pts[2].Y, z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, pts[1].Y, z, pts[1].W);
                            }
                        }
                        if (elevation < 0)
                        {
                            if ((azimuth >= -180 && azimuth < -90) ||
                                (azimuth >= 0 && azimuth < 90))
                            {
                                pta[1] = new Point3(pts[0].X, pts[2].Y, z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, pts[0].Y, z, pts[1].W);

                            }
                            else
                            {
                                pta[1] = new Point3(pts[2].X, pts[0].Y, z, pts[1].W);
                                pta[2] = new Point3(pts[0].X, pts[0].Y, z, pts[1].W);
                            }
                        }
                        for (int i = 0; i < pta.Length; i++)
                        {
                            pta[i].Transform(m, form1, this);
                        }
                        g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        g.DrawLine(aPen, pta[1].X, pta[1].Y, pta[2].X, pta[2].Y);
                    }
                }
            }
        }

        private void AddLabels(Graphics g)
        {
            Matrix3 m = Matrix3.AzimuthElevation(Elevation, Azimuth);
            Point3 pt = new Point3();
            Point3[] pts = CoordinatesOfChartBox();
            SolidBrush aBrush = new SolidBrush(LabelColor);
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;

            // Add x tick labels:
            float offset = (YMax - YMin) / 20;
            float labelSpace = offset;
            for (float x = XMin + XTick; x < XMax; x = x + XTick)
            {
                if (elevation >= 0)
                {
                    if (azimuth >= -90 && azimuth < 90)
                        labelSpace = -offset;
                }
                else if (elevation < 0)
                {
                    if ((azimuth >= -180 && azimuth < -90) ||
                        azimuth >= 90 && azimuth <= 180)
                        labelSpace = -offset;
                }
                pt = new Point3(x, pts[1].Y + labelSpace, pts[1].Z, pts[1].W);
                pt.Transform(m, form1, this);
                g.DrawString(x.ToString(), TickFont, aBrush,
                    new PointF(pt.X, pt.Y), sf);
            }

            // Add y tick labels:
            offset = (XMax - XMin) / 20;
            labelSpace = offset;
            for (float y = YMin + yTick; y < YMax; y = y + YTick)
            {
                pts = CoordinatesOfChartBox();
                if (elevation >= 0)
                {
                    if (azimuth >= -180 && azimuth < 0)
                        labelSpace = -offset;
                }
                else if (elevation < 0)
                {
                    if (azimuth >= 0 && azimuth < 180)
                        labelSpace = -offset;
                }
                pt = new Point3(pts[1].X + labelSpace, y, pts[1].Z, pts[1].W);
                pt.Transform(m, form1, this);
                g.DrawString(y.ToString(), TickFont, aBrush,
                    new PointF(pt.X, pt.Y), sf);

            }

            // Add z tick labels:
            float xoffset = (XMax - XMin) / 30.0f;
            float yoffset = (YMax - YMin) / 15.0f;
            float xlabelSpace = xoffset;
            float ylabelSpace = yoffset;
            SizeF s = g.MeasureString("A", TickFont);
            for (float z = ZMin; z <= ZMax; z = z + ZTick)
            {
                sf.Alignment = StringAlignment.Far;
                pts = CoordinatesOfChartBox();
                if (elevation >= 0)
                {
                    if (azimuth >= -180 && azimuth < -90)
                    {
                        xlabelSpace = 0;
                        ylabelSpace = yoffset;
                    }
                    else if (azimuth >= -90 && azimuth < 0)
                    {
                        xlabelSpace = xoffset;
                        ylabelSpace = 0;
                    }
                    else if (azimuth >= 0 && azimuth < 90)
                    {
                        xlabelSpace = 0;
                        ylabelSpace = -yoffset;
                    }
                    else if (azimuth >= 90 && azimuth <= 180)
                    {
                        xlabelSpace = -xoffset;
                        ylabelSpace = 0;
                    }
                }
                else if (elevation < 0)
                {
                    if (azimuth >= -180 && azimuth < -90)
                    {
                        ylabelSpace = 0;
                        xlabelSpace = xoffset;
                    }
                    else if (azimuth >= -90 && azimuth < 0)
                    {
                        ylabelSpace = -yoffset;
                        xlabelSpace = 0;
                    }
                    else if (azimuth >= 0 && azimuth < 90)
                    {
                        ylabelSpace = 0;
                        xlabelSpace = -xoffset;
                    }
                    else if (azimuth >= 90 && azimuth <= 180)
                    {
                        ylabelSpace = yoffset;
                        xlabelSpace = 0;
                    }
                }

                pt = new Point3(pts[2].X + ylabelSpace,
                    pts[2].Y + xlabelSpace, z, pts[2].W);
                pt.Transform(m, form1, this);
                g.DrawString(z.ToString(), TickFont, aBrush,
                    new PointF(pt.X - labelSpace, pt.Y - s.Height / 2), sf);
            }

            // Add Title:
            sf.Alignment = StringAlignment.Center;
            aBrush = new SolidBrush(TitleColor);
            if (Title != "No Title")
            {
                g.DrawString(Title, titleFont, aBrush,
                    new PointF(form1.PlotPanel.Width / 2, form1.Height / 30), sf);
            }
            aBrush.Dispose();

            // Add x axis label:
            offset = (YMax - YMin) / 3;
            labelSpace = offset;
            sf.Alignment = StringAlignment.Center;
            aBrush = new SolidBrush(LabelColor);
            float offset1 = (XMax - XMin) / 10;
            float xc = offset1;
            if (elevation >= 0)
            {
                if (azimuth >= -90 && azimuth < 90)
                    labelSpace = -offset;
                if (azimuth >= 0 && azimuth <= 180)
                    xc = -offset1;
            }
            else if (elevation < 0)
            {
                if ((azimuth >= -180 && azimuth < -90) ||
                    azimuth >= 90 && azimuth <= 180)
                    labelSpace = -offset;
                if (Azimuth >= -180 && azimuth <= 0)
                    xc = -offset1;
            }
            Point3[] pta = new Point3[2];
            pta[0] = new Point3(XMin, pts[1].Y + labelSpace, pts[1].Z, pts[1].W);
            pta[1] = new Point3((XMin + XMax) / 2 - xc, pts[1].Y + labelSpace,
                pts[1].Z, pts[1].W);
            pta[0].Transform(m, form1, this);
            pta[1].Transform(m, form1, this);
            float theta = (float)Math.Atan((pta[1].Y - pta[0].Y) / (pta[1].X - pta[0].X));
            theta = theta * 180 / (float)Math.PI;
            GraphicsState gs = g.Save();
            g.TranslateTransform(pta[1].X, pta[1].Y);
            g.RotateTransform(theta);
            g.DrawString(XLabel, LabelFont, aBrush,
                new PointF(0, 0), sf);
            g.Restore(gs);

            // Add y axis label:
            offset = (XMax - XMin) / 3;
            offset1 = (YMax - YMin) / 5;
            labelSpace = offset;
            float yc = YTick;
            if (elevation >= 0)
            {
                if (azimuth >= -180 && azimuth < 0)
                    labelSpace = -offset;
                if (azimuth >= -90 && azimuth <= 90)
                    yc = -offset1;
            }
            else if (elevation < 0)
            {
                yc = -offset1;
                if (azimuth >= 0 && azimuth < 180)
                    labelSpace = -offset;
                if (azimuth >= -90 && azimuth <= 90)
                    yc = offset1;
            }
            pta[0] = new Point3(pts[1].X + labelSpace, YMin, pts[1].Z, pts[1].W);
            pta[1] = new Point3(pts[1].X + labelSpace, (YMin + YMax)/2 + yc, pts[1].Z, pts[1].W);
            pta[0].Transform(m, form1, this);
            pta[1].Transform(m, form1, this);
            theta = (float)Math.Atan((pta[1].Y - pta[0].Y) / (pta[1].X - pta[0].X));
            theta = theta * 180 / (float)Math.PI;
            gs = g.Save();
            g.TranslateTransform(pta[1].X, pta[1].Y);
            g.RotateTransform(theta);
            g.DrawString(YLabel, LabelFont, aBrush,
                new PointF(0, 0), sf);
            g.Restore(gs);

            // Add z axis labels:
            float zticklength = 10;
            labelSpace = -1.3f * offset;
            offset1 = (ZMax - ZMin) / 8;
            float zc = -offset1;
            for (float z = ZMin; z < ZMax; z = z + ZTick)
            {
                SizeF size = g.MeasureString(z.ToString(), TickFont);
                if (zticklength < size.Width)
                    zticklength = size.Width;
            }
            float zlength = -zticklength;
            if (elevation >= 0)
            {
                if (azimuth >= -180 && azimuth < -90)
                {
                    zlength = -zticklength;
                    labelSpace = -1.3f * offset;
                    zc = -offset1;
                }
                else if (azimuth >= -90 && azimuth < 0)
                {
                    zlength = zticklength;
                    labelSpace = 2 * offset / 3;
                    zc = offset1;
                }
                else if (azimuth >= 0 && azimuth < 90)
                {
                    zlength = zticklength;
                    labelSpace = 2 * offset / 3;
                    zc = -offset1;
                }
                else if (azimuth >= 90 && azimuth <= 180)
                {
                    zlength = -zticklength;
                    labelSpace = -1.3f * offset;
                    zc = offset1;
                }
            }
            else if (elevation < 0)
            {
                if (azimuth >= -180 && azimuth < -90)
                {
                    zlength = -zticklength;
                    labelSpace = -1.3f * offset;
                    zc = offset1;
                }
                else if (azimuth >= -90 && azimuth < 0)
                {
                    zlength = zticklength;
                    labelSpace = 2 * offset / 3;
                    zc = -offset1;
                }
                else if (azimuth >= 0 && azimuth < 90)
                {
                    zlength = zticklength;
                    labelSpace = 2 * offset / 3;
                    zc = offset1;
                }
                else if (azimuth >= 90 && azimuth <= 180)
                {
                    zlength = -zticklength;
                    labelSpace = -1.3f * offset;
                    zc = -offset1;
                }
            }
            pta[0] = new Point3(pts[2].X - labelSpace, pts[2].Y,
                (ZMin + ZMax) / 2 + zc, pts[2].W);
            pta[0].Transform(m, form1, this);
            gs = g.Save();
            g.TranslateTransform(pta[0].X - zlength, pta[0].Y);
            g.RotateTransform(270);
            g.DrawString(ZLabel, LabelFont, aBrush,
                new PointF(0, 0), sf);
            g.Restore(gs);
        }
    }
}