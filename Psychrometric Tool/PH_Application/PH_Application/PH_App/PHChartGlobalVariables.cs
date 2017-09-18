using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{

    /// <summary>
    /// Contains all the event operations
    /// </summary>
   public class PHChartGlobalVariables
    {
        //--These are the chart properties.
        //public int XAxisMinimum { get; set; }
        //public int XAxisMaximum { get; set; }
        //public int YAxisMinimum { get; set; }
        //public int YAxisMaximum { get; set; }


        //=============Variables from nodeandlineclass ===================
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
            //public string lastUpdatedDate { get; set; }
            public string lastUpdatedXValue { get; set; }
            public string lastUpdatedYValue { get; set; }

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
      public  List<string> listNodeSeriesPlotted = new List<string>();

        public int previousNodeIndexForLineInput = 0;
        public int index = 0;
        public int indexRun = 0;
        public int lineDefaultThickness = 3;
        public  int nodeDefaultThickness = 20;
        //--variable for storing the indexOfThePoints so that we can gather other properties..
        public string indexOfPrevPointForLineMovement = "";
        public string indexOfNextNodeForLineMovement = "";


        public Series series1 = new Series("My Series"); //This series for plotting node 
        public int indexForSeriesNodePoint = 0;
        //public Chart chart1 { get; set; }
       public  Series newLineSeries;//--This is temporary for storing series name

      // public int FlagForNodeDelete = 0;//0 means node not selected 1 means selected
      //public  string nodeID_ForDeletingNode = "";
        //=====================End of varaibles form node and line classs===========//



        //===================Variables form chartoperations and =======//
        //dotted series when the mouese is moved
        public Series addDottedSeries = new Series("newdottedSeries");


        //--Mouse pointer location Info
        public double xCoordinateValue = 0;
        public double yCoordinateValue = 0;
        public Point? prevPosition = null;

        //variables
        public double xAxis1;
        public double yAxis1;
        public int load = 0;//false
        public string idOfNodeSelected;//--initially it represents nothing...
        public int indexOfLineInTheList = 0; //This holds the id of the line selected.
        public string idSelected;
        public int FlagForNodeDelete = 0;//0 means node not selected 1 means selected
        public string nodeID_ForDeletingNode = "";
        public int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..
        public int mouseClickAction = 0;
        public int incrementIndex = 0;//--Defining the index
        public string setItemSelectedID;
        public int oneTimeClick = 1;
        public int flagResistingForDGVChangeSelection = 0;//disENABLE;



        //Current pointer location
        public double currentXAxis = 0.000;
        public double currentYAxis = 0.000;

        //--Flags decleration over here
        public int FlagForDisconnectingLineChoice = 0;// 0 means off and 1 means on
        public int FlagForDissableLeftAndRightClicksInChart = 0;//==do not dissable /OFF
        public int flagForDisconnectClick = 0;//0 means false it is used to see if the disconnect option is clicked or not.
        public int flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 0;//-- 0 means off 1 means on for previous node    and 2 means next node
        public  int flagNodeSelectedForConnect = 0;
        //int indexForSeriesNodePoint = 0;

        public int tempIndexForNode = 0;

        //=================End of variables from chart operation========//


        //==========Variables form MainController class=============//




        //==============Varibales form building Operation=====//

        // public int flagResistingForDGVChangeSelection = 0;//disENABLE;
        public string beginEditText = "";
        public int flagSinglCellClick = 0;
        public int FlagForCntdBG_Update = 1;//On currently
        public System.Timers.Timer atimer;
        public int flagForTimer = 0;
        public int FlagForLockUnlock = 0;//0 means lock and 1 means unlock
       // public int FlagForDissableLeftAndRightClicksInChart = 0;//==do not dissable /OFF
       // public int indexRun = 0;
        public int FlagForStopingRunningProcessInstantly = 0;//Currently OFF
        public int flagForInsertOrUpdateDataToDB = 0;

        public Form_Main_PH_Application frm1;//This one is for backgroundworker 

       // public int incrementIndex = 0;
        public int indexForWhichChartIsSelected = 0;//--This index value stores the index value in a list of which chart is selected
        public int indexOfChartSelected = 0;
        //int index=0;

        //=============Begning of the variables in the mainController class===//
        // public double AirPressureFromDB = 0;
        public double AirPressureFromDB { get; set; }
        public string CurrentSelectedBuilding { get; set; }
        public string PathToT3000BuildingDB { get; set; }

        //===========End of varaibales in main controllers=======//

        //==========END OF VARIABLES FORM BUILDING OPERATION===//



        //============Variable form main controllerclass===========//



         //=====Methods not used=======//
         //NodeAndLineClass nlc = new NodeAndLineClass();

        public int CountNode(List<DataTypeForNode> listNode)
        {
            int t = listNode.Count;
            int indexRun = 1;
            if (t == 0)
            {
                indexRun = 1;
            }
            else
            {
                //--we will use lambda expression
                indexRun += listNodeInfoValues.Count(Node => Node.ID != "");
            }
            return indexRun;
        }
        /// <summary>
        /// For finding inital node name
        /// </summary>
        /// <returns>return one unique name of node eg. Node1, Node2,etc</returns>
        public string NodeName(int indexRunEndCount, List<DataTypeForNode> listNode)
        {
            //int indNodeRun = indexRun;
            string nodeName = $"Node{indexRunEndCount}";
            int countItemPresent = listNode.Count(x => x.name == nodeName);
            string newNodeName = $"Node{indexRunEndCount + countItemPresent}";
            return newNodeName;
        }

        public string LineName(int indexRunEndCount, List<DataTypeForLine> listLine)
        {
            //int indNodeRun = indexRun;
            string lineName = $"Line{indexRunEndCount - 1}";//-1 because for two node we have one line
            int countItemPresent = listLine.Count(x => x.name == lineName);
            string newLineName = $"Line{indexRunEndCount - 1 + countItemPresent}";
            return newLineName;
        }
        //==End method not used=======//
    }


}
