using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace Example6_3
{
    public partial class Form1 : Form
    {
        ChartStyle cs;
        ChartStyle2D cs2d;
        DataSeries ds;
        DrawChart dc;
        ChartFunctions cf;
        ColorMap cm;
     
        public Form1()
        {




            InitializeComponent();

            this.SetStyle(ControlStyles.AllPaintingInWmPaint | 
            ControlStyles.UserPaint | 
            ControlStyles.DoubleBuffer,true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.BackColor = Color.White;

            //  MessageBox.Show("Form1 constructor");

            // Subscribing to a paint eventhandler to drawingPanel: 
          //  MessageBox.Show("Subscribing to a paint eventhandler to drawingPanel: ");
            PlotPanel.Paint +=
                new PaintEventHandler(PlotPanelPaint);




            cs = new ChartStyle(this);
        cs2d = new ChartStyle2D(this);
        ds = new DataSeries();
        dc = new DrawChart(this);
        cf = new ChartFunctions();
        cm = new ColorMap();
            //dc.ChartType = DrawChart.ChartTypeEnum.Contour;

          //  MessageBox.Show("All initialization finish eg. new wala");
            cs.GridStyle.LineColor = Color.LightGray;
     cs.GridStyle.Pattern = DashStyle.Dash;
     cs.Title = "No Title";
            cs.IsColorBar = false;//true;
     cs2d.ChartBackColor = Color.White;
     cs2d.ChartBorderColor = Color.Black;

     ds.LineStyle.IsVisible = false;
            //ds.BarStyle.XLength = 0.3f;
            //ds.BarStyle.YLength = 0.3f;
            //ds.BarStyle.IsBarSingleColor = false;
            //ds.BarStyle.ZOrigin = 0;

            /*    
                     dc.ChartType = DrawChart.ChartTypeEnum.Slice;
                     //dc.IsColorMap = true;
                     dc.IsHiddenLine = false;
                     //dc.IsInterp = true;
                     dc.NumberInterp = 5;
                      dc.CMap = cm.Jet(); //cm.Autumn();//cm.Jet();

                      dc.NumberContours = 15;
                     dc.XYZSlice = DrawChart.SliceEnum.ZSlice;
                     dc.SliceLocation = 1;
                      //dc.IsBarSingleColor = false;  
          */
           // MessageBox.Show("Before contour");
            dc.ChartType = DrawChart.ChartTypeEnum.Contour;
            //  dc.IsHiddenLine = false;
            dc.IsColorMap = false;//true;
            //dc.IsInterp = true;
            dc.NumberContours = 25;
            // dc.AddChart()       
            cs.IsColorBar = true;
            dc.IsColorMap = true;
            dc.CMap = cm.Jet();
           // MessageBox.Show("end of constructor");

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
                //cf.Exp4D(ds, cs);
               // MessageBox.Show("inside OnPaint Funtion call for contour ");
                cf.Peak3D(ds, cs);
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
               // MessageBox.Show("We are added a fxn here plot_panel_plot");
                cf.Peak3D(ds, cs);
                cs2d.AddChartStyle2D(g, cs);
                dc.AddChart(g, ds, cs, cs2d);
              
            }
            else
            {
              //  MessageBox.Show("We are here nnnnnnnnnnnnnnnnnn");
                //cs.Elevation = trkElevation.Value;
                //cs.Azimuth = trkAzimuth.Value;
                //cf.Exp4D(ds, cs);
                cf.Peak3D(ds, cs);
                cs.AddChartStyle(g);
                dc.AddChart(g, ds, cs, cs2d);
            }
        }

  
      
        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}