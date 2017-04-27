using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    class NodeAndLineClass
    {
        public class DataTypeForNode
        {
            public string ID { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            public double yVal { get; set; }
            public string temperature_source { get; set; }
            public string pressure_source { get; set; }
            public string name { get; set; }
            public Color colorValue { get; set; }
            public int marker_Size { get; set; }
            public string lastUpdatedDate { get; set; }


        }
        public List<DataTypeForNode> listNodeInfoValues = new List<DataTypeForNode>();


        public class DataTypeForLine
        {
            //--Line ID
            public string ID { get; set; }
            public string prevNodeId { get; set; }
            public string nextNodeId { get; set; }
            public Color lineColorValue { get; set; }
            public Series lineSeriesID { get; set; }
            public int lineThickness { get; set; }
            public string name { get; set; }
            public int status { get; set; }
        }

        public List<DataTypeForLine> listLineInfoValues = new List<DataTypeForLine>();

    }
}
