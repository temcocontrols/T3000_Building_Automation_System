using System;
using System.Drawing;
using System.Windows.Forms;

namespace WFA_PH_CurveDemo
{



    public partial class FunctionGraph : UserControl
    {
        private Rectangle _area = new Rectangle(-10, -10, 20, 20);

        private Func<float, float, float> _func;

        public  FunctionGraph()
        {
            SetStyle(
                ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.DoubleBuffer |
                ControlStyles.OptimizedDoubleBuffer |
                ControlStyles.ResizeRedraw, true);

            //this.Padding.All = 20;
            //InitializeComponent();
        }


        public Rectangle Area
        {
            get { return _area; }
            set
            {
                _area = value;

                Invalidate();
            }
        }


        public Func<float, float, float> Function
        {
            get { return _func; }
            set
            {
                _func = value;
                Invalidate();
            }
        }


        public PointF GraphToClient(PointF p)
        {
            var xreal = this.ClientRectangle.Width - this.Padding.All * 2;
            var yreal = this.ClientRectangle.Height - this.Padding.All * 2;
            int n = (this.ClientRectangle.Width - this.Padding.All * 2) / 5;

            int m = (this.ClientRectangle.Height - this.Padding.All * 2) / 5;

            return new PointF
            {
                X = this.Padding.All + p.X / (xreal / (n * 5)),
                Y = this.Padding.All + p.Y / (yreal / (m * 5))
            };




        }

        protected override void OnPaint(PaintEventArgs e)
        {
            var graphics = e.Graphics;

            if (_func != null && !DesignMode && Enabled)
            {

                float min = Single.MaxValue;

                float max = Single.MinValue;

                int n = (this.ClientRectangle.Width - this.Padding.All * 2) / 5;

                int m = (this.ClientRectangle.Height - this.Padding.All * 2) / 5;

                var pts = new Point3F[n, m];

                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < m; j++)
                    {




                        var xreal = this.ClientRectangle.Width - this.Padding.All * 2;
                        var yreal = this.ClientRectangle.Height - this.Padding.All * 2;


                        var pt = new Point3F(this.Padding.All + i * 5, this.Padding.All + j * 5, _func((i * 5) * (xreal / (n * 5)), (j * 5) * (yreal / (m * 5))));

                        if (i == n - 1)
                            pt.X = this.ClientRectangle.Width - this.Padding.All;
                        if (j == m - 1)
                            pt.Y = this.ClientRectangle.Height - this.Padding.All;

                        pts[i, j] = pt;
                        if (pts[i, j].Z > max)
                            max = pts[i, j].Z;
                        if (pts[i, j].Z < min)
                            min = pts[i, j].Z;



                    }

                }


                DrawContour(graphics, pts, min, max, 10);
                DrawColor(graphics, pts, min, max);

                var rect = this.ClientRectangle;

                rect.Inflate(-this.Padding.All + 1, -this.Padding.All + 1);

                ControlPaint.DrawBorder(graphics, rect, SystemColors.ControlDarkDark, ButtonBorderStyle.Dashed);

                int stepx = (this.ClientRectangle.Width - 2 * this.Padding.All) / _area.Width;
                int stepy = (this.ClientRectangle.Height - 2 * this.Padding.All) / _area.Height;

                for (int i = _area.X; i < _area.Width; i++)
                {
                    graphics.DrawString(i.ToString("0.0"), this.Font, Brushes.Black, this.Padding.All + (i - _area.X) * stepx, this.ClientRectangle.Height - this.Padding.All * 0.75f);

                }
                using (var format = new StringFormat(StringFormatFlags.DirectionRightToLeft) { Alignment = StringAlignment.Far })
                {
                    // graphics.TranslateTransform(this.ClientRectangle.Width,0);
                    // graphics.RotateTransform(90);

                    for (int i = _area.Y; i < _area.Height; i++)
                    {
                        graphics.DrawString(i.ToString("0.0"), this.Font, Brushes.Black, this.Padding.All * 0.25f - this.Font.Size * 0.5f
                            , this.ClientRectangle.Height - this.Padding.All - (i - _area.Y) * stepy, format);
                    }
                    graphics.ResetTransform();
                }
            }

