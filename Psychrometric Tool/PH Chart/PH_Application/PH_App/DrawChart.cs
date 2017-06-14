using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;

namespace PH_App
{
    public class DrawChart
    {
        private Form1_jack_xu form1;
        private ChartTypeEnum chartType;
        private int[,] cmap;
        private bool isColorMap = true;
        private bool isHiddenLine = false;
        private bool isInterp = false;
        private int numberInterp = 2;
        public int numberContours = 10;
        private SliceEnum xyzSlice = SliceEnum.XSlice;
        private float sliceLocation = 0;

        public DrawChart(Form1_jack_xu fm1)
        {
            form1 = fm1;
        }

        public float SliceLocation
        {
            get { return sliceLocation; }
            set { sliceLocation = value; }
        }

        public int NumberContours
        {
            get { return numberContours; }
            set { numberContours = value; }
        }

        public int NumberInterp
        {
            get { return numberInterp; }
            set { numberInterp = value; }
        }

        public bool IsInterp
        {
            get { return isInterp; }
            set { isInterp = value; }
        }

        public bool IsColorMap
        {
            get { return isColorMap; }
            set { isColorMap = value; }
        }

        public bool IsHiddenLine
        {
            get { return isHiddenLine; }
            set { isHiddenLine = value; }
        }

        public int[,] CMap
        {
            get { return cmap; }
            set { cmap = value; }
        }

        public ChartTypeEnum ChartType
        {
            get { return chartType; }
            set { chartType = value; }
        }

        public SliceEnum XYZSlice
        {
            get { return xyzSlice; }
            set { xyzSlice = value; }
        }

        public enum SliceEnum
        {
            XSlice,
            YSlice,
            ZSlice
        }

        public enum ChartTypeEnum
        {
            Line,
            Mesh,
            MeshZ,
            Waterfall,
            Surface,
            XYColor,
            Contour,
            FillContour,
            MeshContour,
            SurfaceContour,
            SurfaceFillContour,
            Slice,
            Bar3D,
        }

