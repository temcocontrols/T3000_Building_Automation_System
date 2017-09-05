using System.Windows.Forms.DataVisualization.Charting;
using System.Drawing;
using System.Collections.Generic;
namespace PH_App
{
    /// <summary>
    /// This has no use , does not work as expected for log axis
    /// </summary>
    class LogarithmicCrossSign
    {
        /// <summary>
        /// Creates a cross mark over a node
        /// </summary>
        /// <param name="p"></param>
        /// <param name="chart1"></param>
        /// <param name="nodeSize"></param>
        /// <param name="nodeID"></param>
        /// <param name="nodeColor"></param>
        /// <returns>List of series having tow series name of cross mark</returns>
        public static List<string> CrossSign(double x,double y,Chart chart1 ,int nodeSize,string nodeID,Color nodeColor)
        {
            
            double radius = nodeSize / 2;
            //double y_Up=0, y_Down=0;

            double xLeft = x + radius;
            double xRight = x- radius;
            double logVal = System.Math.Log10(radius);
            //0.001 ymin and xmin
            double yScale = 0;
            if (y >= 0.001 && y< 0.01)
            {
                yScale = 0.0005;
            }
            else if(y >= 0.01 && y < 0.1)
            {
                yScale = 0.005;
            }
            else if (y >= 0.1 && y < 1)
            {
                yScale = 0.05;
            }
            else
            {
                yScale =  y/10;
            }
            double yUp = y +0.25*y;
            double yDown = y - yScale;
            if(yUp> 100)
            {
                yUp = 100;
            }

            if (yDown < 0.001)
            {
                yDown = 0.001;
            }

            //Drawing function
            Series seriesLeft = new Series(nodeID + "_a");
            Series seriesRight = new Series(nodeID + "_b");
            Color sampleColor = Color.Red;
            if(nodeColor == Color.Red)
            {
                sampleColor = Color.Black;
            }

            DrawCross(xLeft, yUp, xRight, yDown, chart1, seriesLeft, sampleColor);
            DrawCross(xRight, yUp, xLeft, yDown, chart1, seriesLeft, sampleColor);

            //--Storing this in a list provided
            var list = new List<string>();
            list.Add(seriesLeft.Name);
            list.Add(seriesRight.Name);
            return list;
        }

        public static void DrawCross(double x1, double y1,double x2,double y2,Chart chart1,Series newSeries,Color c)
        {
            newSeries.ChartType = SeriesChartType.Line;        
            newSeries.MarkerSize = 5;
            newSeries.Points.AddXY(x1,y1);        
            newSeries.Color = c;
            chart1.Series.Add(newSeries);   
        }
    }
}
