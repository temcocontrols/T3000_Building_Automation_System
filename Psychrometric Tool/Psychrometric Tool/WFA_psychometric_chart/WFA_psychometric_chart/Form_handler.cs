using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Xml;

namespace WFA_psychometric_chart
{
    public partial class Form_handler : Form
    {
        Form1_main form1Object;
        public Form_handler(Form1_main f)
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
        int indexOfPrevPointForLineMovement;

        int setItemSelectedID = 0;

        int oneTimeClick = 1;
        //int twoTimeClick = 0;
        //int incrementIndex = 0;
        int mouseClickAction = 0;

        int idSelected = 0;
        int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..

        bool arrowOn = false;



        //--All the series decleared here..
        Series series1 = new Series("My Series");

        //This series is for temporary line drawing for line movements...
        Series addDottedSeries = new Series("newSeries101");
        //--If series is present delete the previouse series


        //--id of the node selected, this is used when line is detached and for reconnecting we need to know which node we are connecting to.
        int idOfNodeSelected = 0;//--initially it represents nothing...


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
        private void Form_handler_Load(object sender, EventArgs e)
        {
            //MessageBox.Show("Error ");
            DataGridView_Show_Data();
            dataGridView1.Rows.Add();

            chart1.Series.Add(series1);
            chart1.Series.Add(series1xx);
            chart1.Series.Add(seriesLineIndicator);
            //--Lets add some data 
            add_t_pg();

            //--Load the chart 
            plot_new_graph();

            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);

            //--For datagridview1 editing option.
            // readOnlyModeSetUp();

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

            //now we have the data lets do some ploting...
            //===========NOTE WE HAVE PRESSURE IN Pa but this chart users it in terms of kpa===//
            /*
            Later we need to user it in terms of HPa (heteropascal) in formula so we need conversion
            */
            double pressureConverted =form1Object.AirPressureFromDB * 0.001;//now in terms of kpa
                                                                 // MessageBox.Show("pressure(kpa) = " + pressureConverted);
            lb_pressure_display.Text = "Pressure : " + Math.Round(pressureConverted, 2) + " KPa";//in terms of kpa
            double patm = pressureConverted;//101.235;//constant..we will make it take as input later...   in 

            //double patm = 101.235;//constant..we will make it take as input later...
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
                    chart1.Series["Line_b" + hval].Points[1].Label = WFA_psychometric_chart.Properties.Resources.Enthalpy_kj_kg_dry_air;
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
            PullDataFromDB();

//            MessageBox.Show("datapulled ,count =  " + listForDataFromDB.Count);

            //This fills the datagridview
            fillDataGridView();


        }