        public void AddChart(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            switch (ChartType)
            {
                case ChartTypeEnum.Line:
                    AddLine(g, ds, cs);
                    break;
                case ChartTypeEnum.Mesh:
                    AddMesh(g, ds, cs);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.MeshZ:
                    AddMeshZ(g, ds, cs);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.Waterfall:
                    AddWaterfall(g, ds, cs);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.Surface:
                    AddSurface(g, ds, cs, cs2d);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.XYColor:
                    AddXYColor(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.Contour:
                    AddContour(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.FillContour:
                    AddXYColor(g, ds, cs, cs2d);
                    AddContour(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.MeshContour:
                    AddContour3D(g, ds, cs, cs2d);
                    AddMesh(g, ds, cs);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.SurfaceContour:
                    AddContour3D(g, ds, cs, cs2d);
                    AddSurface(g, ds, cs, cs2d);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.SurfaceFillContour:
                    AddXYColor3D(g, ds, cs, cs2d);
                    AddContour3D(g, ds, cs, cs2d);
                    AddSurface(g, ds, cs, cs2d);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.Slice:
                    AddSlice(g, ds, cs, cs2d);
                    AddColorBar(g, ds, cs, cs2d);
                    break;
                case ChartTypeEnum.Bar3D:
                    AddBar3D(g, ds, cs, cs2d);
                    AddColorBar(g,ds,cs,cs2d);
                    break;
            }
        }

        private void AddLine(Graphics g, DataSeries ds, ChartStyle cs)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            Point3[] pts = new Point3[ds.PointList.Count];

            // Find zmin and zmax values, then perform transformation on points:
            float zmin = 0;
            float zmax = 0;
            for (int i = 0; i < pts.Length; i++)
            {
                pts[i] = (Point3)ds.PointList[i];
                zmin = Math.Min(zmin, pts[i].Z);
                zmax = Math.Max(zmax, pts[i].Z);
                pts[i].Transform(m, form1, cs);
            }

            // Draw line:
            if (ds.LineStyle.IsVisible == true)
            {
                for (int i = 1; i < pts.Length; i++)
                {
                    Color color = AddColor(cs, pts[i], zmin, zmax);
                    if (IsColorMap)
                    {
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawLine(aPen, pts[i - 1].X, pts[i - 1].Y, pts[i].X, pts[i].Y);
                }
            }
            aPen.Dispose();
        }

        private void AddMesh(Graphics g, DataSeries ds, ChartStyle cs)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            PointF[] pta = new PointF[4];
            Point3[,] pts = ds.PointArray;

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();
            
            // Perform transformations on points:
            for (int i = 0; i < pts.GetLength(0); i++)
            {
                for (int j = 0; j < pts.GetLength(1); j++)
                {
                    pts[i, j].Transform(m, form1, cs);
                }
            }

            // Draw color mesh:
            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    int ii = i;
                    if (cs.Azimuth >= -180 && cs.Azimuth < 0)
                    {
                        ii = pts.GetLength(0) - 2 - i;
                    }
                    pta[0] = new PointF(pts[ii, j].X, pts[ii, j].Y);
                    pta[1] = new PointF(pts[ii, j + 1].X, pts[ii, j + 1].Y);
                    pta[2] = new PointF(pts[ii + 1, j + 1].X, pts[ii + 1, j + 1].Y);
                    pta[3] = new PointF(pts[ii + 1, j].X, pts[ii + 1, j].Y);
                    if (!IsHiddenLine)
                    {
                        g.FillPolygon(aBrush, pta);
                    }
                    if (IsColorMap)
                    {
                        Color color = AddColor(cs, pts[ii, j], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawPolygon(aPen, pta);
                }
            }
            aPen.Dispose();
            aBrush.Dispose();
        }

        private void AddMeshZ(Graphics g, DataSeries ds, ChartStyle cs)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            PointF[] pta = new PointF[4];
            Point3[,] pts = ds.PointArray;
            Point3[,] pts1 = new Point3[pts.GetLength(0), pts.GetLength(1)];
            Color color;

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            for (int i = 0; i < pts.GetLength(0); i++)
            {
                for (int j = 0; j < pts.GetLength(1); j++)
                {
                    // Make a deep copy the points array:
                    pts1[i, j] = new Point3(pts[i, j].X, pts[i, j].Y, pts[i, j].Z, 1);
                    // Perform transformations on points:
                    pts[i, j].Transform(m, form1, cs);
                }
            }

            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    int ii = i;
                    if (cs.Azimuth >= -180 && cs.Azimuth < 0)
                    {
                        ii = pts.GetLength(0) - 2 - i;
                    }
                    pta[0] = new PointF(pts[ii, j].X, pts[ii, j].Y);
                    pta[1] = new PointF(pts[ii, j + 1].X, pts[ii, j + 1].Y);
                    pta[2] = new PointF(pts[ii + 1, j + 1].X, pts[ii + 1, j + 1].Y);
                    pta[3] = new PointF(pts[ii + 1, j].X, pts[ii + 1, j].Y);
                    g.FillPolygon(aBrush, pta);
                    if (IsColorMap)
                    {
                        color = AddColor(cs, pts[ii, j], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawPolygon(aPen, pta);
                }
            }

            Point3[] pt3 = new Point3[4];
            for (int i = 0; i < pts1.GetLength(0); i++)
            {
                int jj = pts1.GetLength(0) - 1;
                if (cs.Elevation >= 0)
                {
                    if (cs.Azimuth >= -90 && cs.Azimuth <= 90)
                        jj = 0;
                }
                else if (cs.Elevation < 0)
                {
                    jj = 0;
                    if (cs.Azimuth >= -90 && cs.Azimuth <= 90)
                        jj = pts1.GetLength(0) - 1;
                }

                if (i < pts1.GetLength(0) - 1)
                {
                    pt3[0] = new Point3(pts1[i, jj].X, pts1[i, jj].Y, pts1[i, jj].Z, 1);
                    pt3[1] = new Point3(pts1[i + 1, jj].X, pts1[i + 1, jj].Y, pts1[i + 1, jj].Z, 1);
                    pt3[2] = new Point3(pts1[i + 1, jj].X, pts1[i + 1, jj].Y, cs.ZMin, 1);
                    pt3[3] = new Point3(pts1[i, jj].X, pts1[i, jj].Y, cs.ZMin, 1);
                    for (int k = 0; k < 4; k++)
                        pt3[k].Transform(m, form1, cs);
                    pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                    pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                    pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                    pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                    g.FillPolygon(aBrush, pta);
                    if (IsColorMap)
                    {
                        color = AddColor(cs, pt3[0], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawPolygon(aPen, pta);
                }
            }
            for (int j = 0; j < pts1.GetLength(1); j++)
            {
                int ii = 0;
                if (cs.Elevation >= 0)
                {
                    if (cs.Azimuth >= 0 && cs.Azimuth <= 180)
                        ii = pts1.GetLength(1) - 1;
                }
                else if (cs.Elevation < 0)
                {
                    if (cs.Azimuth >= -180 && cs.Azimuth <= 0)
                        ii = pts1.GetLength(1) - 1;
                }
                if (j < pts1.GetLength(1) - 1)
                {
                    pt3[0] = new Point3(pts1[ii, j].X, pts1[ii, j].Y, pts1[ii, j].Z, 1);
                    pt3[1] = new Point3(pts1[ii, j + 1].X, pts1[ii, j + 1].Y, pts1[ii, j + 1].Z, 1);
                    pt3[2] = new Point3(pts1[ii, j + 1].X, pts1[ii, j + 1].Y, cs.ZMin, 1);
                    pt3[3] = new Point3(pts1[ii, j].X, pts1[ii, j].Y, cs.ZMin, 1);
                    for (int k = 0; k < 4; k++)
                        pt3[k].Transform(m, form1, cs);
                    pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                    pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                    pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                    pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                    g.FillPolygon(aBrush, pta);
                    if (IsColorMap)
                    {
                        color = AddColor(cs, pt3[0], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawPolygon(aPen, pta);
                }
            }
            aPen.Dispose();
            aBrush.Dispose();
        }

        private void AddWaterfall(Graphics g, DataSeries ds, ChartStyle cs)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            Point3[,] pts = ds.PointArray;
            Point3[] pt3 = new Point3[pts.GetLength(0) + 2];
            PointF[] pta = new PointF[pts.GetLength(0) + 2];
            Color color;

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            for (int j = 0; j < pts.GetLength(1); j++)
            {
                int jj = j;
                if (cs.Elevation >= 0)
                {
                    if (cs.Azimuth >= -90 && cs.Azimuth < 90)
                    {
                        jj = pts.GetLength(1) - 1 - j;
                    }
                }
                else if (cs.Elevation < 0)
                {
                    jj = pts.GetLength(1) - 1 - j;
                    if (cs.Azimuth >= -90 && cs.Azimuth < 90)
                    {
                        jj = j;
                    }
                }
                for (int i = 0; i < pts.GetLength(0); i++)
                {
                    pt3[i + 1] = pts[i, jj];
                    if (i == 0)
                    {
                        pt3[0] = new Point3(pt3[i + 1].X, pt3[i + 1].Y, cs.ZMin, 1);
                    }
                    if (i == pts.GetLength(0) - 1)
                    {
                        pt3[pts.GetLength(0) + 1] = new Point3(pt3[i + 1].X,
                            pt3[i + 1].Y, cs.ZMin, 1);
                    }
                }

                for (int i = 0; i < pt3.Length; i++)
                {
                    pt3[i].Transform(m, form1, cs);
                    pta[i] = new PointF(pt3[i].X, pt3[i].Y);
                }
                g.FillPolygon(aBrush, pta);

                for (int i = 1; i < pt3.Length; i++)
                {
                    if (IsColorMap)
                    {
                        color = AddColor(cs, pt3[i], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    g.DrawLine(aPen, pta[i - 1], pta[i]);
                }
            } 
            aPen.Dispose();
            aBrush.Dispose();
        }

        private void AddSurface(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            PointF[] pta = new PointF[4];
            Point3[,] pts = ds.PointArray;
            Point3[,] pts1 = new Point3[pts.GetLength(0), pts.GetLength(1)];

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            // Perform transformation on points:
            for (int i = 0; i < pts.GetLength(0); i++)
            {
                for (int j = 0; j < pts.GetLength(1); j++)
                {
                    // Make a deep copy the points array:
                    pts1[i, j] = new Point3(pts[i, j].X, pts[i, j].Y, pts[i, j].Z, 1);
                    // Perform transformation on points:
                    pts[i, j].Transform(m, form1, cs);
                }
            }

            // Draw surface:
            if (!IsInterp)
            {
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        int ii = i;
                        if (cs.Azimuth >= -180 && cs.Azimuth < 0)
                        {
                            ii = pts.GetLength(0) - 2 - i;
                        }
                        pta[0] = new PointF(pts[ii, j].X, pts[ii, j].Y);
                        pta[1] = new PointF(pts[ii, j + 1].X, pts[ii, j + 1].Y);
                        pta[2] = new PointF(pts[ii + 1, j + 1].X, pts[ii + 1, j + 1].Y);
                        pta[3] = new PointF(pts[ii + 1, j].X, pts[ii + 1, j].Y);
                        Color color = AddColor(cs, pts[ii, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }

            // Draw refined surface:
            else if (IsInterp)
            {
               for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        int ii = i;
                        if (cs.Azimuth >= -180 && cs.Azimuth < 0)
                         {
                             ii = pts.GetLength(0) - 2 - i;
                         }
                        Point3[] points = new Point3[4];

                        points[0] = pts1[ii, j];
                        points[1] = pts1[ii, j + 1];
                        points[2] = pts1[ii + 1, j + 1];
                        points[3] = pts1[ii + 1, j];

                        Interp(g, cs, cs2d, m, points, zmin, zmax, 1);

                        pta[0] = new PointF(pts[ii, j].X, pts[ii, j].Y);
                        pta[1] = new PointF(pts[ii, j + 1].X, pts[ii, j + 1].Y);
                        pta[2] = new PointF(pts[ii + 1, j + 1].X, pts[ii + 1, j + 1].Y);
                        pta[3] = new PointF(pts[ii + 1, j].X, pts[ii + 1, j].Y);

                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }
        }

        private void AddXYColor(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            PointF[] pta = new PointF[4];
            Point3[,] pts = ds.PointArray;
            Matrix3 m = new Matrix3();

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            // Draw surface on the XY plane:
            if (!IsInterp)
            {
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        pta[0] = cs2d.Point2D(new PointF(pts[i, j].X, pts[i, j].Y), cs);
                        pta[1] = cs2d.Point2D(new PointF(pts[i, j + 1].X, pts[i, j + 1].Y), cs);
                        pta[2] = cs2d.Point2D(new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y), cs);
                        pta[3] = cs2d.Point2D(new PointF(pts[i + 1, j].X, pts[i + 1, j].Y), cs);
                        Color color = AddColor(cs, pts[i, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }

            // Draw refined surface:
            else if (IsInterp)
            {
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        Point3[] points = new Point3[4];

                        points[0] = pts[i, j];
                        points[1] = pts[i, j + 1];
                        points[2] = pts[i + 1, j + 1];
                        points[3] = pts[i + 1, j];

                        Interp(g, cs, cs2d, m, points, zmin, zmax, 2);

                        pta[0] = cs2d.Point2D(new PointF(pts[i, j].X, pts[i, j].Y),cs);
                        pta[1] = cs2d.Point2D(new PointF(pts[i, j + 1].X, pts[i, j + 1].Y),cs);
                        pta[2] = cs2d.Point2D(new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y), cs);
                        pta[3] = cs2d.Point2D(new PointF(pts[i + 1, j].X, pts[i + 1, j].Y), cs);

                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }
        }

        private void AddXYColor3D(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            PointF[] pta = new PointF[4];
            Point3[,] pts = ds.PointArray;
            Point3[,] pts1 = new Point3[pts.GetLength(0), pts.GetLength(1)];
            Point3[,] pts2 = new Point3[pts.GetLength(0), pts.GetLength(1)];
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            // Perform transformation on points:
            for (int i = 0; i < pts.GetLength(0); i++)
            {
                for (int j = 0; j < pts.GetLength(1); j++)
                {
                    // Make a deep copy the points array:
                    pts1[i, j] = new Point3(pts[i, j].X, pts[i, j].Y, cs.ZMin, 1);
                    pts2[i, j] = new Point3(pts[i, j].X, pts[i, j].Y, cs.ZMin, 1);
                    pts1[i, j].Transform(m, form1, cs);
                }
            }

            // Draw surface on the XY plane:
            if (!IsInterp)
            {
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        pta[0] = new PointF(pts1[i, j].X, pts1[i, j].Y);
                        pta[1] = new PointF(pts1[i, j + 1].X, pts1[i, j + 1].Y);
                        pta[2] = new PointF(pts1[i + 1, j + 1].X, pts1[i + 1, j + 1].Y);
                        pta[3] = new PointF(pts1[i + 1, j].X, pts1[i + 1, j].Y);
                        Color color = AddColor(cs, pts[i, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            //g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }

            // Draw refined surface:
            else if (IsInterp)
            {
                for (int i = 0; i < pts1.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts1.GetLength(1) - 1; j++)
                    {
                        Point3[] points = new Point3[4];
                        points[0] = pts[i, j];
                        points[1] = pts[i, j + 1];
                        points[2] = pts[i + 1, j + 1];
                        points[3] = pts[i + 1, j];
                        Interp(g, cs, cs2d, m, points, zmin, zmax, 3);
                    }
                }
            }
        }

        private void AddContour(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.White);
            PointF[] pta = new PointF[2];
            Point3[,] pts = ds.PointArray;
            Matrix3 m = new Matrix3();

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();
            float[] zlevels = new float[numberContours];
            for (int i = 0; i < numberContours; i++)
            {
                zlevels[i] = zmin + i * (zmax - zmin) / (numberContours - 1);
            }

            int i0, i1, i2, j0, j1, j2;
            float zratio = 1;
            // Draw contour on the XY plane:
            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    if (IsColorMap && ChartType != ChartTypeEnum.FillContour && 
                        ChartType != ChartTypeEnum.SurfaceFillContour)
                    {
                        Color color = AddColor(cs, pts[i, j], zmin, zmax);
                        aPen = new Pen(color, ds.LineStyle.Thickness);
                        aPen.DashStyle = ds.LineStyle.Pattern;
                    }
                    for (int k = 0; k < numberContours; k++)
                    {
                        // Left triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i;
                        j1 = j + 1;
                        i2 = i + 1;
                        j2 = j + 1;
                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) / 
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X,
                                (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y), cs);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = cs2d.Point2D(new PointF((1- zratio) * pts[i1, j1].X +
                                zratio * pts[i2, j2].X, pts[i1, j1].Y), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF((1-zratio) * pts[i0, j0].X + 
                                zratio * pts[i2, j2].X,
                               (1-zratio) * pts[i0, j0].Y + zratio * pts[i2, j2].Y), cs);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                               (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = cs2d.Point2D(new PointF((1-zratio) * pts[i1, j1].X +
                                zratio * pts[i2, j2].X, pts[i1, j1].Y), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X,
                                (1-zratio)* pts[i0, j0].Y  + zratio * pts[i1, j1].Y), cs);
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[1] = cs2d.Point2D(new PointF(pts[i0, j0].X *(1-zratio) +
                                pts[i2, j2].X * zratio, pts[i0, j0].Y * (1-zratio) + 
                                pts[i2,j2].Y *zratio), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }

                        // right triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i+1;
                        j1 = j;
                        i2 = i + 1;
                        j2 = j + 1;
                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                                pts[i1, j1].X * zratio, pts[i0, j0].Y), cs);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = cs2d.Point2D(new PointF(pts[i1, j1].X,
                                pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X *(1-zratio) + 
                                pts[i2, j2].X *zratio, pts[i0, j0].Y *(1-zratio) + 
                                pts[i2, j2].Y *zratio), cs);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = cs2d.Point2D(new PointF(pts[i1, j1].X,
                                pts[i1, j1].Y *(1-zratio) + pts[i2, j2].Y *zratio), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X *(1-zratio) + 
                                pts[i1,j1].X * zratio, pts[i0, j0].Y), cs);
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[1] = cs2d.Point2D(new PointF(pts[i0, j0].X *(1-zratio) +
                                pts[i2, j2].X *zratio, pts[i0, j0].Y * (1-zratio) + 
                                pts[i2, j2].Y *zratio), cs);
                            g.DrawLine(aPen, pta[0], pta[1]);
                        }
                    }
                }
            }
        }


        //==============================My contour code for plotting ==================//


        int prevZLabelCheck = 0;
        int changingZLevel = 0;
        int prevIIndex = 0;
        int changingIIndex = 0;

        int flagForIncSeries = 0; //--This one makes sure series is changed only once 
        ArrayList arrayListSeries = new ArrayList();
        string selectedSeriesName = "";
        public List<DataTypeForPointList> AddContour_MyCustomFxn(DataSeries ds)
        {
            // Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            //aPen.DashStyle = ds.LineStyle.Pattern;
            //SolidBrush aBrush = new SolidBrush(Color.White);
            indexForSeries = 0;
            arrayListSeries.Clear();
            // arrayListSeries = allSeriesInChart;
            // selectedSeriesName = arrayListSeries[0].ToString();
            PointF[] pta = new PointF[2];
            Point3[,] pts = ds.PointArray;
            Matrix3 m = new Matrix3();


            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();
            float[] zlevels = new float[numberContours];
            for (int i = 0; i < numberContours; i++)
            {
                zlevels[i] = zmin + i * (zmax - zmin) / (numberContours - 1);
            }

            int i0, i1, i2, j0, j1, j2;
            float zratio = 1;
            string s = null;

            prevZLabelCheck = 0;
            changingZLevel = 0;
            prevIIndex = 0;
            changingIIndex = 0;
            flagForIncSeries = 0; //--This one makes sure series is changed only once 
            listStoredPoints.Clear();

            // Draw contour on the XY plane:
            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                changingIIndex = i;
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {

                    for (int k = 0; k < numberContours; k++)
                    {
                        // Left triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i;
                        j1 = j + 1;
                        i2 = i + 1;
                        j2 = j + 1;

                        changingZLevel = k;


                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);

                            //pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X,
                            //    (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y), cs);
                            pta[0].X = pts[i0, j0].X;
                            pta[0].Y = (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y;

                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            //pta[1] = cs2d.Point2D(new PointF((1 - zratio) * pts[i1, j1].X +
                            //    zratio * pts[i2, j2].X, pts[i1, j1].Y), cs);
                            pta[1].X = (1 - zratio) * pts[i1, j1].X + zratio * pts[i2, j2].X;
                            pta[1].Y = pts[i1, j1].Y;


                            //--Calling the function
                            //PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);

                            //g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            //pta[0] = cs2d.Point2D(new PointF((1 - zratio) * pts[i0, j0].X +
                            //    zratio * pts[i2, j2].X,
                            //   (1 - zratio) * pts[i0, j0].Y + zratio * pts[i2, j2].Y), cs);

                            pta[0].X = (1 - zratio) * pts[i0, j0].X + zratio * pts[i2, j2].X;
                            pta[0].Y = (1 - zratio) * pts[i0, j0].Y + zratio * pts[i2, j2].Y;


                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                               (pts[i2, j2].Z - pts[i1, j1].Z);
                            //pta[1] = cs2d.Point2D(new PointF((1 - zratio) * pts[i1, j1].X +
                            //    zratio * pts[i2, j2].X, pts[i1, j1].Y), cs);

                            pta[1].X = (1 - zratio) * pts[i1, j1].X + zratio * pts[i2, j2].X;
                            pta[1].Y = pts[i1, j1].Y;

                            s += "zlevel k = " + k + ",i = " + i + ",j=" + j + "pts x1,y1=(" + pta[0].X + "," + pta[0].Y + "),\n";

                            //--Calling the function
                            //PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);
                            // g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            //pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X,
                            //    (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y), cs);

                            pta[0].X = pts[i0, j0].X;
                            pta[0].Y = (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y;


                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            //pta[1] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                            //    pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                            //    pts[i2, j2].Y * zratio), cs);

                            pta[1].X = pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio;
                            pta[1].Y = pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio;

                            //--Calling the function
                            //PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);
                            // g.DrawLine(aPen, pta[0], pta[1]);
                        }

                        // right triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i + 1;
                        j1 = j;
                        i2 = i + 1;
                        j2 = j + 1;
                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            //pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                            //    pts[i1, j1].X * zratio, pts[i0, j0].Y), cs);

                            pta[0].X = pts[i0, j0].X * (1 - zratio) + pts[i1, j1].X * zratio;
                            pta[0].Y = pts[i0, j0].Y;



                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            //pta[1] = cs2d.Point2D(new PointF(pts[i1, j1].X,
                            //    pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio), cs);

                            pta[1].X = pts[i1, j1].X;
                            pta[1].Y = pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio;

                            //--Calling the function
                            // PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);
                            // g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            //pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                            //    pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                            //    pts[i2, j2].Y * zratio), cs);

                            pta[0].X = pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio;
                            pta[0].Y = pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio;



                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            //pta[1] = cs2d.Point2D(new PointF(pts[i1, j1].X,
                            //    pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio), cs);

                            pta[1].X = pts[i1, j1].X;
                            pta[1].Y = pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio;

                            //--Calling the function
                            //PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);
                            // g.DrawLine(aPen, pta[0], pta[1]);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            //pta[0] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                            //    pts[i1, j1].X * zratio, pts[i0, j0].Y), cs);


                            pta[0].X = pts[i0, j0].X * (1 - zratio) + pts[i1, j1].X * zratio;
                            pta[0].Y = pts[i0, j0].Y;



                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            //pta[1] = cs2d.Point2D(new PointF(pts[i0, j0].X * (1 - zratio) +
                            //    pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                            //    pts[i2, j2].Y * zratio), cs);

                            pta[1].X = pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio;
                            pta[1].Y = pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio;


                            s += "zlevel k = " + k + ",i = " + i + ",j=" + j + "pts x1,y1=(" + pta[0].X + "," + pta[0].Y + "),\n";
                            //--Calling the function
                            //PlotLogicFxnForContour(pta, i, k, frm1);
                            PlotLogicFxnForContour(pta, i, k);
                            //g.DrawLine(aPen, pta[0], pta[1]);
                        }
                    }
                }
            }

