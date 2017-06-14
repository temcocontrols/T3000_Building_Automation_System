using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace PH_App
{
    public partial class Form1_jack_xu : Form
    {
        ChartStyle cs;
        ChartStyle2D cs2d;
        DataSeries ds;
        DrawChart dc;
        ChartFunctions cf;
        ColorMap cm;
     
        public Form1_jack_xu()
        {
            InitializeComponent();
          
        this.SetStyle(ControlStyles.AllPaintingInWmPaint | 
            ControlStyles.UserPaint | 
            ControlStyles.DoubleBuffer,true);
        this.SetStyle(ControlStyles.ResizeRedraw, true);
        this.BackColor = Color.White;


        // Subscribing to a paint eventhandler to drawingPanel: 
        PlotPanel.Paint += new PaintEventHandler(PlotPanelPaint);
     
        cs = new ChartStyle(this);
        cs2d = new ChartStyle2D(this);
        ds = new DataSeries();
        dc = new DrawChart(this);
        cf = new ChartFunctions();
        cm = new ColorMap();

        cs.GridStyle.LineColor = Color.LightGray;
        cs.GridStyle.Pattern = DashStyle.Dash;
        cs.Title = "No Title";
        cs.IsColorBar = true;
        cs2d.ChartBackColor = Color.White;
        cs2d.ChartBorderColor = Color.Black;

        ds.LineStyle.IsVisible = false;
        ds.BarStyle.XLength = 0.3f;
        ds.BarStyle.YLength = 0.3f;
        ds.BarStyle.IsBarSingleColor = false;
        ds.BarStyle.ZOrigin = 0;


        dc.ChartType = DrawChart.ChartTypeEnum.Slice;
        //dc.IsColorMap = true;
        dc.IsHiddenLine = false;
        //dc.IsInterp = true;
        dc.NumberInterp = 5;
        dc.CMap = cm.Jet();

        dc.NumberContours = 15;
        dc.XYZSlice = DrawChart.SliceEnum.ZSlice;
        dc.SliceLocation = 1;
        //dc.IsBarSingleColor = false;           
   
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            if (dc.ChartType == DrawChart.ChartTypeEnum.XYColor||
                dc.ChartType == DrawChart.ChartTypeEnum.Contour ||
                dc.ChartType == DrawChart.ChartTypeEnum.FillContour)
            {
                Rectangle rect = this.ClientRectangle;
                cs2d.ChartArea = new Rectangle(rect.X, rect.Y,
                    rect.Width, 19 * rect.Height / 30);
                cf.Exp4D(ds, cs);
                cs2d.SetPlotArea(g, cs);
                dc.AddColorBar(g, ds, cs, cs2d);
            }
        }

        private void PlotPanelPaint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            g.SmoothingMode = SmoothingMode.AntiAlias;

            if (dc.ChartType == DrawChart.ChartTypeEnum.XYColor||
                dc.ChartType == DrawChart.ChartTypeEnum.Contour||
                dc.ChartType == DrawChart.ChartTypeEnum.FillContour)
            {
                cs2d.AddChartStyle2D(g, cs);
                dc.AddChart(g, ds, cs, cs2d);
            }
            else
            {
                cs.Elevation = trkElevation.Value;
                cs.Azimuth = trkAzimuth.Value;
                cf.Exp4D(ds, cs);
                cs.AddChartStyle(g);
                dc.AddChart(g, ds, cs, cs2d);
            }
        }

        private void trkElevation_Scroll(object sender, EventArgs e)
        {
            tbElevation.Text = trkElevation.Value.ToString();
            PlotPanel.Invalidate();
        }

        private void trkAzimuth_Scroll(object sender, EventArgs e)
        {
            tbAzimuth.Text = trkAzimuth.Value.ToString();
            PlotPanel.Invalidate();
        }

        private void tbElevation_KeyUp(object sender, KeyEventArgs e)
        {
            int value;
            bool result = Int32.TryParse(tbElevation.Text, out value);
            if (result)
            {
                if (value <= -90)
                    value = -90;
                else if (value >= 90)
                    value = 90;
                trkElevation.Value = value;
            }
            PlotPanel.Invalidate();
        }

        private void tbAzimuth_KeyUp(object sender, KeyEventArgs e)
        {
            int value;
            bool result = Int32.TryParse(tbAzimuth.Text, out value);
            if (result)
            {
                if (value <= -180)
                    value = -180;
                else if (value >= 180)
                    value = 180;
                trkAzimuth.Value = value;
            }
            PlotPanel.Invalidate();
        }
    }
}