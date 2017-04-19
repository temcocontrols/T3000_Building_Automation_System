using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WFA_PH_CurveDemo
{
    /// <summary>
    /// this class will perform all the operation that is done in ph chart
    /// such as node insetion, moving node , deleting node, clearing chart, etc.
    /// </summary>
    class PHChartOperation:formPHChartMain
    {
        public void AddNode(Chart chart,Series s1 ,Color c,double xAxis, double yAxis)
        {

            int index = numberOfPoints;
            s1.ChartType = SeriesChartType.Point;         
            s1.MarkerSize = 20;
            s1.MarkerStyle = MarkerStyle.Circle;
            s1.Points.AddXY(xAxis, yAxis);
            string s = "source :\nx-axis" + xAxis + "\ny-axis" + yAxis;
            s1.Points[index].Color = c;
            s1.Points[index].ToolTip = s;

        }




        System.Drawing.Point? prevPosition = null;
       public  double currentXAxis = 0.000;
       public double currentYAxis = 0.000;

        public void MouseMoveAction(Chart chart1,object sender, MouseEventArgs e)
        {

            var pos = e.Location;
            if (prevPosition.HasValue && pos == prevPosition.Value)
                return;

            prevPosition = pos;
            var results = chart1.HitTest(pos.X, pos.Y, false, ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(pos.X);
                    var yVal = result.ChartArea.AxisY.PixelPositionToValue(pos.Y);


                    if (((double)xVal >= 0.01 && (double)xVal <= 4000) && ((double)yVal >= 0.01 && (double)yVal <= 100))
                    {

                        currentXAxis = (double)xVal;
                        currentYAxis = (double)yVal;

                        lbCoordinate.Text = "x = " + currentXAxis + ",y = " + currentYAxis;
                    }

                }

            }

        } //Close of public void


    }//--Close of class
}