            return listStoredPoints;
        }
        public class DataTypeForPointList
        {
            public double x1 { get; set; }
            public double y1 { get; set; }
            public double x2 { get; set; }
            public double y2 { get; set; }

            public int zlevel { get; set; }
        }
        List<DataTypeForPointList> listStoredPoints = new List<DataTypeForPointList>();
        int indexForSeries = 0;
        public void PlotLogicFxnForContour(PointF[] pta, int i, int k)
        {


            //--x-asix range 0-4000 and  y -axis range0-100 so 
            if ((pta[0].X > 0 && pta[0].X < 4000) && (pta[0].Y > 0 && pta[0].Y < 100))
            {

                if ((pta[1].X > 0 && pta[1].X < 4000) && (pta[1].Y > 0 && pta[1].Y < 100))
                {


                    if (changingZLevel == prevZLabelCheck && changingIIndex == prevIIndex)
                    {
                        //--We have match the index and z- level so put the point in the series.
                        flagForIncSeries = 1;
                        //--Now lets add the values 
                        //frm1.ph_chart.Series[selectedSeriesName].Points.AddXY(pta[0].X, pta[0].Y);
                        //frm1.ph_chart.Series[selectedSeriesName].Points.AddXY(pta[1].X, pta[1].Y);
                        listStoredPoints.Add(new DataTypeForPointList
                        {
                            x1 = pta[0].X,
                            y1 = pta[0].Y,
                            x2 = pta[1].X,
                            y2 = pta[1].Y,
                            zlevel = changingZLevel

                        });


                    }
                    else if (changingZLevel > prevZLabelCheck && changingIIndex > prevIIndex)
                    {
                        //==We need to change the series to new one
                        //--we should only increment the index once if previous value is filled 
                        if (flagForIncSeries == 1)
                        {
                            //==Then we increment the series or change series and plot the points.
                            //selectedSeriesName = arrayListSeries[indexForSeries++].ToString();

                            //frm1.ph_chart.Series[selectedSeriesName].Points.AddXY(pta[0].X, pta[0].Y);
                            //frm1.ph_chart.Series[selectedSeriesName].Points.AddXY(pta[1].X, pta[1].Y);
                            listStoredPoints.Add(new DataTypeForPointList
                            {
                                x1 = pta[0].X,
                                y1 = pta[0].Y,
                                x2 = pta[1].X,
                                y2 = pta[1].Y,
                                zlevel = changingZLevel

                            });



                            flagForIncSeries = 0;
                        }

                    }

                    prevZLabelCheck = k;
                    prevIIndex = i;


                }
            }


        }


