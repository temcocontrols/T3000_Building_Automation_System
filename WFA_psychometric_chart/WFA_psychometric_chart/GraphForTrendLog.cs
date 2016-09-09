using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class GraphForTrendLog : Form
    {
       // Form3 f3;
        public GraphForTrendLog()
        {
            //this.f3 = f;
            InitializeComponent();
        }
             
        private void TrendLogForm_Load(object sender, EventArgs e)
        {
            comboBox1.SelectedIndex = 0;
            
              
           // chart1.Series.Add("Series1BlueTemp");
            chart1.Series["Series1BlueTemp"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            chart1.Series["Series1BlueTemp"].Color = Color.Blue;

           // chart1.Series.Add("Series2RedHum");
            chart1.Series["Series2RedHum"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            chart1.Series["Series2RedHum"].Color = Color.Green;

            DrawGraph();
            //now lets call the adding function 
            AddDataPoints();
        }


        public void DrawGraph()
        {
            if (comboBox1.SelectedIndex == 0)
            {
                //PLot for 10 minutes
                MessageBox.Show("selc= 0");
                //setting the boundary xaxis =0-30 and yaxis = 0-50..
                chart1.ChartAreas[0].AxisX.Minimum = 0;
                chart1.ChartAreas[0].AxisX.Maximum = 10;
                chart1.ChartAreas[0].AxisY.Minimum = 0;
                chart1.ChartAreas[0].AxisY.Maximum = 100;
                chart1.ChartAreas[0].AxisY.Interval = 25;
                chart1.ChartAreas[0].AxisX.Interval = 1;

            }
            else if(comboBox1.SelectedIndex == 1)
            {
                //Plot for 1 hour
                MessageBox.Show("selc= 1");
                //setting the boundary xaxis =0-30 and yaxis = 0-50..
                chart1.ChartAreas[0].AxisX.Minimum = 0;
                chart1.ChartAreas[0].AxisX.Maximum = 60;
                chart1.ChartAreas[0].AxisY.Minimum = 0;
                chart1.ChartAreas[0].AxisY.Maximum = 100;
                chart1.ChartAreas[0].AxisY.Interval = 25;
                chart1.ChartAreas[0].AxisX.Interval = 10;

            }
            else if (comboBox1.SelectedIndex == 2)
            {
                //Plot for 1 day
                MessageBox.Show("selc= 2");
                chart1.ChartAreas[0].AxisX.Minimum = 0;
                chart1.ChartAreas[0].AxisX.Maximum = 24;
                chart1.ChartAreas[0].AxisY.Minimum = 0;
                chart1.ChartAreas[0].AxisY.Maximum = 100;
                chart1.ChartAreas[0].AxisY.Interval = 25;
                chart1.ChartAreas[0].AxisX.Interval = 2;



            }
            else if (comboBox1.SelectedIndex == 3)
            {
                //Plot for 1 month
                MessageBox.Show("selc= 3");
                chart1.ChartAreas[0].AxisX.Minimum = 0;
                chart1.ChartAreas[0].AxisX.Maximum = 30;
                chart1.ChartAreas[0].AxisY.Minimum = 0;
                chart1.ChartAreas[0].AxisY.Maximum = 100;
                chart1.ChartAreas[0].AxisY.Interval = 25;
                chart1.ChartAreas[0].AxisX.Interval = 3;

            }
        }



        public void AddDataPoints()
        {
            //chart1.Series["Series1BlueTemp"].Points.Clear();
            //int time = 0;

            //if(comboBox1.SelectedIndex == 0) {
            //// MessageBox.Show("num of point = " + f3.HumTempList.Count);

            //for(int i = 0; i < f3.HumTempList.Count; i++) {
            //        //--adding at every five seconds
            //        //   chart1.Series["Series1BlueTemp"].MarkerSize = 5;
            //        chart1.Series["Series1BlueTemp"].MarkerSize = 5;
            //        chart1.Series["Series1BlueTemp"].BorderWidth = 5;
            //chart1.Series["Series1BlueTemp"].Points.AddXY(time, f3.HumTempList[i].Temp);
            //time += 15/60;//five seconds   

            //}//close of for..
            //}else if(comboBox1.SelectedIndex == 1)
            //{
            //    //--adding at every 1 hour

            //}
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

            //on selected index change do this one

            DrawGraph();
            AddDataPoints();


        }
    }
}