            ControlPaint.DrawBorder(graphics, ClientRectangle, SystemColors.ActiveBorder, ButtonBorderStyle.Solid);
            base.OnPaint(e);
        }

        public struct Point3F
        {
            public float X { get; set; }
            public float Y { get; set; }
            public float Z { get; set; }


            public Point3F(float x, float y, float z) : this()
            {
                this.X = x;
                this.Y = y;
                this.Z = z;
            }
        }




        public static void DrawContour(Graphics g, Point3F[,] pts, float zmin, float zmax, int ncount)
        {
            using (var aPen = new Pen(Color.DimGray) { Width = 0.25f })
            {
                var pta = new PointF[2];

                //mY CODE
                Form1 frm1 = new Form1();
                var seriesList  = new string[ncount];

                var zlevels = new float[ncount];
                for (int i = 0; i < ncount; i++)
                {
                    zlevels[i] = zmin + i * (zmax - zmin) / (ncount - 1);
                    seriesList[i] = "ContourSeries" + i;
                    frm1.ph_chart.Series.Add("ContourSeries" + i);
                }
                
                int i0, i1, i2, j0, j1, j2;
                float zratio = 1; // Draw contour on the XY plane:
                for (int i = 0; i < pts.GetLength(0) - 1; i++)
                {
                    for (int j = 0; j < pts.GetLength(1) - 1; j++)
                    {
                        for (int k = 0; k < ncount; k++)
                        {
                            string seriesSelected = seriesList[k];
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
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i1, j1].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF(pts[i0, j0].X, (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y);
                                zratio = (zlevels[k] - pts[i1, j1].Z) / (pts[i2, j2].Z - pts[i1, j1].Z);
                                pta[1] =
                                    new PointF((1 - zratio) * pts[i1, j1].X + zratio * pts[i2, j2].X, pts[i1, j1].Y);
                                // g.DrawLine(aPen, pta[0], pta[1]);
                                
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X,pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            }
                            else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                                     (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                            {
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i2, j2].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF((1 - zratio) * pts[i0, j0].X + zratio * pts[i2, j2].X,
                                               (1 - zratio) * pts[i0, j0].Y + zratio * pts[i2, j2].Y);
                                zratio = (zlevels[k] - pts[i1, j1].Z) / (pts[i2, j2].Z - pts[i1, j1].Z);
                                pta[1] =
                                    new PointF((1 - zratio) * pts[i1, j1].X + zratio * pts[i2, j2].X, pts[i1, j1].Y);
                                // g.DrawLine(aPen, pta[0], pta[1]);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X, pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            }
                            else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                                     (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                            {
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i1, j1].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF(pts[i0, j0].X, (1 - zratio) * pts[i0, j0].Y + zratio * pts[i1, j1].Y)
                                    ;
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i2, j2].Z - pts[i0, j0].Z);
                                pta[1] =
                                    new PointF(pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio,
                                               pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio);
                                //  g.DrawLine(aPen, pta[0], pta[1]);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X, pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            } // right triangle:
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
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i1, j1].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF(pts[i0, j0].X * (1 - zratio) + pts[i1, j1].X * zratio, pts[i0, j0].Y);
                                zratio = (zlevels[k] - pts[i1, j1].Z) / (pts[i2, j2].Z - pts[i1, j1].Z);
                                pta[1] =
                                    new PointF(pts[i1, j1].X, pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio);
                                //g.DrawLine(aPen, pta[0], pta[1]);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X, pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            }
                            else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z) &&
                                     (zlevels[k] >= pts[i1, j1].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i1, j1].Z && zlevels[k] >= pts[i2, j2].Z))
                            {
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i2, j2].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF(pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio,
                                               pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio);
                                zratio = (zlevels[k] - pts[i1, j1].Z) / (pts[i2, j2].Z - pts[i1, j1].Z);
                                pta[1] =
                                    new PointF(pts[i1, j1].X, pts[i1, j1].Y * (1 - zratio) + pts[i2, j2].Y * zratio);
                                //  g.DrawLine(aPen, pta[0], pta[1]);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X, pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            }
                            else if ((zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i1, j1].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i1, j1].Z) &&
                                     (zlevels[k] >= pts[i0, j0].Z && zlevels[k] < pts[i2, j2].Z ||
                                      zlevels[k] < pts[i0, j0].Z && zlevels[k] >= pts[i2, j2].Z))
                            {
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i1, j1].Z - pts[i0, j0].Z);
                                pta[0] =
                                    new PointF(pts[i0, j0].X * (1 - zratio) + pts[i1, j1].X * zratio, pts[i0, j0].Y);
                                zratio = (zlevels[k] - pts[i0, j0].Z) / (pts[i2, j2].Z - pts[i0, j0].Z);
                                pta[1] =
                                    new PointF(pts[i0, j0].X * (1 - zratio) + pts[i2, j2].X * zratio,
                                               pts[i0, j0].Y * (1 - zratio) + pts[i2, j2].Y * zratio);
                                // g.DrawLine(aPen, pta[0], pta[1]);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[0].X, pta[0].Y);
                                frm1.ph_chart.Series[seriesSelected].Points.AddXY(pta[1].X, pta[1].Y);

                            }
                        }
                    }
                }
            }
        }

        private static Color GetColor(float value, float maxValue, float minValue)
        {
            // Convert into a value between 0 and 1023.
            int int_value = (int)(1023 * (value - maxValue) / (minValue - maxValue));

            // Map different color bands.
            if (int_value < 256)
            {
                // Red to yellow. (255, 0, 0) to (255, 255, 0).
                return Color.FromArgb(255, int_value, 0);
            }
            else if (int_value < 512)
            {
                // Yellow to green. (255, 255, 0) to (0, 255, 0).
                int_value -= 256;
                return Color.FromArgb(255 - int_value, 255, 0);
            }
            else if (int_value < 768)
            {
                // Green to aqua. (0, 255, 0) to (0, 255, 255).
                int_value -= 512;
                return Color.FromArgb(0, 255, int_value);
            }
            else
            {
                // Aqua to blue. (0, 255, 255) to (0, 0, 255).
                int_value -= 768;
                return Color.FromArgb(0, 255 - int_value, 255);
            }
        }

        private static void DrawColor(Graphics g, Point3F[,] pts, float zmin, float zmax)
        {
            var pta = new PointF[4];

            for (int i = 0; i < pts.GetLength(0) - 1; i++)
            {
                for (int j = 0; j < pts.GetLength(1) - 1; j++)
                {
                    pta[0] = new PointF(pts[i, j].X, pts[i, j].Y);
                    pta[1] = new PointF(pts[i, j + 1].X, pts[i, j + 1].Y);
                    pta[2] = new PointF(pts[i + 1, j + 1].X, pts[i + 1, j + 1].Y);
                    pta[3] = new PointF(pts[i + 1, j].X, pts[i + 1, j].Y);
                    using (var aBrush = new SolidBrush(Color.FromArgb(200, GetColor(pts[i, j].Z, zmin, zmax))))
                    {
                        g.FillPolygon(aBrush, pta);
                    }
                }
            }
        }

    }
}
