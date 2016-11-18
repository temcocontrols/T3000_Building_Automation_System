using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Xml;
using System.Xml.Linq;

namespace WFA_psychometric_chart
{
    public partial class buildingChartSetting : Form
    {
        Form1_main form1Object;
        public buildingChartSetting(Form1_main f)
        {
            this.form1Object = f;
            InitializeComponent();
        }

        ArrayList t = new ArrayList();//this stores the temperature(deg.cel)
        ArrayList pg = new ArrayList();//this stores the saturated vapour pressure(kpa).

        //this array list is used in the web part where we pull the value from an api and store in this array list
        ArrayList temp_AL = new ArrayList();
        ArrayList hum_AL = new ArrayList();

        //this is for updateing the values constantly
        ArrayList temp2_AL = new ArrayList();
        ArrayList hum2_AL = new ArrayList();

        double temperature, humidity, Patm, TDewpoint, A, m, Tn, B, Pws, X, h;
        int index = 0;
        ToolTip tp = new ToolTip();

        //--Flags are defined here...
        int flagForDisconnectClick = 0;//0 means false it is used to see if the disconnect option is clicked or not.
        int flagNodeSelectedForConnect = 0;

        //--variable for storing the indexOfThePoints so that we can gather other properties..
        string indexOfPrevPointForLineMovement;

        //int setItemSelectedID = 0;
        string setItemSelectedID;
        int oneTimeClick = 1;
        //int twoTimeClick = 0;
        //int incrementIndex = 0;
        int mouseClickAction = 0;

        string idSelected;
        int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..

        bool arrowOn = false;

        //--All the series decleared here..
        Series series1 = new Series("My Series");

        //This series is for temporary line drawing for line movements...
        Series addDottedSeries = new Series("newSeries101");
        //--If series is present delete the previouse series

        //--id of the node selected, this is used when line is detached and for reconnecting we need to know which node we are connecting to.
        string idOfNodeSelected;//--initially it represents nothing...

        double xAxis1;
        double yAxis1;

        int load = 0;//false

        //--This is used in redrawline function
        double humidityCalculated = 0;
        double enthalpyCalculated = 0;

        Series newLineSeries;//--This is temporary for storing series name

        //--this is used by set data button
        int countNumberOfPoints = 0;
        int xCoord = 0;



        Point? prevPosition = null;

        double currentXAxis = 0.000;
        double currentYAxis = 0.000;

        //--Seres used
        Series series1xx = new Series("My Series values plot ");//this series is used by plot_on_graph_values() method...
        Series seriesLineIndicator = new Series("LineIndicator");//--This line indicator is temporary show the line in the chart for Node Movement.

        //--Flag for inserting or updating data to databse.
        int flagForInsertOrUpdateDataToDB = 0;


        //Current node table selected and line table selected..
        string currentNodeTableFromDB = null;
        string currentLineTableFromDB = null;

        private void buildingChartSetting_Load(object sender, EventArgs e)
        {

            chart1.Series.Add(series1);
            chart1.Series.Add(series1xx);
            chart1.Series.Add(seriesLineIndicator);
            //--Lets add some data 
            add_t_pg();

            //--Load the chart 
            plot_new_graph();

            //==========================For building infor==========//

            FindPathOfBuildingDB();
            //==========================end of building info========//


            //--For datagridview1 editing option.
            // readOnlyModeSetUp();
            /*
            Steps :
            0.Check which DB is selected and display that value also store it in a variable...
            1.Create required DB if not present 
            2.If DB presnet then pull the values to display 
            */

            //===============================Building Selection starts=========================//
            CheckSelectedBuilding();
            string buildingNameValue = selectedBuildingList[0].BuildingName;
            lb_db_name.Text = buildingNameValue;
            CreateRequireTableIfNotPresent(buildingNameValue);

            DataGridView_Show_Data();
            dataGridView1.Rows.Add();
            //loading the comfortzone when set             
            //This to prevent anonomous plotting
            chart1.Enabled = false;
            //==============================end of building selection starts==================//
        }






        public class SelectedBuildingDatatype
        {
            public string Main_BuildingName { get; set; }
            public string Building_Name { get; set; }
            public string Building_Path { get; set; }
        }
        List<SelectedBuildingDatatype> BuildingSelected = new List<SelectedBuildingDatatype>();

        public void FindPathOfBuildingDB()
        {
            try
            {
                BuildingSelected.Clear();
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.mdb";
                // MessageBox.Show("New path : " + againDbPath);
                // bool returnValue = false;
                //string latValue = "";
                using (OleDbConnection connection = new OleDbConnection(againDbPath))
                {
                    connection.Open();
                    OleDbDataReader reader = null;
                    string queryString = "SELECT * from Building WHERE Default_SubBuilding = -1 ";//-1 or True  can be used
                    OleDbCommand command = new OleDbCommand(queryString, connection);

                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {

                        BuildingSelected.Add(new SelectedBuildingDatatype
                        {
                            Main_BuildingName = reader["Main_BuildingName"].ToString(),
                            Building_Name = reader["Building_Name"].ToString(),
                            Building_Path = reader["Building_Path"].ToString()
                        });
                    }
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            //MessageBox.Show("count = " + BuildingSelected.Count);
        }



        /// <summary>
        /// Declearing the variable for storing the selected building information
        /// </summary>
        private class SelectedBuildingDT
        {
            public int ID { get; set; }
            public string country { get; set; }

            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
            public int ZIP { get; set; }
            public double longitude { get; set; }
            public double latitude { get; set; }
            public double elevation { get; set; }
            public string BuildingName { get; set; }  //This one is what we are interested in ....
        }
        List<SelectedBuildingDT> selectedBuildingList = new List<SelectedBuildingDT>();
        public void CheckSelectedBuilding()
        {


            selectedBuildingList.Clear();//Reset the values first ...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from tbl_building_location where selection = 1 ";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                // command.Parameters.AddWithValue("@selection_value", id);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //Now lets add to the list 
                    selectedBuildingList.Add(new SelectedBuildingDT
                    {
                        ID = int.Parse(reader["id"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString(),
                        street = reader["street"].ToString(),
                        ZIP = int.Parse(reader["ZIP"].ToString()),
                        longitude = double.Parse(reader["longitude"].ToString()),
                        latitude = double.Parse(reader["latitude"].ToString()),
                        elevation = double.Parse(reader["elevation"].ToString()),
                        BuildingName = reader["BuildingName"].ToString()
                    });
                }
            }//close of using..
        }



        public void CreateRequireTableIfNotPresent(string buildingName)
        {
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string tableForNode = "tbl_" + buildingName + "_node_value";
            string tableForLine = "tbl_" + buildingName + "_line_value";
            string tableFordevice = "tbl_" + buildingName + "_device_info_for_node";

            //These two tables are for comfort zone 
            string tableforComfortZoneDetail = "tbl_" + buildingName + "_comfort_zone_detail";
            string tableForChartComfortZoneSetting = "tbl_" + buildingName + "_chart_comfort_zone_setting";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),source varchar(255),name varchar(255),label varchar(255),colorValue varchar(255),showTextItem varchar(255),nodeSize varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID varchar(255),IP varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();

                //These tables are for comfort zone...
                string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                command6.ExecuteNonQuery();


                string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                command7.ExecuteNonQuery();


            }

        }

        public class chartDetailDT
        {
            public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
        }
        public List<chartDetailDT> chartDetailList = new List<chartDetailDT>();//This is used for storing the chart detail ids