        //===========End of my contour code for plotting =============================//




        private void AddContour3D(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            Point3[] pta = new Point3[2];
            Point3[,] pts = ds.PointArray;
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();
            float[] zlevels = new float[numberContours];
            for (int i = 0; i < numberContours; i++)
            {
                zlevels[i] = zmin + i * (zmax - zmin) / (numberContours - 1);
            }

            int i0, i1, i2, j0, j1, j2;
            float zratio = 1;
            // Draw contour on the XY plane:
            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    for (int k = 0; k < numberContours; k++)
                    {
                        if (IsColorMap && ChartType != ChartTypeEnum.FillContour &&
                            ChartType != ChartTypeEnum.SurfaceFillContour)
                        {
                            Color color = AddColor(cs, pts[i, j], zmin, zmax);
                            aPen = new Pen(color, ds.LineStyle.Thickness);
                            aPen.DashStyle = ds.LineStyle.Pattern;
                        }
 
                        // Left triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i;
                        j1 = j + 1;
                        i2 = i + 1;
                        j2 = j + 1;
                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = new Point3(pts[i0, j0].X, (1 - zratio) *
                                pts[i0, j0].Y + zratio * pts[i1, j1].Y, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = new Point3((1 - zratio) * pts[i1, j1].X +
                                zratio * pts[i2, j2].X, pts[i1, j1].Y, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[0] = new Point3((1 - zratio) * pts[i0, j0].X +
                                zratio * pts[i2, j2].X, (1 - zratio) * pts[i0, j0].Y +
                                zratio * pts[i2, j2].Y, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                               (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = new Point3((1 - zratio) * pts[i1, j1].X +
                                zratio * pts[i2, j2].X, pts[i1, j1].Y, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = new Point3(pts[i0, j0].X, (1 - zratio) *
                                pts[i0, j0].Y + zratio * pts[i1, j1].Y, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[1] = new Point3(pts[i0, j0].X * (1 - zratio) +
                                pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                                pts[i2, j2].Y * zratio, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }

                        // right triangle:
                        i0 = i;
                        j0 = j;
                        i1 = i + 1;
                        j1 = j;
                        i2 = i + 1;
                        j2 = j + 1;
                        if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = new Point3(pts[i0, j0].X * (1 - zratio) +
                                pts[i1, j1].X * zratio, pts[i0, j0].Y, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = new Point3(pts[i1, j1].X, pts[i1, j1].Y * 
                                (1 - zratio) + pts[i2, j2].Y * zratio, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                            (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[0] = new Point3(pts[i0, j0].X * (1 - zratio) +
                                pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                                pts[i2, j2].Y * zratio, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i1, j1].Z) /
                                (pts[i2, j2].Z - pts[i1, j1].Z);
                            pta[1] = new Point3(pts[i1, j1].X, pts[i1, j1].Y *
                                (1 - zratio) + pts[i2, j2].Y * zratio, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }
                        else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                            (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                            zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                        {
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i1, j1].Z - pts[i0, j0].Z);
                            pta[0] = new Point3(pts[i0, j0].X * (1 - zratio) +
                                pts[i1, j1].X * zratio, pts[i0, j0].Y, cs.ZMin, 1);
                            zratio = (zlevels[k] - pts[i0, j0].Z) /
                                (pts[i2, j2].Z - pts[i0, j0].Z);
                            pta[1] = new Point3(pts[i0, j0].X * (1 - zratio) +
                                pts[i2, j2].X * zratio, pts[i0, j0].Y * (1 - zratio) +
                                pts[i2, j2].Y * zratio, cs.ZMin, 1);
                            pta[0].Transform(m, form1, cs);
                            pta[1].Transform(m, form1, cs);
                            g.DrawLine(aPen, pta[0].X, pta[0].Y, pta[1].X, pta[1].Y);
                        }
                    }
                }
            }
            aPen.Dispose();
        }

        private void AddSlice(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            SolidBrush aBrush = new SolidBrush(Color.Black);
            Point4[, ,] pts = ds.Point4Array;
            PointF[] pta = new PointF[4];
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);

            // Find the minumum and maximum v values:
            float vmin = ds.VDataMin();
            float vmax = ds.VDataMax();

            // Perform transformation on points:
            for (int i = 0; i < pts.GetLength(0); i++)
            {
                for (int j = 0; j < pts.GetLength(1); j++)
                {
                    for (int k = 0; k < pts.GetLength(2); k++)
                    {
                        pts[i, j, k].point3.Transform(m, form1, cs);
                        pts[i, j, k].point3.Z = pts[i, j, k].V;
                    }
                }
            }

            // Select slice:
            if (XYZSlice == SliceEnum.XSlice)
            {
                if (SliceLocation < cs.XMin)
                    SliceLocation = cs.XMin;
                if (SliceLocation > cs.XMax)
                    SliceLocation = cs.XMax;
                int nfix = (int)((SliceLocation - ds.XDataMin) / ds.XSpacing) + 1;
                for (int j = 0; j < pts.GetLength(1)-1; j++)
                {
                    for (int k = 0; k < pts.GetLength(2) - 1; k++)
                    {
                        pta[0] = new PointF(pts[nfix, j, k].point3.X, pts[nfix, j, k].point3.Y);
                        pta[1] = new PointF(pts[nfix, j+1, k].point3.X, pts[nfix, j+1, k].point3.Y);
                        pta[2] = new PointF(pts[nfix, j+1, k+1].point3.X, pts[nfix, j+1, k+1].point3.Y);
                        pta[3] = new PointF(pts[nfix, j, k+1].point3.X, pts[nfix, j, k+1].point3.Y);
                        Color color = AddColor(cs, pts[nfix, j, k].point3, vmin, vmax);
                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }
            else if (XYZSlice == SliceEnum.YSlice)
            {
                if (SliceLocation < cs.YMin)
                    SliceLocation = cs.YMin;
                if (SliceLocation > cs.YMax)
                    SliceLocation = cs.YMax;
                int nfix = (int)((SliceLocation - ds.YDataMin) / ds.YSpacing) + 1;
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int k = 0; k < pts.GetLength(2) - 1; k++)
                    {
                        pta[0] = new PointF(pts[i, nfix, k].point3.X, pts[i, nfix, k].point3.Y);
                        pta[1] = new PointF(pts[i + 1, nfix, k].point3.X, pts[i + 1, nfix, k].point3.Y);
                        pta[2] = new PointF(pts[i + 1, nfix, k + 1].point3.X, pts[i + 1, nfix, k + 1].point3.Y);
                        pta[3] = new PointF(pts[i, nfix, k + 1].point3.X, pts[i, nfix, k + 1].point3.Y);
                        Color color = AddColor(cs, pts[i,nfix, k].point3, vmin, vmax);

                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }
            else if (XYZSlice == SliceEnum.ZSlice)
            {
                if (SliceLocation < cs.ZMin)
                    SliceLocation = cs.ZMin;
                if (SliceLocation > cs.ZMax)
                    SliceLocation = cs.ZMax;
                int nfix = (int)((SliceLocation - ds.ZZDataMin) / ds.ZSpacing) + 1;
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        pta[0] = new PointF(pts[i, j,nfix].point3.X, pts[i,j, nfix].point3.Y);
                        pta[1] = new PointF(pts[i + 1, j,nfix].point3.X, pts[i + 1,j, nfix].point3.Y);
                        pta[2] = new PointF(pts[i + 1, j+1,nfix].point3.X, pts[i + 1,j+1, nfix].point3.Y);
                        pta[3] = new PointF(pts[i, j+1,nfix].point3.X, pts[i, j+1,nfix].point3.Y);
                        Color color = AddColor(cs, pts[i, j,nfix].point3, vmin, vmax);
                        aBrush = new SolidBrush(color);
                        g.FillPolygon(aBrush, pta);
                        if (ds.LineStyle.IsVisible)
                        {
                            g.DrawPolygon(aPen, pta);
                        }
                    }
                }
            }
            aPen.Dispose();
            aBrush.Dispose();
        }

        private void AddBar3D(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            Matrix3 m = Matrix3.AzimuthElevation(cs.Elevation, cs.Azimuth);
            PointF[] pt = new PointF[4];
            Point3[,] pts = ds.PointArray;

            // Find the minumum and maximum z values:
            float zmin = ds.ZDataMin();
            float zmax = ds.ZDataMax();

            // Check parameters:
            float xlength = ds.BarStyle.XLength;
            if (xlength <= 0)
                xlength = 0.1f * ds.XSpacing;
            else if (xlength > 0.5f)
                xlength = 0.5f * ds.XSpacing;
            else
                xlength = ds.BarStyle.XLength * ds.XSpacing;
            float ylength = ds.BarStyle.YLength;
            if (ylength <= 0)
                ylength = 0.1f * ds.YSpacing;
            else if (ylength > 0.5f)
                ylength = 0.5f * ds.YSpacing;
            else
                ylength = ds.BarStyle.YLength * ds.YSpacing;
            float zorigin = ds.BarStyle.ZOrigin;

            // Draw 3D bars:
            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    int ii = i;
                    int jj = j;
                    if (cs.Azimuth >= -180 && cs.Azimuth < -90)
                    {
                        ii = pts.GetLength(0) - 2 - i;
                        jj = j;
                    }
                    else if (cs.Azimuth >= -90 && cs.Azimuth < 0)
                    {
                        ii = pts.GetLength(0) - 2 - i;
                        jj = pts.GetLength(1) - 2 - j;
                    }
                    else if (cs.Azimuth >= 0 && cs.Azimuth < 90)
                    {
                        ii = i;
                        jj = pts.GetLength(1) - 2 - j;
                    }
                    else if (cs.Azimuth >= 90 && cs.Azimuth <= 180)
                    {
                        ii = i;
                        jj = j;
                    }
                    DrawBar(g, ds, cs, m, pts[ii, jj], xlength, ylength, zorigin, zmax, zmin);
                }
            }
        }

        private void DrawBar(Graphics g, DataSeries ds, ChartStyle cs, Matrix3 m, 
            Point3 pt, float xlength, float ylength, float zorign, float zmax, float zmin)
        {
            Pen aPen = new Pen(ds.LineStyle.LineColor, ds.LineStyle.Thickness);
            aPen.DashStyle = ds.LineStyle.Pattern;
            Color color = AddColor(cs, pt, zmin, zmax);
            SolidBrush aBrush = new SolidBrush(color);
            Point3[] pts = new Point3[8];
            Point3[] pts1 = new Point3[8];
            Point3[] pt3 = new Point3[4];
            PointF[] pta = new PointF[4];
            pts[0] = new Point3(pt.X - xlength, pt.Y - ylength, zorign, 1);
            pts[1] = new Point3(pt.X - xlength, pt.Y + ylength, zorign, 1);
            pts[2] = new Point3(pt.X + xlength, pt.Y + ylength, zorign, 1);
            pts[3] = new Point3(pt.X + xlength, pt.Y - ylength, zorign, 1);
            pts[4] = new Point3(pt.X + xlength, pt.Y - ylength, pt.Z, 1);
            pts[5] = new Point3(pt.X + xlength, pt.Y + ylength, pt.Z, 1);
            pts[6] = new Point3(pt.X - xlength, pt.Y + ylength, pt.Z, 1);
            pts[7] = new Point3(pt.X - xlength, pt.Y - ylength, pt.Z, 1);

            for (int i = 0; i < pts.Length; i++)
            {
                pts1[i] = new Point3(pts[i].X, pts[i].Y, pts[i].Z, 1);
                pts[i].Transform(m, form1, cs);
            }

            int[] nconfigs = new int[8];
            if (ds.BarStyle.IsBarSingleColor)
            {
                pta[0] = new PointF(pts[4].X, pts[4].Y);
                pta[1] = new PointF(pts[5].X, pts[5].Y);
                pta[2] = new PointF(pts[6].X, pts[6].Y);
                pta[3] = new PointF(pts[7].X, pts[7].Y);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);
                pta[0] = new PointF(pts[0].X, pts[0].Y);
                pta[1] = new PointF(pts[1].X, pts[1].Y);
                pta[2] = new PointF(pts[2].X, pts[2].Y);
                pta[3] = new PointF(pts[3].X, pts[3].Y);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);

                if (cs.Azimuth >= -180 && cs.Azimuth < -90)
                {
                    nconfigs = new int[8] { 1, 2, 5, 6, 1, 0, 7, 6 };
                }
                else if (cs.Azimuth >= -90 && cs.Azimuth < 0)
                {
                    nconfigs = new int[8] { 1, 0, 7, 6, 0, 3, 4, 7 };
                }
                else if (cs.Azimuth >= 0 && cs.Azimuth < 90)
                {
                    nconfigs = new int[8] { 0, 3, 4, 7, 2, 3, 4, 5 };
                }
                else if (cs.Azimuth >= 90 && cs.Azimuth < 180)
                {
                    nconfigs = new int[8] { 2, 3, 4, 5, 1, 2, 5, 6 };
                }
                pta[0] = new PointF(pts[nconfigs[0]].X, pts[nconfigs[0]].Y);
                pta[1] = new PointF(pts[nconfigs[1]].X, pts[nconfigs[1]].Y);
                pta[2] = new PointF(pts[nconfigs[2]].X, pts[nconfigs[2]].Y);
                pta[3] = new PointF(pts[nconfigs[3]].X, pts[nconfigs[3]].Y);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);
                pta[0] = new PointF(pts[nconfigs[4]].X, pts[nconfigs[4]].Y);
                pta[1] = new PointF(pts[nconfigs[5]].X, pts[nconfigs[5]].Y);
                pta[2] = new PointF(pts[nconfigs[6]].X, pts[nconfigs[6]].Y);
                pta[3] = new PointF(pts[nconfigs[7]].X, pts[nconfigs[7]].Y);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);
            }
            else if (!ds.BarStyle.IsBarSingleColor && IsColorMap)
            {
               pta[0] = new PointF(pts[4].X, pts[4].Y);
                pta[1] = new PointF(pts[5].X, pts[5].Y);
                pta[2] = new PointF(pts[6].X, pts[6].Y);
                pta[3] = new PointF(pts[7].X, pts[7].Y);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);
                pta[0] = new PointF(pts[0].X, pts[0].Y);
                pta[1] = new PointF(pts[1].X, pts[1].Y);
                pta[2] = new PointF(pts[2].X, pts[2].Y);
                pta[3] = new PointF(pts[3].X, pts[3].Y);
                color = AddColor(cs, pts[0], zmin, zmax);
                aBrush = new SolidBrush(color);
                g.FillPolygon(aBrush, pta);
                g.DrawPolygon(aPen, pta);

                float dz = (zmax - zmin) / 63;
                if (pt.Z < zorign)
                    dz = -dz;
                int nz = (int)((pt.Z - zorign) / dz) + 1;
                if (nz < 1)
                    nz = 1;
                float z = zorign;

                if (cs.Azimuth >= -180 && cs.Azimuth < -90)
                {
                    nconfigs = new int[4] { 1, 2, 1, 0 };
                }
                else if (cs.Azimuth >= -90 && cs.Azimuth < 0)
                {
                    nconfigs = new int[4] { 1, 0, 0, 3 };
                }
                else if (cs.Azimuth >= 0 && cs.Azimuth < 90)
                {
                    nconfigs = new int[4] { 0, 3, 2, 3 };
                }
                else if (cs.Azimuth >= 90 && cs.Azimuth <= 180)
                {
                    nconfigs = new int[4] { 2, 3, 1, 2 };
                }
                for (int i = 0; i < nz; i++)
                {
                    z = zorign + i * dz;
                    pt3[0] = new Point3(pts1[nconfigs[0]].X, pts1[nconfigs[0]].Y, z, 1);
                    pt3[1] = new Point3(pts1[nconfigs[1]].X, pts1[nconfigs[1]].Y, z, 1);
                    pt3[2] = new Point3(pts1[nconfigs[1]].X, pts1[nconfigs[1]].Y, z + dz, 1);
                    pt3[3] = new Point3(pts1[nconfigs[0]].X, pts1[nconfigs[0]].Y, z + dz, 1);
                    for (int j = 0; j < pt3.Length; j++)
                    {
                        pt3[j].Transform(m, form1, cs);
                    }
                    pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                    pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                    pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                    pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                    color = AddColor(cs, pt3[0], zmin, zmax);
                    aBrush = new SolidBrush(color);
                    g.FillPolygon(aBrush, pta);
                }
                pt3[0] = new Point3(pts1[nconfigs[0]].X, pts1[nconfigs[0]].Y, zorign, 1);
                pt3[1] = new Point3(pts1[nconfigs[1]].X, pts1[nconfigs[1]].Y, zorign, 1);
                pt3[2] = new Point3(pts1[nconfigs[1]].X, pts1[nconfigs[1]].Y, pt.Z, 1);
                pt3[3] = new Point3(pts1[nconfigs[0]].X, pts1[nconfigs[0]].Y, pt.Z, 1);
                for (int j = 0; j < pt3.Length; j++)
                {
                    pt3[j].Transform(m, form1, cs);
                }
                pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                g.DrawPolygon(aPen, pta);
                for (int i = 0; i < nz; i++)
                {
                    z = zorign + i * dz;
                    pt3[0] = new Point3(pts1[nconfigs[2]].X, pts1[nconfigs[2]].Y, z, 1);
                    pt3[1] = new Point3(pts1[nconfigs[3]].X, pts1[nconfigs[3]].Y, z, 1);
                    pt3[2] = new Point3(pts1[nconfigs[3]].X, pts1[nconfigs[3]].Y, z + dz, 1);
                    pt3[3] = new Point3(pts1[nconfigs[2]].X, pts1[nconfigs[2]].Y, z + dz, 1);
                    for (int j = 0; j < pt3.Length; j++)
                    {
                        pt3[j].Transform(m, form1, cs);
                    }
                    pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                    pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                    pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                    pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                    color = AddColor(cs, pt3[0], zmin, zmax);
                    aBrush = new SolidBrush(color);
                    g.FillPolygon(aBrush, pta);
                }
                pt3[0] = new Point3(pts1[nconfigs[2]].X, pts1[nconfigs[2]].Y, zorign, 1);
                pt3[1] = new Point3(pts1[nconfigs[3]].X, pts1[nconfigs[3]].Y, zorign, 1);
                pt3[2] = new Point3(pts1[nconfigs[3]].X, pts1[nconfigs[3]].Y, pt.Z, 1);
                pt3[3] = new Point3(pts1[nconfigs[2]].X, pts1[nconfigs[2]].Y, pt.Z, 1);
                for (int j = 0; j < pt3.Length; j++)
                {
                    pt3[j].Transform(m, form1, cs);
                }
                pta[0] = new PointF(pt3[0].X, pt3[0].Y);
                pta[1] = new PointF(pt3[1].X, pt3[1].Y);
                pta[2] = new PointF(pt3[2].X, pt3[2].Y);
                pta[3] = new PointF(pt3[3].X, pt3[3].Y);
                g.DrawPolygon(aPen, pta);
            }
            aPen.Dispose();
            aBrush.Dispose();
        }

        public void AddColorBar(Graphics g, DataSeries ds, ChartStyle cs, ChartStyle2D cs2d)
        {
            if (cs.IsColorBar && IsColorMap)
            {
                Pen aPen = new Pen(Color.Black, 1);
                SolidBrush aBrush = new SolidBrush(cs.TickColor);
                StringFormat sFormat = new StringFormat();
                sFormat.Alignment = StringAlignment.Near;
                SizeF size = g.MeasureString("A", cs.TickFont);

                int x, y, width, height;
                Point3[] pts = new Point3[64];
                PointF[] pta = new PointF[4];
                float zmin, zmax;
                if (ChartType == ChartTypeEnum.Slice)
                {
                    zmin = ds.VDataMin();
                    zmax = ds.VDataMax();
                }
                else
                {
                    zmin = ds.ZDataMin();
                    zmax = ds.ZDataMax();
                }
                float dz = (zmax - zmin) / 63;
                if (ChartType == ChartTypeEnum.Contour ||
                    ChartType == ChartTypeEnum.FillContour ||
                    ChartType == ChartTypeEnum.XYColor)
                {
                    x = 5 * cs2d.ChartArea.Width / 6;
                    y = form1.PlotPanel.Top;
                    width = cs2d.ChartArea.Width / 25;
                    height = form1.PlotPanel.Height;
                    // Add color bar:
                    for (int i = 0; i < 64; i++)
                    {
                        pts[i] = new Point3(x, y, zmin + i * dz, 1);
                    }
                    for (int i = 0; i < 63; i++)
                    {
                        Color color = AddColor(cs, pts[i], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        float y1 = y + height - (pts[i].Z - zmin) * height / (zmax - zmin);
                        float y2 = y + height - (pts[i + 1].Z - zmin) * height / (zmax - zmin);
                        pta[0] = new PointF(x, y2);
                        pta[1] = new PointF(x + width, y2);
                        pta[2] = new PointF(x + width, y1);
                        pta[3] = new PointF(x, y1);
                        g.FillPolygon(aBrush, pta);
                    }
                    g.DrawRectangle(aPen, x, y, width, height);

                    // Add ticks and labels to the color bar:
                    float ticklength = 0.1f * width;
                    for (float z = zmin; z <= zmax; z = z + (zmax - zmin) / 6)
                    {
                        float yy = y + height - (z - zmin) * height / (zmax - zmin);
                        g.DrawLine(aPen, x, yy, x + ticklength, yy);
                        g.DrawLine(aPen, x + width, yy, x + width - ticklength, yy);
                        g.DrawString((Math.Round(z, 2)).ToString(), cs.TickFont, Brushes.Black,
                            new PointF(x + width + 5, yy - size.Height / 2), sFormat);
                    }
                }
                else                 
                {
                    x = 5 * form1.PlotPanel.Width / 6;
                    y = form1.PlotPanel.Height / 10;
                    width = form1.PlotPanel.Width / 25;
                    height = 8 * form1.PlotPanel.Height / 10;

                    // Add color bar:
                    for (int i = 0; i < 64; i++)
                    {
                        pts[i] = new Point3(x, y, zmin + i * dz, 1);
                    }
                    for (int i = 0; i < 63; i++)
                    {
                        Color color = AddColor(cs, pts[i], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        float y1 = y + height - (pts[i].Z - zmin) * height / (zmax - zmin);
                        float y2 = y + height - (pts[i + 1].Z - zmin) * height / (zmax - zmin);
                        pta[0] = new PointF(x, y2);
                        pta[1] = new PointF(x + width, y2);
                        pta[2] = new PointF(x + width, y1);
                        pta[3] = new PointF(x, y1);
                        g.FillPolygon(aBrush, pta);
                    }
                    g.DrawRectangle(aPen, x, y, width, height);

                    // Add ticks and labels to the color bar:
                    float ticklength = 0.1f * width;
                    for (float z = zmin; z <= zmax; z = z + (zmax - zmin) / 6)
                    {
                        float yy = y + height - (z - zmin) * height / (zmax - zmin);
                        g.DrawLine(aPen, x, yy, x + ticklength, yy);
                        g.DrawLine(aPen, x + width, yy, x + width - ticklength, yy);
                        g.DrawString((Math.Round(z, 2)).ToString(), cs.TickFont, Brushes.Black,
                            new PointF(x + width + 5, yy - size.Height / 2), sFormat);
                    }
                }
            }
        }

        private Color AddColor(ChartStyle cs, Point3 pt,
            float zmin, float zmax)
        {
            int colorLength = CMap.GetLength(0);
            int cindex = (int)Math.Round((colorLength * (pt.Z - zmin) +
                        (zmax - pt.Z)) / (zmax - zmin));
            if (cindex < 1)
                cindex = 1;
            if (cindex > colorLength)
                cindex = colorLength;
            Color color = Color.FromArgb(CMap[cindex - 1, 0],
                CMap[cindex - 1, 1], CMap[cindex - 1, 2],
                CMap[cindex - 1, 3]);
            return color;
        }

        private void Interp(Graphics g, ChartStyle cs, ChartStyle2D cs2d, Matrix3 m,
             Point3[] pta, float zmin, float zmax, int flag)
        {
            SolidBrush aBrush = new SolidBrush(Color.Black);
            PointF[] points = new PointF[4];
            int npoints = NumberInterp;
            Point3[,] pts = new Point3[npoints + 1, npoints + 1];
            Point3[,] pts1 = new Point3[npoints + 1, npoints + 1];
            float x0 = pta[0].X;
            float y0 = pta[0].Y;
            float x1 = pta[2].X;
            float y1 = pta[2].Y;
            float dx = (x1 - x0) / npoints;
            float dy = (y1 - y0) / npoints;
            float C00 = pta[0].Z;
            float C10 = pta[3].Z;
            float C11 = pta[2].Z;
            float C01 = pta[1].Z;
            float x, y, C;
            Color color;

            if(flag == 1) // For Surface chart
            {
                for (int i = 0; i <= npoints; i++)
                {
                    x = x0 + i * dx;
                    for (int j = 0; j <= npoints; j++)
                    {
                        y = y0 + j * dy;
                        C = (y1 - y) * ((x1 - x) * C00 +
                            (x - x0) * C10) / (x1 - x0) / (y1 - y0) +
                            (y - y0) * ((x1 - x) * C01 +
                            (x - x0) * C11) / (x1 - x0) / (y1 - y0);
                        pts[i, j] = new Point3(x, y, C, 1);
                        pts[i, j].Transform(m, form1, cs);
                    }
                }

                for (int i = 0; i < npoints; i++)
                {
                    for (int j = 0; j < npoints; j++)
                    {

                        color = AddColor(cs, pts[i, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        points[0] = new PointF(pts[i, j].X, pts[i, j].Y);
                        points[1] = new PointF(pts[i + 1, j].X, pts[i + 1, j].Y);
                        points[2] = new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y);
                        points[3] = new PointF(pts[i, j + 1].X, pts[i, j + 1].Y);
                        g.FillPolygon(aBrush, points);
                        aBrush.Dispose();
                    }
                }
            }
            else if (flag == 2) // For XYColor chart
            {
                for (int i = 0; i <= npoints; i++)
                {
                    x = x0 + i * dx;
                    for (int j = 0; j <= npoints; j++)
                    {
                        y = y0 + j * dy;
                        C = (y1 - y) * ((x1 - x) * C00 +
                            (x - x0) * C10) / (x1 - x0) / (y1 - y0) +
                            (y - y0) * ((x1 - x) * C01 +
                            (x - x0) * C11) / (x1 - x0) / (y1 - y0);
                        pts[i, j] = new Point3(x, y, C, 1);
                    }
                }

                for (int i = 0; i < npoints; i++)
                {
                    for (int j = 0; j < npoints; j++)
                    {

                        color = AddColor(cs, pts[i, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        points[0] = cs2d.Point2D(new PointF(pts[i, j].X, pts[i, j].Y), cs);
                        points[1] = cs2d.Point2D(new PointF(pts[i + 1, j].X, pts[i + 1, j].Y), cs);
                        points[2] = cs2d.Point2D(new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y), cs);
                        points[3] = cs2d.Point2D(new PointF(pts[i, j + 1].X, pts[i, j + 1].Y), cs);
                        g.FillPolygon(aBrush, points);
                        aBrush.Dispose();
                    }
                }
            }
            else if(flag == 3)  // For XYColor3D chart
            {
                for (int i = 0; i <= npoints; i++)
                {
                    x = x0 + i * dx;
                    for (int j = 0; j <= npoints; j++)
                    {
                        y = y0 + j * dy;
                        C = (y1 - y) * ((x1 - x) * C00 +
                            (x - x0) * C10) / (x1 - x0) / (y1 - y0) +
                            (y - y0) * ((x1 - x) * C01 +
                            (x - x0) * C11) / (x1 - x0) / (y1 - y0);
                        pts1[i, j] = new Point3(x, y, C, 1);
                        pts[i, j] = new Point3(x, y, cs.ZMin, 1);
                        pts[i, j].Transform(m, form1, cs);
                    }
                }

                for (int i = 0; i < npoints; i++)
                {
                    for (int j = 0; j < npoints; j++)
                    {

                        color = AddColor(cs, pts1[i, j], zmin, zmax);
                        aBrush = new SolidBrush(color);
                        points[0] = new PointF(pts[i, j].X, pts[i, j].Y);
                        points[1] = new PointF(pts[i + 1, j].X, pts[i + 1, j].Y);
                        points[2] = new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y);
                        points[3] = new PointF(pts[i, j + 1].X, pts[i, j + 1].Y);
                        g.FillPolygon(aBrush, points);
                        aBrush.Dispose();
                    }
                }
            }
        }

    }
}
