using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{

    /// <summary>
    /// Contains all the event operations
    /// </summary>
    class PHChartOperation
    {
        //--These are the chart properties.
        public int XAxisMinimum { get; set; }
        public int XAxisMaximum { get; set; }
        public int YAxisMinimum { get; set; }
        public int YAxisMaximum { get; set; }
        NodeAndLineClass nlc = new NodeAndLineClass();

        public int  CountNode(List<NodeAndLineClass.DataTypeForNode> listNode)
        {            
            int t = listNode.Count;
            int indexRun = 1;           
            if (t == 0){
                indexRun = 1;
            }else {           
                //--we will use lambda expression
                indexRun +=  nlc.listNodeInfoValues.Count(Node => Node.ID != "");
            }
            return indexRun;
        }
        /// <summary>
        /// For finding inital node name
        /// </summary>
        /// <returns>return one unique name of node eg. Node1, Node2,etc</returns>
        public string NodeName(int indexRunEndCount, List<NodeAndLineClass.DataTypeForNode> listNode)
        {
            //int indNodeRun = indexRun;
            string nodeName = $"Node{indexRunEndCount}";
            int countItemPresent = listNode.Count(x => x.name == nodeName);
            string newNodeName = $"Node{indexRunEndCount + countItemPresent}";
            return newNodeName;
        }

        public string LineName(int indexRunEndCount, List<NodeAndLineClass.DataTypeForLine> listLine)
        {
            //int indNodeRun = indexRun;
            string lineName = $"Line{indexRunEndCount-1}";//-1 because for two node we have one line
            int countItemPresent = listLine.Count(x => x.name == lineName);
            string newLineName = $"Line{indexRunEndCount-1 + countItemPresent}";
            return newLineName;
        }

       



    }
}