        public void PullChartList(string buildingName)
        {

            chartDetailList.Clear();//resetting the chart list value..
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "select * from " + tableForChartDetail + " ";

                SQLiteCommand command = new SQLiteCommand(sql, connection);

                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    chartDetailList.Add(new chartDetailDT
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chartID = reader["chartID"].ToString(),
                        chartName = reader["chartName"].ToString(),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        chart_respective_lineID = reader["chart_respective_lineID"].ToString()

                    });
                }
            }

        }  //Close of the function  PullChartList

        //For storing node and line detail we need a dynamic list 
        public class nodeDataType
        {
            public int count { get; set; }
            public string chart_respective_nodeID { get; set; } //This is the chartID for grouping the node for single node values.
            public string nodeID { get; set; }  //This is the individual node id for each node 
            public double xValue { get; set; }
            public double yValue { get; set; }
            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public string colorValue { get; set; }
            public string showTextItem { get; set; }
            public int nodeSize { get; set; }

        }

        List<nodeDataType> nodeList = new List<nodeDataType>();

        public class lineDataType
        {
            public int count { get; set; }
            public string chart_respective_lineID { get; set; }
            public string lineID { get; set; }
            public string prevNodeID { get; set; }
            public string nextNodeID { get; set; }
            public string lineColorValue { get; set; }
            public string lineSeriesID { get; set; }
            public string thickness { get; set; }

        }
        List<lineDataType> lineList = new List<lineDataType>();


        /// <summary>
        /// Getting the values of the database on the chartID
        /// </summary>
        public void ReadNodeAndLineDetails(string chart_respective_node_id, string chart_respective_line_id, string buildingName)
        {
            nodeList.Clear();//resetting the node value
            lineList.Clear();//Resetting the line value
            string tableForNode = "tbl_" + buildingName + "_node_value";
            string tableForLine = "tbl_" + buildingName + "_line_value";


            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                //This is done because for each different building we need different
                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForNode + " where chart_respective_nodeID = " + chart_respective_node_id;
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    nodeList.Add(new nodeDataType
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        nodeID = reader["nodeID"].ToString(),
                        xValue = double.Parse(reader["xValue"].ToString()),
                        yValue = double.Parse(reader["yValue"].ToString()),
                        source = reader["source"].ToString(),
                        name = reader["name"].ToString(),
                        label = reader["label"].ToString(),
                        colorValue = reader["colorValue"].ToString(),
                        showTextItem = reader["showTextItem"].ToString()

                    });
                }



                //This is for the line 
                string sql1 = "select * from " + tableForLine + " where chart_respective_nodeID = " + chart_respective_line_id;
                SQLiteCommand command1 = new SQLiteCommand(sql1, connection);
                SQLiteDataReader reader1 = command1.ExecuteReader();
                while (reader1.Read())
                {
                    //This is the reading part of the data...
                    lineList.Add(new lineDataType
                    {
                        count = int.Parse(reader1["count"].ToString()),
                        chart_respective_lineID = reader1["chart_respective_lineID"].ToString(),
                        lineID = reader1["lineID"].ToString(),
                        prevNodeID = reader1["prevNodeID"].ToString(),
                        nextNodeID = reader1["nextNodeID"].ToString(),
                        lineColorValue = reader1["lineColorValue"].ToString(),
                        lineSeriesID = reader1["lineSeriesID"].ToString(),
                        thickness = reader1["thickness"].ToString()

                    });
                }

            }
        }


        public void add_t_pg()
        {
            //Adding values in t and pg.
            t.Add(0.01); pg.Add(0.61165);
            t.Add(1.00); pg.Add(0.65709);
            t.Add(2.00); pg.Add(0.70599);
            t.Add(3.00); pg.Add(0.75808);
            t.Add(4.00); pg.Add(0.81355);
            t.Add(5.00); pg.Add(0.87258);
            t.Add(6.00); pg.Add(0.93536);
            t.Add(7.00); pg.Add(1.00210);
            t.Add(8.00); pg.Add(1.07300);
            t.Add(9.00); pg.Add(1.14830);
            t.Add(10.00); pg.Add(1.22820);
            t.Add(11.00); pg.Add(1.31300);
            t.Add(12.00); pg.Add(1.40280);
            t.Add(13.00); pg.Add(1.49810);
            t.Add(14.00); pg.Add(1.59900);
            t.Add(15.00); pg.Add(1.70580);
            t.Add(16.00); pg.Add(1.81880);
            t.Add(17.00); pg.Add(1.93840);
            t.Add(18.00); pg.Add(2.06470);
            t.Add(19.00); pg.Add(2.19830);
            t.Add(20.00); pg.Add(2.33930);
            t.Add(21.00); pg.Add(2.48820);
            t.Add(22.00); pg.Add(2.64530);
            t.Add(23.00); pg.Add(2.81110);
            t.Add(24.00); pg.Add(2.98580);
            t.Add(25.00); pg.Add(3.16990);
            t.Add(26.00); pg.Add(3.36390);
            t.Add(27.00); pg.Add(3.56810);
            t.Add(28.00); pg.Add(3.78310);
            t.Add(29.00); pg.Add(4.00920);
            t.Add(30.00); pg.Add(4.24700);
            t.Add(31.00); pg.Add(4.49690);
            t.Add(32.00); pg.Add(4.75960);
            t.Add(33.00); pg.Add(5.03540);
            t.Add(34.00); pg.Add(5.32510);
            t.Add(35.00); pg.Add(5.62900);
            t.Add(36.00); pg.Add(5.94790);
            t.Add(37.00); pg.Add(6.28230);
            t.Add(38.00); pg.Add(6.63280);
            t.Add(39.00); pg.Add(7.00020);
            t.Add(40.00); pg.Add(7.38490);
            t.Add(41.00); pg.Add(7.78780);
            t.Add(42.00); pg.Add(8.20960);
            t.Add(43.00); pg.Add(8.65080);
            t.Add(44.00); pg.Add(9.11240);
            t.Add(45.00); pg.Add(9.59500);
            t.Add(46.00); pg.Add(10.09900);
            t.Add(47.00); pg.Add(10.62700);
            t.Add(48.00); pg.Add(11.17700);
            t.Add(49.00); pg.Add(11.75200);
            t.Add(50.00); pg.Add(12.35200);
        }


        int countIndexForChart = 0;
        int map_loaded = 0;
        int indexI = 0;//--This is used for another purpose
        public void plot_new_graph()
        {

            /*
              steps:
              * 1.set x and y axis in graph
              * 2.plot red lines
              * 3.plot green lines
              * 4.plot wet bult curve red line.
              * 5.
              * 
              */

            countIndexForChart = 1;

            indexI = 0;//resetting the index value....
            //lets reset the chart1 value.
            //chart1 = null;
            map_loaded = 1;
            foreach (var series in chart1.Series)
            {
                series.Points.Clear();
            }

            //first we need variables to plot in the chart....
            chart1.Series["Series1"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            chart1.Series["Series1"].Color = Color.Red;

            chart1.Series["Series2"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series2"].Color = Color.Blue;

            chart1.Series["Series3"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series3"].Color = Color.Blue;
            chart1.Series["Series4"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series4"].Color = Color.Blue;
            chart1.Series["Series5"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series5"].Color = Color.Blue;

            chart1.Series["Series6"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series6"].Color = Color.Blue;
            chart1.Series["Series7"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series7"].Color = Color.Blue;


            //setting the boundary xaxis =0-30 and yaxis = 0-50..
            chart1.ChartAreas[0].AxisX.Minimum = 0;
            chart1.ChartAreas[0].AxisX.Maximum = 50;
            chart1.ChartAreas[0].AxisY.Minimum = 0;
            chart1.ChartAreas[0].AxisY.Maximum = 30;
            chart1.ChartAreas[0].AxisY.Interval = 5;
            chart1.ChartAreas[0].AxisX.Interval = 5;
            //we need to plot the chart so lets build an array where we can hold the data
            //form text file..



            //now lets read from the text file...
            string line1;

            //now lets test the value..
            string s = "";
            for (int i = 0; i < t.Count; i++)
            {
                s += t[i].ToString() + "," + pg[i].ToString() + "\n";
            }

            // MessageBox.Show(""+s);

            //now we have the data lets do some ploting...

            //===========NOTE WE HAVE PRESSURE IN Pa but this chart users it in terms of kpa===//
            /*
            Later we need to user it in terms of HPa (heteropascal) in formula so we need conversion
            */
            double pressureConverted = form1Object.AirPressureFromDB * 0.001;//now in terms of kpa
                                                                             // MessageBox.Show("pressure(kpa) = " + pressureConverted);
            lb_pressure_display.Text = "Pressure : " + Math.Round(pressureConverted, 2) + " KPa";//in terms of kpa
            double patm = pressureConverted;//101.235;//constant..we will make it take as input later...   in 

            //double patm = form1Object.AirPressureFromDB * 0.001;//this in terms of kpa now  //101.235;//constant..we will make it take as input later...
            double rair = 0.287;//rideburg constant i guess

            //now we need array list to hold the values form calculation formula..
            ArrayList wg = new ArrayList();
            //now we need equation to calculate..
            //here pg_value contains pg which is in arraylist ..            
            //double pg_value=0; 

            double wg_calc = 0; // = 622 * a_value / (patm - pg_value);
            //now calculation begins..
            double x = 0;
            for (int i = 0; i < t.Count; i++)
            {
                double pg_value = Double.Parse(pg[i].ToString());
                wg_calc = 622 * pg_value / (patm - pg_value);
                x = double.Parse(t[i].ToString());
                double y = wg_calc;
                chart1.Series["Series1"].Points.AddXY(x, y);



            }//close of for

            chart1.Series["Series1"].Points[16].Label = "Wet Bulb Temp";
            chart1.Series["Series1"].Points[16].LabelBackColor = Color.Red;
            //chart1.Legends["wet_bulb_temp"].DockedToChartArea = "jjj";

            //now lets plot the blue curves...
            //WE NEED two for loop to plot values for 10% to 40%
            double phi = 0.1;
            //int index = 0;
            double x2 = 0;
            int ival = 2;
            for (phi = 0.1; phi <= 0.4; phi += 0.1)
            {
                //   chart1.Series["Series1"].Color = Color.Blue;
                string s1 = "";

                for (int i = 0; i < t.Count; i++)
                {

                    double pg_value = Double.Parse(pg[i].ToString());
                    wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
                    //double x = Double.Parse(t[i].ToString());
                    double y = wg_calc;
                    x2 = double.Parse(t[i].ToString());
                    chart1.Series["Series" + ival].Points.AddXY(x2, y);
                    s1 += x2 + "," + y + ";";


                    //index++;
                }//close of for
                //MessageBox.Show(s1);
                ival++;
                //this is to print 10%,20,30,40% 
                int c = int.Parse((phi * 10 + 1).ToString());
                //chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                //chart1.Series["Series"+c].Points[46].LabelBackColor = Color.Blue;
                if (phi >= 0.30 && phi < 0.4)
                {
                    chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                    chart1.Series["Series" + c].Points[42].Label = "Relative Humidity";
                    // MessageBox.Show("Hello");
                }
                else
                {
                    chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                }
            }

            //for plotting 60%-80%  
            int ival2 = 6;
            for (phi = 0.6; phi <= 0.8; phi += 0.2)
            {
                //   chart1.Series["Series1"].Color = Color.Blue;
                string s1 = "";

                for (int i = 0; i < t.Count; i++)
                {

                    double pg_value = Double.Parse(pg[i].ToString());
                    wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
                    //double x = Double.Parse(t[i].ToString());
                    double y = wg_calc;
                    x2 = double.Parse(t[i].ToString());
                    chart1.Series["Series" + ival2].Points.AddXY(x2, y);
                    s1 += x2 + "," + y + ";";

                    //index++;
                }//close of for
                //MessageBox.Show(s1);
                ival2++;
                //this is to print 60% and 80%
                int c = 0;
                if (phi == 0.6)
                {
                    c = 6;
                }
                else
                {
                    c = 7;
                }
                chart1.Series["Series" + c].Points[33].Label = phi * 100 + "%";

            }



            /*Now towards next part ie plotting the red lines with 30 deg angle..*/
            //% specific volume and enthalpy/wet-bulb-temp

            ArrayList t1 = new ArrayList();//this stores the temperature(deg.cel)
            ArrayList pg1 = new ArrayList();//this stores the saturated vapour pressure(kpa).



            //now lets read from the text file...
            string line2;

            //--Creating second file...
            for (int i = 0; i < t.Count; i++)
            {
                if (double.Parse(t[i].ToString()) == 5.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 10.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 15.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 20.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 25.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 30.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }
                else if (double.Parse(t[i].ToString()) == 35.00)
                {
                    t1.Add(t[i]);
                    pg1.Add(pg[i]);
                }

            }//--CLOSE OF THE FOR LOOP 


            //this part is finished... now towards calcuation...
            ArrayList wg1 = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double tempval = double.Parse(pg1[i].ToString());
                double tempwg1 = 622 * tempval / (patm - tempval);
                wg1.Add(tempwg1);
            }

            //specific volume of dry air  (cubic m/kg dry air) (green)

            ArrayList vol = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double temppg1 = double.Parse(pg1[i].ToString());
                double tempt1 = double.Parse(t1[i].ToString());
                double temp = rair * (tempt1 + 273) / (patm - temppg1);
                vol.Add(temp);
            }

            //% air temperature at zero humidity

            ArrayList tv0 = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double tempvol = double.Parse(vol[i].ToString());

                double temp = patm * tempvol / rair - 273;
                tv0.Add(temp);
            }

            //now lets plot..
            double xtemp = 0.79;
            for (int i = 0; i < 7; i++)
            {
                //chart1.Series.Add("Line"+i);//this series is added statically from chart control so no need to add dynamically 
                chart1.Series["Line" + i].Color = Color.Green;
                chart1.Series["Line" + i].Points.Add(new DataPoint(double.Parse(t1[i].ToString()), double.Parse(wg1[i].ToString())));
                chart1.Series["Line" + i].Points.Add(new DataPoint(double.Parse(tv0[i].ToString()), 0));
                chart1.Series["Line" + i].ChartType = SeriesChartType.Line;
                chart1.Series["Line" + i].Points[1].Label = xtemp + "";
                xtemp = xtemp + 0.02;

            }


            //now towards the plotting of next part....
            //% wet bulb temperature and enthalpy lines (red)
            //arraylist for storing h 


            ArrayList h = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double tempval1 = double.Parse(t1[i].ToString());
                double tempval2 = double.Parse(wg1[i].ToString());

                double temp = tempval1 + 2.5 * tempval2;
                h.Add(temp);
            }

            //temperature at zero humidity..
            ArrayList t0 = new ArrayList();
            t0 = h;
            int t_plot_value = 5;
            for (int i = 0; i < 6; i++)
            {
                //for plotting different lines 
                //  chart1.Series.Add("Line_r" + i);//this series is added statically from chart control so no need to add dynamically 
                chart1.Series["Line_r" + i].Color = Color.Red;
                chart1.Series["Line_r" + i].Points.Add(new DataPoint(double.Parse(t1[i].ToString()), double.Parse(wg1[i].ToString())));
                chart1.Series["Line_r" + i].Points.Add(new DataPoint(double.Parse(t0[i].ToString()), 0));
                chart1.Series["Line_r" + i].ChartType = SeriesChartType.Line;
                //ploting the values 5,10deg c...
                chart1.Series["Line_r" + i].Points[0].Label = t_plot_value + "deg c";
                t_plot_value += 5;
            }


            //toward next part...
            // enthalpy axis and enthalpy lines (black)
            //testing values of h ,t1,t0
            string hv = " ";
            string t0v = "";
            string t1v = "";
            for (int i = 0; i < h.Count; i++)
            {
                hv += " " + h[i] + "; ";
                t0v += " " + t0[i] + ";";
                t1v += " " + t1[i] + ";";

            }
            //MessageBox.Show("h=  " + hv + " \n t0 =" + t0v + " \n t1 = " + t1v);

            int t_plot1 = 10;
            for (int hval = 10; hval <= 110; hval += 10)
            {
                //% temperature on enthalpy axis
                double t1_temp = (hval - 12.5) / 3.5;//this is t1;
                double w1_temp = t1_temp + 5;//% specific humidity on enthalpy axis
                int t0val = hval;//t0
                //chart1.Series.Add("Line_b" + hval);//this series is added statically from chart control so no need to add dynamically 
                chart1.Series["Line_b" + hval].Color = Color.Black;
                chart1.Series["Line_b" + hval].Points.Add(new DataPoint(t0val, 0));
                chart1.Series["Line_b" + hval].Points.Add(new DataPoint(t1_temp, w1_temp));
                chart1.Series["Line_b" + hval].ChartType = SeriesChartType.Line;
                chart1.Series["Line_b" + hval].Points[1].Label = t_plot1 + "";
                t_plot1 += 10;
                if (t_plot1 == 60)
                {
                    chart1.Series["Line_b" + hval].Points[1].Label = "Enthalpy kj/kg dry air";
                }

            }
            //now plotting the black straight  line...
            //chart1.Series.Add("Line_b_straight");//this series is added statically from chart control so no need to add dynamically 
            chart1.Series["Line_b_straight"].Color = Color.Black;
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(0, 5));
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(25, 30));
            chart1.Series["Line_b_straight"].ChartType = SeriesChartType.Line;
            //chart1.Series["Line_b_straight"].Points[0].Label = "Enthalpy kj/kg dry air";
        }   //--Close of plot new graph


        public void DataGridView_Show_Data()
        {
            //check to see if the data is present in the database or not...
            //--Pulls and fills in a dynamic list
            //   PullDataFromDB();
            //Pulling the data form the data grid view...
            PullChartList(selectedBuildingList[0].BuildingName);//This is the list of the building present ...


            //            MessageBox.Show("datapulled ,count =  " + listForDataFromDB.Count);

            //This fills the datagridview
            fillDataGridView();
            //     fillDataGridView_WithChartList();

        }

        private class listDataType
        {
            public string id { get; set; }
            public string name { get; set; }
            public string node_table { get; set; }
            public string line_table { get; set; }
        }
        //--Lets have a list to add the data to..
        List<listDataType> listForDataFromDB = new List<listDataType>();
        public void PullDataFromDB()
        {

            ////Reading data from the database

            ////--Clearing the list data..
            //listForDataFromDB.Clear();

            ////--Database detials           
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            //string connString = @"Data Source=" + databaseFile + ";Version=3;";

            //using (SQLiteConnection connection = new SQLiteConnection(connString))
            //{
            //    connection.Open();
            //    SQLiteDataReader reader = null;
            //    string queryString = "SELECT *  from tbl_air_handler_details";
            //    SQLiteCommand command = new SQLiteCommand(queryString, connection);
            //    //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

            //    reader = command.ExecuteReader();
            //    while (reader.Read())
            //    {

            //        listForDataFromDB.Add(new listDataType
            //        {
            //            id = reader["id"].ToString(),
            //            name = reader["name"].ToString(),
            //            node_table = reader["node_table"].ToString(),
            //            line_table = reader["line_table"].ToString(),
            //        });
            //    }
            //}//close of using..



        } //close of the function pulldatafromdb

        private void fillDataGridView()
        {
            //if (listForDataFromDB.Count > 0)
            //{
            //    for (int i = 0; i < listForDataFromDB.Count; i++)
            //    {
            //        dataGridView1.Rows.Add();
            //        dataGridView1.Rows[i].Cells["ID"].Value = listForDataFromDB[i].id;
            //        dataGridView1.Rows[i].Cells["Name"].Value = listForDataFromDB[i].name;

            //    }
            //}//--if close

            //lb_numberOfHandler.Text = listForDataFromDB.Count.ToString();

            int xCount = 0;
            if (chartDetailList.Count > 0)
            {
                for (int i = 0; i < chartDetailList.Count; i++)
                {
                    dataGridView1.Rows.Add();
                    dataGridView1.Rows[i].Cells["ID"].Value = ++xCount;//chartDetailList[i].;
                    dataGridView1.Rows[i].Cells["Name"].Value = chartDetailList[i].chartName;

                }
            }//--if close

            lb_numberOfHandler.Text = chartDetailList.Count.ToString();


        }//--close fo filldatagridview

        private void NodeSelectionAndRelease(MouseEventArgs e)
        {//this is used to select the partciular id values..


            if (readyForMouseClick == 1)
            {

                if (oneTimeClick == 1)
                {

                    //this is for dissabling insert node when a node is selected
                    CMSinsertNode.Enabled = false;

                    //One tiem click then select the mouse
                    //This is changed to string bbk305
                    setItemSelectedID = idSelected;
                    //  MessageBox.Show("Node grabbed - id=" + setItemSelectedID);
                    Cursor = Cursors.Cross;
                    oneTimeClick = 0;
                    //MessageBox.Show("one time click");
                    mouseClickAction = 1;
                }

                else
                {
                    //reenabling... the insert node 
                    CMSinsertNode.Enabled = false;

                    mouseClickAction = 0;
                    //two time click 
                    oneTimeClick = 1;//again reset to oneTimeClick
                    Cursor = Cursors.Arrow;
                    //MessageBox.Show("Node released by second click");



                    if (Control.ModifierKeys == Keys.Alt)
                    {
                        //--This alter key is for moving along constant x-axis ...
                        //MessageBox.Show(" alt is pressed for x axis constant");


                        //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        menuStripNodeInfoValues[tempIndexForNode].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripNodeInfoValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripNodeInfoValues[x].label;
                            }


                            else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripNodeInfoValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripNodeInfoValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            //incrementIndex++;
                            indexForSeriesNodePoint++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;

                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                            }

                        }

                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting the values...




                    }
                    else if (Control.ModifierKeys == Keys.Shift)
                    {
                        //--This ctrl key is for moving along the y-  axis...

                        //MessageBox.Show("shift  is pressed for y  axis constant");

                        menuStripNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                        }

                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripNodeInfoValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripNodeInfoValues[x].label;
                            }
                            else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripNodeInfoValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripNodeInfoValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            // incrementIndex++;
                            indexForSeriesNodePoint++;
                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;

                                // ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);


                            }

                        }

                        chart1.Invalidate();
                        //  incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;
                    }
                    else
                    {

                        //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;


                        menuStripNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                        menuStripNodeInfoValues[tempIndexForNode].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {
                            for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                            }
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        indexForSeriesNodePoint = 0;



                        for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripNodeInfoValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripNodeInfoValues[x].label;
                            }
                            else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripNodeInfoValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripNodeInfoValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            //  incrementIndex++;
                            indexForSeriesNodePoint++;//incrementing the values

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {

                                incrementIndex++;

                                // ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);


                            }

                        }

                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting again

                    }//closing of key else part
                }//closing of second click



            }//closing of else block





        }

        public void ResettingLines()
        {

            menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected;

            //--Lets update the data here...
            if (flagForInsertOrUpdateDataToDB == 1)
            {
                //Testing...
                //MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                //--This is the update function for line values..
                UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, idOfNodeSelected, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness);
            }


        }

        //This is for the index of replotting the node
        int indexForSeriesNodePoint = 0;
        // int indexForSeriesLine = 0;
        public void ReDrawingLineAndNode()
        {
            if (menuStripNodeInfoValues.Count > 0)
            {
                //--This is for replotting all the things again...
                series1.Points.Clear();
                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                {
                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                    menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                }
                //--this is redraw functionality
                //foreach(var values in menuStripNodeInfoValues)
                //--Resetting the index value
                indexForSeriesNodePoint = 0;
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    string labelValue;
                    if (menuStripNodeInfoValues[x].showItemText == "Label")
                    {
                        labelValue = menuStripNodeInfoValues[x].label;
                    }
                    else if (menuStripNodeInfoValues[x].showItemText == "Name")
                    {
                        labelValue = menuStripNodeInfoValues[x].name;
                    }
                    else
                    {
                        labelValue = menuStripNodeInfoValues[x].source;
                    }


                    //--Redefined code bbk305
                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);

                    //CODE : BBK305A
                    //--incrementIndex++;
                    indexForSeriesNodePoint++;

                }
                //--resetting incrementIndex
                //CODE: BBK305A  
                // incrementIndex = 0;

                if (menuStripNodeLineInfoValues.Count > 0)
                {

                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                    {
                        //  incrementIndex++;


                        //--tHIS IS REDEFINED code bbk305
                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                    }

                }

                chart1.Invalidate();//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.

                //CODE :BBK305A \\
                //incrementIndex = 0;//reset the values again..
                indexForSeriesNodePoint = 0;//Resetting the value
            }//Close of if menustripnodeinfovalues

        }//--Close of the actual function....


        int incrementIndex = 0;//--Defining the index

        //--this class is used for storing temporary the values of id xCoord,yCoord,source,name,label,color 
        //--so that a line could be plotted in it and we can do some processing as well 

        //--This information is required for node and node only
        public class TempDataType
        {
            public string id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            public double yVal { get; set; }
            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public Color colorValue { get; set; }
            public string showItemText { get; set; }
            public int marker_Size { get; set; }



        }
        public List<TempDataType> menuStripNodeInfoValues = new List<TempDataType>();
        //--This one right here is for editing the lines...

        /// <summary>
        /// This also need to be changed we need individual unique id of the line 
        /// </summary>
        public class lineNodeDataType
        {
            //--Line ID
            public string ID { get; set; }
            public string prevNodeId { get; set; }
            public string nextNodeId { get; set; }
            public Color lineColorValue { get; set; }
            public Series lineSeriesID { get; set; }
            public int lineThickness { get; set; }

        }

        public List<lineNodeDataType> menuStripNodeLineInfoValues = new List<lineNodeDataType>();

        public void ReDrawPoints(Series s1, double x, double y, Color c, string source1, string name1, string label1x, string labelValueText, int marker_size_value)
        {

            //s1.ChartType = SeriesChartType.Point;
            
            string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x + "\nindex=" + indexForSeriesNodePoint;
            s1.ChartType = SeriesChartType.Point;
            s1.MarkerSize = marker_size_value;//= 20;
            s1.MarkerStyle = MarkerStyle.Circle;
            //s1.Points[indexForSeriesNodePoint].ToolTip = s;
            //s1.Points[indexForSeriesNodePoint].Label = labelValueText;
            // s1.Points[indexForSeriesNodePoint].Color = c;
            //s1.Color = c;

            s1.Points.AddXY(x, y);
            //s1.Points[1].
                     

            chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s;
            chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText;
            // s1.Points[indexForSeriesNodePoint].Color = c;
            chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c;
            s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle;
            s1.Points[indexForSeriesNodePoint].Color = c;
            // chart1.Series["My Series"].Points[indexForSeriesNodePoint].MarkerColor = c;
        }

          /// <summary>
          /// This function helps to draw the points before loading
          /// </summary>
          /// <param name="s1"></param>
          /// <param name="x"></param>
          /// <param name="y"></param>
          /// <param name="c"></param>
          /// <param name="source1"></param>
          /// <param name="name1"></param>
          /// <param name="label1x"></param>
          /// <param name="labelValueText"></param>
          /// <param name="marker_size_value"></param>
        public void ReDrawPointsForHardware(Series s1, double x, double y, Color c, string source1, string name1, string label1x, string labelValueText, int marker_size_value)
        {

            string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x + "\nindex=" + indexForSeriesNodePoint;
            if (chart1.InvokeRequired)
            {
                chart1.Invoke(new Action(() =>s1.ChartType = SeriesChartType.Point ));
                chart1.Invoke(new Action(() => s1.MarkerSize = marker_size_value));
                chart1.Invoke(new Action(() => s1.MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points.AddXY(x, y)));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].Color = c));
 
              //  ));
            }
            else
            {
                // lb_device_status.Text = "connected";
              
                s1.ChartType = SeriesChartType.Point;
                s1.MarkerSize = marker_size_value;//= 20;
                s1.MarkerStyle = MarkerStyle.Circle;
                s1.Points.AddXY(x, y);
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s;
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText;
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c;
                s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle;
                s1.Points[indexForSeriesNodePoint].Color = c;

            }


            // string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x + "\nindex=" + indexForSeriesNodePoint;
           
            //s1.ChartType = SeriesChartType.Point;
            //s1.MarkerSize = marker_size_value;//= 20;
            //s1.MarkerStyle = MarkerStyle.Circle;                                              
            //s1.Points.AddXY(x, y);                                                            
            //chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s;
            //chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText;            
            //chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c;
            //s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle;
            //s1.Points[indexForSeriesNodePoint].Color = c;
            

        }











        //This is used to stroe node info in 0 index (for start)       
        //and in 1 index for end
        List<TempDataType> temporaryNodeValueStore = new List<TempDataType>();
        //--Redraw line function
        public void ReDrawLines(string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color c, int thickness_value)
        {

            // if (incrementIndex > 0)
            //  if(indexForSeriesNodePoint>0) //This index is resetted later

            //   {
            temporaryNodeValueStore.Clear();//Clearing the values of the list
                                            // MessageBox.Show("ReDrawLines FRIST LINE");


            double startHumidity1 = 0;
            double startEnthalpy1 = 0;
            double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
            double endEnthalpy1 = 0;
            //now lets plot lines between tow points...

            newLineSeries = lineSeriesID;//new Series("LineSeries" + incrementIndex); //lineSeriesID; 
                                         //--If the line series is present remove the line series first
            if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
            {
                //MessageBox.Show("Series exits");
                //--This  means the series is present....
                chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name));
            }


            //new Series("LineSeries"+incrementIndex);
            // newLineSeries.Points.Clear();//--Clearing out the points

            newLineSeries.MarkerSize = 1;
            //newSeries.MarkerStyle = MarkerStyle.Triangle;
            newLineSeries.ChartType = SeriesChartType.Line;
            if (thickness_value <= 0)
            {
                newLineSeries.BorderWidth = 3;
            }
            else
            {
                newLineSeries.BorderWidth = thickness_value;//This is the thickness of lines
            }
            //newSeries.ToolTip = 
            newLineSeries.Color = c;
            /*
            We need to calculate the previous node id values and the next node id values.
            */
            //First for previous node id
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (prevNodeID == menuStripNodeInfoValues[i].id)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore =  menuStripNodeInfoValues.GetRange(i,1);  //This is for copying the value.
                    //Copying the values on index 0 assumption 
                    temporaryNodeValueStore.Add(new TempDataType
                    {
                        id = menuStripNodeInfoValues[i].id,
                        xVal = menuStripNodeInfoValues[i].xVal,
                        yVal = menuStripNodeInfoValues[i].yVal,
                        source = menuStripNodeInfoValues[i].source,
                        name = menuStripNodeInfoValues[i].name,
                        label = menuStripNodeInfoValues[i].label,
                        showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size

                    });                                                                               

                    break;//Break form loop
                }
            }

            //Second for the next node id
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (nextNodeID == menuStripNodeInfoValues[i].id)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore[1].Equals(menuStripNodeInfoValues[i]);

                    //temporaryNodeValueStore = menuStripNodeInfoValues.GetRange(i, 1);
                    //The index of this values will be temporaryNodeValueStore[1] ==> 1
                    temporaryNodeValueStore.Add(new TempDataType
                    {
                        id = menuStripNodeInfoValues[i].id,
                        xVal = menuStripNodeInfoValues[i].xVal,
                        yVal = menuStripNodeInfoValues[i].yVal,
                        source = menuStripNodeInfoValues[i].source,
                        name = menuStripNodeInfoValues[i].name,
                        label = menuStripNodeInfoValues[i].label,
                        showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size

                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStore.Count > 0) { 

            //--this sets the initial values of humidity and enthalpy
            CalculateHumidityEnthalpy(temporaryNodeValueStore[0].xVal, temporaryNodeValueStore[0].yVal);
            startHumidity1 = Math.Round(humidityCalculated, 2);
            startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
            //--This calculates the end humidity and the enthalpy values..
            CalculateHumidityEnthalpy((double)temporaryNodeValueStore[1].xVal, (double)temporaryNodeValueStore[1].yVal);
            endHumidity1 = Math.Round(humidityCalculated, 2);
            endEnthalpy1 = Math.Round(enthalpyCalculated, 2);

            // MessageBox.Show("Start hum" + startHumidity1 + " end enth" + endEnthalpy1);
            //MessageBox.Show("menustripinfovalues[prevNodeID].xVal=" + menuStripNodeInfoValues[prevNodeID].xVal + "menuStripNodeInfoValues[nextNodeID].yVal=" + menuStripNodeInfoValues[nextNodeID].yVal + "menuStripNodeInfoValues[nextNodeID].xVal = "+ menuStripNodeInfoValues[nextNodeID].xVal + " menuStripNodeInfoValues[nextNodeID].yVal" + menuStripNodeInfoValues[nextNodeID].yVal);

            double enthalpyChange = endEnthalpy1 - startEnthalpy1;

            string sequenceDetected = temporaryNodeValueStore[0].name + " to " + temporaryNodeValueStore[1].name;

            string tooltipString = "Sequence :  " + sequenceDetected + " \n" + @"                 start             end 
" + "Temp         :" + Math.Round(temporaryNodeValueStore[0].xVal, 2) + "               " + Math.Round(temporaryNodeValueStore[1].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;

            newLineSeries.ToolTip = tooltipString;
            //newSeries.MarkerStyle = MarkerStyle.Circle;
            //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
            newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStore[0].xVal, temporaryNodeValueStore[0].yVal));   //for prevnodeid
            newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStore[1].xVal, temporaryNodeValueStore[1].yVal));   //for nextnodeid


            chart1.Series.Add(newLineSeries);
            temporaryNodeValueStore.Clear();//--Lets reset the value CODE:BBK305A

                //   }  //close of if incrementindex


            }//close of temporary node value

        }

        private void CalculateHumidityEnthalpy(double xVal, double yVal)
        {
            //now lets move towards printing the relative humidity at that position and dew point and enthalpy also wbt
            //first Relative humidity...
            //first we need to see equation w = 622*phi*pg./(patm-phi*pg);
            /*
             we need to calc phi value given by ycord/30 as the max value is 30..
             * second pg which is calculated by temperature pulled from the text file we need to fist 
             * calculate the round up value of x coord to an integer...
             */

            //--this part is not correct yet we need to do this again....

            double phi = 0.00000;
            //double y_axis = yVal;
            //now for pg..
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();


            //--Copying ref temp and humidity 
            temperature_value = t;
            pg_value_from_txtfile = pg;
            double temperature = Math.Round(xVal);
            double corres_pg_value = 0.000000;
            for (int i = 0; i < temperature_value.Count; i++)
            {
                if (temperature == Double.Parse(temperature_value[i].ToString()))
                {
                    corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                    break;
                }
            }//close of for

            double patm = form1Object.AirPressureFromDB * 0.001; //101.325;//this is constant...
                                                                 // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                                                 //double w1 = 622*phi*pg/(patm-phi*pg);
            double w = yVal;
            phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
            phi = phi * 100;//changing into percent..
                            //now display in label...
            humidityCalculated = phi;//--This is the Relative humidity calculated value

            //now lets calculate the dew point...
            double humidity = phi;
            double temperature1 = xVal;
            double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
            //now lets print this value..
            //        lb_DP.Text = TD.ToString();


            //now lets move towards enthalpy...

            double Patm = form1Object.AirPressureFromDB * 0.001; //1013;
            double A = 6.116441;
            double m = 7.591386;
            double Tn = 240.7263;
            double B = 621.9907;


            double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

            double X = B * Pws / (Patm - Pws);

            double h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
            //now lets display this value ..
            enthalpyCalculated = h;//--this is the enthalpy calculated value 

        }

        private void insertNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //upon this click the form should pop up

            //Form_point_add form_point_input = new Form_point_add(this);
            // form_point_input.Show();
        }

        int yCoord = 0;
        double humidityValue; //--This is universal used to calculate humidityValue
        double temperatureValue; //--This is universal used to calculate temperatureValue
        //--These are the property of node...
        string tbSource;


        int tempIndexForNode = 0;

        private void ProcessDiagramMouseMoveFunction(MouseEventArgs e)
        {
            //--This function helps to draw a mouse move event..
            //--This is done to prevent mouse event e.x called before chart is loaded other wise the program will crash
            if (!chart1.IsAccessible && load == 0)
            {
                load = 1;
                return;

            }

            //this event occurs and compares the values in the list first and identifies if the values
            if ((e.X > chart1.ChartAreas[0].Position.X && e.Y > chart1.ChartAreas[0].Position.Y) && (e.X < chart1.Width && e.Y < chart1.Height))
            {
                try
                {
                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
                    if ((xValue >= 0 && xValue <= 50) && (yValue >= 0 && yValue <= 30))
                    {

                        xAxis1 = xValue;
                        yAxis1 = yValue;
                        //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                        if (menuStripNodeInfoValues.Count > 0)
                        {
                            //foreach(var values in menuStripNodeInfoValues)

                            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
                            {

                                if ((xValue > menuStripNodeInfoValues[i].xVal - 0.25 && xValue < menuStripNodeInfoValues[i].xVal + 0.25) && (yValue > menuStripNodeInfoValues[i].yVal - 0.25 && yValue < menuStripNodeInfoValues[i].yVal + 0.25))
                                {

                                    //--This is changed from int to string  code bbk305
                                    idSelected = menuStripNodeInfoValues[i].id; //Now this is a string 
                                    tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                    if (Cursor != Cursors.Cross)
                                    {
                                        Cursor = Cursors.Hand;
                                    }
                                    //this.Cursor = Cursors.Hand;
                                    //now this works so lets move forward.
                                    readyForMouseClick = 1;//enable on click event

                                    break;//this break is for if found the value no longer loop increases the perfomances..
                                }
                                else
                                {
                                    if (Cursor != Cursors.Cross)
                                    {
                                        this.Cursor = Cursors.Arrow;
                                        readyForMouseClick = 0;//dissable on click event.

                                    }

                                }
                            }
                        }//close of if menuStripAllValue>0


                        if (mouseClickAction == 1)
                        {

                            if (Control.ModifierKeys == Keys.Alt)
                            {
                                //--This alter key is for moving along constant x-axis ...
                                // MessageBox.Show(" alt is pressed for x axis constant");


                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                menuStripNodeInfoValues[tempIndexForNode].yVal = yAxis1;//This value is changed...


                                // label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    // chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);//--removing line series that joins node..
                                    menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].label;
                                    }
                                    else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    }
                                    else
                                    {
                                        labelValue = menuStripNodeInfoValues[x].source;
                                    }

                                    //--this is changed as well code :bbk305
                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);

                                    //Updating values in database
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //   UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;//Incrementing the index values
                                                              //--Every time it redraws the point we need to update to database the node values
                                }
                                //--resetting incrementIndex
                                //incrementIndex = 0;
                                if (menuStripNodeLineInfoValues.Count > 0)
                                {


                                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                    {
                                        // incrementIndex++;

                                        //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                                    }

                                }

                                chart1.Invalidate();
                                // incrementIndex = 0;//reset the values again..
                                //  indexForSeriesNodePoint = 0;//Resetting the index value BBK305A

                            }
                            else if (Control.ModifierKeys == Keys.Shift)
                            {
                                //--This ctrl key is for moving along the y-  axis...

                                //--THis function basically evolve when the shift key is pressed and mouse move.
                                // MessageBox.Show("shift  is pressed for y  axis constant");

                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                menuStripNodeInfoValues[tempIndexForNode].xVal = xAxis1;//--This value is just changed 
                                                                                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();

                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].label;
                                    }
                                    else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    }
                                    else
                                    {
                                        labelValue = menuStripNodeInfoValues[x].source;
                                    }


                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);

                                    //Updating values in database...
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }

                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                incrementIndex = 0;
                                if (menuStripNodeLineInfoValues.Count > 0)
                                {
                                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                    {
                                        incrementIndex++;

                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                                    }

                                }

                                chart1.Invalidate();
                                // incrementIndex = 0;//reset the values again..
                                // indexForSeriesNodePoint = 0;

                            }
                            else
                            {

                                //--Show indicator
                                ////--Lets clear the indicator point first.
                                //seriesLineIndicator.Points.Clear();

                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                                menuStripNodeInfoValues[tempIndexForNode].xVal = xAxis1;
                                menuStripNodeInfoValues[tempIndexForNode].yVal = yAxis1;

                                //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                                series1.Points.Clear();
                                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                                {
                                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                    menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                                }
                                //--this is redraw functionality
                                //foreach(var values in menuStripNodeInfoValues)
                                indexForSeriesNodePoint = 0;
                                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                                {
                                    string labelValue;
                                    if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].label;
                                    }
                                    else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    {
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    }
                                    else
                                    {
                                        labelValue = menuStripNodeInfoValues[x].source;
                                    }


                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);

                                    //Updating values in database...
                                    if (flagForInsertOrUpdateDataToDB == 1)
                                    {
                                        //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                                    }


                                    // incrementIndex++;
                                    indexForSeriesNodePoint++;
                                }
                                //--resetting incrementIndex
                                // incrementIndex = 0;
                                if (menuStripNodeLineInfoValues.Count > 0)
                                {
                                    // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");

                                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                    {
                                        // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");
                                        // incrementIndex++;

                                        //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                                    }
                                }


                                chart1.Invalidate();
                                //incrementIndex = 0;//reset the values again..
                                //   indexForSeriesNodePoint = 0;

                                //indexForSeriesNodePoint = 0;

                            }//closing of key else part
                        }

                        //Need to add here
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }//close of if chart1.ChartAreas[0]


        }


        public void addCursorFunctionForLineDisconnectConnect(MouseEventArgs e)
        {//--This function helps to draw a mouse move event..
            //--This is done to prevent mouse event e.x called before chart is loaded other wise the program will crash
            if (!chart1.IsAccessible && load == 0)
            {
                load = 1;
                return;

            }

            //this event occurs and compares the values in the list first and identifies if the values
            if ((e.X > chart1.ChartAreas[0].Position.X && e.Y > chart1.ChartAreas[0].Position.Y) && (e.X < chart1.Width && e.Y < chart1.Height))
            {
                try
                {
                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);

                    xAxis1 = xValue;
                    yAxis1 = yValue;
                    //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                    if (menuStripNodeInfoValues.Count > 0)
                    {
                        //foreach(var values in menuStripNodeInfoValues)

                        for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
                        {

                            if ((xValue > menuStripNodeInfoValues[i].xVal - 0.25 && xValue < menuStripNodeInfoValues[i].xVal + 0.25) && (yValue > menuStripNodeInfoValues[i].yVal - 0.25 && yValue < menuStripNodeInfoValues[i].yVal + 0.25))
                            {

                                idOfNodeSelected = menuStripNodeInfoValues[i].id;
                                if (Cursor == Cursors.Cross)
                                {
                                    Cursor = Cursors.Hand;
                                }

                                //--Whenever this occurs lets move on to attaching the the node or say refreshing and replotting....
                                //--For this as well lets rise a flag..
                                flagNodeSelectedForConnect = 1;
                                break;//this break is for if found the value no longer loop increases the perfomances..
                            }
                            else
                            {
                                if (Cursor != Cursors.Cross)
                                {
                                    this.Cursor = Cursors.Cross;
                                    // readyForMouseClick = 0;//dissable on click event.
                                    flagNodeSelectedForConnect = 0;
                                }

                            }
                        }
                    }//close of if menuStripAllValue>0
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }//--close of the if..



        }//close of the actual function...public void

        public void addTemporarySeries()
        {
            //--then redraw it again...
            addDottedSeries.Points.Clear();
            addDottedSeries.ChartType = SeriesChartType.FastLine;
            addDottedSeries.BorderDashStyle = ChartDashStyle.Dash;//--This gives the dashed style
            addDottedSeries.Color = Color.Black;
            addDottedSeries.BorderWidth = 3;

            double xAxisValue = 0.00;
            double yAxisvalue = 0.00;
            //We need to find the previous point
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (menuStripNodeInfoValues[i].id == indexOfPrevPointForLineMovement)
                {
                    //This is the node of previous point we need to find the x and y coordinate of
                    xAxisValue = menuStripNodeInfoValues[i].xVal;
                    yAxisvalue = menuStripNodeInfoValues[i].yVal;
                    break;//If value found no need to search all exit of loop


                }

            }


            //addDottedSeries.Points.AddXY(menuStripNodeInfoValues[indexOfPrevPointForLineMovement].xVal, menuStripNodeInfoValues[indexOfPrevPointForLineMovement].yVal);
            addDottedSeries.Points.AddXY(xAxisValue, yAxisvalue);//--this is the new changed code:bbk305
            addDottedSeries.Points.AddXY(currentXAxis, currentYAxis);

        }


        Color storeColor;
        int flagForColor = 0;

        //--Lets store the series for futher processing...
        Series tempSeries;
        int indexOfLineInTheList = 0; //This holds the id of the line selected.
        private void LineDeterctOnMouseMove(MouseEventArgs e)
        {

            HitTestResult hit = chart1.HitTest(e.X, e.Y);
            // Text = "Element: " + hit.ChartElementType;
            DataPoint dp = null;
            if (hit.ChartElementType == ChartElementType.DataPoint)
                dp = hit.Series.Points[hit.PointIndex];

            //lb_test.Text = "nothing ";

            if (dp != null)
            {
                //  Text += " Point #" + hit.PointIndex  + " x-value:" + dp.XValue + " y-value: " + dp.YValues[0]+" series name = "+hit.Series.Name;



                if (menuStripNodeLineInfoValues.Count > 0)
                {
                    for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)
                    {

                        if (hit.Series.Name != null)
                        {
                            if ((string)hit.Series.Name == (string)menuStripNodeLineInfoValues[i].lineSeriesID.Name)
                            {
                                //--lets store previous color first
                                storeColor = menuStripNodeLineInfoValues[i].lineColorValue;
                                flagForColor = 1;
                                tempSeries = hit.Series;

                                //--Logging the index so that it could be used for futher processing later...

                                //--This is the change id value which is now a string rather than int,
                                indexOfPrevPointForLineMovement = menuStripNodeLineInfoValues[i].prevNodeId;//This gets the previous node id value...

                                //
                                //indexOfLineInTheList    = i;//This index is used for updating the line when the line is moved...
                                indexOfLineInTheList = i;// = menuStripNodeLineInfoValues[i].ID;
                                hit.Series.Color = Color.Black;
                                disconnectLineToolStripMenuItem.Enabled = true;
                                // lb_test.Text = Text;
                            }


                        }


                    }
                }//CLOSE OF IF MENUSTRIP

            }//CLOSE of if dp 
            else
            {
                if (flagForColor == 1)
                    tempSeries.Color = storeColor;

            }
        }
        public void AddOrUpdateNewRecord(string name, DataGridViewCellEventArgs e)
        {
            /*
            //--We need to count the index of the current datagridview if the row index 
                is greater then the data in the list of data 
            
            */
            if (e.RowIndex + 1 > chartDetailList.Count)
            {
                //insert the data...
                string nameValue = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                insertNewDataFromDGV(nameValue);

            }
            else
            {
                //--Update the data

                //int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
                //The id value is same as we see that is present
                string chartIDValue = chartDetailList[e.RowIndex].chartID;
                string nameValue = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                UpdateNewDataFromDGV(nameValue, chartIDValue);

            }
        }

        public string GetGUID()
        {

            return Guid.NewGuid().ToString("N");
        }
        public void insertNewDataFromDGV(string name)
        {
            string buildingNameValue = selectedBuildingList[0].BuildingName;

            string nodeIdValue = "tbl_" + buildingNameValue + "_node";
            string lineIdValue = "tbl_" + buildingNameValue + "_line";
            string tableNameOfChart = "tbl_" + buildingNameValue + "_chart_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string chartIDValue = buildingNameValue + name + GetGUID();//This is the chart id value
            string chart_resp_nodes_group_ID = chartIDValue + "_node_group";
            string chart_resp_lines_group_ID = chartIDValue + "_line_group";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into  " + tableNameOfChart + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID) VALUES(@chartID_value,@nodeName_value,@nodeRespectiveID,@lineRespectiveID)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartID_value", chartIDValue);
                command.Parameters.AddWithValue("@nodeName_value", name);
                command.Parameters.AddWithValue("@nodeRespectiveID", chart_resp_nodes_group_ID);
                command.Parameters.
                    AddWithValue("@lineRespectiveID", chart_resp_lines_group_ID);

                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();


                //--We also need to create table for particular data added..
                //string sql3 = "create table " + newTableNodeName + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,id INTEGER,xValue varchar(255),yValue varchar(255),source varchar(255),name varchar(255), label varchar(255),colorValue varchar(255),showItemText varchar(255))";
                //SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                //command3.ExecuteNonQuery();


                //string sql4 = "create table " + newTableLineName + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,id INTEGER,prevNodeId varchar(255),nextNodeId varchar(255),lineColorValue varchar(255),lineSeriesId varchar(255))";
                //SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                //command4.ExecuteNonQuery();

            }


        }

        public void UpdateNewDataFromDGV(string name, string chartID)
        {
            //string newTableNodeName = "tbl_" + name + "_node";
            //string newTableLineName = "tbl_" + name + "_line";
            string buildingNameValue = selectedBuildingList[0].BuildingName;
            string tableNameOfChart = "tbl_" + buildingNameValue + "_chart_detail";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();


                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableNameOfChart + "   set  chartName=@chartName_value  where chartID = @chartID_value;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartName_value", name);
                command.Parameters.AddWithValue("@chartID_value", chartID);
                // command.Parameters.AddWithValue("@line_table_value", newTableLineName);
                // command.Parameters.AddWithValue("@id_provided", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();

            }


        }
        string beginEditText = "";


        int flagSinglCellClick = 0;

        public void RefreshGraph()
        {
            try
            {
                this.Invalidate();
                chart1.Invalidate();
                // chart1.Dispose();//--Releases all the resources used by the chart...
                plot_new_graph();

                //--Reseting the menustrip values for new plotting....
                menuStripNodeLineInfoValues.Clear();
                menuStripNodeInfoValues.Clear();
                index = 0;  //This is resetting the index values
                incrementIndex = 0;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        int countDB_Item = 0;

        public int CountingDB_Item(string db_name)
        {
            string stmt = "SELECT COUNT(*) FROM  " + db_name + "";


            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection thisConnection = new SQLiteConnection(connString))
            {
                using (SQLiteCommand cmdCount = new SQLiteCommand(stmt, thisConnection))
                {
                    SQLiteDataReader reader = null;
                    thisConnection.Open();

                    //   countDB_Item = (int)cmdCount.ExecuteScalar();
                    reader = cmdCount.ExecuteReader();
                    while (reader.Read())
                    {
                        countDB_Item = int.Parse(reader[0].ToString());
                        // currentLineTableFromDB = reader["line_table"].ToString();
                    }

                }
            }
            return countDB_Item;
        }


        /// <summary>
        /// We need have three parameters
        /// </summary>
        /// <param name="e">event </param>
        /// <param name="chartnodeid">This is the id that will identify different nodes in single node table</param>
        /// <param name="chartlineid">This is the id that will identify different lines in single line table</param>
        /// Lets pass the index alue

        public void LoadNodeAndLineFromDB(int indexValue)
        {
            //Based on this row index we need to update the values and redraw lines..

            // listForDataFromDB.Clear();//Lets clear the node...

            //Lets identify the node
            // int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
            //int id = e.RowIndex;//This index is used to identify which cell or chart is clicked.
            int id = indexValue;//This index is used to identify which cell or chart is clicked.
            /*
            Now lets find the chart line id and chart node id 
            */
            string chartNodeGroupID = chartDetailList[id].chart_respective_nodeID;//This is for the node
            string chartLineGroupID = chartDetailList[id].chart_respective_lineID;//This is for the line

            //--Reset the context menu stip first..
            menuStripNodeInfoValues.Clear();
            //--Reset the context menu stip first..
            menuStripNodeLineInfoValues.Clear();


            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            /*
            Table name for line and node values...
            ie: 
            "tbl_"+ buildingname+ "_node_value"
            "tbl_"+ buildingname+ "_line_value"
            */
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();



                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + nodeTableName + " WHERE chart_respective_nodeID = @chartnodeID";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@chartnodeID", chartNodeGroupID);//This is the group id that is used to identify each node


            
                int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    menuStripNodeInfoValues.Add(new TempDataType
                    {
                        id = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        source = reader["source"].ToString(),
                        name = reader["name"].ToString(),
                        label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString())


                    });

                }

                //--Resetting the index value...

                if (menuStripNodeInfoValues.Count > 0)
                {
                    count = menuStripNodeInfoValues.Count;//--This is used for udpdating the index values..
                }
                else
                {
                    count = 0;
                }
                //--Resetting the actual index value
                index = count; //Index is set to the count values of the node

                //--Adding data form the line node values...
                SQLiteDataReader reader2x = null;
                string queryString2x = "SELECT *  from  " + lineTableName + " WHERE chart_respective_lineID = @lineID";
                //--Testing..
                //   MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

                SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);
                command2x.Parameters.AddWithValue("@lineID", chartLineGroupID);//This is the group id that is used to identify each node

                //int count2 = 0;
                reader2x = command2x.ExecuteReader();
                while (reader2x.Read())
                {
                    menuStripNodeLineInfoValues.Add(new lineNodeDataType
                    {
                        ID = reader2x["lineID"].ToString(),//This is just change code :bbk305
                        prevNodeId = reader2x["prevNodeId"].ToString(),
                        nextNodeId = reader2x["nextNodeId"].ToString(),
                        lineColorValue = ColorTranslator.FromHtml(reader2x["lineColorValue"].ToString()),
                        lineSeriesID = new Series(reader2x["lineSeriesId"].ToString()),
                        lineThickness = int.Parse(reader2x["thickness"].ToString())
                    });

                }
                //count2 = menuStripNodeLineInfoValues.Count-1; //--This is used for udpdating the index values..

                //   MessageBox.Show("count line data in menuStripNodeLineInfoValues = " + menuStripNodeLineInfoValues.Count);

            }//close of using..

        }
            
        //lets make three list for replaing these : 
        //1.menustipnodeinfovalue ==> nodeInfoForSavingToXML
        //2.menustriplineinfovalue ==> lineInfoForSavingToXML
        //3.deviceInfoForSavingToXML

        public List<TempDataType> nodeInfoForSavingToXML = new List<TempDataType>();

        public List<lineNodeDataType> lineInfoForSavingToXML = new List<lineNodeDataType>();


        public class dt_for_device_info   //datatype for device info
        {

            public string nodeID { get; set; } //--for identifying which point is selected..
            public string device_instance_id { get; set; }//--this is the values that represent the point in a chart
            public string ip { get; set; }
            public string param1id { get; set; }
            public string param2id { get; set; }
            public string param1info { get; set; }
            public string param2info { get; set; }
            public string param1_id_type { get; set; }
            public string param2_id_type { get; set; }

        }

        public List<dt_for_device_info> deviceInfoForSavingToXML = new List<dt_for_device_info>();
        /// <summary>
        /// This function is for getting node info , correspoing device info for particular node,and line info
        /// this is required for saving in xml formate.
        /// </summary>
        /// <param name="indexValue">index of the chart selected</param>

        public void LoadNode_LineAndDeviceInfoFromDB(int indexValue)
        {
            //Based on this row index we need to update the values and redraw lines..

            // listForDataFromDB.Clear();//Lets clear the node...

            //Lets identify the node
            // int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
            //int id = e.RowIndex;//This index is used to identify which cell or chart is clicked.
            int id = indexValue;//This index is used to identify which cell or chart is clicked.
            /*
            Now lets find the chart line id and chart node id 
            */
            string chartNodeGroupID = chartDetailList[id].chart_respective_nodeID;//This is for the node
            string chartLineGroupID = chartDetailList[id].chart_respective_lineID;//This is for the line


            //--Reset the context menu stip first..
            lineInfoForSavingToXML.Clear();
            //--Reset the context menu stip first..
            nodeInfoForSavingToXML.Clear();



            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            /*
            Table name for line and node values...
            ie: 
            "tbl_"+ buildingname+ "_node_value"
            "tbl_"+ buildingname+ "_line_value"
            */
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            string deviceTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";


            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();



                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + nodeTableName + " WHERE chart_respective_nodeID = @chartnodeID";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@chartnodeID", chartNodeGroupID);//This is the group id that is used to identify each node


               
                int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    nodeInfoForSavingToXML.Add(new TempDataType
                    {
                        id = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        source = reader["source"].ToString(),
                        name = reader["name"].ToString(),
                        label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString())


                    });

                }

                //--Resetting the index value...

                if (nodeInfoForSavingToXML.Count > 0)
                {
                    count = nodeInfoForSavingToXML.Count;//--This is used for udpdating the index values..
                }
                else
                {
                    count = 0;
                }
                //--Resetting the actual index value
                index = count; //Index is set to the count values of the node

                //--Adding data form the line node values...
                SQLiteDataReader reader2x = null;
                string queryString2x = "SELECT *  from  " + lineTableName + " WHERE chart_respective_lineID = @lineID";
                //--Testing..
                //   MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

                SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);
                command2x.Parameters.AddWithValue("@lineID", chartLineGroupID);//This is the group id that is used to identify each node

              
                //int count2 = 0;
                reader2x = command2x.ExecuteReader();
                while (reader2x.Read())
                {
                    lineInfoForSavingToXML.Add(new lineNodeDataType
                    {
                        ID = reader2x["lineID"].ToString(),//This is just change code :bbk305
                        prevNodeId = reader2x["prevNodeId"].ToString(),
                        nextNodeId = reader2x["nextNodeId"].ToString(),
                        lineColorValue = ColorTranslator.FromHtml(reader2x["lineColorValue"].ToString()),
                        lineSeriesID = new Series(reader2x["lineSeriesId"].ToString()),
                        lineThickness = int.Parse(reader2x["thickness"].ToString())
                    });

                }
                //count2 = menuStripNodeLineInfoValues.Count-1; //--This is used for udpdating the index values..

                //   MessageBox.Show("count line data in menuStripNodeLineInfoValues = " + menuStripNodeLineInfoValues.Count);

            }//close of using..

            //***********************************************thsi one if for pulling device info*************************************//

            //--Reset the context menu stip first..
            deviceInfoForSavingToXML.Clear();

            foreach (var node in nodeInfoForSavingToXML)
            {
                if (node.source == "Device")
                { 

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + deviceTableName + " WHERE nodeID = @nodeID";

                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@nodeID", node.id);//This is the group id that is used to identify each node


               
                //int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    deviceInfoForSavingToXML.Add(new dt_for_device_info
                    {                  
                        nodeID = reader["nodeID"].ToString(),
                        device_instance_id = reader["device_instanceID"].ToString(),
                        ip = reader["IP"].ToString(),
                        param1id = reader["param1ID"].ToString(),
                        param2id = reader["param2ID"].ToString(),
                        param1info = reader["param1_info"].ToString(),
                        param2info = reader["param2_info"].ToString(),
                        param1_id_type  = reader["param1_identifier_type"].ToString(),
                        param2_id_type = reader["param2_identifier_type"].ToString()

                    });
                }
          }//close of using..


                } //close of if


            }//close of foreach





        }
            

        string tbName;
        string tbLabel;
        Color colorValue;
                                          
        string comboboxItemText;
        int indexSelectedForDeletion = 0;
                                
        public void deleteDatabaseTable(string tableName)
        {
            // string table1, table2;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string queryString = "Drop  table  " + tableName + "";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.ExecuteNonQuery();
            }
        }

        private void chart1_MouseClick(object sender, MouseEventArgs e)
        {

            //This flagForDisconnectClick is for the line not node
            if (flagForDisconnectClick == 1)
            {                   

                if (flagNodeSelectedForConnect == 1)
                {
                    //--Here we need to do the resetting of the datas in the arraylist and replotting it ....
                    //--This function does the resetting the line properties...
                    ResettingLines();//--Calling the resetting the lines..

                    ReDrawingLineAndNode();

                    //--Again resetting the values as well ..
                    chart1.Series.Remove(addDottedSeries);//--lets remove the indicator if present 
                    flagForDisconnectClick = 0;
                    flagNodeSelectedForConnect = 0;

                }

            }
            else {

                //--This function is used for nodeSelection and releasing node to desired place 
                //--This gets triggered based on mouse select and release..

                //Updating values in database
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                    {
                        UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                    }
                }


                NodeSelectionAndRelease(e);

                //The node update function should be placed here and the calling 
                //to the redrawing function has to be done  after that.

            }
        }

        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {
    

            if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            {
                //we need to show context menu strip

                //MessageBox.Show("Right pressed");    
                //--this is calculated based on this location the graphics will be plotted..
                xCoord = e.Location.X;
                yCoord = e.Location.Y;
                //contextMenuStrip1.Show(MousePosition);//--This is dissabled


                CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer


            }
        }

        private void chart1_MouseMove(object sender, MouseEventArgs e)
        {

            //this part helps to get the x  and the y coordinate 
            //this coordinate finding is based on the plotting part of chart element type..
            var pos = e.Location;
            if (prevPosition.HasValue && pos == prevPosition.Value)
                return;
            // tooltip.RemoveAll();
            prevPosition = pos;
            var results = chart1.HitTest(pos.X, pos.Y, false, ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(pos.X);
                    var yVal = result.ChartArea.AxisY.PixelPositionToValue(pos.Y);

                    if(((double)xVal>=0 && (double)xVal <= 50) && (yVal>=0 && yVal <= 30)) { 
                    currentXAxis = (double)xVal;
                    currentYAxis = (double)yVal;

                    //now lets move on to making other part 
                    /*1.find dbt value => this is x axis value 
                     * 2.find sp.ratio value => this is yaxis value
                     */
                    // lb_dbt.Text = Math.Round(xVal, 4).ToString();
                    //lb_humidity_ratio.Text = Math.Round(yVal, 4).ToString();


                    //now lets move towards printing the relative humidity at that position and dew point and enthalpy also wbt
                    //first Relative humidity...
                    //first we need to see equation w = 622*phi*pg./(patm-phi*pg);
                    /*
                     we need to calc phi value given by ycord/30 as the max value is 30..
                     * second pg which is calculated by temperature pulled from the text file we need to fist 
                     * calculate the round up value of x coord to an integer...
                     */

                    //this part is not correct yet we need to do this again....

                    double phi = 0.00000;
                    //double y_axis = yVal;
                    //now for pg..
                    ArrayList temperature_value = new ArrayList();
                    ArrayList pg_value_from_txtfile = new ArrayList();

                    //--Copying the ref temp and humidity values..
                    temperature_value = t;
                    pg_value_from_txtfile = pg;

                    double temperature = Math.Round(xVal);
                    double corres_pg_value = 0.000000;
                    for (int i = 0; i < temperature_value.Count; i++)
                    {
                        if (temperature == Double.Parse(temperature_value[i].ToString()))
                        {
                            corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                            break;
                        }
                    }//close of for

                    double patm = form1Object.AirPressureFromDB * 0.001;//101.325;//this is constant...
                        double w = yVal;
                    phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                    phi = phi * 100;//changing into percent..
                                    //now display in label...
                                    // lb_RH.Text = Math.Round(phi, 4).ToString();

                    //now lets calculate the dew point...
                    double humidity = phi;
                    double temperature1 = xVal;
                    double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
                    //now lets print this value..
                    // lb_DP.Text = Math.Round(TD, 4).ToString();


                    //now lets move towards enthalpy...

                    Patm = form1Object.AirPressureFromDB * 0.001;//1013;
                        A = 6.116441;
                    m = 7.591386;
                    Tn = 240.7263;
                    B = 621.9907;

                    double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

                    double X = B * Pws / (Patm - Pws);

                    h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
                    //now lets display this value ..
                    //  lb_enthalpy.Text = Math.Round(h, 4).ToString();

                }
                }

            }  //Close of foreach

            //--IF the line is selected/disconnected and then we need to connect to a node
            if (flagForDisconnectClick == 1)
            {
                //--Creating temporary line..
                //--then redraw it again...
                addTemporarySeries();
                //--Now lets move on the showing the hand when hover over the Node lets do it bro...

                addCursorFunctionForLineDisconnectConnect(e);


            }
            else {

                disconnectLineToolStripMenuItem.Enabled = false;
                //--This is for the weather the line is moverover or not...
                LineDeterctOnMouseMove(e);

                //--Lets add a function for the process diagram drawing..

                ProcessDiagramMouseMoveFunction(e);//--This does the adding and removing part
            }
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            //--This start the editing of the cells
            //Here we save a current value of cell to some variable, that later we can compare with a new value
            //For example using of dgv.Tag property
            // MessageBox.Show("value  = " + dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString());
            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                this.dataGridView1.Tag = this.dataGridView1.CurrentCell.Value;
                if (dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
                {
                    beginEditText = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                    //Or cast sender to DataGridView variable-> than this handler can be used in another datagridview
                }
            }
        }
     
        private void disconnectLineToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            flagForDisconnectClick = 1;
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
        }

        private void insertNodeToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            //This is the insert node part...
            //InsertNodeFormForBuildingChartSetting insertF = new InsertNodeFormForBuildingChartSetting(this);
            //insertF.Show();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //This should show the line editing part of the chart 
         //EditNodeLineForm f = new EditNodeLineForm(this);
         // f.Show();
        }

        private void dataGridView1_CellEndEdit_1(object sender, DataGridViewCellEventArgs e)
        {

            // if(Control.ModifierKeys == Keys.Tab) { 
            //--Here we need to add datato the database...
            try
            {
                //MessageBox.Show("end edit");
                //--This section helps in validation..
                if (dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
                {
                    var regex = new Regex(@"^[a-zA-Z0-9_]+$");
                    string name = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                    // MessageBox.Show("name= " + name);
                    if (regex.IsMatch(name))
                    {
                        int length = name.Length;
                        if (length > 30)
                        {
                        //}
                        //else
                        //{

                            MessageBox.Show("You can only have name upto 30 characters");
                            dataGridView1.Refresh();
                            if(beginEditText == "") { 
                            dataGridView1.CurrentCell.Value = "";
                            }
                            else
                            {
                                dataGridView1.CurrentCell.Value = beginEditText;
                            }
                            return;
                       // }

                    }
                    //--Testing ..
                    //  MessageBox.Show(" name = " + name);
                    //--we need to check that the table name enter doesnot matches previous values

                    if (name != beginEditText)
                        {
                            for (int i = 0; i < chartDetailList.Count; i++)
                            {
                                if (name == chartDetailList[i].chartName)
                                {
                                    MessageBox.Show("chart can not have same name");
                                    dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;

                                    return;
                                }
                            }

                            //--The value is ok 
                            //--Now lets do the insert opeation for the data...

                            AddOrUpdateNewRecord(name, e);

                            //  MessageBox.Show("regx name");
                            //--now lets add new rows..
                            dataGridView1.Refresh();
                            //dataGridView1.Rows.Clear();//Remove the rows
                            // BeginInvoke(new MethodInvoker(PopulateControl));
                            dataGridView1.Rows.Clear();

                            //--Lets get the data and refill the data in datagridview.
                            // PullDataFromDB();


                            //LETS FILL THE DATA
                            // fillDataGridView();
                            DataGridView_Show_Data();//This will do both pulling data and filing the data...

                            dataGridView1.Rows.Add();

                        }
                        else
                        {
                            MessageBox.Show("No change in name");
                        }

                        ///close of if we need to add here ...
                

                    }
                    else
                    {
                        MessageBox.Show("You can only have letters, numbers and underscores");
                        dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = string.Empty;

                    }


             }

                //Catch exception before do this : 
               // this.dataGridView1.CurrentCell.ReadOnly = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show("Edit and then press tab to commit changes");
                dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;

            }
        }

        string chartSelectedIDValue = null;
      public  int indexForWhichChartIsSelected = 0;//--This index value stores the index value in a list of which chart is selected
  public   int indexOfChartSelected = 0;
        private void dataGridView1_CellClick_1(object sender, DataGridViewCellEventArgs e)
        {

            //--Showing the data on cell selected...
            // MessageBox.Show("CELL SELECT " );
            //When dgv is click it clicks twice this if is written to stop those twice click.
      
            if (flagSinglCellClick == 1)
            {
                flagSinglCellClick = 0;
                return;
            }
            if ((e.RowIndex >= 0) && (e.ColumnIndex == 1))
            {

                if (dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                {
                    // MessageBox.Show("CELL SELECT ROW= " + e.RowIndex+",columns = "+e.ColumnIndex);
                    //The row is selected ..
                    for (int i = 0; i < chartDetailList.Count; i++)
                    {
                        //--Checking for evey item
                        if (dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString() == chartDetailList[i].chartName)
                        {
                            //if match found load
                            //data_load();
                            chart1.Enabled = true;

                            RefreshGraph();
                            /*
                            We need to identify which item in the list was clicked .
                              //Before we load line we need to identify the id of the chart to load the data..
                              */
                            //chartSelectedIDValue = chartDetailList[i].chartID;//--tHIS CONTAINS THE ID OF THE CHART
                            indexForWhichChartIsSelected = i;
                            //This has been changed
                            indexOfChartSelected = e.RowIndex;    //This value is changed 
                     LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

                            flagForInsertOrUpdateDataToDB = 1;
                            //--This is also completed..
                            ReDrawingLineAndNode();


                            flagSinglCellClick = 1;

                            //lets enable the button
                            btn_pull_data.Enabled = true;
                            button1.Enabled = true;
                            //--Now lets load the vaiables in dgv2
                            // loadVariableInDGV2();

                            //==============This part is for comfort zone=====================
                            //lets make the flag off first.
                            flagShow = 0;//off
                                         //this is for loading comfort zone 

                            //Lets clear all the chart first
                            default_comfort_zone_of_chart.Clear();
                            listchartComfortZoneInfoSingle.Clear();

                            LoadComfortZone(chartDetailList[indexForWhichChartIsSelected].chartID);

                            if (default_comfort_zone_of_chart.Count > 0)
                            {
                                //This means chart is present enable it
                                enableToolStripMenuItem.Enabled = true;

                                //AFTER WE know that there is chart then lets add this function  to check the show option

                               // MessageBox.Show("enabltool = ture"+ default_comfort_zone_of_chart.Count);
                                if (flagShow == 1)
                                {
                                    //  enableToolStripMenuItem.Enabled = true;
                                    enableToolStripMenuItem.Checked = true;
                                }
                                else
                                {
                                    // enableToolStripMenuItem.Enabled = false;
                                    enableToolStripMenuItem.Checked = false;
                                }


                            }
                            else
                            {
                                enableToolStripMenuItem.Enabled = false;
                                enableToolStripMenuItem.Checked = false;
                                //MessageBox.Show("enabltool = false" + default_comfort_zone_of_chart.Count);
                            }

                             //====================end of comfort zone=========================
                            break;

                        }
                        else
                        {
                            flagForInsertOrUpdateDataToDB = 0;
                            RefreshGraph();
                        }
                    }


                }  //close of != null if
                else
                {
                    flagForInsertOrUpdateDataToDB = 0;
                }

                //This is for cell formating and editing...

                if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                   // this.dataGridView1.BeginEdit(false);
                    this.dataGridView1.CurrentCell.ReadOnly = true;
                }

            }//close of columnindex if


            // if(atimer.Enabled == true)
            // {
            //   atimer.Dispose();
            //atimer.Stop();
            //}

            if (flagForTimer == 1)
            {
                if (atimer.Enabled) // Check if the timer is running
                {
                    // Return error.........
                    atimer.Enabled = false;
                    atimer.Dispose();
                    flagForTimer = 0;

                }
            }//close of flagfortimer
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.RowIndex >= 0 && e.ColumnIndex == 0)
                {
                    if (dataGridView1.Rows[e.RowIndex].Cells[0].Value != null)
                    {
                        Point p = new Point(e.X,e.Y);
                        indexSelectedForDeletion = e.RowIndex;
                        //dataGridView1.SelectedCells = true;
                        //DataGridViewCell c = (sender as DataGridView)[e.ColumnIndex, e.RowIndex];
                        //if (!c.Selected)
                        //{
                        //    c.DataGridView.ClearSelection();
                        //    c.DataGridView.CurrentCell = c;
                        //    c.Selected = true;
                        //}
                                 
                         contextMenuStrip1.Show(dataGridView1,new Point( (int)e.X,(int)e.Y));
                        //contextMenuStrip1.Show(dataGridView1.PointToScreen(e.Location));
                    }
                }


            }
        }

        int markerSize = 20; //This is used to make marker of node change...
        int lineBorderThickness = 15;//This is used to change the thickness of the line....

        /// <summary>
        /// This create a node in the chart
        /// </summary>
        /// <param name="source">what is the source that the data is comming from</param>
        /// <param name="name">what is the name of the data</param>
        /// <param name="label">What label it has</param>
        /// <param name="c1">color it should have</param>
        /// <param name="comboboxItemText1">which items to be displayed</param>
        /// This one is for the Web
        string medium_device_or_web_value = "";
        public void SetNodeForWeb(string source, string name, string label, Color c1, string comboboxItemText1,int marker_size)
        {

            markerSize = marker_size; //This is for changing the marker size 
            tbSource = source;
            tbName = name;
            tbLabel = label;
            colorValue = c1;
            comboboxItemText = comboboxItemText1;
         //medium_device_or_web_value = device_or_web;
            //lets do the processing 
            //lets count how many items were inserted
            // countNumberOfPoints += 1;  //countNumberOfPoints++;  //This is only used here for counting number of node items//This needs to set to values of nodes while pulling.
            //lets get the coordinates to plot on the graph..
            //this will be set on right click..not here

            //lets process the data 
            /*
            calculating the humidity and temperature value form the coordinates..
            */
         
            //Checked ok..BBK305A

            //This value is temporary change.

            //--This is currently now required : required previously
            //  HumTempCalcByCoordinate();


            //MessageBox.Show("Temp= " + temperatureValue + ",hum = " + humidityValue);
            //hardwareTemperatureRead = temp;
            //hardwareHumidityRead = humidity;

            //Temp and humidity value 
            double temp1Val = 0.000;
            double hum1Val = 0.00;
           //if(device_or_web == "Device")
           // {
           //     //Device choice is selected
           //     ReadDataFromDevice();
           //     temp1Val = hardwareTemperatureRead;
           //     hum1Val = hardwareHumidityRead;

           //     if(hardwareTemperatureRead == 0.0000 && hardwareHumidityRead == 0.0000)
           //     {
           //         //if hardware is damaged or not pulling the temp and hum then we need to pull form the web
           //         //We need to pull form the web..
           //         //web choice is selected.
           //         string db_name = "tbl_building_location";
           //         PullLongitudeAndLatitude(db_name);

           //         //Now lets pass the value to weather api
           //         GetDataFromWeb(latValueFromDB, longValueFromDB);//This gets the temperature and humidity

           //         temp1Val = temp_pulled_from_web;
           //         hum1Val = double.Parse(hum_pulled);

           //     }

           // }
           // else
           // {
                //web choice is selected.
                string db_name = "tbl_building_location";
                PullLongitudeAndLatitude(db_name);

                //Now lets pass the value to weather api
                GetDataFromWeb(latValueFromDB, longValueFromDB);//This gets the temperature and humidity

                temp1Val = temp_pulled_from_web;
                hum1Val =double.Parse( hum_pulled);


           // }


            //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if ((hum1Val > 0 && hum1Val <= 100) && (temp1Val >= 0 && temp1Val <= 50))
            {
                //now lets plot the values....

                plot_by_DBT_HR_process_diagram((double)(int)temp1Val, (double)hum1Val / 100);

            }
            else
            {
                MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
            }


            ////now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            //if ((humidityValue > 0 && humidityValue <= 100) && (temperatureValue >= 0 && temperatureValue <= 50))
            //{
            //    //now lets plot the values....

            //    plot_by_DBT_HR_process_diagram((double)(int)temperatureValue, (double)humidityValue / 100);
            //}
            //else
            //{
            //    MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
            //}

        }



        //public Tuple<int, int> plot_by_DBT_Enthalpy(double dbt, double enthalpy)
        public int plot_by_DBT_Enthalpy(double dbt, double enthalpy)
        {
            //this is DBT 
            double x_axis = dbt;
            double h = enthalpy;
            // MessageBox.Show("h = " + h+" T = "+dbt);

            //lets fit this value in the curve...
            double x1 = (h - 12.5) / 3.5;
            //        MessageBox.Show("X1= " + x1);
            double y1 = x1 + 5;//this is given temp..
            //      MessageBox.Show("y1= " + y1);
            double x2 = h;
            double y2 = 0;

            //    MessageBox.Show("x2= " + x2+" y2  = "+y2);

            double x = dbt;
            double y = y1 + (((y2 - y1) * (x - x1) / (x2 - x1)));

            double y_axis = y;

            //return Tuple.Create((int)Math.Round( x_axis,0),(int)Math.Round(y_axis));
            return (int)Math.Round(y_axis);
        }


            /// <summary>
            /// This inserts the device id value
            /// </summary>       
            /// <param name="source">Source </param>
            /// <param name="name"></param>
            /// <param name="label"></param>
            /// <param name="c1">color</param>
            /// <param name="comboboxItemText1">Which to select</param>
            /// <param name="marker_size">size fo the marker</param>
            /// <param name="deviceInstance">int value of instance of device</param>
            /// <param name="port">port of the device</param>
            /// <param name="param1ID">int value of param 1 ie deice port number ANALOG_VALUE :[PORT]</param>
            /// <param name="param2ID">ANALOG_VALUE PORT for param2</param>
            string deviceInstanceValue = "";
        string deviceIP = "";
        string deviceParam1ID = "";
        string deviceParam2ID = "";
        string device_param1_info_for_node = "";
        string device_param2_info_for_node = "";
        string object_param1_identifier_type = ""; //for identifying different analog inputs.
        string object_param2_identifier_type = "";

        public void SetNodeForDeviceUsingTempAndHum(string source, string name, string label, Color c1, string comboboxItemText1, int marker_size,string deviceInstance,string IP,string param1ID,string param2ID,string param1_info,string param2_info,string param1_value,string param2_value,string  object_identifier_type_param1, string object_identifier_type_param2)
        {

            markerSize = marker_size; //This is for changing the marker size 
            tbSource = source;
            tbName = name;
            tbLabel = label;
            colorValue = c1;
            comboboxItemText = comboboxItemText1;
            deviceInstanceValue = deviceInstance;
            deviceIP = IP;
            deviceParam1ID = param1ID;
            deviceParam2ID = param2ID;
            device_param1_info_for_node = param1_info;

            device_param2_info_for_node = param2_info;
            //MessageBox.Show("param2 infor=="+param2_info);

            //--These parameter identifiy which section the node belongs to ie analog_input, analog_output,analog_value 
            object_param1_identifier_type = object_identifier_type_param1;
            object_param2_identifier_type = object_identifier_type_param2;

           // medium_device_or_web_value = device_or_web;
           //Temp and humidity value 

            if (source == "Device")
            {
             
                if (param1_info == "temp" && param2_info == "hum")
                {
                    double temp1Val = 0.0000;
                    double hum1Val = 0.0000;
                    //Device choice is selected
                    //  ReadDataFromDevice();
                    temp1Val =double.Parse(param1_value);//we have tmep here//hardwareTemperatureRead;
                    hum1Val = double.Parse(param2_value);//we have humidity here//hardwareHumidityRead;

                   // MessageBox.Show("Temp = " + temp1Val + ",hum=" + hum1Val);
                        if (temp1Val == 0.0000 && hum1Val == 0.0000)
                        {
                            //if hardware is damaged or not pulling the temp and hum then we need to pull form the web
                            //We need to pull form the web..
                            //web choice is selected.
                            MessageBox.Show("Cound not get data from device. Pulling data from web");
                            string db_name = "tbl_building_location";
                            PullLongitudeAndLatitude(db_name);

                            //Now lets pass the value to weather api
                            GetDataFromWeb(latValueFromDB, longValueFromDB);//This gets the temperature and humidity

                            temp1Val = temp_pulled_from_web;
                            hum1Val = double.Parse(hum_pulled);

                        }

                    //Now lets plot the values
                    //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
                    if ((hum1Val > 0 && hum1Val <= 100) && (temp1Val >= 0 && temp1Val <= 50))
                    {
                        //now lets plot the values....
                        plot_by_DBT_HR_process_diagram((double)(int)temp1Val, (double)hum1Val / 100);

                    }
                    else
                    {
                        MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
                    }


                }//close of if value
                else if(param1_info == "temp"&& param2_info == "enthalpy")
                {
                    //temperature and enthalpy
                    /*
                    Steps :1.get temp and enthalpy =>done
                           2. convert to x and y values => done
                           3. convert to x                      
                    */

                    // Tuple<int,int> x = plot_by_DBT_Enthalpy(double.Parse(param1_value),double.Parse(param2_value));

                    double x_value = double.Parse(param1_value);
                    double y_value = plot_by_DBT_Enthalpy(x_value, double.Parse(param2_value));


                    plot_on_graph_values_process_diagram(x_value, y_value);





                }
                
                                                  
            }

        } //Close of setnode_forweb function

        public void HumTempCalcByCoordinate()
        {
            //this is not working properly why i dont know...


            var results = chart1.HitTest(xCoord, yCoord, false, ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    // double xVal = xCoord - chart1.ChartAreas[0].Position.X ;
                    //double yVal = yCoord-chart1.ChartAreas[0].Position.Y;

                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(xCoord);
                    var yVal = result.ChartArea.AxisY.PixelPositionToValue(yCoord);
        
                    /*
                     we need to calc phi value given by ycord/30 as the max value is 30..
                     * second pg which is calculated by temperature pulled from the text file we need to fist 
                     * calculate the round up value of x coord to an integer...
                     */
                    //this part is not correct yet we need to do this again....
                    double phi = 0.00000;
                    //double y_axis = yVal;
                    //now for pg..
                    ArrayList temperature_value = new ArrayList();
                    ArrayList pg_value_from_txtfile = new ArrayList();

                    temperature_value = t;
                    pg_value_from_txtfile = pg;

                    double temperature = (double)Math.Round((double)xVal);
                    double corres_pg_value = 0.000000;
                    for (int i = 0; i < temperature_value.Count; i++)
                    {
                        if (temperature == Double.Parse(temperature_value[i].ToString()))
                        {
                            corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                            break;
                        }
                    }//close of for

                    double patm = form1Object.AirPressureFromDB * 0.001; //101.325;//this is constant...
                                                                         // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                                                         //double w1 = 622*phi*pg/(patm-phi*pg);
                    double w = yVal;
                    phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                    phi = phi * 100;//changing into percent..

                    //now lets calculate the dew point...
                    double humidity = phi;
                    double temperature1 = xVal;

                    humidityValue = humidity; //This humidityValue and the temperatureValue are used to get the humidity and temperature 
                    temperatureValue = temperature1;
                }
            }
        }

        //this series is used for plotting on the graph
        //Series series1 = new Series("My series");
        int index_series = 0;
        //int index = 0;
        public int plot_by_DBT_HR_process_diagram(double DBT, double HR)
        {
            /*           
             *We need to cal x-asis which is given by DBT 
             */
            // MessageBox.Show("reached here dbt=" + DBT + ", hr = " + HR);
            int x_axis = (int)DBT;

            //here the HR is  relative humidity like 20%,30% etc os phi = 0.3 for 30%
            double phi = HR;
            //we need to calculate the y-axis value 
            /*For y axis the value has to be pulled from the t_pg text file....
             */
            //lets create two arraylist to add those and store it in the arraylist
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();

            //--Copying the ref temp and humidity to temporary arraylist
            temperature_value = t;
            pg_value_from_txtfile = pg;
            double patm = form1Object.AirPressureFromDB * 0.001;//in kpa//101.235;//constant..we will make it take as input later...
            //double rair = 0.287;//rideburg constant i guess
            double wg_calc = 0;
            double pg_value = 0.000000;
            //now for corresponding DBT lets calculate constant value pg..
            try
            {
                for (int i = 0; i < temperature_value.Count; i++)
                {
                    ///x-axis contains the DBT
                    if (DBT == Double.Parse(temperature_value[i].ToString()))
                    {
                        //if matched find the corresponding pg_value
                        pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());
                        break;//break out of loop.
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            //now calc the y axis.
            //wg_calc =  622 * pg_value / (patm - pg_value);
            wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
            //MessageBox.Show("pg_value=" + pg_value + ",wg_calc" + wg_calc);
            double y_axis = wg_calc;

            plot_on_graph_values_process_diagram( x_axis, y_axis);   //Checking this one now...\//\//

            //MessageBox.Show("reached series print" +series1.ToString());

            // index++;

            return 0;
        }

        /// <summary>
        /// plot using x coord(=temp) and y coord (=hum ratio)
        /// </summary>
        /// <param name="xval">Temperature(x-axis)</param>
        /// <param name="yval">humidity ratio(y-axis)</param>
        public void plot_on_graph_values_process_diagram(double xval, double yval)
        {
            //chart1.Series.Clear();

            try
            {

                /*
                This series1 is used to drwa points[ie. node] on the chart, for single point or node the series type will be same but is represented in the form of the datapoints.
                */                
                series1.ChartType = SeriesChartType.Point;   
                //int r, g, b;
                series1.MarkerSize = markerSize;    //By default the marker size is 20 ok ok ok 
                series1.MarkerStyle = MarkerStyle.Circle;
                // MessageBox.Show("finally added xvalue = " + xval + " yvalue = " + yval);
                series1.Points.AddXY(xval, yval);
                string s = "source : " + tbSource + "\n Name : " + tbName + "\nLable : " + tbLabel;
                series1.Points[index].Color = colorValue;    //This index is used for i
                series1.Points[index].ToolTip = s;

                string labelStringValue = null;
                //labeling part
                if (comboboxItemText == "Label")
                {
                    //label is selected
                    labelStringValue = tbLabel;
                }
                else if (comboboxItemText == "Name")
                {
                    //Name is selected
                    labelStringValue = tbName;

                }
                else
                {
                    //Source is selected
                    labelStringValue = tbSource;
                }

                //This index is ok 
                /*
                This value is used to set to counted value form menuStripNodeInfoValues.count on load of the node
                */
                series1.Points[index].Label = labelStringValue;

                //  MessageBox.Show("value xval =" + xval + ",yval = " + yval);
                //series1.Points[index_series++].Color = colorValue;//blue
                //    MessageBox.Show("end re");
                //index_series++;
                //series1.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            //now lets move on to storing those values and futher porcessing it...

            /*
            I have to edit this section...
            menu stripNodeInfoValues need to store different values such as the size and color as well
            */

            //we need to get this a unique id 
            string unique_id_for_node = selectedBuildingList[0].BuildingName + GetGUID();//This is the unique id 

             //--Recent changes code id : bbk305
            //the value is added...
            menuStripNodeInfoValues.Add(new TempDataType
            {
                id = unique_id_for_node,
                xVal = xval,
                yVal = yval,
                source = tbSource,
                name = tbName,
                label = tbLabel,
                colorValue = colorValue,
                showItemText = comboboxItemText  ,
                marker_Size =  markerSize

            });

            if (flagForInsertOrUpdateDataToDB == 1)
            {
                //do only if the  flag is raised.
   

                InsertNodeInfoToDB(unique_id_for_node, xval, yval, tbSource, tbName, tbLabel, colorValue, comboboxItemText, markerSize,deviceInstanceValue,deviceIP,deviceParam1ID,deviceParam2ID,device_param1_info_for_node,device_param2_info_for_node,object_param1_identifier_type,object_param2_identifier_type);
            }

            //the liine plot part is only done when ther is two points or more
            if (index > 0)
            {
                     
                double startHumidity1 = 0;
                double startEnthalpy1 = 0;
                double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
                double endEnthalpy1 = 0;

                //now lets plot lines between tow points...
                Series newLineSeries = new Series("LineSeries" + index);
                //string nameSeries = newLineSeries.Name;

                //--If the series already present lets remove from the chart ok ol :)
                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //MessageBox.Show("Series exits");
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name));
                }


                string unique_id_for_line = selectedBuildingList[0].BuildingName + GetGUID();//This is the unique id 


                //--Lets store the nodeline info as well
                menuStripNodeLineInfoValues.Add(new lineNodeDataType
                {

                    //--Id of this ..
                    ID = unique_id_for_line,
                    prevNodeId = menuStripNodeInfoValues[index - 1].id,
                    nextNodeId = menuStripNodeInfoValues[index].id,
                    lineColorValue = menuStripNodeInfoValues[index - 1].colorValue,
                    lineSeriesID = newLineSeries,
                    lineThickness = 3  //default thickness is 3

                });

                //--Inserting the line to database here....


                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    //--Insert the values when the flag is raised.
                    InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[index - 1].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries, menuStripNodeLineInfoValues[index-1].lineThickness);
                }



                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newLineSeries.ChartType = SeriesChartType.Line;
                //newLineSeries.MarkerStyle = MarkerStyle.Circle;
                //newLineSeries.MarkerStyle = MarkerStyle.Star6;
                //newLineSeries.MarkerBorderWidth.Equals(15);
                newLineSeries.MarkerSize.Equals(1);
                newLineSeries.BorderWidth.Equals(lineBorderThickness);
                // newLineSeries.SetCustomProperty(newLineSeries.MarkerSize.ToString(),newLineSeries.MarkerSize.Equals(25).ToString());
                newLineSeries.Color = menuStripNodeInfoValues[index].colorValue;

                //--this sets the initial values of humidity and enthalpy
                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[index - 1].xVal, (double)menuStripNodeInfoValues[index - 1].yVal);
                startHumidity1 = Math.Round(humidityCalculated, 2);//--Fro showing only up to 2 dec. eg."34.52"
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[index].xVal, (double)menuStripNodeInfoValues[index].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripNodeInfoValues[index - 1].name + " to " + menuStripNodeInfoValues[index].name;


                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :" + Math.Round(menuStripNodeInfoValues[index - 1].xVal, 2) + "               " + Math.Round(menuStripNodeInfoValues[index].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : " + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;
                newLineSeries.ToolTip = tooltipString;
                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index - 1].yVal));
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index].yVal));

                chart1.Series.Add(newLineSeries);
                chart1.Series[newLineSeries.Name].BorderWidth = 3;


            }



            index++;  //This is the increment of the index value


        }//close of buttons 
        private void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            if(dataGridView1.CurrentCell.ColumnIndex == 1)
            {
                this.dataGridView1.CurrentCell.ReadOnly = false;
                this.dataGridView1.BeginEdit(true);
               
            }


        }

        double hardwareTemperatureRead = 0;
        double hardwareHumidityRead = 0;

        double hardwareValue1 = 0;
        double hardwareValue2 = 0;

             /// <summary>
             /// helps to pull the data from the hardware of the device
             /// </summary>
        public void ReadDataFromDevice(int deviceID,uint temp_panID,uint hum_panID,string param1_identifier_type,string param2_identifier_type)
        {
            //lets do some operation regarding the pannel id and stuff
                                      
            //then perform this task 
            try
            {
                uint panID_1 = temp_panID;//0; //uint.Parse(panelID1);

                uint panID_2 = hum_panID;//1;//uint.Parse(panelID2);
                BACnetClass b = new BACnetClass();

                //for temperature value
                b.StartProgramForScanHardware(deviceID, panID_1,param1_identifier_type);
                double  temperary1 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_temp_panel_value.Text = temp;
                //For humidity value
                b.StartProgramForScanHardware(deviceID, panID_2,param2_identifier_type);
                double temperary2 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_hum_panel_value.Text = humidity;
                hardwareValue1 = temperary1;
                hardwareValue2 = temperary2;
                //lets store these two values in a temporary list
                                      
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }

        }  

        public void RefreshDataFromDeviceAndWeb()
        {
        // MessageBox.Show("calling....test.."+ chartDetailList[indexForWhichChartIsSelected].chartName);
            //This fxn help in refreshing the data value
            //both for points form the web as well as from the device
            /*
            Steps :
            1.Read number of corresponding chart points and lines 
            2.For each point read value 
              a)if for device read device info and port info   and get value form hardware
              b)pass the presented value to plotting fxn containning xaxis and y axis value.
            3.or if from web do read lat long and update to database
            4.plot values         
            or
            1.Read the nodes and lines form db 
            2.update info in db 
            3.call plot fxn
            */
            //Read 
            LoadNodeAndLineFromDB(indexOfChartSelected);//indexOfChartSelected= index chosen

          //  try { 
           //Read each node and then perform the following fxn
           foreach(var node in menuStripNodeInfoValues)
            {
               // Timer c = new Timer();
               // c.Start();

                if(node.source == "Device")
                {
                    //The node is pulled from device
                    //steps read node's device info
                    /*Steps :
                    1.Read the node info from db (this contains panel id values and what are those for)
                    2.based on panel id value calculate the x and y coordinate.
                    3.Update the database value.
                    */
                   // MessageBox.Show("node id= " + node.id);
                    ReadDeviceInfoForNode(node.id);

                    //Here we will check for the device parameter id values

                    if (CheckDeviceOnlineOffline(int.Parse(device_info_list[0].device_instance_id), 0) == true)
                    { 
                                //online mode...
                        //2nd step calc x and y
                        //The value will always be unique and always be in the 0 index  
                        if(device_info_list[0].param1_info=="temp" && device_info_list[0].param2_info == "hum")
                            {
                            //This meand the value is humidity and temperature so we process like wise
                            //This gets the value
                            ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id),uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id),device_info_list[0].param1_identifier_type,device_info_list[0].param2_identifier_type);
                            //we have recent value in hardwareValue1 and hardwareValue2 so lets calc corresponding x and y value
                            //now temp itself is x value we need to calculate y value
                            double x_Value = hardwareValue1;
                            double y_value = CalculateYFromXandHumidity(hardwareValue1, hardwareValue2/100);

                            //   MessageBox.Show("x val /temp/hardwareValue1 = "+x_Value+"\nhardwareValue2"+hardwareValue2+"\ny value hardware= " + y_value);
                            //Now lets update the values in db

                           
                            UpdateNodeInfoToDB(node.id, x_Value, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);

                            //=============STATUS SHOWING ONLINE OR DEVICE OFFLINE=================
                            if (lb_device_status.InvokeRequired)
                            {
                                lb_device_status.Invoke(new Action(() =>  lb_device_status.Text = "connected"));
                            }
                            else
                            {
                                lb_device_status.Text = "connected";
                            }
                            //=======================END OF ONLINE/OFFLINE===========

                            //if (lb_value_test.InvokeRequired)
                            //{
                            //    lb_value_test.Invoke(new Action(() => lb_value_test.Text = "xval = " + x_Value + ",y val=" + y_value));
                            //}
                            //else
                            //{
                            //    lb_value_test.Text = "xval = " + x_Value + ",y val=" + y_value;
                            //}

                          //  lb_value_test.Text = "xval = " + x_Value + ",y val=" + y_value;//+"timer interval = "+c.Interval;
                           // c.Stop();

                        }
                        else if(device_info_list[0].param1_info == "temp" && device_info_list[0].param2_info == "enthalpy")
                        {
                            //First is temp and second is enthalpy


                            //2nd step calc x and y
                            //The value will always be unique and always be in the 0 index  
                           // if (device_info_list[0].param1_info == "temp" && device_info_list[0].param2_info == "hum")
                            //{
                                //This meand the value is humidity and temperature so we process like wise
                                //This gets the value
                                ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id), uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param1_identifier_type, device_info_list[0].param2_identifier_type);
                                //we have recent value in hardwareValue1 and hardwareValue2 so lets calc corresponding x and y value
                                //now temp itself is x value we need to calculate y value
                                double x_Value = hardwareValue1;
                                double y_value = plot_by_DBT_Enthalpy(x_Value, hardwareValue2); //CalculateYFromXandHumidity(hardwareValue1, hardwareValue2 / 100);

                                //   MessageBox.Show("x val /temp/hardwareValue1 = "+x_Value+"\nhardwareValue2"+hardwareValue2+"\ny value hardware= " + y_value);
                                //Now lets update the values in db


                                UpdateNodeInfoToDB(node.id, x_Value, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);

                                //=============STATUS SHOWING ONLINE OR DEVICE OFFLINE=================
                                if (lb_device_status.InvokeRequired)
                                {
                                    lb_device_status.Invoke(new Action(() => lb_device_status.Text = "connected"));
                                }
                                else
                                {
                                    lb_device_status.Text = "connected";
                                }

                            }
                            else
                        {
                            //First is humidity and second is enthalpy
                        }

                    }
                    else
                    {
                        //offline mode
                        lb_device_status.Text = "disconnected";
                    }

                }
                else
                {
                    //The node id pulled form web
                    /*Steps :
                    1.Read the node latitude and longitude value                   
                    2.Update the database value from the web.
                    */
                    //first latitude and longitude
                    double latitudeVal = selectedBuildingList[0].latitude;
                    double longitudeVal = selectedBuildingList[0].longitude;
                    //Pulling data form web it will always be in temp and humidity form
                    GetDataFromWeb(latitudeVal, longitudeVal);

                   double temperature_Val = temp_pulled_from_web;
                   double  humidity_Val = double.Parse(hum_pulled);
                   double y_value = CalculateYFromXandHumidity(temperature_Val, humidity_Val/100);
                 //  MessageBox.Show("temp / temp_pulled_form_web = "+temperature_Val+"\nhum = "+humidity_Val+"\ny value web= " + y_value);
                    //Now lets update the values in db
                    UpdateNodeInfoToDB(node.id, temperature_Val, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);
                    //==============THIS ONE IF FOR ONLINE OFFLINE==========
                  //  lb_web_status.Text = "active";                            
                    //=====================END OF ONLINE OFFLINE============           
                }
            }  //Close of foreach now lets plot the values..

            //}catch(Exception ex)
            //{
            //    //Show nothingelse
            //}
            //now lets call the plot function...
            RefreshGraph();
            if (chartDetailList.Count > 0)
            {
                int id = indexOfChartSelected;    //This value is changed 
                LoadNodeAndLineFromDB(id);   //Lets make it passing the stirngs 

                // flagForInsertOrUpdateDataToDB = 1;
                //--This is also completed..
                // ReDrawingLineAndNode();

                if (InvokeRequired) {
                    this.Invoke(new MethodInvoker(ReDrawingLineAndNodeForScanHardware));
               // ReDrawingLineAndNodeForScanHardware();
                }
                else
                {
                    ReDrawingLineAndNodeForScanHardware();
                }

            }

            
    } //Close of the fxn



        public void ReDrawingLineAndNodeForScanHardware()
        {
            if (menuStripNodeInfoValues.Count > 0)
            {
                //--This is for replotting all the things again...

             // series1.Points.Clear();

                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() => series1.Points.Clear() ));

                }
                else
                {
                    // lb_device_status.Text = "connected";
                    series1.Points.Clear();
                }


                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                {
                    //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                    menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                }
                //--this is redraw functionality
                //foreach(var values in menuStripNodeInfoValues)
                //--Resetting the index value
                indexForSeriesNodePoint = 0;
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    string labelValue;
                    if (menuStripNodeInfoValues[x].showItemText == "Label")
                    {
                        labelValue = menuStripNodeInfoValues[x].label;
                    }
                    else if (menuStripNodeInfoValues[x].showItemText == "Name")
                    {
                        labelValue = menuStripNodeInfoValues[x].name;
                    }
                    else
                    {
                        labelValue = menuStripNodeInfoValues[x].source;
                    }


                    //   //--Redefined code bbk305
                    //   if (InvokeRequired) { 

                    //// this.Invoke(new MethodInvoker(ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size) ));

                    //   }

                     ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                   // ReDrawPointsForHardware(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);

                   //CODE : BBK305A
                   //--incrementIndex++;
                   indexForSeriesNodePoint++;

                }
                //--resetting incrementIndex
                //CODE: BBK305A  
                // incrementIndex = 0;

                if (menuStripNodeLineInfoValues.Count > 0)
                {

                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                    {
                        //  incrementIndex++;


                        //--tHIS IS REDEFINED code bbk305
                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness);

                    }

                }

                chart1.Invalidate();//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.

                //CODE :BBK305A \\
                //incrementIndex = 0;//reset the values again..
                indexForSeriesNodePoint = 0;//Resetting the value
            }//Close of if menustripnodeinfovalues

        }//--Close of the actual function....





        // int countX = 0;
        /// <summary>
        ///This info is about the nodes 
        /// </summary>
        //Lets build a dynaic list
        public class device_info_class
        {
            public string nodeID { get; set; }
            public string device_instance_id { get; set; }
            public string device_IP { get; set; }
            public string param1_id { get; set; }
            public string param2_id { get; set; }
            public string param1_info { get; set; }
            public string param2_info { get; set; }

            public string param1_identifier_type { get; set; }
            public string param2_identifier_type { get; set; }
            
        }
        List<device_info_class> device_info_list = new List<device_info_class>();//This list will store the values of device info

        /// <summary>
        /// This get the device info values form the database 
        /// </summary>
        /// <param name="nodeID1">the node id selected to search for </param>
        public void ReadDeviceInfoForNode(string nodeID1)
        {
            device_info_list.Clear();//Resetting list
            //string deviceTableName = "tbl_"+
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //lets get the id values...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            string sql1 = "SELECT * From  " + tableNameDevice + "  where nodeID='"+nodeID1+"'";
          //  MessageBox.Show("sql1=" + sql1);

            //(nodeID,device_instanceID,IP,param1ID_X,param2ID_Y) VALUES(@id,@instanceID,@IP,@param1,@param2)";

            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);
                
                    thisConnection.Open();
                           
                SQLiteDataReader reader = cmd.ExecuteReader();

            //    MessageBox.Show("cmd error= " + cmd.CommandText);

                while (reader.Read())
                    {
                        // AL_nodeId.Add(reader["nodeID"].ToString());
                        device_info_list.Add(new device_info_class
                        {
                           nodeID = nodeID1,
                           device_instance_id = reader["device_instanceID"].ToString(),
                           device_IP = reader["IP"].ToString(),
                           param1_id = reader["param1ID"].ToString(),
                           param2_id = reader["param2ID"].ToString(),
                           param1_info = reader["param1_info"].ToString(),
                           param2_info = reader["param2_info"].ToString()  ,
                           param1_identifier_type = reader["param1_identifier_type"].ToString(),
                           param2_identifier_type = reader["param2_identifier_type"].ToString()
                           

                        });
                    }

                
            }

        }


        public void PullDataFromHardware(string device_instance_id,string param1id,string param2id,string param1info,string param2info)
        {
            if(param1info == "temp" && param2info == "hum")
            {
                //param1 is temperature and param2 is humidity

            }
            else if (param1info == "temp" && param2info == "enthalpy")
            {

            }
            else if (param1info == "hum" && param2info == "enthalpy")
            {

            }
        }

        
        //private System.Timers.Timer timerForDevice;       
        private System.Timers.Timer atimer;
        int flagForTimer = 0;
        public void InitTimerForDevice()
        {
    
            flagForTimer = 1;
            atimer = new System.Timers.Timer();
            atimer.Enabled = true;
            atimer.Elapsed += timer1_Tick_For_Device;

            atimer.Interval = 1000*10;
         
        }

        //async Task<> foo()
        //{

        //}
        public async Task AsyncMethod1ForPullingData()
        {
            try { 
            // Do asynchronous work.
            await Task.Run(() => RefreshDataFromDeviceAndWeb());
            }
            catch (Exception ex)
            {

            }
        }
        public void timer1_Tick_For_Device(object sender, EventArgs e)
        {

            try { 
                if (InvokeRequired)
                {
                    //this.Invoke(new Action(() => RefreshDataFromDeviceAndWeb()));
                    this.Invoke(new Action(() => AsyncMethod1ForPullingData()));
                    return;
                }
                else
                {
                    AsyncMethod1ForPullingData();
                }

            }
            catch
            {

            }
          
                
        }


        public    double latValueFromDB = 0.0000;
       public     double longValueFromDB = 0.0000;
        public void PullLongitudeAndLatitude(string dbName)
        {

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string sql = "SELECT * From " + dbName + "  WHERE selection=1";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString))
            {
                using (SQLiteCommand cmdCount = new SQLiteCommand(sql, thisConnection))
                {
                    SQLiteDataReader reader = null;
                    thisConnection.Open();                   
                    reader = cmdCount.ExecuteReader();
                    while (reader.Read())
                    {
                        latValueFromDB = double.Parse(reader["latitude"].ToString());
                        longValueFromDB = double.Parse(reader["longitude"].ToString());
                         
                    }

                }
            }


        }

        List<bool> dataCheckList = new List<bool>();

        public bool CheckDeviceOnlineOffline(int deviceID, int Parent_SerialNum)
        {
            //Resetting
            dataCheckList.Clear();
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //   string databaseFile = databasePath + @"\Database\Buildings\Default_Building\Default_Building.mdb";
            //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source="+ databaseFile+ @";Persist Security Info=True ";
            //C:\Program Files (x86)\T3000\Database\Buildings\Default_Building
            //String connection = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\Program Files (x86)\T3000\Database\Buildings\Default_Building\Default_Building.mdb;Persist Security Info=True ";

            string sql = "select * from ALL_NODE  where Object_Instance = '" + deviceID + "' and Parent_SerialNum = '" + Parent_SerialNum + "'";
            //  MessageBox.Show("Connection = " + connection);
            //  MessageBox.Show("sql = " + sql);
            //DataSet myDataSet = new DataSet();
            bool status = false;
            if (BuildingSelected.Count > 0)
            {
                //MessageBox.Show("Building infor not found");
                //return null;

                /*
                This path is dynamic path we calculated based on this 
                <installed directory of T3000 (not psycho)>\... where ... = Database\..value returned by files
                */
                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].Building_Path;

                // MessageBox.Show(" onlin offline Path to building = " + againDbPath);


                try
                {
                    using (OleDbConnection conn = new OleDbConnection(againDbPath))
                    {
                        using (OleDbCommand cmd = new OleDbCommand(sql, conn))
                        {
                            conn.Open();
                            OleDbDataReader reader = cmd.ExecuteReader();
                            while (reader.Read())
                            {
                                dataCheckList.Add((bool)reader["Online_Status"]);
                            }

                        }

                        //MessageBox.Show("Count"+myDataSet[0])
                        //  MessageBox.Show("Count the value = " + dataCheckList.Count+" , value = "+dataCheckList[0]);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                if (dataCheckList.Count > 0)
                {
                    if (dataCheckList[0] == true)
                    {
                        status = true;
                    }
                }

                // MessageBox.Show( "datacheckcount= " + dataCheckList.Count + " Status = " + status);
            }
            return status;
        }




        /// <summary>
        ///        This is pulling the data from the hardware and putting it in the table
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_pull_data_Click(object sender, EventArgs e)
        {
            //This scan hardware will pull the data from the hardware and the weather station value.
            /*
            Steps : 1.calculate pull the data form the hardware and store it to the database .==>This will be node1
            2. Pull the data form the openweatherstation.org store the data to db ==>This will be node2 
            3. now plot these to nodes 
            4. Update the data of node one and node2 
            5. Replot the data again and agin in interval of 4 seconds.
            */
            //pulls the data form hardware
            //This is initial pulls 
            // ReadDataFromDevice();
            //  try { 
            //First time click the refresh...
            if (flagForTimer == 1)
            {
                atimer.Stop();
                atimer.Elapsed -=timer1_Tick_For_Device;
                atimer.Dispose();
                flagForTimer = 0;//This should not be done 
            }

            RefreshDataFromDeviceAndWeb();
            //AsyncMethod1ForPullingData();
            //initializes the timer
     InitTimerForDevice();
            //}catch(Exception ex)
            //{
            //    MessageBox.Show("error :"+ex.Message);
            //}


        }

        private void buildingChartSetting_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Disposing the timer when the device is closed.
            //timerForDevice.Stop();
            //timerForDevice.Tick -= new EventHandler(timer1_Tick_For_Device);
            //timerForDevice.Dispose();

            //this is for stoping the atimer of system.timer.timers
            if(flagForTimer == 1) { 
                atimer.Stop();
                atimer.Elapsed -= new System.Timers.ElapsedEventHandler(timer1_Tick_For_Device);
                atimer.Dispose();
                flagForTimer = 0;//This should not be done 
            }

        }


        /// <summary>
        /// Some vairbale  to store the humidity tmeperature and wind speed pulled form the weather services..
        /// </summary>

        double lat_val = 0.0000;
        double lng_val = 0.0000;
        //lets declare some variable to store the value...
        string city_name_pulled = "";
        string country_name_pulled = "";
        string last_update_pulled = "";
        string temp_pulled = "";
        string hum_pulled = "";
        string pressure_pulled = "";
        string wind_speed_pulled = "";
        string direction_pulled = "";
        string lat_pulled = "";
        string long_pulled = "";
        double temp_pulled_from_web = 0.000;
        /// <summary>
        /// This return the different values such as temperatue ,humidity and other parameters.
        /// </summary>
        /// <param name="latValue">Latitude </param>
        /// <param name="longValue">Longitude</param>


        public void GetDataFromWeb(double latValue,double longValue)
        {
         
         /*
         steps:
         * 0.pull the lat and long value stored in the database
         * 1.pull the data form web 
         * 2.Parse the xml data
         * 3.insert data
         * 4.display data
         */
                try
                {

                    //lets pull the vales offline values stored in db...  

                    //string connString = @"Data Source=" + databaseFile + ";Version=3;";

                     lat_val = latValue;//store_station_list[index_for_station_selected].lat;
                     lng_val = longValue; //store_station_list[index_for_station_selected].lng;
                        //display lat lng...
                        //MessageBox.Show("lat= "+lat_val+" lng = "+lng_val);
                        using (var wc = new System.Net.WebClient())
                        {
                            // var json = await httpClient.GetStringAsync(api_url);

                            // double station_id = store_station_list[index_for_station_selected].id;
                            //MessageBox.Show("lat = " + lat_val + "lng = " + lng_val);
                            string api_url = "http://api.openweathermap.org/data/2.5/weather?mode=xml&lat=" + lat_val + "&lon=" + lng_val + "&appid=615afd606af791f572a1f92b27a68bcd";
                            //string api_url = "http://api.openweathermap.org/data/2.5/station?id="+station_id+"&APPID=615afd606af791f572a1f92b27a68bcd";

                            var data = wc.DownloadString(api_url);
                            //  MessageBox.Show("string apic return =" + data);
                            string xml_string = data.ToString();
                                                
                            //xml parsing...
                            XmlDocument xml = new XmlDocument();
                            xml.LoadXml(xml_string);

                            XmlNodeList elem_city = xml.GetElementsByTagName("city");
                            foreach (XmlNode x in elem_city)
                            {
                                city_name_pulled = x.Attributes["name"].Value;
                                // MessageBox.Show("city name = " + city_name_pulled);
                            }
                            //for temperature
                            XmlNodeList temp_list = xml.GetElementsByTagName("temperature");
                            foreach (XmlNode x in temp_list)
                            {
                                temp_pulled = x.Attributes["value"].Value;
                                //      MessageBox.Show("temp  = " + temp_pulled);
                            }
                            //for humidity
                            XmlNodeList hum_list = xml.GetElementsByTagName("humidity");
                            foreach (XmlNode x in hum_list)
                            {
                                hum_pulled = x.Attributes["value"].Value;
                                //MessageBox.Show("hum  = " + hum_pulled);
                            }
                            //for pressure..
                            XmlNodeList pressure_list = xml.GetElementsByTagName("pressure");
                            foreach (XmlNode x in pressure_list)
                            {
                                pressure_pulled = x.Attributes["value"].Value;
                                //MessageBox.Show("press = " + pressure_pulled);
                            }
                            //for wind 

                            XmlNodeList wind_list = xml.GetElementsByTagName("speed");
                            foreach (XmlNode x in wind_list)
                            {
                                wind_speed_pulled = x.Attributes["value"].Value;
                                //   MessageBox.Show("wind speed = " + wind_speed_pulled);
                            }
                            //for direction..
                            XmlNodeList direction_list = xml.GetElementsByTagName("direction");
                            foreach (XmlNode x in direction_list)
                            {
                                direction_pulled = x.Attributes["name"].Value;
                                // MessageBox.Show("direction name = " + direction_pulled);
                            }
                            //for lat and long of station...
                            XmlNodeList coord_list = xml.GetElementsByTagName("coord");
                            foreach (XmlNode x in coord_list)
                            {
                                lat_pulled = x.Attributes["lat"].Value;
                                long_pulled = x.Attributes["lon"].Value;

                                // MessageBox.Show("lat = " + lat_pulled +"long" +long_pulled);
                            }
                            //for last date update time 
                            XmlNodeList last_update_list = xml.GetElementsByTagName("lastupdate");
                            foreach (XmlNode x in last_update_list)
                            {
                                last_update_pulled = x.Attributes["value"].Value;
                                // MessageBox.Show("last update date = " + last_update_pulled);
                            }

                            //for country..
                            XmlNodeList country_list = xml.GetElementsByTagName("country");
                            foreach (XmlNode x in country_list)
                            {
                                country_name_pulled = x.InnerText;
                                //  MessageBox.Show("country name = " + country_name_pulled);
                            }
                                                
                            //step3. insert the values pulled into a database..
                
                            string loc_value = "";
                            double d = 0.0000;
                            double temp_adjust = double.Parse(temp_pulled.ToString()) - 273.15;
                            temp_pulled_from_web = temp_adjust;//This is the temperature in deg celcius,while temp_pulled is in kelvin
                      
                                double R = 6371.0;//radius of earth in Km
                                double dlon = double.Parse(long_pulled.ToString()) - lng_val;
                                double dlat = double.Parse(lat_pulled.ToString()) - lat_val;
                                double a = Math.Pow((Math.Sin(dlat / 2)), 2) + Math.Cos(lat_val) * Math.Cos(double.Parse(lat_pulled.ToString())) * Math.Pow((Math.Sin(dlon / 2)), 2);
                                double c = 2 * Math.Atan2(Math.Sqrt(a), Math.Sqrt(1 - a));
                                d = R * c;//This is the distance
                                loc_value = country_name_pulled + "," + city_name_pulled;
                                //lets check a simple thing if the index is pressent then update else insert the data...

                                /*steps.. count the number of items in a column 
                                 1.if the count is less than the index_selected then update else insert
                                 */
                                                  
                    lb_web_status.Text = "active";
                    // MessageBox.Show("success !");
                }

                  

                }
                catch (Exception ex)
                {
                //The device is offline and so need to be made inactive 
                lb_web_status.Text = "inactive";

                }


        } //Close of the main function 

        private void button2_Click(object sender, EventArgs e)
        {
            //GetDataFromWeb(27, 86);
            //MessageBox.Show("data form web x temp =" + temp_pulled_from_web + " ,y hum = " + hum_pulled);

        }

        private void btn_reset_graph_Click(object sender, EventArgs e)
        {

        }

        private void showComfortZoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Steps to set the comfort zone ...
            /*
            1.Let give user to set min and max humidity and min and max temp
             1.1 give user option for types of comfort zone.
            2.lets save those value in db 
            3.plot on graph base on those values.
            */
            //Lets show the comfortzone option
           // comfort_zone cf = new comfort_zone(this);
            //cf.Show();
        }


        //data type for comfort zone
        public class dataTypeForCF
        {
            public string id { get; set; }
            public string name { get; set; }
            public string min_temp { get; set; }
            public string max_temp { get; set; }
            public string min_hum { get; set; }
            public string max_hum { get; set; }
            public Color colorValue { get; set; }
        }

      public  List<dataTypeForCF> listComfortZoneDetail = new List<dataTypeForCF>();
        public void PullComfortZoneData()
        {
            listComfortZoneDetail.Clear();//resetting the chart list value..
            string tableForComfortZoneDetail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "select * from " + tableForComfortZoneDetail + " ";

                SQLiteCommand command = new SQLiteCommand(sql, connection);

                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    listComfortZoneDetail.Add(new dataTypeForCF
                    {

                        id = reader["id"].ToString(),
                        name = reader["name"].ToString(),
                        min_temp = reader["min_temp"].ToString(), //this is in string formate
                        max_temp = reader["max_temp"].ToString(),
                        min_hum = reader["min_hum"].ToString(),
                        max_hum = reader["max_hum"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString())


                    });
                }
            }

        } //Close of fxn

        public List<dataTypeForCF> listchartComfortZoneInfoSingle = new List<dataTypeForCF>(); //This one contains the single data for the comfort zone 

        public void pullComfortZoneInfoSingle(string comfortzoneid)
        {
            listchartComfortZoneInfoSingle.Clear();//resetting the chart list value..
            string tableForComfortZoneDetail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();                     
                string sql = "select * from " + tableForComfortZoneDetail + " where id='"+comfortzoneid+"'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    listchartComfortZoneInfoSingle.Add(new dataTypeForCF
                    {
                        id = comfortzoneid,
                        name = reader["name"].ToString(),
                        min_temp = reader["min_temp"].ToString(), //this is in string formate
                        max_temp = reader["max_temp"].ToString(),
                        min_hum = reader["min_hum"].ToString(),
                        max_hum = reader["max_hum"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString())
                    });
                }
            }

        }

        public class datatype_for_comfortzone
        {
            public string chartid { get; set; }
            public string comfortzoneid { get; set; }
            public string status { get; set; }
        }
        public List<datatype_for_comfortzone> default_comfort_zone_of_chart = new List<datatype_for_comfortzone>();
        public void InfoForComfortZoneOfChart(string chartID)
        {
            default_comfort_zone_of_chart.Clear();//resetting the chart list value..
            string tableForComfortZoneDetail = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql = "select * from " + tableForComfortZoneDetail + " where chartID='" + chartID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);

                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    default_comfort_zone_of_chart.Add(new datatype_for_comfortzone
                    {                     
                         chartid   = chartID,//reader["chartID"].ToString(),
                         comfortzoneid  = reader["comfort_zone_ID"].ToString(),
                         status = reader["status"].ToString()
                    });
                }
            }

        }

        /// <summary>
        /// updates the chart info for comfort zone
        /// </summary>
        /// <param name="chartid">id of chart selected</param>
        /// <param name="comfortzone_id">comfort zone id for each chart</param>
        public void update_comfort_zone_for_chart(string chartid, string comfortzone_id,string status)
        {

            //string idValue = GetGUID();
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();                
                string sql_string = "UPDATE " + tableName + "   set  comfort_zone_ID =@cfid ,status =@status  where chartID  =@chartid ";
                 SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@cfid", comfortzone_id);
                command.Parameters.AddWithValue("@chartid", chartid);
                command.Parameters.AddWithValue("@status", status);
                command.ExecuteNonQuery();
            }


        }

        public int CountNumberOfItem(string chartId)
        {

            //default_comfort_zone_of_chart.Clear();//resetting the chart list value..
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            int count = 0;
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {               
                connection.Open();
                string sql = "SELECT COUNT(1) FROM "+tableName+" WHERE chartID='"+chartId+"'";
                //MessageBox.Show("sql : " + sql);
                SQLiteCommand command = new SQLiteCommand(sql, connection);                   
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {               
                    count = int.Parse(reader[0].ToString());
                }
            }

            return count;
        }
        /// <summary>
        /// load the comfort zone when chart is selected
        /// </summary>
        /// <param name="chartid"></param>
        public void LoadComfortZone(string chartid)
        {
            //First load the char id if present
            //This load the comfortzone info for a chart if present
            int count_item = 0;
            count_item = CountNumberOfItem(chartid);//This will count the value
            if (count_item > 0)
            { 
                InfoForComfortZoneOfChart(chartid);

                if(default_comfort_zone_of_chart.Count > 0)
                {
                    //the info is present 
                    //now load the comfort zone..
                    pullComfortZoneInfoSingle(default_comfort_zone_of_chart[0].comfortzoneid); //This pulls the comfort zone data
                    //only plot if the comfortzone is enabled
                    if(default_comfort_zone_of_chart[0].status == "enable")
                    { 
                        if (listchartComfortZoneInfoSingle.Count > 0)
                        {
                        //Then we need to plot comfort zone 
                        PlotComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp),double.Parse( listchartComfortZoneInfoSingle[0].max_temp),double.Parse( listchartComfortZoneInfoSingle[0].min_hum),double.Parse( listchartComfortZoneInfoSingle[0].max_hum),listchartComfortZoneInfoSingle[0].colorValue, listchartComfortZoneInfoSingle[0].name);

                            flagShow = 1;//lets enable flag
                           // MessageBox.Show("Enable flowshow =1");
                        }
                    }
                    else
                    {
                        flagShow = 0;//Off if not enable
                       // MessageBox.Show("disable flowshow =0");
                    }
                }
            }//close of if

        }

        public void LoadComfortZoneIfOn(string chartid)
        {
            //First load the char id if present
            //This load the comfortzone info for a chart if present
            int count_item = 0;
            count_item = CountNumberOfItem(chartid);//This will count the value
            if (count_item > 0)
            {
                InfoForComfortZoneOfChart(chartid);

                if (default_comfort_zone_of_chart.Count > 0)
                {
                    //the info is present 
                    //now load the comfort zone..
                    pullComfortZoneInfoSingle(default_comfort_zone_of_chart[0].comfortzoneid); //This pulls the comfort zone data
                    //only plot if the comfortzone is enabled
                    if (default_comfort_zone_of_chart[0].status == "enable")
                    {
                        if (listchartComfortZoneInfoSingle.Count > 0)
                        {
                            //Then we need to plot comfort zone 
                            PlotComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum), listchartComfortZoneInfoSingle[0].colorValue, listchartComfortZoneInfoSingle[0].name);

                            // flagShow = 1;//lets enable flag
                            // MessageBox.Show("Enable flowshow =1");
                        }

                    }
                }//close of if

            }
        }



        public void insertOrUpdateComfortChartSetting(string chartID,string comfortzone_ID)
        {
            //InsertChartComfortzoneInfo(string chartid, string comfortzone_id)

            int count_item = 0;
            count_item = CountNumberOfItem(chartID);//This will count the value
            if (count_item > 0)
            {

                InfoForComfortZoneOfChart(chartID);

                if (default_comfort_zone_of_chart.Count > 0)
                {
                    //chart presnet update
                    string status = "enable";
                    update_comfort_zone_for_chart(chartID, comfortzone_ID,status);
                }
                else
                {
                    //not present insert info
                    InsertChartComfortzoneInfo(chartID, comfortzone_ID);
                }
            }
        }

        public void PlotComfortZone(double Tmin, double Tmax, double Hmin, double Hmax, Color c, string name)
        {
            //This function will help to plot the values in the chart.
             for (int i = (int)Tmin; i <= (int)Tmax; i++)
            //for (double i = Tmin; i <= Tmax; i+=0.25)
            {
                //First reset the series if present and the re add them

                Series s = new Series("vertical_temp" + i);
        
                if (chart1.Series.IndexOf(s.Name) != -1)
                {
                    //MessageBox.Show("Series exits");
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                }


            }

            //MessageBox.Show("Plot comfort zone");

            //NOw add horizontal series.
            for (int i = (int)Hmin; i <= (int)Hmax; i += 1)
            {
             //   for (double i = Hmin; i <= Hmax; i += 0.25)
            //{
                //remove if first present

                Series s = new Series("horizontal_hum" + i);
                
                if (chart1.Series.IndexOf(s.Name) != -1)
                {                                              
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                }

            }


            //add both types of series
            for (int i = (int)Tmin; i <= (int)Tmax; i++)
            {
            //    for (double i = Tmin; i <= Tmax; i += 0.25)
            //{
                Series s = new Series("vertical_temp" + i);
                s.Color = c;                
                s.ChartType = SeriesChartType.Line;
                // s.MarkerSize = 5;
                //s.BorderWidth = 10;
                chart1.Series.Add(s);
            }

            //Refresh  the chart now
            chart1.Invalidate();
            chart1.Refresh();//This is for refresh

            for (int i = (int)Hmin; i <= (int)Hmax; i += 1)
            {
            //    for (double i = Hmin; i <= Hmax; i += 0.25)
           // {
                //add now
                Series s = new Series("horizontal_hum" + i);
                s.Color = c;
                s.ChartType = SeriesChartType.Spline;
                //s.MarkerSize = 15;
                s.BorderWidth = 10;
                chart1.Series.Add(s);
            }

            //Now lets do the actual plotting part.


            double phi_min = Hmin/100;//need to change to decimal
            double phi_max = Hmax / 100;


            //This one is for adding horizontal lines 
            double phi = phi_min; //0.1;            
            double x2 = 0;
           // int ival = 2;
            //int indexValue = (int)Hmin;
            double indexValue = Hmin;
            double  patm = form1Object.AirPressureFromDB * 0.001;//101.325; ;//thsi need tochange
            for (phi = phi_min; phi <= phi_max; phi += 0.01) //increment by 2 value
           // for (phi = phi_min; phi <= phi_max; phi += 0.0025) //increment by 2 value
            {
                for (int temp = (int)Tmin; temp <(int)Tmax;temp++)
                //for (double temp = Tmin; temp <Tmax;temp+=0.25)
                {

                    double pg_value = Double.Parse(pg[temp].ToString());
                    double  wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
                    double y = wg_calc;
                    x2 = temp;//double.Parse(t[i].ToString());
                    chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y);
             
                }//close of for
                 //MessageBox.Show(s1);
                indexValue += 1;
                //indexValue += 0.25;
            }  //close fo the second for loop


            //Now this one is for adding vertical line for lines
            for (int temp =(int)Tmin; temp < (int)Tmax; temp++)                                                               
           // for (double temp =Tmin; temp < Tmax; temp+=0.25)
                {
                     double xx1, yy1, xx2, yy2;
                    double pg_value = Double.Parse(pg[temp].ToString());
                    double wg_calc = (622 * phi_min * pg_value / (patm - phi_min * pg_value));
                    yy1 = wg_calc;
                    xx1 = temp;//double.Parse(t[i].ToString());


                    xx2 = xx1; //here xx1 =xx2 same ony y value changes
                    double wg_calc2 = (622 * phi_max * pg_value / (patm - phi_max * pg_value));
                    yy2 = wg_calc2;

                    chart1.Series["vertical_temp" + temp].Points.AddXY(xx1,yy1);
                    chart1.Series["vertical_temp" + temp].Points.AddXY(xx2, yy2);

            }//close of for                                                 

        } //Close of our function

        /// <summary>
        /// This function helps to insert the comfort zone setting in the database.
        /// </summary>
        /// <param name="name">name of the comfort zone</param>
        /// <param name="min_temp">min temperature</param>
        /// <param name="max_temp">max temperature</param>
        /// <param name="min_hum">min humidity</param>
        /// <param name="max_hum">max humidity</param>
        /// <param name="color">color value</param>
        public void InsertComfortZoneValue(string name, double  min_temp, double max_temp, double min_hum,double max_hum,Color color)
        {

            string idValue = GetGUID(); //insert new id value
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableName + "(id,name,min_temp,max_temp,min_hum,max_hum,colorValue) VALUES(@id,@name,@min_t,@max_t,@min_h,@max_h,@color)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
             
                command.Parameters.AddWithValue("@id", idValue);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@min_t", min_temp.ToString());
                command.Parameters.AddWithValue("@max_t", max_temp.ToString());
                command.Parameters.AddWithValue("@min_h", min_hum.ToString());
                command.Parameters.AddWithValue("@max_h", max_hum.ToString());
                command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(color));               
                command.ExecuteNonQuery();
            }
        }

        public void InsertChartComfortzoneInfo(string chartid, string comfortzone_id)
        {

            //string idValue = GetGUID();
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableName + "(chartID,comfort_zone_ID) VALUES(@chartid,@comfortzoneid)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chartid);
                command.Parameters.AddWithValue("@comfortzoneid", comfortzone_id);               
                command.ExecuteNonQuery();
            }
        }


        public void UpdateComfortZoneValue(string id,string name, double min_temp, double max_temp, double min_hum, double max_hum, Color color)
        {

            //string idValue = GetGUID();
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "UPDATE " + tableName + "   set  name =@name ,  min_temp=@min_t, max_temp=@max_t, min_hum=@min_h, max_hum=@max_h , colorValue=@color  where id  =@id ";

              //  "update  " + tableName + "(id,name,min_temp,max_temp,min_hum,max_hum,colorValue) VALUES(@id,@name,@min_t,@max_t,@min_h,@max_h,@color)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@min_t", min_temp.ToString());
                command.Parameters.AddWithValue("@max_t", max_temp.ToString());
                command.Parameters.AddWithValue("@min_h", min_hum.ToString());
                command.Parameters.AddWithValue("@max_h", max_hum.ToString());
                command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(color));
                command.ExecuteNonQuery();
            }


        }

        //deletion of the values...
        public void DeleteComfortZoneData(string id)
        {
            /*
            steps:1. Delete form the ..._comfort_zone_detail and then form _comfort_zone_chart_setting

            */

            //string idValue = GetGUID();
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string tableName2 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";   // currentLineTableFromDB;

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "delete from  " + tableName + "  where id  =@id ";           
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", id);              
                command.ExecuteNonQuery();


                string sql_string2 = "delete from  " + tableName2 + "  where comfort_zone_ID  =@id ";
                SQLiteCommand command2 = new SQLiteCommand(sql_string2, connection);
                command2.CommandType = CommandType.Text;
                command2.Parameters.AddWithValue("@id", id);
                command2.ExecuteNonQuery();

            }

        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Steps to set the comfort zone ...
            /*
            1.Let give user to set min and max humidity and min and max temp
             1.1 give user option for types of comfort zone.
            2.lets save those value in db 
            3.plot on graph base on those values.
            */
            //Lets show the comfortzone option
         // comfort_zone cf = new comfort_zone(this);
         // cf.Show();
        }

        int flagShow = 1; //means on
        public void ShowOrHideForComfortZone()
        {      
            /*
            Note : This flogShow has just opposite property it si correct 
            1 means on before the chart load and while loading but when already on we dont need to on it again
            when click again when the intial state is on it should be off so 1 triggers off because the state is ON previously
            we need to make it OFF
            and IF the state is OFF initially 0 makes it ON the next time it is clicked 
            it is little confusing but true
            */
            if(flagShow == 0)
            {
                //=================uncomment this is requred after========= //
                //It is dissabled so enable it
                enableToolStripMenuItem.Checked = true;
                flagShow = 1;
                //enable off flag
                             //When enabled we need to show the comfort zone
                             //First update the value and then enable or dissable
                string status = "enable";
                //===========================end===============================//


                //MessageBox.Show("Enable ");
                //default_comfort_zone_of_chart already contains the loaded value
                //default_comfort_zone_of_chart[0].
                if (default_comfort_zone_of_chart.Count > 0)
                { 
                    update_comfort_zone_for_chart(default_comfort_zone_of_chart[0].chartid, default_comfort_zone_of_chart[0].comfortzoneid, status);
                    //After updating we need to load the values
                    //we need to clear the chart before loading.
                    ClearComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum));
                    LoadComfortZone(default_comfort_zone_of_chart[0].chartid);
                }
            }
            else
            {
                //================enable this if required=========
                enableToolStripMenuItem.Checked = false;
                flagShow = 0; //Enable on flag for next time
                //When dissable hide comfort zone
                string status = "disable";                                             
                //==================end enable this if require============//
                //MessageBox.Show("disable ");
                //default_comfort_zone_of_chart already contains the loaded value
                //default_comfort_zone_of_chart[0].
                if (default_comfort_zone_of_chart.Count > 0)
                {
                    update_comfort_zone_for_chart(default_comfort_zone_of_chart[0].chartid, default_comfort_zone_of_chart[0].comfortzoneid, status);
                    //After updating we need to load the values
                    //We need to clear the chart first..
                    ClearComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum));
                    LoadComfortZone(default_comfort_zone_of_chart[0].chartid);
                }

            }

        }

        //Comfrot zone setting after node edited
       

        public void ClearComfortZone(double Tmin, double Tmax, double Hmin, double Hmax)
        {
            //This function will help to plot the values in the chart.
            for (int i = (int)Tmin; i <= (int)Tmax; i++)

        //    for (double i = Tmin; i <= Tmax; i+=0.25)
            {
                //First reset the series if present and the re add them

                Series s = new Series("vertical_temp" + i);

                if (chart1.Series.IndexOf(s.Name) != -1)
                {
                    //MessageBox.Show("Series exits");
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                }


            }


            //NOw add horizontal series.
            for (int i = (int)Hmin; i <= (int)Hmax; i += 1)
           // for (double i = Hmin; i <= Hmax; i += 0.25)
            {
                //remove if first present

                Series s = new Series("horizontal_hum" + i);
                if (chart1.Series.IndexOf(s.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                }

            }

            //This one for refreshing the chart
            chart1.Invalidate();
            chart1.Refresh();
        }
        private void enableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ////checked or not 
            //if(flagShow == 0)
            //{
            //    //dissable
            //    //It is dissabled so enable it
            //    enableToolStripMenuItem.Checked = true;
            //    flagShow = 1;//enable off flag
            //    //When enabled we need to show the comfort zone
            //    //First update the value and then enable or dissable
            //    string status = "enable";
            //}
            //else
            //{
            //    //do this ...

            //}


            ShowOrHideForComfortZone();
        }


        
        //This list holds the detail of the chart to be deleted
        List<nodeDataType> deleteNodeDetailList = new List<nodeDataType>();

        //Read node Values
        /// <summary>
        ///    help in reading all the node info for a particular chart
        /// </summary>
        /// <param name="chartID">char id</param>
        public void ReadNodeInfoToDelete(string chartNodeID)
        {
            deleteNodeDetailList.Clear();
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from  "+ tableName + "  where chart_respective_nodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartNodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    deleteNodeDetailList.Add(new nodeDataType
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        nodeID = reader["nodeID"].ToString(),
                        xValue = double.Parse(reader["xValue"].ToString()),
                        yValue = double.Parse(reader["yValue"].ToString()),
                        source = reader["source"].ToString(),
                        name = reader["name"].ToString(),
                        label = reader["label"].ToString(),
                        colorValue = reader["colorValue"].ToString(),
                        showTextItem = reader["showTextItem"].ToString(),
                        nodeSize = int.Parse(reader["nodeSize"].ToString())                       
                    });
                }
            }//Close of using 
        }

        //Same for the line 
        public void DeleteLine(string lineID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chart_respective_lineID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", lineID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
           
                
            }//Close of using 
        }



        public void DeleteNode(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chart_respective_nodeID = @id_value";                 
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();


            }//Close of using 
        }

        public void DeleteComfortZoneSettingForChart(string chartID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using 
        }


        public void DeleteNodeDeviceInfo(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using 
        }



        public void DeleteChart(string chartID)
        {
            /*
            Steps :1. Frist read line info,nodeinfo,comfortzone info for particular chart id
                    2.Then delete line value using lineID,
                    3.Delete node values using nodeID
                    4.Delete comfortzone value using chartid
                    5.delete the chart info using chart id value
            */

            //string chartID =   
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using          

        }


        private void deleteHandlerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This is the delete operation for the handler.....
            /*
            Steps: 1. delete the row  of the table using the id portion.
            2. Delete the corresponding tables related to the row.             
            */
           
            if (dataGridView1.CurrentCell.RowIndex > -1 &&  dataGridView1.CurrentCell.RowIndex  < chartDetailList.Count )//Header is selected..
            {
                int selectedItemIndex = dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

                //we need to find the corresponding tables for deletion.

                //int id = selectedItemIndex;

                string chartID = chartDetailList[selectedItemIndex].chartID;
                  string chart_respective_node_ID = chartDetailList[selectedItemIndex].chart_respective_nodeID;
                string chart_respective_line_ID = chartDetailList[selectedItemIndex].chart_respective_lineID;
                //First read the node values for particular chart 
                ReadNodeInfoToDelete(chart_respective_node_ID);

                //For all node delete the device list
                if (deleteNodeDetailList.Count > 0)
                {
                    //if there is data then delete the device infor
                    foreach(var item in deleteNodeDetailList)
                    {
                        DeleteNodeDeviceInfo(item.nodeID);
                    }
                }

                //After this deletion lets delete the line info
                DeleteLine(chart_respective_line_ID);//This deletes the line

                //now delete comfort zone..
                DeleteComfortZoneSettingForChart(chartID);

                //now delete the node value
                DeleteNode(chart_respective_node_ID);

                //Now delete the chart itself
                DeleteChart(chartID);


            }//Close of if

            //--Refreshing the tables for all the datas..

            dataGridView1.Rows.Clear();
            DataGridView_Show_Data();
            dataGridView1.Rows.Add();
            //If there is no rows in datagridview...
            //if(dataGridView1.Rows.Count < 1) { 

            //  dataGridView1.Rows.Add();//Adding new rows in datagridview.
            //  }else if (dataGridView1.Rows[dataGridView1.Rows.Count].Cells[0].Value != null)
            //  {
            //      dataGridView1.Rows.Add();//Adding new rows in datagridview.
            //  }
            RefreshGraph();

            //dataGridView2.Rows.Clear();
            MessageBox.Show("Chart deleted successfully !");

        }


        public void SaveConfiguration()
        {

            LoadNode_LineAndDeviceInfoFromDB(indexOfChartSelected);

           //--test
           // MessageBox.Show("count both = " + deviceInfoForSavingToXML.Count + "," + nodeInfoForSavingToXML.Count);
            //string sx=null;
            //foreach (var node in nodeInfoForSavingToXML)
            //{
            //    sx += node.id+",\t";
            //}
            //sx += "\n";
            //foreach (var node in deviceInfoForSavingToXML)
            //{
            //    sx += node.nodeID + ",\t";
            //}
            //MessageBox.Show("values = " + sx);

            XmlDocument xmlDoc = new XmlDocument();
            //XmlWriter xw = new XmlWriter();
            //lets create an xml document using a string in xml formate
                                     
            XmlNode r_node = xmlDoc.CreateElement("RootNode");

            xmlDoc.AppendChild(r_node);

            XmlNode identifier = xmlDoc.CreateElement("identifier");
            identifier.InnerText = "BuildingSetting";

            r_node.AppendChild(identifier);


            XmlNode rootNode = xmlDoc.CreateElement("nodes");
            // xmlDoc.AppendChild(rootNode);
            r_node.AppendChild(rootNode);
            string s = null;
            //loading the string ...

            if (nodeInfoForSavingToXML.Count > 0) { 

            foreach (var node in nodeInfoForSavingToXML)
            {
               if(node.source == "Device") { 

                XmlNode userNode = xmlDoc.CreateElement("node");
     
                rootNode.AppendChild(userNode);


                        //now append name
                        XmlNode nodeID = xmlDoc.CreateElement("nodeID");
                        nodeID.InnerText = node.id.ToString();
                        userNode.AppendChild(nodeID);




                        //now append name
                        XmlNode userNodeName = xmlDoc.CreateElement("name");                
                userNodeName.InnerText = node.name.ToString();
                userNode.AppendChild(userNodeName);


                //now append the label
                XmlNode userNodeLable = xmlDoc.CreateElement("label");
                userNodeLable.InnerText = node.label.ToString();//'"' + node.label + '"';
                userNode.AppendChild(userNodeLable);

                //now append the source
                XmlNode userNodeSource = xmlDoc.CreateElement("source");
                userNodeSource.InnerText = node.source.ToString();  //'"' + node.source + '"';
                userNode.AppendChild(userNodeSource);

                //now append the color
                XmlNode userNodeColor = xmlDoc.CreateElement("color");
                userNodeColor.InnerText = ColorTranslator.ToHtml(node.colorValue).ToString();//'"' + ColorTranslator.ToHtml(node.colorValue).ToString() + '"';
                userNode.AppendChild(userNodeColor);

                //now append the xvalue
                XmlNode userNodeXValue = xmlDoc.CreateElement("xvalue");
                userNodeXValue.InnerText = node.xVal.ToString();//'"' + node.xVal.ToString() + '"';
                userNode.AppendChild(userNodeXValue);

                //now append the yvalue
                XmlNode userNodeYValue = xmlDoc.CreateElement("yvalue");
                userNodeYValue.InnerText = node.yVal.ToString(); //'"' + node.yVal.ToString() + '"';
                userNode.AppendChild(userNodeYValue);


                //now append the showTextItem
                XmlNode userNodeShowTextItem = xmlDoc.CreateElement("showTextItem");
                userNodeShowTextItem.InnerText = node.showItemText.ToString(); //'"' + node.showItemText.ToString() + '"';
                userNode.AppendChild(userNodeShowTextItem);

                        //--node size
                 XmlNode nodesize = xmlDoc.CreateElement("nodesize");
                 nodesize.InnerText = node.marker_Size.ToString(); //'"' + node.showItemText.ToString() + '"';
                 userNode.AppendChild(nodesize);


                        //--WE ALSO need to add the user information.

                        foreach (var node_device_info in deviceInfoForSavingToXML)
                    {
                        if(node.id == node_device_info.nodeID)
                        {
                            //for instance id
                            XmlNode deviceinstance = xmlDoc.CreateElement("deviceInstance");
                            deviceinstance.InnerText = node_device_info.device_instance_id.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(deviceinstance);

                            //for ip 
                            XmlNode ip = xmlDoc.CreateElement("ip");
                            ip.InnerText = node_device_info.ip.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(ip);

                            //for param1id
                            XmlNode param1id = xmlDoc.CreateElement("param1id");
                            param1id.InnerText = node_device_info.param1id.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param1id);

                            //for param2id
                            XmlNode param2id = xmlDoc.CreateElement("param2id");
                            param2id.InnerText = node_device_info.param2id.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param2id);

                            //for param1info
                            XmlNode param1info = xmlDoc.CreateElement("param1info");
                            param1info.InnerText = node_device_info.param1info.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param1info);

                            //for param2info
                            XmlNode param2info = xmlDoc.CreateElement("param2info");
                            param2info.InnerText = node_device_info.param2info.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param2info);

                            //for param1id_type
                            XmlNode param1id_type = xmlDoc.CreateElement("param1id_type");
                            param1id_type.InnerText = node_device_info.param1_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param1id_type);

                            //for param2id_type
                            XmlNode param2id_type = xmlDoc.CreateElement("param2id_type");
                            param2id_type.InnerText = node_device_info.param2_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
                            userNode.AppendChild(param2id_type);

                                                       

                        }


                    }

                    }//clsoe of 
                    else
                    {
                        //the node is form the web so do not load


                        XmlNode userNode = xmlDoc.CreateElement("node");

                        rootNode.AppendChild(userNode);


                        //now append nodeid
                        XmlNode nodeID = xmlDoc.CreateElement("nodeID");
                        nodeID.InnerText = node.id.ToString();
                        userNode.AppendChild(nodeID);


                        //now append name
                        XmlNode userNodeName = xmlDoc.CreateElement("name");
                        // XmlAttribute attribute = xmlDoc.CreateAttribute("value");
                        //attribute.Value = '"' + node.name + '"';
                        //userNodeName.Attributes.Append(attribute);
                        userNodeName.InnerText = node.name.ToString();
                        userNode.AppendChild(userNodeName);


                        //now append the label
                        XmlNode userNodeLable = xmlDoc.CreateElement("label");
                        userNodeLable.InnerText = node.label.ToString();//'"' + node.label + '"';
                        userNode.AppendChild(userNodeLable);

                        //now append the source
                        XmlNode userNodeSource = xmlDoc.CreateElement("source");
                        userNodeSource.InnerText = node.source.ToString();  //'"' + node.source + '"';
                        userNode.AppendChild(userNodeSource);

                        //now append the color
                        XmlNode userNodeColor = xmlDoc.CreateElement("color");
                        userNodeColor.InnerText = ColorTranslator.ToHtml(node.colorValue).ToString();//'"' + ColorTranslator.ToHtml(node.colorValue).ToString() + '"';
                        userNode.AppendChild(userNodeColor);

                        //now append the xvalue
                        XmlNode userNodeXValue = xmlDoc.CreateElement("xvalue");
                        userNodeXValue.InnerText = node.xVal.ToString();//'"' + node.xVal.ToString() + '"';
                        userNode.AppendChild(userNodeXValue);

                        //now append the yvalue
                        XmlNode userNodeYValue = xmlDoc.CreateElement("yvalue");
                        userNodeYValue.InnerText = node.yVal.ToString(); //'"' + node.yVal.ToString() + '"';
                        userNode.AppendChild(userNodeYValue);


                        //now append the showTextItem
                        XmlNode userNodeShowTextItem = xmlDoc.CreateElement("showTextItem");
                        userNodeShowTextItem.InnerText = node.showItemText.ToString(); //'"' + node.showItemText.ToString() + '"';
                        userNode.AppendChild(userNodeShowTextItem);

                        //--node size
                        XmlNode nodesize = xmlDoc.CreateElement("nodesize");
                        nodesize.InnerText = node.marker_Size.ToString(); //'"' + node.showItemText.ToString() + '"';
                        userNode.AppendChild(nodesize);

                        //--WE ALSO need to add the user information.
                        
                        //for instance id
                        XmlNode deviceinstance = xmlDoc.CreateElement("deviceInstance");
                        deviceinstance.InnerText = "";// node_device_info.device_instance_id.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(deviceinstance);

                                //for ip 
                                XmlNode ip = xmlDoc.CreateElement("ip");
                                ip.InnerText = "";// node_device_info.ip.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(ip);

                                //for param1id
                                XmlNode param1id = xmlDoc.CreateElement("param1id");
                                param1id.InnerText = "";//node_device_info.param1id.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param1id);

                                //for param2id
                                XmlNode param2id = xmlDoc.CreateElement("param2id");
                                param2id.InnerText =  "";// node_device_info.param2id.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param2id);

                                //for param1info
                                XmlNode param1info = xmlDoc.CreateElement("param1info");
                                param1info.InnerText = "";//node_device_info.param1info.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param1info);

                                //for param2info
                                XmlNode param2info = xmlDoc.CreateElement("param2info");
                                param2info.InnerText = "";// node_device_info.param2info.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param2info);

                                //for param1id_type
                                XmlNode param1id_type = xmlDoc.CreateElement("param1id_type");
                                param1id_type.InnerText = "";//node_device_info.param1_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param1id_type);

                                //for param2id_type
                                XmlNode param2id_type = xmlDoc.CreateElement("param2id_type");
                                param2id_type.InnerText = "";//node_device_info.param2_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
                                userNode.AppendChild(param2id_type);

                        
                        }


                    }//This one is for each node in the section

            }//close of if

            //--Now lets append the line information...
            XmlNode lineNode = xmlDoc.CreateElement("lines");
            // xmlDoc.AppendChild(rootNode);
            r_node.AppendChild(lineNode);

        
            if (lineInfoForSavingToXML.Count > 0)
            {
                //--if present ie value do following.

                foreach(var line in lineInfoForSavingToXML)
                {

                    XmlNode singleLine = xmlDoc.CreateElement("line");
                    lineNode.AppendChild(singleLine);


                    //for id
                    XmlNode lineID = xmlDoc.CreateElement("ID");
                    lineID.InnerText = line.ID;
                    singleLine.AppendChild(lineID);

                    //--for prev node id
                    XmlNode prevNodeID = xmlDoc.CreateElement("prevNodeID");
                    prevNodeID.InnerText = line.prevNodeId;
                    singleLine.AppendChild(prevNodeID);


                    //for next nodeid
                    XmlNode nextNodeID = xmlDoc.CreateElement("nextNodeID");
                    nextNodeID.InnerText = line.nextNodeId;
                    singleLine.AppendChild(nextNodeID);

                    //--for thickness
                    XmlNode thickness = xmlDoc.CreateElement("linethickness");
                    thickness.InnerText = line.lineThickness.ToString();
                    singleLine.AppendChild(thickness);


                    //--for series id
                    XmlNode seriesname = xmlDoc.CreateElement("seriesname");
                    seriesname.InnerText = line.lineSeriesID.Name.ToString();//This prints the line series name
                    singleLine.AppendChild(seriesname);


                    //--for color value
                    XmlNode linecolor = xmlDoc.CreateElement("linecolor");
                    linecolor.InnerText = ColorTranslator.ToHtml(line.lineColorValue).ToString();
                    singleLine.AppendChild(linecolor);



                }//close of foreach




            }







            //now saving the doucment 
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "xml file|*.xml";  //|Bitmap Image|*.bmp|Gif Image|*.gif";
            saveFileDialog1.Title = "Save an Image File";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string name = saveFileDialog1.FileName;
                xmlDoc.Save(name);
            }


        }

        private void saveSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //This will save the settings..

            //try
            //{
            //    //We need to get the configuration for load here 
                SaveConfiguration();
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
        }
        public class Tempdt
        {
             public string id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            public double yVal { get; set; }
            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public Color colorValue { get; set; }
            public string showItemText { get; set; }
            public string nodeSize { get; set; }

             //--THIS REPRESENTS THE DEVICE INFO
            //public string nodeID { get; set; } //--for identifying which point is selected..
            public string device_instance_id { get; set; }//--this is the values that represent the point in a chart
            public string ip { get; set; }
            public string param1id { get; set; }
            public string param2id { get; set; }
            public string param1info { get; set; }
            public string param2info { get; set; }
            public string param1_id_type { get; set; }
            public string param2_id_type { get; set; }
        }


        public List<Tempdt> nodeInfoFromXMLfile = new List<Tempdt>();

        public List<lineNodeDataType> lineInfoFromXMLfile = new List<lineNodeDataType>();

   
        public void loadXMLDoc()
        {
            nodeInfoFromXMLfile.Clear();
            lineInfoFromXMLfile.Clear();
            OpenFileDialog saveFileDialog1 = new OpenFileDialog();
            saveFileDialog1.Filter = "xml file|*.xml";  //|Bitmap Image|*.bmp|Gif Image|*.gif";
            saveFileDialog1.Title = "Load an Image File";
            string path = null;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                path = saveFileDialog1.FileName;
                //xmlDoc.Save(name);
            }

            if (path == "")
            {
                return;
            }


            //now lets read the data from the file
            XmlDocument xmlDoc = new XmlDocument();
            try { 
            xmlDoc.Load(path);
            }catch(Exception ex)
            {
                MessageBox.Show("File could not be loaded : "+ex.Message);
                return;
            }

            XmlNodeList identifierList = xmlDoc.SelectNodes("RootNode/identifier");

            //because if the node is empty it returns null
            if(identifierList == null)
            {
                MessageBox.Show("This file can not be loaded.");
                return;


            }
            else
            {
                if (identifierList.Count > 0)
                {
                    string identifierSignature = identifierList[0].InnerText;

                    if(identifierSignature == "")
                    {
                        MessageBox.Show("File contains is edited outside,can not load");
                        return;

                    }
                    else if(identifierSignature == "MainForm")
                    {
                        MessageBox.Show("This file belongs to offline mode Load in main section");
                        return;
                    }
                    else if(identifierSignature == "AirHandler")
                    {
                        MessageBox.Show("This file belongs to air handler. Please load in air handler section");
                        return;
                    }
                  
                }
            }


            XmlNodeList xnList = xmlDoc.SelectNodes("RootNode/nodes/node");
            foreach (XmlNode xn in xnList)
            {

                string nodeID = xn["nodeID"].InnerText; 
                string name = xn["name"].InnerText;
                string label = xn["label"].InnerText;
                string source = xn["source"].InnerText;
                string color = xn["color"].InnerText;
                string xvalue = xn["xvalue"].InnerText;
                string yvalue = xn["yvalue"].InnerText;
                string showTextItem = xn["showTextItem"].InnerText;
                string nodeSize = xn["nodesize"].InnerText;
                string deviceInstanceVal = xn["deviceInstance"].InnerText;
                string ipVal = xn["ip"].InnerText;
                string param1idVal = xn["param1id"].InnerText;
                string param2idVal = xn["param2id"].InnerText;
                string param1infoVal = xn["param1info"].InnerText;
                string param2infoVal = xn["param2info"].InnerText;
                string param1typeVal = xn["param1id_type"].InnerText;
                string param2typeVal = xn["param2id_type"].InnerText;

                //now lets add these values to list
                nodeInfoFromXMLfile.Add(new Tempdt
                {
                    id = nodeID,
                    name = name,
                    label = label,
                    source = source,
                    colorValue = ColorTranslator.FromHtml(color),
                    xVal = double.Parse(xvalue),
                    yVal = double.Parse(yvalue),
                    showItemText = showTextItem ,
                      nodeSize = nodeSize,
                    device_instance_id = deviceInstanceVal,
                    ip = ipVal,
                    param1id = param1idVal,
                    param2id = param2idVal,
                    param1info = param1infoVal,
                    param2info = param2infoVal,
                    param1_id_type = param1typeVal,
                    param2_id_type = param2typeVal
                });
            }//close of foreach


            //--loading the line info from the doc
            XmlNodeList xlList = xmlDoc.SelectNodes("RootNode/lines/line");

            foreach (XmlNode xn in xlList)
            {
                string idVal = xn["ID"].InnerText;
                string prevNodeIDVal = xn["prevNodeID"].InnerText;
                string nextNodeIDVal = xn["nextNodeID"].InnerText;
                string lineThicknessVal = xn["linethickness"].InnerText;
                string serieNameIDval = xn["seriesname"].InnerText;
                string linecolor = xn["linecolor"].InnerText;               
                //now lets add these values to list
                lineInfoFromXMLfile.Add(new lineNodeDataType
                {                   
                    ID = idVal,
                    prevNodeId = prevNodeIDVal,
                    nextNodeId = nextNodeIDVal,
                    lineThickness  =int.Parse(lineThicknessVal),
                    lineSeriesID = new Series( serieNameIDval),
                    lineColorValue = ColorTranslator.FromHtml(linecolor)
                });
            }//close of foreach
           // MessageBox.Show("count node values " + nodeInfoFromXMLfile.Count + ",line count" + lineInfoFromXMLfile.Count);
            //--now since both value is added we need to insert these values to db now
            //-- and reload them
            /*
            Task : 
            0.Replace the previous id with new ids so there is no conflict in ids if present in db.
              0.1 also change the series id so that there is no conflict between them        
            1.insert value
            2.Relaod value
            */

            //FIRST change the ids in both node and line section
           //================================start of the id conversion====================================// 
            foreach (var nodex in nodeInfoFromXMLfile)
            {
                //get new id ,
                //identify old id 
                
                //replace it first and then replace in line as well
                string newID = GetGUID();
                string oldID = nodex.id;
               // MessageBox.Show("old id=" + oldID + "new id = " + newID);
                //replace node id
                nodex.id = newID;
                //for line section replace with new id             
               foreach(var line in lineInfoFromXMLfile)
                {
                    //we need to replace it with the new id
                    if (oldID == line.prevNodeId)
                    {
                        line.prevNodeId = newID;//setting with the new node id
                    // MessageBox.Show("prev node id one match, prevnode id = " + line.prevNodeId+",oldid ="+oldID+",new id= "+newID);
                    }
                    if(oldID == line.nextNodeId)
                    {
                        line.nextNodeId = newID;
                       // MessageBox.Show("next nodeid one match,nex nodeid =" + line.nextNodeId + ",oldid =" + oldID + ",new id= " + newID);
                    }
                }

            }



             //---This part is done to not conflict the xml file when loaded in same page. the node
             //will be duplicated.
            foreach (var line in lineInfoFromXMLfile)
            {    
                string ln= GetGUID();
                Series s = new Series(ln);
                line.lineSeriesID = s;//This change is done to not conflict when loaded in same page   
            }

           
            //==================================end of the id conversion================================//

            //===================================inserting into db=====================================//
            foreach (var node in nodeInfoFromXMLfile)
            {
                //we need to inset it to db but if the node id is already present then dont inset just load....
                InsertNodeInfoToDB(node.id, node.xVal, node.yVal, node.source, node.name, node.label, node.colorValue, node.showItemText,int.Parse(node.nodeSize), node.device_instance_id, node.ip, node.param1id, node.param2id, node.param1info, node.param2info, node.param1_id_type, node.param2_id_type);
            }

            foreach(var line in lineInfoFromXMLfile)
            {
                InsertLineInfoToDB(line.ID, line.prevNodeId, line.nextNodeId, line.lineColorValue, line.lineSeriesID, line.lineThickness);
            }
          //=====================================end of inserting in db=============================//
            //now lets load the data back again..
             //--helps in loading in redrawing the parts.
            LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs       
            ReDrawingLineAndNode();

            MessageBox.Show("Load success");
 

        }

        void ClearChart()
        {
            this.Invalidate();
            chart1.Invalidate();
            // chart1.Dispose();//--Releases all the resources used by the chart...
            plot_new_graph();
           // lb_title_display.Text = "";
            //--Reseting the menustrip values for new plotting....
            menuStripNodeLineInfoValues.Clear();
            menuStripNodeInfoValues.Clear();
            index = 0;
            incrementIndex = 0;
            insertNodeToolStripMenuItem.Enabled = true;/*insert node will be dissable with historical plot so reenabling it*/

        }


        public void SetNodeWithValuesXYCoord(string source, string name, string label, Color c1, string comboboxItemText1, double xvalue, double yvalue)
        {

            tbSource = source;
            tbName = name;
            tbLabel = label;
            colorValue = c1;
            comboboxItemText = comboboxItemText1;
            //lets do the processing 
            //lets count how many items were inserted
            countNumberOfPoints += 1;

            plot_on_graph_values_process_diagram(xvalue, yvalue);


        }


        private void loadSettingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //try
            //{
               loadXMLDoc();
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
        }

        //--Insert node to database....
        public void InsertNodeInfoToDB(string id, double xVal, double yVal, string source, string name, string label, Color colorValue, string showItemText,int nodeSizeValue,string deviceinstance,string deviceip,string param1id,string param2id,string param1info,string param2info,string param1type,string param2type)
        {
            //This is the name of the table that stores the node information...
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableName + "(chart_respective_nodeID,nodeID,xValue,yValue,source,name,label,colorValue,showTextItem,nodeSize) VALUES(@chartid,@id,@xVal,@yVal,@source,@name,@label,@colorVal,@text,@node_size_value)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chartDetailList[indexForWhichChartIsSelected].chart_respective_nodeID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.ExecuteNonQuery();
            }


            //If we have device we need to store the device info as well

            if(source == "Device")
            {

               // MessageBox.Show("param2 info" + device_param2_info_for_node);
                //Device is selected so insert devcie to corresponding able
                string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID,IP,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID,@IP,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                   
                    command.Parameters.AddWithValue("@id", id);
                    command.Parameters.AddWithValue("@instanceID", deviceinstance);
                    command.Parameters.AddWithValue("@IP", deviceip);
                    command.Parameters.AddWithValue("@param1", param1id);
                    command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param1info", param1info);
                    command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", param1type);
                    command.Parameters.AddWithValue("@param2_iden_type", param2type);

                    //MessageBox.Show("SQL = " + sql_string);
                    //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                    command.ExecuteNonQuery();
                }

            }


        }//--close of insertnodeinfotodb fxn

        public void UpdateNodeInfoToDB(string id, double xVal, double yVal, string source, string name, string label, Color colorValue, string showItemText, int nodeSizeValue)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();                
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, source=@source, name=@name, label=@label , colorValue=@colorVal, showTextItem=@text , nodeSize=@node_size_value  where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn

        public void InsertLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, Series lineSeriesVal,int linethickness)
        {
            //--Note for series the series.name property will be stored in databse and later it will be converted.
            string lineSeriesName = lineSeriesVal.Name;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();               
                string sql_string = "insert into " + tableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);                                                                      
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chartDetailList[indexForWhichChartIsSelected].chart_respective_lineID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@thicknessValue", linethickness);
                command.ExecuteNonQuery();
            }


        }


        /// <summary>
        /// This value is edited just now bbk305
        /// </summary>
        /// <param name="id"></param>
        /// <param name="prevNodeId"></param>
        /// <param name="nextNodeId"></param>
        /// <param name="lineColor"></param>
        /// <param name="lineSeriesVal"></param>
        /// <param name="lineThicknessValue"></param>
        public void UpdateLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, Series lineSeriesVal,int lineThicknessValue)
        {
            string lineSeriesName = lineSeriesVal.Name;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_"+selectedBuildingList[0].BuildingName+"_line_value";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set prevNodeId=@pn,nextNodeId=@nn,lineColorValue=@lc,lineSeriesId=@ls ,thickness = @linethickness where   lineID=@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@linethickness", lineThicknessValue);
                command.Parameters.AddWithValue("@id", id);
                command.ExecuteNonQuery();
            }


        }


        public void UpdateLineInfoToDBWithouSeriesID(string id, string prevNodeId, string nextNodeId, Color lineColor, int lineThicknessValue)
        {
           // string lineSeriesName = lineSeriesVal.Name;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set prevNodeId=@pn,nextNodeId=@nn,lineColorValue=@lc ,thickness = @linethickness where   lineID=@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                //command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@linethickness", lineThicknessValue);
                command.Parameters.AddWithValue("@id", id);
                command.ExecuteNonQuery();
            }


        }


        //--This is for the humidity and the enthalpy value base on the x and y coordinate
        public double enthalpyDataGridView;
    public double humDataGridValue;
        public void enthalpyHumidityCalculatorForXYvalue(double xVal, double yVal)
        {

            double phi = 0.00000;
            //double y_axis = yVal;
            //now for pg..
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();

            //--Copying the ref temp and humidity values..
            temperature_value = t;
            pg_value_from_txtfile = pg;

            double temperature = Math.Round(xVal);
            double corres_pg_value = 0.000000;
            for (int i = 0; i < temperature_value.Count; i++)
            {
                if (temperature == Double.Parse(temperature_value[i].ToString()))
                {
                    corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                    break;
                }
            }//close of for

            double patm = form1Object.AirPressureFromDB * 0.001;//101.325;//this is constant...
            double w = yVal;
            phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
            phi = phi * 100;//changing into percent..
                            //now display in label...
                            //lb_RH.Text = Math.Round(phi, 4).ToString();

            //now lets calculate the dew point...
            double humidity = phi;
            humDataGridValue = Math.Round(humidity, 4);//--lets make the humidity set the humidity...
            double temperature1 = xVal;
            double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
            //now lets print this value..
            //lb_DP.Text = Math.Round(TD, 4).ToString();

            //now lets move towards enthalpy...

            Patm = form1Object.AirPressureFromDB * 0.001; //1013;
            A = 6.116441;
            m = 7.591386;
            Tn = 240.7263;
            B = 621.9907;

            double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

            double X = B * Pws / (Patm - Pws);

            h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
            //now lets display this value ..
            // lb_enthalpy.Text = Math.Round(h, 4).ToString();
            enthalpyDataGridView = Math.Round(h, 4);



        }

       public double y_coord_value ;

        public double CalculateYFromXandHumidity(double xVal,double humidity)
        {

            double phi = 0.00000;
            //double y_axis = yVal;
            //now for pg..
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();

            temperature_value = t;
            pg_value_from_txtfile = pg;

            double temperature = (double)Math.Round((double)xVal);
            double corres_pg_value = 0.000000;
            for (int i = 0; i < temperature_value.Count; i++)
            {
                if (temperature == Double.Parse(temperature_value[i].ToString()))
                {
                    corres_pg_value = Double.Parse(pg_value_from_txtfile[i].ToString());

                    break;
                }
            }//close of for

            phi = humidity;

            double patm = form1Object.AirPressureFromDB * 0.001;//101.325;//this is constant...
                                                                // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                                                //double w1 = 622*phi*pg/(patm-phi*pg);
            double w = phi * 622 * corres_pg_value / (patm - phi* corres_pg_value);//This is the y value


            //now lets calculate the dew point...
            //double humidity = phi;
            y_coord_value = w;
            return y_coord_value;

        }


    }
}
