using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    class LineOperation
    {

        //--Working on it...not complete
        public void SetLine(Series series1, int nodePointCount, double xAxisValue, double yAxisValue, string nodeName, Color colorValue, int markerSize, string temperatureSource, string pressureSource)
        {
            series1.ChartType = SeriesChartType.Point;
            series1.MarkerStyle = MarkerStyle.Circle;
            series1.Points.AddXY(xAxisValue, yAxisValue);
            //string s = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";
            series1.Points[nodePointCount].Color = colorValue;
            series1.Points[nodePointCount].ToolTip = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";//s;
        }

    }
}
