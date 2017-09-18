using System;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Collections;
using System.Data.SqlClient;
using System.IO;
using System.Data.SQLite;
using System.Reflection;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Data;
using System.Xml;
using System.Net;
using System.Linq;
using System.Data.OleDb;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.ComponentModel;

using Excel = Microsoft.Office.Interop.Excel;

namespace WFA_psychometric_chart
{
    //-- this line is for com visibility...
    [ComVisible(true)]

    
    public partial class Form1_main : Form
    {
        //OleDbCommand cmd = new OleDbCommand();
        //OleDbConnection con = new OleDbConnection();
        SqlConnection con = new SqlConnection();
        SqlCommand cmd = new SqlCommand();

        //----------This one for database operation calls to reuse this part-------//

      //  DatabaseOperations dbClientClass;
         //--------end of the database operations ------------------//
        public Form1_main()
        {
            InitializeComponent();
            //this.Disposed += new System.EventHandler ( this.Form1_main_Disposed );
            //this is done to copy the instance of form1 to be reusable in DatabaseOperations
          //  dbClientClass = new DatabaseOperations(this);
        }



        /// <summary>
        /// This returns air pressure in pascal (pa)
        /// </summary>
        public double AirPressureFromDB = 0;

        //--lets define the constanst..
        double temperature, humidity, Patm, TDewpoint, A, m, Tn, B, Pws, X, h;
        int index = 0;
        ToolTip tp = new ToolTip();
        //this is for if the map is not loaded we need to load the map  first..
        int map_loaded = 0;//till now no if yes then it will set to 1.

        //this array list is used in the web part where we pull the value from an api and store in this array list
        ArrayList temp_AL = new ArrayList();
        ArrayList hum_AL = new ArrayList();

        //this is for updateing the values constantly
        ArrayList temp2_AL = new ArrayList();
        ArrayList hum2_AL = new ArrayList();
        Series series1xx = new Series("My Series values plot ");//this series is used by plot_on_graph_values() method...
        Series seriesLineIndicator = new Series("LineIndicator");//--This line indicator is temporary show the line in the chart for Node Movement.

        //--This is the constant list that is being used by our program i.g :it contains t_pg.tx and t_pg1.txt values
        //List<double,double> t_pg_list =  


        ArrayList t = new ArrayList();//this stores the temperature(deg.cel)
        ArrayList pg = new ArrayList();//this stores the saturated vapour pressure(kpa).

        //--This is a global variable that is used by heat map 
        int index_selected;


        //--Flags are defined here...
        int flagForDisconnectClick = 0;//0 means false it is used to see if the disconnect option is clicked or not.
        int flagNodeSelectedForConnect = 0;


        //--variable for storing the indexOfThePoints so that we can gather other properties..
        string indexOfPrevPointForLineMovement="";
        string indexOfNextNodeForLineMovement = "";

        //This series is for temporary line drawing for line movements...
        Series addDottedSeries = new Series("newSeries101");
        //--If series is present delete the previouse series


        //--id of the node selected, this is used when line is detached and for reconnecting we need to know which node we are connecting to.
        string idOfNodeSelected;//--initially it represents nothing...

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
            chart1.Series["Series1"].Color = Color.OrangeRed;    //was Red

            chart1.Series["Series2"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series2"].Color = Color.LightBlue;   //Was blue

            chart1.Series["Series3"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series3"].Color = Color.LightBlue; //WAS BLUE
            chart1.Series["Series4"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series4"].Color = Color.LightBlue;  //WAS BLUE
            chart1.Series["Series5"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series5"].Color = Color.LightBlue;  //WAS BLUE

            chart1.Series["Series6"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series6"].Color = Color.LightBlue;   //WAS BLUE
            chart1.Series["Series7"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series7"].Color = Color.LightBlue; //WAS BLUE


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
            double pressureConverted = AirPressureFromDB * 0.001;//now in terms of kpa
                                                                 // MessageBox.Show("pressure(kpa) = " + pressureConverted);
            lb_pressure_display.Text = "Pressure : " + Math.Round(pressureConverted, 2) + " KPa";//in terms of kpa
            double patm =   pressureConverted;//101.235;//constant..we will make it take as input later...   in KPa
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

            chart1.Series["Series1"].Points[16].Label = WFA_psychometric_chart.Properties.Resources.Wet_bulb_temp;
            chart1.Series["Series1"].Points[16].LabelBackColor = Color.OrangeRed;  //WAS Red
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

                    //if (phi == 0.3)
                    //{
                    //    chart1.Series["Series" + ival].Points[15].Label = "Humidity Ratio";
                    //}
                    // s1 += x2 + "," + y + ";";


                    //index++;
                }//close of for
                //MessageBox.Show(s1);
                ival++;
                //this is to print 10%,20,30,40% 
                int c = int.Parse((phi * 10 + 1).ToString());
                if (phi >= 0.30 && phi < 0.4) {
                    chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                    chart1.Series["Series" + c].Points[42].Label = "Relative Humidity";
                    // MessageBox.Show("Hello");
                }
                else
                {
                    chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                }
                //chart1.Series["Series"+c].Points[46].LabelBackColor = Color.Blue;

                //MessageBox.Show("hel1");
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
                chart1.Series["Line" + i].Color = Color.LightGreen ; //WAS GREEN
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
                chart1.Series["Line_r" + i].Color = Color.OrangeRed;  //WAS Red
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
                chart1.Series["Line_b" + hval].Color = Color.MediumVioletRed; //was black
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
            chart1.Series["Line_b_straight"].Color = Color.MediumVioletRed; //Was black
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(0, 5));
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(25, 30));
            chart1.Series["Line_b_straight"].ChartType = SeriesChartType.Line;
            //chart1.Series["Line_b_straight"].Points[0].Label = "Enthalpy kj/kg dry air";
        }   //--Close of plot new graph
        private void button1_Click(object sender, EventArgs e)
        {
            //ClearChart();
        }

        void ClearChart()
        {
            this.Invalidate();
            chart1.Invalidate();
            // chart1.Dispose();//--Releases all the resources used by the chart...
            plot_new_graph();
            lb_title_display.Text = "";
            //--Reseting the menustrip values for new plotting....
            menuStripNodeLineInfoValues.Clear();
            menuStripNodeInfoValues.Clear();
            index = 0;
            incrementIndex = 0;
            insertNodeToolStripMenuItem.Enabled = true;/*insert node will be dissable with historical plot so reenabling it*/

        }

        public bool checkForDataInSqlite()
        {
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile1 + ";Version=3;";
            //MessageBox.Show("Path = " + databaseFile1);
            bool returnValue = false;
            string id = "";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //command.Parameters.AddWithValue("@index", index);
                //command.Parameters.Add("@index",DbType.Int32).Value= index_selected;
                //command.Parameters.AddWithValue("@index", index_selected);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    id = reader["ID"].ToString();
                }
            }
            if (id != "")
            {
                returnValue = true;
            }
            else
            {
                returnValue = false;
            }
            //--This will be either true or false based on the check value..
            return returnValue;
        }

        public string PathToT3000BuildingDB = "";
        public string CurrentSelectedBuilding = "";
        public void Form1_Load(object sender, EventArgs e)
        {

            //simulationMode.Text = WFA_psychometric_chart.Properties.Resources.Historical_Plot;
            lb_title_display.Text = "";
            //=====================================DATABASE OPERATION===============================//
            //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";
            

            //*************************This section is already implemented before program run is in Program.cs file*********//
            //So no need to run agin

            /*
           Find the selected building in T3000 database and also see which building is selected based on that 
           building the tables will be created if not present else 
           */
            //  FindPathOfBuildingDB();
            /*
            FindPathOfBuildingDBNewVersion();
            MessageBox.Show("Building Selected Name = " + BuildingSelected[0].Building_Name);
            string selectedBuildingFromT3000 = BuildingSelected[0].Building_Name;//This stores the building name selected in alex part
                                                                                                                                      
            if (File.Exists(databaseFile1))
            {
                //file exist so dont create the database      
            }
            else {
                MessageBox.Show("Internal database not found. Creating fresh database");
                //this.Close();
                if(selectedBuildingFromT3000 != "") { 
                //--sqlite new databse creation
                sqlite_database_creation(selectedBuildingFromT3000);
                }
                else
                {
                    //End the application
                    MessageBox.Show("Could not create new databse.Closingn!");
                    this.Close();
                }
            }
               */
            //**************************end of this section ******************************//


            //first lets check for the data and then give user a message if a building is not create and selected.
            if(checkForDataInSqlite() != true)
            {
                //value is not present so say user select a building first / configure a building first then open again.
                MessageBox.Show("Please Configure the building location in T3000 first and restart the application again");
                //this.exit();
                this.Close();
                return;
            }
            //--This pulls the location info form alex db
            PullLocationInformation();//this is for loading location information

            //MessageBox.Show("ONe");

            //--This part is for checking the database and update the lat,long,elevation values in database...
            if (CheckLatLongAvailable() == true)
            {
                FillLatLongValueAutomatically();//--Fill the lat long values...
               //--MessageBox.Show("show filllat");
            }
            //--Now here lets pull the values from the database...
            /*
            This is basically for pulling altitude value for calculating the pressure value.
            */
            get_stored_data_about_building();

            //We have formula for altitude and pressure calculation
            /*
            #formula:
            P= 101325(1-2.25577*10^(-5)*h)^5.25588
            where p = air pressure in pa 
             h = altitude in meteres
            */
            // AirPressureFromDB = 

           //MessageBox.Show("Two");
            double altitue = buildingList[0].elevation;
            double P = 101325 * Math.Pow((1 - (2.25577 * Math.Pow(10, -5) * altitue)), 5.25588);

            if (P == 0 || P.ToString() == "")
            {
                //if empty or null put a default value
                AirPressureFromDB = 101325;//in terms of pa
            }
            else {
                AirPressureFromDB = P;
                //LETS PRINTAND SEE 
               // MessageBox.Show("pressure =" + AirPressureFromDB);
            }

           // AirPressureFromDB = 101325;//in terms of pa
           //=====================================END DB OPERATION=================================//

            //lets add the t and pg values
            add_t_pg();//Calling this method add the value...

            //--This is for label1 and label2
            //label1.Text = WFA_psychometric_chart.Properties.Resources.From;
            //label2.Text = WFA_psychometric_chart.Properties.Resources.To;
            //button2.Text = WFA_psychometric_chart.Properties.Resources.Show_Heat_Map;


            //lets plot the graph as soon as the form loads.
            plot_new_graph();

            //button1.Text = "Clear Chart"; //WFA_psychometric_chart.Properties.Resources.Refresh_Graph;
            //this is for adding values dynamically as the program loads. used by plot_on_graph_values() method 
            chart1.Series.Add(series1xx);
            //--This is added for the process diagram part...
            chart1.Series.Add(series1);
            chart1.Series.Add(series1_heat_map);
            chart1.Series.Add(seriesLineIndicator);//--This line indicator is for show temporary line for movement...
             //=============================================Code from buildingSetting section ================================//  
             //--ADDITIONAL PART OF CODE...

             //==========================For building infor==========//

              //  MessageBox.Show("Test :Shows path of the building "+ BuildingSelected[0].Building_Path+",Building Name= "+ BuildingSelected[0].Building_Name);
              //==========================end of building info========//

              // MessageBox.Show("THREE");
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
            lb_unit_chosen_display.Text = "Unit : " + buildingList[0].EngineeringUnits;
            lb_db_name.Text = buildingNameValue;

            //--Storing the currently selected building in a variable
            CurrentSelectedBuilding = selectedBuildingList[0].BuildingName;
            // MessageBox.Show("Current Building Selected" + CurrentSelectedBuilding);

            //--This is where the table creation is done
            //--Commented later uncomment it
            CreateRequireTableIfNotPresent(buildingNameValue);

            DataGridView_Show_Data();
            dataGridView1.Rows.Add();
            //loading the comfortzone when set             
            //This to prevent anonomous plotting
            chart1.Enabled = false;
            //==============================end of building selection starts==================//

            //=======================================End of building Setting section.=======================================//
            // public List<chartDetailDT> chartDetailList = new List<chartDetailDT>();//This is used for storing the chart detail ids

            if (dataGridView1.Rows.Count > 0)  //If there is data then only do this one
            {
                //set parameters of your event args
                // var eventArgs = new DataGridViewCellEventArgs(1, 0);
                //// or setting the selected cells manually before executing the function
                // dataGridView1.Rows[0].Cells[1].Selected = true;
                // dataGridView1_CellClick(sender, eventArgs);
                //if (dataGridView1.CurrentCell.RowIndex >= 0 && dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                //{
                //    var eventArgs = new DataGridViewCellEventArgs(1, 0);
                //    // or setting the selected cells manually before executing the function
                //    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                //    dataGridView1_CellClick(sender, eventArgs);

                //}
                if (dataGridView1.CurrentCell.RowIndex >= 0 && dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                {
                    var eventArgs = new DataGridViewCellEventArgs(dataGridView1.CurrentCell.ColumnIndex, dataGridView1.CurrentCell.RowIndex);
                    //or setting the selected cells manually before executing the function
                    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                    dataGridView1_CellClick(sender, eventArgs);
                    //MessageBox.Show("selecteion change clicked");
                }
                else
                {
                    var eventArgs = new DataGridViewCellEventArgs(1, 0);
                    // or setting the selected cells manually before executing the function
                    dataGridView1.Rows[0].Cells[1].Selected = true;
                    dataGridView1_CellClick(sender, eventArgs);
                    // }
                }
            }


            //--Assing the location path to alex database 
            //goind one step above to the porject
            string path = databasePath1 ;
            //string path = @"C:\Folder1\Folder2\Folder3\Folder4";
            string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
            string againNewPath = newPath+@"Database\Buildings\"+ buildingNameValue +@"\"+buildingNameValue+".db";  //psychopath+ database\Buildings\"BuildingName"\"BuildingName.db" 
            PathToT3000BuildingDB = againNewPath;
            //MessageBox.Show(againNewPath);




            //--This one is for date calculation
            //AssemblyCreationDate acd = new AssemblyCreationDate();
            this.Text = "Psychometric Chart [" + AssemblyCreationDate.Value.ToShortDateString() + "]";


        }  //Close of the laod function


    //***************************************Codes from building settings*********************************************//


            /// <summary>
            /// This function is used by trash section 
            /// and helps in refreshing everything
            /// </summary>
            public void LoadFunctionForTrash()
        {
            //simulationMode.Text = WFA_psychometric_chart.Properties.Resources.Historical_Plot;
            lb_title_display.Text = "";
            //=====================================DATABASE OPERATION===============================//
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";
            if (File.Exists(databaseFile1))
            {
                //file exist so dont create the database      
            }
            else {
                MessageBox.Show("Internal database not found");
                // this.Close();
                if (BuildingSelected.Count > 0)
                { 

                //--sqlite new databse creation
                sqlite_database_creation(BuildingSelected[0].Building_Name);
                }
            }

            //first lets check for the data and then give user a message if a building is not create and selected.
            if (checkForDataInSqlite() != true)
            {
                //value is not present so say user select a building first / configure a building first then open again.
                MessageBox.Show("Please Configure the building location in T3000 first and restart the application again");
                //this.exit();
                this.Close();
                return;
            }
            //--This pulls the location info form alex db
            PullLocationInformation();//this is for loading location information

            //--This part is for checking the database and update the lat,long,elevation values in database...
            if (CheckLatLongAvailable() == true)
            {
                FillLatLongValueAutomatically();//--Fill the lat long values...
                                                //  MessageBox.Show("show filllat");
            }
            //Now here lets pull the values from the database...
            /*
            This is basically for pulling altitude value for calculating the pressure value.
            */
            get_stored_data_about_building();
            //We have formula for altitude and pressure calculation
            /*
            #formula:
            P= 101325(1-2.25577*10^(-5)*h)^5.25588
            where p = air pressure in pa 
             h = altitude in meteres
            */
            // AirPressureFromDB = 
            double altitue = buildingList[0].elevation;
            double P = 101325 * Math.Pow((1 - (2.25577 * Math.Pow(10, -5) * altitue)), 5.25588);

            if (P == 0 || P.ToString() == "")
            {
                //if empty or null put a default value
                AirPressureFromDB = 101325;//in terms of pa
            }
            else {
                AirPressureFromDB = P;
            }

            // AirPressureFromDB = 101325;//in terms of pa
            //=====================================END DB OPERATION=================================//

            //lets add the t and pg values
            add_t_pg();//Calling this method add the value...

            //--This is for label1 and label2
            //label1.Text = WFA_psychometric_chart.Properties.Resources.From;
            //label2.Text = WFA_psychometric_chart.Properties.Resources.To;
            //button2.Text = WFA_psychometric_chart.Properties.Resources.Show_Heat_Map;


            //lets plot the graph as soon as the form loads.
            plot_new_graph();

            //button1.Text = "Clear Chart"; //WFA_psychometric_chart.Properties.Resources.Refresh_Graph;
            //this is for adding values dynamically as the program loads. used by plot_on_graph_values() method 
            chart1.Series.Add(series1xx);
            //--This is added for the process diagram part...
            chart1.Series.Add(series1);
            chart1.Series.Add(series1_heat_map);
            chart1.Series.Add(seriesLineIndicator);//--This line indicator is for show temporary line for movement...
                                                   //=============================================Code from buildingSetting section ================================//  
                                                   //--ADDITIONAL PART OF CODE...

            //==========================For building infor==========//

            FindPathOfBuildingDB();
           // MessageBox.Show("Test :Shows path of the building " + BuildingSelected[0].Building_Path + ",Building Name= " + BuildingSelected[0].Building_Name);
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
            lb_unit_chosen_display.Text = "Unit : " + buildingList[0].EngineeringUnits;
            lb_db_name.Text = buildingNameValue;

            //--Storing the currently selected building in a variable
            CurrentSelectedBuilding = selectedBuildingList[0].BuildingName;
            MessageBox.Show("Current Building Selected" + CurrentSelectedBuilding);

            //--This is where the table creation is done
        //CreateRequireTableIfNotPresent(buildingNameValue);

            DataGridView_Show_Data();
            dataGridView1.Rows.Add();
            //loading the comfortzone when set             
            //This to prevent anonomous plotting
           // chart1.Enabled = false;

        }


        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Section for hardware%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


        // int countX = 0;
        /// <summary>
        ///This info is about the nodes 
        /// </summary>
        //Lets build a dynaic list
        public class device_info_class
        {
            public string nodeID { get; set; }
            public string device_instance_id_for_param1 { get; set; }
            public string device_IP_for_param1 { get; set; }

            public string device_instance_id_for_param2 { get; set; }
            public string device_IP_for_param2 { get; set; }
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

            string sql1 = "SELECT * From  " + tableNameDevice + "  where nodeID='" + nodeID1 + "'";
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
                        device_instance_id_for_param1 = reader["device_instanceID_for_param1"].ToString(),
                        device_IP_for_param1 = reader["IP_for_param1"].ToString(),
                        device_instance_id_for_param2 = reader["device_instanceID_for_param2"].ToString(),
                        device_IP_for_param2 = reader["IP_for_param2"].ToString(),
                        param1_id = reader["param1ID"].ToString(),
                        param2_id = reader["param2ID"].ToString(),
                        param1_info = reader["param1_info"].ToString(),
                        param2_info = reader["param2_info"].ToString(),
                        param1_identifier_type = reader["param1_identifier_type"].ToString(),
                        param2_identifier_type = reader["param2_identifier_type"].ToString()
                                                                         
                    });
                }


            }

        }


        public void PullDataFromHardware(string device_instance_id, string param1id, string param2id, string param1info, string param2info)
        {
            if (param1info == "temp" && param2info == "hum")
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

           // MessageBox.Show("inside timer");
            flagForTimer = 1;
            atimer = new System.Timers.Timer();
            atimer.Enabled = true;
            atimer.Elapsed += timer1_Tick_For_Device;

            atimer.Interval = 1000 * 5; //x seconds[ 1000 ms * x  =  x seconds]

        }

     
        public async Task AsyncMethod1ForPullingData()
        {
            try
            {
                // Do asynchronous work.
                await Task.Run(() => RefreshDataFromDeviceAndWeb());
            }
            catch (Exception ex)
            {

            }
        }
        public void timer1_Tick_For_Device(object sender, EventArgs e)
        {

            try
            {
                //    if (InvokeRequired)
                //    {
                //        //this.Invoke(new Action(() => RefreshDataFromDeviceAndWeb()));
                //        this.Invoke(new Action(() => AsyncMethod1ForPullingData()));
                //        return;
                //    }
                //    else
                //    {
                //        AsyncMethod1ForPullingData();
                //    }
                //--Now lets do the same task with background worker
                //if (!bgWorker.IsBusy)
                //{
                //   // MessageBox.Show("background worker calling");
                //    bgWorker.RunWorkerAsync();//--Running the worker async
                //}

               // MessageBox.Show("Entered form timer");
                if (FlagForCntdBG_Update == 0)
                {
                    //Means the edit/OFFLINE mode is ON then return 
                  //  MessageBox.Show("Entered FlagForCntdBG_Update=0");
                   // chart1.Enabled = true;
                    return;
                }
                else {
                   // chart1.Enabled = false;
                    //MessageBox.Show("Entered just else ");
                    if (!backgroundWorker1.IsBusy)
                    {
                       //chart1.Enabled = true;
                      //  MessageBox.Show("Entered FlagForCntdBG_Update=1");
                        // MessageBox.Show("background worker calling");
                        backgroundWorker1.RunWorkerAsync();//--Running the worker async
                       // chart1.Enabled = false;

                    }


                }



            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }



        public void StartHardwareScan()
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
                atimer.Elapsed -= timer1_Tick_For_Device;
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

        int countTime = 0;

        int FlagForStopingRunningProcessInstantly = 0;//Currently OFF

        public void RefreshDataFromDeviceAndWeb()
        {

           // lock (menuStripNodeInfoValues)
            // {

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

             if(FlagForStopingRunningProcessInstantly == 1)
            {
                //if it is commanded to stop then stop here;
                //also make this flag turn OFF when leaving 
                FlagForStopingRunningProcessInstantly = 0; //Do not stop next time
                return;
            }

                LoadNodeAndLineFromDB(indexOfChartSelected);//indexOfChartSelected= index chosen

                if (menuStripNodeInfoValues.Count < 1)
                {
                    //--No node then 
                    return;
                }
              try { 

            //--Read each node and then perform the following fxn

            //--*****************************From here commented the code for alex db****************--//
            foreach (var node in menuStripNodeInfoValues)
            {

                //Check if there is command to stop or not
                if (FlagForStopingRunningProcessInstantly == 1)
                {
                    //if it is commanded to stop then stop here;
                    //also make this flag turn OFF when leaving 
                    FlagForStopingRunningProcessInstantly = 0;
                    return;
                }


                //=======================For with single node for for Temperature source check and update===========//
                 //param1 value is temperature 


             if (node.temperature_source == "Device")
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

                if (CheckDeviceOnlineOffline(int.Parse(device_info_list[0].device_instance_id_for_param1), 0) == true)
                {
                    //online mode...
                    //2nd step calc x and y
                    //The value will always be unique and always be in the 0 index  
                    if (device_info_list[0].param1_info == "temp")
                    {
                        //This meand the value is humidity and temperature so we process like wise
                        //This gets the value
                        //ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id), uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param1_identifier_type, device_info_list[0].param2_identifier_type);
                         //ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id_for_param1), uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param1_identifier_type, device_info_list[0].param2_identifier_type);
                                ReadDataFromDeviceForTemperature(int.Parse(device_info_list[0].device_instance_id_for_param1), uint.Parse(device_info_list[0].param1_id), device_info_list[0].param1_identifier_type);
                                //we have recent value in hardwareValue1 and hardwareValue2 so lets calc corresponding x and y value
                                //now temp itself is x value we need to calculate y value

                                // if ((hardwareValue1.ToString() == null || hardwareValue1 == 0.00) || (hardwareValue2.ToString() == null || hardwareValue2 == 0.00))
                                if ((hardwareValue1.ToString() == null || hardwareValue1 == 0.00))
                                {
                            return;
                        }

                        //MessageBox.Show("inside")

                        double x_Value = hardwareValue1;
                      //  double y_value = CalculateYFromXandHumidity(hardwareValue1, hardwareValue2 / 100);

                        //   MessageBox.Show("x val /temp/hardwareValue1 = "+x_Value+"\nhardwareValue2"+hardwareValue2+"\ny value hardware= " + y_value);
                        //Now lets update the values in db

                        lock (menuStripNodeInfoValues)
                        {
                                    // UpdateNodeInfoToDB(node.id, x_Value, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);
                                    UpdateNodeInfoToDBForTemeperatureFromHardware(node.id, x_Value);//This is completed
                        }

                        countTime++;
                                //=============STATUS SHOWING ONLINE OR DEVICE OFFLINE=================


                                //*************************Uncomment later----------------------------//

                                if (lb_device_status.InvokeRequired)
                                {
                                    lb_device_status.Invoke(new Action(() => lb_device_status.Text = "connected"));
                                }
                                else
                                {
                                    lb_device_status.Text = "connected";
                                }

                                //************************end of uncomment later------------------------/

                                ////--This is test of weather out function is working or not
                                //if (lb_test1.InvokeRequired)
                                //{
                                //    lb_test1.Invoke(new Action(() => lb_test1.Text = countTime + "=>inside device ,xValue/HV1=" + hardwareValue1 + ",HV2=" + hardwareValue2 + ",yvalue=" + y_value));
                                //}
                                //else
                                //{
                                //    lb_test1.Text = countTime + "=>inside device ,xValue/HV1=" + hardwareValue1 + ",HV2=" + hardwareValue2 + ",yvalue=" + y_value;
                                //}

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
                    //else if (device_info_list[0].param1_info == "temp" && device_info_list[0].param2_info == "enthalpy")
                    //{
                    //   //--No data for now
                    //}
                    //else
                    //{
                    //    //First is humidity and second is enthalpy
                    //}

                }
                else
                {
                    //offline mode
                    lb_device_status.Text = "disconnected";
                }

            }
            else if (node.temperature_source == "Web")
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
                //double humidity_Val = double.Parse(hum_pulled);
               // double y_value = CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);
                //  MessageBox.Show("temp / temp_pulled_form_web = "+temperature_Val+"\nhum = "+humidity_Val+"\ny value web= " + y_value);
                //Now lets update the values in db
                lock (menuStripNodeInfoValues)
                {
                            //  UpdateNodeInfoToDB(node.id, temperature_Val, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);
                            UpdateNodeInfoToDBForTemeperatureFromHardware(node.id, temperature_Val);//This is completed
                }
                //==============THIS ONE IF FOR ONLINE OFFLINE==========
                //  lb_web_status.Text = "active";                            
                //=====================END OF ONLINE OFFLINE============           
            } //Close of web

            //=============================================Code for parameter 1 ie temperature complete===================//

                    ///===================================================Now for second parameter of node humidity updateing=======//

                    if (node.humidity_source == "Device")
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

                        if (CheckDeviceOnlineOffline(int.Parse(device_info_list[0].device_instance_id_for_param2), 0) == true)
                        {
                            //online mode...
                            //2nd step calc x and y
                            //The value will always be unique and always be in the 0 index  
                            if (device_info_list[0].param2_info == "hum")
                            {
                                //This meand the value is humidity and temperature so we process like wise
                                //This gets the value
                                //ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id), uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param1_identifier_type, device_info_list[0].param2_identifier_type);
                                //ReadDataFromDevice(int.Parse(device_info_list[0].device_instance_id_for_param1), uint.Parse(device_info_list[0].param1_id), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param1_identifier_type, device_info_list[0].param2_identifier_type);
                                ReadDataFromDeviceForHumidity(int.Parse(device_info_list[0].device_instance_id_for_param2), uint.Parse(device_info_list[0].param2_id), device_info_list[0].param2_identifier_type);
                                //we have recent value in hardwareValue1 and hardwareValue2 so lets calc corresponding x and y value
                                //now temp itself is x value we need to calculate y value

                                // if ((hardwareValue1.ToString() == null || hardwareValue1 == 0.00) || (hardwareValue2.ToString() == null || hardwareValue2 == 0.00))
                                if ((hardwareValue2.ToString() == null || hardwareValue2== 0.00))
                                {
                                    return;
                                }

                                //MessageBox.Show("inside")

                                double x_Value = node.xVal;   //This one is the x value
                                 double y_value = CalculateYFromXandHumidity(x_Value, hardwareValue2 / 100);

                                //   MessageBox.Show("x val /temp/hardwareValue1 = "+x_Value+"\nhardwareValue2"+hardwareValue2+"\ny value hardware= " + y_value);
                                //Now lets update the values in db

                                lock (menuStripNodeInfoValues)
                                {
                                    // UpdateNodeInfoToDB(node.id, x_Value, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);
                                    // UpdateNodeInfoToDBForTemeperatureFromHardware(node.id, x_Value);//This is completed
                                    UpdateNodeInfoToDBForHumidityFromHardware(node.id, y_value);
                                }

                                countTime++;
                                //=============STATUS SHOWING ONLINE OR DEVICE OFFLINE=================

                                //*****************uNCOMMENT LATER-------------------//
                                if (lb_device_status.InvokeRequired)
                                {
                                    lb_device_status.Invoke(new Action(() => lb_device_status.Text = "connected"));
                                }
                                else
                                {
                                    lb_device_status.Text = "connected";
                                }

                                //**************************END OF UNCOMMENT LATER----------------//
                                ////--This is test of weather out function is working or not
                                //if (lb_test1.InvokeRequired)
                                //{
                                //    lb_test1.Invoke(new Action(() => lb_test1.Text = countTime + "=>inside device ,xValue/HV1=" + hardwareValue1 + ",HV2=" + hardwareValue2 + ",yvalue=" + y_value));
                                //}
                                //else
                                //{
                                //    lb_test1.Text = countTime + "=>inside device ,xValue/HV1=" + hardwareValue1 + ",HV2=" + hardwareValue2 + ",yvalue=" + y_value;
                                //}

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
                            //else if (device_info_list[0].param1_info == "temp" && device_info_list[0].param2_info == "enthalpy")
                            //{
                            //    //--No data for now
                            //}
                            //else
                            //{
                            //    //First is humidity and second is enthalpy
                            //}

                        }
                        else
                        {
                            //offline mode
                            lb_device_status.Text = "disconnected";
                        }

                    }
                    else if (node.humidity_source == "Web")
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

                        double temperature_Val = node.xVal;//--New x value from device
                        double humidity_Val = double.Parse(hum_pulled);
                         double y_value = CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);
                        //  MessageBox.Show("temp / temp_pulled_form_web = "+temperature_Val+"\nhum = "+humidity_Val+"\ny value web= " + y_value);
                        //Now lets update the values in db
                        lock (menuStripNodeInfoValues)
                        {
                            //  UpdateNodeInfoToDB(node.id, temperature_Val, y_value, node.source, node.name, node.label, node.colorValue, node.showItemText, node.marker_Size);
                            // UpdateNodeInfoToDBForTemeperatureFromHardware(node.id, temperature_Val);//This is completed
                            UpdateNodeInfoToDBForHumidityFromHardware(node.id, y_value);
                        }
                        //==============THIS ONE IF FOR ONLINE OFFLINE==========
                        //  lb_web_status.Text = "active";                            
                        //=====================END OF ONLINE OFFLINE============           
                    } //Close of web



                    ///==========================================end of second parameter value==============================//

   

                    //else
                    //{
                    //    //Inserted by manual mode do nothing
                    //}



                }  //Close of foreach now lets plot the values..

            }
            catch(Exception ex)
            {
                //Show nothingelse
            }


            //now lets call the plot function...


            //--Commented for some time later uncomment-----//

            //if (InvokeRequired)
            //{
            //    Invoke(new MethodInvoker(RefreshGraph));
            //}
            //else
            //{
            //    RefreshGraph();
            //}
            //if (chartDetailList.Count > 0)
            //{
            //    int id = indexOfChartSelected;    //This value is changed 
            //    LoadNodeAndLineFromDB(id);   //Lets make it passing the stirngs 

            //    // flagForInsertOrUpdateDataToDB = 1;
            //    //--This is also completed..
            //    ReDrawingLineAndNode();


            //}
            //-----Comment for sometime close--------------//

            // }//--CLose of lock


            //--Now lets think differently and plot differently 

            /*Steps : 
            0. First find the path to the alex dabase for a building 
            1. Read the data form the alex db and pull to my db
            2. if there are sources of web in node then pull the web value :temperature and humidity
               store it in variable so that for new we do not have to pull again - again
            3.update the menustiripnodeinfovalue
            4.plot the data 
            */




            //======================================New code added nove 29th,2016====================================//

            ///*steps:
            //1.Reading data from nodetable
            //2.Updating in the list first 
            //3.updating in the db
            //4.plotting
            //*/
            //===================================================end of new code added ============================//




            //===========================Redraw the comfortzones as well =======================================//


            //MessageBox.Show("Here we are");
            //=====Uncomment lateer=============//
            //ReloadComfortZoneForBackGroundWorker();


            //==========================End of comfort zone redraw==============================================//







        } //Close of the fxn


        public void ReloadComfortZoneForBackGroundWorker()
        {
            if (listchartComfortZoneInfoSingle.Count > 0)
            {
                if(flagShow == 1)
                { 
                //Then we need to plot comfort zone 
                PlotComfortZoneForBackGroundWorker(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum), listchartComfortZoneInfoSingle[0].colorValue, listchartComfortZoneInfoSingle[0].name);

                }
                // flagShow = 1;//lets enable flag
                // MessageBox.Show("Enable flowshow =1");
            }
        }




        public class datatypeForT300InputTable
        {
            public string panelID { get; set; }
            public string inputIndex { get; set; }
            public string inputDescription { get; set; }
            public string inputAM { get; set; }
            public string inputValue { get; set; }
            public string inputUnit { get; set; }

            public string inputRange { get; set; }
            public string inputCalibration { get; set; }
            public string inputCalSign { get; set; }
            public string inputFilter { get; set; }
            public string inputDecon { get; set; }

            public string inputJumper { get; set; }
            public string inputLabel { get; set; }

            //public Color colorValue { get; set; }
        }

        public List<datatypeForT300InputTable> listForInputFromT3000 = new List<datatypeForT300InputTable>();

        public List<datatypeForT300InputTable> listForInputFromPsychoDB = new List<datatypeForT300InputTable>();




        public void PullingDataFromT3000BuildingDB(string databaseFileWithPath)
        {
            //this name databsefilewithpath contains the databse if empty the return
            if (databaseFileWithPath == "")
            {
                return;
            }

            string connString = @"Data Source=" + databaseFileWithPath + ";Version=3;";

            string TableName = "INPUTable";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + TableName + " WHERE InputUnit != '' AND InputRange != '@UnusedValue'";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@UnusedValue", "Unused");//This is the group id that is used to identify each node

                int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    //We need to filter temperature and humidity value
                    double tempVal = double.Parse(reader["InputValue"].ToString());
                    //--Filtering the value for temperature it should be between 0-50 
                    //--and for humidity (represented by % ) should be between 0-100
                    if ((reader["InputUnit"].ToString() == "Deg.C" && (tempVal >= 0 && tempVal <= 50)) || (reader["InputUnit"].ToString().Contains("%") && (tempVal >= 0 && tempVal <= 100)))
                    {
                          
                        // if (tempVal )
                        listForInputFromT3000.Add(new datatypeForT300InputTable
                        {
                            panelID = reader["PanelID"].ToString(),
                            inputIndex = reader["InputIndex"].ToString(),
                            inputDescription = reader["InputDescription"].ToString(),
                            inputAM = reader["InputAM"].ToString(),
                            inputValue = reader["InputValue"].ToString(),
                            inputUnit = reader["InputUnit"].ToString(),
                            inputRange = reader["InputRange"].ToString(),
                            inputCalibration = reader["InputCalibration"].ToString(),
                            inputCalSign = reader["InputCalSign"].ToString(),
                            inputFilter = reader["InputFilter"].ToString(),
                            inputDecon = reader["InputDecon"].ToString(),
                            inputJumper = reader["InputJumper"].ToString(),
                            inputLabel = reader["InputLabel"].ToString(),

                        });

                    }//--close of if
                } //--close of while
            }  //--Close of using
        }  //--Min fxn close




        public void PullingDataFromPsychrometricDB(string tableName)
        {
            //this name databsefilewithpath contains the databse if empty the return
            //if (databaseFileWithPath == "")
            //{
            //    return;
            //}

             string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
             string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

            //string databaseFileWithPath = "";//NEED TO FILL

            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            string TableName = tableName;
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                SQLiteDataReader reader = null;
                string queryString = "SELECT *  from " + TableName ;
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //command.Parameters.AddWithValue("@UnusedValue", "Unused");//This is the group id that is used to identify each node

                int count = 0;
                reader = command.ExecuteReader();
                while (reader.Read())
                {

                    //We need to filter temperature and humidity value
                    //  double tempVal = double.Parse(reader["InputValue"].ToString());
                    //--Filtering the value for temperature it should be between 0-50 
                    //--and for humidity (represented by % ) should be between 0-100
                    //    if ((reader["InputUnit"].ToString() == "Deg.C" && (tempVal >= 0 && tempVal <= 50)) || (reader["InputUnit"].ToString().Contains("%") && (tempVal >= 0 && tempVal <= 100)))
                    // {


                    // if (tempVal )
                    listForInputFromPsychoDB.Add(new datatypeForT300InputTable
                        {
                            panelID = reader["PanelID"].ToString(),
                            inputIndex = reader["InputIndex"].ToString(),
                            inputDescription = reader["InputDescription"].ToString(),
                            inputAM = reader["InputAM"].ToString(),
                            inputValue = reader["InputValue"].ToString(),
                            inputUnit = reader["InputUnit"].ToString(),
                            inputRange = reader["InputRange"].ToString(),
                            inputCalibration = reader["InputCalibration"].ToString(),
                            inputCalSign = reader["InputCalSign"].ToString(),
                            inputFilter = reader["InputFilter"].ToString(),
                            inputDecon = reader["InputDecon"].ToString(),
                            inputJumper = reader["InputJumper"].ToString(),
                            inputLabel = reader["InputLabel"].ToString(),

                        });

                   // }//--close of if
                } //--close of while
            }  //--Close of using
        }  //--Min fxn close



        public void InsertingInputDataOfT3000ToPsychroDB(string panelID, string inputIndex,string inputDesc,string inputAM,string inputValue,string inputUnit,string inputRange,string inputCalibration,string inputCalSign,string inputFilter,string inputDecon,string inputJumper,string inputLabel )
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableName + " ( PanelID ,InputIndex ,InputDescription,InputAM ,InputValue,InputUnit,InputRange ,InputCalibration ,InputFilter ,InputJumper ,InputLabel ) VALUES( @PanelID ,@InputIndex ,@InputDescription,@InputAM , @InputValue  ,@InputUnit,@InputRange ,@InputCalibration ,@InputFilter ,@InputJumper ,@InputLabel )";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@PanelID", panelID);
                command.Parameters.AddWithValue("@InputIndex", inputIndex);
                command.Parameters.AddWithValue("@InputDescription",inputDesc);
                command.Parameters.AddWithValue("@InputAM", inputAM);
                command.Parameters.AddWithValue("@InputValue", inputValue);
                command.Parameters.AddWithValue("@InputUnit", inputUnit);
                command.Parameters.AddWithValue("@InputRange", inputRange);                        
                command.Parameters.AddWithValue("@InputCalibration", inputCalibration);
                command.Parameters.AddWithValue("@InputFilter", inputFilter);
                command.Parameters.AddWithValue("@InputJumper", inputJumper);
                command.Parameters.AddWithValue("@InputLabel", inputLabel);
                command.ExecuteNonQuery();
            }
        }


        double hardwareTemperatureRead = 0;
        double hardwareHumidityRead = 0;

        double hardwareValue1 = 0;
        double hardwareValue2 = 0;

        /// <summary>
        /// helps to pull the data from the hardware of the device
        /// </summary>
        public void ReadDataFromDevice(int deviceID, uint temp_panID, uint hum_panID, string param1_identifier_type, string param2_identifier_type)
        {
            //lets do some operation regarding the pannel id and stuff

            //then perform this task 
            try
            {
                uint panID_1 = temp_panID;//0; //uint.Parse(panelID1);

                uint panID_2 = hum_panID;//1;//uint.Parse(panelID2);
                BACnetClass b = new BACnetClass();

                //for temperature value
                b.StartProgramForScanHardware(deviceID, panID_1, param1_identifier_type);
                double temperary1 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_temp_panel_value.Text = temp;
                //For humidity value
                b.StartProgramForScanHardware(deviceID, panID_2, param2_identifier_type);
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



        public void ReadDataFromDeviceForHumidity(int deviceID,uint hum_panID,  string param2_identifier_type)
        {
            //lets do some operation regarding the pannel id and stuff

            //then perform this task 
            try
            {
               // uint panID_1 = temp_panID;//0; //uint.Parse(panelID1);

                uint panID_2 = hum_panID;//1;//uint.Parse(panelID2);
                BACnetClass b = new BACnetClass();

                //for temperature value
               // b.StartProgramForScanHardware(deviceID, panID_1, param1_identifier_type);
               // double temperary1 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_temp_panel_value.Text = temp;
                //For humidity value
                b.StartProgramForScanHardware(deviceID, panID_2, param2_identifier_type);
                double temperary2 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_hum_panel_value.Text = humidity;
                //hardwareValue1 = temperary1;//--This value contains the temperature values
                hardwareValue2 = temperary2; //This one is humidity
                //lets store these two values in a temporary list

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }

        }

        public void ReadDataFromDeviceForTemperature(int deviceID, uint temp_panID,  string param1_identifier_type)
        {
            //lets do some operation regarding the pannel id and stuff

            //then perform this task 
            try
            {
                uint panID_1 = temp_panID;//0; //uint.Parse(panelID1);

               // uint panID_2 = hum_panID;//1;//uint.Parse(panelID2);
                BACnetClass b = new BACnetClass();

                //for temperature value
                b.StartProgramForScanHardware(deviceID, panID_1, param1_identifier_type);
                double temperary1 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_temp_panel_value.Text = temp;
                //For humidity value
              //  b.StartProgramForScanHardware(deviceID, panID_2, param2_identifier_type);
                //double temperary2 = double.Parse(b.PresentValueFromBacnet.ToString());
                //tb_hum_panel_value.Text = humidity;
                hardwareValue1 = temperary1;  //--This one is temperature 
               // hardwareValue2 = temperary2;
                //lets store these two values in a temporary list

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }

        }


 
        public double y_coord_value;

        public double CalculateYFromXandHumidity(double xVal, double humidity)
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

            double patm = AirPressureFromDB * 0.001;//101.325;//this is constant...
                                                    // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                                    //double w1 = 622*phi*pg/(patm-phi*pg);
            double w = phi * 622 * corres_pg_value / (patm - phi * corres_pg_value);//This is the y value


            //now lets calculate the dew point...
            //double humidity = phi;
            y_coord_value = w;
            return y_coord_value;

        }
    

        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%end of hardware section%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


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
            if (flagShow == 0)
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
                  //  LoadComfortZone(default_comfort_zone_of_chart[0].chartid);
                }

            }//close of else...

        }

        public void ShowOrHideForComfortZoneFromEditNode(int truefalseValue)
        {
            /*
            Note : This flogShow has just opposite property it si correct 
            1 means on before the chart load and while loading but when already on we dont need to on it again
            when click again when the intial state is on it should be off so 1 triggers off because the state is ON previously
            we need to make it OFF
            and IF the state is OFF initially 0 makes it ON the next time it is clicked 
            it is little confusing but true
            */
             if(truefalseValue == 1) { 
            if (flagShow == 0)
            {
                    //=================uncomment this is requred after========= //
                    //It is dissabled so enable it

                    //--Lets pull the comfort zone data first..
                    PullComfortZoneData();//This will load the listComfortZoneDetail

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
            }


            else
            {

                if(flagShow != 0) { 
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
                    //  LoadComfortZone(default_comfort_zone_of_chart[0].chartid);
                }
                }//Close of if

            }//close of else...

        }

        public void ClearSeriesInChart(string seriesName)
        {
            if(seriesName != "") { 
            Series s = new Series(seriesName);

            if (chart1.Series.IndexOf(s.Name) != -1)
            {
                //MessageBox.Show("Series exits");
                //--This  means the series is present....
                chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
            }
            }
        }


        //Comfrot zone setting after node edited

        /// <summary>
        /// Clears the comfortzone that was created based on the 
        /// min and max temperature and min and max humidity
        /// </summary>
        /// <param name="Tmin">minimum temperature</param>
        /// <param name="Tmax">maximum temperature</param>
        /// <param name="Hmin">minimum humidity</param>
        /// <param name="Hmax">maximum humidity</param>
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



        /// <summary>
        /// Some vairbale  to store the humidity tmeperature and wind speed pulled form the weather services..
        /// </summary>

        //==These variables are being accessed form different forms
        public double lat_val = 0.0000;
        public double lng_val = 0.0000;
        //lets declare some variable to store the value...
        public string city_name_pulled = "";
        public string country_name_pulled = "";
        public string last_update_pulled = "";
        public string temp_pulled = "";
        public string hum_pulled = "";
        public string pressure_pulled = "";
        public string wind_speed_pulled = "";
        public string direction_pulled = "";
        public string lat_pulled = "";
        public string long_pulled = "";
        public double temp_pulled_from_web = 0.000;
        /// <summary>
        /// This return the different values such as temperatue ,humidity and other parameters.
        /// </summary>
        /// <param name="latValue">Latitude </param>
        /// <param name="longValue">Longitude</param>


        public void GetDataFromWeb(double latValue, double longValue)
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
                using (var wc = new WebClient())
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
                return;//also return from the current process

            }


        } //Close of the main function 


        public double latValueFromDB = 0.0000;
        public double longValueFromDB = 0.0000;
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
            bool status = false;
            if (BuildingSelected.Count > 0)
            {
                /*
                This path is dynamic path we calculated based on this 
                <installed directory of T3000 (not psycho)>\... where ... = Database\..value returned by files
                */
                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + "" + BuildingSelected[0].building_path;

                try
                {
                    using (SQLiteConnection conn = new SQLiteConnection(againDbPath))
                    {
                        using (SQLiteCommand cmd = new SQLiteCommand(sql, conn))
                        {
                            conn.Open();
                            SQLiteDataReader reader = cmd.ExecuteReader();
                            while (reader.Read())
                            {
                                dataCheckList.Add((bool)reader["Online_Status"]);
                            }

                        }

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
             //MessageBox.Show( "datacheckcount= " + dataCheckList.Count + " Status = " + status);
            }
            return status;
        }

 
        // int markerSize = 20; //This is used to make marker of node change...
       public int lineBorderThickness = 15;//This is used to change the thickness of the line....

        /// <summary>
        /// This create a node in the chart
        /// </summary>
        /// <param name="source">what is the source that the data is comming from</param>
        /// <param name="name">what is the name of the data</param>
        /// <param name="label">What label it has</param>
        /// <param name="c1">color it should have</param>
        /// <param name="comboboxItemText1">which items to be displayed</param>
        /// This one is for the Web
       public  string medium_device_or_web_value = "";
        public void SetNodeForWeb(string source, string name, string label, Color c1, string comboboxItemText1, int marker_size)
        {

            markerSize = marker_size; //This is for changing the marker size 
            tbSource = source;
            tbName = name;
            tbLabel = label;
            colorValue = c1;
            comboboxItemText = comboboxItemText1;

            //Temp and humidity value 
            double temp1Val = 0.000;
            double hum1Val = 0.00;
            //web choice is selected.
            string table_name = "tbl_building_location";
            PullLongitudeAndLatitude(table_name);

            //Now lets pass the value to weather api
            GetDataFromWeb(latValueFromDB, longValueFromDB);//This gets the temperature and humidity

            temp1Val = temp_pulled_from_web;
            hum1Val = double.Parse(hum_pulled);
         
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
        public void UpdateLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, Series lineSeriesVal, int lineThicknessValue,string line_name ,int line_status)
        {
            string lineSeriesName = lineSeriesVal.Name;
            //string tableName = currentLineTableFromDB;
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "update " + tableName + " set prevNodeId=@pn,nextNodeId=@nn,lineColorValue=@lc,lineSeriesId=@ls ,thickness = @linethickness ,name = @lnName,status = @lnStatus where   lineID=@id ";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@linethickness", lineThicknessValue);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@lnName", line_name);
                command.Parameters.AddWithValue("@lnStatus", line_status);
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

        //public class nodeDataType

        //This list holds the detail of the chart to be deleted
      public  List<TempDataType> deleteNodeDetailList = new List<TempDataType>();

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
                string queryString = "SELECT *  from  " + tableName + "  where chart_respective_nodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartNodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    deleteNodeDetailList.Add(new TempDataType
                    {
                        //count = int.Parse(reader["count"].ToString()),
                        //chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        //nodeID = reader["nodeID"].ToString(),
                        //xValue = double.Parse(reader["xValue"].ToString()),
                        //yValue = double.Parse(reader["yValue"].ToString()),
                        //source = reader["source"].ToString(),
                        //name = reader["name"].ToString(),
                        //label = reader["label"].ToString(),
                        //colorValue = reader["colorValue"].ToString(),
                        //showTextItem = reader["showTextItem"].ToString(),
                        //nodeSize = int.Parse(reader["nodeSize"].ToString())
                        id = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        temperature_source = reader["temperature_source"].ToString(),
                        humidity_source = reader["humidity_source"].ToString(),
                        name = reader["name"].ToString(),

                        // label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        // showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString()),
                        airFlow = int.Parse(reader["airFlow"].ToString()),
                        lastUpdatedDate = reader["lastUpdatedDate"].ToString()


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

        public void DeleteMixNodeInfo(string chartID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";// "tbl_" ++"_node_value";
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



        public void DisableChart(string chartID)
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
                string queryString = " UPDATE " + tableName + "  set enableChartStatus = @status    where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", "false");
                command.Parameters.AddWithValue("@id_value", chartID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using          

        }

        /// <summary>
        /// Helps to update the temperature source info in to database
        /// </summary>
        /// <param name="nodeID">node id </param>
        /// <param name="TemperatureSourceValue">Temepraturesource value like 1-var1, var2, label1 etc</param>
        public void DB_TemperatureSourceUpdate(string nodeID,string TemperatureSourceValue)
        {
            /*
           //This function helps to update the temperature source detail such as 1-VAR1, 2-var2 etc.
           or even label name from alex db
            */

           
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set TemperatureSourceInfo = @status    where NodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", TemperatureSourceValue);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();

            }//Close of using          

        }

        public void UpdateOrInsertTemperatureSourceInfo(string nodeID, string TemperaturSourceInfo)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";


            bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    dataPresent = true;
                }

            }

            //Data is present then update else insert
            if(dataPresent == true)
            {
                DB_TemperatureSourceUpdate(nodeID, TemperaturSourceInfo);
            }else
            {
                InsertValueOfTemperatureHumiditySoure(nodeID, TemperaturSourceInfo,"");
            }


        }

        public void InsertValueOfTemperatureHumiditySoure(string nodeID, string TemperatureSource,string HumiditySource)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into " + tableForTempHum + "(NodeID,chartID,TemperatureSourceInfo,HumiditySourceInfo) VALUES(@id,@chartID,@t,@h)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@id", nodeID);
                command.Parameters.AddWithValue("@chartID", chartDetailList[indexForWhichChartIsSelected].chartID);
                command.Parameters.AddWithValue("@t", TemperatureSource);
                command.Parameters.AddWithValue("@h", HumiditySource);
                command.ExecuteNonQuery();
            }
        }

        public void UpdateOrInsertHumiditySourceInfo(string nodeID, string HumiditySourceInfo)
        {
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";


            bool dataPresent = false;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    dataPresent = true;
                }

            }

            //Data is present then update else insert
            if (dataPresent == true)
            {
                DB_HumiditySourceUpdate(nodeID, HumiditySourceInfo);
            }
            else
            {
                InsertValueOfTemperatureHumiditySoure(nodeID, "", HumiditySourceInfo);
            }


        }



        /// <summary>
        /// Function helps to update the humidity source value into the database
        /// </summary>
        /// <param name="nodeID"></param>
        /// <param name="HumiditySourceValue"></param>
        public void DB_HumiditySourceUpdate(string nodeID, string HumiditySourceValue)
        {
            /*
           //This function helps to update the temperature source detail such as 1-VAR1, 2-var2 etc.
           or even label name from alex db
            */


            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = " UPDATE " + tableName + "  set HumiditySourceInfo = @status    where NodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@status", HumiditySourceValue);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();
            }//Close of using          

        }

        public class TemperatureHumiditySourceInfo
        {
            public string chartID { get; set; }
            public string nodeID { get; set; }
            public string TemepratureSoureString { get; set; }
            public string HumiditySourceString { get; set; }
        }
        public List<TemperatureHumiditySourceInfo> listTempHumSourceInfo = new List<TemperatureHumiditySourceInfo>();

        public void PullDataForTemperatureHumiditySource(string nodeID)
        {

            listTempHumSourceInfo.Clear();
            string tableForTempHum = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                // tables and also we need to all the chart of single building in a s single table .
                string sql = "select * from " + tableForTempHum + " where nodeID = '" + nodeID + "'";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    listTempHumSourceInfo.Add(new TemperatureHumiditySourceInfo
                    {
                        chartID = reader["chartID"].ToString(),
                        nodeID = nodeID,
                        TemepratureSoureString = reader["TemperatureSourceInfo"].ToString(),
                        HumiditySourceString = reader["HumiditySourceInfo"].ToString(),

                    });
                  
                }

            }//Close of using statement
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

        public List<dataTypeForCF> listComfortZoneDetail = new List<dataTypeForCF>();
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

        /// <summary>
        /// Pulls the comfortzone parameters such as minTemp,maxTemp,minHum ,maxHum and colorValue
        /// </summary>
        /// <param name="comfortzoneid"></param>
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
                string sql = "select * from " + tableForComfortZoneDetail + " where id='" + comfortzoneid + "'";
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

        /// <summary>
        /// This counts the number of items in ...chart_comfrotzone_setting
        /// is there any comfortzone item associated with a chart or not
        /// </summary>
        /// <param name="chartId"></param>
        /// <returns></returns>
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
                string sql = "SELECT COUNT(1) FROM " + tableName + " WHERE chartID='" + chartId + "'";
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

        public class datatype_for_comfortzone
        {
            public string chartid { get; set; }
            public string comfortzoneid { get; set; }
            public string status { get; set; }
        }
        public List<datatype_for_comfortzone> default_comfort_zone_of_chart = new List<datatype_for_comfortzone>();
        /// <summary>
        /// This gets the information of comfortzone such as status ,
        /// comfortzoneid, chartid etc
        /// </summary>
        /// <param name="chartID"></param>
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
                        chartid = chartID,//reader["chartID"].ToString(),
                        comfortzoneid = reader["comfort_zone_ID"].ToString(),
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
        public void update_comfort_zone_for_chart(string chartid, string comfortzone_id, string status)
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


        public void insertOrUpdateComfortChartSetting(string chartID, string comfortzone_ID)
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
                    update_comfort_zone_for_chart(chartID, comfortzone_ID, status);
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
                s.BorderWidth = 5;
                chart1.Series.Add(s);
            }

            //Now lets do the actual plotting part.


            double phi_min = Hmin / 100;//need to change to decimal
            double phi_max = Hmax / 100;


            //This one is for adding horizontal lines 
            double phi = phi_min; //0.1;            
            double x2 = 0;
            // int ival = 2;
            //int indexValue = (int)Hmin;
            double indexValue = Hmin;
            double patm = AirPressureFromDB * 0.001;//101.325; ;//thsi need tochange
            for (phi = phi_min; phi <= phi_max; phi += 0.01) //increment by 2 value
                                                             // for (phi = phi_min; phi <= phi_max; phi += 0.0025) //increment by 2 value
            {
                for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)
                //for (double temp = Tmin; temp <Tmax;temp+=0.25)
                {

                    double pg_value = Double.Parse(pg[temp].ToString());
                    double wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
                    double y = wg_calc;
                    x2 = temp;//double.Parse(t[i].ToString());
                    chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y);

                }//close of for
                 //MessageBox.Show(s1);
                indexValue += 1;
                //indexValue += 0.25;
            }  //close fo the second for loop


            //Now this one is for adding vertical line for lines
            for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)
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

                chart1.Series["vertical_temp" + temp].Points.AddXY(xx1, yy1);
                chart1.Series["vertical_temp" + temp].Points.AddXY(xx2, yy2);

            }//close of for                                                 

        } //Close of our function


        public void PlotComfortZoneForBackGroundWorker(double Tmin, double Tmax, double Hmin, double Hmax, Color c, string name)
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
                    if (chart1.InvokeRequired)
                    {
                        chart1.Invoke(new Action(() => chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name))));
                    }
                    else
                    {
                        //lb_device_status.Text = "connected";
                        //  series1.Points.Clear();
                        chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                    }
                    //chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
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
                    if (chart1.InvokeRequired)
                    {
                        chart1.Invoke(new Action(() => chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name))));
                    }
                    else
                    {
                        //lb_device_status.Text = "connected";
                        //  series1.Points.Clear();
                        chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
                    }
                    //chart1.Series.RemoveAt(chart1.Series.IndexOf(s.Name));
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
                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() => chart1.Series.Add(s)));
                }
                else
                {
                    chart1.Series.Add(s);
                }
                //chart1.Series.Add(s);
            }

            //Refresh  the chart now

            if (chart1.InvokeRequired)
            {
                chart1.Invoke(new Action(() => chart1.Invalidate()));
            }
            else
            {
                //chart1.Series.Add(s);
                chart1.Invalidate();
            }
            //chart1.Invalidate();

            if (chart1.InvokeRequired)
            {
                chart1.Invoke(new Action(() => chart1.Refresh()));
            }
            else
            {
                chart1.Refresh();
            }
            //chart1.Refresh();//This is for refresh

            for (int i = (int)Hmin; i <= (int)Hmax; i += 1)
            {
                //    for (double i = Hmin; i <= Hmax; i += 0.25)
                // {
                //add now
                Series s = new Series("horizontal_hum" + i);
                s.Color = c;
                s.ChartType = SeriesChartType.Spline;
                //s.MarkerSize = 15;
                s.BorderWidth = 5;
                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() => chart1.Series.Add(s)));
                }
                else
                {
                    chart1.Series.Add(s);
                }
                //chart1.Series.Add(s);
            }

            //Now lets do the actual plotting part.


            double phi_min = Hmin / 100;//need to change to decimal
            double phi_max = Hmax / 100;


            //This one is for adding horizontal lines 
            double phi = phi_min; //0.1;            
            double x2 = 0;
            // int ival = 2;
            //int indexValue = (int)Hmin;
            double indexValue = Hmin;
            double patm = AirPressureFromDB * 0.001;//101.325; ;//thsi need tochange
            for (phi = phi_min; phi <= phi_max; phi += 0.01) //increment by 2 value
                                                             // for (phi = phi_min; phi <= phi_max; phi += 0.0025) //increment by 2 value
            {
                for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)
                //for (double temp = Tmin; temp <Tmax;temp+=0.25)
                {

                    double pg_value = Double.Parse(pg[temp].ToString());
                    double wg_calc = (622 * phi * pg_value / (patm - phi * pg_value));
                    double y = wg_calc;
                    x2 = temp;//double.Parse(t[i].ToString());

                    if (chart1.InvokeRequired)
                    {
                        chart1.Invoke(new Action(() => chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y)));
                    }
                    else
                    {
                        // chart1.Series.Add(s);
                        chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y);
                    }
                    //chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y);

                }//close of for
                 //MessageBox.Show(s1);
                indexValue += 1;
                //indexValue += 0.25;
            }  //close fo the second for loop


            //Now this one is for adding vertical line for lines
            for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)
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

                // chart1.Series["vertical_temp" + temp].Points.AddXY(xx1, yy1);
                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() => chart1.Series["vertical_temp" + temp].Points.AddXY(xx1, yy1)));
                }
                else
                {
                    // chart1.Series.Add(s);
                    //chart1.Series["horizontal_hum" + indexValue].Points.AddXY(x2, y);
                    chart1.Series["vertical_temp" + temp].Points.AddXY(xx1, yy1);
                }

                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() => chart1.Series["vertical_temp" + temp].Points.AddXY(xx2, yy2)));
                }
                else
                {
                    chart1.Series["vertical_temp" + temp].Points.AddXY(xx2, yy2);
                }
                //chart1.Series["vertical_temp" + temp].Points.AddXY(xx2, yy2);

            }//close of for                                                 

        } //Close of our function





        /// <summary>
        /// This variable store comfort zone id which is used for updating comfortzone info
        /// </summary>
        string temporaryComfortZoneID_Store = "";

        /// <summary>
        /// This function helps to insert the comfort zone setting in the database.
        /// </summary>
        /// <param name="name">name of the comfort zone</param>
        /// <param name="min_temp">min temperature</param>
        /// <param name="max_temp">max temperature</param>
        /// <param name="min_hum">min humidity</param>
        /// <param name="max_hum">max humidity</param>
        /// <param name="color">color value</param>
        public void InsertComfortZoneValue(string name, double min_temp, double max_temp, double min_hum, double max_hum, Color color)
        {

            string idValue = GetGUID(); //insert new id value
            temporaryComfortZoneID_Store = idValue;
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


        public void UpdateComfortZoneValue(string id, string name, double min_temp, double max_temp, double min_hum, double max_hum, Color color)
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

        public class SelectedBuildingDatatype
        {
            public string Main_BuildingName { get; set; }
            public string Building_Name { get; set; }
            //public string Building_Path { get; set; }


            //public string protocol { get; set; }
            //public string com_port { get; set; }
            //public string ip_address { get; set; }

            //public string ip_port { get; set; }
            //public string braudrate { get; set; }



            public string default_subBuilding { get; set; }
            public string building_path { get; set; } //Here is the building Path we need
            public string longitude { get; set; }
            public string latitude { get; set; }

            public string elevation { get; set; }
            public string ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
            //public string ZIP { get; set; }
            public string EngineeringUnits { get; set; }
            
            

        }
        public  List<SelectedBuildingDatatype> BuildingSelected = new List<SelectedBuildingDatatype>();

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
                // string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.db";
                string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db";
                // MessageBox.Show("New path : " + againDbPath);
                // bool returnValue = false;
                //string latValue = "";
                using (OleDbConnection connection = new OleDbConnection(againDbPath))
                {
                    connection.Open();
                    OleDbDataReader reader = null;
                    string queryString = "SELECT * from Building WHERE Default_SubBuilding = '1' ";//-1 or True  can be used//New changed to 1 value
                    OleDbCommand command = new OleDbCommand(queryString, connection);

                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {

                        BuildingSelected.Add(new SelectedBuildingDatatype
                        {
                            Main_BuildingName = reader["Main_BuildingName"].ToString(),
                            Building_Name = reader["Building_Name"].ToString(),
                          
                            //protocol = reader["Protocol"].ToString(),
                            //com_port = reader["Com_Port"].ToString(),
                            //ip_address = reader["Ip_Address"].ToString(),
                            //ip_port = reader["Ip_Port"].ToString(),
                            //braudrate = reader["Braudrate"].ToString(),
                            default_subBuilding = reader["Default_SubBuilding"].ToString(),
                            building_path = reader["Building_Path"].ToString(),
                            longitude = reader["Longitude"].ToString(),
                            latitude = reader["Latitude"].ToString(),
                            elevation = reader["Elevation"].ToString(),
                            ID = reader["ID"].ToString(),
                            country = reader["country"].ToString(),
                            state  = reader["state"].ToString(),
                            city = reader["city"].ToString(),
                            street = reader["street"].ToString(),
                           // ZIP  = reader["ZIP"].ToString(),
                            EngineeringUnits= reader["EngineeringUnits"].ToString(),


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

        public void FindPathOfBuildingDBNewVersion()
        {
            //try
            //{
                BuildingSelected.Clear();
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                //  string connString = @"Data Source=" + databaseFile + ";Version=3;";

                string path = databasePath;  //@"C:\Folder1\Folder2\Folder3\Folder4";
                string newPath = Path.GetFullPath(Path.Combine(path, @"..\"));
                // string againDbPath = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + newPath + @"Database\T3000.db";
                string againDbPath = @"Data Source=" + newPath + @"Database\T3000.db";

                 //MessageBox.Show("New path : " + againDbPath);
                // bool returnValue = false;
                //string latValue = "";
                using (SQLiteConnection connection = new SQLiteConnection(againDbPath))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    string queryString = "SELECT * from Building WHERE Default_SubBuilding = '1' ";//-1 or True  can be used//New changed to 1 value
                    SQLiteCommand command = new SQLiteCommand(queryString, connection);

                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {

                        BuildingSelected.Add(new SelectedBuildingDatatype
                        {
                            Main_BuildingName = reader["Main_BuildingName"].ToString(),
                            Building_Name = reader["Building_Name"].ToString(),

                            //protocol = reader["Protocol"].ToString(),
                            //com_port = reader["Com_Port"].ToString(),
                            //ip_address = reader["Ip_Address"].ToString(),
                            //ip_port = reader["Ip_Port"].ToString(),
                            //braudrate = reader["Braudrate"].ToString(),
                            default_subBuilding = reader["Default_SubBuilding"].ToString(),
                            building_path = reader["Building_Path"].ToString(),
                            longitude = reader["Longitude"].ToString(),
                            latitude = reader["Latitude"].ToString(),
                            elevation = reader["Elevation"].ToString(),
                            ID = reader["ID"].ToString(),
                            country = reader["country"].ToString(),
                            state = reader["state"].ToString(),
                            city = reader["city"].ToString(),
                            street = reader["street"].ToString(),
                            //ZIP = reader["ZIP"].ToString(),
                            EngineeringUnits = reader["EngineeringUnits"].ToString()

                            //Main_BuildingName = reader["1"].ToString(),
                            //Building_Name = reader["2"].ToString(),

                            //protocol = reader["3"].ToString(),
                            //com_port = reader["4"].ToString(),
                            //ip_address = reader["5"].ToString(),
                            //ip_port = reader["6"].ToString(),
                            //braudrate = reader["7"].ToString(),
                            //default_subBuilding = reader["8"].ToString(),
                            //building_path = reader["9"].ToString(),
                            //longitude = reader["10"].ToString(),
                            //latitude = reader["11"].ToString(),
                            //elevation = reader["12"].ToString(),
                            //ID = reader["13"].ToString(),
                            //country = reader["14"].ToString(),
                            //state = reader["15"].ToString(),
                            //city = reader["16"].ToString(),
                            //street = reader["17"].ToString(),
                            //ZIP = reader["18"].ToString(),
                            //EngineeringUnits = reader["19"].ToString()



                        });
                    }
                }


            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
            //MessageBox.Show("count = " + BuildingSelected.Count);
        }


        public void CreateRequireTableIfNotPresent(string buildingName)
        {
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string tableForNode = "tbl_" + buildingName + "_node_value";
            string tableForLine = "tbl_" + buildingName + "_line_value";
            string tableFordevice = "tbl_" + buildingName + "_device_info_for_node";
            string tableForInputStorageFromT3000 = "tbl_" + buildingName + "_input_storage_from_T3000";
            string tableForValuesFromT3000 = "tbl_" + buildingName + "_node_data_related_T3000";
            string tableMixNode = "tbl_" + buildingName + "_mix_node_info";


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
                //--MODIFIED by bbk: for bulling data from alex db (2016-nov-10/thrus/10:06AM)
                // string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255))";

                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                //--Modified to new: This one was intial query
                //string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),source varchar(255),name varchar(255),label varchar(255),colorValue varchar(255),showTextItem varchar(255),nodeSize varchar(255))";
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                //--Modified by bbk: for pulling information from the device
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                //string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id_from_nodeID varchar(255) ,panelID varchar(255),inputIndex varchar(255),inputDescription varchar(255),inputAM varchar(255), inputValue  varchar(255) ,inputUnit varchar(255),inputRange varchar(255),inputCalibration varchar(255),inputFilter varchar(255),inputJumper varchar(255),inputLabel varchar(255))";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();


                //gives the info about the which value From T3000 is associated with nodes in psychometric
                // string sql_For_nodeInfoFromT3000 = "create table IF NOT EXISTS  " +  + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID varchar(255),IP varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                //string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id_from_nodeID varchar(255) ,panelID varchar(255),inputIndex varchar(255),inputDescription varchar(255),inputAM varchar(255), inputValue  varchar(255) ,inputUnit varchar(255),inputRange varchar(255),inputCalibration varchar(255),inputFilter varchar(255),inputJumper varchar(255),inputLabel varchar(255))";
                string sql_For_nodeInfoFromT3000 = "create table IF NOT EXISTS  " + tableForValuesFromT3000 + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,param1_panelID varchar(255),param1_inputIndex varchar(255),param2_panelID varchar(255),param2_inputIndex varchar(255))";
                SQLiteCommand command_T3000 = new SQLiteCommand(sql_For_nodeInfoFromT3000, connection);
                command_T3000.ExecuteNonQuery();


                //These tables are for comfort zone...
                string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                command6.ExecuteNonQuery();

                //--Comfortzonetable
                string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                command7.ExecuteNonQuery();


                //-- NOW table for storing values tableForInputStorageFromT3000
                string sql8 = "create table IF NOT EXISTS  " + tableForInputStorageFromT3000 + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


                //tableMixNode
                string sql9 = "create table IF NOT EXISTS  " + tableMixNode + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                SQLiteCommand command9 = new SQLiteCommand(sql9, connection);
                command9.ExecuteNonQuery();



            }

        }

        public class chartDetailDT
        {
            public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
            public string enableChartStatus { get; set; }
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

                string sql = "select * from " + tableForChartDetail + " where enableChartStatus ='true' ";

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
                        chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                        enableChartStatus = reader["enableChartStatus"].ToString() //either true or false

                    });
                }
            }

        }  //Close of the function  PullChartList

        /// <summary>
        /// it helps to fill the datagridview with its item form db
        /// </summary>
        private void fillDataGridView()
        {

            //dataGridView1.Rows.Clear();
            int xCount = 0;
            if (chartDetailList.Count > 0)
            {
                for (int i = 0; i < chartDetailList.Count; i++)
                {
                    if(chartDetailList[i].enableChartStatus == "true") //when enable then only display
                    { 
                    dataGridView1.Rows.Add();
                    dataGridView1.Rows[i].Cells["ID"].Value = ++xCount;//chartDetailList[i].;
                    dataGridView1.Rows[i].Cells["Name"].Value = chartDetailList[i].chartName;
                    }
                }
                //dataGridView1.DataBindingComplete += new DataGridViewBindingCompleteEventHandler(dataGridView1.DataBindingComplete);
                
            }//--if close
            //dataGridView1.ClearSelection();

            lb_numberOfHandler.Text = chartDetailList.Count.ToString();


        }//--close fo filldatagridview


        public void DataGridView_Show_Data()
        {
            //check to see if the data is present in the database or not...
            //--Pulls and fills in a dynamic list
            //   PullDataFromDB();
            //dataGridView1.ClearSelection();//--this one is for clearing the selection
            if (selectedBuildingList.Count <= 0)
            {
                //We no need to proceed futher 
                return;
            }
            //Pulling the data form the data grid view...
             PullChartList(selectedBuildingList[0].BuildingName);//This is the list of the building present ...
            //PullChartList(CurrentSelectedBuilding);//This is the list of the building present ...

            //MessageBox.Show("datapulled ,count =  " + listForDataFromDB.Count);

            //This fills the datagridview
            fillDataGridView();
            //     fillDataGridView_WithChartList();
            //DataGridView_AutoSelectSuppressed fx = new DataGridView_AutoSelectSuppressed();
            //fx.ClearSelectionAndResetSuppression();
            //dataGridView1.ClearSelection();
            //dataGridView1.SelectionChanged -= new EventHandler(dataGridView1.SelectionChanged);

        }



        /// <summary>
        /// Declearing the variable for storing the selected building information
        /// </summary>
        public class SelectedBuildingDT
        {
            public int ID { get; set; }
            public string country { get; set; }

            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
          //  public int ZIP { get; set; }
            public double longitude { get; set; }
            public double latitude { get; set; }
            public double elevation { get; set; }
            public string BuildingName { get; set; }  //This one is what we are interested in ....
        }
      public List<SelectedBuildingDT> selectedBuildingList = new List<SelectedBuildingDT>();
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
                       // ZIP = int.Parse(reader["ZIP"].ToString()),  //--tHIS HAS BEEN REMOVED
                        longitude = double.Parse(reader["longitude"].ToString()),
                        latitude = double.Parse(reader["latitude"].ToString()),
                        elevation = double.Parse(reader["elevation"].ToString()),
                        BuildingName = reader["BuildingName"].ToString()
                    });
                }
            }//close of using..
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
            string enableChartStatusVar = "true";   //--Firs first time insertion its always true
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                //--Modified this value by bbk : for pulling data form alex db
                //string sql_string = "insert into  " + tableNameOfChart + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID) VALUES(@chartID_value,@nodeName_value,@nodeRespectiveID,@lineRespectiveID)";
                string sql_string = "insert into  " + tableNameOfChart + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@chartID_value,@nodeName_value,@nodeRespectiveID,@lineRespectiveID,@enableChartStatus1)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartID_value", chartIDValue);
                command.Parameters.AddWithValue("@nodeName_value", name);
                command.Parameters.AddWithValue("@nodeRespectiveID", chart_resp_nodes_group_ID);
                command.Parameters.AddWithValue("@lineRespectiveID", chart_resp_lines_group_ID);
                command.Parameters.AddWithValue("@enableChartStatus1", enableChartStatusVar);

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
            if(name == "")
            {
                //If name is empty the do not update
                return;
            }

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
                string chartIDValue = chartDetailList[e.RowIndex].chartID;
                string nameValue = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                UpdateNewDataFromDGV(nameValue, chartIDValue);

            }
        }

        public void chart1RefreshCustom()
        {
            this.Invalidate();
            chart1.Invalidate();
            plot_new_graph();
        }

        public void RefreshGraph()
        {
           // lock (this) { 
            //try
            //{
                this.Invalidate();
                chart1.Invalidate();
                // chart1.Dispose();//--Releases all the resources used by the chart...
                plot_new_graph();

                //--Reseting the menustrip values for new plotting....
                menuStripNodeLineInfoValues.Clear();
                menuStripNodeInfoValues.Clear();
                index = 0;  //This is resetting the index values
                incrementIndex = 0;
            ReloadComfortZoneForBackGroundWorker();
            // }
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
            // }//--Close of lock
        }

        public void InsertNodeInfoToDB(string id, double xVal, double yVal, string source, string name, string label, Color colorValue, string showItemText, int nodeSizeValue, string deviceinstance, string deviceip, string param1id, string param2id, string param1info, string param2info, string param1type, string param2type)
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

            if (source == "Device")
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


        public void InsertIntoDeviceInfoDB(string nodeid,string deviceinstance,string deviceip,string param1id,string param2id,string param1info,string param2info,string param1type,string param2type)
        {                                 
                                                
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";            
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                                                                               
            //checking if the data is present or not
            SQLiteDataReader reader = null;
            string queryString = "SELECT *  from " + tableNameDevice + " WHERE nodeID = @id";

            bool flag = false;
            using (SQLiteConnection cxn = new SQLiteConnection(connString))
            {
                cxn.Open();
                SQLiteCommand command = new SQLiteCommand(queryString, cxn);

                command.Parameters.AddWithValue("@id", nodeid);//This is the group id that is used to identify each node
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    if (reader["nodeID"].ToString() != "")
                    {
                        flag = true;
                    }
                }   //Close of while

            } //Close of using



            if (flag == false)
            { 
                //insert the value 

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                  //  MessageBox.Show("Insert the node value flag = false wala and id  = "+nodeid);
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID,IP,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID,@IP,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", deviceinstance);
                    command.Parameters.AddWithValue("@IP", deviceip);
                    command.Parameters.AddWithValue("@param1", param1id);
                    command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param1info", param1info);
                    command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", param1type);
                    command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }
            }
            else
            {
                //Update the value
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                  //  MessageBox.Show("Update mechanism the node value flag = true  wala,id = "+nodeid);

                    string sql_string = "UPDATE  " + tableNameDevice + "  SET device_instanceID = @instanceID,IP =@IP ,param1ID = @param1 ,param2ID = @param2,param1_info = @param1info,param2_info = @param2info,param1_identifier_type= @param1_iden_type,param2_identifier_type= @param2_iden_type where nodeID = @id ";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                     command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", deviceinstance);
                    command.Parameters.AddWithValue("@IP", deviceip);
                    command.Parameters.AddWithValue("@param1", param1id);
                    command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param1info", param1info);
                    command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", param1type);
                    command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }

            }


        }




        public void InsertUpdateFromTemperatureDeviceSelectionToDBOnlyTempertureUpdate(string nodeid, string temp_deviceInstanceID, string temp_ip, string temp_param1id, string temp_param1info, string temp_param1type)
        {

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 

            //checking if the data is present or not
            SQLiteDataReader reader = null;
            string queryString = "SELECT *  from " + tableNameDevice + " WHERE nodeID = @id";

            bool flag = false;
            using (SQLiteConnection cxn = new SQLiteConnection(connString))
            {
                cxn.Open();
                SQLiteCommand command = new SQLiteCommand(queryString, cxn);

                command.Parameters.AddWithValue("@id", nodeid);//This is the group id that is used to identify each node
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    if (reader["nodeID"].ToString() != "")
                    {
                        flag = true;
                    }
                }   //Close of while

            } //Close of using



            if (flag == false)
            {
                //insert the value 

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Insert the node value flag = false wala and id  = "+nodeid);
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID_for_param1,device_instanceID_for_param2,IP_for_param1,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceIDparam1,@deviceInstanceParam2,@IP_param1,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceIDparam1", temp_deviceInstanceID);
                    command.Parameters.AddWithValue("@deviceInstanceParam2", "");//Insert empty value

                    command.Parameters.AddWithValue("@IP_param1", temp_ip);
                    command.Parameters.AddWithValue("@IP_param2", "");//Insert empty value

                    command.Parameters.AddWithValue("@param1", temp_param1id);
                    command.Parameters.AddWithValue("@param2", "");
                    command.Parameters.AddWithValue("@param1info", temp_param1info);
                    command.Parameters.AddWithValue("@param2info", "");
                    command.Parameters.AddWithValue("@param1_iden_type", temp_param1type);
                    command.Parameters.AddWithValue("@param2_iden_type", "");
                    command.ExecuteNonQuery();
                }
            }
            else
            {
                //Update the value
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Update mechanism the node value flag = true  wala,id = "+nodeid);

                    string sql_string = "UPDATE  " + tableNameDevice + "  SET device_instanceID_for_param1 = @instanceID,IP_for_param1 =@IP ,param1ID = @param1 ,param1_info = @param1info,param1_identifier_type= @param1_iden_type  where nodeID = @id ";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", temp_deviceInstanceID);
                    command.Parameters.AddWithValue("@IP", temp_ip);
                    command.Parameters.AddWithValue("@param1", temp_param1id);
                   // command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param1info", temp_param1info);
                  //  command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param1_iden_type", temp_param1type);
                   // command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }

            }


        }


                     /// <summary>
                     /// For humidity values
                     /// </summary>
                     /// <param name="nodeid"></param>
                     /// <param name="Humidity_deviceInstanceID"></param>
                     /// <param name="Humidity_ip"></param>
                     /// <param name="Humidity_param1id"></param>
                     /// <param name="Humidity_param1info"></param>
                     /// <param name="Humidity_param1type"></param>


        public void InsertUpdateFromHumidityDeviceSelectionToDBOnlyTempertureUpdate(string nodeid, string Humidity_deviceInstanceID, string Humidity_ip, string Humidity_param1id, string Humidity_param1info, string Humidity_param1type)
        {

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 

            //checking if the data is present or not
            SQLiteDataReader reader = null;
            string queryString = "SELECT *  from " + tableNameDevice + " WHERE nodeID = @id";

            bool flag = false;
            using (SQLiteConnection cxn = new SQLiteConnection(connString))
            {
                cxn.Open();
                SQLiteCommand command = new SQLiteCommand(queryString, cxn);

                command.Parameters.AddWithValue("@id", nodeid);//This is the group id that is used to identify each node
                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    if (reader["nodeID"].ToString() != "")
                    {
                        flag = true;
                    }
                }   //Close of while

            } //Close of using



            if (flag == false)
            {
                //insert the value 

                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Insert the node value flag = false wala and id  = "+nodeid);
                    string sql_string = "insert into " + tableNameDevice + "(nodeID,device_instanceID_for_param1,device_instanceID_for_param2,IP_for_param1,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceIDparam1,@deviceInstanceParam2,@IP_param1,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceIDparam1", "");
                    command.Parameters.AddWithValue("@deviceInstanceParam2", Humidity_deviceInstanceID);//Insert empty value

                    command.Parameters.AddWithValue("@IP_param1", "");
                    command.Parameters.AddWithValue("@IP_param2", Humidity_ip);//Insert empty value

                    command.Parameters.AddWithValue("@param1", "");
                    command.Parameters.AddWithValue("@param2", Humidity_param1id);
                    command.Parameters.AddWithValue("@param1info", "");
                    command.Parameters.AddWithValue("@param2info", Humidity_param1info);
                    command.Parameters.AddWithValue("@param1_iden_type", "");
                    command.Parameters.AddWithValue("@param2_iden_type", Humidity_param1type);
                    command.ExecuteNonQuery();
                }
            }
            else
            {
                //Update the value
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    //SQLiteDataReader reader = null;
                    //  MessageBox.Show("Update mechanism the node value flag = true  wala,id = "+nodeid);

                    string sql_string = "UPDATE  " + tableNameDevice + "  SET device_instanceID_for_param2 = @instanceID,IP_for_param2 =@IP ,param2ID = @param2 ,param2_info = @param2info,param2_identifier_type= @param2_iden_type  where nodeID = @id ";
                    SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                    command.CommandType = CommandType.Text;
                    command.Parameters.AddWithValue("@id", nodeid);
                    command.Parameters.AddWithValue("@instanceID", Humidity_deviceInstanceID);
                    command.Parameters.AddWithValue("@IP", Humidity_ip);
                    command.Parameters.AddWithValue("@param2", Humidity_param1id);
                    // command.Parameters.AddWithValue("@param2", param2id);
                    command.Parameters.AddWithValue("@param2info", Humidity_param1info);
                    //  command.Parameters.AddWithValue("@param2info", param2info);
                    command.Parameters.AddWithValue("@param2_iden_type", Humidity_param1type);
                    // command.Parameters.AddWithValue("@param2_iden_type", param2type);
                    command.ExecuteNonQuery();
                }

            }


        }







        public void InsertLineInfoToDB(string id, string prevNodeId, string nextNodeId, Color lineColor, Series lineSeriesVal, int linethickness)
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
                string sql_string = "insert into " + tableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@nameVal,@statusVal)";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", chartDetailList[indexForWhichChartIsSelected].chart_respective_lineID);
                command.Parameters.AddWithValue("@id", id);
                command.Parameters.AddWithValue("@pn", prevNodeId);
                command.Parameters.AddWithValue("@nn", nextNodeId);
                command.Parameters.AddWithValue("@lc", ColorTranslator.ToHtml(lineColor));
                command.Parameters.AddWithValue("@ls", lineSeriesName);
                command.Parameters.AddWithValue("@thicknessValue", linethickness);
                command.Parameters.AddWithValue("@nameVal", lineName);
                command.Parameters.AddWithValue("@statusVal", lineStatus_ON_OFF);
                command.ExecuteNonQuery();
            }


        }



        public class TempDataTypeForMixNode
        {
            public string chartID { get; set; } //--for identifying which point is selected..
            public string nodeID { get; set; }//--this is the values that represent the point in a chart
            public string previousNodeID { get; set; }
            // public string source { get; set; }

            public string nextNodeID { get; set; }
           
        }
        public List<TempDataTypeForMixNode> mixNodeInfoList = new List<TempDataTypeForMixNode>();
        public List<TempDataTypeForMixNode> temporaryMixNodeList = new List<TempDataTypeForMixNode>();


        public int previousNodeIndexForLineInput = 0;


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

            try { 
            // listForDataFromDB.Clear();//Lets clear the node...
            if(indexValue < 0)
            {
                return;
            }
            if(chartDetailList.Count <= 0)
                {
                    return;
                }

            if(selectedBuildingList.Count <= 0)
                {
                    return;
                }
            //Lets identify the node
            // int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
            //int id = e.RowIndex;//This index is used to identify which cell or chart is clicked.
            int id = indexValue;//This index is used to identify which cell or chart is clicked.
            /*
            Now lets find the chart line id and chart node id 
            */
            string chartNodeGroupID = chartDetailList[id].chart_respective_nodeID;//This is for the node
            string chartLineGroupID = chartDetailList[id].chart_respective_lineID;//This is for the line

            string selectedChartID = chartDetailList[id].chartID;

            //--Reset the context menu stip first..
            menuStripNodeInfoValues.Clear();
            //--Reset the context menu stip first..
            menuStripNodeLineInfoValues.Clear();
            //--Now lets clear the mix node list first and then refill agiain
            mixNodeInfoList.Clear();


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
            string tableMixNode = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

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
                    //editied for reading from alex db
                    menuStripNodeInfoValues.Add(new TempDataType
                    {
                        id = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                        temperature_source = reader["temperature_source"].ToString(),
                        humidity_source = reader["humidity_source"].ToString(),
                        name = reader["name"].ToString(),
                        
                       // label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                       // showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString()),
                        airFlow = int.Parse(reader["airFlow"].ToString()),
                        lastUpdatedDate = reader["lastUpdatedDate"].ToString()


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

                //--Now we can find the previous id values as follows:
                //=====================================Finding previous id value=================================//

                previousNodeIndexForLineInput= IndexOfPreviousNodeForLineFunction();
                //==================================End of previous id=======================================//

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
                        lineThickness = int.Parse(reader2x["thickness"].ToString())   ,
                        name= reader2x["name"].ToString(),
                        status = int.Parse(reader2x["status"].ToString())
                    });

                }
                //count2 = menuStripNodeLineInfoValues.Count-1; //--This is used for udpdating the index values..

                //   MessageBox.Show("count line data in menuStripNodeLineInfoValues = " + menuStripNodeLineInfoValues.Count);


                //===============================Loading Mix NODE info===================================//


                SQLiteDataReader readerMix = null;
                string queryStringMix = "SELECT *  from " + tableMixNode + " WHERE chartID = @chartID";

                SQLiteCommand commandMix = new SQLiteCommand(queryStringMix, connection);
                commandMix.Parameters.AddWithValue("@chartID",selectedChartID );//This is the group id that is used to identify each node



                // int count = 0;
                readerMix = commandMix.ExecuteReader();
                while (readerMix.Read())
                {
                    //editied for reading from alex db
                    mixNodeInfoList.Add(new  TempDataTypeForMixNode
                    {
                        chartID = readerMix["chartID"].ToString(),
                         nodeID = readerMix["nodeID"].ToString(), //This is just changed code : bbk305
                        previousNodeID = readerMix["previousNodeID"].ToString(),
                        nextNodeID = readerMix["nextNodeID"].ToString(),
                       

                    });

                }



                //=================================End of loading mix node============================//










            }//close of using..
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }




        public int IndexOfPreviousNodeForLineFunction()
        {
            int indCountForPrevIdOfLine = 0;
            string idValue = "";
            int indexX = 0;
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                //--Now we need to count the valus
                if (menuStripNodeInfoValues[i].temperature_source == "Mix")
                {
                    //Do not count 
                }
                else
                {
                    //Its not mix node so count every other nodes
                    //indCountForPrevIdOfLine++;
                    idValue = menuStripNodeInfoValues[i].id;
                }
            }
            //--Now lets identify the node value
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
                
            {
                /*
               indCountForPrevIdOfLine-1 this -1 is done because 
                          indCountForPrevIdOfLine 1 means index is 0 
               */
                if (idValue == menuStripNodeInfoValues[i].id)
                {
                    indexX = i;
                    break;
                }
            }
            return indexX;
        }

        //****************************************end of the functions and code from buiding setting******************//

        public class building_info_datatype
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            public string street { get; set; }
           // public int ZIP { get; set; }
            public string longitude { get; set; }
            public string latitude { get; set; }
            public double elevation { get; set; }
            public string buildingName { get; set; }
            public string EngineeringUnits { get; set; }
        }

        public List<building_info_datatype> buildingList = new List<building_info_datatype>();

        /// <summary>
        /// it helps to pull the information of the building stored
        /// </summary>
        private void get_stored_data_about_building()
        {
            try {
                buildingList.Clear();
                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
                string connString = @"Data Source=" + databaseFile + ";Version=3;";
                using (SQLiteConnection connection = new SQLiteConnection(connString))
                {
                    connection.Open();
                    SQLiteDataReader reader = null;
                    string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    reader = command.ExecuteReader();
                    while (reader.Read())
                    {

                        buildingList.Add(new building_info_datatype
                        {
                            ID = int.Parse(reader["ID"].ToString()),
                            country = reader["country"].ToString(),
                            state = reader["state"].ToString(),
                            city = reader["city"].ToString(),
                            street = reader["street"].ToString(),
                            //ZIP = int.Parse(reader["ZIP"].ToString()),//--This zip code has be removed in latest version
                            latitude = reader["latitude"].ToString(),
                            longitude = reader["longitude"].ToString(),
                            elevation = double.Parse(reader["elevation"].ToString()),
                            buildingName = reader["BuildingName"].ToString(),
                            EngineeringUnits = reader["EngineeringUnits"].ToString()
                            
                        });

                    }


                }

            } catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        public bool CheckLatLongAvailable()
        {
            //--Lets do some connection checking and validating the data returned...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            bool returnValue = false;
            string latValue = "";
            string lngValue = "";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                //command.Parameters.AddWithValue("@index", index);
                //command.Parameters.Add("@index",DbType.Int32).Value= index_selected;
                // command.Parameters.AddWithValue("@index", index_selected);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    latValue = reader["latitude"].ToString();
                    lngValue = reader["longitude"].ToString();
                }
              //  MessageBox.Show("latitude value " + latValue + ",longvalue = " + lngValue);
            }
            if ((latValue == "" || lngValue =="" ) || (latValue ==0.ToString() && lngValue == 0.ToString() ))
            {
                returnValue = true;
               // MessageBox.Show("value is ture = " + returnValue);

            }
       
            //--This will be either true or false based on the check value..
            return returnValue;
        }
        string latPulledValue, longPulledValue, elevationPulledValue;
        double latVal, longVal;//--This is used for storing temporary lat long value...

        public void FillLatLongValueAutomatically()
        {

            string country = null, state = null, city = null, street = null;//, zip = null;
            //--This portion fill the lat,long and elevation value is not present in the database by users..
            //--Lets do some connection checking and validating the data returned...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "SELECT * from tbl_building_location WHERE selection=1";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);

                reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //ListboxItems.Add(reader[1].ToString()+","+reader[2].ToString());
                    country = reader["country"].ToString();
                    state = reader["state"].ToString();
                    city = reader["city"].ToString();
                    street = reader["street"].ToString();
                   // zip = reader["zip"].ToString();
                }
            }

            // MessageBox.Show("Country = " + country + ",city " + city);

            pull_data_online(country, state, city, street);//--This will fill the online values form the database
                                                                //--After pulling above we get three values we need to push it to database...
           // MessageBox.Show("inside  FillLatLongValueAutomatically lat pulled value=" + latPulledValue + ",long = " + longPulledValue + ",elev = " + elevationPulledValue);

            //--Upadating the table which has no values ...
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE tbl_building_location SET  latitude=@latitude_value,longitude=@longitude_value,elevation=@elevation  where selection=1;";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@latitude_value", latPulledValue.ToString());
                command.Parameters.AddWithValue("@longitude_value", longPulledValue.ToString());
                command.Parameters.AddWithValue("@elevation", elevationPulledValue.ToString());
                command.ExecuteNonQuery();
            }


        }
        private void pull_data_online(string country1, string state1, string city1, string street1)
        {
            //this function pulls the data from online devices...


            /*
            1.country,state,city,street,latitude,longitude,elev,zip
            */
            string country = country1;
            string state = state1;
            string city = city1;
            string street = street1;
           // string zip = zip1;
            int value;
          //  if (int.TryParse(zip, out value))
           // {

                if (country != "" && city != "")
                {
                    string join_string = "";
                    
                    //--THIS IS DONE BECAUSE state,and street if present some api do not give response
                    //if (state != "" && street != "")
                    //{
                    //    join_string = country + "," + state + "," + city + "," + street;
                    //}
                    //else
                    //{
                        join_string = country + "," + city;
                   // }

                    //geo location code goes here..
                    try
                    {                                                                                   

                        var address = join_string;
             
                        string BingMapsKey = "AgMVAaLqK8vvJe6OTRRu57wu0x2zBX1bUaqSizo0QhE32fqEK5fN8Ek4wWmO4QR4";
             
                        //Create REST Services geocode request using Locations API
                        string geocodeRequest = "http://dev.virtualearth.net/REST/v1/Locations/" + address + "?o=xml&key=" + BingMapsKey;

                      using (var wc = new WebClient())
                        {
                            string api_url = geocodeRequest;

                            var data = wc.DownloadString(api_url);
                            //  MessageBox.Show("string apic return =" + data);
                            string xml_string = data.ToString();
                            //MessageBox.Show(xml_string);


                            //--Parsing the xml document int the c# application...                     
                            //xml parsing...
                            XmlDocument xml = new XmlDocument();
                            xml.LoadXml(xml_string);

                            ProcessResponse(xml);
                            latPulledValue = latVal.ToString();//--Storing it in this variable to make it a string...
                            longPulledValue = longVal.ToString();

                            //MessageBox.Show("lat = " + latPulledValue + "long = " + longPulledValue);

                            //--This is for the elevation part...
                            string elevationAPI_URL = "http://dev.virtualearth.net/REST/v1/Elevation/List?pts=" + latVal + "," + longVal + "&key=AgMVAaLqK8vvJe6OTRRu57wu0x2zBX1bUaqSizo0QhE32fqEK5fN8Ek4wWmO4QR4&output=xml";

                            var elevationData = wc.DownloadString(elevationAPI_URL);
                            // MessageBox.Show("elev data = " + elevationData);
                            //--Now lets do the parsing...
                            //xml parsing...
                            XmlDocument xmlElevation = new XmlDocument();
                            xmlElevation.LoadXml(elevationData);
                            elevationPulledValue = elevationProcess(xmlElevation).ToString();//--This gives the elevation...
                                                                                             // MessageBox.Show("Pulled elevation");
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }//close of if...
           // }//close of if int try parse.
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_zip_numbe);
            }

        }


        //--Now lets process the elevation data....
        double elev;
        public double elevationProcess(XmlDocument locationsResponse)
        {

            //Create namespace manager
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(locationsResponse.NameTable);
            nsmgr.AddNamespace("rest", "http://schemas.microsoft.com/search/local/ws/rest/v1");

            //Get formatted addresses: Option 1
            //Get all locations in the response and then extract the formatted address for each location
            XmlNodeList locationElements = locationsResponse.SelectNodes("//rest:Elevations", nsmgr);
            //Console.WriteLine("Show all formatted addresses: Option 1");
            foreach (XmlNode location in locationElements)
            {
                //MessageBox.Show("Lat = "+location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                elev = double.Parse(location.SelectSingleNode(".//rest:int", nsmgr).InnerText);
                //  MessageBox.Show("elev = " + elev);
                //longVal = double.Parse(location.SelectSingleNode(".//rest:Longitude", nsmgr).InnerText);
                //MessageBox.Show("Long = " + longVal);
            }

            return elev;
        }
        public void ProcessResponse(XmlDocument locationsResponse)
        {
            //Create namespace manager
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(locationsResponse.NameTable);
            nsmgr.AddNamespace("rest", "http://schemas.microsoft.com/search/local/ws/rest/v1");

            //Get formatted addresses: Option 1
            //Get all locations in the response and then extract the formatted address for each location
            XmlNodeList locationElements = locationsResponse.SelectNodes("//rest:Location", nsmgr);
            Console.WriteLine("Show all formatted addresses: Option 1");
            foreach (XmlNode location in locationElements)
            {
                //MessageBox.Show("Lat = "+location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                latVal = double.Parse(location.SelectSingleNode(".//rest:Latitude", nsmgr).InnerText);
                // MessageBox.Show("lat = " + latVal);
                longVal = double.Parse(location.SelectSingleNode(".//rest:Longitude", nsmgr).InnerText);
                // MessageBox.Show("Long = " + longVal);
            }
            //   Console.WriteLine();
        }

        public void sqlite_database_creation(string buildingNameSelected)
        {

            //--lets do try catch
            try
            {
                //--This is where we are going to create all the database  and tables of sqlite
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";


                //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath1 + @"\db_psychrometric_project.s3db";
                //string connString = @"Data Source=" + databaseFile1 + ";Version=3;";

              //  MessageBox.Show("Create table section path Path = " + databaseFile);
                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                m_dbConnection.Open();

                //--building location table : tbl_building_location
                //--This one is with the zip code later zip code is removed
                //string sql = "create table tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255), ZIP int,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";
                string sql = "create table IF NOT EXISTS tbl_building_location (selection int,ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255) ,longitude varchar(255),latitude varchar(255),elevation varchar(255),BuildingName varchar(255),EngineeringUnits varchar(255))";

                SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
                command.ExecuteNonQuery();
                //--next table geo location value : tbl_geo_location_value
                //string sql1 = "create table tbl_geo_location_value (ID int ,longitude varchar(255),latitude varchar(255),elevation varchar(255))";
                //SQLiteCommand command1 = new SQLiteCommand(sql1, m_dbConnection);
                //command1.ExecuteNonQuery();

                //--next table historical data:tbl_historical_data
                string sql2 = "create table IF NOT EXISTS tbl_historical_data (ID INTEGER,date_current datetime,hour_current int,minute_current int,distance_from_building varchar(255),temperature varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
                command2.ExecuteNonQuery();
                //--next table tbl_temp_himidity 
                //string sql3 = "create table tbl_temp_humidity (temp int,humidity int)";
                //SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                //command3.ExecuteNonQuery();

                string sql3 = "create table IF NOT EXISTS tbl_language_option (ID int, language_id int)";
                SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                command3.ExecuteNonQuery();


                ////--next table weather related datas...
                string sql4 = "create table IF NOT EXISTS tbl_weather_related_values (ID INTEGER ,location varchar(255),distance_from_building varchar(255),last_update_date varchar(255),temp varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, m_dbConnection);
                command4.ExecuteNonQuery();


                //==================Lets create some more essential tables  added in jan 26th 2017=========//

                /*
                 Tables to be added in this section 
                 1.tbl_[] _chart_comfort_zone_setting
                 2.tbl_[]_chart_detail
                 3.tbl_[]_comfort_zone_detail
                 4.tbl_[]_device_info_for_node
                 5.tbl_[]_input_storage_from_T3000
                 6.tbl_[]_line_value
                 7.tbl_[]_mix_node_info
                 8.tbl_[]_node_data_related_T300
                 9. tbl_[]_node_value
                 */
                string tbl_comfortzoneSetting = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_comfort_zone_setting  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                string tbl_chart_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_chart_detail(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                string tbl_comfort_zone_detail = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_comfort_zone_detail  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                string tbl_device_info_for_node = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_device_info_for_node  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param1 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                string tbl_input_storage_form_T3000 = " CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_input_storage_from_T3000  ( count INTEGER PRIMARY KEY AUTOINCREMENT,PanelID varchar(255),InputIndex varchar(255),InputDescription varchar(255),InputAM varchar(255), InputValue  varchar(255) ,InputUnit varchar(255),InputRange varchar(255),InputCalibration varchar(255),InputFilter varchar(255),InputJumper varchar(255),InputLabel varchar(255)  )";
                string tbl_line_value = " CREATE TABLE  IF NOT EXISTS tbl_" + buildingNameSelected + "_line_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255), status INTEGER)";
                string tbl_mix_node_info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_mix_node_info  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255),chartID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                string tbl_node_data_related_T3000 = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_data_related_T3000  (count INTEGER PRIMARY KEY AUTOINCREMENT, nodeID varchar(255), param1_panelID varchar(255), param1_inputIndex varchar(255), param2_panelID varchar(255), param2_inputIndex varchar(255)) ";
                string tbl_node_value = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_node_value(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),name varchar(255),temperature_source varchar(255),humidity_source varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(225),lastUpdatedDate varchar(255))";
                string tbl_TemperatureHumiditySourceInfo = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_TemperatureHumiditySourceInfo(count INTEGER PRIMARY KEY AUTOINCREMENT,NodeID varchar(255), chartID varchar(255) ,TemperatureSourceInfo VARCHAR(255),HumiditySourceInfo varchar(255))";
                string tbl_Weather_Controller_Restor_Info= "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Controller_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255), ControllerNameInfo varchar(255) ,TemperatureParameterInfo VARCHAR(255),HumidityParameterInfo varchar(255),TempValue varchar(255),HumValue varchar(255))";
                string tbl_Weather_Web_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_Web_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), StationInfo varchar(255))";
                string tbl_Weather_HumSelfCalibration_Restor_Info = "CREATE TABLE IF NOT EXISTS tbl_" + buildingNameSelected + "_Weather_HumSelfCalibration_Restor_Info(count INTEGER PRIMARY KEY AUTOINCREMENT,BuildingName varchar(255),Enable_dissable_info varchar(255), max_adjustment_per_day varchar(255))";


                //now execute the query
                SQLiteCommand cm1 = new SQLiteCommand(tbl_comfortzoneSetting, m_dbConnection);
                cm1.ExecuteNonQuery();

                SQLiteCommand cm2 = new SQLiteCommand(tbl_chart_detail, m_dbConnection);
                cm2.ExecuteNonQuery();

                SQLiteCommand cm3 = new SQLiteCommand(tbl_comfort_zone_detail, m_dbConnection);
                cm3.ExecuteNonQuery();

                SQLiteCommand cm4 = new SQLiteCommand(tbl_device_info_for_node, m_dbConnection);
                cm4.ExecuteNonQuery();

                SQLiteCommand cm5 = new SQLiteCommand(tbl_input_storage_form_T3000, m_dbConnection);
                cm5.ExecuteNonQuery();

                SQLiteCommand cm6 = new SQLiteCommand(tbl_line_value, m_dbConnection);
                cm6.ExecuteNonQuery();

                SQLiteCommand cm7 = new SQLiteCommand(tbl_mix_node_info, m_dbConnection);
                cm7.ExecuteNonQuery();

                SQLiteCommand cm8 = new SQLiteCommand(tbl_node_data_related_T3000, m_dbConnection);
                cm8.ExecuteNonQuery();

                SQLiteCommand cm9 = new SQLiteCommand(tbl_node_value, m_dbConnection);
                cm9.ExecuteNonQuery();

                SQLiteCommand cm10 = new SQLiteCommand(tbl_TemperatureHumiditySourceInfo, m_dbConnection);
                cm10.ExecuteNonQuery();

                SQLiteCommand cm11 = new SQLiteCommand(tbl_Weather_Controller_Restor_Info, m_dbConnection);
                cm11.ExecuteNonQuery();

                SQLiteCommand cm12 = new SQLiteCommand(tbl_Weather_Web_Restor_Info, m_dbConnection);
                cm12.ExecuteNonQuery();

                SQLiteCommand cm13 = new SQLiteCommand(tbl_Weather_HumSelfCalibration_Restor_Info, m_dbConnection);
                cm13.ExecuteNonQuery();


                /*
                 Now lets read the data form alex database and store it in the our db_psychometric_project.s3db databse 
                 
                 //==Read complete now writing the values to our database
                
                
                  */

                //MessageBox.Show("We have created all tables now insertion is left");
                //This function will write to tbl_bulding_location as well as will make a building selected
                // WriteT3000BuildingInfoToPsychoDB("1", BuildingSelected[0].country, BuildingSelected[0].state, BuildingSelected[0].city, BuildingSelected[0].street, BuildingSelected[0].longitude, BuildingSelected[0].latitude, BuildingSelected[0].elevation, BuildingSelected[0].Building_Name, BuildingSelected[0].EngineeringUnits);

                string sql_stringx = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                SQLiteCommand command9 = new SQLiteCommand(sql_stringx, m_dbConnection);
                command9.CommandType = CommandType.Text;
                command9.Parameters.AddWithValue("@sel", "1");
                command9.Parameters.AddWithValue("@con", BuildingSelected[0].country);
                command9.Parameters.AddWithValue("@state", BuildingSelected[0].state);
                command9.Parameters.AddWithValue("@city", BuildingSelected[0].city);
                command9.Parameters.AddWithValue("@stre", BuildingSelected[0].street);
                command9.Parameters.AddWithValue("@lng", BuildingSelected[0].longitude);
                command9.Parameters.AddWithValue("@lat", BuildingSelected[0].latitude);
                command9.Parameters.AddWithValue("@elev", BuildingSelected[0].elevation);
                command9.Parameters.AddWithValue("@bname", BuildingSelected[0].Building_Name);
                command9.Parameters.AddWithValue("@engUnit", BuildingSelected[0].EngineeringUnits);
                command9.ExecuteNonQuery();

                //==Now lets make a single default comfort zone in our database.
                //--This one is for default value.
                string id = GetGUID();
                string sql_data_for_comfortzone = "INSERT INTO tbl_"+ buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('"+id+"', 'Default_comfort_zone','22','28','60','80','GREEN')";
                SQLiteCommand cmddata = new SQLiteCommand(sql_data_for_comfortzone, m_dbConnection);
                cmddata.ExecuteNonQuery();

                //--comfortzone airport.
                string id_airport = GetGUID();
                string sql_comfortzone_airport = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_airport + "', 'Airport','22','28','35','65','Blue')";
                SQLiteCommand cmd_airport = new SQLiteCommand(sql_comfortzone_airport, m_dbConnection);
                cmd_airport.ExecuteNonQuery();

                //--comfortzone club.
                string id_club = GetGUID();
                string sql_comfortzone_club = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_club + "', 'Club','22','28','35','65','Blueviolet')";
                SQLiteCommand cmd_club = new SQLiteCommand(sql_comfortzone_club, m_dbConnection);
                cmd_club.ExecuteNonQuery();

                //--comfortzone computer.
                string id_computer = GetGUID();
                string sql_comfortzone_computer = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_computer + "', 'Computer','22','28','35','65','Turquoise')";
                SQLiteCommand cmd_computer = new SQLiteCommand(sql_comfortzone_computer, m_dbConnection);
                cmd_computer.ExecuteNonQuery();

                //--comfortzone datacenter.
                string id_datacenter = GetGUID();
                string sql_comfortzone_datacenter = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_datacenter + "', 'Data Center','22','28','35','65','Lightgreen')";
                SQLiteCommand cmd_datacenter = new SQLiteCommand(sql_comfortzone_datacenter, m_dbConnection);
                cmd_datacenter.ExecuteNonQuery();

                //--comfortzone gym.
                string id_gym = GetGUID();
                string sql_comfortzone_gym = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_gym + "', 'Gym','22','28','35','65','Lightgray')";
                SQLiteCommand cmd_gym = new SQLiteCommand(sql_comfortzone_gym, m_dbConnection);
                cmd_gym.ExecuteNonQuery();

                //--comfortzone Hospital.
                string id_Hospital = GetGUID();
                string sql_comfortzone_Hospital = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Hospital + "', 'Hospital','22','28','35','65','Red')";
                SQLiteCommand cmd_Hospital = new SQLiteCommand(sql_comfortzone_Hospital, m_dbConnection);
                cmd_Hospital.ExecuteNonQuery();

                //--comfortzone Music.
                string id_Music = GetGUID();
                string sql_comfortzone_Music = "INSERT INTO tbl_" + buildingNameSelected + "_comfort_zone_detail( id ,name,min_temp ,max_temp,min_hum , max_hum  ,colorValue   )   VALUES('" + id_Music + "', 'Music','22','28','35','65','Orangered')";
                SQLiteCommand cmd_Music = new SQLiteCommand(sql_comfortzone_Music, m_dbConnection);
                cmd_Music.ExecuteNonQuery();


                //===============END OF THE NEW CODE ADDED IN THIS SECTION==========================//




                //--Lets input some values in the tbl_building_location and in tbl_language_option default 

                //string sql_input1 = "INSERT INTO tbl_building_location (selection,country,state,city,street,BuildingName,EngineeringUnits) VALUES(1, 'china','SangHai','SangHai','No.35,yi yuan garden','Default_Building','SI') ";
                //SQLiteCommand commandINput5 = new SQLiteCommand(sql_input1, m_dbConnection);
                //commandINput5.ExecuteNonQuery();

                //MessageBox.Show("Test We reached this part ln 5207");
                //Adding to language option
                string sql_input2 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(1, 1) ";
                string sql_input3 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(2, 0) ";
                string sql_input4 = "INSERT INTO tbl_language_option (ID,language_id) VALUES(3, 0) ";

                SQLiteCommand c2 = new SQLiteCommand(sql_input2, m_dbConnection);
                c2.ExecuteNonQuery();

                SQLiteCommand c3 = new SQLiteCommand(sql_input3, m_dbConnection);
                c3.ExecuteNonQuery();
                SQLiteCommand c4 = new SQLiteCommand(sql_input4, m_dbConnection);
                c4.ExecuteNonQuery();                
                m_dbConnection.Close();//--closing the connection

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
         
        }


        /// <summary>
        /// Functions writes to tbl_building_location about the building selected in T3000 databse.
        /// </summary>
        /// <param name="selection"></param>
        /// <param name="country"></param>
        /// <param name="state"></param>
        /// <param name="city"></param>
        /// <param name="street"></param>
        /// <param name="lng"></param>
        /// <param name="lat"></param>
        /// <param name="elev"></param>
        /// <param name="buildingName"></param>
        /// <param name="engUnits"></param>
        public void WriteT3000BuildingInfoToPsychoDB(string selection, string country, string state,string city,string street, string lng,string lat,string elev,string buildingName, string engUnits)
        {

            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device";   // currentLineTableFromDB;
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string sql_string = "insert into tbl_building_location ( selection ,country ,state,city ,street,longitude,latitude ,elevation ,BuildingName ,EngineeringUnits  ) VALUES( @sel ,@con,@state ,@city,@stre , @lng  ,@lat,@elev ,@bname ,@engUnit  )";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@sel", selection);
                command.Parameters.AddWithValue("@con", country);
                command.Parameters.AddWithValue("@state", state);
                command.Parameters.AddWithValue("@city", city);
                command.Parameters.AddWithValue("@stre", street);
                command.Parameters.AddWithValue("@lng", lng);
                command.Parameters.AddWithValue("@lat", lat);
                command.Parameters.AddWithValue("@elev", elev);
                command.Parameters.AddWithValue("@bname", buildingName);
                command.Parameters.AddWithValue("@engUnit", engUnits);                
                command.ExecuteNonQuery();
            }



        }

        /*This is the function that plots the graph 
         */

        int indexI = 0;
        public void plot_on_graph_values(double dbt, double hr, double xval, double yval)
        {


            series1xx.ChartType = SeriesChartType.Point;
            series1xx.Color = Color.FromArgb(0, 0, 255);//blue
            series1xx.MarkerStyle = MarkerStyle.Circle;

            series1xx.MarkerSize = 12;

            string label = "DBT=" + dbt + ",HR=" + hr;

            //chart1.Series["SeriesDBT_HR" + index].;
            series1xx.Points.AddXY(xval, yval);
            series1xx.Points[indexI++].Label = label;
            //series1.Enabled = true;

        }

        public int plot_by_DBT_HR(double DBT1, double HR)
        {
            /*           
             *We need to cal x-asis which is given by DBT 
             */
            // MessageBox.Show("reached here dbt=" + DBT + ", hr = " + HR);
            int x_axis = (int)DBT1;
            double DBT = (double)(int)(DBT1);
            //here the HR is  relative humidity like 20%,30% etc os phi = 0.3 for 30%
            double phi = HR;
            //we need to calculate the y-axis value 
            /*For y axis the value has to be pulled from the t_pg text file....
             */
            //lets create two arraylist to add those and store it in the arraylist
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();


            string line1;

            //--- we need to copy the values to the corresponding array list
            temperature_value = t;
            pg_value_from_txtfile = pg;

            double patm = 101.235;//constant..we will make it take as input later...
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
            double y_axis = wg_calc;
            // now lets plot on graph...
            //chart1.Series.Add("SeriesDBT_HR"+index);//dont need this we have declared this is chart control
            //MessageBox.Show("reached near chart");
            //Series series1 = new Series("My Series"+index);
            //chart1.Series.Add(series1);

            //series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
            //series1.Color = Color.FromArgb(0, 0, 255);//blue
            //series1.MarkerSize = 7;
            //string label = "DBT=" + DBT + ",HR=" + HR;
            //series1.Label = label;
            ////chart1.Series["SeriesDBT_HR" + index].;
            //series1.Points.AddXY(x_axis, y_axis);

            plot_on_graph_values(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            index++;
            //if (index == 400)
            //{
            //    index = 0;
            //}


            return 0;
        }

        public double CalculateYFromXandEnthalpy(double dbt, double enthalpy)
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

            //MessageBox.Show("x2= " + x2+" y2  = "+y2);

            double x = dbt;
            double y = y1 + (((y2 - y1) * (x - x1) / (x2 - x1)));

            double y_axis = y;
            return y_axis;
        }


        //this was for ploting dbt and enthalpy which we dont require now...
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

            //MessageBox.Show("x2= " + x2+" y2  = "+y2);

            double x = dbt;
            double y = y1 + (((y2 - y1) * (x - x1) / (x2 - x1)));

            double y_axis = y;

            //MessageBox.Show("y = " + y);

            chart1.Series.Add("SeriesDBT_enthalpy" + index);//this is already delceared in chart control so we dont need it
            chart1.Series["SeriesDBT_enthalpy" + index].ChartType = SeriesChartType.Point;
            chart1.Series["SeriesDBT_enthalpy" + index].Color = Color.Blue;
            chart1.Series["SeriesDBT_enthalpy" + index].MarkerSize = 15;
            chart1.Series["SeriesDBT_enthalpy" + index].Label = "DBT = " + dbt + "degC ,enthalpy = " + enthalpy;
            //chart1.Series["SeriesDBT_HR" + index].;
            chart1.Series["SeriesDBT_enthalpy" + index].Points.AddXY(x_axis, y_axis);

            index++;



            return 0;
        }


        private void button5_Click(object sender, EventArgs e)
        {
            /*

            double enthalpy = 0.000000;
            double dew_point = 0.000000;
            try
            {
               enthalpy = Double.Parse(tb_enthalpy.Text.Trim());
                dew_point = Double.Parse(tb_dew_point.Text.Trim());
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


            plot_by_enthalpy_dew_point(enthalpy, dew_point);

            */

        }


        public class get_temp_hum
        {
            public string temp { get; set; }
            public string humidity { get; set; }
        }

        Point? prevPosition = null;

        double currentXAxis = 0.000;
        double currentYAxis = 0.000;

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

                    // if((currentXAxis>=0 && currentXAxis<=50)&&(currentYAxis>=0 && currentYAxis <= 30)) { 

                    if (((double)xVal >= 0 && (double)xVal <= 50) && ((double)yVal >= 0 && (double)yVal <= 30))
                    {
                        //--These currentXAxis and currentYAxis is used for showing the dotted line 
                        currentXAxis = (double)xVal;
                        currentYAxis = (double)yVal;

                        //lb_test.Text = "x = " + currentXAxis + ",y  = " + currentYAxis;
                        // if ((currentXAxis >= 0 && currentXAxis <= 50) && (currentYAxis >= 0 && currentYAxis <= 30))
                        //{

                        //now lets move on to making other part 
                        /*1.find dbt value => this is x axis value 
                         * 2.find sp.ratio value => this is yaxis value
                         */
                        lb_dbt.Text = Math.Round(xVal, 2).ToString();
                        lb_humidity_ratio.Text = Math.Round(yVal, 2).ToString();

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

                        double patm = AirPressureFromDB * 0.001; // this is in terms of kpa //101.325;//this is constant... //101.325;//this is constant...
                        //Lets check the patm value
                      //  lb_test1.Text = "patm = " + patm;
                        double w = yVal;
                        phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                        phi = phi * 100;//changing into percent..
                                        //now display in label...
                        lb_RH.Text = Math.Round(phi, 2).ToString();

                       // lb_test1.Text = " Humidity = " + phi;

                        //now lets calculate the dew point...
                        double humidity = phi;
                        double temperature1 = xVal;
                        double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
                        //now lets print this value..
                        lb_DP.Text = Math.Round(TD, 2).ToString();


                        //now lets move towards enthalpy...

                        Patm = patm * 10;  // 1013;
                        A = 6.116441;
                        m = 7.591386;
                        Tn = 240.7263;
                        B = 621.9907;

                        double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

                        double X = B * Pws / (Patm - Pws);   //This one is formula 

                        h = temperature1 * (1.01 + (0.00189 * X)) + 2.5 * X; //This one is the enthalpy
                        //now lets display this value ..
                        lb_enthalpy.Text = Math.Round(h, 2).ToString();

                    }


                }//Closing of currentxval= 0-50 and 0-30 currentyval
            }

            if(flagForEditComfortZoneGraphically == 1)
            {

               if(flagForBorderLineSelectedForMoveForEditCF == 1)
                {
                    //--Perform redraw function 

                   
                    CursorFunctionForComfortZoneEditMode(e);

                }
                else { 
                //--IF edit comfort zone is enabled dont do other task just do the 
                //Task of comfort zone only and let other task hault for now
                ComfortZoneBorderLineDetectForEdit(e); // --This methods detects line movement
                }




            }
            else
            { 

            //--IF the line is selected/disconnected and then we need to connect to a node
            if (flagForDisconnectClick == 1)  //Disconnect is clicked then they talk
            {
                //--Creating temporary line..
                //--then redraw it again...
                addTemporarySeries();
                //--Now lets move on the showing the hand when hover over the Node lets do it bro...
                addCursorFunctionForLineDisconnectConnect(e);
                // lb_where.Text = "me : discon =1";

            }
            else {
                // lb_where.Text = "me : else line detect on";
                disconnectLineToolStripMenuItem.Enabled = false;

                //--This one is for two side of line disconnection -------//
                disconnectLineFromAToolStripMenuItem.Enabled = false;
                //disconnectLineFromBToolStripMenuItem.Enabled = false;

                nodeAToolStripMenuItem.Enabled = false;
                nodeBToolStripMenuItem.Enabled = false;
                addMixNodeToolStripMenuItem.Enabled = false;//Dissable add mix node
                //---End of two side of line disconnection section

                //--This is for the weather the line is moverover or not...
                LineDetectOnMouseMove(e);  
                                                                                             
                //--Lets add a function for the process diagram drawing..

                ProcessDiagramMouseMoveFunction(e);//--This does the adding and removing part             
            }

            }

        }//close of the main private void...


          /// <summary>
          /// This function helps to add the cursor function while single 
          /// series is selected
          /// </summary>
          /// <param name="e">Mouse Event arguments selected</param>
        public void CursorFunctionForComfortZoneEditMode(MouseEventArgs e)
        {
            if (!chart1.IsAccessible && load == 0)
            {
                load = 1;
                return;

            }

            //We need to get the temperatue and humidity based on the series selected
            /*
           Steps : 1 Chose which seires is selected 
           2. Then for left series change only temperatue should be less than maxTemp -2 and 
           calculated form x-axis and humidity is same for these cases 
           */

            double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
            double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);

            if (selectedSeriesForMoveForEditComfortZone == "Vertical_Temp_Left_Side")
            {


                if ( ( xValue < (int)(maxTemperatureForNewComfortZoneCreate-1)) && (xValue >= 0 && xValue<=50 )) //Let user be upper than 1 unit
                { 

                //Left is selected
                //Series s1 = Series.Equals ("Vertical_Temp_Left_Side"); 
                chart1.Series["Vertical_Temp_Left_Side"].Points.Clear();//We have cleared these series first
                chart1.Series["Vertical_Temp_Right_Side"].Points.Clear();
                chart1.Series["Horizontal_Hum_Down_Side"].Points.Clear();
                chart1.Series["Horizontal_Hum_Upper_Side"].Points.Clear();

                 chart1.Invalidate();

                    minTemperatureForNewComfortZoneCreate = (int)xValue;
                    // maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                    // minHumidityForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                    // maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);

                    //Now replotting the values 
                    PlotOuterBorderForComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, Color.Black);

                }

            } else if (selectedSeriesForMoveForEditComfortZone == "Vertical_Temp_Right_Side")
            {

                //Here we need to do operation related to right side 

                //right series is selected
                if ((xValue > (int)(minTemperatureForNewComfortZoneCreate + 1)) && (xValue >= 0 && xValue <= 50)) //Let user be upper than 1 unit
                {

                    //Left is selected
                    //Series s1 = Series.Equals ("Vertical_Temp_Left_Side"); 
                    chart1.Series["Vertical_Temp_Left_Side"].Points.Clear();//We have cleared these series first
                    chart1.Series["Vertical_Temp_Right_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Down_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Upper_Side"].Points.Clear();
                    chart1.Invalidate();
                   // minTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_temp) ;//  (int)xValue;
                    maxTemperatureForNewComfortZoneCreate =(int)xValue; //int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                                                                        // minHumidityForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                                                                        // maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);

                    //Now replotting the values 
                    // PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum), Color.Black);
                    // PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp),xValue, double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum), Color.Black);
                    PlotOuterBorderForComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, Color.Black);

                }

            }
            else if (selectedSeriesForMoveForEditComfortZone == "Horizontal_Hum_Down_Side")
            {
                //Down series is selected
                //--This is related to humidity now 
              double hum =   CalculateHumidityFromXandYCoordinate(xValue, yValue);//This gets the humidity 

                if ((hum < (int)(maxHumidityForNewComfortZoneCreate - 1)) && (hum >= 0 && hum <= 100)) //Mx hum -1 hum of lower should be in lower position
                {

                    //Left is selected
                    //Series s1 = Series.Equals ("Vertical_Temp_Left_Side"); 
                    chart1.Series["Vertical_Temp_Left_Side"].Points.Clear();//We have cleared these series first
                    chart1.Series["Vertical_Temp_Right_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Down_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Upper_Side"].Points.Clear();
                    chart1.Invalidate();
                    //Now replotting the values 
                   // minTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_temp);//  (int)xValue;
                   // maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                    minHumidityForNewComfortZoneCreate = (int)hum; //int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                                                                   // maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);


                    // PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), hum, double.Parse(listchartComfortZoneInfoSingle[0].max_hum), Color.Black);
                    PlotOuterBorderForComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, Color.Black);
                }

            }  else if (selectedSeriesForMoveForEditComfortZone == "Horizontal_Hum_Upper_Side")
            {
                //Upper series is selected
                double hum = CalculateHumidityFromXandYCoordinate(xValue,yValue);//This gets the humidity 

                if ((hum > (int)(minHumidityForNewComfortZoneCreate + 1)) && (hum >= 0 && hum <= 100)) //Mx hum -1 hum of lower should be in lower position
                {

                    //Left is selected
                    //Series s1 = Series.Equals ("Vertical_Temp_Left_Side"); 
                    chart1.Series["Vertical_Temp_Left_Side"].Points.Clear();//We have cleared these series first
                    chart1.Series["Vertical_Temp_Right_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Down_Side"].Points.Clear();
                    chart1.Series["Horizontal_Hum_Upper_Side"].Points.Clear();
                    chart1.Invalidate();
                    //Now replotting the values 
                    //minTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_temp);//  (int)xValue;
                    //maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                    //minHumidityForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                    maxHumidityForNewComfortZoneCreate =(int)hum; //  int.Parse(listchartComfortZoneInfoSingle[0].max_hum);

                    //PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), hum, double.Parse(listchartComfortZoneInfoSingle[0].max_hum), Color.Black);
                   // PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), hum, Color.Black);
                    PlotOuterBorderForComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, Color.Black);
                }

            }





        }



        private double CalculateHumidityFromXandYCoordinate(double xVal, double yVal)
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
          //  humidityCalculated = phi;//--This is the Relative humidity calculated value
            return phi; //This phi is humidity
        }


        /// <summary>
        /// This adds cursor function as dotted cursor which need to be redrawn constantly
        /// it updates the value of the  dotted cursor 
        /// </summary>
        /// <param name="e">Mouse event argument while performing onMouseMove</param>
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
                    if ((xAxis1 >= 0 && xAxis1 <= 50) && (yAxis1 >= 0 && yAxis1 <= 30)) {
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
                    }//close of if
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

            if(flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 0)
            {
                //--If the line is not reseted then return form here
                return;
            }

            //We need to find the previous point
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                //1 means prev node is selected 
                if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 2) //This is done because we are detaching from node1
                {
                    if (menuStripNodeInfoValues[i].id == indexOfPrevPointForLineMovement)
                    {
                        //This is the node of previous point we need to find the x and y coordinate of
                        xAxisValue = menuStripNodeInfoValues[i].xVal;
                        yAxisvalue = menuStripNodeInfoValues[i].yVal;
                        break;//If value found no need to search all exit of loop


                    }

            } //2 means next node is selected 0 means off
                else if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 1)//--This is done we are detaching form node2
            {
                if (menuStripNodeInfoValues[i].id == indexOfNextNodeForLineMovement)
                {
                    //This is the node of previous point we need to find the x and y coordinate of
                    xAxisValue = menuStripNodeInfoValues[i].xVal;
                    yAxisvalue = menuStripNodeInfoValues[i].yVal;
                    break;//If value found no need to search all exit of loop


                }

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
        /// <summary>
        /// Detects the line which connects the two nodes
        /// when hovered over it 
        /// </summary>
        /// <param name="e">mouse event argument an event simply</param>
        private void LineDetectOnMouseMove(MouseEventArgs e)
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

                                //  idOfLineSelecteForDisconnect = menuStripNodeLineInfoValues[i].ID;
                                indexOfLineInTheList = i; // updating the index 

                               //--Logging the index so that it could be used for futher processing later...
                               indexOfPrevPointForLineMovement = menuStripNodeLineInfoValues[i].prevNodeId;//This gets the previous node id value...

                                indexOfNextNodeForLineMovement = menuStripNodeLineInfoValues[i].nextNodeId;//==This gets the next node id

                                hit.Series.Color = Color.Black;
                                disconnectLineToolStripMenuItem.Enabled = true;
                                // lb_test.Text = Text;

                                //---This code is for detecting form A and form B point in line detachment---//

                                /*
                                Steps : here we do following task
                                1. Enable both the click option for now just enable.
                                2. We have node info in this->indexOfPrevPointForLineMovement and  this->indexOfNextNodeForLineMovement
                                */
                                disconnectLineFromAToolStripMenuItem.Enabled = true;
                               //disconnectLineFromBToolStripMenuItem.Enabled = true;

                                nodeAToolStripMenuItem.Enabled = true;
                                nodeBToolStripMenuItem.Enabled = true;
                                addMixNodeToolStripMenuItem.Enabled = true;//For adding mix node

                                FlagForDisconnectingLineChoice = 1;                              
                                //---End of line deteachment section --------------------//
                         
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

                                if(menuStripNodeInfoValues[i].temperature_source != "Mix") { 

                                if ((xValue > menuStripNodeInfoValues[i].xVal - 0.25 && xValue < menuStripNodeInfoValues[i].xVal + 0.25) && (yValue > menuStripNodeInfoValues[i].yVal - 0.25 && yValue < menuStripNodeInfoValues[i].yVal + 0.25))
                                {

                                    //--This is changed from int to string  code bbk305
                                    idSelected = menuStripNodeInfoValues[i].id; //Now this is a string 
                                    tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                    if (Cursor != Cursors.Cross)
                                    {
                                        Cursor = Cursors.Hand;
                                        //====This flag is for deleting the node===========//

                                        FlagForNodeDelete = 1;//flag is ready on Node selected
                                        nodeID_ForDeletingNode = idSelected;
                                        deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
                                        //=============end of flag for deleting===========//
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

                                        //====This flag is for deleting the node===========//

                                        FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                        deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
                                        //nodeID_ForDeletingNode = idSelected;
                                        //=============end of flag for deleting===========//

                                    }

                                }
                                    //--Lets filter out the mix nodes---

                                }//Close of if section Mix section

                                //==This one is special case for mix node only
                                else if (menuStripNodeInfoValues[i].temperature_source == "Mix")
                                {
                                    if ((xValue > menuStripNodeInfoValues[i].xVal - 0.25 && xValue < menuStripNodeInfoValues[i].xVal + 0.25) && (yValue > menuStripNodeInfoValues[i].yVal - 0.25 && yValue < menuStripNodeInfoValues[i].yVal + 0.25))
                                    {

                                        //--This is changed from int to string  code bbk305
                                        idSelected = menuStripNodeInfoValues[i].id; //Now this is a string 
                                        tempIndexForNode = i;//This is for finding other values with searching for this we need index
                                        if (Cursor != Cursors.Cross)
                                        {
                                            Cursor = Cursors.Hand;
                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 1;//flag is ready on Node selected
                                            nodeID_ForDeletingNode = idSelected;
                                            deleteNodeToolStripMenuItem.Enabled = true; //Turn on the delete buttton
                                                                                        //=============end of flag for deleting===========//
                                        }
                                        //this.Cursor = Cursors.Hand;
                                        //now this works so lets move forward.
                                        //===This should be dissabled
                                        // readyForMouseClick = 1;//enable on click event


                                        break;//this break is for if found the value no longer loop increases the perfomances..
                                    }
                                    else
                                    {
                                        if (Cursor != Cursors.Cross)
                                        {
                                            this.Cursor = Cursors.Arrow;
                                           /// readyForMouseClick = 0;//dissable on click event.

                                            //====This flag is for deleting the node===========//

                                            FlagForNodeDelete = 0;//flag is ready OFF , Node NOT SELECTED
                                            deleteNodeToolStripMenuItem.Enabled = false;//Turn of the delet button
                                                                                        //nodeID_ForDeletingNode = idSelected;
                                                                                        //=============end of flag for deleting===========//

                                        }

                                    }

                                }


                            }//Close of for loop
                        }//close of if menuStripAllValue>0


                        if (mouseClickAction == 1)
                        {

                            if (Control.ModifierKeys == Keys.Alt)
                            {
                                //--This alter key is for moving along constant x-axis ...
                                // MessageBox.Show(" alt is pressed for x axis constant");


                                //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                                menuStripNodeInfoValues[tempIndexForNode].yVal = yAxis1;//This value is changed...


                                //====================For mix node movement = ===============//

                                UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//

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
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}

                                    //--this is changed as well code :bbk305
                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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
                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);

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

                                //====================For mix node movement = ===============//

                                UpdateMixPointOnNodeMovement();

                                //=====================End of mix node movemnt===================//
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
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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

                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);

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


                                //***********************For mix node section functions******************//

                                //Function helps in updating the mix point x and y value when other node are moved
                                UpdateMixPointOnNodeMovement();
                                //************************End of the mix node ************************//
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
                                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].label;
                                    //}
                                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                                    //{
                                        labelValue = menuStripNodeInfoValues[x].name;
                                    //}
                                    //else
                                    //{
                                    //    labelValue = menuStripNodeInfoValues[x].source;
                                    //}


                                    // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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
                                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);

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



        /// <summary>
        /// Function update the mix values and helps in movement when the mouse 
        /// movement is done related values
        /// Function helps in updating the mix point x and y value when other node are moved
        /// </summary>
        public void UpdateMixPointOnNodeMovement()
        {
            /*
                         1.Identify the node that has been changed
                         2. Find the corresponding node and change its x and y coordinate accordingly

            */

            string nodeID = menuStripNodeInfoValues[tempIndexForNode].id;//This node has change

            temporaryMixNodeList.Clear();//Clearing the mix node
          //  for (int j = 0; j < menuStripNodeInfoValues.Count; j++)
            //{
                //if (nodeID == menuStripNodeInfoValues[j].id)
                //{
                    //For every node we need to check ok 
                    //if (menuStripNodeInfoValues[j].temperature_source == "Mix")
                    //{
                        //Only for mix hai
                        string mixNodePrevNodeId = "";
                        string mixNodeNextNodeId = "";
                        for (int i = 0; i < mixNodeInfoList.Count; i++)
                        {
                            //--we need to identify mix node
                            if (nodeID == mixNodeInfoList[i].previousNodeID)
                            {
                                // mixNodePrevNodeId = mixNodeInfoList[i].previousNodeID;
                                temporaryMixNodeList.Add(new TempDataTypeForMixNode
                                {
                                    chartID = mixNodeInfoList[i].chartID,
                                    nodeID = mixNodeInfoList[i].nodeID,
                                    nextNodeID = mixNodeInfoList[i].nextNodeID,
                                    previousNodeID = mixNodeInfoList[i].previousNodeID
                                });
                            }
                            else if (nodeID == mixNodeInfoList[i].nextNodeID)
                            {
                                //mixNodeNextNodeId = mixNodeInfoList[i].nextNodeID;
                                temporaryMixNodeList.Add(new TempDataTypeForMixNode
                                {
                                    chartID = mixNodeInfoList[i].chartID,
                                    nodeID = mixNodeInfoList[i].nodeID,
                                    nextNodeID = mixNodeInfoList[i].nextNodeID,
                                    previousNodeID = mixNodeInfoList[i].previousNodeID
                                });
                            }

                        }

                        //--Found mix node and previous node id 
                        //--Now lets get x1,y1 ,x2,y2, and m and n value

                       // break;//Exit form the loop no need to do more
                    //}//close of if mix value
               // }//Close of if
           // } //Close of for loop

            //Now lets update the node info form temporary mix nodelists
            for (int i = 0; i < temporaryMixNodeList.Count; i++)
            {
                string previousNodeID = temporaryMixNodeList[i].previousNodeID;
                string nextNodeID = temporaryMixNodeList[i].nextNodeID;
                double n = 0;//from next nodeid
                double m = 0;//from previous node id
                double x1 = 0;  //x1,y1 is for previous node id 
                double y1 = 0;
                double x2 = 0; //For next node id
                double y2 = 0;
                //Start the updating here
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    if (previousNodeID == menuStripNodeInfoValues[x].id)
                    {
                        //Previous node id update
                        m = menuStripNodeInfoValues[x].airFlow;
                        x1 = menuStripNodeInfoValues[x].xVal;
                        y1 = menuStripNodeInfoValues[x].yVal;
                    }
                    else if (nextNodeID == menuStripNodeInfoValues[x].id)
                    {
                        //Previous node id update
                        n = menuStripNodeInfoValues[x].airFlow;
                        x2 = menuStripNodeInfoValues[x].xVal;
                        y2 = menuStripNodeInfoValues[x].yVal;
                    }
                }

                double x_mix = ((n * x2) + (m * x1)) / (m + n);
                double y_mix = ((n * y2) + (m * y1)) / (m + n);

                //Now lets update the values in 
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    if (temporaryMixNodeList[i].nodeID == menuStripNodeInfoValues[x].id)
                    {
                        //update and break

                        menuStripNodeInfoValues[x].xVal = x_mix;
                        menuStripNodeInfoValues[x].yVal = y_mix;    //Update complete
                        break;
                    }
                }

            }//Close of for

        }


        /// <summary>
        /// Function for making changes in mix nodes value
        /// </summary>
        /// <param name="NodeIDOfChangedNode">NodeID of the node changed</param>
        public void DBUpdateMixPointOnNodeValueChange(string NodeIDOfChangedNode)
        {
            /*
                         1.Identify the node that has been changed
                         2. Find the corresponding node and change its x and y coordinate accordingly

            */

            string nodeID = NodeIDOfChangedNode; //menuStripNodeInfoValues[tempIndexForNode].id;//This node has change

            temporaryMixNodeList.Clear();//Clearing the mix node
                                         //  for (int j = 0; j < menuStripNodeInfoValues.Count; j++)
                                         //{
                                         //if (nodeID == menuStripNodeInfoValues[j].id)
                                         //{
                                         //For every node we need to check ok 
                                         //if (menuStripNodeInfoValues[j].temperature_source == "Mix")
                                         //{
                                         //Only for mix hai
            string mixNodePrevNodeId = "";
            string mixNodeNextNodeId = "";
            for (int i = 0; i < mixNodeInfoList.Count; i++)
            {
                //--we need to identify mix node
                if (nodeID == mixNodeInfoList[i].previousNodeID)
                {
                    // mixNodePrevNodeId = mixNodeInfoList[i].previousNodeID;
                    temporaryMixNodeList.Add(new TempDataTypeForMixNode
                    {
                        chartID = mixNodeInfoList[i].chartID,
                        nodeID = mixNodeInfoList[i].nodeID,
                        nextNodeID = mixNodeInfoList[i].nextNodeID,
                        previousNodeID = mixNodeInfoList[i].previousNodeID
                    });
                }
                else if (nodeID == mixNodeInfoList[i].nextNodeID)
                {
                    //mixNodeNextNodeId = mixNodeInfoList[i].nextNodeID;
                    temporaryMixNodeList.Add(new TempDataTypeForMixNode
                    {
                        chartID = mixNodeInfoList[i].chartID,
                        nodeID = mixNodeInfoList[i].nodeID,
                        nextNodeID = mixNodeInfoList[i].nextNodeID,
                        previousNodeID = mixNodeInfoList[i].previousNodeID
                    });
                }

            }

            //--Found mix node and previous node id 
            //--Now lets get x1,y1 ,x2,y2, and m and n value

            // break;//Exit form the loop no need to do more
            //}//close of if mix value
            // }//Close of if
            // } //Close of for loop

            //Now lets update the node info form temporary mix nodelists
            for (int i = 0; i < temporaryMixNodeList.Count; i++)
            {
                string previousNodeID = temporaryMixNodeList[i].previousNodeID;
                string nextNodeID = temporaryMixNodeList[i].nextNodeID;
                double n = 0;//from next nodeid
                double m = 0;//from previous node id
                double x1 = 0;  //x1,y1 is for previous node id 
                double y1 = 0;
                double x2 = 0; //For next node id
                double y2 = 0;
                //Start the updating here
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    if (previousNodeID == menuStripNodeInfoValues[x].id)
                    {
                        //Previous node id update
                        m = menuStripNodeInfoValues[x].airFlow;
                        x1 = menuStripNodeInfoValues[x].xVal;
                        y1 = menuStripNodeInfoValues[x].yVal;
                    }
                    else if (nextNodeID == menuStripNodeInfoValues[x].id)
                    {
                        //Previous node id update
                         n = menuStripNodeInfoValues[x].airFlow;
                        x2 = menuStripNodeInfoValues[x].xVal;
                        y2 = menuStripNodeInfoValues[x].yVal;
                    }
                }

                double x_mix = ((n * x2) + (m * x1)) / (m + n);
                double y_mix = ((n * y2) + (m * y1)) / (m + n);

                int mixAirFlowValue = (int)(m + n);

                //Now lets update the values in 
                for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                {
                    if (temporaryMixNodeList[i].nodeID == menuStripNodeInfoValues[x].id)
                    {
                        //update and break

                        menuStripNodeInfoValues[x].xVal = x_mix;
                        menuStripNodeInfoValues[x].yVal = y_mix;    //Update complete

                        //--Here also update the node values to database...
                        //UpdateMixNodeValuesToDB()
                       // MessageBox.Show("Mix node value change = " + menuStripNodeInfoValues[x].name + "\n xval" + x_mix + "\n yval= " + y_mix+"\n airflow = "+ mixAirFlowValue);
                        UpdateMixNodeValuesToDB(temporaryMixNodeList[i].nodeID, x_mix, y_mix, mixAirFlowValue);//==m+n 
                        break;
                    }
                }

            }//Close of for

        }

        public void UpdateMixNodeValuesToDB(string NodeID,double xValue,double yValue,int AirFlowValue)
        {
            //We need to be able to change the air flows as well                      
            // string tableName = "tbl_" + buildingName + "_node_value";//currentNodeTableFromDB;  
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

           // MessageBox.Show("HERE WE ARE"+ NodeID+"\n xValue = "+ xValue+"\n yval ="+yValue+"\n air flow= "+AirFlowValue);

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, airFlow=@airflow   where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xValue.ToString());
                command.Parameters.AddWithValue("@yVal", yValue.ToString());       
                command.Parameters.AddWithValue("@airflow", AirFlowValue);
                command.Parameters.AddWithValue("@id", NodeID);            
                command.ExecuteNonQuery();
            }

        }
           



        //--This is for the indicator showing part but it not implemented right now.
        private void IndicatorLineForNodeMovement(int idSelected, double x11, double y11, double x22, double y22)
        {
            //--This function basically reorders the chart controls when the alter key is pressed.
            //--To use this function we need to clear the seriesLineIndicator every time we call it...

            double x1 = x11;
            double y1 = y11;
            double x2 = x22;
            double y2 = y22;


            seriesLineIndicator.ChartType = SeriesChartType.FastLine;
            seriesLineIndicator.MarkerSize = 10;
            seriesLineIndicator.MarkerColor = Color.FromArgb(0, 255, 0);//--Light blue color....
            //seriesLineIndicator.BorderWidth = 3;
            //seriesLineIndicator.BorderDashStyle = ChartDashStyle.Dash;


            seriesLineIndicator.Points.Add(x1, y1);
            seriesLineIndicator.Points.Add(x2, y2);


        }


        //private void button8_Click(object sender, EventArgs e)
        //{
        //    //this is the database connection part...

        //    //lets check if the data is present or not it the arraylist if present then only perfom the insert.

        //    if (temp_AL.Count > 0)
        //    {

        //        int count = temp_AL.Count;
        //        // the insertion part here...

        //        for (int i = 0; i < count; i++)
        //        {

        //            string q = "insert into tbl_temp_humidity(temperature,humidity)values ('" + double.Parse(temp_AL[i].ToString()) + " ',  '" + double.Parse(hum_AL[i].ToString()) + "')";
        //            insert_in_db(q);

        //        }

        //        MessageBox.Show(WFA_psychometric_chart.Properties.Resources.db_insertion_success);



        //    }//close of if

        //    //btn_insert_values.Enabled = false;

        //}

        //private void heatMapToolStripMenuItem_Click(object sender, EventArgs e)
        //{
        //    //lets add heat map form to the application ...
        //    form_heat_map fm_hm = new form_heat_map(this);//--This is done because we are making the form1_main change the values to main form ie form1_main
        //    fm_hm.Show();

        //}

        private void exportDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_export_data formExportData = new form_export_data();
            formExportData.Show();

        }

        private void processDiagramToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //This is the process diagram part and it just calls the another form for the process part .
            form_process_diagram frmProcessDiagram = new form_process_diagram();
            frmProcessDiagram.Show();
        }

        private void insert_in_db(string q)
        {
            try
            {
                con.Open();
                cmd.CommandText = q;
                cmd.ExecuteNonQuery();
                con.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                con.Close();
            }
        }

        private void psychrometricCalculatorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form_Psychrometric_Calculator fpc = new Form_Psychrometric_Calculator();
            fpc.Show();


        }

        private void button7_Click(object sender, EventArgs e)
        {
            /*
            try
            {

                if (tb_city_name.Text != "")
                {
                    //if not null the perform geo coding..
                        var address = tb_city_name.Text;
                        var requestUri = string.Format("http://maps.googleapis.com/maps/api/geocode/xml?address={0}&sensor=false", Uri.EscapeDataString(address));

                        var request = WebRequest.Create(requestUri);
                        var response = request.GetResponse();
                       
                        var xdoc = XDocument.Load(response.GetResponseStream());

                        var result = xdoc.Element("GeocodeResponse").Element("result");
                      //  MessageBox.Show(result.ToString());
                        var locationElement = result.Element("geometry").Element("location");
                        var lat = locationElement.Element("lat");
                        var lng = locationElement.Element("lng");
                        double lat2 = Double.Parse(lat.Value);
                        double lng2 = Double.Parse(lng.Value);

                       // MessageBox.Show(lat.ToString());
                       // MessageBox.Show(lat2.ToString());
                       // double lat_val = double.Parse(lat.ToString());
                       // double long_val = double.Parse(lng.ToString());
                  
                     tb_lat.Text = lat2.ToString();
                        tb_long.Text = lng2.ToString();
                     

             //double lat =   GetCoordinatesLat(tb_city_name.Text);
             //double lng = GetCoordinatesLng(tb_city_name.Text);
             //tb_lat.Text = lat.ToString();
             //tb_long.Text = lng.ToString();

                        btn_plot_values.Enabled = true;
                        btn_update_constantly.Enabled = true;
                }
                else
                {
                    MessageBox.Show("Please enter a valid location (country,city)");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            */
        }



        private void UpdateDataConstantly()
        {
            /*
            //resetting arrarylist..         
            temp2_AL.Clear();
            hum2_AL.Clear();
            //We are using JSON.NET library to parse the json file and get the data form it..

            if (tb_lat.Text != null && tb_long.Text != null)
            {
                try
                {

                    using (var wc = new WebClient())
                    {
                        // var json = await httpClient.GetStringAsync(api_url);
                        //pulling the saved data form text file....
                        string path = AppDomain.CurrentDomain.BaseDirectory + @"long_lat_value.txt";
                        string[] lines = System.IO.File.ReadAllLines(path);


                        double lat_val = Double.Parse(lines[0]);
                        double lng_val = Double.Parse(lines[1]);
                        string api_url = "http://api.openweathermap.org/data/2.5/station/find?lat=" + lat_val + "&lon=" + lng_val + "&APPID=615afd606af791f572a1f92b27a68bcd";
                        var data = wc.DownloadString(api_url);                    
                       // MessageBox.Show("Data = " + data);                
                        try
                        {
                            var jArray = JArray.Parse(data);                     
                            foreach (var result in jArray.Children<JObject>())
                            {
                                try
                                {

                                    if ((result["last"]["main"]["temp"] != null) && (result["last"]["main"]["humidity"] != null))
                                    {
                                        string tem = result["last"]["main"]["temp"].ToString();



                                       // MessageBox.Show("tem = " + tem);
                                        double kelvin = double.Parse(tem);
                                        double degree = Math.Round(kelvin - 273.15);
                                        temp_AL.Add((int)degree);


                                        //for humidity part
                                        string tem2 = result["last"]["main"]["humidity"].ToString();
                                        //lets divide the humidity by 100 to convert it to decimal value...
                                        double hum = double.Parse(tem2);
                                        //MessageBox.Show("hum = " + hum);
                                        hum_AL.Add(hum);



                                    }
                               
                                }
                                catch (Exception ex)
                                {
                                    MessageBox.Show("bbk message = " + ex.Message);
                                }
                            }

                            //now lets test these
                            //string test = null;
                            //string test2 = null;

                            //for (int i = 0; i < temp_AL.Count; i++)
                            //{
                            //    test += temp_AL[i] + " , \t";
                            //    test2 += hum_AL[i] + ", \t";

                            //}
                            //MessageBox.Show("temperature pulled t = " + test);
                            //MessageBox.Show("humidity pulled h = " + test2);
                            //test = null;
                          //  test2 = null;
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("exception = " + ex.Message);
                        }
                        //now lets load the map if not loaded if loaded dont need to load the map..
                        try
                        {
                            if (map_loaded == 0)
                            {
                                //not loaded so load..
                                button1.PerformClick();
                            }

                            //testing..
                           // string s = null;

                            for (int i = 0; i < temp_AL.Count; i++)
                            {
                                //calling the plot function to plot the values...
                                double DBT = double.Parse(temp_AL[i].ToString());
                                double RH = (double)double.Parse(hum_AL[i].ToString()) / 100;
                            //    s += "(DBT = " + DBT + ",HR= " + RH + ") \n";
                                //plot_by_DBT_HR(t, h);//div by 100 because 34% = 34/100 so..
                                plot_by_DBT_HR(DBT, RH);
                            }

                          //  MessageBox.Show("val = " + s);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                        }

                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }//close of if...

            */
        }
        private System.Windows.Forms.Timer timer1;
        public void InitTimer()
        {
            timer1 = new System.Windows.Forms.Timer();
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Interval = 1000 * 60; // in miliseconds //2min * 30 = 60 min minute ie every 1 hour
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // MessageBox.Show("pulling...");
            UpdateDataConstantly();
            // MessageBox.Show("pulled...");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            //this code basically makes the upadating part constantly...
            //it calls the function UpdateDataConstantly()           
            //this function basically calls every  50 minuest...
            InitTimer();
            //MessageBox.Show("success");

        }

        private void weatherServicesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Form3 f3 = new Form3();
            //f3.Show();


        }

        private void insertNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //upon this click the form should pop up

            Form_Input_For_Seriespoint form_point_input = new Form_Input_For_Seriespoint(this);
            form_point_input.Show();

        }
      public  Series series1 = new Series("My Series");

        string setItemSelectedID;

        int oneTimeClick = 1;
        //int twoTimeClick = 0;
        //int incrementIndex = 0;
        int mouseClickAction = 0;

        string idSelected;
        int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..

        bool arrowOn = false;

        double xAxis1;
        double yAxis1;

        int load = 0;//false

        /// <summary>
        /// This information updates the node information 
        /// </summary>
        /// <param name="id">Nodeid</param>
        /// <param name="xVal">x coordinate</param>
        /// <param name="yVal">y coordinate</param>
        /// <param name="source">source of node</param>
        /// <param name="name">name of the node</param>
        /// <param name="label">label of node</param>
        /// <param name="colorValue">color value of node</param>
        /// <param name="showItemText">which text to show</param>
        /// <param name="nodeSizeValue">node size value</param>

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





        public void UpdateNodeInfoToDBForTemeperatureFromHardware(string id, double xVal)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal , lastUpdatedDate=@date   where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                //command.Parameters.AddWithValue("@yVal", yVal.ToString());
                //command.Parameters.AddWithValue("@source", source);
                //command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                //command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                //  command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@date", DateTime.Now.ToString());
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn

        public void UpdateNodeInfoToDBForHumidityFromHardware(string id, double yVal)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set   yValue=@yVal, lastUpdatedDate=@date  where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
               // command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                //command.Parameters.AddWithValue("@source", source);
                //command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                //command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@date", DateTime.Now.ToString());
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn








        /// <summary>
        /// Updates the node info in db using Temperature 
        /// Device input value
        /// </summary>
        /// <param name="id"></param>
        /// <param name="xVal"></param>
        /// <param name="yVal"></param>
        /// <param name="source"></param>
        /// <param name="name"></param>
        /// <param name="label"></param>
        /// <param name="colorValue"></param>
        /// <param name="showItemText"></param>
        /// <param name="nodeSizeValue"></param>
        public void UpdateNodeInfoToDBFromTemperatureDeviceSource(string id, double xVal, double yVal, string TemperatureSource, string HumiditySource,string name, Color colorValue, int airFlow, int nodeSizeValue)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";//currentNodeTableFromDB;  

            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                string sql_string = "UPDATE " + tableName + "   set  xValue =@xVal ,  yValue=@yVal, temperature_source=@tempSource,humidity_source=@humSource ,name=@name,  colorValue=@colorVal, airFlow=@airflow , nodeSize=@node_size_value ,lastUpdatedDate= @date  where nodeID  =@id";
                SQLiteCommand command = new SQLiteCommand(sql_string, connection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@xVal", xVal.ToString());
                command.Parameters.AddWithValue("@yVal", yVal.ToString());
                // command.Parameters.AddWithValue("@source", source);
                command.Parameters.AddWithValue("@tempSource", TemperatureSource);
                command.Parameters.AddWithValue("@humSource", HumiditySource);
                command.Parameters.AddWithValue("@name", name);
                //command.Parameters.AddWithValue("@label", label);
                command.Parameters.AddWithValue("@colorVal", ColorTranslator.ToHtml(colorValue));
                //command.Parameters.AddWithValue("@text", showItemText);
                command.Parameters.AddWithValue("@airflow", airFlow.ToString());
                command.Parameters.AddWithValue("@node_size_value", nodeSizeValue);
                command.Parameters.AddWithValue("@date", DateTime.Now); //This is the date presented at what time the update  was performed
                command.Parameters.AddWithValue("@id", id);
                //MessageBox.Show("selected value = " + cb_station_names.SelectedItem.ToString());
                command.ExecuteNonQuery();
            }

        }//--close of insertnodeinfotodb fxn










        /// <summary>
        /// This one holds the series name which is currently selected at edit comfortzone 
        /// mode
        /// </summary>
        string comfortZoneBorderSeriesCurrentlySelected = "";
        /// <summary>
        /// This varaiable is for name comfort zone as 
        /// name_1 ,name_2,name_3 everytime it is created
        /// </summary>
        int countForComfortZone = 1;
        private void chart1_MouseClick(object sender, MouseEventArgs e)
        {
            //--Check of enable or dissable the cell click
            if (FlagForDissableLeftAndRightClicksInChart == 1)
            {
                //FlagForDissableLeftAndRightClicksInChart = 0;
                return;//DO not proceed forward
            }
            
            if(flagForEditComfortZoneGraphically == 1)
            {

                //First click then 1 second click then 2
                if ((Cursor == Cursors.SizeWE || Cursor == Cursors.SizeNS))
                {
                    if (selectedSeriesForMoveForEditComfortZone != "")
                    {
                        // Cursor = Cursors.SizeWE;
                        // selectedSeriesForMoveForEditComfortZone = "Vertical_Temp_Left_Side";
                        flagForBorderLineSelectedForMoveForEditCF += 1;
                        comfortZoneBorderSeriesCurrentlySelected = selectedSeriesForMoveForEditComfortZone;
                    }

                }

                if (flagForBorderLineSelectedForMoveForEditCF == 0)
                {
                    ////If the button is clicked outside then we have flagForBorderLineSelectedForMoveForEditCF = 3
                    //this should be done only when clicked outside the chart so use condition
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
                    double hum = CalculateHumidityFromXandYCoordinate(xValue, yValue);//This gets the humidity 

                    if (( hum < int.Parse(listchartComfortZoneInfoSingle[0].min_hum) || hum > int.Parse(listchartComfortZoneInfoSingle[0].max_hum)) ||(xValue < int.Parse(listchartComfortZoneInfoSingle[0].min_temp) || xValue> int.Parse(listchartComfortZoneInfoSingle[0].max_temp))) 
                    {
                        flagForBorderLineSelectedForMoveForEditCF = 3;
                    }
                   
                }

                if (flagForBorderLineSelectedForMoveForEditCF == 3)
                {
                    /*
                   Means create new chart if the data is updated to new values  
                   */
                    
                    //Checking if current value matches with previous values or not if so then do not do anything
                    if ( ( minTemperatureForNewComfortZoneCreate != 0 )&& ( maxTemperatureForNewComfortZoneCreate != 0) && (minHumidityForNewComfortZoneCreate != 0) &&( maxHumidityForNewComfortZoneCreate != 0))
                    {
                        //If the values has changed then default then only change else no change 
                        //Lets get the previous chart value to make new chart 
                        string name = listchartComfortZoneInfoSingle[0].name;
                        int minTempOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].min_temp);
                        int maxTempOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                        int minHumidityOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                        int maxHumidityOfPrevComfortzone = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);
                        Color prevComfortzoneColor = listchartComfortZoneInfoSingle[0].colorValue;

                        if (((minTemperatureForNewComfortZoneCreate == minTempOfPrevComfortzone) && (maxTemperatureForNewComfortZoneCreate == maxTempOfPrevComfortzone) && (minHumidityForNewComfortZoneCreate == minHumidityOfPrevComfortzone) && (maxHumidityForNewComfortZoneCreate == maxHumidityOfPrevComfortzone)))
                        {
                            //If equal do not do any thing 
                            //Refreshing the chart
                            if (dataGridView1.Rows.Count > 0)  //If there is data then only do this one
                            {
                                //set parameters of your event args
                                var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex);
                                // or setting the selected cells manually before executing the function
                                dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                                dataGridView1_CellClick(sender, eventArgs);
                            }

                        }
                        else {
                            //If they do not match then only create new comfort zone and assign them the value
                            /*Steps "
                            1.Clear the existing chart first
                            2.Create the new chart with new values and new name 
                            3.Update the chart list 
                            */
                            // MessageBox.Show("Chart creating and updating");

                            //Clearing the border lines 
                            ClearSeriesInChart("Vertical_Temp_Left_Side");
                            ClearSeriesInChart("Vertical_Temp_Right_Side");
                            ClearSeriesInChart("Horizontal_Hum_Down_Side");
                            ClearSeriesInChart("Horizontal_Hum_Upper_Side");


                            //Clearing the chart 
                            ClearComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum));

                            //We need to generate name
                            //First splite if underscore is present
                            string[] word = name.Split('_');


                            string newComfortZoneName = "";
                            do
                            {
                                newComfortZoneName = word[0] + "_" + countForComfortZone++;
                            } while (newComfortZoneName == name);


                            PlotComfortZone(minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, prevComfortzoneColor, newComfortZoneName);

                            //Now we need to insert new comfort zone now  
                            InsertComfortZoneValue(newComfortZoneName, minTemperatureForNewComfortZoneCreate, maxTemperatureForNewComfortZoneCreate, minHumidityForNewComfortZoneCreate, maxHumidityForNewComfortZoneCreate, prevComfortzoneColor);

                            //After inserting updating the comfort zone info as well
                            insertOrUpdateComfortChartSetting(chartDetailList[indexOfChartSelected].chartID, temporaryComfortZoneID_Store);


                            //--Refresh the comfort zone

                            if (dataGridView1.Rows.Count > 0)  //If there is data then only do this one
                            {
                                //set parameters of your event args
                                var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex);
                                // or setting the selected cells manually before executing the function
                                dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                                dataGridView1_CellClick(sender, eventArgs);
                            }



                        }
                        flagForEditComfortZoneGraphically = 0;//Resetting flag


                    }
                }









                    //Now we do others task

                    //This one is last one when released go back to 0 initial state not selected
                    if (flagForBorderLineSelectedForMoveForEditCF== 2)
                {
                    flagForBorderLineSelectedForMoveForEditCF = 0;
                }





            }
            else { 

            //This flagForDisconnectClick is for the line not node
            if (flagForDisconnectClick == 1)
            {
                  //--This one is for line
                if (flagNodeSelectedForConnect == 1)  //Node selected for connect 
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
                //Updating values in database  //This part is of node movement and update not line
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                    {
                            // UpdateNodeInfoToDB(menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].showItemText, menuStripNodeInfoValues[x].marker_Size);
                            DatabaseOperations Obj1 = new DatabaseOperations();
                            Obj1.UpdateNodeInfoToDBWithNewParameters(CurrentSelectedBuilding,menuStripNodeInfoValues[x].id, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].marker_Size,(int)menuStripNodeInfoValues[x].airFlow);
                    }
                }
                NodeSelectionAndRelease(e);
                //The node update function should be placed here and the calling 
                //to the redrawing function has to be done  after that.

            }


            }//Close of else of edit mode
        }//Close of mouse click event 

        //--Lets have indexID here for line and find its corresponding index value here
        string idOfLineSelecteForDisconnect = "";

        //--This function is for resetting the stored values..
        int indexOfLineInTheList = 0; //This holds the id of the line selected.
        public void ResettingLines()
        {
            if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 0)
            {
                //No setting is done so return
                //MessageBox.Show("nO UPDATE"); 
                return;
            }

             if(flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 1) //From prevNode
            { 
              //  menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected; //Id of the node selected and

            menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId = idOfNodeSelected; //Id of the node selected and
                                                                                             //--Lets update the data here...
            if (flagForInsertOrUpdateDataToDB == 1)
                {
                //Testing...
                //MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                //--This is the update function for line values..
                // UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, idOfNodeSelected, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness);
                UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId , menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness, menuStripNodeLineInfoValues[indexOfLineInTheList].name, menuStripNodeLineInfoValues[indexOfLineInTheList].status);
            }
            }
            else if (flagForDisconnectLineFromNode_PrevNode_Or_NextNode == 2)
            {
                menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId = idOfNodeSelected;
                //--Lets update the data here...
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    //Testing...
                   // MessageBox.Show( " index = "+ indexOfLineInTheList + ", previd"+ menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId+",nextid idsnodeseleted ="+ idOfNodeSelected+", color = " +menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue+"seriesid = "+ menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID);

                    //--This is the update function for line values..
                    UpdateLineInfoToDB(menuStripNodeLineInfoValues[indexOfLineInTheList].ID, menuStripNodeLineInfoValues[indexOfLineInTheList].prevNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].nextNodeId, menuStripNodeLineInfoValues[indexOfLineInTheList].lineColorValue, menuStripNodeLineInfoValues[indexOfLineInTheList].lineSeriesID, menuStripNodeLineInfoValues[indexOfLineInTheList].lineThickness, menuStripNodeLineInfoValues[indexOfLineInTheList].name, menuStripNodeLineInfoValues[indexOfLineInTheList].status);
                }

            }


          


        }


        public void ReDrawingLineAndNode()
        {

            try { 

            lock (this) { 
            if (menuStripNodeInfoValues.Count > 0)
            {
                //--This is for replotting all the things again...
     

                if (chart1.InvokeRequired)
                {
                    chart1.Invoke(new Action(() =>series1.Points.Clear()));
                }
                else
                {
                    //lb_device_status.Text = "connected";
                    series1.Points.Clear();
                }
                
                //series1.Points.Clear();
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
                    //if (menuStripNodeInfoValues[x].showItemText == "Label")
                    //{
                    //    labelValue = menuStripNodeInfoValues[x].label;
                    //}
                    //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                    //{
                        labelValue = menuStripNodeInfoValues[x].name;
                    //}
                    //else
                    //{
                    //    labelValue = menuStripNodeInfoValues[x].source;
                    //}


                    //--Redefined code bbk305
                    ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source,  menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);

                    //CODE : BBK305A
                    //--incrementIndex++;
                    indexForSeriesNodePoint++;

                }
                //--resetting incrementIndex
                //CODE:BBK305A  
                // incrementIndex = 0;

                if (menuStripNodeLineInfoValues.Count > 0)
                {

                    for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
                    {
                        //  incrementIndex++;


                        //--tHIS IS REDEFINED code bbk305
                        ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);

                    }

                }

                if (chart1.InvokeRequired) { 
                chart1.Invoke(new Action(()=> chart1.Invalidate()));//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.
                }
                else
                {
                    chart1.Invalidate();//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.

                }
                //CODE :BBK305A \\
                //incrementIndex = 0;//reset the values again..
                indexForSeriesNodePoint = 0;//Resetting the value
            }//Close of if menustripnodeinfovalues

             }//==Close of the lock

            } catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }//--Close of the actual function....

        int indexForSeriesNodePoint = 0;
        // int indexForSeriesLine = 0;
        public void ReDrawingLineAndNodeForLoadXML()
        {
            //if (menuStripNodeInfoValues.Count > 0)
            //{
            //    //--This is for replotting all the things again...
            //    series1.Points.Clear();
            //    for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
            //    {
            //        //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
            //        menuStripNodeLineInfoValues[i].lineSeriesID.Points.Clear();
            //    }
            //    //--this is redraw functionality
            //    //foreach(var values in menuStripNodeInfoValues)
            //    //--Resetting the index value
            //    incrementIndex = 0;
            //    for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
            //    {
            //        string labelValue;
            //        if (menuStripNodeInfoValues[x].showItemText == "Label")
            //        {
            //            labelValue = menuStripNodeInfoValues[x].label;
            //        }
            //        else if (menuStripNodeInfoValues[x].showItemText == "Name")
            //        {
            //            labelValue = menuStripNodeInfoValues[x].name;
            //        }
            //        else
            //        {
            //            labelValue = menuStripNodeInfoValues[x].source;
            //        }


            //        //--Redefined code bbk305
            //        ReDrawPointsForXML(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue);

            //        //CODE : BBK305A
            //        incrementIndex++;
            //        index++;
            //        //indexForSeriesNodePoint++;

            //    }
            //    //--resetting incrementIndex
            //    //CODE: BBK305A  
            //    incrementIndex = 0;

            //    if (menuStripNodeLineInfoValues.Count > 0)
            //    {

            //        for (int x = 0; x < menuStripNodeLineInfoValues.Count; x++)
            //        {
            //            incrementIndex++;


            //            //--tHIS IS REDEFINED code bbk305
            //            ReDrawLinesForXML(menuStripNodeLineInfoValues[x].ID.ToString(), menuStripNodeLineInfoValues[x].prevNodeId.ToString(), menuStripNodeLineInfoValues[x].nextNodeId.ToString(), menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue);

            //        }

            //    }

            //    chart1.Invalidate();//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.

            //    //CODE :BBK305A \\
            //    incrementIndex = 0;//reset the values again..
            //    //indexForSeriesNodePoint = 0;//Resetting the value
            //}//Close of if menustripnodeinfovalues

        }//--Close of the actual function....



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
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}


                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                                labelValue = menuStripNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}


                            //   ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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

                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);

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
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}
                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                                labelValue = menuStripNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}


                            // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);


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
                            //if (menuStripNodeInfoValues[x].showItemText == "Label")
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].label;
                            //}
                            //else if (menuStripNodeInfoValues[x].showItemText == "Name")
                            //{
                                labelValue = menuStripNodeInfoValues[x].name;
                            //}
                            //else
                            //{
                            //    labelValue = menuStripNodeInfoValues[x].source;
                            //}


                            // ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].source, menuStripNodeInfoValues[x].name, menuStripNodeInfoValues[x].label, labelValue, menuStripNodeInfoValues[x].marker_Size);
                            ReDrawPoints(series1, menuStripNodeInfoValues[x].xVal, menuStripNodeInfoValues[x].yVal, menuStripNodeInfoValues[x].colorValue, menuStripNodeInfoValues[x].temperature_source, menuStripNodeInfoValues[x].humidity_source, menuStripNodeInfoValues[x].name, labelValue, menuStripNodeInfoValues[x].marker_Size);
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
                                ReDrawLines(menuStripNodeLineInfoValues[x].ID, menuStripNodeLineInfoValues[x].prevNodeId, menuStripNodeLineInfoValues[x].nextNodeId, menuStripNodeLineInfoValues[x].lineSeriesID, menuStripNodeLineInfoValues[x].lineColorValue, menuStripNodeLineInfoValues[x].lineThickness, menuStripNodeLineInfoValues[x].name, menuStripNodeLineInfoValues[x].status);


                            }

                        }

                        chart1.Invalidate();
                        // incrementIndex = 0;//reset the values again..
                        indexForSeriesNodePoint = 0;//Resetting again

                    }//closing of key else part
                }//closing of second click



            }//closing of else block




        }


        private void humiditySensorCalibrationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Application_Form4 ap_f4 = new Application_Form4(this);
            //ap_f4.Show();


        }
        private void realTimePlottingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // this is the part of real time plotting where we try to plot the different previous values ...
            //Form5_real_time_plot f5 = new Form5_real_time_plot(this);
            //f5.Show();
            //form_app_timer f5 = new form_app_timer(this);
            //f5.Show();

        }

        int incrementIndex = 0;//--Defining the index

        //--this class is used for storing temporary the values of id xCoord,yCoord,source,name,label,color 
        //--so that a line could be plotted in it and we can do some processing as well 

        //--This information is required for node and node only
        public class TempDataType
        {
            public string id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            public double yVal { get; set; }
            // public string source { get; set; }

             public string temperature_source { get; set; }
            public string humidity_source { get; set; }

            public string name { get; set; }
            //public string label { get; set; }
            public Color colorValue { get; set; }
           // public string showItemText { get; set; } //--No need now for this one
            public int marker_Size { get; set; }

            public double airFlow { get; set; }
            public string lastUpdatedDate { get; set; }
            

        }
        public List<TempDataType> menuStripNodeInfoValues = new List<TempDataType>();
        //--This one right here is for editing the lines...
        public class lineNodeDataType
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

        public List<lineNodeDataType> menuStripNodeLineInfoValues = new List<lineNodeDataType>();

        public void ReDrawPoints(Series s1, double x, double y, Color c, string source_temperature, string source_humidity ,string name1,  string labelValueText, int marker_size_value)
        {
            // lock (this) { 

            //s1.ChartType = SeriesChartType.Point;

            //string s = "source => temp:" + source_temperature+",hum : "+source_humidity + "\n Name : " + name1 + "\nLable : " + label1x;// + "\nindex=" + indexForSeriesNodePoint;
            string s = "source \t\n temp: " + source_temperature + ",\t\n hum : " + source_humidity + "\n Name : " + name1;// + "\nindex=" + indexForSeriesNodePoint;
            if (chart1.InvokeRequired)
            {
                chart1.Invoke(new Action(() => s1.ChartType = SeriesChartType.Point));
                chart1.Invoke(new Action(() => s1.MarkerSize = marker_size_value));//= 20;
                chart1.Invoke(new Action(() => s1.MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points.AddXY(x, y)));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText));
                //chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label));
                chart1.Invoke(new Action(() => chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].Color = c));
                chart1.Invoke(new Action(() => s1.Points[indexForSeriesNodePoint].MarkerSize = marker_size_value));
               
            }
            else
            {
                s1.ChartType = SeriesChartType.Point;
                s1.MarkerSize = marker_size_value;//= 20;
                s1.MarkerStyle = MarkerStyle.Circle;
                s1.Points.AddXY(x, y);
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].ToolTip = s;
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].Label = labelValueText;
                chart1.Series["My Series"].Points[indexForSeriesNodePoint].Color = c;
                s1.Points[indexForSeriesNodePoint].MarkerStyle = MarkerStyle.Circle;
                s1.Points[indexForSeriesNodePoint].Color = c;
                s1.Points[indexForSeriesNodePoint].MarkerSize = marker_size_value;

            }

          //  }//--Close of lock 

        }

       public  double humidityCalculated = 0;
       public double enthalpyCalculated = 0;

        Series newLineSeries;//--This is temporary for storing series name


        //This is used to stroe node info in 0 index (for start)       
        //and in 1 index for end
        List<TempDataType> temporaryNodeValueStoreForRedrawLine = new List<TempDataType>();
        //--Redraw line function
        public void ReDrawLines(string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color c, int thickness_value,string name,int status)
        {
           // lock (this) { 

            // if (incrementIndex > 0)
            //  if(indexForSeriesNodePoint>0) //This index is resetted later

            //   {
            temporaryNodeValueStoreForRedrawLine.Clear();//Clearing the values of the list
                                                         // MessageBox.Show("ReDrawLines FRIST LINE");


            double startHumidity1 = 0;
            double startEnthalpy1 = 0;
            double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
            double endEnthalpy1 = 0;
            double startSpecificVolume1 = 0;//--specific volume
            double endSpecificVolume1 = 0;


            if (chart1.InvokeRequired) {
                    //now lets plot lines between tow points...
                    chart1.Invoke(new Action(() => newLineSeries = lineSeriesID));//new Series("LineSeries" + incrementIndex); //lineSeriesID; 

                
                  if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                    {
                        //--This  means the series is present....
                        chart1.Invoke(new Action(() => chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name)) )); 
                    }

                    //}
                    chart1.Invoke(new Action(() => newLineSeries.MarkerSize = 1));
                    chart1.Invoke(new Action(() => newLineSeries.ChartType = SeriesChartType.Line));
                    if (thickness_value <= 0)
                    {
                            chart1.Invoke(new Action(() => newLineSeries.BorderWidth = 3));
                    }
                    else
                    {
                            chart1.Invoke(new Action(() => newLineSeries.BorderWidth = thickness_value));//This is the thickness of lines
                    }
                    //newSeries.ToolTip = 
                    chart1.Invoke(new Action(() => newLineSeries.Color = c));
            }
            else
            {

                newLineSeries = lineSeriesID;//new Series("LineSeries" + incrementIndex); //lineSeriesID; 
                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name));
                }
                newLineSeries.MarkerSize = 1;
                newLineSeries.ChartType = SeriesChartType.Line;
                if (thickness_value <= 0)
                {
                    newLineSeries.BorderWidth = 3;
                }
                else
                {
                    newLineSeries.BorderWidth = thickness_value;//This is the thickness of lines
                }
                newLineSeries.Color = c;

            }


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
                    temporaryNodeValueStoreForRedrawLine.Add(new TempDataType
                    {
                        id = menuStripNodeInfoValues[i].id,
                        xVal = menuStripNodeInfoValues[i].xVal,
                        yVal = menuStripNodeInfoValues[i].yVal,
                       // source = menuStripNodeInfoValues[i].source,
                       temperature_source = menuStripNodeInfoValues[i].temperature_source,
                       humidity_source = menuStripNodeInfoValues[i].humidity_source,
                        name = menuStripNodeInfoValues[i].name,
                       // label = menuStripNodeInfoValues[i].label,
                       // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size,
                        airFlow = menuStripNodeInfoValues[i].airFlow

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
                    temporaryNodeValueStoreForRedrawLine.Add(new TempDataType
                    {
                        id = menuStripNodeInfoValues[i].id,
                        xVal = menuStripNodeInfoValues[i].xVal,
                        yVal = menuStripNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = menuStripNodeInfoValues[i].temperature_source,
                        humidity_source = menuStripNodeInfoValues[i].humidity_source,
                        name = menuStripNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size,
                        airFlow = menuStripNodeInfoValues[i].airFlow

                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStoreForRedrawLine.Count > 0)
            {

                //--this sets the initial values of humidity and enthalpy
                CalculateHumidityEnthalpy(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal);
                startHumidity1 = Math.Round(humidityCalculated, 2);
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                startSpecificVolume1 = SpecificVolumeReturn;
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)temporaryNodeValueStoreForRedrawLine[1].xVal, (double)temporaryNodeValueStoreForRedrawLine[1].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                endSpecificVolume1 = SpecificVolumeReturn;

                // MessageBox.Show("Start hum" + startHumidity1 + " end enth" + endEnthalpy1);
                //MessageBox.Show("menustripinfovalues[prevNodeID].xVal=" + menuStripNodeInfoValues[prevNodeID].xVal + "menuStripNodeInfoValues[nextNodeID].yVal=" + menuStripNodeInfoValues[nextNodeID].yVal + "menuStripNodeInfoValues[nextNodeID].xVal = "+ menuStripNodeInfoValues[nextNodeID].xVal + " menuStripNodeInfoValues[nextNodeID].yVal" + menuStripNodeInfoValues[nextNodeID].yVal);

                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

                //                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + @"                 start             end 
                //" + "Temp         :" + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "               " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;

                string tooltipString = "";
                //"            " + sequenceDetected + " \n" + @"                 "+ temporaryNodeValueStoreForRedrawLine[0].name + "             end 
                //" + "Temp         :" + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "               " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;


                string ZeroLine = "Process:  " + name + " ";
                string FirstLine = @"Parameters                      " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
                string SecondLine = @"DBT                                   " + "\x00B0 C                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                           " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
                string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                string FourthLine = @"Humidity Ratio                " + "Kg/Kg dryair  " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                       " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);
                string FifthLine = "Volume Flow Rate           " + "m\xB3/s                " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2) + "                      " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2);

                string SixthLine = "Specific Volume              " + "m\xB3/Kg             " + startSpecificVolume1 + "                    " + endSpecificVolume1;
                double massFlowRate1 = temporaryNodeValueStoreForRedrawLine[0].airFlow / startSpecificVolume1;
                double massFlowRate2 = temporaryNodeValueStoreForRedrawLine[1].airFlow / endSpecificVolume1;

                string SeventhLine = @"Mass flow rate(dry air)   " + "Kg(dry air)/s   " +Math.Round(massFlowRate1,2) + "                        " +Math.Round( massFlowRate2,2);
                string EighthLine = @"Enthalpy                           " + "KJ/Kg              " + startEnthalpy1 + "                       " + endEnthalpy1;
                double totalEnthalpyFlow1 = massFlowRate1 * startEnthalpy1;
                double totalEnthalpyFlow2 = massFlowRate2 * endEnthalpy1;
                string NinthLine = @"Total Enthalpy Flow         " + "KJ/s                " +Math.Round( totalEnthalpyFlow1,2) + "                      " +Math.Round( totalEnthalpyFlow2,2);
                double heatChange = totalEnthalpyFlow2 - totalEnthalpyFlow1;
                string TenthLine = @"Heat Change                    " + "KW                  " +Math.Round(heatChange,2) + "                     ";
                tooltipString = ZeroLine+"\n"+ FirstLine + "\n" + SecondLine + "\n" + ThirdLine + "\n" + FourthLine + "\n" + FifthLine + "\n" + SixthLine + "\n" + SeventhLine + "\n" + EighthLine + "\n" + NinthLine + "\n" + TenthLine;


                //===============This one is for datatable======================================================//
                //DataTable table = new DataTable();
                //table.Columns.Add("Parameters", typeof(string));
                //table.Columns.Add("Units", typeof(string));
                //table.Columns.Add(temporaryNodeValueStoreForRedrawLine[0].name.ToString(), typeof(string));
                //table.Columns.Add(temporaryNodeValueStoreForRedrawLine[1].name.ToString(), typeof(string));

                //table.Rows.Add("DBT", "\x00B0 C", Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2).ToString());
                //table.Rows.Add("Relative Humidity", "%  ", startHumidity1.ToString(), endHumidity1.ToString());
                //table.Rows.Add("Humidity Ratio", "Kg/Kg dryair", Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2).ToString());
                //table.Rows.Add("Volume flow rate", "m\xB3/s ", Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2).ToString(), Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2).ToString());
                //table.Rows.Add("Sp. Volume", "m\xB3/Kg", startSpecificVolume1.ToString(), endSpecificVolume1.ToString());
                //table.Rows.Add("Mass Flow rate(dry air)", "Kg(dry air)/s", Math.Round(massFlowRate1, 2).ToString(), Math.Round(massFlowRate2, 2).ToString());
                //table.Rows.Add("Enthalpy", "KJ/Kg ", startEnthalpy1.ToString(), endEnthalpy1.ToString());
                //table.Rows.Add("Total Energy Flow", "KJ/s", Math.Round(totalEnthalpyFlow1, 2).ToString(), Math.Round(totalEnthalpyFlow2, 2).ToString());
                //table.Rows.Add("Heat Change", "KW ", Math.Round(heatChange, 2).ToString(), "");
               // MessageBox.Show("Table \n" + table.Columns[2].Rows[3].ToString());
                //tooltipString = table.ToString();
                 //================================datatable close==============================================//
                if (chart1.InvokeRequired) {

                    chart1.Invoke(new Action(() => newLineSeries.ToolTip = tooltipString));
                    //newSeries.MarkerStyle = MarkerStyle.Circle;
                    //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);

                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal))));   //for prevnodeid
                    double mid_point_XValue = (temporaryNodeValueStoreForRedrawLine[0].xVal + temporaryNodeValueStoreForRedrawLine[1].xVal)/ 2;
                    double mid_point_YValue = (temporaryNodeValueStoreForRedrawLine[0].yVal + temporaryNodeValueStoreForRedrawLine[1].yVal) / 2;
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue))));   //Middle point for plotting the Label
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[1].xVal, temporaryNodeValueStoreForRedrawLine[1].yVal))));   //for nextnodeid

                    if(status == 1)
                    {
                        //Then show the label in the chart
                        chart1.Invoke(new Action(() => newLineSeries.Points[1].Label=name));
                    }

                    chart1.Invoke(new Action(() => chart1.Series.Add(newLineSeries)));

                }
                else
                {
                    newLineSeries.ToolTip = tooltipString;
                    newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal));   //for prevnodeid
                    double mid_point_XValue = (temporaryNodeValueStoreForRedrawLine[0].xVal + temporaryNodeValueStoreForRedrawLine[1].xVal) / 2;
                    double mid_point_YValue = (temporaryNodeValueStoreForRedrawLine[0].yVal + temporaryNodeValueStoreForRedrawLine[1].yVal) / 2;
                    newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue));   //Middle point for plotting the Label

                    newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[1].xVal, temporaryNodeValueStoreForRedrawLine[1].yVal));   //for nextnodeid
                    if (status == 1)
                    {
                        //Then show the label in the chart
                        newLineSeries.Points[1].Label = name;
                    }

                    chart1.Series.Add(newLineSeries);
                }

                temporaryNodeValueStoreForRedrawLine.Clear();//--Lets reset the value CODE:BBK305A

                //   }  //close of if incrementindex


            }//close of temporary node value

           // }//--Close of LOCK
        }

        public double CalculateSpecificVolume(double xVal,double yVal)
        {

            return 0.00;
        }


        public void ReDrawPointsForXML(Series s1, double x, double y, Color c, string source1, string name1, string label1x, string labelValueText)
        {

            //s1.ChartType = SeriesChartType.Point;
            string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x + "\nindex=" + incrementIndex;
            s1.ChartType = SeriesChartType.Point;
            s1.MarkerSize = 20;//marker_size_value;//= 20;
            s1.MarkerStyle = MarkerStyle.Circle;

            s1.Points.AddXY(x, y);

            chart1.Series["My Series"].Points[incrementIndex].ToolTip = s;
            chart1.Series["My Series"].Points[incrementIndex].Label = labelValueText;
            // s1.Points[indexForSeriesNodePoint].Color = c;
            chart1.Series["My Series"].Points[incrementIndex].Color = c;
            s1.Points[incrementIndex].MarkerStyle = MarkerStyle.Circle;
            s1.Points[incrementIndex].Color = c;
            // chart1.Series["My Series"].Points[indexForSeriesNodePoint].MarkerColor = c;
        }


        //--This line if for xml
        List<TempDataType> temporaryNodeValueStore = new List<TempDataType>();
        //--Redraw line function
        public void ReDrawLinesForXML(string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color c)
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
            // if (thickness_value <= 0)
            // {
            newLineSeries.BorderWidth = 3;
            // }
            //else
            //{
            //    newLineSeries.BorderWidth = thickness_value;//This is the thickness of lines
            //}
            //newSeries.ToolTip = 
            newLineSeries.Color = c;
            /*
            We need to calculate the previous node id values and the next node id values.
            */
            //First for previous node id
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (prevNodeID == menuStripNodeInfoValues[i].id.ToString())
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
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = menuStripNodeInfoValues[i].temperature_source,
                        humidity_source = menuStripNodeInfoValues[i].humidity_source,
                        name = menuStripNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size,
                        airFlow = menuStripNodeInfoValues[i].airFlow

                    });

                    break;//Break form loop
                }
            }

            //Second for the next node id
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (nextNodeID == menuStripNodeInfoValues[i].id.ToString())
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
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = menuStripNodeInfoValues[i].temperature_source,
                        humidity_source = menuStripNodeInfoValues[i].humidity_source,
                        name = menuStripNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = menuStripNodeInfoValues[i].colorValue,
                        marker_Size = menuStripNodeInfoValues[i].marker_Size,
                        airFlow = menuStripNodeInfoValues[i].airFlow
                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStore.Count > 0)
            {

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


        public double SpecificVolumeReturn = 0;

         /// <summary>
         /// Calculates humidity , enthalpy and specific volume form temperature and humidity ratio
         /// </summary>
         /// <param name="xVal">Temperature(deg cel)</param>
         /// <param name="yVal">Humidity Ratio(unitless)</param>

        public void CalculateHumidityEnthalpy(double xVal, double yVal)
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

            // double patm = pressureConverted;//This should be the value
            double patm = AirPressureFromDB / 1000; //--This is interms of KPa so divided by 1000 changes to pascal
            //101.325;//this is constant...
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

            double Patm = patm * 10;//1013;
            double A = 6.116441;
            double m = 7.591386;
            double Tn = 240.7263;
            double B = 621.9907;

            double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

            double X = B * Pws / (Patm - Pws);

            double h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
            //now lets display this value ..
            enthalpyCalculated = h;//--this is the enthalpy calculated value 

            double V = (287.1 * (xVal + 273.15) * (1 + (1.6078 * humidity))) / (patm * 100);
            SpecificVolumeReturn = Math.Round(V, 2);

        }
        //--this is used by set data button
        int countNumberOfPoints = 0;
        int xCoord = 0;

        int FlagForDisconnectingLineChoice = 0;// 0 means off and 1 means on


        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {
            

            if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            {

                //--Check of enable or dissable the cell click
                //if (FlagForDissableLeftAndRightClicksInChart == 1)
                //{
                //    //FlagForDissableLeftAndRightClicksInChart = 0;
                //    return;//DO not proceed forward
                //}


                //we need to show context menu strip

                //MessageBox.Show("Right pressed");    
                //--this is calculated based on this location the graphics will be plotted..
                xCoord = e.Location.X;
                yCoord = e.Location.Y;

                //contextMenuStrip1.Show(MousePosition);//--This is dissabled

               

                //==Now lets reset the values first,displaying the previous and next node ====//

                if(FlagForDisconnectingLineChoice == 1)
                {
                    //--We need to show the text and weather he wants to delete form first or seconds
                    //First lets enable and change the Text
                    disconnectLineFromAToolStripMenuItem.Enabled = true;
                    //disconnectLineFromAToolStripMenuItem.Visible = true;
                    //disconnectLineFromBToolStripMenuItem.Enabled = true;

                    //if(addMixNodeToolStripMenuItem.Enabled == true)
                    //{ 
                    //addMixNodeToolStripMenuItem.Visible = true;
                    //}

                    //--Now lets make the text 
                    //steps :
                    /*
                       1.Detect previous and next node id 
                       2.Detect corresponding name value for the corresponding  id
                       3.Display the node Name  
                    */
                    string prevNodeIDTemp = "";
                    string nextNodeIDTemp = "";

                    foreach(var node in menuStripNodeInfoValues)
                    {
                        if(node.id == indexOfNextNodeForLineMovement)
                        {
                            //--Next noe is identified 
                            nextNodeIDTemp = node.name;//--Store the name of the thing
                        }
                        else if(node.id == indexOfPrevPointForLineMovement)
                        {
                            //--Previous node is identified 
                            prevNodeIDTemp = node.name;//--Stroing the name of the node
                        }

                    }

                    //--we need lineID
                  //   disconnectLineFromAToolStripMenuItem.Text = "Disconnect line from " + prevNodeIDTemp;
                   // disconnectLineFromBToolStripMenuItem.Text = "Disconnect line from " + nextNodeIDTemp;

                    nodeAToolStripMenuItem.Text = "Node Name: " + prevNodeIDTemp;
                    nodeBToolStripMenuItem.Text = "Node Name: " + nextNodeIDTemp;



                }
                else
                {
                    //--We need to turn off the click option
                    disconnectLineFromAToolStripMenuItem.Enabled = false;
                   // disconnectLineFromAToolStripMenuItem.Visible = false;
                    //disconnectLineFromBToolStripMenuItem.Enabled = false;

                    //--lets dissable the line
                    nodeAToolStripMenuItem.Enabled = false;
                    nodeBToolStripMenuItem.Enabled = false;
                    addMixNodeToolStripMenuItem.Enabled = false;//For dissabling the add mix node
                    //--Lets make it invisible
                     //addMixNodeToolStripMenuItem.Visible = false;
                    
                    //--Lets dissbale
                    //nodeAToolStripMenuItem.Enabled = false;
                    //nodeBToolStripMenuItem.Enabled = false;

                }

                //==this one is for delete node to hide
                if (deleteNodeToolStripMenuItem.Enabled == true)
                {
                    deleteNodeToolStripMenuItem.Visible = true;
                }
                else
                {
                    deleteNodeToolStripMenuItem.Visible = false;
                }

                //--------This one is for mix node to show or hide
                if (addMixNodeToolStripMenuItem.Enabled == false)
                {
                    addMixNodeToolStripMenuItem.Visible = false;
                }
                else
                {
                    addMixNodeToolStripMenuItem.Visible = true;
                }

                //--This one is for disconnecting line 
                if (nodeAToolStripMenuItem.Enabled == false)
                {
                    //disconnecting line invisible
                    disconnectLineFromAToolStripMenuItem.Visible = false;
                }
                else
                {

                    //visible
                    disconnectLineFromAToolStripMenuItem.Visible = true;
                }

                //--Now lets do for the comfort zone parts...

                //--now lets check some things
                InfoForComfortZoneOfChart(chartDetailList[indexForWhichChartIsSelected].chartID);
                //MessageBox.Show("here = " + default_comfort_zone_of_chart[0].status + " count = "+ default_comfort_zone_of_chart.Count);
                if(default_comfort_zone_of_chart.Count > 0)
                { 
                if (default_comfort_zone_of_chart[0].status == "enable")
                    {
                        //now lets calculate the humidity because we can only make comparision with the humidity values....
                        HumTempCalcByCoordinate();//This will return humidityValue AND TemperatureValue by mouse coordinate
                                                  //MessageBox.Show("check min temp = " + listchartComfortZoneInfoSingle[0].min_temp + "\n max temp = " + listchartComfortZoneInfoSingle[0].max_temp + "\n min hum= " + listchartComfortZoneInfoSingle[0].min_hum + "\n max hum=" + listchartComfortZoneInfoSingle[0].max_hum);

                        if (listchartComfortZoneInfoSingle.Count > 0) { 
                        if ((temperatureValue > double.Parse(listchartComfortZoneInfoSingle[0].min_temp) && temperatureValue < double.Parse(listchartComfortZoneInfoSingle[0].max_temp)) && (humidityValue > double.Parse(listchartComfortZoneInfoSingle[0].min_hum) && humidityValue < double.Parse(listchartComfortZoneInfoSingle[0].max_hum)))
                        {
                            if (showComfortZoneToolStripMenuItem.Enabled == true)
                            {
                                showComfortZoneToolStripMenuItem.Visible = true;
                            }
                            else
                            {
                                showComfortZoneToolStripMenuItem.Visible = false;
                            }
                        }
                        else
                        {
                            showComfortZoneToolStripMenuItem.Visible = false;
                        }
                            //--Chart is enable futher processing is needed
                        }//close of if listchartComfortZoneInfoSingle
                    }
                else
                {
                    //--Hide the comfort zone
                    showComfortZoneToolStripMenuItem.Visible = false;
                }
                }
                else
                {
                    showComfortZoneToolStripMenuItem.Visible = false;
                }



                //===End of the reset the values=========// 
                this.quickNodeInsertToolStripMenuItem.Image = null;
                if (FlagForDissableLeftAndRightClicksInChart == 1)
                {
                    //FlagForDissableLeftAndRightClicksInChart = 0;
                    CMSinsertNode.Enabled = false;
                    //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                    //string NewDirectory = Path.GetFullPath(Path.Combine(dir, @"..\"));
                    //string file = NewDirectory + @"Database\image\lock.png";

                    string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = imagePath + @"\image\lock.png";


                    this.quickNodeInsertToolStripMenuItem.Image = Bitmap.FromFile(file);
                   this.quickNodeInsertToolStripMenuItem.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
                   // this.quickNodeInsertToolStripMenuItem.TextAlign = System.Drawing.ContentAlignment.TopCenter;
                    //quickNodeInsertToolStripMenuItem.ImageAlign = 
                    this.quickNodeInsertToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.ImageAndText;
                    
                    CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer

                    //return;//DO not proceed forward
                }
                else {
                    //==This shows the contextmenustrip on right click
                    CMSinsertNode.Enabled =true;
                    CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                }

            }

        } //close of mouse down
        public int yCoord = 0;
        public double humidityValue; //--This is universal used to calculate humidityValue
        public double temperatureValue; //--This is universal used to calculate temperatureValue
                                 //--These are the property of node...
        public string tbSource;
        public string tbName;
        public string tbLabel;
        public Color colorValue;
        public string comboboxItemText;
        public int markerSize = 20;
        public string deviceInstanceValue = "";
        public string deviceIP = "";
        public string deviceParam1ID = "";
        public string deviceParam2ID = "";
        public string device_param1_info_for_node = "";
        public string device_param2_info_for_node = "";
        public string object_param1_identifier_type = ""; //for identifying different analog inputs.
        public string object_param2_identifier_type = "";

        //These parameters is for lines ok 
        public string lineName = "";
        public int lineStatus_ON_OFF = 0;

        //public void SetNode(string source, string name, string label, Color c1, string comboboxItemText1, int markerSize1,string lineNameVal,int lineStatusVal)
        //{

        //    tbSource = source;
        //    tbName = name;
        //    tbLabel = label;
        //    colorValue = c1;
        //    comboboxItemText = comboboxItemText1;
        //    markerSize = markerSize1;
        //    lineName = lineNameVal;
        //    lineStatus_ON_OFF = lineStatusVal;
        //    //lets do the processing 
        //    //lets count how many items were inserted
        //    countNumberOfPoints += 1;
        //    //lets get the coordinates to plot on the graph..
        //    //this will be set on right click..not here


        //    //lets process the data 
        //    /*
        //    calculating the humidity and temperature value form the coordinates..
        //    */
        //    HumTempCalcByCoordinate();
        //    //MessageBox.Show("Temp= " + temperatureValue + ",hum = " + humidityValue);

        //    //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
        //    if ((humidityValue > 0 && humidityValue <= 100) && (temperatureValue >= 0 && temperatureValue <= 50))
        //    {
        //        //now lets plot the values....
        //        plot_by_DBT_HR_process_diagram((double)(int)temperatureValue, (double)humidityValue / 100);

        //    }
        //    else
        //    {
        //        MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
        //    }


        //}

        public string temperature_sourceGlobal = "";
        public string humidity_sourceGlobal = "";
      //  public string airFlowGlobal = "";

        public void SetNode(string temperature_sourceVal,string humidity_sourceVal, string name,  Color c1,int markerSize1, string lineNameVal, int lineStatusVal,int airflow)
        {

            //--Here we are updating the variables for storing 
           // tbSource = source;
            tbName = name;
           // tbLabel = label;
            colorValue = c1;
           // comboboxItemText = comboboxItemText1;
            markerSize = markerSize1;
            lineName = lineNameVal;
            lineStatus_ON_OFF = lineStatusVal;
            //lets do the processing 
            //lets count how many items were inserted
            countNumberOfPoints += 1;
            //--New added items
            temperature_sourceGlobal = temperature_sourceVal;
            humidity_sourceGlobal = humidity_sourceVal;
            airFlowValueGlobal = airflow;
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




        public void SetNodeWithValues(string source, string name, string label, Color c1, string comboboxItemText1, double temperature, double humidity)
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
            // HumTempCalcByCoordinate();
            //MessageBox.Show("Temp= " + temperatureValue + ",hum = " + humidityValue);

            //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if ((humidity > 0 && humidity <= 100) && (temperature >= 0 && temperature <= 50))
            {
                //now lets plot the values....

                plot_by_DBT_HR_process_diagram((double)(int)temperature, (double)humidity / 100);

            }
            else
            {
                MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
            }


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

                    double patm = AirPressureFromDB * 0.001; // this is in terms of kpa //101.325;//this is constant...
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
            double patm = AirPressureFromDB * 0.001;//in terms of kpa //101.235;//constant..we will make it take as input later...
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


            //changed here
            plot_on_graph_values_process_diagram(x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            // index++;

            return 0;
        }

        int flagForInsertOrUpdateDataToDB = 0;
        /// <summary>
        /// Amount of air flow
        /// default:1000
        /// </summary>
        int airFlowValueGlobal = 1000; //This will be updated 
        public void plot_on_graph_values_process_diagram(double xval, double yval)
        {
            //chart1.Series.Clear();


            try
            {


                series1.ChartType = SeriesChartType.Point;
                //int r, g, b;

                series1.MarkerSize = 20;
                series1.MarkerStyle = MarkerStyle.Circle;
                series1.Points.AddXY(xval, yval);
                string s = "source :\nTemperature Source " + temperature_sourceGlobal + "\nHumidity Source" + humidity_sourceGlobal + "\n Name : " + tbName;
                series1.Points[index].Color = colorValue;
                series1.Points[index].ToolTip = s;

                string labelStringValue = null;
                //labeling part
                //if (comboboxItemText == "Label")
                //{
                //    //label is selected
                //    labelStringValue = tbLabel;
                //}
                //else if (comboboxItemText == "Name")
                //{
                    //Name is selected
                    labelStringValue = tbName;
                //}
                //else
                //{
                //    //Source is selected
                //    labelStringValue = tbSource;
                //}

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


            //we need to get this a unique id 
            string unique_id_for_node = selectedBuildingList[0].BuildingName + GetGUID();//This is the unique id 


            //the value is added...
            menuStripNodeInfoValues.Add(new TempDataType
            {
                id = unique_id_for_node,
                xVal = xval,
                yVal = yval,
               // source = tbSource,
               temperature_source = temperature_sourceGlobal,
               humidity_source = humidity_sourceGlobal,
                name = tbName,
               // label = tbLabel,
                colorValue = colorValue,
               // showItemText = comboboxItemText,
                marker_Size = markerSize  ,
                airFlow = airFlowValueGlobal


            });

            //--Inserting the node values in the database sqlite


            if (flagForInsertOrUpdateDataToDB == 1)
            {
                //do only if the  flag is raised.

                // InsertNodeInfoToDB(unique_id_for_node, xval, yval, tbSource, tbName, tbLabel, colorValue, comboboxItemText, markerSize, deviceInstanceValue, deviceIP, deviceParam1ID, deviceParam2ID, device_param1_info_for_node, device_param2_info_for_node, object_param1_identifier_type, object_param2_identifier_type);
                //--New code is added to here
                DatabaseOperations ObjDbOperation = new DatabaseOperations();
                //  MessageBox.Show("Building Name= " + selectedBuildingList[0].BuildingName);
               // MessageBox.Show(unique_id_for_node+"xval = "+ xval+" yval"+ yval+"temp "+ temperature_sourceGlobal+"hum="+ humidity_sourceGlobal+",name="+ tbName+",col="+ colorValue+",size=" +markerSize +",air="+airFlowValueGlobal.ToString());
                ObjDbOperation.InsertNodeInfoToDBWithoutDeviceInfo(CurrentSelectedBuilding, chartDetailList[indexForWhichChartIsSelected].chart_respective_nodeID, unique_id_for_node, xval, yval, temperature_sourceGlobal, humidity_sourceGlobal, tbName, colorValue, markerSize, airFlowValueGlobal.ToString());
               // MessageBox.Show("Operation Complete Test");
            //InsertNodeInfoToDBWithoutDeviceInfo(unique_id_for_node,)
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
                    // prevNodeId = menuStripNodeInfoValues[index - 1].id, //previousNodeIndexForLineInput
                    prevNodeId = menuStripNodeInfoValues[previousNodeIndexForLineInput].id,
                    nextNodeId = menuStripNodeInfoValues[index].id,
                    lineColorValue = menuStripNodeInfoValues[previousNodeIndexForLineInput].colorValue,
                    lineSeriesID = newLineSeries,
                    lineThickness = 3, //default thickness is 3
                    name = lineName,
                    status = lineStatus_ON_OFF

                });


                //--Adding to db
                if (flagForInsertOrUpdateDataToDB == 1)
                {
                    //--Insert the values when the flag is raised.
                    //InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[index - 1].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries, menuStripNodeLineInfoValues[index - 1].lineThickness);
                    // InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[index - 1].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[index - 1].colorValue, newLineSeries,3);
                    InsertLineInfoToDB(unique_id_for_line, menuStripNodeInfoValues[previousNodeIndexForLineInput].id, menuStripNodeInfoValues[index].id, menuStripNodeInfoValues[previousNodeIndexForLineInput].colorValue, newLineSeries, 3);
                }



                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newLineSeries.ChartType = SeriesChartType.Line;
                //newLineSeries.MarkerStyle = MarkerStyle.Circle;
                //newLineSeries.MarkerStyle = MarkerStyle.Star6;
                newLineSeries.MarkerBorderWidth.Equals(15);
                newLineSeries.MarkerSize.Equals(35);
                newLineSeries.BorderWidth.Equals(15);
                // newLineSeries.SetCustomProperty(newLineSeries.MarkerSize.ToString(),newLineSeries.MarkerSize.Equals(25).ToString());
                newLineSeries.Color = menuStripNodeInfoValues[index].colorValue;

                //--this sets the initial values of humidity and enthalpy
                //CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[index - 1].xVal, (double)menuStripNodeInfoValues[index - 1].yVal);//previousNodeIndexForLineInput


                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[previousNodeIndexForLineInput].xVal, (double)menuStripNodeInfoValues[previousNodeIndexForLineInput].yVal);//previousNodeIndexForLineInput
                startHumidity1 = Math.Round(humidityCalculated, 2);//--Fro showing only up to 2 dec. eg."34.52"
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                double startSpecificVolume1 = SpecificVolumeReturn;
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripNodeInfoValues[index].xVal, (double)menuStripNodeInfoValues[index].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                double endSpecificVolume1 = SpecificVolumeReturn;
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                // string sequenceDetected = menuStripNodeInfoValues[index - 1].name + " to " + menuStripNodeInfoValues[index].name;
                string sequenceDetected = menuStripNodeInfoValues[previousNodeIndexForLineInput].name + " to " + menuStripNodeInfoValues[index].name;


                //string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :" + Math.Round(menuStripNodeInfoValues[index - 1].xVal, 2) + "               " + Math.Round(menuStripNodeInfoValues[index].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : " + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;
                
                //=====================================THisi used========================//

                //--this sets the initial values of humidity and enthalpy
                //CalculateHumidityEnthalpy(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal);
                //startHumidity1 = Math.Round(humidityCalculated, 2);
                //startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
              
                //--This calculates the end humidity and the enthalpy values..
                //CalculateHumidityEnthalpy((double)temporaryNodeValueStoreForRedrawLine[1].xVal, (double)temporaryNodeValueStoreForRedrawLine[1].yVal);
                //endHumidity1 = Math.Round(humidityCalculated, 2);
                //endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
              

                // MessageBox.Show("Start hum" + startHumidity1 + " end enth" + endEnthalpy1);
                //MessageBox.Show("menustripinfovalues[prevNodeID].xVal=" + menuStripNodeInfoValues[prevNodeID].xVal + "menuStripNodeInfoValues[nextNodeID].yVal=" + menuStripNodeInfoValues[nextNodeID].yVal + "menuStripNodeInfoValues[nextNodeID].xVal = "+ menuStripNodeInfoValues[nextNodeID].xVal + " menuStripNodeInfoValues[nextNodeID].yVal" + menuStripNodeInfoValues[nextNodeID].yVal);

               // double enthalpyChange = endEnthalpy1 - startEnthalpy1;

               // string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

               // string tooltipString = "";
                string ZeroLine = "Process:  " + lineName + " ";
                string FirstLine = @"Parameters                      " + "Units               " + menuStripNodeInfoValues[previousNodeIndexForLineInput].name + "                  " + menuStripNodeInfoValues[index].name;
                string SecondLine = @"DBT                                   " + "\x00B0 C                   " + Math.Round(menuStripNodeInfoValues[previousNodeIndexForLineInput].xVal, 2) + "                           " + Math.Round(menuStripNodeInfoValues[index].xVal, 2);
                string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                string FourthLine = @"Humidity Ratio                " + "Kg/Kg dryair  " + Math.Round(menuStripNodeInfoValues[previousNodeIndexForLineInput].yVal, 2) + "                       " + Math.Round(menuStripNodeInfoValues[index].yVal, 2);
                string FifthLine = "Volume Flow Rate           " + "m\xB3/s                " + Math.Round(menuStripNodeInfoValues[previousNodeIndexForLineInput].airFlow, 2) + "                      " + Math.Round(menuStripNodeInfoValues[index].airFlow, 2);

                string SixthLine = "Specific Volume              " + "m\xB3/Kg             " + startSpecificVolume1 + "                    " + endSpecificVolume1;
                double massFlowRate1 = menuStripNodeInfoValues[previousNodeIndexForLineInput].airFlow / startSpecificVolume1;
                double massFlowRate2 = menuStripNodeInfoValues[index].airFlow / endSpecificVolume1;

                string SeventhLine = @"Mass flow rate(dry air)   " + "Kg(dry air)/s   " + Math.Round(massFlowRate1, 2) + "                        " + Math.Round(massFlowRate2, 2);
                string EighthLine = @"Enthalpy                           " + "KJ/Kg              " + startEnthalpy1 + "                       " + endEnthalpy1;
                double totalEnthalpyFlow1 = massFlowRate1 * startEnthalpy1;
                double totalEnthalpyFlow2 = massFlowRate2 * endEnthalpy1;
                string NinthLine = @"Total Enthalpy Flow         " + "KJ/s                " + Math.Round(totalEnthalpyFlow1, 2) + "                      " + Math.Round(totalEnthalpyFlow2, 2);
                double heatChange = totalEnthalpyFlow2 - totalEnthalpyFlow1;
                string TenthLine = @"Heat Change                    " + "KW                  " + Math.Round(heatChange, 2) + "                     ";
               string tooltipString = ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + ThirdLine + "\n" + FourthLine + "\n" + FifthLine + "\n" + SixthLine + "\n" + SeventhLine + "\n" + EighthLine + "\n" + NinthLine + "\n" + TenthLine;
                newLineSeries.ToolTip = tooltipString;

                //=============================end of this is used======================//

                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
                //newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index - 1].yVal));
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[previousNodeIndexForLineInput].xVal, menuStripNodeInfoValues[previousNodeIndexForLineInput].yVal));
                //double mid_point_XValue = (menuStripNodeInfoValues[index - 1].xVal + menuStripNodeInfoValues[index].xVal)/ 2;
                //double mid_point_YValue = (menuStripNodeInfoValues[index - 1].yVal + menuStripNodeInfoValues[index].yVal) / 2;
                double mid_point_XValue = (menuStripNodeInfoValues[previousNodeIndexForLineInput].xVal + menuStripNodeInfoValues[index].xVal) / 2;
                double mid_point_YValue = (menuStripNodeInfoValues[previousNodeIndexForLineInput].yVal + menuStripNodeInfoValues[index].yVal) / 2;

                newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue));
                newLineSeries.Points.Add(new DataPoint(menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index].yVal));

                if(lineStatus_ON_OFF == 1)
                {
                    newLineSeries.Points[1].Color = colorValue;
                    newLineSeries.Points[1].Label = lineName;
                }
                chart1.Series.Add(newLineSeries);
                chart1.Series[newLineSeries.Name].BorderWidth = 3;


            }



            index++;
            previousNodeIndexForLineInput = IndexOfPreviousNodeForLineFunction();


        }//close of buttons

        //------Heat Map-------------------///
        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
        }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();


        double min_value = 0;
        double max_value = 0;

        //lets create two arraylist to add those and store it in the arraylist
        ArrayList temperature_value = new ArrayList();
        ArrayList pg_value_from_txtfile = new ArrayList();

        Series series1_heat_map = new Series("Heat_map_series");
        int load_map_checker = 0;//checks weather to load a map or not

        private void label21_MouseHover(object sender, EventArgs e)
        {
            //label21.Tag = "Dry bulb temperature";
            toolTip1.SetToolTip(label21, "Dry Bulb Temperauter");
        }

        private void label23_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label23, "Specific Humidity");
        }

        private void label24_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label24, "Relative Humidity");
        }

        private void label25_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label25, "Dew Point Temperature");
        }

        private void label26_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label26, "Enthalpy");
        }

        private void label5_H_unit_MouseHover(object sender, EventArgs e)
        {
            // toolTip1.SetToolTip(label5_H_unit, "Kilo Joule per K.G");
        }

        private void label2_DBT_units_MouseHover(object sender, EventArgs e)
        {
            // toolTip1.SetToolTip(label2_DBT_units, "Degree Celcius");

        }

        private void label3_Sp_H_unit_MouseHover(object sender, EventArgs e)
        {
            //   toolTip1.SetToolTip(label3_Sp_H_unit, "ratio of mass of water vapur to mass of dry air(K.G(w)/K.G(dry_air))");
        }

        private void label4_RH_unit_MouseHover(object sender, EventArgs e)
        {
            // toolTip1.SetToolTip(label4_RH_unit, "Percent");
        }

        private void label6_DP_Unit_MouseHover(object sender, EventArgs e)
        {
            // toolTip1.SetToolTip(label6_DP_Unit, "Degree Celcius");
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e)
        {
            printHeatMap();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveAsImageHeatMap();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void showComfortZoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
             Show comfort zone is used to see the comfort zone in the code..
             */

        }

        public void ComfortZonePlot()
        {
            //--This file is used to show comfort zone in the chart




        }

        public class DataTypeTempBuildingValue
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
        }

        //--This function helps to disconnect the line and provides a virtual line option.



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

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);


        }


        //--This basically triggered when the datagridview is required for showing the data...

        //--Require variables ...
        public double humDataGridValue;
        public double enthalpyDataGridView;
        private void nodeGridViewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This will trigger the grid view in c# for viewing the datapoints and editing it...

            DataGridViewDisplay d = new DataGridViewDisplay(this);
            d.Show();
        }

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

           // double patm =      101.325;//this is constant...
            double patm = AirPressureFromDB * 0.001; // this is in terms of kpa //101.325;//this is constant... //101.325;//this is constant...
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

            Patm = patm * 10;// 1013; //This is hPa = 887.5 not kpa = 88.75 patm is kpa so convert to hpa 
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

        private void airHandlerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form_handler formhandler = new Form_handler(this);
            formhandler.Show();
        }

        private void trendLogToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This part is the trend log
            //GraphForTrendLog tf = new GraphForTrendLog();
            //tf.Show();

        }

        private void buildingChartConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {




        }

        private void buildingChartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This is for building chart setting pop up form
            buildingChartSetting bcf = new buildingChartSetting(this);
            bcf.ShowDialog();
        }

        private void weatherServicesToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //Weather services form
            try { 
            Form3_WeatherService f3 = new Form3_WeatherService();
            f3.ShowDialog();
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void buildingChartToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //--This is for building chart setting pop up form
            buildingChartSetting bcf = new buildingChartSetting(this);
            bcf.ShowDialog();
        }

        private void psychometricCalculatorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void airHandlerToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //Air handler form
            Form_handler formhandler = new Form_handler(this);
            formhandler.ShowDialog();

        }

        private void exportDataToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            

        }

        private void historyPlotToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void nodeViewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This will trigger the grid view in c# for viewing the datapoints and editing it...

            DataGridViewDisplay d = new DataGridViewDisplay(this);
            d.ShowDialog();
        }

        private void insertNodeWithValuesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This is plotting with temperature and humidity or some other parameters.
            Form_input_by_temp_hum_for_main f_input = new Form_input_by_temp_hum_for_main(this);
            f_input.Show();
        }

        private void saveConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {
           // try {
                //We need to get the configuration for load here 
                // SaveConfiguration();
                //  SaveConfigurationForBuildingSetting();//--This actual saving the data
                SQLiteSaveConfigurationSetting();
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
        }





        public class nodeDataTypeForSaving
        {
          //  public int count { get; set; }
            public string chart_respective_nodeID { get; set; } //This is the chartID for grouping the node for single node values.
            public string nodeID { get; set; }  //This is the individual node id for each node 
            public double xValue { get; set; }
            public double yValue { get; set; }
            // public string source { get; set; }
            public string temperature_source { get; set; }
            public string humidity_source { get; set; }
            public string name { get; set; }
           // public string label { get; set; }
            public string colorValue { get; set; }
            //public string showTextItem { get; set; }
            public int nodeSize { get; set; }
            public int airFlow { get; set; }
            public string lastUpdatedDate { get; set; }

        }





        public class chartDetailDT_X
        {
           // public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
            public string enableChartStatus { get; set; }
        }


        public class lineDataType_X
        {
          //  public int count { get; set; }
            public string chart_respective_lineID { get; set; }
            public string lineID { get; set; }
            public string prevNodeID { get; set; }
            public string nextNodeID { get; set; }
            public string lineColorValue { get; set; }
            public string lineSeriesID { get; set; }
            public string thickness { get; set; }
            public string name { get;set;}
            public string status { get; set; }
        }
        //Lets have a list for chartDetailList
        public List<chartDetailDT_X> chartInfoPulledForSaving = new List<chartDetailDT_X>();//This is used for storing the chart detail ids

        //Nowlets have the node information ==>> all node information
        public List<nodeDataTypeForSaving> nodeInfoPulledForSaving = new List<nodeDataTypeForSaving>();

        //NOw list for line info
        public List<lineDataType_X> lineInfoPulledForSaving = new List<lineDataType_X>();

        //Now  list for pulling the device info
        public List<dt_for_device_info> deviceInfoPulledForSaving = new List<dt_for_device_info>();

        //Now for storing the comfortzone setting...
        public List<datatype_for_comfortzone> comfortZoneInforForEachChartForSaving = new List<datatype_for_comfortzone>();//which chart is associated with which comfortzone

        //Now for storing the actual confort zone values
        public List<dataTypeForCF> ComfortZonesDetailForSaving = new List<dataTypeForCF>();  //This one contains the comfortzone setting

        public class dataTypeFor_mix_node_info
        {
            public string ChartID { get; set; }

            public string nodeID { get; set; }
            //public string min_temp { get; set; }
           // public string max_temp { get; set; }
            public string previousNodeID { get; set; }
            public string nextNodeID { get; set; }
            //public Color colorValue { get; set; }
        }

        public List<dataTypeFor_mix_node_info> mixNodeInfoListForSaveConfiguration = new List<dataTypeFor_mix_node_info>();  //This one contains the comfortzone setting

        /// <summary>
        /// Read data form db to save as SQLite file
        /// </summary>

        public void ReadDataForSavingConfiguration(string path,string chartTableName,string nodeTableName,string lineTableName,string tableNameDevice,string tableForComfortZoneSetting,string tableForCF_Detail,string mixnodeInfoTable)
        {

            //--Resetting values first
            chartInfoPulledForSaving.Clear();
            nodeInfoPulledForSaving.Clear();
            lineInfoPulledForSaving.Clear();
            deviceInfoPulledForSaving.Clear();
            comfortZoneInforForEachChartForSaving.Clear();
            ComfortZonesDetailForSaving.Clear();
            mixNodeInfoListForSaveConfiguration.Clear();

            ////--This reads the value of data form db lets use list for storing the data
            //string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            //string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            //string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            //string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";

            ////lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connectionString = @"Data Source=" + path + ";Version=3;";


            string sql_for_chart_detail = "SELECT * From  " + chartTableName;
            string sql_for_node = "SELECT * From  " + nodeTableName;
            string sql_for_line = "SELECT * From  " + lineTableName;
            string sql_forDevice = "SELECT * From  " + tableNameDevice;
            string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
            string sql_for_mix_node_info = "SELECT * From  " + mixnodeInfoTable;
                ;



            //  MessageBox.Show("sql1=" + sql1);         
            using (SQLiteConnection conx = new SQLiteConnection(connectionString))
            {

                conx.Close();
                conx.Open();

                SQLiteCommand cmd = new SQLiteCommand(sql_for_chart_detail, conx);
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {

                    if (reader["chartID"].ToString() != "") { 
                    //--Reading and inserting in a List
                    chartInfoPulledForSaving.Add(new chartDetailDT_X
                    {

                      //  count = int.Parse(reader["count"].ToString()),
                        chartID = reader["chartID"].ToString(),
                        chartName = reader["chartName"].ToString(),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        chart_respective_lineID = reader["chart_respective_lineID"].ToString() ,
                        enableChartStatus = reader["enableChartStatus"].ToString()

                    });
                    }
                }

                //----Reading the node information 

                SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conx);
                SQLiteDataReader reader1 = cmd1.ExecuteReader();
                while (reader1.Read())
                {

                    if (reader1["chart_respective_nodeID"].ToString() != "") { 
                    //--Reading and inserting in a List
                    nodeInfoPulledForSaving.Add(new nodeDataTypeForSaving
                    {

                        //count = int.Parse(reader1["count"].ToString()),
                        chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                        nodeID = reader1["nodeID"].ToString(),
                        xValue = double.Parse(reader1["xValue"].ToString()),
                        yValue = double.Parse(reader1["yValue"].ToString()),
                       temperature_source = reader1["temperature_source"].ToString(),
                        humidity_source = reader1["humidity_source"].ToString(),
                        name = reader1["name"].ToString(),
                       // label = reader1["label"].ToString(),
                        colorValue = reader1["colorValue"].ToString(),
                       // showTextItem = reader1["showTextItem"].ToString(),
                        nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                        airFlow = int.Parse(reader1["airFlow"].ToString()),
                       lastUpdatedDate = reader1["lastUpdatedDate"].ToString()

                    });
                }

                }

                //--Reading the line information
                SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conx);
                SQLiteDataReader reader2 = cmd2.ExecuteReader();
                while (reader2.Read())
                {


                    if(reader2["chart_respective_lineID"].ToString() != "") { 
                    //This is the reading part of the data...
                    lineInfoPulledForSaving.Add(new lineDataType_X
                    {
                       // count = int.Parse(reader2["count"].ToString()),
                        chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                        lineID = reader2["lineID"].ToString(),
                        prevNodeID = reader2["prevNodeID"].ToString(),
                        nextNodeID = reader2["nextNodeID"].ToString(),
                        lineColorValue = reader2["lineColorValue"].ToString(),
                        lineSeriesID = reader2["lineSeriesID"].ToString(),
                        thickness = reader2["thickness"].ToString(),
                        name = reader2["name"].ToString(),
                        status = reader2["status"].ToString()


                    });

                    }
                }


                //--Pulling data for device info

                SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conx);
                SQLiteDataReader reader3 = cmd3.ExecuteReader();
                while (reader3.Read())
                {

                    if (reader3["nodeID"].ToString() != "")
                    {
                        deviceInfoPulledForSaving.Add(new dt_for_device_info
                        {
                            nodeID = reader3["nodeID"].ToString(),
                            device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                            ip_for_param1 = reader3["IP_for_param1"].ToString(),

                            device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                            ip_for_param2 = reader3["IP_for_param2"].ToString(),

                            param1id = reader3["param1ID"].ToString(),
                            param2id = reader3["param2ID"].ToString(),
                            param1info = reader3["param1_info"].ToString(),
                            param2info = reader3["param2_info"].ToString(),
                            param1_id_type = reader3["param1_identifier_type"].ToString(),
                            param2_id_type = reader3["param2_identifier_type"].ToString()

                        });
                    }
                }

                //--Now reading the chart info
                SQLiteCommand cmd4 = new SQLiteCommand(sql_for_CF_Detail, conx);
                SQLiteDataReader reader4 = cmd4.ExecuteReader();
                while (reader4.Read())
                {

                 if(reader4["id"].ToString() != "") { 
                    ComfortZonesDetailForSaving.Add(new dataTypeForCF
                    {
                        id = reader4["id"].ToString(),
                        name = reader4["name"].ToString(),
                        min_temp = reader4["min_temp"].ToString(), //this is in string formate
                        max_temp = reader4["max_temp"].ToString(),
                        min_hum = reader4["min_hum"].ToString(),
                        max_hum = reader4["max_hum"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader4["colorValue"].ToString())
                    });
                    }
                }

                //--Now reading the last part associated comfortzone with each chart
                SQLiteCommand cmd5 = new SQLiteCommand(sql_for_CF_Setting, conx);
                SQLiteDataReader reader5 = cmd5.ExecuteReader();

                while (reader5.Read())
                {
                    if (reader5["chartID"].ToString() != "") { 
                    //This is the reading part of the data...
                    comfortZoneInforForEachChartForSaving.Add(new datatype_for_comfortzone
                    {
                        chartid = reader5["chartID"].ToString(),//reader["chartID"].ToString(),
                        comfortzoneid = reader5["comfort_zone_ID"].ToString(),
                        status = reader5["status"].ToString()
                    });
                    }
                }



                //==For mix node loading up 
                SQLiteCommand cmd6 = new SQLiteCommand(sql_for_mix_node_info, conx);
                SQLiteDataReader reader6 = cmd6.ExecuteReader();

                while (reader6.Read())
                {
                    if (reader6["chartID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        mixNodeInfoListForSaveConfiguration.Add(new dataTypeFor_mix_node_info
                        {
                            ChartID = reader6["chartID"].ToString(),//reader["chartID"].ToString(),
                            nodeID = reader6["nodeID"].ToString(),
                            previousNodeID = reader6["previousNodeID"].ToString(),
                            nextNodeID = reader6["nextNodeID"].ToString(), 
                        });
                    }
                }



            } //close of using statement 




        //} //close of using statement 



    }

        //This function is for the save template function

        public void ReadDataForSavingTemplateFile(string chartId, string path, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice, string tableForComfortZoneSetting, string tableForCF_Detail, string mixnodeInfoTable)
        {

            //--Resetting values first
            chartInfoPulledForSaving.Clear();
            nodeInfoPulledForSaving.Clear();
            lineInfoPulledForSaving.Clear();
            deviceInfoPulledForSaving.Clear();
            comfortZoneInforForEachChartForSaving.Clear();
            ComfortZonesDetailForSaving.Clear();
            mixNodeInfoListForSaveConfiguration.Clear();

            ////--This reads the value of data form db lets use list for storing the data
            //string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            //string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            //string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            //string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            //string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            //string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";

            ////lets get the id values...
            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connectionString = @"Data Source=" + path + ";Version=3;";


            string sql_for_chart_detail = "SELECT * From  " + chartTableName +"  where chartID = '"+chartId+"'";
            //string sql_for_node = "SELECT * From  " + nodeTableName;
            //string sql_for_line = "SELECT * From  " + lineTableName;
            //string sql_forDevice = "SELECT * From  " + tableNameDevice;
            //string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            //string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
            //string sql_for_mix_node_info = "SELECT * From  " + mixnodeInfoTable;
            //;



            //  MessageBox.Show("sql1=" + sql1);         
            using (SQLiteConnection conx = new SQLiteConnection(connectionString))
            {

                conx.Close();
                conx.Open();

                SQLiteCommand cmd = new SQLiteCommand(sql_for_chart_detail, conx);
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {

                    if (reader["chartID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        chartInfoPulledForSaving.Add(new chartDetailDT_X
                        {

                            //  count = int.Parse(reader["count"].ToString()),
                            chartID = reader["chartID"].ToString(),
                            chartName = reader["chartName"].ToString(),
                            chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                            chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                            enableChartStatus = reader["enableChartStatus"].ToString()

                        });
                    }
                }

                string sql_for_node = "SELECT * From  " + nodeTableName + "  where chart_respective_nodeID = '" + chartInfoPulledForSaving[0].chart_respective_nodeID + "'"; 
              
                //----Reading the node information 

                SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conx);
                SQLiteDataReader reader1 = cmd1.ExecuteReader();
                while (reader1.Read())
                {

                    if (reader1["chart_respective_nodeID"].ToString() != "")
                    {
                        //--Reading and inserting in a List
                        nodeInfoPulledForSaving.Add(new nodeDataTypeForSaving
                        {

                            //count = int.Parse(reader1["count"].ToString()),
                            chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                            nodeID = reader1["nodeID"].ToString(),
                            xValue = double.Parse(reader1["xValue"].ToString()),
                            yValue = double.Parse(reader1["yValue"].ToString()),
                            temperature_source = reader1["temperature_source"].ToString(),
                            humidity_source = reader1["humidity_source"].ToString(),
                            name = reader1["name"].ToString(),
                            // label = reader1["label"].ToString(),
                            colorValue = reader1["colorValue"].ToString(),
                            // showTextItem = reader1["showTextItem"].ToString(),
                            nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                            airFlow = int.Parse(reader1["airFlow"].ToString()),
                            lastUpdatedDate = reader1["lastUpdatedDate"].ToString()

                        });
                    }

                }

                string sql_for_line = "SELECT * From  " + lineTableName + "  where chart_respective_lineID = '" + chartInfoPulledForSaving[0].chart_respective_lineID + "'"; 
              

                //--Reading the line information
                SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conx);
                SQLiteDataReader reader2 = cmd2.ExecuteReader();
                while (reader2.Read())
                {


                    if (reader2["chart_respective_lineID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        lineInfoPulledForSaving.Add(new lineDataType_X
                        {
                            // count = int.Parse(reader2["count"].ToString()),
                            chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                            lineID = reader2["lineID"].ToString(),
                            prevNodeID = reader2["prevNodeID"].ToString(),
                            nextNodeID = reader2["nextNodeID"].ToString(),
                            lineColorValue = reader2["lineColorValue"].ToString(),
                            lineSeriesID = reader2["lineSeriesID"].ToString(),
                            thickness = reader2["thickness"].ToString(),
                            name = reader2["name"].ToString(),
                            status = reader2["status"].ToString()


                        });

                    }
                }


                for(int i = 0; i < nodeInfoPulledForSaving.Count; i++) { 


                if(nodeInfoPulledForSaving[i].temperature_source == "Device" || nodeInfoPulledForSaving[i].humidity_source == "Device")
                {                   
                string sql_forDevice = "SELECT * From  " + tableNameDevice+"  WHERE nodeID = " + nodeInfoPulledForSaving[i].nodeID;
                 
                //--Pulling data for device info
                SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conx);
                SQLiteDataReader reader3 = cmd3.ExecuteReader();
                while (reader3.Read())
                {

                    if(reader3["nodeID"].ToString() != "")
                    {
                        deviceInfoPulledForSaving.Add(new dt_for_device_info
                        {
                            nodeID = reader3["nodeID"].ToString(),
                            device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                            ip_for_param1 = reader3["IP_for_param1"].ToString(),
                            device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                            ip_for_param2 = reader3["IP_for_param2"].ToString(),
                            param1id = reader3["param1ID"].ToString(),
                            param2id = reader3["param2ID"].ToString(),
                            param1info = reader3["param1_info"].ToString(),
                            param2info = reader3["param2_info"].ToString(),
                            param1_id_type = reader3["param1_identifier_type"].ToString(),
                            param2_id_type = reader3["param2_identifier_type"].ToString()

                        });
                    }
                }

                    }//--Close of if loop


                }//Close of the for loop


                string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting + "  where chartID = '"+chartId+"'";


                //--Now reading the last part associated comfortzone with each chart
                SQLiteCommand cmd5 = new SQLiteCommand(sql_for_CF_Setting, conx);
                SQLiteDataReader reader5 = cmd5.ExecuteReader();

                while (reader5.Read())
                {
                    if (reader5["chartID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        comfortZoneInforForEachChartForSaving.Add(new datatype_for_comfortzone
                        {
                            chartid = reader5["chartID"].ToString(),//reader["chartID"].ToString(),
                            comfortzoneid = reader5["comfort_zone_ID"].ToString(),
                            status = reader5["status"].ToString()
                        });
                    }
                }



                string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail +" where id = '"+ comfortZoneInforForEachChartForSaving[0].comfortzoneid+"'";
              

                //--Now reading the chart info
                SQLiteCommand cmd4 = new SQLiteCommand(sql_for_CF_Detail, conx);
                SQLiteDataReader reader4 = cmd4.ExecuteReader();
                while (reader4.Read())
                {

                    if (reader4["id"].ToString() != "")
                    {
                        ComfortZonesDetailForSaving.Add(new dataTypeForCF
                        {
                            id = reader4["id"].ToString(),
                            name = reader4["name"].ToString(),
                            min_temp = reader4["min_temp"].ToString(), //this is in string formate
                            max_temp = reader4["max_temp"].ToString(),
                            min_hum = reader4["min_hum"].ToString(),
                            max_hum = reader4["max_hum"].ToString(),
                            colorValue = ColorTranslator.FromHtml(reader4["colorValue"].ToString())
                        });
                    }
                }



                string sql_for_mix_node_info = "SELECT * From  " + mixnodeInfoTable +" where chartID  = '"+chartId+"'";



                //==For mix node loading up 
                SQLiteCommand cmd6 = new SQLiteCommand(sql_for_mix_node_info, conx);
                SQLiteDataReader reader6 = cmd6.ExecuteReader();

                while (reader6.Read())
                {
                    if (reader6["chartID"].ToString() != "")
                    {
                        //This is the reading part of the data...
                        mixNodeInfoListForSaveConfiguration.Add(new dataTypeFor_mix_node_info
                        {
                            ChartID = reader6["chartID"].ToString(),//reader["chartID"].ToString(),
                            nodeID = reader6["nodeID"].ToString(),
                            previousNodeID = reader6["previousNodeID"].ToString(),
                            nextNodeID = reader6["nextNodeID"].ToString(),
                        });
                    }
                }



            } //close of using statement 
                                                                                                                     

            //} //close of using statement 



        }









        //Lets have a list for chartDetailList
        public List<chartDetailDT_X> chartInfoPulledForSaving_For_Load = new List<chartDetailDT_X>();//This is used for storing the chart detail ids

        //Nowlets have the node information ==>> all node information
        public List<nodeDataTypeForSaving> nodeInfoPulledForSaving_For_Load = new List<nodeDataTypeForSaving>();

        //NOw list for line info
        public List<lineDataType_X> lineInfoPulledForSaving_For_Load = new List<lineDataType_X>();

        //Now  list for pulling the device info
        public List<dt_for_device_info> deviceInfoPulledForSaving_For_Load = new List<dt_for_device_info>();

        //Now for storing the comfortzone setting...
        public List<datatype_for_comfortzone> comfortZoneInforForEachChartForSaving_For_Load = new List<datatype_for_comfortzone>();//which chart is associated with which comfortzone

        //Now for storing the actual confort zone values
        public List<dataTypeForCF> ComfortZonesDetailForSaving_For_Load = new List<dataTypeForCF>();  //This one contains the comfortzone setting

        public List<dataTypeFor_mix_node_info> mixNodeList_For_Load = new List<dataTypeFor_mix_node_info>();  //This one contains the comfortzone setting


        public void ReadDataForSavingConfiguration_For_Load(string DB_String_path, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice, string tableForComfortZoneSetting, string tableForCF_Detail,string tableForMixNode)
        {

            //--Resetting values first
            chartInfoPulledForSaving_For_Load.Clear();
            nodeInfoPulledForSaving_For_Load.Clear();
            lineInfoPulledForSaving_For_Load.Clear();
            deviceInfoPulledForSaving_For_Load.Clear();
            comfortZoneInforForEachChartForSaving_For_Load.Clear();
            ComfortZonesDetailForSaving_For_Load.Clear();
            mixNodeList_For_Load.Clear();

            string connectionString = DB_String_path;// @"Data Source=" + path + ";Version=3;";
           
            string sql_for_chart_detail = "SELECT * From  " + chartTableName;
            string sql_for_node = "SELECT * From  " + nodeTableName;
            string sql_for_line = "SELECT * From  " + lineTableName;
            string sql_forDevice = "SELECT * From  " + tableNameDevice;
            string sql_for_CF_Setting = "SELECT * From  " + tableForComfortZoneSetting;
            string sql_for_CF_Detail = "SELECT * From  " + tableForCF_Detail;
            string sql_for_mix_node = "SELECT * From  " + tableForMixNode;



            // MessageBox.Show("inside sqlite path =" + path);         
            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {

                //MessageBox.Show("BHIRA");
                //conx.Close();
                conName.Open();
                using (SQLiteCommand cmdx = new SQLiteCommand(sql_for_chart_detail, conName))
                {
                    SQLiteDataReader reader = cmdx.ExecuteReader();
                    while (reader.Read())
                    {

                        if (reader["chartID"].ToString() != "")
                        {
                            //--Reading and inserting in a List
                            // if(!chartInfoPulledForSaving_For_Load.Contains())

                            // if(chartInfoPulledForSaving_For_Load)
                            foreach (var items in chartInfoPulledForSaving_For_Load)
                            {
                                if (items.chartID == reader["chartID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }

                            chartInfoPulledForSaving_For_Load.Add(new chartDetailDT_X
                            {
                                chartID = reader["chartID"].ToString(),
                                chartName = reader["chartName"].ToString(),
                                chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                                chart_respective_lineID = reader["chart_respective_lineID"].ToString() ,
                                enableChartStatus = reader["enableChartStatus"].ToString()

                            });
                        }
                    }

                    reader.Close();
                }
            }

           // MessageBox.Show("chart count value = " + chartInfoPulledForSaving_For_Load.Count);

            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //----Reading the node information 

                using (SQLiteCommand cmd1 = new SQLiteCommand(sql_for_node, conName))
                {
                    SQLiteDataReader reader1 = cmd1.ExecuteReader();
                    while (reader1.Read())
                    {

                        if (reader1["chart_respective_nodeID"].ToString() != "")
                        {

                            //if items present then do not add

                            foreach (var items in nodeInfoPulledForSaving_For_Load)
                            {
                                if (items.nodeID == reader1["nodeID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }


                            //--Reading and inserting in a List
                            nodeInfoPulledForSaving_For_Load.Add(new nodeDataTypeForSaving
                            {

                                ////--count = int.Parse(reader1["count"].ToString()),


                                chart_respective_nodeID = reader1["chart_respective_nodeID"].ToString(),
                                nodeID = reader1["nodeID"].ToString(),
                                xValue = double.Parse(reader1["xValue"].ToString()),
                                yValue = double.Parse(reader1["yValue"].ToString()),
                                //source = reader1["source"].ToString(),
                                temperature_source = reader1["temperature_source"].ToString(),
                                humidity_source = reader1["humidity_source"].ToString(),
                                name = reader1["name"].ToString(),
                                //label = reader1["label"].ToString(),
                                colorValue = reader1["colorValue"].ToString(),
                                //showTextItem = reader1["showTextItem"].ToString(),
                                nodeSize = int.Parse(reader1["nodeSize"].ToString()),
                                airFlow = int.Parse(reader1["airFlow"].ToString()),
                                lastUpdatedDate = reader1["lastUpdatedDate"].ToString()
                            });
                        }

                    }
                    reader1.Close();

                }
            }

           // MessageBox.Show("Node count value = " + nodeInfoPulledForSaving_For_Load.Count);

            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Reading the line information
                using (SQLiteCommand cmd2 = new SQLiteCommand(sql_for_line, conName))
                {
                    SQLiteDataReader reader2 = cmd2.ExecuteReader();
                    while (reader2.Read())
                    {


                        if (reader2["chart_respective_lineID"].ToString() != "")
                        {


                            foreach (var items in lineInfoPulledForSaving_For_Load)
                            {
                                if (items.lineID == reader2["lineID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }


                            //This is the reading part of the data...
                            lineInfoPulledForSaving_For_Load.Add(new lineDataType_X
                            {
                                // count = int.Parse(reader2["count"].ToString()),
                                chart_respective_lineID = reader2["chart_respective_lineID"].ToString(),
                                lineID = reader2["lineID"].ToString(),
                                prevNodeID = reader2["prevNodeID"].ToString(),
                                nextNodeID = reader2["nextNodeID"].ToString(),
                                lineColorValue = reader2["lineColorValue"].ToString(),
                                lineSeriesID = reader2["lineSeriesID"].ToString(),
                                thickness = reader2["thickness"].ToString(),
                                name = reader2["name"].ToString(),
                                status = reader2["status"].ToString()

                            });

                        }
                    }
                    reader2.Close();
                }
            }

          //  MessageBox.Show("Line count value = " + lineInfoPulledForSaving_For_Load.Count);


            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Pulling data for device info

                using (SQLiteCommand cmd3 = new SQLiteCommand(sql_forDevice, conName))
                {
                    SQLiteDataReader reader3 = cmd3.ExecuteReader();
                    while (reader3.Read())
                    {

                        if (reader3["nodeID"].ToString() != "")
                        {

                            foreach (var items in deviceInfoPulledForSaving_For_Load)
                            {
                                if (items.nodeID == reader3["nodeID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }



                            deviceInfoPulledForSaving_For_Load.Add(new dt_for_device_info
                            {
                                nodeID = reader3["nodeID"].ToString(),
                                device_instance_id_for_param1 = reader3["device_instanceID_for_param1"].ToString(),
                               ip_for_param1 = reader3["IP_for_param1"].ToString(),
                                device_instance_id_for_param2 = reader3["device_instanceID_for_param2"].ToString(),
                                ip_for_param2 = reader3["IP_for_param2"].ToString(),
                                param1id = reader3["param1ID"].ToString(),
                                param2id = reader3["param2ID"].ToString(),
                                param1info = reader3["param1_info"].ToString(),
                                param2info = reader3["param2_info"].ToString(),
                                param1_id_type = reader3["param1_identifier_type"].ToString(),
                                param2_id_type = reader3["param2_identifier_type"].ToString()

                            });
                        }
                    }

                    reader3.Close();
                }

            }
           // MessageBox.Show("device count value = " + deviceInfoPulledForSaving_For_Load.Count);


            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Now reading the chart info
                using (SQLiteCommand cmd4 = new SQLiteCommand(sql_for_CF_Detail, conName))
                {
                    SQLiteDataReader reader4 = cmd4.ExecuteReader();
                    while (reader4.Read())
                    {

                        if (reader4["id"].ToString() != "")
                        {
                            foreach (var items in ComfortZonesDetailForSaving_For_Load)
                            {
                                if (items.id == reader4["id"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }

                            ComfortZonesDetailForSaving_For_Load.Add(new dataTypeForCF
                            {
                                id = reader4["id"].ToString(),
                                name = reader4["name"].ToString(),
                                min_temp = reader4["min_temp"].ToString(), //this is in string formate
                                max_temp = reader4["max_temp"].ToString(),
                                min_hum = reader4["min_hum"].ToString(),
                                max_hum = reader4["max_hum"].ToString(),
                                colorValue = ColorTranslator.FromHtml(reader4["colorValue"].ToString())
                            });
                        }
                    }
                    reader4.Close();
                }
            }
          //  MessageBox.Show("cf detail count value = " + ComfortZonesDetailForSaving_For_Load.Count);

            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Now reading the last part associated comfortzone with each chart
                using (SQLiteCommand cmdFive = new SQLiteCommand(sql_for_CF_Setting, conName)) { 
                    SQLiteDataReader readerFive = cmdFive.ExecuteReader();

                while (readerFive.Read())
                {
                    if (readerFive["chartID"].ToString() != "")
                    {
                        foreach (var items in comfortZoneInforForEachChartForSaving_For_Load)
                        {
                            if (items.chartid == readerFive["chartID"].ToString())
                            {
                                //if contain do not add
                                return;
                            }
                        }

                        //This is the reading part of the data...
                        comfortZoneInforForEachChartForSaving_For_Load.Add(new datatype_for_comfortzone
                        {
                            
                            chartid = readerFive["chartID"].ToString(),//reader["chartID"].ToString(),
                            comfortzoneid = readerFive["comfort_zone_ID"].ToString(),
                            status = readerFive["status"].ToString()
                        });
                    }
                }
                    readerFive.Close();                
            } //close of using statement
              //  MessageBox.Show("cf single for chart count value = " + comfortZoneInforForEachChartForSaving_For_Load.Count);
            }



            //=============For MXING PART====================//




            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {
                conName.Open();
                //--Now reading the chart info
                using (SQLiteCommand cmd5 = new SQLiteCommand(sql_for_mix_node, conName))
                {
                    SQLiteDataReader reader5 = cmd5.ExecuteReader();
                    while (reader5.Read())
                    {

                        if (reader5["chartID"].ToString() != "")
                        {
                            foreach (var items in mixNodeList_For_Load)
                            {
                                if (items.ChartID == reader5["chartID"].ToString())
                                {
                                    //if contain do not add
                                    return;
                                }
                            }

                            mixNodeList_For_Load.Add(new dataTypeFor_mix_node_info
                            {                                                                             
                                ChartID = reader5["chartID"].ToString(),
                                nodeID = reader5["nodeID"].ToString(),
                                previousNodeID = reader5["previousNodeID"].ToString(),
                                nextNodeID = reader5["nextNodeID"].ToString()

                            });
                        }
                    }
                    reader5.Close();
                }
            }

          //=======================END MIXING=========================//



        }



        public bool TableExistOrNotCheckTemplateFile(string DB_String_path)
        {

            bool returnValue = false;
            string connectionString = DB_String_path;// @"Data Source=" + path + ";Version=3;";

            //string sql_for_chart_detail = "SELECT * From  " + chartTableName;
              string dbCheckSql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'signature_table'";

            // MessageBox.Show("inside sqlite path =" + path);         
            using (SQLiteConnection conName = new SQLiteConnection(connectionString))
            {

                //MessageBox.Show("BHIRA");
                //conx.Close();
                conName.Open();
                using (SQLiteCommand cmdx = new SQLiteCommand(dbCheckSql, conName))
                {
                    SQLiteDataReader reader = cmdx.ExecuteReader();
                    while (reader.Read())
                    {

                        returnValue = true;//Present 

                       // reader.Close();
                    }
                }

            }


            return returnValue;
        }


        /// <summary>
        /// This helps in insertion of the data to db
        /// </summary>
        /// <param name="fileWithPath">file name with path</param>


        public void InsertForSavingConfiguration(string fileWithPath, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice, string tableForComfortZoneSetting, string tableForCF_Detail,string tableForMixNode)
        {

            //--This is where we are going to create all the database  and tables of sqlite
            //   string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";




            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + fileWithPath + ";Version=3;")) { 
                m_dbConnection.Open();

            //Lets input chart detail id...if we are inserting we need to clear first
            string sql_input1 = null;
            foreach (var ch in chartInfoPulledForSaving)
            {
                sql_input1 = "INSERT INTO "+ chartTableName + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@id,@chartname,@cnid,@clid,@enableChartSt) ";
                SQLiteCommand cmd = new SQLiteCommand(sql_input1, m_dbConnection);
                cmd.Parameters.AddWithValue("@id", ch.chartID);
                cmd.Parameters.AddWithValue("@chartname", ch.chartName);
                cmd.Parameters.AddWithValue("@cnid", ch.chart_respective_nodeID);
                cmd.Parameters.AddWithValue("@clid", ch.chart_respective_lineID);
                cmd.Parameters.AddWithValue("@enableChartSt", ch.enableChartStatus);
                    cmd.ExecuteNonQuery();
            }


            //--Now lest input the node id
            foreach (var ch in nodeInfoPulledForSaving)
            {
                string sql_string = "insert into "+ nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,humidity_source,name,colorValue,nodeSize,airFlow,lastUpdatedDate) VALUES(@chartid,@id,@xVal,@yVal,@tempsource,@humsource,@name,@colorVal,@node_size_value,@airflow,@lastUpdatedValue)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chart_respective_nodeID);
                command.Parameters.AddWithValue("@id", ch.nodeID);
                command.Parameters.AddWithValue("@xVal", ch.xValue.ToString());
                command.Parameters.AddWithValue("@yVal", ch.yValue.ToString());
                command.Parameters.AddWithValue("@tempsource", ch.temperature_source);
                    command.Parameters.AddWithValue("@humsource", ch.humidity_source);
                    command.Parameters.AddWithValue("@name", ch.name);
                //command.Parameters.AddWithValue("@label", ch.label);
                command.Parameters.AddWithValue("@colorVal", ch.colorValue);
                //command.Parameters.AddWithValue("@text", ch.showTextItem);
                command.Parameters.AddWithValue("@node_size_value", ch.nodeSize);
                    command.Parameters.AddWithValue("@airflow", ch.airFlow);
                    command.Parameters.AddWithValue("@lastUpdatedValue", ch.lastUpdatedDate);
                    command.ExecuteNonQuery();
            }


            //--Now lets input the line value

            foreach (var ch in lineInfoPulledForSaving)
            {
                string sql_string = "insert into  "+ lineTableName+"(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@lnName,@lnStatus)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chart_respective_lineID);
                command.Parameters.AddWithValue("@id", ch.lineID);
                command.Parameters.AddWithValue("@pn", ch.prevNodeID);
                command.Parameters.AddWithValue("@nn", ch.nextNodeID);
                command.Parameters.AddWithValue("@lc",ch.lineColorValue);
                command.Parameters.AddWithValue("@ls", ch.lineSeriesID);
                command.Parameters.AddWithValue("@thicknessValue", ch.thickness);
                    command.Parameters.AddWithValue("@lnName", ch.name);
                    command.Parameters.AddWithValue("@lnStatus", ch.status);
                    command.ExecuteNonQuery();

               // command.ExecuteNonQuery();
            }


            //--This one is for device input  
            foreach (var ch in deviceInfoPulledForSaving)
            {
            string sql_string = "insert into  "+ tableNameDevice + " ( nodeID,device_instanceID_for_param1,IP_for_param1,device_instanceID_for_param2,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID_param1,@IP_param1,@instanceID_param2,@IP_param2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
            SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
            command.CommandType = CommandType.Text;
            command.Parameters.AddWithValue("@id", ch.nodeID);
            command.Parameters.AddWithValue("@instanceID_param1", ch.device_instance_id_for_param1);
            command.Parameters.AddWithValue("@IP_param1",ch.ip_for_param1);
            command.Parameters.AddWithValue("@instanceID_param2", ch.device_instance_id_for_param2);
            command.Parameters.AddWithValue("@IP_param2", ch.ip_for_param2);
            command.Parameters.AddWithValue("@param1", ch.param1id);
            command.Parameters.AddWithValue("@param2", ch.param2id);
            command.Parameters.AddWithValue("@param1info", ch.param1info);
            command.Parameters.AddWithValue("@param2info", ch.param2info);
            command.Parameters.AddWithValue("@param1_iden_type", ch.param1_id_type);
            command.Parameters.AddWithValue("@param2_iden_type", ch.param2_id_type);
             command.ExecuteNonQuery();            
             }

     

            //---Now for the comfort zone setting i guess... :)
            foreach (var ch in comfortZoneInforForEachChartForSaving)
            {
                string sql_string = "insert into "+ tableForComfortZoneSetting + "(chartID,comfort_zone_ID,status) VALUES(@chartid,@comfortzoneid,@status)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chartid);
                command.Parameters.AddWithValue("@comfortzoneid", ch.comfortzoneid);
                command.Parameters.AddWithValue("@status", ch.status);
                command.ExecuteNonQuery(); 
            }


            //--Information about detail comfortzone

            foreach (var ch in ComfortZonesDetailForSaving)
            {
             
                string sql_string = "insert into "+ tableForCF_Detail + "(id,name,min_temp,max_temp,min_hum,max_hum,colorValue) VALUES(@id,@name,@min_t,@max_t,@min_h,@max_h,@color)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;

                command.Parameters.AddWithValue("@id", ch.id);
                command.Parameters.AddWithValue("@name", ch.name);
                command.Parameters.AddWithValue("@min_t",ch.min_temp.ToString());
                command.Parameters.AddWithValue("@max_t", ch.max_temp.ToString());
                command.Parameters.AddWithValue("@min_h",ch.min_hum.ToString());
                command.Parameters.AddWithValue("@max_h", ch.max_hum.ToString());
                command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(ch.colorValue));
                command.ExecuteNonQuery();



            }

                //================MixNodeInfo=====================//
                foreach (var ch in mixNodeInfoListForSaveConfiguration)
                {

                    string sql_string1 = "insert into " + tableForMixNode + "(chartID,nodeID,previousNodeID,nextNodeID) VALUES(@chartid,@nodeid,@prevnodeid,@nextnodeid)";
                    SQLiteCommand command = new SQLiteCommand(sql_string1, m_dbConnection);
                    command.CommandType = CommandType.Text;

                    command.Parameters.AddWithValue("@chartid", ch.ChartID);
                    command.Parameters.AddWithValue("@nodeid", ch.nodeID);
                    command.Parameters.AddWithValue("@prevnodeid", ch.previousNodeID.ToString());
                    command.Parameters.AddWithValue("@nextnodeid", ch.nextNodeID.ToString());
                    //command.Parameters.AddWithValue("@min_h", ch.min_hum.ToString());
                    //command.Parameters.AddWithValue("@max_h", ch.max_hum.ToString());
                    //command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(ch.colorValue));
                    command.ExecuteNonQuery();



                }


            } //Close of using




        }


        /// <summary>
        /// This helps in insertion of the data to db
        /// </summary>
        /// <param name="fileWithPath">file name with path</param>


        public void InsertForSavingConfiguration_For_Load(string fileWithPath, string chartTableName, string nodeTableName, string lineTableName, string tableNameDevice, string tableForComfortZoneSetting, string tableForCF_Detail,string tableMixNode)
        {

            //--This is where we are going to create all the database  and tables of sqlite
            //   string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";




            //--now lets create the tables
            using (SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + fileWithPath + ";Version=3;")) { 
                m_dbConnection.Open();

            //Lets input chart detail id...if we are inserting we need to clear first
            string sql_input1 = null;
            foreach (var ch in chartInfoPulledForSaving_For_Load)
            {
                sql_input1 = "INSERT INTO " + chartTableName + " (chartID,chartName,chart_respective_nodeID,chart_respective_lineID,enableChartStatus) VALUES(@id,@chartname,@cnid,@clid,@enablestatus) ";
                SQLiteCommand cmd = new SQLiteCommand(sql_input1, m_dbConnection);
                cmd.Parameters.AddWithValue("@id", ch.chartID);
                cmd.Parameters.AddWithValue("@chartname", ch.chartName);
                cmd.Parameters.AddWithValue("@cnid", ch.chart_respective_nodeID);
                cmd.Parameters.AddWithValue("@clid", ch.chart_respective_lineID);
                 cmd.Parameters.AddWithValue("@enablestatus", ch.enableChartStatus);
                cmd.ExecuteNonQuery();
            }


            //--Now lest input the node id
            foreach (var ch in nodeInfoPulledForSaving_For_Load)
            {
                string sql_string = "insert into " + nodeTableName + " (chart_respective_nodeID,nodeID,xValue,yValue,temperature_source,humidity_source,name,colorValue,nodeSize,airFlow,lastUpdatedDate) VALUES(@chartid,@id,@xVal,@yVal,@temperature_source,@humidity_source,@name,@colorVal,@node_size_value,@airflow,@lastupdateddate)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chart_respective_nodeID);
                command.Parameters.AddWithValue("@id", ch.nodeID);
                command.Parameters.AddWithValue("@xVal", ch.xValue.ToString());
                command.Parameters.AddWithValue("@yVal", ch.yValue.ToString());
                //command.Parameters.AddWithValue("@source", ch.source);
                command.Parameters.AddWithValue("@temperature_source", ch.temperature_source);
                command.Parameters.AddWithValue("@humidity_source", ch.humidity_source);
                command.Parameters.AddWithValue("@name", ch.name);
                   //command.Parameters.AddWithValue("@label", ch.label);
                command.Parameters.AddWithValue("@colorVal", ch.colorValue);
                     //command.Parameters.AddWithValue("@text", ch.showTextItem);
                command.Parameters.AddWithValue("@node_size_value", ch.nodeSize);
                command.Parameters.AddWithValue("@airflow", ch.airFlow);
                command.Parameters.AddWithValue("@lastupdateddate", ch.lastUpdatedDate);
                command.ExecuteNonQuery();
            }


            //--Now lets input the line value

            foreach (var ch in lineInfoPulledForSaving_For_Load)
            {
                string sql_string = "insert into  " + lineTableName + "(chart_respective_lineID,lineID,prevNodeId,nextNodeId,lineColorValue,lineSeriesId,thickness,name,status) VALUES(@chartid,@id,@pn,@nn,@lc,@ls,@thicknessValue,@lnName,@lnStatus)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chart_respective_lineID);
                command.Parameters.AddWithValue("@id", ch.lineID);
                command.Parameters.AddWithValue("@pn", ch.prevNodeID);
                command.Parameters.AddWithValue("@nn", ch.nextNodeID);
                command.Parameters.AddWithValue("@lc", ch.lineColorValue);
                command.Parameters.AddWithValue("@ls", ch.lineSeriesID);
                command.Parameters.AddWithValue("@thicknessValue", ch.thickness);
                command.Parameters.AddWithValue("@lnName", ch.name);
                command.Parameters.AddWithValue("@lnStatus", ch.status);
                command.ExecuteNonQuery();

                // command.ExecuteNonQuery();
            }


            //--This one is for device input  
            foreach (var ch in deviceInfoPulledForSaving_For_Load)
            {
                string sql_string = "insert into  " + tableNameDevice + "(nodeID,device_instanceID_for_param1,IP_for_param1,device_instanceID_for_param2,IP_for_param2,param1ID,param2ID,param1_info,param2_info,param1_identifier_type,param2_identifier_type) VALUES(@id,@instanceID1,@IP1,@instanceID2,@IP2,@param1,@param2,@param1info, @param2info, @param1_iden_type, @param2_iden_type)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", ch.nodeID);
                command.Parameters.AddWithValue("@instanceID1", ch.device_instance_id_for_param1);
                command.Parameters.AddWithValue("@IP1", ch.ip_for_param1);
                command.Parameters.AddWithValue("@instanceID2", ch.device_instance_id_for_param2);
                command.Parameters.AddWithValue("@IP2", ch.ip_for_param2);
                command.Parameters.AddWithValue("@param1", ch.param1id);
                command.Parameters.AddWithValue("@param2", ch.param2id);
                command.Parameters.AddWithValue("@param1info", ch.param1info);
                command.Parameters.AddWithValue("@param2info", ch.param2info);
                command.Parameters.AddWithValue("@param1_iden_type", ch.param1_id_type);
                command.Parameters.AddWithValue("@param2_iden_type", ch.param2_id_type);
                command.ExecuteNonQuery();
            }



            //---Now for the comfort zone setting i guess... :)
            foreach (var ch in comfortZoneInforForEachChartForSaving_For_Load)
            {
                string sql_string = "insert into " + tableForComfortZoneSetting + "(chartID,comfort_zone_ID,status) VALUES(@chartid,@comfortzoneid,@status)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@chartid", ch.chartid);
                command.Parameters.AddWithValue("@comfortzoneid", ch.comfortzoneid);
                command.Parameters.AddWithValue("@status", ch.status);
                command.ExecuteNonQuery();
            }


            //--Information about detail comfortzone

            foreach (var ch in ComfortZonesDetailForSaving_For_Load)
            {

                string sql_string = "insert into " + tableForCF_Detail + "(id,name,min_temp,max_temp,min_hum,max_hum,colorValue) VALUES(@id,@name,@min_t,@max_t,@min_h,@max_h,@color)";
                SQLiteCommand command = new SQLiteCommand(sql_string, m_dbConnection);
                command.CommandType = CommandType.Text;
                command.Parameters.AddWithValue("@id", ch.id);
                command.Parameters.AddWithValue("@name", ch.name);
                command.Parameters.AddWithValue("@min_t", ch.min_temp.ToString());
                command.Parameters.AddWithValue("@max_t", ch.max_temp.ToString());
                command.Parameters.AddWithValue("@min_h", ch.min_hum.ToString());
                command.Parameters.AddWithValue("@max_h", ch.max_hum.ToString());
                command.Parameters.AddWithValue("@color", ColorTranslator.ToHtml(ch.colorValue));
                command.ExecuteNonQuery();
        
            }


            foreach (var ch in mixNodeList_For_Load)
            {
                string sql_input1x = "insert into  " + tableMixNode + " (chartID,nodeID,previousNodeID,nextNodeID) VALUES(@chartid,@nodeid,@cnid,@clid) ";
                SQLiteCommand cmdx = new SQLiteCommand(sql_input1x, m_dbConnection);
                cmdx.Parameters.AddWithValue("@chartid", ch.ChartID);
                cmdx.Parameters.AddWithValue("@nodeid", ch.nodeID);
                cmdx.Parameters.AddWithValue("@cnid", ch.previousNodeID);
                cmdx.Parameters.AddWithValue("@clid", ch.nextNodeID);
                cmdx.ExecuteNonQuery();
            }

            }//cLOSE OF USING

        }

        public void CreateTableForSavingConfiguration(string filePathWithName)
        {
            string tableForChartDetail = "tbl_chart_detail";
            string tableForNode = "tbl_node_value";
            string tableForLine = "tbl_line_value";
            string tableFordevice = "tbl_device_info_for_node";
            string tableFormixNodeInfo = "tbl_mix_node_info";


            //These two tables are for comfort zone 
            string tableforComfortZoneDetail = "tbl_comfort_zone_detail";
            string tableForChartComfortZoneSetting = "tbl_chart_comfort_zone_setting";

           // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
           // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + filePathWithName + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),temperature_source varchar(255),humidity_source varchar(255),name varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(255),lastUpdatedDate varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255),status varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),IP_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();

                //These tables are for comfort zone...
                string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                command6.ExecuteNonQuery();


                string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                command7.ExecuteNonQuery();

                string sql8 = "create table IF NOT EXISTS  " + tableFormixNodeInfo + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,nodeID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


            }

        }



        public void CreateTableForSavingTemplateFile(string filePathWithName)
        {
            string signaturetableForCheck = "signature_table";
            string tableForChartDetail = "tbl_chart_detail";
            string tableForNode = "tbl_node_value";
            string tableForLine = "tbl_line_value";
            string tableFordevice = "tbl_device_info_for_node";
            string tableFormixNodeInfo = "tbl_mix_node_info";


            //These two tables are for comfort zone 
            string tableforComfortZoneDetail = "tbl_comfort_zone_detail";
            string tableForChartComfortZoneSetting = "tbl_chart_comfort_zone_setting";

            // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + filePathWithName + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..
                string sql3x = "create table IF NOT EXISTS " + signaturetableForCheck + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartName VARCHAR(255))";
                SQLiteCommand command3x = new SQLiteCommand(sql3x, connection);
                command3x.ExecuteNonQuery();


                //--We also need to create table for particular data added..
                string sql3 = "create table IF NOT EXISTS " + tableForChartDetail + "(count INTEGER PRIMARY KEY AUTOINCREMENT ,chartID VARCHAR(255),chartName varchar(255),chart_respective_nodeID varchar(255),chart_respective_lineID varchar(255),enableChartStatus varchar(255))";
                SQLiteCommand command3 = new SQLiteCommand(sql3, connection);
                command3.ExecuteNonQuery();

                //for node info
                string sql = "create table IF NOT EXISTS " + tableForNode + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_nodeID varchar(255) ,nodeID VARCHAR(255),xValue varchar(255),yValue varchar(255),temperature_source varchar(255),humidity_source varchar(255),name varchar(255),colorValue varchar(255),nodeSize varchar(255),airFlow varchar(255),lastUpdatedDate varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.ExecuteNonQuery();

                //for line info
                string sql4 = "create table IF NOT EXISTS " + tableForLine + "(count INTEGER PRIMARY KEY AUTOINCREMENT,chart_respective_lineID varchar(255) ,lineID string,prevNodeID varchar(255),nextNodeID varchar(255),lineColorValue varchar(255),lineSeriesID varchar(255),thickness varchar(255),name varchar(255),status varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, connection);
                command4.ExecuteNonQuery();

                //for device info
                string sql5 = "create table IF NOT EXISTS  " + tableFordevice + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,nodeID varchar(255) ,device_instanceID_for_param1 varchar(255),IP_for_param1 varchar(255),device_instanceID_for_param2 varchar(255),IP_for_param2 varchar(255),param1ID varchar(255),param2ID varchar(255), param1_info  varchar(255) ,param2_info varchar(255),param1_identifier_type varchar(255),param2_identifier_type varchar(255)  )";
                SQLiteCommand command5 = new SQLiteCommand(sql5, connection);
                command5.ExecuteNonQuery();

                //These tables are for comfort zone...
                string sql6 = "create table IF NOT EXISTS  " + tableforComfortZoneDetail + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,id varchar(255) ,name varchar(255),min_temp varchar(255),max_temp varchar(255),min_hum varchar(255), max_hum  varchar(255) ,colorValue varchar(255)  )";
                SQLiteCommand command6 = new SQLiteCommand(sql6, connection);
                command6.ExecuteNonQuery();


                string sql7 = "create table IF NOT EXISTS  " + tableForChartComfortZoneSetting + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,comfort_zone_ID varchar(255),status varchar(255) )";
                SQLiteCommand command7 = new SQLiteCommand(sql7, connection);
                command7.ExecuteNonQuery();

                string sql8 = "create table IF NOT EXISTS  " + tableFormixNodeInfo + "  ( count INTEGER PRIMARY KEY AUTOINCREMENT,chartID varchar(255) ,nodeID varchar(255),previousNodeID varchar(255),nextNodeID varchar(255) )";
                SQLiteCommand command8 = new SQLiteCommand(sql8, connection);
                command8.ExecuteNonQuery();


            }

        }



        /// <summary>
        /// this fxn helps to create a sqlite file and write it value to db
        /// </summary>
        /// <param name="fileName">The name of the file user enters</param>
        /// <param name="path">path where to install the file</param>

        public void SQLiteSaveConfigurationSetting()
        {


            //--Path and table names
            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            string tableFor_mix_node_info= "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

            //lets get the id values...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile_x = databasePath + @"\db_psychrometric_project.s3db";

            //Reading the data form SQLite DB
            ReadDataForSavingConfiguration(databaseFile_x,  chartTableName, nodeTableName, lineTableName,tableNameDevice, tableForComfortZoneSetting,  tableForCF_Detail, tableFor_mix_node_info);//This reads all the data

            string fileName = "";
            saveFD.InitialDirectory = "C:";
            saveFD.FileName = "SavePsySetting";
            saveFD.Filter = "s3db(.s3db) |*.s3db";
            //ImageFormat format = ImageFormat.Png;
            if (saveFD.ShowDialog() == DialogResult.OK)
            {
              //  Cursor.Current = Cursors.WaitCursor;
                fileName = saveFD.FileName;

                // Cursor.current = Cursors.WaitCursor;
               
                //saving the file in that path
                string databaseFile = fileName;//path + fileName; //databasePath1 + @"\db_psychrometric_project.s3db";

                //--Creating file and table details 

                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                //SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                //m_dbConnection.Open();

                CreateTableForSavingConfiguration(databaseFile);

                //After the table has been created we can write to the database 
                //--InsertDataForSavingtoSQLiteDB(string pathWithFileName);

                //--Reading data will be done before this so 

                //---Writing to this db ==>insertion


                string chartTableNamex = "tbl_chart_detail";
                string nodeTableNamex = "tbl_node_value";
                string lineTableNamex = "tbl_line_value";
                string tableNameDevicex= "tbl_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSettingx = "tbl_chart_comfort_zone_setting";
                string tableForCF_Detailx = "tbl_comfort_zone_detail";
                string tableForMixNode = "tbl_mix_node_info";

               
                InsertForSavingConfiguration(databaseFile, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForComfortZoneSettingx, tableForCF_Detailx, tableForMixNode);

               // SQLiteConnection.ClearPool();//This helps in 
            }





        }


        /// <summary>
        /// This function helps to save the data of single
        /// chart file such as node ,lines and comfortzone 
        /// as a template
        /// </summary>
        /// <param name="chartID"> id of the chart selected</param>
        public void SaveChartAsTemplate(string chartID)
        {
            //--Path and table names
            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            string tableForComfortZoneSetting = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
            string tableForCF_Detail = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
            string tableFor_mix_node_info = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

            //lets get the id values...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile_x = databasePath + @"\db_psychrometric_project.s3db";

            //Reading the data form SQLite DB
            ReadDataForSavingTemplateFile(chartID,databaseFile_x, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForComfortZoneSetting, tableForCF_Detail, tableFor_mix_node_info);//This reads all the data

            string fileName = "";
            saveFD.InitialDirectory = "C:";
            saveFD.FileName = "SavePsyTemplateFile";
            saveFD.Filter = "s3db(.s3db) |*.s3db";
            //ImageFormat format = ImageFormat.Png;
            if (saveFD.ShowDialog() == DialogResult.OK)
            {
                //  Cursor.Current = Cursors.WaitCursor;
                fileName = saveFD.FileName;

                // Cursor.current = Cursors.WaitCursor;

                //saving the file in that path
                string databaseFile = fileName;//path + fileName; //databasePath1 + @"\db_psychrometric_project.s3db";

                //--Creating file and table details 

                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                //SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                //m_dbConnection.Open();

                CreateTableForSavingTemplateFile(databaseFile); //Creates a file and saves in the files

                //After the table has been created we can write to the database 
                //--InsertDataForSavingtoSQLiteDB(string pathWithFileName);

                //--Reading data will be done before this so 

                //---Writing to this db ==>insertion


                string chartTableNamex = "tbl_chart_detail";
                string nodeTableNamex = "tbl_node_value";
                string lineTableNamex = "tbl_line_value";
                string tableNameDevicex = "tbl_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSettingx = "tbl_chart_comfort_zone_setting";
                string tableForCF_Detailx = "tbl_comfort_zone_detail";
                string tableForMixNode = "tbl_mix_node_info";


                //--This will work same as for saving the entire files ok ok
                InsertForSavingConfiguration(databaseFile, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForComfortZoneSettingx, tableForCF_Detailx, tableForMixNode);

                // SQLiteConnection.ClearPool();//This helps in 
            }


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
            public string device_instance_id_for_param1 { get; set; }//--this is the values that represent the point in a chart
            public string ip_for_param1 { get; set; }
            public string device_instance_id_for_param2 { get; set; }//--this is the values that represent the point in a chart
            public string ip_for_param2 { get; set; }
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
          
            //*************************************Commented uncomment later**********************//
            
            
            ////Based on this row index we need to update the values and redraw lines..

            //// listForDataFromDB.Clear();//Lets clear the node...

            ////Lets identify the node
            //// int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString());
            ////int id = e.RowIndex;//This index is used to identify which cell or chart is clicked.
            //int id = indexValue;//This index is used to identify which cell or chart is clicked.
            ///*
            //Now lets find the chart line id and chart node id 
            //*/
            //string chartNodeGroupID = chartDetailList[id].chart_respective_nodeID;//This is for the node
            //string chartLineGroupID = chartDetailList[id].chart_respective_lineID;//This is for the line


            ////--Reset the context menu stip first..
            //lineInfoForSavingToXML.Clear();
            ////--Reset the context menu stip first..
            //nodeInfoForSavingToXML.Clear();



            //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            //string connString = @"Data Source=" + databaseFile + ";Version=3;";

            ///*
            //Table name for line and node values...
            //ie: 
            //"tbl_"+ buildingname+ "_node_value"
            //"tbl_"+ buildingname+ "_line_value"
            //*/
            //string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            //string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            //string deviceTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";


            //using (SQLiteConnection connection = new SQLiteConnection(connString))
            //{
            //    connection.Open();



            //    SQLiteDataReader reader = null;
            //    string queryString = "SELECT *  from " + nodeTableName + " WHERE chart_respective_nodeID = @chartnodeID";


            //    SQLiteCommand command = new SQLiteCommand(queryString, connection);
            //    command.Parameters.AddWithValue("@chartnodeID", chartNodeGroupID);//This is the group id that is used to identify each node



            //    int count = 0;
            //    reader = command.ExecuteReader();
            //    while (reader.Read())
            //    {
            //        nodeInfoForSavingToXML.Add(new TempDataType
            //        {
            //            id = reader["nodeID"].ToString(), //This is just changed code : bbk305
            //            xVal = double.Parse(reader["xValue"].ToString()),
            //            yVal = double.Parse(reader["yValue"].ToString()),
            //            source = reader["source"].ToString(),
            //            name = reader["name"].ToString(),
            //            label = reader["label"].ToString(),
            //            colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
            //            showItemText = reader["showTextItem"].ToString(),
            //            marker_Size = int.Parse(reader["nodeSize"].ToString())


            //        });

            //    }

            //    //--Resetting the index value...

            //    //if (nodeInfoForSavingToXML.Count > 0)
            //    //{
            //    //    count = nodeInfoForSavingToXML.Count;//--This is used for udpdating the index values..
            //    //}
            //    //else
            //    //{
            //    //    count = 0;
            //    //}
            //    ////--Resetting the actual index value
            //    //index = count; //Index is set to the count values of the node

            //    //--Adding data form the line node values...
            //    SQLiteDataReader reader2x = null;
            //    string queryString2x = "SELECT *  from  " + lineTableName + " WHERE chart_respective_lineID = @lineID";
            //    //--Testing..
            //    //   MessageBox.Show("CurrentLineTableFromDB = " + currentLineTableFromDB);

            //    SQLiteCommand command2x = new SQLiteCommand(queryString2x, connection);
            //    command2x.Parameters.AddWithValue("@lineID", chartLineGroupID);//This is the group id that is used to identify each node


            //    //int count2 = 0;
            //    reader2x = command2x.ExecuteReader();
            //    while (reader2x.Read())
            //    {
            //        lineInfoForSavingToXML.Add(new lineNodeDataType
            //        {
            //            ID = reader2x["lineID"].ToString(),//This is just change code :bbk305
            //            prevNodeId = reader2x["prevNodeId"].ToString(),
            //            nextNodeId = reader2x["nextNodeId"].ToString(),
            //            lineColorValue = ColorTranslator.FromHtml(reader2x["lineColorValue"].ToString()),
            //            lineSeriesID = new Series(reader2x["lineSeriesId"].ToString()),
            //            lineThickness = int.Parse(reader2x["thickness"].ToString())
            //        });

            //    }
            //    //count2 = menuStripNodeLineInfoValues.Count-1; //--This is used for udpdating the index values..

            //    //   MessageBox.Show("count line data in menuStripNodeLineInfoValues = " + menuStripNodeLineInfoValues.Count);

            //}//close of using..

            ////***********************************************thsi one if for pulling device info*************************************//

            ////--Reset the context menu stip first..
            //deviceInfoForSavingToXML.Clear();

            //foreach (var node in nodeInfoForSavingToXML)
            //{
            //    if (node.source == "Device")
            //    {

            //        using (SQLiteConnection connection = new SQLiteConnection(connString))
            //        {
            //            connection.Open();

            //            SQLiteDataReader reader = null;
            //            string queryString = "SELECT *  from " + deviceTableName + " WHERE nodeID = @nodeID";

            //            SQLiteCommand command = new SQLiteCommand(queryString, connection);
            //            command.Parameters.AddWithValue("@nodeID", node.id);//This is the group id that is used to identify each node



            //            //int count = 0;
            //            reader = command.ExecuteReader();
            //            while (reader.Read())
            //            {
            //                deviceInfoForSavingToXML.Add(new dt_for_device_info
            //                {
            //                    nodeID = reader["nodeID"].ToString(),
            //                    device_instance_id = reader["device_instanceID"].ToString(),
            //                    ip = reader["IP"].ToString(),
            //                    param1id = reader["param1ID"].ToString(),
            //                    param2id = reader["param2ID"].ToString(),
            //                    param1info = reader["param1_info"].ToString(),
            //                    param2info = reader["param2_info"].ToString(),
            //                    param1_id_type = reader["param1_identifier_type"].ToString(),
            //                    param2_id_type = reader["param2_identifier_type"].ToString()

            //                });
            //            }
            //        }//close of using..


            //    } //close of if


            //}//close of foreach


       //*********************************end : Uncomment this later*****************//


        }


        /// <summary>
        /// New setting for saving into xml file formate.
        /// </summary>
        public void SaveConfigurationForBuildingSetting()
        {



       //*********************************************Uncomment these things later*********************//   

           // XmlDocument xmlDoc = new XmlDocument();
           // //XmlWriter xw = new XmlWriter();
           // //lets create an xml document using a string in xml formate

           // XmlNode r_node = xmlDoc.CreateElement("RootNode");

           // xmlDoc.AppendChild(r_node);

           // XmlNode identifier = xmlDoc.CreateElement("identifier");
           // identifier.InnerText = "BuildingSetting";

           // r_node.AppendChild(identifier);


           //for(int item = 0;item<chartDetailList.Count;item++)
           // { 
           // LoadNode_LineAndDeviceInfoFromDB(item);  //PASSING THE INDEX VALUE


           //     XmlNode chartDetail = xmlDoc.CreateElement("Chart");
           //     r_node.AppendChild(chartDetail);
           //     XmlNode chartInfo = xmlDoc.CreateElement("ChartInfo");

           //     //lets create the chart id
           //     XmlNode chartID = xmlDoc.CreateElement("ChartID");
           //     chartID.InnerText = chartDetailList[item].chartID;
           //     chartInfo.AppendChild(chartID);

           //     //lets create the chart name
           //     XmlNode chartName = xmlDoc.CreateElement("ChartName");
           //     chartName.InnerText = chartDetailList[item].chartName;
           //     chartInfo.AppendChild(chartName);


           //     //lets create the chart resp nodeid
           //     XmlNode chart_resp_nodeid = xmlDoc.CreateElement("Chart_respective_nodeID");
           //     chart_resp_nodeid.InnerText = chartDetailList[item].chart_respective_nodeID;
           //     chartInfo.AppendChild(chart_resp_nodeid);


           //     //lets create the chart resp lineid
           //     XmlNode chart_resp_lineid = xmlDoc.CreateElement("Chart_respective_lineID");
           //     chart_resp_lineid.InnerText = chartDetailList[item].chart_respective_lineID;
           //     chartInfo.AppendChild(chart_resp_lineid);

           //     chartDetail.AppendChild(chartInfo);//appending the chart info


           //     XmlNode rootNode = xmlDoc.CreateElement("nodes");
           //     // xmlDoc.AppendChild(rootNode);
           //     chartDetail.AppendChild(rootNode);
           // string s = null;
           // //loading the string ...

           // if (nodeInfoForSavingToXML.Count > 0)
           // {

           //     foreach (var node in nodeInfoForSavingToXML)
           //     {
           //         if (node.source == "Device")
           //         {

           //             XmlNode userNode = xmlDoc.CreateElement("node");

           //             rootNode.AppendChild(userNode);


           //             //now append name
           //             XmlNode nodeID = xmlDoc.CreateElement("nodeID");
           //             nodeID.InnerText = node.id.ToString();
           //             userNode.AppendChild(nodeID);




           //             //now append name
           //             XmlNode userNodeName = xmlDoc.CreateElement("name");
           //             userNodeName.InnerText = node.name.ToString();
           //             userNode.AppendChild(userNodeName);


           //             //now append the label
           //             XmlNode userNodeLable = xmlDoc.CreateElement("label");
           //           //  userNodeLable.InnerText = node.label.ToString();//'"' + node.label + '"';
           //             userNode.AppendChild(userNodeLable);

           //             //now append the source
           //             XmlNode userNodeSource = xmlDoc.CreateElement("source");
           //            // userNodeSource.InnerText = node.source.ToString();  //'"' + node.source + '"';
           //             userNode.AppendChild(userNodeSource);

           //             //now append the color
           //             XmlNode userNodeColor = xmlDoc.CreateElement("color");
           //             userNodeColor.InnerText = ColorTranslator.ToHtml(node.colorValue).ToString();//'"' + ColorTranslator.ToHtml(node.colorValue).ToString() + '"';
           //             userNode.AppendChild(userNodeColor);

           //             //now append the xvalue
           //             XmlNode userNodeXValue = xmlDoc.CreateElement("xvalue");
           //             userNodeXValue.InnerText = node.xVal.ToString();//'"' + node.xVal.ToString() + '"';
           //             userNode.AppendChild(userNodeXValue);

           //             //now append the yvalue
           //             XmlNode userNodeYValue = xmlDoc.CreateElement("yvalue");
           //             userNodeYValue.InnerText = node.yVal.ToString(); //'"' + node.yVal.ToString() + '"';
           //             userNode.AppendChild(userNodeYValue);


           //             //now append the showTextItem
           //             XmlNode userNodeShowTextItem = xmlDoc.CreateElement("showTextItem");
           //            // userNodeShowTextItem.InnerText = node.showItemText.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(userNodeShowTextItem);

           //             //--node size
           //             XmlNode nodesize = xmlDoc.CreateElement("nodesize");
           //             nodesize.InnerText = node.marker_Size.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(nodesize);


           //             //--WE ALSO need to add the user information.

           //             foreach (var node_device_info in deviceInfoForSavingToXML)
           //             {
           //                 if (node.id == node_device_info.nodeID)
           //                 {
           //                     //for instance id
           //                     XmlNode deviceinstance = xmlDoc.CreateElement("deviceInstance");
           //                     deviceinstance.InnerText = node_device_info.device_instance_id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(deviceinstance);

           //                     //for ip 
           //                     XmlNode ip = xmlDoc.CreateElement("ip");
           //                     ip.InnerText = node_device_info.ip.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(ip);

           //                     //for param1id
           //                     XmlNode param1id = xmlDoc.CreateElement("param1id");
           //                     param1id.InnerText = node_device_info.param1id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param1id);

           //                     //for param2id
           //                     XmlNode param2id = xmlDoc.CreateElement("param2id");
           //                     param2id.InnerText = node_device_info.param2id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param2id);

           //                     //for param1info
           //                     XmlNode param1info = xmlDoc.CreateElement("param1info");
           //                     param1info.InnerText = node_device_info.param1info.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param1info);

           //                     //for param2info
           //                     XmlNode param2info = xmlDoc.CreateElement("param2info");
           //                     param2info.InnerText = node_device_info.param2info.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param2info);

           //                     //for param1id_type
           //                     XmlNode param1id_type = xmlDoc.CreateElement("param1id_type");
           //                     param1id_type.InnerText = node_device_info.param1_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param1id_type);

           //                     //for param2id_type
           //                     XmlNode param2id_type = xmlDoc.CreateElement("param2id_type");
           //                     param2id_type.InnerText = node_device_info.param2_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
           //                     userNode.AppendChild(param2id_type);



           //                 }


           //             }

           //         }//clsoe of 
           //         else
           //         {
           //             //the node is form the web/Manual so do not load


           //             XmlNode userNode = xmlDoc.CreateElement("node");

           //             rootNode.AppendChild(userNode);


           //             //now append nodeid
           //             XmlNode nodeID = xmlDoc.CreateElement("nodeID");
           //             nodeID.InnerText = node.id.ToString();
           //             userNode.AppendChild(nodeID);


           //             //now append name
           //             XmlNode userNodeName = xmlDoc.CreateElement("name");
           //             // XmlAttribute attribute = xmlDoc.CreateAttribute("value");
           //             //attribute.Value = '"' + node.name + '"';
           //             //userNodeName.Attributes.Append(attribute);
           //             userNodeName.InnerText = node.name.ToString();
           //             userNode.AppendChild(userNodeName);


           //             //now append the label
           //             XmlNode userNodeLable = xmlDoc.CreateElement("label");
           //            // userNodeLable.InnerText = node.label.ToString();//'"' + node.label + '"';
           //             userNode.AppendChild(userNodeLable);

           //             //now append the source
           //             XmlNode userNodeSource = xmlDoc.CreateElement("source");
           //           //  userNodeSource.InnerText = node.source.ToString();  //'"' + node.source + '"';
           //             userNode.AppendChild(userNodeSource);

           //             //now append the color
           //             XmlNode userNodeColor = xmlDoc.CreateElement("color");
           //             userNodeColor.InnerText = ColorTranslator.ToHtml(node.colorValue).ToString();//'"' + ColorTranslator.ToHtml(node.colorValue).ToString() + '"';
           //             userNode.AppendChild(userNodeColor);

           //             //now append the xvalue
           //             XmlNode userNodeXValue = xmlDoc.CreateElement("xvalue");
           //             userNodeXValue.InnerText = node.xVal.ToString();//'"' + node.xVal.ToString() + '"';
           //             userNode.AppendChild(userNodeXValue);

           //             //now append the yvalue
           //             XmlNode userNodeYValue = xmlDoc.CreateElement("yvalue");
           //             userNodeYValue.InnerText = node.yVal.ToString(); //'"' + node.yVal.ToString() + '"';
           //             userNode.AppendChild(userNodeYValue);


           //             //now append the showTextItem
           //             XmlNode userNodeShowTextItem = xmlDoc.CreateElement("showTextItem");
           //           //  userNodeShowTextItem.InnerText = node.showItemText.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(userNodeShowTextItem);

           //             //--node size
           //             XmlNode nodesize = xmlDoc.CreateElement("nodesize");
           //             nodesize.InnerText = node.marker_Size.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(nodesize);

           //             //--WE ALSO need to add the user information.

           //             //for instance id
           //             XmlNode deviceinstance = xmlDoc.CreateElement("deviceInstance");
           //             deviceinstance.InnerText = "";// node_device_info.device_instance_id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(deviceinstance);

           //             //for ip 
           //             XmlNode ip = xmlDoc.CreateElement("ip");
           //             ip.InnerText = "";// node_device_info.ip.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(ip);

           //             //for param1id
           //             XmlNode param1id = xmlDoc.CreateElement("param1id");
           //             param1id.InnerText = "";//node_device_info.param1id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param1id);

           //             //for param2id
           //             XmlNode param2id = xmlDoc.CreateElement("param2id");
           //             param2id.InnerText = "";// node_device_info.param2id.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param2id);

           //             //for param1info
           //             XmlNode param1info = xmlDoc.CreateElement("param1info");
           //             param1info.InnerText = "";//node_device_info.param1info.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param1info);

           //             //for param2info
           //             XmlNode param2info = xmlDoc.CreateElement("param2info");
           //             param2info.InnerText = "";// node_device_info.param2info.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param2info);

           //             //for param1id_type
           //             XmlNode param1id_type = xmlDoc.CreateElement("param1id_type");
           //             param1id_type.InnerText = "";//node_device_info.param1_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param1id_type);

           //             //for param2id_type
           //             XmlNode param2id_type = xmlDoc.CreateElement("param2id_type");
           //             param2id_type.InnerText = "";//node_device_info.param2_id_type.ToString(); //'"' + node.showItemText.ToString() + '"';
           //             userNode.AppendChild(param2id_type);


           //         }


           //     }//This one is for each node in the section

           // }//close of if

           // //--Now lets append the line information...
           // XmlNode lineNode = xmlDoc.CreateElement("lines");
           //     // xmlDoc.AppendChild(rootNode);
           // chartDetail.AppendChild(lineNode);//This also need to be saved in the chart detail list


           // if (lineInfoForSavingToXML.Count > 0)
           // {
           //     //--if present ie value do following.

           //     foreach (var line in lineInfoForSavingToXML)
           //     {

           //         XmlNode singleLine = xmlDoc.CreateElement("line");
           //         lineNode.AppendChild(singleLine);


           //         //for id
           //         XmlNode lineID = xmlDoc.CreateElement("ID");
           //         lineID.InnerText = line.ID;
           //         singleLine.AppendChild(lineID);

           //         //--for prev node id
           //         XmlNode prevNodeID = xmlDoc.CreateElement("prevNodeID");
           //         prevNodeID.InnerText = line.prevNodeId;
           //         singleLine.AppendChild(prevNodeID);


           //         //for next nodeid
           //         XmlNode nextNodeID = xmlDoc.CreateElement("nextNodeID");
           //         nextNodeID.InnerText = line.nextNodeId;
           //         singleLine.AppendChild(nextNodeID);

           //         //--for thickness
           //         XmlNode thickness = xmlDoc.CreateElement("linethickness");
           //         thickness.InnerText = line.lineThickness.ToString();
           //         singleLine.AppendChild(thickness);


           //         //--for series id
           //         XmlNode seriesname = xmlDoc.CreateElement("seriesname");
           //         seriesname.InnerText = line.lineSeriesID.Name.ToString();//This prints the line series name
           //         singleLine.AppendChild(seriesname);


           //         //--for color value
           //         XmlNode linecolor = xmlDoc.CreateElement("linecolor");
           //         linecolor.InnerText = ColorTranslator.ToHtml(line.lineColorValue).ToString();
           //         singleLine.AppendChild(linecolor);



           //     }//close of foreach




           // }

           // }//close of the foreach loop
           // //now saving the doucment 
           // SaveFileDialog saveFileDialog1 = new SaveFileDialog();
           // saveFileDialog1.Filter = "xml file|*.xml";  //|Bitmap Image|*.bmp|Gif Image|*.gif";
           // saveFileDialog1.Title = "Save an Image File";
           // if (saveFileDialog1.ShowDialog() == DialogResult.OK)
           // {
           //     string name = saveFileDialog1.FileName;
           //     xmlDoc.Save(name);
           // }


         ////*****************Saveconfig end point**********************************//


        }




        public void SaveConfiguration()
        {
                XmlDocument xmlDoc = new XmlDocument();
                //XmlWriter xw = new XmlWriter();
                //lets create an xml document using a string in xml formate


                XmlNode mainNode = xmlDoc.CreateElement("RootNode");
                xmlDoc.AppendChild(mainNode);



                XmlNode identifier = xmlDoc.CreateElement("identifier");
                identifier.InnerText = "MainForm";//This is for identiying main form
                mainNode.AppendChild(identifier);



            XmlNode rootNode = xmlDoc.CreateElement("nodes");
            mainNode.AppendChild(rootNode);

            //string s = null;
            //loading the string ...
            foreach(var node in menuStripNodeInfoValues)
            {
                //s += "<node><name>"+ node.name+ "</name><label>" + node.label + "</label><source>" + node.source + "</source><xvalue>" + node.xVal + "</xvalue><yvalue>" + node.yVal + "</yvalue></node>";

                XmlNode userNode = xmlDoc.CreateElement("node");        
                rootNode.AppendChild(userNode);

                //now append name
                XmlNode nodeid = xmlDoc.CreateElement("nodeID");
                nodeid.InnerText = node.id.ToString();
                userNode.AppendChild(nodeid);


                //now append name
                XmlNode userNodeName = xmlDoc.CreateElement("name");
                userNodeName.InnerText =  node.name.ToString();
                userNode.AppendChild(userNodeName);


                //now append the label
                XmlNode userNodeLable = xmlDoc.CreateElement("label");
               // userNodeLable.InnerText = node.label.ToString();//'"' + node.label + '"';
                userNode.AppendChild(userNodeLable);

                //now append the source
                XmlNode userNodeSource = xmlDoc.CreateElement("source");
               // userNodeSource.InnerText = node.source.ToString();  //'"' + node.source + '"';
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
               // userNodeShowTextItem.InnerText = node.showItemText.ToString(); //'"' + node.showItemText.ToString() + '"';
                userNode.AppendChild(userNodeShowTextItem);


            }

            XmlNode lineNode = xmlDoc.CreateElement("lines");
            // xmlDoc.AppendChild(rootNode);
            mainNode.AppendChild(lineNode);



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
                    //thickness.InnerText = line..ToString();
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
            saveFileDialog1.Filter =  "xml file|*.xml";  //|Bitmap Image|*.bmp|Gif Image|*.gif";
            saveFileDialog1.Title = "Save an Image File";
            if(saveFileDialog1.ShowDialog()== DialogResult.OK)
            {
                string name = saveFileDialog1.FileName;
                xmlDoc.Save(name);
            }
            

        }

        private void loadConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                //loadXMLDoc();
                LoadDataFromFileToDBConfiguration();


                //--We need a load function here
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }





        //--loading function required now
        public void LoadDataFromFileToDBConfiguration()
        {
            //--This helps in loading function 

            //--Now required function and tables 

            //--nothing just clearing the values
            //--Resetting values first
            //chartInfoPulledForSaving.Clear();
            //nodeInfoPulledForSaving.Clear();
            //lineInfoPulledForSaving.Clear();
            //deviceInfoPulledForSaving.Clear();
            //comfortZoneInforForEachChartForSaving.Clear();
            //ComfortZonesDetailForSaving.Clear();


            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_chart_detail";
            string nodeTableName = "tbl_node_value";
            string lineTableName = "tbl_line_value";
            string tableNameDevice = "tbl_device_info_for_node";//currentNodeTableFromDB; 
            string tableForComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            string tableForCF_Detail = "tbl_comfort_zone_detail";
            string tableForMixNode = "tbl_mix_node_info";

           
            OpenFileDialog theDialog = new OpenFileDialog();
            theDialog.Title = "Open s3db File";
            theDialog.Filter = "s3db files|*.s3db";
            theDialog.InitialDirectory = @"C:\";
            if (theDialog.ShowDialog() == DialogResult.OK)
            {
               // MessageBox.Show(theDialog.FileName.ToString());
                //here we are going to read the data and all the function 

                //lets get the id values...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = theDialog.FileName;
                string pathToFile =@"Data Source=" + databaseFile + ";Version=3;";

                // MessageBox.Show("BBK TEST Hellow,PATH = "+pathToFile);


                //Test(pathToFile);
                //return;

                //--This function reads the data
                //####### TASK1 :Read data
                // try {
               // MessageBox.Show("read data"); 
                ReadDataForSavingConfiguration_For_Load(pathToFile, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForComfortZoneSetting, tableForCF_Detail, tableForMixNode);//This reads all the data

               // MessageBox.Show("finish read data");
                //}catch(Exception ex)
                //{
                //    MessageBox.Show("Exception data : " + ex.Data + ",\nexp mess:" + ex.Message + "\nexp source" + ex.Source + "exp " + ex);
                //}
                // MessageBox.Show("Hellow END");

                //######### TASK2: Delete data
                //--Now removing the table datas form db 
                // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
                string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFileName1 = databasePath1 + @"\db_psychrometric_project.s3db";

                //--Table names
                string chartTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevice1 = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSetting1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                string tableForCF_Detail1 = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableformixnode = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                DeleteAllDataFromTable(databaseFileName1, chartTableName1);
                DeleteAllDataFromTable(databaseFileName1, nodeTableName1);
                DeleteAllDataFromTable(databaseFileName1, lineTableName1);
                DeleteAllDataFromTable(databaseFileName1, tableNameDevice1);
                DeleteAllDataFromTable(databaseFileName1, tableForComfortZoneSetting1);
                DeleteAllDataFromTable(databaseFileName1, tableForCF_Detail1);
                DeleteAllDataFromTable(databaseFileName1, tableformixnode);

                //####### TASK3 : Writing to db 
                //Now writing the data to db

                string chartTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevicex = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSettingx = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                string tableForCF_Detailx = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableformixnodex = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

               // MessageBox.Show("Loading Here mix table "+ tableformixnodex+"dbfILE NAME"+ databaseFileName1);

                InsertForSavingConfiguration_For_Load(databaseFileName1, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForComfortZoneSettingx, tableForCF_Detailx, tableformixnodex);


               // MessageBox.Show("InsertioncOMPLETE");
                //--Pulling data and plotting values--
                //DataGridView_Show_Data();
                //Now reloading the data 
               PullDataAndPlot();

            }
        }


        public void LoadDataFromTemplateFileToDBActionTemplateFileUpload(string chartId, string chartName)
        {
            //--This helps in loading function 

            //--Now required function and tables 

       

            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_chart_detail";
            string nodeTableName = "tbl_node_value";
            string lineTableName = "tbl_line_value";
            string tableNameDevice = "tbl_device_info_for_node";//currentNodeTableFromDB; 
            string tableForComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            string tableForCF_Detail = "tbl_comfort_zone_detail";
            string tableForMixNode = "tbl_mix_node_info";


            OpenFileDialog theDialog = new OpenFileDialog();
            theDialog.Title = "Open s3db File";
            theDialog.Filter = "s3db files|*.s3db";
            theDialog.InitialDirectory = @"C:\";
            if (theDialog.ShowDialog() == DialogResult.OK)
            {
                // MessageBox.Show(theDialog.FileName.ToString());
                //here we are going to read the data and all the function 

                //lets get the id values...
                //string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = theDialog.FileName;
                string pathToFile = @"Data Source=" + databaseFile + ";Version=3;";

                // MessageBox.Show("BBK TEST Hellow,PATH = "+pathToFile);


                //Test(pathToFile);
                //return;

                //--This function reads the data
                //####### TASK1 :Read data
                // try {

                // MessageBox.Show("read data"); 

                //--Checking the validity of the template file

               if(! TableExistOrNotCheckTemplateFile(pathToFile))
                {
                    MessageBox.Show("Please load a valid template file");
                    return;

                }


                ReadDataForSavingConfiguration_For_Load(pathToFile, chartTableName, nodeTableName, lineTableName, tableNameDevice, tableForComfortZoneSetting, tableForCF_Detail, tableForMixNode);//This reads all the data

                // MessageBox.Show("finish read data");
                //}catch(Exception ex)
                //{
                //    MessageBox.Show("Exception data : " + ex.Data + ",\nexp mess:" + ex.Message + "\nexp source" + ex.Source + "exp " + ex);
                //}
                // MessageBox.Show("Hellow END");

                //######### TASK2: Delete data
                //--Now removing the table datas form db 
                // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
                string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFileName1 = databasePath1 + @"\db_psychrometric_project.s3db";

                //--Table names
                string chartTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableName1 = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevice1 = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSetting1 = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                string tableForCF_Detail1 = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableformixnode = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                //DeleteAllDataFromTable(databaseFileName1, chartTableName1);
                //DeleteAllDataFromTable(databaseFileName1, nodeTableName1);
                //DeleteAllDataFromTable(databaseFileName1, lineTableName1);
                //DeleteAllDataFromTable(databaseFileName1, tableNameDevice1);
                //DeleteAllDataFromTable(databaseFileName1, tableForComfortZoneSetting1);
                //DeleteAllDataFromTable(databaseFileName1, tableForCF_Detail1);
                //DeleteAllDataFromTable(databaseFileName1, tableformixnode);

                //####### TASK3 : Writing to db 
                //Now writing the data to db

                string chartTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
                string nodeTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
                string lineTableNamex = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
                string tableNameDevicex = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
                string tableForComfortZoneSettingx = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";
                string tableForCF_Detailx = "tbl_" + selectedBuildingList[0].BuildingName + "_comfort_zone_detail";
                string tableformixnodex = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";

                // MessageBox.Show("Loading Here mix table "+ tableformixnodex+"dbfILE NAME"+ databaseFileName1);



                for(int i =0;i< chartInfoPulledForSaving_For_Load.Count; i++)
                {
                    if(chartInfoPulledForSaving_For_Load[i].chartID == chartId)
                    {
                        MessageBox.Show("Chart with same ID already present. Could not load the template file");
                        return;

                    }   else if(chartInfoPulledForSaving_For_Load[i].chartName == chartName)
                    {
                        MessageBox.Show("Chart with same name already present. Could not load the template file");
                        return;

                    }
                }


                InsertForSavingConfiguration_For_Load(databaseFileName1, chartTableNamex, nodeTableNamex, lineTableNamex, tableNameDevicex, tableForComfortZoneSettingx, tableForCF_Detailx, tableformixnodex);


                // MessageBox.Show("InsertioncOMPLETE");
                //--Pulling data and plotting values--
                //DataGridView_Show_Data();
                //Now reloading the data 
                PullDataAndPlot();

            }
        }



        //--Lets have some tables for storing the data ....



        /// <summary>
        /// This function load the data form s3db file to our project
        /// </summary>
        public void loadDataFromIndependentFileAndRefreshData()
        {
            /*Steps :
            1.Read the data from the file
            2.Store the data in a dynamic List 
            3.Delete all the data form the table OR drop table and re-create it
            4.Upload the list data to original database
            5.Reload the data of node and line 
            6.Refresh the chart             
            */
            //--This reads the value of data form db lets use list for storing the data
            string chartTableName = "tbl_chart_detail";
            string nodeTableName = "tbl_node_value";
            string lineTableName = "tbl_line_value";
            string tableNameDevice = "tbl_device_info_for_node";//currentNodeTableFromDB; 
            string tableForComfortZoneSetting = "tbl_chart_comfort_zone_setting";
            string tableForCF_Detail = "tbl_comfort_zone_detail";


            OpenFileDialog O_Dialog = new OpenFileDialog();
            O_Dialog.Title = "Open s3db File";
            O_Dialog.Filter = "s3db files|*.s3db";
            O_Dialog.InitialDirectory = @"C:\";
            string FilePathWithName = "";
            if (O_Dialog.ShowDialog() == DialogResult.OK)
            {
                //--This is where we are going to add all the code we require
                FilePathWithName = O_Dialog.FileName;//Now we have the file name 


            }

            if(FilePathWithName != "")
            {
                //--Then we are going to read/write to db

                string pathToFile = @"Data Source=" + FilePathWithName + ";Version=3;";

                //--Now lets read the data value




            }





        }





        /// <summary>
        /// THIS HELPS To retrive data from psychometric db and plot in db
        /// </summary>
        public void PullDataAndPlot()
        {
            //--Refreshing the tables for all the datas..

            dataGridView1.Rows.Clear();
            DataGridView_Show_Data();
            dataGridView1.Rows.Add();
            //If there is no rows in datagridview...
            //if(dataGridView1.Rows.Count < 1) { 

            RefreshGraph();

            //after deletion lets select the its corresponding upper chart
            //Select the chart with was selected
            if (chartDetailList.Count > 0)
            {
                //We need to select the particular index 
                //--I hope this will do the replotting thing as well
                dataGridView1.Rows[0].Selected = true;//The row is selected 

            }
        }



        //--Deleting all data form any table
        public void DeleteAllDataFromTable(string db_path,string tableName)
        {
           // string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";// "tbl_" ++"_node_value";
           // string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
           // string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + db_path + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                //SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName;
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.ExecuteNonQuery();

            }//Close of using 
        }



        //temporary data type
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
        }


        public List<Tempdt> nodeInfoFromXMLfile = new List<Tempdt>();
        public List<lineNodeDataType> lineInfoFromXMLfile = new List<lineNodeDataType>();
        public void loadXMLDoc()
        {
            nodeInfoFromXMLfile.Clear();
            lineInfoFromXMLfile.Clear();
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "xml file|*.xml";  //|Bitmap Image|*.bmp|Gif Image|*.gif";
            openFileDialog.Title = "Save an Image File";
            string path = null;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                path= openFileDialog.FileName;
                //xmlDoc.Save(name);
            }

            if(path == "")
            {
                return;
            }
            //now lets read the data from the file
            XmlDocument xmlDoc = new XmlDocument();
            try { 
            xmlDoc.Load(path);
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
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
                        MessageBox.Show("File contains are edited outside,can not load");
                        return;

                    }
                    else if (identifierSignature == "BuildingSetting")
                    {
                        MessageBox.Show("This file belongs to online mode Load in Building Setting");
                        return;
                    }
                    else if (identifierSignature == "AirHandler")
                    {
                        MessageBox.Show("This file belongs to air handler. Please load in air handler section");
                        return;
                    }

                }
            }



            XmlNodeList xnList = xmlDoc.SelectNodes("RootNode/nodes/node");
            foreach (XmlNode xn in xnList)
            {
                string idVal = xn["nodeID"].InnerText;
                string name = xn["name"].InnerText;
                string label = xn["label"].InnerText;                
                string source = xn["source"].InnerText;
                string color = xn["color"].InnerText;
                string xvalue = xn["xvalue"].InnerText;
                string yvalue = xn["yvalue"].InnerText;
                string showTextItem = xn["showTextItem"].InnerText;

                //now lets add these values to list
                nodeInfoFromXMLfile.Add(new Tempdt
                {
                    id = idVal ,
                    name = name,
                    label = label,
                    source = source,
                    colorValue = ColorTranslator.FromHtml(color),
                    xVal = double.Parse(xvalue),
                    yVal = double.Parse(yvalue),
                    showItemText = showTextItem
                });            
            }//close of foreach

            //--loading the line info from the doc
            XmlNodeList xlList = xmlDoc.SelectNodes("RootNode/lines/line");

            foreach (XmlNode xn in xlList)
            {
                string idVal = xn["ID"].InnerText;
                string prevNodeIDVal = xn["prevNodeID"].InnerText;
                string nextNodeIDVal = xn["nextNodeID"].InnerText;
               // string lineThicknessVal = xn["linethickness"].InnerText;
                string serieNameIDval = xn["seriesname"].InnerText;
                string linecolor = xn["linecolor"].InnerText;
                //now lets add these values to list
                lineInfoFromXMLfile.Add(new lineNodeDataType
                {
                    ID = idVal,
                    prevNodeId = prevNodeIDVal,
                    nextNodeId =nextNodeIDVal,                   
                    lineSeriesID = new Series(serieNameIDval),
                    lineColorValue = ColorTranslator.FromHtml(linecolor)
                });
            }//close of foreach



            //Now lets load the values  first copy in two list and the load

            menuStripNodeInfoValues.Clear();//First clear the node info
            menuStripNodeLineInfoValues.Clear();//Line info value

            //now clone the newly create list
            // menuStripNodeInfoValues = Clone(lineInfoFromXMLfile);

            //var mi = typeof(Form1_main).GetMethod("Clone");
            //var fooRef = mi.MakeGenericMethod(bar);
            //fooRef.Invoke(new Test(), null);

           //=====================loading the data ============================// 
            foreach(var node in nodeInfoFromXMLfile)
            {
                menuStripNodeInfoValues.Add(new TempDataType
                {
                    //==This need to be uncommented later=============//
                     //Some task letf 
                    id = node.id,
                   // label = node.label,
                    name = node.name,
                   // source = node.source,
                   //temperature_source =  node.
                    xVal = node.xVal,
                    yVal = node.yVal,
                    colorValue = node.colorValue,
                   // showItemText = node.showItemText

                 //==============end uncomment=====================//
                });

            }

            foreach(var line in lineInfoFromXMLfile)
            {
                menuStripNodeLineInfoValues.Add(new lineNodeDataType
                {
                       ID = line.ID,
                       nextNodeId = line.nextNodeId,
                       prevNodeId = line.prevNodeId,
                       lineColorValue = line.lineColorValue,
                       lineSeriesID = line.lineSeriesID

                });
            }

          //  MessageBox.Show("Count  node = " + menuStripNodeInfoValues.Count + " , line count = " + menuStripNodeLineInfoValues.Count);
            //=======================end of loading data : now lets plot the data values=======================//


            //ClearChart();   //this will clear the chart first
            // ResettingLines();
            // ReDrawingLineAndNode();
           // ReDrawingLineAndNodeForLoadXML();

        }

        string beginEditText = "";


        int flagSinglCellClick = 0;


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

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
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
                            //This flag is raised so that first coloumn is not selected everytime we do update
                            flagResistingForDGVChangeSelection = 1;//enable cell edit
                                    dataGridView1.Rows.Clear();

                                    //--Lets get the data and refill the data in datagridview.
                                    // PullDataFromDB();


                                    //LETS FILL THE DATA
                                    // fillDataGridView();
                                    DataGridView_Show_Data();//This will do both pulling data and filing the data...

                                    dataGridView1.Rows.Add();
                            dataGridView1.CurrentCell.Selected = false;
                          
                            //===============This one for refreshing the chart and selecting new created chart===//

                            //if (chartDetailList.Count > 0)
                            //{
                                if (dataGridView1.Rows.Count > 0) { 
                                //We need to select the particular index 
                                //--I hope this will do the replotting thing as well
                                if (dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                                {
                                    dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;//The row is selected 
                                }

                            }


                            if (dataGridView1.Rows.Count > 0) //If there is data in datagridview then only
                            {
                                //MessageBox.Show("Clicked here row index: " + e.RowIndex);
                                var eventArgs = new DataGridViewCellEventArgs(1, e.RowIndex);
                                // or setting the selected cells manually before executing the function
                               // MessageBox.Show("END edit = " + e.RowIndex);
                                dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;
                                dataGridView1_CellClick(sender, eventArgs);
                                // }
                            }

                            flagResistingForDGVChangeSelection = 0;//close enable of dgv_selectionChange event 

                            //===================end of selection of new created chart =======================//


                        }
                        else
                                {
                                  //  MessageBox.Show("No change in name");
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
                MessageBox.Show("Edit and then press tab to commit changes " + ex.Message);

               // var cell = dataGridView1.CurrentCell;
                //if ( cell == null)
                //{
                //    //  dataGridView1.CurrentCell.Value = string.Empty;
                //  //  return;
                //}
                //else { 
                // dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;
                dataGridView1.CurrentCell.Value = beginEditText;
                // }
            }
        }

         //--These are form the BuildingChartSetting
        string chartSelectedIDValue = null;
        public int indexForWhichChartIsSelected = 0;//--This index value stores the index value in a list of which chart is selected
        public int indexOfChartSelected = 0;
      public  int flagShow = 1; //means on

        // this is our worker
        BackgroundWorker bgWorker; //--This is the background worker for hevay lifting works


        public void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {

            if(dataGridView1.Rows.Count <= 0)
            {
                return;
            }

            if (flagForTimer == 1)
            {
                if (atimer.Enabled) // Check if the timer is running
                {
                    //Return error.........
                    atimer.Enabled = false;
                    atimer.Dispose();
                    flagForTimer = 0;

                }
            }//close of flagfortimer

                      
            //--Lets check if the datagridview1 is empty or chartDetailList is empty
            if (chartDetailList.Count <= 0)
            {
                //--This flag is for OFFLINE mode Or ONLINE mode
                //--Now you are going to turn OFF the offline mode and go into realtime mode
                FlagForCntdBG_Update = 0;//currently in realtime mode so 1
               

                //--also make the radio button to be OFF and other to be ON
               // rb_ON.Checked = true;

                //--IFno chart is present then make edit mode on
                FlagForLockUnlock = 1;//   This means edit mode ON 
                LockAndUnlock();//This method will make things lock

                return;
            }

                      
            //--Showing the data on cell selected...
            // MessageBox.Show("CELL SELECT " );
            //When dgv is click it clicks twice this if is written to stop those twice click.

            if (flagSinglCellClick == 1)
            {
                flagSinglCellClick = 0;
                return;
            }
            if ((e.RowIndex >= 0) && (e.ColumnIndex >= 0))
            {

                if (dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                {
                    // MessageBox.Show("CELL SELECT ROW= " + e.RowIndex+",columns = "+e.ColumnIndex);
                    //The row is selected ..
                    for (int i = 0; i < chartDetailList.Count; i++)
                    {
                        //--Checking for evey item
                        if (dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString() == chartDetailList[i].chartName)
                        {
                            //if match found load
                            //data_load();
                            lb_title_display.Text = "";//--Historical plot needs to be cleared
                            indexRun = 0;//For inserting nodeName and lineName like node1,line1 etc
                            chart1.Enabled = true;

                            //--This flag is for OFFLINE mode Or ONLINE mode
                            //--Now you are going to turn OFF the offline mode and go into realtime mode
                            FlagForCntdBG_Update = 1;//currently in realtime mode so 1
                            
                            //--also make the radio button to be OFF and other to be ON
                            // rb_OFF.Checked = true;

                            FlagForLockUnlock = 0;//   This means edit mode OFF 
                            LockAndUnlock();//This method will make things lock

                            //This one(the signal) is for stoping the background running process instantly 
                            FlagForStopingRunningProcessInstantly = 1;


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
                            ReDrawingLineAndNode();  //Done checking bbk modif: changing to alex db 


                            flagSinglCellClick = 1;

                            //lets enable the button
                       //  btn_pull_data.Enabled = true;
                        // button1.Enabled = true;
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

                            //---Here we are going to call scan hardware function ----------------//

                            //StartHardwareScan();
                            //---End of call for scan hardware-----------------------------------//

                            //--Lets make the status of the device and web to be dissabled 
                            lb_web_status.Text = "inactive";
                            lb_device_status.Text = "disconnected";



                          
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
                    flagForInsertOrUpdateDataToDB = 0; //==insert or update OFF
                }


                //This is for cell formating and editing...
                //  if (dataGridView1.CurrentCell.ColumnIndex >= 0) { 
                try { 
                if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    // this.dataGridView1.BeginEdit(false);
                    this.dataGridView1.CurrentCell.ReadOnly = true;
                }
                }
                catch { }
               // }
                //===============This one is for realtime update and other things




                // if(atimer.Enabled == true)
                // {
                //   atimer.Dispose();
                //atimer.Stop();
                //}
                //===========================BackGroundWorker WORKS HERE============//

                //--First if backgound worker if working already we need to cancel and start the new instance

                //  backgroundWorker1.CancelAsync();//first cancel any work that is being done 
                //if(backgroundWorker1.CancellationPending== true)
                //{
                //   backgroundWorker1.can
                //}
                //if (lb_test1.InvokeRequired)
                //{
                //    lb_test1.Invoke(new Action(() => lb_test1.Text = "testActionInvokeFromOnClick"));
                //}
                //else
                //{
                //    lb_test1.Text = "testnoinvokeFromOnClick11";
                //}


                if (backgroundWorker1.IsBusy)
                {
                   // MessageBox.Show("background asyc dissabled");
                    backgroundWorker1.CancelAsync(); //==First dissabling background worker

                }

                //--This willcancel the pending
                //this is our worker
                //bgWorker = new BackgroundWorker();
                //// work happens in this method
                //bgWorker.DoWork += new DoWorkEventHandler(bg_DoWork);
                //bgWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bg_RunWorkerCompleted);

                if (FlagForCntdBG_Update == 1)
                {
                    // MessageBox.Show("Entered");//--test
                    // chart1.Enabled = false;//should be dissabled 
                    //If the chart is in online mode then only do updating task

                    //--***********************Uncomment later********************//
                    InitTimerForDevice();
                    //--*************************end*****************************//

                }
                else
                {
                   // chart1.Enabled = true;
                }


                //===========================End of the backgound worker ====================================================//



                //================END of the real time update and other things .===========//



            }//close of columnindex if
            else if(e.RowIndex >= 0 && (e.ColumnIndex >= 0 && e.ColumnIndex <=1))
            {
                //we need to return and does not proceed forwared
                //select all 
               // dataGridView1.CurrentCell.Selected = true;

                return;
            }


        }
        //--Here is the task done by back ground worker
        void bg_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
                //--we cancel the worker 
                    
            }else if (e.Error != null)
            {
                //--we need to find the errors
                MessageBox.Show("Error: " + e.Error.Message);
  
            }
            else
            {
                //--The actual task


            }

        }

        void bg_DoWork(object sender, DoWorkEventArgs e)
        {
            // does a job like reading data from hardware and internet(web)

            RefreshDataFromDeviceAndWeb();


        }







        private void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            if (dataGridView1.CurrentCell.ColumnIndex == 1)
            {
                this.dataGridView1.CurrentCell.ReadOnly = false;
                this.dataGridView1.BeginEdit(true);

            }

        }
        int indexSelectedForDeletion = 0;

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.RowIndex >= 0 && e.ColumnIndex == 0)
                {
                    if (dataGridView1.Rows[e.RowIndex].Cells[0].Value != null)
                    {
                        Point p = new Point(e.X, e.Y);
                        indexSelectedForDeletion = e.RowIndex;
                        //dataGridView1.SelectedCells = true;
                        //DataGridViewCell c = (sender as DataGridView)[e.ColumnIndex, e.RowIndex];
                        //if (!c.Selected)
                        //{
                        //    c.DataGridView.ClearSelection();
                        //    c.DataGridView.CurrentCell = c;
                        //    c.Selected = true;
                        //}
                        //==Calculating the x and y coordinate

                        int xAxis = (int)e.X;
                        //--now alsow count the row index so that yavis = e.y+ (rowIndex*constantMultiple)
                        int rowIndex = dataGridView1.CurrentCell.RowIndex;
                        int yAxis = (int)e.Y+(rowIndex*22);

                        //new Point((int)e.X, (int)e.Y)
                        contextMenuStrip1.Show(dataGridView1, new Point(xAxis, yAxis));
                        //contextMenuStrip1.Show(dataGridView1.PointToScreen(e.Location));
                    }
                }


            }
        }


        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void insertNodeToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            //--Insert node got nothing...
            Form_Input_For_Seriespoint form_point_input = new Form_Input_For_Seriespoint(this);
            form_point_input.Show();

        }
        int indexRun = 0;
        private void quickNodeInsertToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //This will help in quick insertion           
            int t = menuStripNodeInfoValues.Count;
            indexRun = 1;
            //if (t > 0)
            //{
            //    indexRun = t+1; 
            //} else if(t == 0)
            //{
            //    indexRun = 1;
            //}



            if (t == 0)
            {
                indexRun = 1;
            }
            else
            {
                foreach (var node in menuStripNodeInfoValues)
                {
                    if (node.temperature_source != "Mix")//either temperature or humidity source will have mix as property
                    {
                        indexRun++;
                    }

                }
            }


            //We also need to check if the value is present or not after setting 


            int indNodeRun = indexRun;
            string nodeName = "Node"+ indNodeRun;//This is the node name
            for(int i =0;i<menuStripNodeInfoValues.Count;i++)
            {
                if(menuStripNodeInfoValues[i].name == nodeName)
                {
                    indNodeRun++;
                    nodeName = "Node" + indNodeRun;
                    i = 0;
                }

            }
            
            //indexRun = t + 1;//Resetting indexRun  Hami reset na garnea
            //string nodeLabel = "Label" + indNodeRun;
            //for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            //{
            //    if (menuStripNodeInfoValues[i].label == nodeLabel)
            //    {
            //        indNodeRun++;
            //        nodeLabel = "Node" + indNodeRun;
            //        i = 0;
            //    }

            //}
            int indLineRun = indexRun-1;
            //This one is for line Name
            string lineName = "Line" + indLineRun;
            for (int i = 0; i < menuStripNodeLineInfoValues.Count; i++)
            {
                if (menuStripNodeLineInfoValues[i].name == lineName)
                {
                    indLineRun++;
                    lineName = "Line" + indLineRun;
                    i = 0;
                }

            }
            //status of line is always on for quick line insert
            //  SetNode("Manual", nodeName, nodeLabel, Color.Blue, "Name", 20,lineName,1);//This will insert node and also update the nodes
         SetNode("Manual","Manual", nodeName,  Color.Blue,  20, lineName, 1,1000);//This will insert node and also update the nodes
        }

        private void disconnectLineToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            return;//no work
            //--This one is for disconnect lines
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            //flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            //// Cursor.Equals(Cursors.Cross);
            //if (Cursor != Cursors.Cross)
            //{
            //    Cursor = Cursors.Cross;
            //}

            ////--Lets add the series when the button is clicked and remove it when released..
            //chart1.Series.Add(addDottedSeries);
        }

        private void enableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This function helps in hiding or showing the comfort zone
           ShowOrHideForComfortZone(); //--Show comfortzone
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This is comfort zone settings.
            //Steps to set the comfort zone ...
            /*
            1.Let give user to set min and max humidity and min and max temp
             1.1 give user option for types of comfort zone.
            2.lets save those value in db 
            3.plot on graph base on those values.
            */
            //Lets show the comfortzone option
            comfort_zone cf = new comfort_zone(this);
            cf.ShowDialog();

        }

        public void SettingChartOpen()
        {
            comfort_zone cf = new comfort_zone(this);
            cf.ShowDialog();
        }

        int flagResistingForDGVChangeSelection = 0;//disENABLE;

        private void clearChartToolStripMenuItem_Click(object sender, EventArgs e)
        {


            try { 

            if (MessageBox.Show("Are you sure you want to clear this chart?", "Clear chart", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
            //if (dialogResult == DialogResult.Yes)
            {

                //--This function should clear the chart 
                /*
                Steps :
                0.Find which chart is selected.
                1. delete the information from the chart of both node and line
                2. Re plot the chart function 
                */

                string chartidsel = chartDetailList[indexOfChartSelected].chartID;
                string chart_resp_nodeIDX = chartDetailList[indexOfChartSelected].chart_respective_nodeID;
                string chart_resp_lineIDX = chartDetailList[indexOfChartSelected].chart_respective_lineID;

                    flagResistingForDGVChangeSelection = 1;//ENABLE;



                    //MessageBox.Show("DELETE CHART CONT..");
                    //Deleting the content of the chart 
                    DeleteChartContent(chartidsel, chart_resp_nodeIDX, chart_resp_lineIDX);//This deletes the content of chart

                   // MessageBox.Show("CLEAR CHART..");
                    ClearChart();
                    /*
                    clear chart will erase the the menustrip info of line and node so replotting it again
                    */
                    // MessageBox.Show("row clear..");
                    //dataGridView1.DataBindingComplete += this.dataGridView1_DataBindingComplete;
                    //MessageBox.Show("");
                   // MessageBox.Show("Before clear  =" + flagResistingForDGVChangeSelection);

                    dataGridView1.Rows.Clear();
                   // MessageBox.Show("DataGridView_Show_Data..");
                    DataGridView_Show_Data();
                   //MessageBox.Show("row add..");

                    dataGridView1.Rows.Add();
                    //if (dataGridView1.Rows.Count > 0)
                    //{
                    //    dataGridView1.Rows[0].Selected = false;
                    //}
                    //dataGridView1_DataBindingComplete(sender,(DataGridViewBindingCompleteEventArgs)e);
                    ////dataGridView1.ClearSelection();
                    ////dataGridView1.ClearSelection();
                    //MessageBox.Show("IF >SELECTED..");
                    //Select the chart with was selected
                    dataGridView1.CurrentCell.Selected = false;
                  
                    if (chartDetailList.Count > indexOfChartSelected)
                {
                        //We need to select the particular index 
                        //--I hope this will do the replotting thing as well
                        // dataGridView1.Rows[indexOfChartSelected].Selected = true;//The row is selected 
                     //   MessageBox.Show("indexOfChartSelected= " + indexOfChartSelected);
                        dataGridView1.Rows[indexOfChartSelected].Selected = true;//The row is selected 

                 }

                    flagResistingForDGVChangeSelection = 0;//DISENABLE;
                    //if (dataGridView1.CurrentCell.RowIndex.ToString() != "")
                    //{
                    //    //set parameters of your event args
                    //    var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex);
                    //    //or setting the selected cells manually before executing the function
                    //    //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[dataGridView2.CurrentCell.ColumnIndex].Selected = true;
                    //    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                    //    dataGridView1_CellClick(sender, eventArgs);
                    //}





                }//Close of clear chart

            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        //===============================This section if for deleting node,line and deviceinfo of a node from chart============//



        /// <summary>
        /// This list containts the node whose souce = Device
        /// </summary>
        public List<TempDataType> nodeInfoContainingDevice = new List<TempDataType>();
        /// <summary>
        /// This helps to delete node,line and device info for node,form db to clear the chart,does not delete chart only content 
        /// </summary>
        public void DeleteChartContent(string chartID,string chart_resp_nodeID,string chart_resp_lineID)
        {
            /*
            Steps : 
            1.Read the node and line respective chart id info
            2. using node respective chart id read the node having source="Device" [This is used for deleting device info of each selected node]
            3. Delete the line info first using chart_respective_lineID form tbl_..._line_value
            4.Delete the device info form stored source   form tbl_..._device_info_for_node
            5.Delete the node info form tbl_..._node_value
            6.Delete the chart info now from tbl_..._chart_detail
            */

            // int selectedItemIndex = int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

            //we need to find the corresponding tables for deletion.

            //int id = selectedItemIndex;
            // string tableName = "tbl_"+
            nodeInfoContainingDevice.Clear();//Resetting list
            string nodeTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";
            string tableNameDevice = "tbl_" + selectedBuildingList[0].BuildingName + "_device_info_for_node";//currentNodeTableFromDB; 
            string tableComfortZone = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_comfort_zone_setting";//currentNodeTableFromDB; 
            string tableTempHumSourceInfo = "tbl_" + selectedBuildingList[0].BuildingName + "_TemperatureHumiditySourceInfo";
            //lets get the id values...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

            string sql1 = "SELECT * From  " + nodeTableName + "  where chart_respective_nodeID ='" + chart_resp_nodeID + "'";
            //  MessageBox.Show("sql1=" + sql1);

            //(nodeID,device_instanceID,IP,param1ID_X,param2ID_Y) VALUES(@id,@instanceID,@IP,@param1,@param2)";

            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                SQLiteCommand cmd = new SQLiteCommand(sql1, thisConnection);

                thisConnection.Open();

                SQLiteDataReader reader = cmd.ExecuteReader();

                // MessageBox.Show("cmd error= " + cmd.CommandText);

                while (reader.Read())
                {
                    if ((reader["temperature_source"].ToString() == "Device")|| (reader["humidity_source"].ToString() == "Device"))
                    { 
                    nodeInfoContainingDevice.Add(new TempDataType
                    {
                        id = reader["nodeID"].ToString(), //This is just changed code : bbk305
                        xVal = double.Parse(reader["xValue"].ToString()),
                        yVal = double.Parse(reader["yValue"].ToString()),
                       // temperature_source = reader["temperature_source"].ToString(),
                        // humidity_source = reader["humidity_source"].ToString(),


                        name = reader["name"].ToString(),
                       // label = reader["label"].ToString(),
                        colorValue = ColorTranslator.FromHtml(reader["colorValue"].ToString()),
                        //showItemText = reader["showTextItem"].ToString(),
                        marker_Size = int.Parse(reader["nodeSize"].ToString()),
                        temperature_source = reader["temperature_source"].ToString() ,
                        humidity_source = reader["humidity_source"].ToString()  ,
                        airFlow = double.Parse(reader["airFlow"].ToString()),
                        lastUpdatedDate = reader["lastUpdatedDate"].ToString()

                          

                    });
                }  //close of if

                }

            }//close of using...

            //Deleting the node info from device  ie all the device information of node

            if (nodeInfoContainingDevice.Count > 0) {

                for(int i = 0; i < nodeInfoContainingDevice.Count; i++)
                { 
                using (SQLiteConnection connection = new SQLiteConnection(connString1))
                {
                    connection.Open();
                    string queryString = "delete from "+ tableNameDevice + " where nodeID = @id_value";
                    SQLiteCommand command = new SQLiteCommand(queryString, connection);
                    command.Parameters.AddWithValue("@id_value", nodeInfoContainingDevice[i].id);
                    command.ExecuteNonQuery();

                }//Close of using

                } //close of for

            }//close of if

            //-----Now deleting the line

            string lineTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + lineTableName + " where chart_respective_lineID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chart_resp_lineID);
                command.ExecuteNonQuery();

            }//Close of using


            //===Deleting comfort zone setting for clear chart
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + tableComfortZone + " where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                command.ExecuteNonQuery();

            }//Close of using



            //--now deleting the node info from the main node info
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + nodeTableName + " where chart_respective_nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chart_resp_nodeID);
                command.ExecuteNonQuery();

            }//Close of using


            ///===========This one is for deleting the mix node information form chart==========//
            string tableNameForMixValue= "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + tableNameForMixValue + " where chartID = @id_value_x";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value_x", chartID);
                command.ExecuteNonQuery();

            }//Close of using

            //--For clearing source and humidity info
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + tableTempHumSourceInfo + " where chartID = @id_value_x";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value_x", chartID);
                command.ExecuteNonQuery();

            }//Close of using


            ////--Now lets delete the chart actual content for the tbl_..._chart_detail table

            //string chartDetailTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            //using (SQLiteConnection connection = new SQLiteConnection(connString1))
            //{
            //    connection.Open();
            //    string queryString = "delete from " + chartDetailTableName + " where chartID = @id_value";
            //    SQLiteCommand command = new SQLiteCommand(queryString, connection);
            //    command.Parameters.AddWithValue("@id_value", chartID);
            //    command.ExecuteNonQuery();

            //}//Close of using


        }
        //This deletes just the chart name and info
        public void DeleteChartInfoFromChartDetailTable(string chartID)
        {

            //lets get the id values...
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";


            //--Now lets delete the chart actual content for the tbl_..._chart_detail table
            string chartDetailTableName = "tbl_" + selectedBuildingList[0].BuildingName + "_chart_detail";
            using (SQLiteConnection connection = new SQLiteConnection(connString1))
            {
                connection.Open();
                string queryString = "delete from " + chartDetailTableName + " where chartID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", chartID);
                command.ExecuteNonQuery();

            }//Close of using


        }

        private void editNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This section helps in editing the node values
            //This section contains the edit node section
            try
            {
                EditNodeLineForm f = new EditNodeLineForm(this);
                f.ShowDialog();//This will help to wait for second dialog to be closed first.
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void deleteToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
          //  DialogResult dialogResult = MessageBox.Show( "Are you sure you want to delete this chart?", "Delete chart", MessageBoxButtons.YesNo,MessageBoxIcon.Exclamation);

            if (MessageBox.Show("Are you sure you want to delete this chart?", "Delete chart", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation)== DialogResult.Yes)
            //if (dialogResult == DialogResult.Yes)
            {
                //--Delete function 
                //--This is the delete operation for the handler.....
                /*
                Steps: 1. delete the row  of the table using the id portion.
                2. Delete the corresponding tables related to the row.             
                */

                //=========================================================This code is commented for not deleting the values===============//

                //if (dataGridView1.CurrentCell.RowIndex > -1 && dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)//Header is selected..
                //{
                //   int selectedItemIndex = dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());

                //    //we need to find the corresponding tables for deletion.

                //    //int id = selectedItemIndex;

                //  string chartID = chartDetailList[selectedItemIndex].chartID;
                //    string chart_respective_node_ID = chartDetailList[selectedItemIndex].chart_respective_nodeID;
                //    string chart_respective_line_ID = chartDetailList[selectedItemIndex].chart_respective_lineID;
                //    //First read the node values for particular chart 
                //    ReadNodeInfoToDelete(chart_respective_node_ID);

                //    //For all node delete the device list
                //    if (deleteNodeDetailList.Count > 0)
                //    {
                //        //if there is data then delete the device infor
                //        foreach (var item in deleteNodeDetailList)
                //        {    
                //                if(item.source == "Device")
                //                {                         
                //            DeleteNodeDeviceInfo(item.nodeID);
                //                }
                //            }
                //    }

                //    //After this deletion lets delete the line info
                //    DeleteLine(chart_respective_line_ID);//This deletes the line

                //    //now delete comfort zone..
                //    DeleteComfortZoneSettingForChart(chartID);

                //    //now delete the node value
                //    DeleteNode(chart_respective_node_ID);

                //    //Now delete the chart itself
                //    DeleteChart(chartID);


                //}//Close of if

                //    //--Refreshing the tables for all the datas..

                //  //  try { 

                //  //dataGridView1.ref

                //      //  MessageBox.Show("Reached before refresh");
                //        dataGridView1.Rows.Clear(); //--THIS LINE IS THE PROBLEM
                //    //}
                //    //catch (Exception ex)
                //    //{
                //    //    MessageBox.Show("Exception : data" + ex.Data + "\n source" + ex.Source + "\n exception" + ex);
                //    //}




                //=======================================================end of deleting values=============================//



                //===New added code==========================================//
                int selectedItemIndex = dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());
                          
                string chartID = chartDetailList[selectedItemIndex].chartID;

                DisableChart(chartID);
                //===END of NEW ADDED CODE===========================//



                dataGridView1.Rows.Clear();//--This one is for clearing the data
                dataGridView1.Refresh();//--Release the previously selectecd items
                    dataGridView1.Rows.Add();
                 //   MessageBox.Show("Reached before refreshGraph() and after datagv ref");
                    DataGridView_Show_Data();

                        //If there is no rows in datagridview...
                        //if(dataGridView1.Rows.Count < 1) { 
                   
              
               
                RefreshGraph();

            //after deletion lets select the its corresponding upper chart
            //Select the chart with was selected
            if (chartDetailList.Count > 0)
            {
                //We need to select the particular index 
                //--I hope this will do the replotting thing as well
                if(dataGridView1.Rows[0].Cells[1].Value != null)
                    { 
              dataGridView1.Rows[0].Cells[1].Selected = true;//The row is selected 
                    }

                }


                if (chartDetailList.Count > 0)
                {
                    //if (dataGridView1.CurrentCell.RowIndex >= 1)
                    //{
                    //    //set parameters of your event args
                    //    var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex-1);
                    //    // or setting the selected cells manually before executing the function
                    //    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex - 1].Cells[1].Selected = true;
                    //    dataGridView1_CellClick(sender, eventArgs);
                    //} else
                    //{ 
                    //set parameters of your event args


                    var eventArgs = new DataGridViewCellEventArgs(1, 0);
                    // or setting the selected cells manually before executing the function
                    dataGridView1.Rows[0].Cells[1].Selected = true;
                    dataGridView1_CellClick(sender, eventArgs);



                    // }
                }


                //dataGridView2.Rows.Clear();
                // MessageBox.Show("Chart deleted successfully !");
            }


        }


        public void RefreshingEverythingChartAndContent(object sender)
        {


            dataGridView1.Rows.Clear();//--This one is for clearing the data
            dataGridView1.Refresh();//--Release the previously selectecd items
            dataGridView1.Rows.Add();
            //   MessageBox.Show("Reached before refreshGraph() and after datagv ref");
            DataGridView_Show_Data();

            //If there is no rows in datagridview...
            //if(dataGridView1.Rows.Count < 1) { 



            RefreshGraph();

            //after deletion lets select the its corresponding upper chart
            //Select the chart with was selected
            if (chartDetailList.Count > 0)
            {
                //We need to select the particular index 
                //--I hope this will do the replotting thing as well
                if (dataGridView1.Rows[0].Cells[1].Value != null)
                {
                    dataGridView1.Rows[0].Cells[1].Selected = true;//The row is selected 
                }

            }


            if (chartDetailList.Count > 0)
            {
                //if (dataGridView1.CurrentCell.RowIndex >= 1)
                //{
                //    //set parameters of your event args
                //    var eventArgs = new DataGridViewCellEventArgs(1, dataGridView1.CurrentCell.RowIndex-1);
                //    // or setting the selected cells manually before executing the function
                //    dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex - 1].Cells[1].Selected = true;
                //    dataGridView1_CellClick(sender, eventArgs);
                //} else
                //{ 
                //set parameters of your event args
                if(indexOfChartSelected >= 0 && indexOfChartSelected < chartDetailList.Count)
                {
                    var eventArgs = new DataGridViewCellEventArgs(1, indexOfChartSelected);
                    // or setting the selected cells manually before executing the function
                    dataGridView1.Rows[indexOfChartSelected].Cells[1].Selected = true;
                    dataGridView1_CellClick(sender, eventArgs);

                }
                else { 
                var eventArgs = new DataGridViewCellEventArgs(1, 0);
                // or setting the selected cells manually before executing the function
                dataGridView1.Rows[0].Cells[1].Selected = true;
                dataGridView1_CellClick(sender, eventArgs);
                    // }
                }
            }

        }




        private void button1_Click_1(object sender, EventArgs e)
        {


            //string pathToFile = @"Data Source=C:\Users\Asus\Desktop\123.s3db;Version=3;";

            //Test(pathToFile);//This reads all the data


        }

        public void Test(string path)
        {

            //--Resetting values first

            //string connectionString = @"Data Source=" + path + ";Version=3;";

            string sql_for_chart_detail = "SELECT NAME from sqlite_master";


            //  MessageBox.Show("sql1=" + sql1);         
            using (SQLiteConnection conx = new SQLiteConnection(path))
            {

                MessageBox.Show("######");
                // conx.Close();
                conx.Open();
                MessageBox.Show("Khulyo!");
                 string s = "";
                SQLiteCommand cmd = new SQLiteCommand(sql_for_chart_detail, conx);
                SQLiteDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {

                    s += reader["NAME"].ToString()+"\n";
                }

              //  MessageBox.Show("TABLE NAME = " + s);

   
            } //close of using statement 
   
        }
   

        private void dataGridView1_SelectionChanged_1(object sender, EventArgs e)
        {
            //--This need to fire an event when the selection is done 

            if (flagResistingForDGVChangeSelection == 1)
            {
               // MessageBox.Show("Test");
                return;
            }
            //dataGridView1_CellClick(sender, e);
            if (chartDetailList.Count > 0)
            {
                //try { 
                var cellCopy = dataGridView1.CurrentCell;

                if(cellCopy != null)
                { 
                if (dataGridView1.CurrentCell.ColumnIndex.ToString() != "" && dataGridView1.CurrentCell.RowIndex.ToString() != "")
                {
                        //set parameters of your event args
                        //if(dataGridView1.CurrentCell.RowIndex >=0 && dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                        //        { 
                        //var eventArgs = new DataGridViewCellEventArgs(dataGridView1.CurrentCell.ColumnIndex, dataGridView1.CurrentCell.RowIndex);
                        ////or setting the selected cells manually before executing the function
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                        //dataGridView1_CellClick(sender, eventArgs);
                        //            //MessageBox.Show("selecteion change clicked");
                        //        }
                        if (dataGridView1.CurrentCell.RowIndex >= 0 && dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                        {
                            var eventArgs = new DataGridViewCellEventArgs(dataGridView1.CurrentCell.ColumnIndex, dataGridView1.CurrentCell.RowIndex);
                            //or setting the selected cells manually before executing the function
                            dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                            dataGridView1_CellClick(sender, eventArgs);
                            //MessageBox.Show("selecteion change clicked");
                        }
                    }
                  
                }
                //}catch(Exception ex)
                //{
                //    MessageBox.Show(ex.Message);
                //}
            }



        }

        private void dataGridView1_CellEnter(object sender, DataGridViewCellEventArgs e)
        {
            dataGridView1_CellClick(sender, e);
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            //--do worker 

            RefreshDataFromDeviceAndWeb();


        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            //do task if cnd flag is on
            if(FlagForCntdBG_Update == 1) { 
            
            
            //-- work completed

            if (e.Cancelled)
            {
                    //--we cancel the worker 
                   // MessageBox.Show("Cancled : ln :12759");


            }
            else if (e.Error != null)
            {
                //--we need to find the errors
                MessageBox.Show("Error: " + e.Error.Message);

            }
            else
            {
                    ////--The actual task
                    //if (chart1.InvokeRequired)
                    //{ 
                    // chart1.Invoke(new Action(()=>  chart1.Enabled = true));
                    //}
                    //else
                    //{
                    //    chart1.Enabled = true;
                    //}

                    //While we are updating we do not want to change the chart so lock until this task is done
                    try
                    {

                        // lock (dataGridView1)
                        //{ 

                        if (InvokeRequired)
                        {
                           // MessageBox.Show("Let me crash in here");
                            Invoke(new MethodInvoker(RefreshGraph));
                        }
                        else
                        {
                            RefreshGraph();
                        }
                        if (chartDetailList.Count > 0)
                        {
                            int id = indexOfChartSelected;    //This value is changed 
                            LoadNodeAndLineFromDB(id);   //Lets make it passing the stirngs 
                           // MessageBox.Show("Let me be loading in backgoundworker1_");

                            //====Once loaded we need to check and update for mix nodes as they have not been updated and
                            //=======again we need to load the data form db and then redraw okie
                            foreach(var node in menuStripNodeInfoValues)
                            {
                                if(node.temperature_source != "Mix")
                                {
                                    //==Then only update the values
                                    DBUpdateMixPointOnNodeValueChange(node.id);

                                }

                            }

                            //==Now again load the data 
                            LoadNodeAndLineFromDB(id);

                            // flagForInsertOrUpdateDataToDB = 1;
                            //--This is also completed..
                            ReDrawingLineAndNode();
                           // ReloadComfortZoneForBackGroundWorker();

                        }
                        //} //close of lock
                    }catch(Exception ex)
                    {
                       // MessageBox.Show(ex.Message);
                    }

                }

            }
            else
            {
                //chart1.Enabled = false;
                //if (chart1.InvokeRequired)
                //{
                //    chart1.Invoke(new Action(() => chart1.Enabled = false));
                //}
                //else
                //{
                //    chart1.Enabled = false;
                //}
            }


        }
        //==Lets have a flag for ON and OFF 
        int FlagForCntdBG_Update = 1;//On currently
        int FlagForDissableLeftAndRightClicksInChart = 0;//==do not dissable /OFF
        private void rb_OFF_CheckedChanged(object sender, EventArgs e)
        {
            //if(rb_OFF.Checked == true) { 
            ////--If this is clicked the OFF
            ////--When the offline mode is OFF means you are in online mode so check for continuous update and update it
            //FlagForCntdBG_Update = 1;//On currently

            // FlagForDissableLeftAndRightClicksInChart = 1;//turn on 



            //}
        }

        private void rb_ON_CheckedChanged(object sender, EventArgs e)
        {
        //    if (rb_ON.Checked == true)
        //    {
        //        //--When the offline mode is ON means you are in editing mode so dissable the background task that is being done
        //        FlagForCntdBG_Update = 0;//OFF currently

        //        FlagForDissableLeftAndRightClicksInChart = 0;//==TURN OFF
        //    }
        }

        int FlagForLockUnlock = 0;//0 means lock and 1 means unlock
        private void pb_lock_unlock_Click(object sender, EventArgs e)
        {
            LockAndUnlock();
        }

         /// <summary>
         /// This function helps to lock and unlock the when click is performed
         /// in picture box one click = lock and another = unlock
         /// </summary>
        public void LockAndUnlock()
        {
            //--We need to load and unload the image 
            if (FlagForLockUnlock == 0)
            {
                //Edit mode is dissabled //Edit OFF section
                //--Load image lock image
               // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                //string NewDirectory = Path.GetFullPath(Path.Combine(dir, @"..\"));
                //string file = NewDirectory + @"Database\image\lock.png";


                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = imagePath + @"\image\lock.png";

                Bitmap SOME = new Bitmap(Image.FromFile(file));

                Bitmap bp = new Bitmap(SOME, pb_lock_unlock.Width, pb_lock_unlock.Height);
                pb_lock_unlock.Image = bp;//Image.FromFile(file);

                //==Code form edit OFF
                FlagForCntdBG_Update = 1;//On currently
               FlagForDissableLeftAndRightClicksInChart = 1;//turn on 
               //==End of edit OFF
                FlagForLockUnlock = 1;
                lb_realtimeshow.Text = "Real Time Mode";
            }
            else
            {
                //Edit mode is enable
                //--Load image lock image
               // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

               // string NewDirectory = Path.GetFullPath(Path.Combine(dir, @"..\"));
               // string file = NewDirectory + @"Database\image\unlock.png";


                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = imagePath + @"\image\unlock.png";

                Bitmap SOME = new Bitmap(Image.FromFile(file));
                Bitmap bp = new Bitmap(SOME, pb_lock_unlock.Width, pb_lock_unlock.Height);

                pb_lock_unlock.Image = bp;//Image.FromFile(file);
                //==============Edit ON section these flags make the Edit on=====//
                FlagForCntdBG_Update = 0;//OFF currently
                FlagForDissableLeftAndRightClicksInChart = 0;//==TURN OFF
                //==============End of edit on flag decleration =========//

                FlagForLockUnlock = 0;
                lb_realtimeshow.Text = "Edit Mode";
            }
        }

        int FlagForNodeDelete = 0;//0 means node not selected 1 means selected

        string nodeID_ForDeletingNode = "";
        private void deleteNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Delete the node currently selected 
            /*Steps  : 
            1. Find node id first 
            2. delete the node from node list 
            3. Delete the corresponding line id
            4.Refresh the button            
            */


            if(FlagForNodeDelete == 1)//Node has been selected
            {

                if(nodeID_ForDeletingNode != "")
                {
                    //--Then we want to perform some operation
                    DeleteSingleNodeAndAssociatedLinesInDB(nodeID_ForDeletingNode);


                    //--Replotting the data again
                    RefreshGraph();
                 
                    LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

                    ReDrawingLineAndNode();



                }
            }
        }

       public void DeleteSingleNodeAndAssociatedLinesInDB(string nodeID)
        {
            DeleteNodeUsingNodeID(nodeID);
            DeleteLineUsingNodeID(nodeID);


        }
        public void DeleteNodeUsingNodeID(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_node_value";// "tbl_" ++"_node_value";
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

            //==This one is for deleting mix node info if present 
            string tableMixNodeInfo = "tbl_" + selectedBuildingList[0].BuildingName + "_mix_node_info";// "tbl_" ++"_node_value";
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableMixNodeInfo + "  where nodeID = @id_value";
                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string
                reader = command.ExecuteReader();
                   
            }//Close of using 


        }

        public void DeleteLineUsingNodeID(string nodeID)
        {
            string tableName = "tbl_" + selectedBuildingList[0].BuildingName + "_line_value";// "tbl_" ++"_node_value";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";


            //==Deleting if previous nodeid is matched
            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where prevNodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();


            }//Close of using 


            //Deleting if nextNodeISmatched

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();
                SQLiteDataReader reader = null;
                string queryString = "delete   from  " + tableName + "  where nextNodeID = @id_value";


                SQLiteCommand command = new SQLiteCommand(queryString, connection);
                command.Parameters.AddWithValue("@id_value", nodeID);
                //SqlDataAdapter dataAdapter = new SqlDataAdapter(queryString, connection.ConnectionString); //connection.ConnectionString is the connection string

                reader = command.ExecuteReader();


            }//Close of using 


        }

         /// <summary>
         ///variable contains weather prenode is on or next node is on
         /// </summary>
        int flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 0;//-- 0 means off 1 means on for previous node    and 2 means next node

        //indexOfPrevPointForLineMovement  [containt the prenodeid]
        //indexOfnext..   [continas the next node id]
        private void disconnectLineFromAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This one is for  detaching line form A
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 1;//now prev node is ON

            //---------This code is from Disconnect Line menustipe-------//


            //--This one is for disconnect lines
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option-----------//




        }

       // int FlagForDisconnectLineFromNodeB_NextNode = 0;//-- 0 means off 1 means on for previous node
        private void disconnectLineFromBToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //---This one is for detaching line form B
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 2; //Now next node is ON

            //---------This code is from Disconnect Line menustipe-------//
            //--This one is for disconnect lines
           /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option-----------//


        }

        private void nodeAToolStripMenuItem_Click(object sender, EventArgs e)
        {

            //Deleting form A

            //--This one is for  detaching line form A
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 1;//now prev node is ON

            //---------This code is from Disconnect Line menustipe-------//


            //--This one is for disconnect lines
            /*--Steps :
           1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
           2.Then lets show a + cursor to indicate a line has been selected 
           3. When ever the line goes near to a node lets show a hand to drop the line.
           4. When clicked drop then lets connect it to different node which has been dropped.
           */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option-----------//

        }

        private void nodeBToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //---This one is for detaching line form B
            flagForDisconnectLineFromNode_PrevNode_Or_NextNode = 2; //Now next node is ON

            //---------This code is from Disconnect Line menustipe-------//
            //--This one is for disconnect lines
            /*--Steps :
            1.Lets draw a virtual line with a prevNodeId as first point and nextNodeID as second point..
            2.Then lets show a + cursor to indicate a line has been selected 
            3. When ever the line goes near to a node lets show a hand to drop the line.
            4. When clicked drop then lets connect it to different node which has been dropped.
            */
            flagForDisconnectClick = 1; //Enabled flag for disconnect click is enabled 
            // Cursor.Equals(Cursors.Cross);
            if (Cursor != Cursors.Cross)
            {
                Cursor = Cursors.Cross;
            }

            //--Lets add the series when the button is clicked and remove it when released..
            chart1.Series.Add(addDottedSeries);
            //--------End of the disconnect line option--------//

        }


         /// <summary>
         /// Contains minimum temperature which is constantly
         /// updated when moving mouse in edit mode after line selection
         /// </summary>
        int minTemperatureForNewComfortZoneCreate = 0;
        /// <summary>
        /// Contains max temperature which is constantly
        /// updated when moving mouse in edit mode after line selection
        /// </summary>
        int maxTemperatureForNewComfortZoneCreate = 0;
        /// <summary>
        /// Contains minimum humidity which is constantly
        /// updated when moving mouse in edit mode after line selection
        /// </summary>
        int minHumidityForNewComfortZoneCreate = 0;
        /// <summary>
        /// Contains max humidity which is constantly
        /// updated when moving mouse in edit mode after line selection
        /// </summary>
        int maxHumidityForNewComfortZoneCreate = 0;
        private void editComfortzoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
            This function helps to show edit comfortzone 
            steps : 
            1. Create the flag and setting it             
            */

            //==Some debugging if the enableToolStripMenuItem is not checked means no chart is present so  return 
            if(enableToolStripMenuItem.Checked == false)
            {
               // MessageBox.Show("false message");
               //There is no chart currently so return 
                return;
            }


            if (listchartComfortZoneInfoSingle.Count > 0)
            { //--If the chart is present then only do the edit operation
              //If one we assign we 
                minTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_temp);//  (int)xValue;
                maxTemperatureForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_temp);
                minHumidityForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].min_hum);
                maxHumidityForNewComfortZoneCreate = int.Parse(listchartComfortZoneInfoSingle[0].max_hum);

                EditComfortZoneGraphicalWay();

            }

        }

        //We need flag for dissabling other activities and saving our
        /// <summary>
        /// raises a flag when 1 comfortzone edit 
        /// is on and when 0 no edit operation on 
        /// comfortzone
        /// </summary>
        int flagForEditComfortZoneGraphically = 0;//Off currently 0 means OFF 1 means ON
        /// <summary>
        ///  flag is raised when we select one of the border line 
        /// by clicking it in comfortzone edit mode ,has three stage 0 means 
        /// neither selected nor leased,1 means selected ,and 2 means release
        /// 3   means to create new chart
        /// </summary>
        int flagForBorderLineSelectedForMoveForEditCF = 0;//
        /// <summary>
              /// Helps to edit the comfort one when click 
              /// raises four lines which can be dragged based on
              /// temperature along x axis 
              /// and humidity along y axis
              /// </summary>
        public void EditComfortZoneGraphicalWay()
        {
            /*Steps :
            1.show comfortzone with lines These lines should be black in color 
            2.
            */
            PlotOuterBorderForComfortZone(double.Parse(listchartComfortZoneInfoSingle[0].min_temp), double.Parse(listchartComfortZoneInfoSingle[0].max_temp), double.Parse(listchartComfortZoneInfoSingle[0].min_hum), double.Parse(listchartComfortZoneInfoSingle[0].max_hum), Color.Black);

            flagForEditComfortZoneGraphically = 1;

        }
           /// <summary>
           /// it plots outer border lines for editing comfort zone 
           /// normaly 2 lines vertical and 2 spline horizontal are drawn
           /// vertical line represent temperature and horizontal line 
           /// runs through in humidity line
           /// </summary>
           /// <param name="Tmin">minimum temperature in deg Cel </param>
           /// <param name="Tmax">maximum temperature in Deg Cel</param>
           /// <param name="Hmin">minimum humidity in % like 30%</param>
           /// <param name="Hmax">maximum humidity in % like 40%</param>
           /// <param name="c">color value</param>
        public void PlotOuterBorderForComfortZone(double Tmin, double Tmax, double Hmin, double Hmax, Color c)
        {

            //Check out-of-bound condition 
            if ((Tmin < 0 || Tmin > 50)|| (Tmax < 0 || Tmax > 50))
            {
                return;
            }
            if ((Hmin < 0 || Hmin > 100) || (Hmax < 0 || Hmax > 100))
            {
                return;
            }


            //This function will help to plot the values in the chart.

            //First reset the series if present and the re add them

            //ADDING LEFT SIDE SERIES => LEFT SIDE BORDER

            Series leftSideSeries = new Series("Vertical_Temp_Left_Side");

            if (chart1.Series.IndexOf(leftSideSeries.Name) != -1)
                {
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(leftSideSeries.Name));
                }

                //Now adding it
            leftSideSeries.Color = c;
            leftSideSeries.ChartType = SeriesChartType.Line ;
            leftSideSeries.BorderWidth = 3;
            chart1.Series.Add(leftSideSeries);
             
            //This series is for down side 
            Series rightSideSeries = new Series("Vertical_Temp_Right_Side");

            if (chart1.Series.IndexOf(rightSideSeries.Name) != -1)
            {
                //--This  means the series is present....
                chart1.Series.RemoveAt(chart1.Series.IndexOf(rightSideSeries.Name));
            }

            //Now adding it
            rightSideSeries.Color = c;
            rightSideSeries.ChartType = SeriesChartType.Line;
            rightSideSeries.BorderWidth = 3;
            
            chart1.Series.Add(rightSideSeries);


            Series downSideSeries = new Series("Horizontal_Hum_Down_Side" );

                if (chart1.Series.IndexOf(downSideSeries.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Series.RemoveAt(chart1.Series.IndexOf(downSideSeries.Name));
                }

            //Now adding it
            downSideSeries.Color = c;
            downSideSeries.ChartType = SeriesChartType.Spline;
            downSideSeries.BorderWidth = 3;
            chart1.Series.Add(downSideSeries);



            Series upperSideSeries = new Series("Horizontal_Hum_Upper_Side");

            if (chart1.Series.IndexOf(upperSideSeries.Name) != -1)
            {
                //--This  means the series is present....
                chart1.Series.RemoveAt(chart1.Series.IndexOf(upperSideSeries.Name));
            }

            //Now adding it
            upperSideSeries.Color = c;
            upperSideSeries.ChartType = SeriesChartType.Spline;
            upperSideSeries.BorderWidth = 3;
            chart1.Series.Add(upperSideSeries);


         
            //Now lets do the actual plotting part.


            double phi_min = Hmin / 100;//need to change to decimal
            double phi_max = Hmax / 100;


            //This one is for adding horizontal lines 
            double phi = phi_min; //0.1;            
            double x2 = 0;
          //  double indexValue = Hmin;
            double patm1 = AirPressureFromDB * 0.001;//101.325; ;//thsi need tochange

            for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)              
                {

                    double pg_valueX = Double.Parse(pg[temp].ToString());
                    double wg_calcX = (622 * phi_min * pg_valueX / (patm1 - phi_min * pg_valueX));
                    double y =wg_calcX;
                    x2 = temp;//double.Parse(t[i].ToString());
                    chart1.Series["Horizontal_Hum_Down_Side"].Points.AddXY(x2, y);   //Lower Humidity line
                //Test
                //if (temp == (int)Tmin)
                //{
                //    lb_test1.Text += "horizontal humdown x2-" + x2 + ",y -" + y;
                //}
                }//close of for





            //--Now adding the upper side humidity lines 
           // phi = phi_max;
            for (int temp = (int)Tmin; temp <= (int)Tmax; temp++)
            //for (double temp = Tmin; temp <Tmax;temp+=0.25)
            {

                double pg_value = Double.Parse(pg[temp].ToString());
                double wg_calc = (622 * phi_max * pg_value / (patm1 - phi_max * pg_value));
                double y =wg_calc;
                x2 = temp;//double.Parse(t[i].ToString());
                chart1.Series["Horizontal_Hum_Upper_Side"].Points.AddXY(x2, y);   //Lower Humidity line

            }//close of for

            int dataPointNumber =(int)(Tmax - Tmin) / 2;//Middle value    
                                                        //Now lets add the text to these
            string upperHumText = "H= "+(int)Hmax;
            string lowerHumText = "H= "+(int)Hmin;
            chart1.Series["Horizontal_Hum_Upper_Side"].Points[dataPointNumber].Label = upperHumText;
            chart1.Series["Horizontal_Hum_Down_Side"].Points[dataPointNumber].Label = lowerHumText;

            //--Now lets add the two lines min temp and max temp lines
            //Yvalue of lower left part line
            int min_temperature = (int)Tmin;
            double min_humidity_yvalue_y1 = 0;
           // phi = phi_min;
            double pg_value11 = Double.Parse(pg[min_temperature].ToString());
            double wg_calc11 = (622 * phi_min * pg_value11 / (patm1 - phi_min * pg_value11));
            double y1 = wg_calc11;
            min_humidity_yvalue_y1 = y1;  // Math.Round(y1,2);//This contains the min humidity  lower section value

            //Yvalue of upper left part  line          
            double min_humidity_yvalue_y2 = 0;
           // phi = phi_min;
            double pg_value1122 = Double.Parse(pg[min_temperature].ToString());
            double wg_calc1122 = (622 * phi_max * pg_value1122 / (patm1 - phi_max * pg_value1122));
            double y122 = wg_calc1122;
            min_humidity_yvalue_y2 = y122;  //Math.Round(y122, 2);//This contains the min humidity upper section value


            //Yvalue of the lower right part line 
            int max_temperature = (int)Tmax;
           double  max_humidity_yvalue_y1 = 0;
           // phi = phi_max;
            double pg_value12 = Double.Parse(pg[max_temperature].ToString());
            double wg_calc1223 = (622 * phi_min * pg_value12 / (patm1 - phi_min * pg_value12));
            double y21 = wg_calc1223;
            max_humidity_yvalue_y1 = y21;//  Math.Round(y21, 2);

            //Yvalue of the upper right part line
            double max_humidity_yvalue_y2 = 0;
           // phi = phi_max;
            double pg_value123 = Double.Parse(pg[max_temperature].ToString());
            double wg_calc123 = (622 * phi_max * pg_value123 / (patm1 - phi_max * pg_value12));
            double y23 = wg_calc123;
            max_humidity_yvalue_y2 = y23;//  Math.Round(y23, 2);

            //lets find the value between min_hum_yvalue and max_hum_yvalue so we can label at that position
            int midHumYvalueByDifference =(int) (min_humidity_yvalue_y1 + min_humidity_yvalue_y2) / 2;

            int midHumDiffYvalueForRightSideLine =(int) (max_humidity_yvalue_y1 + max_humidity_yvalue_y2) / 2;



            //Now lets actually add to the series 
            //First to the left side
            chart1.Series["Vertical_Temp_Left_Side"].Points.AddXY((int)Tmin, min_humidity_yvalue_y1); // same as  (x1,y1) for left side
            chart1.Series["Vertical_Temp_Left_Side"].Points.AddXY((int)Tmin, midHumYvalueByDifference);//This is middle point (x_mid,y_mid)
            chart1.Series["Vertical_Temp_Left_Side"].Points.AddXY((int)Tmin, min_humidity_yvalue_y2);  // same as  (x2,y2) for left side

            //Now testing
           // if (temp == (int)Tmin)
           // {
              //  lb_test1.Text += "\n Vertical left side minT-" + Tmin + ",minHumY lower -" + min_humidity_yvalue_y1;
            //}


            //Now to the right side
            chart1.Series["Vertical_Temp_Right_Side"].Points.AddXY((int)Tmax, max_humidity_yvalue_y1);   // same as  (x1,y1) for right side
            chart1.Series["Vertical_Temp_Right_Side"].Points.AddXY((int)Tmax, midHumDiffYvalueForRightSideLine);  //This is middle
            chart1.Series["Vertical_Temp_Right_Side"].Points.AddXY((int)Tmax, max_humidity_yvalue_y2);   // same as  (x2,y2) for right side

            //--Now adding text to left and right side of line
            chart1.Series["Vertical_Temp_Left_Side"].Points[1].Label = "T= "+(int)Tmin;
            chart1.Series["Vertical_Temp_Right_Side"].Points[1].Label = "T= "+ (int)Tmax;
           
            //Refresh  the chart now
            chart1.Invalidate();
            chart1.Refresh();//This is for refresh


        } //Close of our function

        /// <summary>
        /// This variable store which series is selected for move
        /// while we are in comfortzone edit mode
        /// </summary>
        string selectedSeriesForMoveForEditComfortZone = "";


        /// <summary>
        /// This function helps to detect the border line 
        /// when comfort zone is in edit mode 
        /// so that we can create new comfort zone by modifying 
        /// it graphically
        /// </summary>
        /// <param name="e">Mouse event argument passed</param>
        private void ComfortZoneBorderLineDetectForEdit(MouseEventArgs e)
        {

            HitTestResult hit = chart1.HitTest(e.X, e.Y);
            // Text = "Element: " + hit.ChartElementType;
            DataPoint dp = null;
            if (hit.ChartElementType == ChartElementType.DataPoint)
                dp = hit.Series.Points[hit.PointIndex];

            if (dp != null)
            {
        
                        if (hit.Series.Name != null)
                        {
                        if ((string)hit.Series.Name == (string)"Vertical_Temp_Left_Side")
                         {                               
                        if (Cursor != Cursors.Cross)
                        {
                            Cursor = Cursors.SizeWE;
                            selectedSeriesForMoveForEditComfortZone = "Vertical_Temp_Left_Side";
                        }
                    } else if((string)hit.Series.Name == (string)"Vertical_Temp_Right_Side")
                    {
                        //--Vertical line on Right side is detected 
                      //  hit.Series.Color = Color.Orange;

                        if (Cursor != Cursors.Cross)
                        {
                            Cursor = Cursors.SizeWE;
                            selectedSeriesForMoveForEditComfortZone = "Vertical_Temp_Right_Side";
                        }
                        }
                        else if ((string)hit.Series.Name == (string)"Horizontal_Hum_Down_Side")
                    {
                        // -- Horizontal_Hum_Down_Side is selected 
                       // hit.Series.Color = Color.Orange;

                        if (Cursor != Cursors.Cross)
                        {
                            Cursor = Cursors.SizeNS;
                            selectedSeriesForMoveForEditComfortZone = "Horizontal_Hum_Down_Side";
                        }
                    }
                    else  if((string)hit.Series.Name == (string)"Horizontal_Hum_Upper_Side")
                    {
                        //--Horizontal_Hum_Upper_Side upper side is selected
                       // hit.Series.Color = Color.Orange;
                        if (Cursor != Cursors.Cross)
                        {
                            Cursor = Cursors.SizeNS;
                            selectedSeriesForMoveForEditComfortZone = "Horizontal_Hum_Upper_Side";
                        }
                    }
                    else
                    {
                        //--NO one is selected so back to original color 
                       // hit.Series.Color = Color.Black;
                        if (Cursor != Cursors.Cross)
                        {
                            Cursor = Cursors.Default;

                        }
                    }                
                }
                
            }//CLOSE of if dp 
          
        }

        private void realTimeMode_Click(object sender, EventArgs e)
        {
            //Weather services form
            try
            {
                Form3_WeatherService f3 = new Form3_WeatherService();
                f3.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void simulationMode_Click(object sender, EventArgs e)
        {
            Form_Psychrometric_Calculator fpc = new Form_Psychrometric_Calculator();
            fpc.ShowDialog();
        }

        private void exportDataToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            form_export_data formExportData = new form_export_data();
            formExportData.ShowDialog();

        }

        private void historyPlotToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            // this is the part of real time plotting where we try to plot the different previous values ...
            //Form5_real_time_plot f5 = new Form5_real_time_plot(this);
            //f5.Show();
            form_app_timer_historical_plot f5 = new form_app_timer_historical_plot(this);
            f5.ShowDialog();
        }

        private void trashBoxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //==This section shows the trash
            try
            {
                TrashBox tb = new TrashBox(this);
                tb.ShowDialog();

            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
       }

        private void addMixNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This oneis mixing part of the node 

            /*
            Steps : Identify the node 1 and node 2 for respective line selected
            
            */
            AddMixNodeBetweenTwoNodes();

        }

          /// <summary>
          /// Contains node1 and node2 details for mixing 
          /// </summary>
        public List<TempDataType> mixCorrespondingNodeValuesList = new List<TempDataType>();
        public void AddMixNodeBetweenTwoNodes()
        {
            //Steps: 
            /*
            1.Identify node1 and node2,
            2. CALCULATE THE mid point using section  formula
           (x1,y1)-------------------------------(x2,y2)
                  <------m-->P(x,y)<------n----->  

           P(x,y) = [(m*x2+n*x1)/(m+n),(m*y2+n*y1)/(m+n)]

            */

            //============Lets check first=======

            if(indexOfPrevPointForLineMovement == ""|| indexOfNextNodeForLineMovement == "")
            {
                return;//If index seleted is empty return
            }





            mixCorrespondingNodeValuesList.Clear();

            //--ADDING NODE1 info
            foreach(var node in menuStripNodeInfoValues)
            {
                //--Now lets find which node is being found
              if(node.id == indexOfPrevPointForLineMovement)
                {
                    //Previous node is found ie node1

                    mixCorrespondingNodeValuesList.Add(new TempDataType
                    {
                        id = node.id,
                        name= node.name,
                        xVal = node.xVal,
                        yVal = node.yVal,
                        temperature_source = node.temperature_source ,
                        humidity_source = node.humidity_source,
                        colorValue = node.colorValue ,
                        airFlow =  node.airFlow,
                        marker_Size =  node.marker_Size,
                        lastUpdatedDate =  node.lastUpdatedDate

                    });//This is the node of previous node found
                    break;
                }
              
            }

            //Adding node2 info
            foreach (var node in menuStripNodeInfoValues)
            {
                //--Now lets find which node is being found
                if (node.id == indexOfNextNodeForLineMovement)
                {
                    //Previous node is found ie node1

                    mixCorrespondingNodeValuesList.Add(new TempDataType
                    {
                        id = node.id,
                        name = node.name,
                        xVal = node.xVal,
                        yVal = node.yVal,
                        temperature_source = node.temperature_source,
                        humidity_source = node.humidity_source,
                        colorValue = node.colorValue,
                        airFlow = node.airFlow,
                        marker_Size = node.marker_Size,
                        lastUpdatedDate = node.lastUpdatedDate

                    });//This is the node of previous node found
                    break;
                }

            }//Close of for each..


            //Now let x_mix,y_mix contain the mix value of node
            double m = mixCorrespondingNodeValuesList[0].airFlow;
            double n = mixCorrespondingNodeValuesList[1].airFlow;
            double x1 = mixCorrespondingNodeValuesList[0].xVal;
            double y1 = mixCorrespondingNodeValuesList[0].yVal;

            //x2,y2;
            double x2 = mixCorrespondingNodeValuesList[1].xVal;
            double y2 = mixCorrespondingNodeValuesList[1].yVal;
                                                          


            double x_mix = ((n * x2) + (m * x1)) / (m + n);
            double y_mix = ((n * y2) + (m * y1)) / (m + n);
            //Now we know the coordinates we need to plot now..

            string idForNode = GetGUID();//This get the id
            int mixAirFlowContent = (int)(m + n);




            //Finding appropriate name
            string mixNodeName = UniqueMixName();


            //--Now insert into db first then only plot the values



            //--Now lets update the values 
            DatabaseOperations ObjDbOperation = new DatabaseOperations();
            //  MessageBox.Show("Building Name= " + selectedBuildingList[0].BuildingName);
            // MessageBox.Show(unique_id_for_node+"xval = "+ xval+" yval"+ yval+"temp "+ temperature_sourceGlobal+"hum="+ humidity_sourceGlobal+",name="+ tbName+",col="+ colorValue+",size=" +markerSize +",air="+airFlowValueGlobal.ToString());
            ObjDbOperation.InsertNodeInfoToDBWithoutDeviceInfo(CurrentSelectedBuilding, chartDetailList[indexForWhichChartIsSelected].chart_respective_nodeID, idForNode, x_mix, y_mix, "Mix","Mix", mixNodeName, Color.Blue, 20, mixAirFlowContent.ToString());

            //--Now inserting the node info
            ObjDbOperation.InsertMixNodeInfo(CurrentSelectedBuilding,chartDetailList[indexForWhichChartIsSelected].chartID, idForNode, indexOfPrevPointForLineMovement, indexOfNextNodeForLineMovement);//Insertion operation

            //Now plotting the values
                             
            //=================Now refreshing the data =======================//
            //indexOfChartSelected = e.RowIndex;    //This value is changed 
            LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

            //flagForInsertOrUpdateDataToDB = 1;
            //--This is also completed..
            ReDrawingLineAndNode();  //Done checking bbk modif: changing to alex db 

             //====================End of data refresh

        }


        int indexRun1x = 1;
        public string UniqueMixName()
        {
            indexRun1x = 1;
            int tx = menuStripNodeInfoValues.Count;
            //if (t > 0)
            //{
            //    indexRun1 = t + 1;
            //}
            //else 
            
            if (tx == 0)
            {
                indexRun1x = 1;
            }
            else
            { 
                foreach(var node in menuStripNodeInfoValues)
                {
                    if (node.temperature_source == "Mix")//either temperature or humidity source will have mix as property
                    {
                        indexRun1x++;
                    }

                }
            }

            //We also need to check if the value is present or not after setting 


            int indNodeRun1 = indexRun1x;
            string nodeName = "Mix" + indNodeRun1;//This is the node name
            for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
            {
                if (menuStripNodeInfoValues[i].name == nodeName)
                {
                    indNodeRun1++;
                    nodeName = "Mix" + indNodeRun1;
                    i = 0;
                }

            }


            return nodeName;
        }

        private void saveAsTemplateToolStripMenuItem_Click(object sender, EventArgs e)
        {

            /*
             This functions helps in saving the chart as template             
             */

            try { 

            int selectedItemIndex = dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());
            string chart_ID = chartDetailList[selectedItemIndex].chartID;


            SaveChartAsTemplate(chart_ID);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void loadTemplateToolStripMenuItem_Click(object sender, EventArgs e)
        {

            /*
             This function helps to load the template file save earlier 
             */

            try { 
            int selectedItemIndex = dataGridView1.CurrentCell.RowIndex; //int.Parse(dataGridView1.Rows[indexSelectedForDeletion].Cells[0].Value.ToString());
            string chart_ID = chartDetailList[selectedItemIndex].chartID;
            string chart_Name = chartDetailList[selectedItemIndex].chartName;


            LoadDataFromTemplateFileToDBActionTemplateFileUpload(chart_ID, chart_Name);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        //int flagDataBindingCompleteForSelection = 0;
        private void dataGridView1_DataBindingComplete(object sender, DataGridViewBindingCompleteEventArgs e)
        {
            //MessageBox.Show("ok");
            dataGridView1.ClearSelection();
            //MessageBox.Show("Databinding");
            dataGridView1.SelectionChanged -= this.dataGridView1_SelectionChanged_1;

           // flagDataBindingCompleteForSelection = 1;
        }

        private void gridViewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This grid view will call the edit node functions 
            //This section contains the edit node section
            try
            {
                EditNodeLineForm f = new EditNodeLineForm(this);
                f.ShowDialog();//This will help to wait for second dialog to be closed first.
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void excelExportToolStripMenuItem_Click(object sender, EventArgs e)
        {

            //--Lets create a function which helps to export the data 

            try { 
            ExcelExport();
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        /// <summary>
        /// Hleps in excel export of the data 
        /// along with the values present in it
        /// </summary>
    

        public void ExcelExport()
        {
            /*
            2 steps : 
            1. pull the data 
            2. convert the data 
            3. place it in excel files

            */
            try
            {

                
                String fileName = "";

                saveFD.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);// "C:";
                saveFD.FileName = "ExcelSave";
                saveFD.Title = Properties.Resources.Save_Excel_file_to;
                saveFD.Filter = "Excel file|*.xls";
                if (saveFD.ShowDialog() == DialogResult.OK)
                {

                    

                    if (menuStripNodeInfoValues.Count <=0)
                {
                    MessageBox.Show("No data present to export");
                }

                if (chartDetailList.Count <= 0)
                {
                    //Do not proceed futher 
                    MessageBox.Show("No data present to save!");
                    return;
                }

                    Cursor = Cursors.WaitCursor;
                 //excel is checked..
                 Excel.Application oApp;
                Excel.Workbook oBook;
                Excel.Worksheet oSheet;

                oApp = new Excel.Application();
                oBook = oApp.Workbooks.Add();
                oSheet = (Excel.Worksheet)oBook.Worksheets.get_Item(1);
               

                //printing the building information..
                oSheet.Cells[1, 1] = "Node Information";//WFA_psychometric_chart.Properties.Resources.Building_Information;
                oSheet.Cells[2, 1] = "Name";
                oSheet.Cells[2, 2] = "Temperature Source";
                oSheet.Cells[2, 3] = "Temperature";
                oSheet.Cells[2, 4] = "Humidity Source";
                oSheet.Cells[2, 5] = "Humidity";
                oSheet.Cells[2, 6] = "Equivalent Enthalpy";
                oSheet.Cells[2, 7] = "Color";
                oSheet.Cells[2, 8] = "Node Size";// building_info[0].zip.ToString();  //zip doesnot exist now
                oSheet.Cells[2, 9] = "Air Flow";

                int count = 3;

                for (int i = 0; i < menuStripNodeInfoValues.Count; i++)
                {



                    //for station 
                    //--This function return humDataGridValue and enthalpyDataGridValue
                    enthalpyHumidityCalculatorForXYvalue(menuStripNodeInfoValues[i].xVal, menuStripNodeInfoValues[i].yVal);

                    //double humValReturn  =  
                    //oSheet.Cells[count+i, 1] = "Node Information";//WFA_psychometric_chart.Properties.Resources.Building_Information;
                    oSheet.Cells[count + i, 1] = menuStripNodeInfoValues[i].name;//dataGridView1.Rows[i].Cells[0].Value.ToString();
                    oSheet.Cells[count + i, 2] = menuStripNodeInfoValues[i].temperature_source;//dataGridView1.Rows[i].Cells[1].Value.ToString();//"Temperature Source";
                    oSheet.Cells[count + i, 3] = menuStripNodeInfoValues[i].xVal;//dataGridView1.Rows[i].Cells[3].Value.ToString();//"Temperature";

                    oSheet.Cells[count + i, 4] = menuStripNodeInfoValues[i].humidity_source;//dataGridView1.Rows[i].Cells[4].Value.ToString();//"Humidity Source";
                    oSheet.Cells[count + i, 5] =Math.Round( humDataGridValue,2);//dataGridView1.Rows[i].Cells[5].Value.ToString(); //"Humidity";
                    oSheet.Cells[count + i, 6] = Math.Round(enthalpyDataGridView, 2); //dataGridView1.Rows[i].Cells[6].Value.ToString(); //"Equivalent Enthalpy";
                    //--For color value
                    //DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)menuStripNodeInfoValues[i].colorValue;//dataGridView1.Rows[i].Cells[7];
                    //buttonCell.FlatStyle = FlatStyle.Popup;
                    //Color c = buttonCell.Style.BackColor;//= colorDialog1.Color;//System.Drawing.Color.Red;

                    oSheet.Cells[count + i, 7] = menuStripNodeInfoValues[i].colorValue;//c.ToString();
                    oSheet.Cells[count + i, 8] = menuStripNodeInfoValues[i].marker_Size;//dataGridView1.Rows[i].Cells[8].Value.ToString(); // building_info[0].zip.ToString();  //zip doesnot exist now
                    oSheet.Cells[count + i, 9] = menuStripNodeInfoValues[i].airFlow;// dataGridView1.Rows[i].Cells[9].Value.ToString(); ;

                } //Close of for loop

                int NewLineCount = count + dataGridView1.RowCount + 2;


                //now printing the value of historical datas..
                oSheet.Cells[NewLineCount, 1] = "Line Information";

                oSheet.Cells[NewLineCount + 1, 1] = "Line Name";
                oSheet.Cells[NewLineCount + 1, 2] = "Start Node Name";
                oSheet.Cells[NewLineCount + 1, 3] = "End Node Name";
                oSheet.Cells[NewLineCount + 1, 4] = "Color";
                oSheet.Cells[NewLineCount + 1, 5] = "Thickness";
                oSheet.Cells[NewLineCount + 1, 6] = "Show Name";
                oSheet.Cells[NewLineCount + 1, 7] = "DBT1";
                oSheet.Cells[NewLineCount + 1, 8] = "RH1";
                oSheet.Cells[NewLineCount + 1, 9] = "HR1";
                oSheet.Cells[NewLineCount + 1, 10] = "SV1";

                oSheet.Cells[NewLineCount + 1, 11] = "MFR1";
                oSheet.Cells[NewLineCount + 1, 12] = "enthalpy1";
                oSheet.Cells[NewLineCount + 1, 13] = "TEF1";
                oSheet.Cells[NewLineCount + 1, 14] = "DBT2";
                oSheet.Cells[NewLineCount + 1, 15] = "RH2";
                oSheet.Cells[NewLineCount + 1, 16] = "HR2";
                oSheet.Cells[NewLineCount + 1, 17] = "SV2";
                oSheet.Cells[NewLineCount + 1, 18] = "MFR2";
                oSheet.Cells[NewLineCount + 1, 19] = "Enthalpy2";
                oSheet.Cells[NewLineCount + 1, 20] = "TEF2";
                oSheet.Cells[NewLineCount + 1, 21] = "heat change";




                //now lets print the value in loop
                for (int i1 = 0; i1 < menuStripNodeLineInfoValues.Count; i1++)
                {




                    //===========================Copied here of the code=========================//


                    string idNode1 = "", lastUpdatedDateNode1 = "", humiditySourceNode1 = "", temperatureSourceNode1 = "", nameNode1 = "";
                    string idNode2 = "", lastUpdatedDateNode2 = "", humiditySourceNode2 = "", temperatureSourceNode2 = "", nameNode2 = "";
                    double xValueNode1 = 0, yValueNode1 = 0;
                    double xValueNode2 = 0, yValueNode2 = 0;
                    int airFlowNode1 = 0, airFlowNode2 = 0;

                    //--Scanning for the values
                   
                            string startNodeName = "";
                            string endNodeName = "";
                            //Now lets calculate the startNodeName and endNodeName
                            for (int x = 0; x < menuStripNodeInfoValues.Count; x++)
                            {
                                if (menuStripNodeLineInfoValues[i1].prevNodeId == menuStripNodeInfoValues[x].id)
                                {
                                    nameNode1 = startNodeName =menuStripNodeInfoValues[x].name;
                                    idNode1 = menuStripNodeInfoValues[x].id;
                                    lastUpdatedDateNode1 = menuStripNodeInfoValues[x].lastUpdatedDate;
                                    humiditySourceNode1 = menuStripNodeInfoValues[x].humidity_source;
                                    temperatureSourceNode1 = menuStripNodeInfoValues[x].temperature_source;
                                    //nameNode1 =
                                    xValueNode1 = menuStripNodeInfoValues[x].xVal;
                                    yValueNode1 = menuStripNodeInfoValues[x].yVal;
                                    airFlowNode1 = (int)menuStripNodeInfoValues[x].airFlow;
                                    break;
                                }

                            }

                            //--This one is for end nodename
                            for (int v = 0; v <menuStripNodeInfoValues.Count; v++)
                            {
                                if (menuStripNodeLineInfoValues[i1].nextNodeId ==menuStripNodeInfoValues[v].id)
                                {
                                    nameNode2 = endNodeName = menuStripNodeInfoValues[v].name;
                                    idNode2 = menuStripNodeInfoValues[v].id;
                                    lastUpdatedDateNode2 = menuStripNodeInfoValues[v].lastUpdatedDate;
                                    humiditySourceNode2 = menuStripNodeInfoValues[v].humidity_source;
                                    temperatureSourceNode2 = menuStripNodeInfoValues[v].temperature_source;
                                    //nameNode1 =
                                    xValueNode2 = menuStripNodeInfoValues[v].xVal;
                                    yValueNode2 = menuStripNodeInfoValues[v].yVal;
                                    airFlowNode2 = (int)menuStripNodeInfoValues[v].airFlow;
                                    break;
                                }

                            }
                            
                          //--Lets make a function which returns all the other values
                          EditNodeLineForm ed_form = new EditNodeLineForm(this);
                    ed_form.EnergyParameterCalculationForTwoNodes(xValueNode1, yValueNode1, airFlowNode1, xValueNode2, yValueNode2, airFlowNode2);
                    
                    //===========================End : Copied here of the code=========================//

                    
                    oSheet.Cells[NewLineCount + 2 + i1, 1] = menuStripNodeLineInfoValues[i1].name;//dataGridView2.Rows[i].Cells[1].Value.ToString();
                    oSheet.Cells[NewLineCount + 2 + i1, 2] = //dataGridView2.Rows[i].Cells[2].Value.ToString();//"Start Node Name";

                    oSheet.Cells[NewLineCount + 2 + i1, 3] = startNodeName;//dataGridView2.Rows[i].Cells[3].Value.ToString();//"End Node Name";
                  
                    oSheet.Cells[NewLineCount + 2 + i1, 4] = menuStripNodeLineInfoValues[i1].lineColorValue;// col.ToString();
                    oSheet.Cells[NewLineCount + 2 + i1, 5] = menuStripNodeLineInfoValues[i1].lineThickness;//dataGridView2.Rows[i].Cells[5].Value.ToString(); //"Thickness";
                    //DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[i].Cells[9];
                    
                    oSheet.Cells[NewLineCount + 2 + i1, 6] = menuStripNodeLineInfoValues[i1].status;//status; //"Show Name";
                    oSheet.Cells[NewLineCount + 2 + i1, 7] = xValueNode1; //dataGridView2.Rows[i].Cells[10].Value.ToString(); //"DBT1";
                    oSheet.Cells[NewLineCount + 2 + i1, 8] = ed_form.relativeHumidity1;//dataGridView2.Rows[i].Cells[11].Value.ToString();//"RH1";
                    oSheet.Cells[NewLineCount + 2 + i1, 9] = yValueNode1;//dataGridView2.Rows[i].Cells[12].Value.ToString(); //"HR1";
                    oSheet.Cells[NewLineCount + 2 + i1, 10] = ed_form.spVol1; //dataGridView2.Rows[i].Cells[13].Value.ToString(); //"SV1";


                    oSheet.Cells[NewLineCount + 2 + i1, 11] = ed_form.massFlowRate1;//dataGridView2.Rows[i].Cells[14].Value.ToString(); //"MFR1";
                    oSheet.Cells[NewLineCount + 2 + i1, 12] = ed_form.enthalpy1;//dataGridView2.Rows[i].Cells[15].Value.ToString(); //"enthalpy1";

                    oSheet.Cells[NewLineCount + 2 + i1, 13] = ed_form.totalEnergyFlow1;// dataGridView2.Rows[i].Cells[16].Value.ToString(); //"TEF1";
                    oSheet.Cells[NewLineCount + 2 + i1, 14] = xValueNode2;//dataGridView2.Rows[i].Cells[17].Value.ToString(); //"DBT2";
                    oSheet.Cells[NewLineCount + 2 + i1, 15] = ed_form.relativeHumidity2;//dataGridView2.Rows[i].Cells[18].Value.ToString();// "RH2";
                    oSheet.Cells[NewLineCount + 2 + i1, 16] = yValueNode2;//dataGridView2.Rows[i].Cells[19].Value.ToString(); //"HR2";
                    oSheet.Cells[NewLineCount + 2 + i1, 17] = ed_form.spVol2;//dataGridView2.Rows[i].Cells[20].Value.ToString(); //"SV2";
                    oSheet.Cells[NewLineCount + 2 + i1, 18] = ed_form.massFlowRate2;//  dataGridView2.Rows[i].Cells[21].Value.ToString(); //"MFR2";
                    oSheet.Cells[NewLineCount + 2 + i1, 19] = ed_form.enthalpy2; //dataGridView2.Rows[i].Cells[22].Value.ToString(); //"Enthalpy2";
                    oSheet.Cells[NewLineCount + 2 + i1, 20] = ed_form.totalEnergyFlow2;// dataGridView2.Rows[i].Cells[23].Value.ToString();// "TEF2";
                    oSheet.Cells[NewLineCount + 2 + i1, 21] = ed_form.heatChangeForBoth;//dataGridView2.Rows[i].Cells[24].Value.ToString(); //"heat change";


                }

                //now lets open the save dialog box and the save it there..

                Cursor = Cursors.Default;
                 //save the file..
                    fileName = saveFD.FileName;
                    oBook.SaveAs(fileName);


                    oBook.Close();
                    oApp.Quit();
                }
                
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }












        //=========================end of the deleting of node,line and ....===================================================//

        //ArrayList temp_building_values = new ArrayList();
        List<DataTypeTempBuildingValue> temp_building_values = new List<DataTypeTempBuildingValue>();
                  
        private void PullLocationInformation()
        {
            try
            {
                //cb1_select_data.Items.Clear();
                ArrayList stored_location = new ArrayList();
                temp_building_values.Clear();//we need to clear the values for new items
               
                
                //while loading it should populate the field...
                  //lets pull the vales offline values stored in db...
                //string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

                //string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";


                // MessageBox.Show("connection string = " + connString);


                using (SQLiteConnection connection = new SQLiteConnection(connString)) { 
                    connection.Open();
                SQLiteDataReader reader = null;
                SQLiteCommand comm = new SQLiteCommand("SELECT * from tbl_building_location where selection = 1", connection);
                //command.Parameters.AddWithValue("@1", userName)
                reader = comm.ExecuteReader();
                while (reader.Read())
                {
                   
                    temp_building_values.Add(new DataTypeTempBuildingValue
                    {
                        ID = int.Parse(reader["id"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString()
                    });

                }

                //--This is index_selected is later used for inserting lat , long value to db if not filled already
                index_selected = temp_building_values[0].ID;
                // MessageBox.Show("stored place = " + s);
                comm.Dispose();
                reader.Dispose();
                connection.Close();
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        private void button2_Click_1(object sender, EventArgs e)
        {

            ////--this is heat map plot...
            ////initially resetting values to empty

            ////resetting ends...
            //DateTime fromDate = dtp_From.Value;
            //DateTime toDate = dtp_To.Value;


            ////--Calling part here.....
            //heat_map_button_click(index_selected, fromDate, toDate);




        }

        private void helpPsychometricChartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try { 
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\PsychometricHelp.chm";
            Help.ShowHelp(this, file);
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        int index_series_heat_map = 0;//this index is used by  plot_on_graph_values method

        //int SeriesCount = 0;
        public void heat_map_button_click(int index_selected_heat_map,DateTime from1,DateTime to1)
        {

            //-- resetting chart series and plotting again..

             //plot_new_graph();

          //Series  series1_heat_map = new Series("My_Series_heat_map");//changed form "My Series"
            //chart1.Series.Add(series1_heat_map);


            if (chart1.Series.IndexOf(series1_heat_map)!= -1) 
            {
                //chart1.Series.Remove(series1_heat_map);//--Removing the series that already exist...
                //chart1.Series.Add(series1_heat_map);

                //    series1_heat_map.Points.Clear();//--This is for resetting the values ...
                //    index_series_heat_map = 0;//--Resetting the index...
                //series1.Points.Clear();
                //chart1.Series.RemoveAt(chart1.Series.IndexOf(series1));
                series1_heat_map.Points.Clear();//--We can clear all the poingt over here and add new one later..
                index_series_heat_map = 0;//--Resetting the values of the series...

            }
            //SeriesCount = 1;

            //--lest reset soem values..
            hist_temp_hum_list.Clear();

            //this  is going to plot the heat map...
            /*Steps:
            1.Get the database values..
            2.filter those values ..
            3.plot those values in the map..       
            */
            DateTime from = from1;//dtp_From.Value;
            DateTime to = to1;/// dtp_To.Value;

            //2.database connection ..

            if (to > from)
            {

                //   string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //  string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                //sql connection string is this..
                //     string connString1 = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString1 = @"Data Source=" + databaseFile + ";Version=3;";



                using (SQLiteConnection connection1 = new SQLiteConnection(connString1))
                {
                    connection1.Open();



                    //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                    //lets pass this string to a query which does the pulling part.
                    SQLiteDataReader reader1 = null;
                    SQLiteCommand command1 = new SQLiteCommand("Select * from tbl_historical_data WHERE date_current BETWEEN @date_first AND @date_second AND ID=@id_value", connection1);
                    command1.Parameters.AddWithValue("@date_first", from);
                    command1.Parameters.AddWithValue("@date_second", to);
                    command1.Parameters.AddWithValue("@id_value", index_selected_heat_map);//--This index selected is required to see which location is seleccted
                    //command1.Parameters.AddWithValue("@station_name", station_name);
                    reader1 = command1.ExecuteReader();
                    while (reader1.Read())
                    {                                                       
                        hist_temp_hum_list.Add(
                            new data_type_hum_temp
                            {
                                temp = double.Parse(reader1["temperature"].ToString()),
                                hum = double.Parse(reader1["humidity"].ToString())

                            });
                    }//close of while loop       
                     // connection1.Close();
                }//close of database using statement 
            }//closing of if statement
            else
            {
                MessageBox.Show(Properties.Resources.Please_select_correct_date_for);
            }


            //this will only be done when the data is returned

            if (hist_temp_hum_list.Count > 0)
            {
                //MessageBox.Show("value counted " + hist_temp_hum_list.Count);
                //after we have the data we do the actual part of heat map plotting...
                //setting up maximum and minimum value to use in color value calculation..

                ArrayList temporary_val_temp = new ArrayList();
                max_value = hist_temp_hum_list[0].temp;
                min_value = hist_temp_hum_list[0].temp;
                for (int i = 1; i < hist_temp_hum_list.Count; i++)//this is done because we are counting from 1 index no error 
                {                                                  //as we are comparing the first index value with all the vlues in the index  

                    if (max_value < hist_temp_hum_list[i].temp)
                    {
                        max_value = hist_temp_hum_list[i].temp;
                    }
                    if (min_value > hist_temp_hum_list[i].temp)
                    {
                        min_value = hist_temp_hum_list[i].temp;
                    }

                }



                //min_value = hist_temp_hum_list.Min<data_type_hum_temp>().temp;

                //MessageBox.Show("max = " + max_value + " ,min = " + min_value);//--printng of min value
                //callin gthe method.
                //lets increase th performance first... this below code if from plot_by_dbt_hr

                //--Lets reset the values first and then re-add the values
                //temperature_value.Clear();
                //pg_value_from_txtfile.Clear();//--Clearing the part of the series



                //string line1;

                //string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                //string file = dir + @"\t_pg.txt";
                //string path1 = file;

                //using (StreamReader st = new StreamReader(path1))
                //{

                //    while ((line1 = st.ReadLine()) != null)
                //    {

                //        string[] value = line1.Split(',');
                //        try
                //        {
                //            double temp1 = Double.Parse(value[0]);
                //            double temp2 = Double.Parse(value[1]);
                //            //now lets add to temperature and pg array..                     
                //            temperature_value.Add(temp1);
                //            pg_value_from_txtfile.Add(temp2);


                //        }
                //        catch (Exception ex)
                //        {
                //            MessageBox.Show(ex.ToString());
                //        }


                //    }//close of while

                //}//close of using

                temperature_value = t;
                pg_value_from_txtfile = pg;



                //this series is used to add to the 
            //    chart1.Series.Add(series1_heat_map);
              


                for (int i = 0; i < hist_temp_hum_list.Count; i++)
                {
                    plot_by_DBT_HR_heat_map(hist_temp_hum_list[i].temp, hist_temp_hum_list[i].hum / 100);

                }

               // MessageBox.Show(Properties.Resources.Success_final);
            }//close of if
            else
            {
                MessageBox.Show(Properties.Resources.No_data_found_in_database);
            }

            if (max_value != min_value)
            {
                marker();
            }
            else
            {
                //make indicator for same color
                marker_for_same_min_max_value();
            }





        }

        private void marker_for_same_min_max_value()
        {
            /*
            Our previous marker formula has a draw back if max_value = min_value the difference 
            between max_value- min_value=0 so which present problem 
            this is solved by this marker assumin in such case the plot is of same color we do this part.
            */

            
            try
            {
                using (Graphics grp1 = this.CreateGraphics())
                {

                    int x1Axis = (int)(chart1.ChartAreas[0].Position.X + chart1.Width + 10);
                    int y1Axis = (int)(chart1.ChartAreas[0].Position.Y + chart1.Height);

                    int x2Axis = x1Axis + 15;
                    int y2Axis = y1Axis;

                    double start = min_value;

                    double value = start;
                    // double temp_value = (max_value - min_value);
                    //double increment = 0;
                    //increment = temp_value / 50;


                    //decimal val = (Decimal)((value - min_value) / (max_value - min_value));

                    Pen pen1 = new Pen(Color.FromArgb(0, 255, 0));
                    //grp1.DrawRectangle(pen1, 958, 537, 15, 15);
                    grp1.DrawRectangle(pen1, x1Axis, y1Axis, 15, 15);

                    SolidBrush drawBrushGreen = new SolidBrush(Color.FromArgb(0, 255, 0));
                    //grp1.FillRectangle(drawBrushGreen, 958, 537, 15, 15);
                    grp1.FillRectangle(drawBrushGreen, x1Axis, y1Axis, 15, 15);

                    String drawString = Math.Round(value,0).ToString();
                    // Create font and brush.
                    Font drawFont = new Font("Arial", 7);
                    SolidBrush drawBrush = new SolidBrush(Color.Black);
                    // Create point for upper-left corner of drawing.
                    PointF drawPoint = new PointF(x1Axis-12, y1Axis);//--537->520
                    // Draw string to screen.
                    grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void marker()
        {
            try
            {
                using (Graphics grp1 = this.CreateGraphics())
                {
                    double start = min_value;

                    double value = start;
                    double temp_value = (max_value - min_value);
                    double increment = 0;
                    increment = temp_value / 50;
                    int  x1Axis = (int)(chart1.Location.X + chart1.Width + 30);
                    int y1Axis = (int)(chart1.Location.Y + chart1.Height);

                    int x2Axis = x1Axis + 15;
                    int y2Axis = y1Axis;


                   // MessageBox.Show("X1 = " + x1Axis + ", y1   =" + y1Axis + ", x2 = " + x2Axis + " , y2 = " + y2Axis);

                    for (int i = 1; i <= 50; i++)
                    {

                        //decimal val = (Decimal)((value - min_value) / (max_value - min_value));
                        double val = (double)((value - min_value) / (max_value - min_value));
                        int r = Convert.ToByte(255 * val);
                        int g = Convert.ToByte(255 * (1 - val));
                        int b = 0;
                        Pen pen1 = new Pen(Color.FromArgb(r, g, b));
                        //grp1.DrawLine(pen1, 958, 520 - i, 973, 520 - i);//--changed
                        grp1.DrawLine(pen1, x1Axis, y1Axis - i, x2Axis, y2Axis - i);//--changed
                        if (i == 0)
                        {
                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            //PointF drawPoint = new PointF(958-12, 520 - i); //--change
                            PointF drawPoint = new PointF(x1Axis - 12, y1Axis - i); //--change
                            // Draw string to screen.
                            grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                        }
                        else if (i == 13)
                        {
                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            //PointF drawPoint = new PointF(958-12, 520 - i);
                            PointF drawPoint = new PointF(x1Axis - 12, y1Axis - i); //--change
                            // Draw string to screen.
                            grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                        }
                        else if (i == 25)
                        {

                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            //PointF drawPoint = new PointF(958-12, 520 - i);
                            PointF drawPoint = new PointF(x1Axis - 12, y1Axis - i); //--change
                            // Draw string to screen.
                            grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                        }
                        else if (i == 35)
                        {

                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            //PointF drawPoint = new PointF(958-12, 520 - i);
                            PointF drawPoint = new PointF(x1Axis - 12, y1Axis - i); //--change
                            // Draw string to screen.
                            grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                        }
                        else if (i == 50)
                        {

                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            //PointF drawPoint = new PointF(958-12, 520 - i);
                            PointF drawPoint = new PointF(x1Axis - 12, y1Axis - i); //--change
                            // Draw string to screen.
                            grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                        }

                        value += increment;
                    }//close of for...

                }//close of using statement..
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        int index_heat_map = 0;
        public int plot_by_DBT_HR_heat_map(double DBT, double HR)
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


            double patm = 101.235;//constant..we will make it take as input later...
            //double rair = 0.287;//rideburg constant i guess
            double wg_calc = 0;
            double pg_value = 0.000000;
            //now for corresponding DBT lets calculate constant value pg..
            try
            {
                for (int i = 0; i < temperature_value.Count; i++)
                {
                    ///x-axis contains the DBT
                    if ((int)DBT == (int)Double.Parse(temperature_value[i].ToString()))
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
            double y_axis = wg_calc;

            //MessageBox.Show("Yaxis value  = " + y_axis);//--check

            plot_on_graph_values_heat_map(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            //index_heat_map++;


            return 0;
        }

        public void plot_on_graph_values_heat_map(double dbt, double hr, double xval, double yval)
        {
            //chart1.Series.Clear();
            //Series series1 = new Series("My Series" + index);
            //chart1.Series.Add(series1);
            try
            {


                series1_heat_map.ChartType = SeriesChartType.Point;
                int r, g, b;

                if (max_value != min_value)
                {

                    double value = dbt;
                    //decimal val = (Decimal)((value - min_value) / (max_value - min_value));
                    double val = (double)((value - min_value) / (max_value - min_value));
                    r = Convert.ToByte(255 * val);
                    g = Convert.ToByte(255 * (1 - val));
                    b = 0;

                    //MessageBox.Show("dbt =" + dbt + "\n xval =" + xval + "\n yval = " + yval+"\n rgb = "+r+","+g+",0");

                }
                else
                {
                    //make all the colors same value..
                    r = 0;
                    g = 255;
                    b = 0;
                }

                series1_heat_map.MarkerSize = 15;
                //string label = "DBT=" + dbt + ",HR=" + hr;
                //series1.Label = label;
                //chart1.Series["SeriesDBT_HR" + index].;
                series1_heat_map.Points.AddXY(xval, yval);
                series1_heat_map.Points[index_series_heat_map++].Color = Color.FromArgb(255, r, g, b);//blue
                                                                                    //series1.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public void printHeatMap()
        {

            try
            {
                //this when click prints the chart.
                // Chart chart1 = form1.chart1;
                System.Drawing.Printing.PrintDocument pd = new System.Drawing.Printing.PrintDocument();
                chart1.Printing.PrintPaint(chart1.CreateGraphics(), chart1.DisplayRectangle);
                PrintDialog pdi = new PrintDialog();
                pdi.Document = pd;
                if (pdi.ShowDialog() == DialogResult.OK)
                    pdi.Document.Print();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
                 
        }
        public void saveAsImageHeatMap()
        {
            string fileName = "";
            saveFD.InitialDirectory = "C:";
            saveFD.FileName = "ChartImage";
            saveFD.Filter = "PNG(.png) |*.png|Bitmap(.bmp) |*.bmp|JPEG |*.jpeg";
            ImageFormat format = ImageFormat.Png;
            if (saveFD.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFD.FileName;
                string ext = System.IO.Path.GetExtension(saveFD.FileName);
                switch (ext)
                {
                    case ".bmp":
                        format = ImageFormat.Bmp;
                        break;
                    case ".jpeg":
                        format = ImageFormat.Jpeg;
                        break;
                }

               chart1.SaveImage(fileName, format);
            }
            //else
            //{
            //    fileName = "ChartImage.png";
            //    MessageBox.Show(Properties.Resources._Your_chart_image_will_be_save + fileName);
            //}


        }

        //======================This code is for not selecting first element when binding data============================================//

       
        //=================End of binding data =============================================================//

    }//close of btn4


    //==========================class==========


    public class DataGridView_AutoSelectSuppressed : DataGridView
    {
        private bool SuppressAutoSelection { get; set; }

        public DataGridView_AutoSelectSuppressed() : base()
        {
            SuppressAutoSelection = true;
        }

        public new /*shadowing*/ object DataSource
        {
            get
            {
                return base.DataSource;
            }
            set
            {
                SuppressAutoSelection = true;
                Form parent = this.FindForm();

                // Either the selection gets cleared on form load....
                parent.Load -= parent_Load;
                parent.Load += parent_Load;

                base.DataSource = value;

                // ...or it gets cleared straight after the DataSource is set
                ClearSelectionAndResetSuppression();
            }
        }

        protected override void OnSelectionChanged(EventArgs e)
        {
            if (SuppressAutoSelection)
                return;

            base.OnSelectionChanged(e);
        }

        public void ClearSelectionAndResetSuppression()
        {
            if (this.SelectedRows.Count > 0 || this.SelectedCells.Count > 0)
            {
                this.ClearSelection();
                SuppressAutoSelection = false;
            }
        }

        private void parent_Load(object sender, EventArgs e)
        {
            ClearSelectionAndResetSuppression();
        }
    }

    //=====================================end ==============================================//
}