        private class listDataType
        {
            public int id { get; set; }
            public string name { get; set; }
            public string node_table { get; set; }
            public string line_table { get; set; }
        }
        //--Lets have a list to add the data to..
        List<listDataType> listForDataFromDB = new List<listDataType>();
        public void PullDataFromDB()
        {

            //Reading data from the database

            //--Clearing the list data..
            listForDataFromDB.Clear();

            //--Database detials           
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from tbl_air_handler_details";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                //SqlCommandBuilder commandBuilder = new SqlCommandBuilder(dataAdapter);
                //DataSet ds = new DataSet();
                //dataAdapter.Fill(ds);
                //dataGridView1.DataSource = ds.Tables[0];


                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    listForDataFromDB.Add(new listDataType
                    {
                        id = int.Parse(reader["id"].ToString()),
                        name = reader["name"].ToString(),
                        node_table = reader["node_table"].ToString(),
                        line_table = reader["line_table"].ToString(),
                    });
                }
            }//close of using..



        } //close of the function pulldatafromdb

        private void fillDataGridView()
        {
            if (listForDataFromDB.Count > 0)
            {
                for (int i = 0; i < listForDataFromDB.Count; i++)
                {
                    dataGridView1.Rows.Add();
                    dataGridView1.Rows[i].Cells["ID"].Value = listForDataFromDB[i].id;//
                    dataGridView1.Rows[i].Cells["Name"].Value = listForDataFromDB[i].name;

                }
            }//--if close

            lb_numberOfHandler.Text = listForDataFromDB.Count.ToString();
        }//--close fo filldatagridview




        private void NodeSelectionAndRelease(MouseEventArgs e)
        {//this is used to select the partciular id values..


            if (readyForMouseClick == 1)
            {

                if (oneTimeClick == 1)
                {

                    //this is for dissabling insert node when a node is selected
                  CMSinsertNode.Enabled = false;

                    setItemSelectedID = idSelected;
                    //  MessageBox.Show("Node grabbed - id=" + setItemSelectedID);
                    Cursor = Cursors.Cross;
                    oneTimeClick = 0;
                    //MessageBox.Show("one time click");
                    mouseClickAction = 1;
                }

                else
                {

                    //this is for re-enabling insert node when a node is selected
                    //insertNodeToolStripMenuItem.Enabled = true;
                    CMSinsertNode.Enabled = true;
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
                        menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
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


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {
                                incrementIndex++;

                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);

                            }

                        }

                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..





                    }
                    else if (Control.ModifierKeys == Keys.Shift)
                    {
                        //--This ctrl key is for moving along the y-  axis...

                        //MessageBox.Show("shift  is pressed for y  axis constant");

                        menuStripNodeInfoValues[idSelected].xVal = xAxis1;
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


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {
                                incrementIndex++;

                                // ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);


                            }

                        }

                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..




                    }
                    else
                    {

                        menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        menuStripNodeInfoValues[idSelected].yVal = yAxis1;

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


                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        if (menuStripNodeLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                            {

                                incrementIndex++;

                                // ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);


                            }

                        }

                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..
                    }//closing of key else part
                }//closing of second click



            }//closing of else block





        }




        public void ResettingLines()
        {

            menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected;

            //--Lets update the data here...
            if(flagForInsertOrUpdateDataToDB == 1)
            {
                //Testing...
                //MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                //--This is the update function for line values..
                UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, idOfNodeSelected, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);
            }


        }

        private void chart1_MouseClick(object sender, MouseEventArgs e)
        {

            if (flagForDisconnectClick == 1)
            {
                ////--Creating temporary line..
                //Series addDottedSeries = new Series();
                //addDottedSeries.ChartType = SeriesChartType.Line;
                //addDottedSeries.Color = Color.Black;


                //addDottedSeries.Points.Add(menust)

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

                    //This for closing the insert node while the disconnect is selected
                    insertNodeToolStripMenuItem.Enabled = true;

                }


            }
            else {

                //--This function is used for nodeSelection and releasing node to desired place 
                //--This gets triggered based on mouse select and release..

                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                    {
                        UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText);
                    }
                }


                NodeSelectionAndRelease(e);

            }

        }


        public void ReDrawingLineAndNode()
        {
            index = 0;//Reset the index as fresh data is going to be inserted
            //--This is for replotting all the things again...
            series1.Points.Clear();
            for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
            {
                //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
            }
            //--this is redraw functionality
            //foreach(var values in menuStripNodeInfoValues)
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


                ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);
                incrementIndex++;
                index++;
            }
            //--resetting incrementIndex
            incrementIndex = 0;
            if (menuStripNodeLineInfoValues.Count > 0)
            {

                for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                {
                    incrementIndex++;

                    ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);
                 
                }

            }

            chart1.Invalidate();
            incrementIndex = 0;//reset the values again..

        }//--Close of the actual function....



        int incrementIndex = 0;//--Defining the index

        //--this class is used for storing temporary the values of id xCoord,yCoord,source,name,label,color 
        //--so that a line could be plotted in it and we can do some processing as well 

        //--This information is required for node and node only
        public class TempDataType
        {
            public int id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            public double yVal { get; set; }
            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public Color colorValue { get; set; }
            public string showItemText { get; set; }
        }
        public List<TempDataType> menuStripNodeInfoValues = new List<TempDataType>();
        //--This one right here is for editing the lines...
        public class lineNodeDataType
        {
            //--Line ID
            public int ID { get; set; }
            public int prevNodeId { get; set; }
            public int nextNodeId { get; set; }
            public Color lineColorValue { get; set; }
            public Series lineSeriesID { get; set; }

        }

        public List<lineNodeDataType> menuStripNodeLineInfoValues = new List<lineNodeDataType>();

        public void ReDrawPoints(Series s1, double x, double y, Color c, string source1, string name1, string label1x, string labelValueText)
        {

            //s1.ChartType = SeriesChartType.Point;
            string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x;
            s1.ChartType = SeriesChartType.Point;
            s1.MarkerSize = 20;
            s1.MarkerStyle = MarkerStyle.Circle;
            s1.Points.AddXY(x, y);
            chart1.Series["My Series"].Points[incrementIndex].ToolTip = s;
            chart1.Series["My Series"].Points[incrementIndex].Label = labelValueText;
            s1.Points[incrementIndex].Color = c;
        }




        //--Redraw line function
        public void ReDrawLines(double id, int prevNodeID, int nextNodeID, Series lineSeriesID, Color c)
        {

            if (incrementIndex > 0)
            {

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
                newLineSeries.BorderWidth = 3;
                //newSeries.ToolTip = 
                newLineSeries.Color = c;
                //--this sets the initial values of humidity and enthalpy
                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[prevNodeID].xVal, (double)menuStripNodeInfoValues[prevNodeID].yVal);
                startHumidity1 = Math.Round(humidityCalculated, 2);
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[nextNodeID].xVal, (double)menuStripNodeInfoValues[nextNodeID].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);

                // MessageBox.Show("Start hum" + startHumidity1 + " end enth" + endEnthalpy1);
                //MessageBox.Show("menustripinfovalues[prevNodeID].xVal=" + menuStripNodeInfoValues[prevNodeID].xVal + "menuStripNodeInfoValues[nextNodeID].yVal=" + menuStripNodeInfoValues[nextNodeID].yVal + "menuStripNodeInfoValues[nextNodeID].xVal = "+ menuStripNodeInfoValues[nextNodeID].xVal + " menuStripNodeInfoValues[nextNodeID].yVal" + menuStripNodeInfoValues[nextNodeID].yVal);

                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripNodeInfoValues[prevNodeID].name + " to " + menuStripNodeInfoValues[nextNodeID].name;

                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + WFA_psychometric_chart.Properties.Resources._start_end + "Temp         :" + Math.Round(menuStripNodeInfoValues[prevNodeID].xVal, 2) + "               " + Math.Round(menuStripNodeInfoValues[nextNodeID].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;

                newLineSeries.ToolTip = tooltipString;
                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[prevNodeID].xVal, menuStripNodeInfoValues[prevNodeID].yVal));
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[nextNodeID].xVal, menuStripNodeInfoValues[nextNodeID].yVal));


                chart1.Series.Add(newLineSeries);
            }




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

            double patm = 101.325;//this is constant...
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

            double Patm = 1013;
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

            Form_point_add form_point_input = new Form_point_add(this);
            form_point_input.Show();
        }

        int yCoord = 0;
        double humidityValue; //--This is universal used to calculate humidityValue
        double temperatureValue; //--This is universal used to calculate temperatureValue
        //--These are the property of node...
        string tbSource;

        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {

            //hittestresult hit = chart1.hittest(e.x, e.y);
            //text = "element : " + hit.chartelementtype;
            //datapoint dp = null;
            //if (hit.chartelementtype == chartelementtype.datapoint)
            //{
            //    //--hittest result...
            //    dp = hit.series.points[hit.pointindex];
            //    if (dp != null)
            //    {
            //        text += " point #" + hit.pointindex + " x-value:" + dp.xvalue + " y-value: " + dp.yvalues[0];
            //        messagebox.show("text = " + text);
            //    }
            //}


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

                    double patm = 101.325;//this is constant...
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

                    Patm = 1013;
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

                    //add here 
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

                                idSelected = menuStripNodeInfoValues[i].id;
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
                            menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                            // label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                // chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);//--removing line series that joins node..
                                menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();

                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripNodeInfoValues)
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


                                ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);

                                //Updating values in database
                                if (flagForInsertOrUpdateDataToDB == 1)
                                {
                                   // UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText);
                                }


                                incrementIndex++;
                                //--Every time it redraws the point we need to update to database the node values
                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            if (menuStripNodeLineInfoValues.Count > 0)
                            {


                                for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                {
                                    incrementIndex++;

                                    //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                    ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);

                                }

                            }

                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..


                        }
                        else if (Control.ModifierKeys == Keys.Shift)
                        {
                            //--This ctrl key is for moving along the y-  axis...

                            //--THis function basically evolve when the shift key is pressed and mouse move.
                            // MessageBox.Show("shift  is pressed for y  axis constant");

                            menuStripNodeInfoValues[idSelected].xVal = xAxis1;
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


                                ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);

                                //Updating values in database...
                                if (flagForInsertOrUpdateDataToDB == 1)
                                {
                                   // UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText);
                                }

                                incrementIndex++;

                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            if (menuStripNodeLineInfoValues.Count > 0)
                            {
                                for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                {
                                    incrementIndex++;

                                    ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);

                                }

                            }

                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..





                        }
                        else
                        {

                            //--Show indicator
                            ////--Lets clear the indicator point first.
                            //seriesLineIndicator.Points.Clear();

                            menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                            menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                            //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                                menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripNodeInfoValues)
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


                                ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);

                                //Updating values in database...
                                if (flagForInsertOrUpdateDataToDB == 1)
                                {
                                    //UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText);
                                }


                                incrementIndex++;

                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            if (menuStripNodeLineInfoValues.Count > 0)
                            {
                                // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");

                                for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                                {
                                    // MessageBox.Show("MENUSTIRP NODE LINE INFO VALUE");
                                    incrementIndex++;

                                    //ReDrawLines(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue);
                                    ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);

                                }
                            }


                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..




                        }//closing of key else part
                    }

                    //if ends here
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
            addDottedSeries.Points.AddXY(menuStripNodeInfoValues[indexOfPrevPointForLineMovement].xVal, menuStripNodeInfoValues[indexOfPrevPointForLineMovement].yVal);
            addDottedSeries.Points.AddXY(currentXAxis, currentYAxis);

        }


        Color storeColor;
        int flagForColor = 0;

        //--Lets store the series for futher processing...
        Series tempSeries;
        int indexOfLineInTheList = 0;
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
                                indexOfPrevPointForLineMovement = menuStripNodeLineInfoValues[i].prevNodeId;//This gets the previous node id value...
                                indexOfLineInTheList = i;//This index is used for updating the line when the line is moved...
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


        //--This is the cell end edit option
        //private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        //{
          

        //}

      public void AddOrUpdateNewRecord(string name, DataGridViewCellEventArgs e)
        {
            /*
            //--We need to count the index of the current datagridview if the row index 
                is greater then the data in the list of data 
            
            */
            if(e.RowIndex+1 > listForDataFromDB.Count)
            {
                //insert the data...
                string nameValue = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                insertNewDataFromDGV(nameValue);

            }
            else
            {
                //--Update the data

                int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
                string nameValue = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                UpdateNewDataFromDGV(nameValue, id);

            }
        }

        public void insertNewDataFromDGV(string name)
        {
            string newTableNodeName = "tbl_" + name + "_node";
            string newTableLineName = "tbl_" + name + "_line";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into tbl_air_handler_details(name,node_table,line_table) VALUES(@name_value,@node_table_value,@line_table_value)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@name_value", name);
                command.Parameters.AddWithValue("@node_table_value", newTableNodeName);
                command.Parameters.AddWithValue("@line_table_value", newTableLineName);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();


                //--We also need to create table for particular data added..
                string sql3 = "create table "+newTableNodeName+ "(count INTEGER PRIMARY KEY AUTOINCREMENT ,id INTEGER,xValue varchar(255),yValue varchar(255),source varchar(255),name varchar(255), label varchar(255),colorValue varchar(255),showItemText varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();


                string sql4 = "create table " + newTableLineName + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,id INTEGER,prevNodeId varchar(255),nextNodeId varchar(255),lineColorValue varchar(255),lineSeriesId varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

            }


        }

        public void UpdateNewDataFromDGV(string name,int id)
        {
            string newTableNodeName = "tbl_" + name + "_node";
            string newTableLineName = "tbl_" + name + "_line";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--Before updating table we need to find the previous tables and the update its correspoing 
                //name as well..
                string updateTableNodeName = null;
                string updateTableLineName = null;
                string sql = "select * from tbl_air_handler_details where id = @id_val";
                SQLiteCommand cmd1 = new SQLiteCommand(sql, connection);
                cmd1.CommandType = CommandType.Text;
                cmd1.Parameters.AddWithValue("@id_val", id);
                SQLiteDataReader reader = null;
                reader = cmd1.ExecuteReader();
                while (reader.Read())
                {
                    updateTableNodeName = reader["node_table"].ToString();
                    updateTableLineName = reader["line_table"].ToString();
                }
              //  MessageBox.Show("node table" + updateTableNodeName);
                //now lets alter table name 
                string alterTable1 = "ALTER TABLE " + updateTableNodeName + " RENAME TO " + newTableNodeName + "";

                SQLiteCommand c1 = new SQLiteCommand(alterTable1, connection);
                c1.ExecuteNonQuery();

                string alterTable2 = "ALTER TABLE " + updateTableLineName + " RENAME TO " + newTableLineName + "";
                SQLiteCommand c2 = new SQLiteCommand(alterTable2, connection);
                c2.ExecuteNonQuery();


                //SQLiteDataReader reader = null;
                string sql_string = "update tbl_air_handler_details set  name=@name_value,node_table=@node_table_value,line_table=@line_table_value   where id = @id_provided;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@name_value", name);
                command.Parameters.AddWithValue("@node_table_value", newTableNodeName);
                command.Parameters.AddWithValue("@line_table_value", newTableLineName);
                command.Parameters.AddWithValue("@id_provided", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();


                //also update the table names...
                //First identify the 


            }


        }
        string beginEditText = "";
        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            //--This start the editing of the cells
            //Here we save a current value of cell to some variable, that later we can compare with a new value
            //For example using of dgv.Tag property
           // MessageBox.Show("value  = " + dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString());
            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                this.dataGridView1.Tag = this.dataGridView1.CurrentCell.Value;
                if(dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null) { 
                beginEditText = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                //Or cast sender to DataGridView variable-> than this handler can be used in another datagridview
                }
            }

        }

        private void dataGridView1_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
        {
            ////--This section helps in validation..   
            //not required now.

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
                            if (beginEditText == "")
                            {
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
                       // MessageBox.Show(" cell end edit ,name = "+name);
                        //--we need to check that the table name enter doesnot matches previous values
            
                        if(name != beginEditText) {      
                        for(int i= 0; i < listForDataFromDB.Count; i++)
                        {
                            if(name == listForDataFromDB[i].name)
                            {
                                MessageBox.Show("Handler can not have same name");
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
                        PullDataFromDB();

                        //LETS FILL THE DATA
                        fillDataGridView();


                        dataGridView1.Rows.Add();

                        }
                        else
                        {
                            MessageBox.Show("No change in name");
                        }

                    }
                    else
                    {
                        MessageBox.Show("You can only have letters, numbers and underscores");
                        dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = string.Empty;

                    }


                }

                 } catch(Exception ex){
                MessageBox.Show("Edit and then press tab to commit changes");
                dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;

            }

}
        /// <summary>
        /// data gridveiw cell click is used when the cell is selected and click...
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        int flagSinglCellClick = 0;

        public int idSelectedOfChart = 0;
        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //--Showing the data on cell selected...
           // MessageBox.Show("CELL SELECT " );
           //When dgv is click it clicks twice this if is written to stop those twice click.
           if(flagSinglCellClick ==1)
            {
                flagSinglCellClick = 0;
                return;
            }
            if ((e.RowIndex >= 0) && (e.ColumnIndex ==1)){

                if(dataGridView1.Rows[e.RowIndex].Cells[1].Value != null) { 
               // MessageBox.Show("CELL SELECT ROW= " + e.RowIndex+",columns = "+e.ColumnIndex);
                //The row is selected ..
                for(int i = 0; i < listForDataFromDB.Count; i++)
                {
                    //--Checking for evey item
                    if(dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString() == listForDataFromDB[i].name)
                    {
                        //if match found load
                        //data_load();
                        RefreshGraph();
                            idOfNodeSelected = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
                            LoadNodeAndLineFromDB(idOfNodeSelected);
                        flagForInsertOrUpdateDataToDB = 1;
                        ReDrawingLineAndNode();
                        flagSinglCellClick = 1;
                            //--Now lets load the vaiables in dgv2
                            loadVariableInDGV2();
                        break;

                    }
                    else
                    {
                        flagForInsertOrUpdateDataToDB = 0;
                        RefreshGraph();
                    }
                }


            }
            else
            {
                flagForInsertOrUpdateDataToDB = 0;
            }
            }//close of != null if
        }//close of the datagridview_cellClick



        public void loadVariableInDGV2()
        {
            int normalCount = 1;
            dataGridView2.Rows.Clear();
            //--This is about the node info
            if(menuStripNodeInfoValues.Count > 0)
            {
                //we need to load the info
                for(int i = 0; i < menuStripNodeInfoValues.Count; i++)
                {
                    
                    dataGridView2.Rows.Add();
                    //   dataGridView1.Rows.Add();
                    string stringName = menuStripNodeInfoValues[i].id.ToString();
                    if(menuStripNodeInfoValues[i].showItemText == "Label")
                    {
                        stringName += ", " + menuStripNodeInfoValues[i].label;
                    }else if(menuStripNodeInfoValues[i].showItemText == "Name")
                    {
                        stringName += ", " + menuStripNodeInfoValues[i].name;
                    }else if(menuStripNodeInfoValues[i].showItemText == "Source")
                    {
                        stringName += ", " + menuStripNodeInfoValues[i].source;
                    }

                    dataGridView2.Rows[i].Cells[0].Value = normalCount;
                    dataGridView2.Rows[i].Cells[1].Value = stringName;
                    normalCount++;

                }//--Close of for
            }//Close of if
            //This is for printing node info values
            if (menuStripNodeLineInfoValues.Count > 0)
            {
                for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)
                {
                    dataGridView2.Rows.Add();
                   
                    string stringName = menuStripNodeLineInfoValues[i].ID.ToString();
                    if (menuStripNodeLineInfoValues[i].lineSeriesID != null)
                    {
                        stringName += ", " + menuStripNodeLineInfoValues[i].lineSeriesID;
                    }

                    dataGridView2.Rows[normalCount-1].Cells[0].Value = normalCount;
                    dataGridView2.Rows[normalCount-1].Cells[1].Value = stringName;
                    normalCount++;

                }//--Close of for

            }
        }



        public void RefreshGraph()
        {
            this.Invalidate();
            chart1.Invalidate();
            // chart1.Dispose();//--Releases all the resources used by the chart...
            plot_new_graph();

            //--Reseting the menustrip values for new plotting....
            menuStripNodeLineInfoValues.Clear();
            menuStripNodeInfoValues.Clear();
            index = 0;
            incrementIndex = 0;
        }

        int countDB_Item = 0;

        public int CountingDB_Item(string db_name)
        {
            string stmt = "SELECT COUNT(*) FROM  "+db_name+"";
            

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
                        countDB_Item =  int.Parse(reader[0].ToString());
                       // currentLineTableFromDB = reader["line_table"].ToString();
                    }

                }
            }
            return countDB_Item;
        }


        private void LoadNodeAndLineFromDB( int idOfChart)
        {
            //Based on this row index we need to update the values and redraw lines..

            // listForDataFromDB.Clear();//Lets clear the node...

            //Lets identify the node
            int id = idOfChart;//int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from tbl_air_handler_details where id = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", id);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    currentNodeTableFromDB = reader["node_table"].ToString();
                    currentLineTableFromDB = reader["line_table"].ToString();
                }
            }//close of using..


            if (CountingDB_Item(currentNodeTableFromDB) > 0) { 

            //--=============================================start of the if statement===================//
            //IF no data dont precess futher 

            //Since it it done.. 
            //nwo pulling the data of different values...

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();



                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + currentNodeTableFromDB + " ";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);


                //--Reset the context menu stip first..
                menuStripNodeInfoValues.Clear();

                int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    menuStripNodeInfoValues.Add(new TempDataType
                    {
                        id = int.Parse(reader["id"].ToString()),
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        source = reader["source"].ToString(),
                        name = reader["name"].ToString(),
                        label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        showItemText = reader["showItemText"].ToString()


                    });

                }

                //--Resetting the index value...

                if(menuStripNodeInfoValues.Count > 0) { 
                count = menuStripNodeInfoValues.Count ;//--This is used for udpdating the index values..
                }
                else
                {
                    count = 0;
                }
                //--Resetting the actual index value
                index = count;
          
                //--Adding data form the line node values...
                SQLiteDataReader reader2x = null;
                string queryString2x = "SELECT *  from  " + currentLineTableFromDB + " ";
                    //--Testing..
                 //   MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

                SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);


                //--Reset the context menu stip first..
                menuStripNodeLineInfoValues.Clear();

                //int count2 = 0;
                reader2x = command2x.ExecuteReader();
                while (reader2x.Read())
                {
                    menuStripNodeLineInfoValues.Add(new lineNodeDataType
                    {
                        ID = int.Parse(reader2x["id"].ToString()),
                        prevNodeId = int.Parse(reader2x["prevNodeId"].ToString()),
                        nextNodeId = int.Parse(reader2x["nextNodeId"].ToString()),
                        lineColorValue = ColorTranslator.FromHtml(reader2x["lineColorValue"].ToString()),
                        lineSeriesID = new Series(reader2x["lineSeriesId"].ToString()),                        
                    });

                }
                 
            }//close of using..

            }//closing here...
            //--==================================Close of the if stat=====================================//                 
        }
           
        string tbName;
        string tbLabel;
        Color colorValue;

        //private void dataGridView1_SelectionChanged(object sender, EventArgs e)
        //{
        //    MessageBox.Show("SEL CHANGE ");
        //}

        //private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        //{
        //    MessageBox.Show("cell content click");
        //}

        string comboboxItemText;

        int indexSelectedForDeletion = 0;

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if(e.RowIndex >= 0 && e.ColumnIndex == 0)
                {
                    if(dataGridView1.Rows[e.RowIndex].Cells[0].Value != null) { 
                    Point p = new Point(e.Location.X, e.Location.Y);
                        indexSelectedForDeletion = e.RowIndex;
                    contextMenuStrip1.Show(dataGridView1,p);

                    }
                }


            }
        }

        private void deleteHandlerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This is the delete operation for the handler.....
            /*
            Steps: 1. delete the row  of the table using the id portion.
            2. Delete the corresponding tables related to the row.             
            */
            if (indexSelectedForDeletion > -1)//Header is selected..
            {
                int selectedItemIndex = int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

                //we need to find the corresponding tables for deletion.

                int id = selectedItemIndex;

                string table1=null, table2=null;
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                string connString = @"Data Source=" + databaseFile + ";Version=3;";

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    string queryString = "SELECT *  from tbl_air_handler_details where id = @id_value";


                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    command.Parameters.AddWithValue("@id_value", id);
                    //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        table1 = reader["node_table"].ToString();
                        table2 = reader["line_table"].ToString();
                    }

                }//Close of using 

                //Deleting corresponding table 
                deleteDatabaseTable(table1);
                deleteDatabaseTable(table2);

                //Deleting rows..

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                  //  SQLiteDataReader reader = null;
                    string queryString = "delete from tbl_air_handler_details where id = @id_value";


                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    command.Parameters.AddWithValue("@id_value", id);
                    //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                    command.ExecuteNonQuery();

                }//Close of using 


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

            dataGridView2.Rows.Clear();
            MessageBox.Show("Handler deleted successfully !");

        }

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
                string queryString = "Drop  table  "+tableName+"";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.ExecuteNonQuery();
            }
        }

        private void statePointsToolStripMenuItem_Click(object sender, EventArgs e)
        {
             //This pop up the node information for editing .
        }

        private void saveSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This section helps to save the settings.

            SaveConfiguration();

        }


        public void SaveConfiguration()
        {

           // LoadNode_LineAndDeviceInfoFromDB(indexOfChartSelected);


            XmlDocument xmlDoc = new XmlDocument();
            //XmlWriter xw = new XmlWriter();
            //lets create an xml document using a string in xml formate

            XmlNode r_node = xmlDoc.CreateElement("RootNode");

            xmlDoc.AppendChild(r_node);

            XmlNode identifier = xmlDoc.CreateElement("identifier");
            identifier.InnerText = "AirHandler";

            r_node.AppendChild(identifier);


            XmlNode rootNode = xmlDoc.CreateElement("nodes");
            // xmlDoc.AppendChild(rootNode);
            r_node.AppendChild(rootNode);
            string s = null;
            //loading the string ...

            if (menuStripNodeInfoValues.Count > 0)
            {

                foreach (var node in menuStripNodeInfoValues)
                {
                   
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
                        //XmlNode nodesize = xmlDoc.CreateElement("nodesize");
                        //nodesize.InnerText = node.marker_Size.ToString(); //'"' + node.showItemText.ToString() + '"';
                        //userNode.AppendChild(nodesize);


                        //--WE ALSO need to add the user information.

                      
                         
                 

                }//This one is foreach node in the section

            }//close of if

            //--Now lets append the line information...
            XmlNode lineNode = xmlDoc.CreateElement("lines");
            // xmlDoc.AppendChild(rootNode);
            r_node.AppendChild(lineNode);


            if (menuStripNodeLineInfoValues.Count > 0)
            {
                //--if present ie value do following.

                foreach (var line in menuStripNodeLineInfoValues)
                {

                    XmlNode singleLine = xmlDoc.CreateElement("line");
                    lineNode.AppendChild(singleLine);


                    //for id
                    XmlNode lineID = xmlDoc.CreateElement("ID");
                    lineID.InnerText = line.ID.ToString();
                    singleLine.AppendChild(lineID);

                    //--for prev node id
                    XmlNode prevNodeID = xmlDoc.CreateElement("prevNodeID");
                    prevNodeID.InnerText = line.prevNodeId.ToString();
                    singleLine.AppendChild(prevNodeID);


                    //for next nodeid
                    XmlNode nextNodeID = xmlDoc.CreateElement("nextNodeID");
                    nextNodeID.InnerText = line.nextNodeId.ToString();
                    singleLine.AppendChild(nextNodeID);

                    ////--for thickness
                    //XmlNode thickness = xmlDoc.CreateElement("linethickness");
                    //thickness.InnerText = line.lineThickness.ToString();
                    //singleLine.AppendChild(thickness);


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



        private void loadSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This section helps to load the setting for handler
            //try
            //{
            loadXMLDoc();
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

        }
        /// <summary>
        /// This load the file and inserts the data in database as well 
        /// </summary>


        public List<TempDataType> nodeInformationFromXML = new List<TempDataType>();
        //--This one right here is for editing the lines...
   
        public List<lineNodeDataType> lineInformationFromXML = new List<lineNodeDataType>();


        public void loadXMLDoc()
        {
            // nodeInfoFromXMLfile.Clear();
            // lineInfoFromXMLfile.Clear();
            nodeInformationFromXML.Clear();
            lineInformationFromXML.Clear();

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
            try
            {
                xmlDoc.Load(path);
            }
            catch (Exception ex)
            {
                MessageBox.Show("File could not be loaded : " + ex.Message);
                return;
            }

            XmlNodeList identifierList = xmlDoc.SelectNodes("RootNode/identifier");

            //because if the node is empty it returns null
            if (identifierList == null)
            {
                MessageBox.Show("This file can not be loaded.");
                return;


            }
            else
            {
                if (identifierList.Count > 0)
                {
                    string identifierSignature = identifierList[0].InnerText;

                    if (identifierSignature == "")
                    {
                        MessageBox.Show("File contains is edited outside,can not load");
                        return;

                    }
                    else if (identifierSignature == "MainForm")
                    {
                        MessageBox.Show("This file belongs to offline mode Load in main section");
                        return;
                    }
                    else if (identifierSignature == "BuildingSetting")
                    {
                        MessageBox.Show("This file belongs to buidding setting . Please load in online mode building setting section");
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

                //=============================comment now,not required===================//
                //string nodeSize = xn["nodesize"].InnerText;
                //string deviceInstanceVal = xn["deviceInstance"].InnerText;
                //string ipVal = xn["ip"].InnerText;
                //string param1idVal = xn["param1id"].InnerText;
                //string param2idVal = xn["param2id"].InnerText;
                //string param1infoVal = xn["param1info"].InnerText;
                //string param2infoVal = xn["param2info"].InnerText;
                //string param1typeVal = xn["param1id_type"].InnerText;
                //string param2typeVal = xn["param2id_type"].InnerText;

              //=================no required============================================//

                //now lets add these values to list
                nodeInformationFromXML.Add(new TempDataType
                {
                    id = int.Parse( nodeID),
                    name = name,
                    label = label,
                    source = source,
                    colorValue = ColorTranslator.FromHtml(color),
                    xVal = double.Parse(xvalue),
                    yVal = double.Parse(yvalue),
                    showItemText = showTextItem,
                    //nodeSize = nodeSize,
                    //device_instance_id = deviceInstanceVal,
                    //ip = ipVal,
                    //param1id = param1idVal,
                    //param2id = param2idVal,
                    //param1info = param1infoVal,
                    //param2info = param2infoVal,
                    //param1_id_type = param1typeVal,
                    //param2_id_type = param2typeVal
                });
            }//close of foreach


            //--loading the line info from the doc
            XmlNodeList xlList = xmlDoc.SelectNodes("RootNode/lines/line");

            foreach (XmlNode xn in xlList)
            {
                string idVal = xn["ID"].InnerText;
                string prevNodeIDVal = xn["prevNodeID"].InnerText;
                string nextNodeIDVal = xn["nextNodeID"].InnerText;
              //  string lineThicknessVal = xn["linethickness"].InnerText;
                string serieNameIDval = xn["seriesname"].InnerText;
                string linecolor = xn["linecolor"].InnerText;
                //now lets add these values to list
                lineInformationFromXML.Add(new lineNodeDataType
                {
                    ID = int.Parse(idVal),
                    prevNodeId =int.Parse( prevNodeIDVal),
                    nextNodeId =int.Parse( nextNodeIDVal),
                   // lineThickness = int.Parse(lineThicknessVal),
                    lineSeriesID = new Series(serieNameIDval),
                    lineColorValue = ColorTranslator.FromHtml(linecolor)
                });
            }//close of foreach
             // MessageBox.Show("count node values " + nodeInfoFromXMLfile.Count + ",line count" + lineInfoFromXMLfile.Count);
             //--now since both value is added we need to insert these values to db now
             //-- and reload them
             /*
             Task : 
              1. delete the existing value first 
              2. insert the existing file value form file
             
             */

            //First lets check if the chart is selected or not.
            if(currentNodeTableFromDB == "" || currentLineTableFromDB == "")
            {
                MessageBox.Show("Please select a chart properly first");
                return;
            }



            //===============================Delete the line and node value================//

            if(currentNodeTableFromDB != null && currentLineTableFromDB != null)
            { 
            //First delete the node values 
            DeleteAllTableData(currentNodeTableFromDB);
                //Delete all data form line table
            DeleteAllTableData(currentLineTableFromDB);

            }
            //===============================end of the delete===========================//




            //===================================inserting data form xml into db=====================================//
            foreach (var node in nodeInformationFromXML)
            {
                //we need to inset it to db but if the node id is already present then dont inset just load....
                InsertNodeInfoToDB(node.id, node.xVal, node.yVal, node.source, node.name, node.label, node.colorValue, node.showItemText);
            }

            foreach (var line in lineInformationFromXML)
            {
                InsertLineInfoToDB(line.ID, line.prevNodeId, line.nextNodeId, line.lineColorValue, line.lineSeriesID);
            }
            //=====================================end of inserting in db=============================//
            //now lets load the data back again..
            //--helps in loading in redrawing the parts.
            RefreshGraph();
            LoadNodeAndLineFromDB(idSelectedOfChart);   //Lets make it passing the stirngs       
            ReDrawingLineAndNode();

            MessageBox.Show("Load success");


        }

        public void DeleteAllTableData(string tableName)
        {

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string queryString = "DELETE   FROM  "+tableName;


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //  command.Parameters.AddWithValue("@id_value", id);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                command.ExecuteNonQuery();
       
            }//close of using..

        }


        public void SetNode(string source, string name, string label, Color c1, string comboboxItemText1)
        {

            tbSource = source;
            tbName = name;
            tbLabel = label;
            colorValue = c1;
            comboboxItemText = comboboxItemText1;
            //lets do the processing 
            //lets count how many items were inserted
            countNumberOfPoints += 1;
            //lets get the coordinates to plot on the graph..
            //this will be set on right click..not here


            //lets process the data 
            /*
            calculating the humidity and temperature value form the coordinates..
            */
            HumTempCalcByCoordinate();
            //MessageBox.Show("Temp= " + temperatureValue + ",hum = " + humidityValue);

            //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if ((humidityValue > 0 && humidityValue <= 100) && (temperatureValue >= 0 && temperatureValue <= 50))
            {
                //now lets plot the values....

                plot_by_DBT_HR_process_diagram((double)(int)temperatureValue, (double)humidityValue / 100);




            }
            else
            {
                MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
            }





        }
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

                    double patm = form1Object.AirPressureFromDB * 0.001;// in kpa//101.325;//this is constant...
                                          // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                          //double w1 = 622*phi*pg/(patm-phi*pg);
                    double w = yVal;
                    phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                    phi = phi * 100;//changing into percent..

                    //now lets calculate the dew point...
                    double humidity = phi;
                    double temperature1 = xVal;

                    humidityValue = humidity;
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

            plot_on_graph_values_process_diagram(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            // index++;

            return 0;
        }

        public void plot_on_graph_values_process_diagram(double dbt, double hr, double xval, double yval)
        {
            //chart1.Series.Clear();


            try
            {


                series1.ChartType = SeriesChartType.Point;
                //int r, g, b;

                series1.MarkerSize = 20;
                series1.MarkerStyle = MarkerStyle.Circle;
                //string label = "DBT=" + dbt + ",HR=" + hr;
                //series1.Label = label;
                //chart1.Series["SeriesDBT_HR" + index].;
                //series1.Points[0].Color = colorValue;//blue
                // MessageBox.Show("finally added xvalue = " + xval + " yvalue = " + yval);
                series1.Points.AddXY(xval, yval);
                string s = "source : " + tbSource + "\n Name : " + tbName + "\nLable : " + tbLabel;
                series1.Points[index].Color = colorValue;
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

            //the value is added...
            menuStripNodeInfoValues.Add(new TempDataType
            {
                id = index,
                xVal = xval,
                yVal = yval,
                source = tbSource,
                name = tbName,
                label = tbLabel,
                colorValue = colorValue,
                showItemText = comboboxItemText

            });

            if(flagForInsertOrUpdateDataToDB == 1) { 
                //do only if the  flag is raised.
                //--Adding these values to the database....
            InsertNodeInfoToDB(index, xval, yval, tbSource, tbName, tbLabel, colorValue,comboboxItemText);
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



                //--Lets store the nodeline info as well
                menuStripNodeLineInfoValues.Add(new lineNodeDataType
                {

                    //--Id of this ..
                    ID = index,
                    prevNodeId = index - 1,
                    nextNodeId = index,
                    lineColorValue = menuStripNodeInfoValues[index - 1].colorValue,
                    lineSeriesID = newLineSeries

                });

                //--Inserting the line to database here....


                  if(flagForInsertOrUpdateDataToDB == 1)
                {
                   //--Insert the values when the flag is raised.
                    InsertLineInfoToDB(index, index - 1, index, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries);
                }



                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newLineSeries.ChartType = SeriesChartType.Line;
                //newLineSeries.MarkerStyle = MarkerStyle.Circle;
                //newLineSeries.MarkerStyle = MarkerStyle.Star6;
                newLineSeries.MarkerBorderWidth.Equals(15);
                newLineSeries.MarkerSize.Equals(1);
                newLineSeries.BorderWidth.Equals(15);
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



            index++;


        }//close of buttons


        //--Insert node to database....
     public void   InsertNodeInfoToDB(int id, double xVal, double yVal, string source, string name, string label, Color colorValue, string showItemText)
        {
            string tableName = currentNodeTableFromDB;

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into "+tableName+"(id,xValue,yValue,source,name,label,colorValue,showItemText) VALUES(@id,@xVal,@yVal,@source,@name,@label,@colorVal,@text)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                command.Parameters.AddWithValue("@text",showItemText);

                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

            }//--close of insertnodeinfotodb fxn

        public void UpdateNodeInfoToDB(int id, double xVal, double yVal, string source, string name, string label, Color colorValue, string showItemText)
        {
            string tableName = currentNodeTableFromDB;

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set  xValue =@xVal ,  yValue=@yVal, source=@source, name=@name, label=@label , colorValue=@colorVal, showItemText=@text where id =@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@name", name);
                command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn



        public void InsertLineInfoToDB(int id,int prevNodeId,int nextNodeId,Color lineColor,Series lineSeriesVal)
        {
            //--Note for series the series.name property will be stored in databse and later it will be converted.
            string lineSeriesName = lineSeriesVal.Name;
            string tableNamex = currentLineTableFromDB;

          //  MessageBox.Show("Table name for line =" + tableNamex);

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableNamex + "(id,prevNodeId,nextNodeId,lineColorValue,lineSeriesId) VALUES(@id,@pn,@nn,@lc,@ls)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                                
                command.ExecuteNonQuery();
            }


        }


        public void UpdateLineInfoToDB(int id, int prevNodeId, int nextNodeId, Color lineColor, Series lineSeriesVal)
        {
            string lineSeriesName = lineSeriesVal.Name;
            string tableName = currentLineTableFromDB;

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set prevNodeId=@pn,nextNodeId=@nn,lineColorValue=@lc,lineSeriesId=@ls where   id=@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@id", id);
                command.ExecuteNonQuery();
            }


        }
        private void disconnectLineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */

            //Series s1;
            //int prevNodeID;
            //int nextNodeId;



            flagForDisconnectClick = 1;
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }
            insertNodeToolStripMenuItem.Enabled = false;
            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);

        }
    }
}
