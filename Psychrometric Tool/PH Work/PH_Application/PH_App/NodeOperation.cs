using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    /// <summary>
    /// This class will do the function of insert node operations
    /// </summary>
    class NodeOperation
    {
        /// <summary>
        /// Sets the node to Chart 
        /// </summary>
        /// <param name="series1"></param>
        /// <param name="xAxisValue"></param>
        /// <param name="yAxisValue"></param>
        /// <param name="nodeName"></param>
        /// <param name="c1"></param>
        /// <param name="markerSize"></param>
        public void SetNode(Series series1, int nodePointCount, double xAxisValue, double yAxisValue, string nodeName, Color colorValue, int markerSize, string temperatureSource, string pressureSource)
        {
            series1.ChartType = SeriesChartType.Point;
            series1.MarkerStyle = MarkerStyle.Circle;
            series1.Points.AddXY(xAxisValue, yAxisValue);
            //string nodeTips = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";
            series1.Points[nodePointCount].Color = colorValue;
            series1.Points[nodePointCount].ToolTip = $"source :\nTemperature Source  {temperatureSource}   \nPressure Source { pressureSource } \n Name : {nodeName}";//nodeTips;

        }



    }
}
