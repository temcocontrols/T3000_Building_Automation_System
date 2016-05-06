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

namespace WFA_psychometric_chart
{
    public partial class Form1_main : Form
    {
        //OleDbCommand cmd = new OleDbCommand();
        //OleDbConnection con = new OleDbConnection();
        SqlConnection con = new SqlConnection();
        SqlCommand cmd = new SqlCommand();
        public Form1_main()
        {
            InitializeComponent();
          //this.Disposed += new System.EventHandler ( this.Form1_main_Disposed );
        }
        //lets define the constanst..
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
        ArrayList hum2_AL =  new ArrayList();
        Series series1xx = new Series("My Series values plot ");//this series is used by plot_on_graph_values() method...
        Series seriesLineIndicator = new Series("LineIndicator");//--This line indicator is temporary show the line in the chart for Node Movement.
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
            ArrayList t = new ArrayList();//this stores the temperature(deg.cel)
            ArrayList pg = new ArrayList();//this stores the saturated vapour pressure(kpa).

            //now lets read from the text file...
            string line1;
            //String[][] point_value= new String[][];
            // string path1 = "C:\\Users\\nischal\\Desktop\\t_pg.txt";
            /*
            var pat_test = System.Environment.
                             GetFolderPath(
                                 Environment.SpecialFolder.Desktop
                             );
           // MessageBox.Show("the path = " + pat_test);
            string path1 = System.IO.Path.Combine(pat_test, "t_pg.txt");
            */
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;

            // string path;
            // path = System.IO.Path.GetDirectoryName(
            //    System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);

            // MessageBox.Show(path);
            //string  path1 = path + "\\t_pg.txt";
            //MessageBox.Show(path1);
            using (StreamReader st = new StreamReader(path1))
            {
                //int i = 0;
                //string s = " ";
                while ((line1 = st.ReadLine()) != null)
                {
                    //line.Skip(1);


                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0]);
                        double temp2 = Double.Parse(value[1].Trim());
                        //now lets add to temperature and pg array..
                        t.Add(temp1);
                        pg.Add(temp2);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

            //now lets test the value..
            string s = "";
            for (int i = 0; i < t.Count; i++)
            {
                s += t[i].ToString() + "," + pg[i].ToString() + "\n";
            }

            // MessageBox.Show(""+s);

            //now we have the data lets do some ploting...
            double patm = 101.235;//constant..we will make it take as input later...
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
        
            chart1.Series["Series1"].Points[16].Label = "Wet bulb temp";
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
                chart1.Series["Series" + c].Points[45].Label = phi * 100 + "%";
                //chart1.Series["Series"+c].Points[46].LabelBackColor = Color.Blue;

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
            //String[][] point_value= new String[][];
            //string path2 = "C:\\Users\\nischal\\Desktop\\t_pg1.txt";
            /*
            var pat_test2 = System.Environment.
                            GetFolderPath(
                                Environment.SpecialFolder.Desktop
                            );
            //MessageBox.Show("the path = " + pat_test);
            string path2 = System.IO.Path.Combine(pat_test, "t_pg1.txt");
            */

            string file1 = dir + @"\t_pg1.txt";
            string path2 = file1;
            using (System.IO.StreamReader st = new System.IO.StreamReader(path2))
            {
                //int i = 0;
                //string s = " ";
                while ((line2 = st.ReadLine()) != null)
                {
                    //line.Skip(1);


                    string[] value = line2.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0].ToString());
                        double temp2 = Double.Parse(value[1].ToString().Trim());
                        //now lets add to temperature and pg array..
                        t1.Add(temp1);
                        pg1.Add(temp2);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

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




        }


        private void button1_Click(object sender, EventArgs e)
        {
            plot_new_graph();

        }

        
        private void Form1_Load(object sender, EventArgs e)
        {
             //lets plot the graph as soon as the form loads.
            plot_new_graph();

            button1.Text = "Refresh Graph";
            //this is for adding values dynamically as the program loads. used by plot_on_graph_values() method 
            chart1.Series.Add(series1xx);
            //--This is added for the process diagram part...
            chart1.Series.Add(series1);
            chart1.Series.Add(seriesLineIndicator);//--This line indicator is for show temporary line for movement...
            //this is other part.
            //radioButton1.Checked = true;

            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //string file = dir + @"\TestDir\TestFile.txt";
            //con.ConnectionString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
            //change here..

            //con.ConnectionString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+dir+@"\T3000.mdb;Persist Security Info=True";
            //this is sql connnection string..
            //con.ConnectionString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";
            //cmd.Connection = con;

            string databasePath1 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile1 = databasePath1 + @"\db_psychrometric_project.s3db";
            if (File.Exists(databaseFile1))
            {
                //file exist so dont create the database
            }

            else { 
            //--sqlite new databse creation
            sqlite_database_creation();
            }
        }


        private void sqlite_database_creation()
        {

            //--lets do try catch
            try
            {
                //--This is where we are going to create all the database  and tables of sqlite
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                //--new database file 
                SQLiteConnection.CreateFile(databaseFile);

                //--now lets create the tables
                SQLiteConnection m_dbConnection = new SQLiteConnection("Data Source=" + databaseFile + ";Version=3;");
                m_dbConnection.Open();

                //--building location table : tbl_building_location
                string sql = "create table tbl_building_location (ID INTEGER PRIMARY KEY AUTOINCREMENT ,country varchar(255),state varchar(255),city varchar(255),street varchar(255), ZIP int,longitude varchar(255),latitude varchar(255),elevation varchar(255))";
                SQLiteCommand command = new SQLiteCommand(sql, m_dbConnection);
                command.ExecuteNonQuery();
                //--next table geo location value : tbl_geo_location_value
                string sql1 = "create table tbl_geo_location_value (ID int ,longitude varchar(255),latitude varchar(255),elevation varchar(255))";
                SQLiteCommand command1 = new SQLiteCommand(sql1, m_dbConnection);
                command1.ExecuteNonQuery();

                //--next table historical data:tbl_historical_data
                string sql2 = "create table tbl_historical_data (ID int ,date_current datetime,hour_current int,minute_current int,temperature varchar(255),humidity varchar(255),station_name varchar(255))";
                SQLiteCommand command2 = new SQLiteCommand(sql2, m_dbConnection);
                command2.ExecuteNonQuery();
                //--next table tbl_temp_himidity 
                string sql3 = "create table tbl_temp_humidity (temp int,humidity int)";
                SQLiteCommand command3 = new SQLiteCommand(sql3, m_dbConnection);
                command3.ExecuteNonQuery();
                //--next table weather related datas...
                string sql4 = "create table tbl_weather_related_values (ID INTEGER PRIMARY KEY AUTOINCREMENT ,location varchar(255),distance_from_building varchar(255),last_update_date varchar(255),temp varchar(255),humidity varchar(255),bar_pressure varchar(255),wind varchar(255),direction varchar(255),station_name varchar(255))";
                SQLiteCommand command4 = new SQLiteCommand(sql4, m_dbConnection);
                command4.ExecuteNonQuery();

                m_dbConnection.Close();//--closing the connection


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }



        }


        //private void radioButton1_CheckedChanged(object sender, EventArgs e)
        //{
        //    if (radioButton1.Checked == true)
        //    {
        //        //MessageBox.Show("You have pressed 1");
        //        // if buttton one is clicked the units has to change...
        //    }
        //}

        private void button2_Click(object sender, EventArgs e)
        {

            //--this is commented no more use we made Psychrometric Calculator
            ////this is where the actual calculation begins ..
            ////lets defin the constants..

            //try
            //{




            //    //lets grab from the textbox input.
            //    temperature = Double.Parse(textBox1.Text.Trim());
            //    humidity = Double.Parse(textBox2.Text.Trim());



            //    Patm = 1013;
            //    A = 6.116441;
            //    m = 7.591386;
            //    Tn = 240.7263;
            //    B = 621.9907;

            //    //lets set the values..
            //    textBox3.Text = Patm.ToString();
            //    textBox5.Text = A.ToString();
            //    textBox6.Text = m.ToString();
            //    textBox7.Text = Tn.ToString();
            //    textBox9.Text = B.ToString();

            //    //calculating 1.dew point..
            //    TDewpoint = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature) / (243.04 + temperature))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature) / (243.04 + temperature)));
            //    //setting to textbox4 its a dew point.
            //    textBox4.Text = Convert.ToString(TDewpoint);
            //    //2.Partial pressure of saturation..
            //    Pws = A * Math.Pow(10, (m * TDewpoint) / (TDewpoint + Tn));
            //    textBox8.Text = Convert.ToString(Pws);

            //    X = B * Pws / (Patm - Pws);
            //    textBox10.Text = Convert.ToString(X);

            //    h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
            //    textBox11.Text = Convert.ToString(h);


            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //    textBox1.Text = "";
            //    textBox2.Text = "";
            //    //lets set the values..
            //    textBox3.Text = "";
            //    textBox5.Text = "";
            //    textBox6.Text = "";
            //    textBox7.Text = "";
            //    textBox9.Text = "";


            //}

            ////for the EU unit just check if EU unit was selected of not if selected then convert
            ////this values here...


        }


        //public class pg_value_to_list
        //{
        //    float temperature_value { get; set; }
        //    float pg_value { get; set; } 
        //}


        /*This is the function that plots the graph 
         */


        public void plot_on_graph_values(double dbt,double hr,double xval,double yval)
        {
            

            series1xx.ChartType = SeriesChartType.Point;
            series1xx.Color = Color.FromArgb(0, 0, 255);//blue
            series1xx.MarkerStyle = MarkerStyle.Circle;
         
            series1xx.MarkerSize = 12;
            
            string label = "DBT=" + dbt + ",HR=" + hr;
            series1xx.Label = label;
            //chart1.Series["SeriesDBT_HR" + index].;
            series1xx.Points.AddXY(xval, yval);
            
            //series1.Enabled = true;

        }

        public int plot_by_DBT_HR(double DBT, double HR)
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
            
            
            string line1;
            //string path1 = "C:\\Users\\nischal\\Desktop\\t_pg.txt";
           /* var pat_test = System.Environment.
                            GetFolderPath(
                                Environment.SpecialFolder.Desktop
                            );
           // MessageBox.Show("the path = " + pat_test);
            string path1 = System.IO.Path.Combine(pat_test, "t_pg.txt");
            */
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;
          
            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {
        
                while ((line1 = st.ReadLine()) != null)
                {
                    
                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0]);
                        double temp2 = Double.Parse(value[1]);
                        //now lets add to temperature and pg array..                     
                        temperature_value.Add(temp1);
                        pg_value_from_txtfile.Add(temp2);

                      
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using
           
            //lets proceed futher..
            //double pg_value = Double.Parse(pg[i].ToString());
            //wg_calc = 622 * pg_value / (patm - pg_value);
            //x = double.Parse(t[i].ToString());
            //double y = wg_calc;
            //chart1.Series["Series1"].Points.AddXY(x, y);

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

       
        //this was for ploting dbt and enthalpy which we dont require now...
        public int plot_by_DBT_Enthalpy(double dbt, double enthalpy)
        {
            //this is DBT 
            double x_axis = dbt;
            double h = enthalpy;
            //          MessageBox.Show("h = " + h+" T = "+dbt);

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

            //   MessageBox.Show("y = " + y);



            chart1.Series.Add("SeriesDBT_enthalpy" + index);//this is already delceared in chart control so we dont need it
            chart1.Series["SeriesDBT_enthalpy" + index].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
            chart1.Series["SeriesDBT_enthalpy" + index].Color = Color.Blue;
            chart1.Series["SeriesDBT_enthalpy" + index].MarkerSize = 15;
            chart1.Series["SeriesDBT_enthalpy" + index].Label = "DBT = " + dbt + "degC ,enthalpy = " + enthalpy;
            //chart1.Series["SeriesDBT_HR" + index].;
            chart1.Series["SeriesDBT_enthalpy" + index].Points.AddXY(x_axis, y_axis);

            index++;



            return 0;
        }

      
        public int plot_by_enthalpy_dew_point(double enthalpy,double dew_point)
        {

           /* double y_axis = 0;
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();


            string line1;
            string path1 = "C:\\Users\\nischal\\Desktop\\t_pg.txt";
            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {

                while ((line1 = st.ReadLine()) != null)
                {

                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0]);
                        double temp2 = Double.Parse(value[1]);
                        //now lets add to temperature and pg array..                     
                        temperature_value.Add(temp1);
                        pg_value_from_txtfile.Add(temp2);


                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using
           

            //now lets proceed forwared..
            double x1 = dew_point;
            //   MessageBox.Show("X1= " + x1);
            double pg = 0;
            for (int i = 0; i < temperature_value.Count; i++)
            {
                if (dew_point ==double.Parse(temperature_value[i].ToString()))
                {

                    pg =double.Parse(pg_value_from_txtfile[i].ToString());
                    break;
                }

            }
            
            double y1 = pg;//given pressure value form table
            //MessageBox.Show("y1= " + y1);
            double x2 = enthalpy;
            double y2 = pg;

            //    MessageBox.Show("x2= " + x2+" y2  = "+y2);

           // double x = enthalpy;
            //double y = y1 + (((y2 - y1) * (x - x1) / (x2 - x1)));
            double y = pg;
            double x = ((y - y1) * (x2 - x1) / (y2 - y1)) + x1;
            double x_axis = x;
            y_axis = pg;
            */


            //since we know dew point and enthalpy so first 
            /*
             from dew point calc . Pws and then put that into the value of mixing ratio..
             */
/*
            Patm = 1013;
            A = 6.116441;
            m = 7.591386;
            Tn = 240.7263;
            B = 621.9907;

            double td = dew_point;
            double h = enthalpy;
            double const_pws = ((m * td )/ (td + Tn));
            MessageBox.Show("td= " + td + " h = " + h +" const_pws = "+const_pws);
            double Pws = A * Math.Pow(10.000000, const_pws);//pressure of water at saturation
            double x = B * Pws / (Patm - Pws);//this is mixing ratio..
            MessageBox.Show("PWs= " + Pws + " x = " + x);
            double temp = (h - (2.5 * x) )/ (1.01 + (0.00189 * x));

            MessageBox.Show("temp = " + temp);

            //now x axis will be temp and y axis will be dew point..

            double x_axis = temp;
            double y_axis = td;

            chart1.Series.Add("Series_DP_enthalpy" + index);
            chart1.Series["Series_DP_enthalpy" + index].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
            chart1.Series["Series_DP_enthalpy" + index].Color = Color.Blue;
            chart1.Series["Series_DP_enthalpy" + index].MarkerSize = 5;
            chart1.Series["Series_DP_enthalpy" + index].Label = "enthalpy =" + enthalpy + "dew point =" + dew_point;
            //chart1.Series["SeriesDBT_HR" + index].;
            chart1.Series["Series_DP_enthalpy" + index].Points.AddXY(x_axis, y_axis);

            index++;


            */
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
        public int plot_by_WBT_humidity(double WBT,double humidity){




            return 0;
        }

        public class get_temp_hum
        {
            public string temp { get; set; }
            public string humidity { get; set; }
        }
       
        Point? prevPosition = null;

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

                 //   tooltip.Show("X=" + xVal + ", Y=" + yVal, this.chart1,
                   //              pos.X, pos.Y - 15);

                    //now printing the x and y coordinate to label..
                  //  lb_x_cord.Text = Math.Round(xVal,4).ToString();
                   // lb_y_cord.Text = Math.Round(yVal,4).ToString();


                    //now lets move on to making other part 
                    /*1.find dbt value => this is x axis value 
                     * 2.find sp.ratio value => this is yaxis value
                     */
                    lb_dbt.Text =Math.Round(xVal,4).ToString();
                    lb_humidity_ratio.Text = Math.Round(yVal,4).ToString();


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

                    /*
                    string line1;
                  //  string path1 = "C:\\Users\\nischal\\Desktop\\t_pg.txt";
                    var pat_test = System.Environment.
                            GetFolderPath(
                                Environment.SpecialFolder.Desktop
                            );
                   // MessageBox.Show("the path = " + pat_test);
                    string path1 = System.IO.Path.Combine(pat_test, "t_pg.txt");
          */
                    string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = dir + @"\t_pg.txt";
                    string path1 = file;
                    string line1;

                    using (StreamReader st = new StreamReader(path1))
                    {

                        while ((line1 = st.ReadLine()) != null)
                        {

                            string[] value = line1.Split(',');
                            try
                            {
                                double temp1 = Double.Parse(value[0]);
                                double temp2 = Double.Parse(value[1]);
                                //now lets add to temperature and pg array..                     
                                temperature_value.Add(temp1);
                                pg_value_from_txtfile.Add(temp2);


                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.ToString());
                            }


                        }//close of while

                    }//close of using

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

                   double patm =101.325;//this is constant...
                   // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                    //double w1 = 622*phi*pg/(patm-phi*pg);
                   double w = yVal;
                   phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                   phi = phi * 100;//changing into percent..
                    //now display in label...
                    lb_RH.Text = Math.Round(phi,4).ToString();

                    //now lets calculate the dew point...
                    double humidity = phi;
                    double temperature1 = xVal;
                   double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
                    //now lets print this value..
                   lb_DP.Text = Math.Round(TD,4).ToString();


                    //now lets move towards enthalpy...

                   Patm = 1013;
                   A = 6.116441;
                   m = 7.591386;
                   Tn = 240.7263;
                   B = 621.9907;

                   double Pws = A * Math.Pow(10, (m * TD) / (TD + Tn));

                 double  X = B * Pws / (Patm - Pws);

                   h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
                    //now lets display this value ..
                   lb_enthalpy.Text = Math.Round(h,4).ToString();

                }
            }



            //--Lets add a function for the process diagram drawing..

            ProcessDiagramMouseMoveFunction(e);//--This does the adding and removing part

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
            //can be compared.
            //if( ((chart1.ChartAreas[0].Position.X + chart1.ChartAreas[0])>e.X && chart1.ChartAreas[0].Position.X < e.X  ) && ((chart1.ChartAreas[0].Position.Y + ) > e.X && chart1.ChartAreas[0].Position.Y < e.Y)  ){           
            //if((chart1.ChartAreas[0].Position.X<e.X && chart1.ChartAreas[0].Position.Y<e.Y)&& ((chart1.ChartAreas[0].Position.Width + chart1.ChartAreas[0].Position.X) > e.X && (chart1.ChartAreas[0].Position.Height + chart1.ChartAreas[0].Position.Y) > e.Y))
            //label1.Text = e.X.ToString();
            //label2.Text = e.Y.ToString();
            if ((e.X > chart1.ChartAreas[0].Position.X && e.Y > chart1.ChartAreas[0].Position.Y) && (e.X < chart1.Width && e.Y <chart1.Height))
            {

                try
                {

                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
                   // label1.Text = xValue.ToString();
                    //label2.Text = yValue.ToString();

                    xAxis1 = xValue;
                    yAxis1 = yValue;
                    //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                    if (menuStripAllValues.Count > 0)
                    {
                        //foreach(var values in menuStripAllValues)

                        for (int i = 0; i < menuStripAllValues.Count; i++)
                        {

                            if ((xValue > menuStripAllValues[i].xVal - 0.25 && xValue < menuStripAllValues[i].xVal + 0.25) && (yValue > menuStripAllValues[i].yVal - 0.25 && yValue < menuStripAllValues[i].yVal + 0.25))
                            {

                                idSelected = menuStripAllValues[i].id;
                                if (Cursor != Cursors.Cross)
                                {
                                    Cursor = Cursors.Hand;
                                }
                                //this.Cursor = Cursors.Hand;
                                //now this works so lets move forward.
                                readyForMouseClick = 1;//enable on click event
                                                       //   DoDragDrop(chart1.Series["My Series"].Points, DragDropEffects.Move);


                                //this.DragEnter += new DragEventHandler(newDrag_DragEnter);
                                //this.MouseMove += new MouseEventHandler(newDrag_MouseMove);




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

                            ////--Lets clear the indicator point first.
                            //seriesLineIndicator.Points.Clear();

                            ////--Thie indicator should be shown 
                            //if(menuStripAllValues.Count > 0)
                            //{
                            //    //--Then only show the line indicator...
                            //    double xOne = menuStripAllValues[idSelected].xVal;
                            //    double yOne = yValue;
                            //    double xTwo = menuStripAllValues[idSelected - 1].xVal;
                            //    double yTwo = menuStripAllValues[idSelected - 1].yVal;
                            //    //--Then only show the line indicator...
                            //    IndicatorLineForNodeMovement(idSelected, xOne, yOne, xTwo, yTwo);
                            //}


                            //--This is the begining of the actual code


                            //menuStripAllValues[idSelected].xVal = xAxis1;
                            menuStripAllValues[idSelected].yVal = yAxis1;

                            // label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripAllValues)
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {
                                string labelValue;
                                if (menuStripAllValues[x].showItemText == "Label")
                                {
                                    labelValue = menuStripAllValues[x].label;
                                }
                                else if (menuStripAllValues[x].showItemText == "Name")
                                {
                                    labelValue = menuStripAllValues[x].name;
                                }
                                else
                                {
                                    labelValue = menuStripAllValues[x].source;
                                }


                                ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                                incrementIndex++;

                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {

                                ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                                incrementIndex++;

                            }



                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..







                        }
                        else if (Control.ModifierKeys == Keys.Shift)
                        {
                            //--This ctrl key is for moving along the y-  axis...

                            ////--Lets clear the indicator point first.
                            //seriesLineIndicator.Points.Clear();

                            ////--Thie indicator should be shown 
                            //if (menuStripAllValues.Count > 0)
                            //{
                            //    double xOne = xValue;
                            //    double yOne = menuStripAllValues[idSelected].yVal;
                            //    double xTwo = menuStripAllValues[idSelected - 1].xVal;
                            //    double yTwo = menuStripAllValues[idSelected - 1].yVal;
                            //    //--Then only show the line indicator...
                            //    IndicatorLineForNodeMovement(idSelected, xOne, yOne,xTwo,yTwo);

                            //}



                            //--THis function basically evolve when the shift key is pressed and mouse move.
                            // MessageBox.Show("shift  is pressed for y  axis constant");

                            menuStripAllValues[idSelected].xVal = xAxis1;
                            //menuStripAllValues[idSelected].yVal = yAxis1;

                            //label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripAllValues)
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {
                                string labelValue;
                                if (menuStripAllValues[x].showItemText == "Label")
                                {
                                    labelValue = menuStripAllValues[x].label;
                                }
                                else if (menuStripAllValues[x].showItemText == "Name")
                                {
                                    labelValue = menuStripAllValues[x].name;
                                }
                                else
                                {
                                    labelValue = menuStripAllValues[x].source;
                                }


                                ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                                incrementIndex++;

                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {

                                ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                                incrementIndex++;

                            }



                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..





                        }
                        else
                        {

                            //--Show indicator

                            ////--Lets clear the indicator point first.
                            //seriesLineIndicator.Points.Clear();

                            ////--Thie indicator should be shown 
                            //if (menuStripAllValues.Count > 0)//--If there is more than two node
                            //{
                            //    //--Then only show the line indicator...
                            //    double xOne = xValue;
                            //    double yOne =yValue;
                            //    double xTwo = menuStripAllValues[idSelected - 1].xVal;
                            //    double yTwo = menuStripAllValues[idSelected - 1].yVal;
                            //    //--Then only show the line indicator...
                            //    IndicatorLineForNodeMovement(idSelected, xOne, yOne, xTwo, yTwo);

                            //}



                            menuStripAllValues[idSelected].xVal = xAxis1;
                            menuStripAllValues[idSelected].yVal = yAxis1;

                            //label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripAllValues)
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {
                                string labelValue;
                                if (menuStripAllValues[x].showItemText == "Label")
                                {
                                    labelValue = menuStripAllValues[x].label;
                                }
                                else if (menuStripAllValues[x].showItemText == "Name")
                                {
                                    labelValue = menuStripAllValues[x].name;
                                }
                                else
                                {
                                    labelValue = menuStripAllValues[x].source;
                                }


                                ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                                incrementIndex++;

                            }
                            //--resetting incrementIndex
                            incrementIndex = 0;
                            for (int x = 0; x < menuStripAllValues.Count; x++)
                            {

                                ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                                incrementIndex++;

                            }



                            chart1.Invalidate();
                            incrementIndex = 0;//reset the values again..




                        }//closing of key else part
                    }


                    /*
                     }ca

                     */


                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }//close of if chart1.ChartAreas[0]









        }

        //--This is for the indicator showing part but it not implemented right now.
        private void IndicatorLineForNodeMovement(int idSelected, double x11, double y11,double x22,double y22)
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


        private void button8_Click(object sender, EventArgs e)
        {
            //this is the database connection part...
            
            //lets check if the data is present or not it the arraylist if present then only perfom the insert.

            if (temp_AL.Count > 0)
            {

                int count = temp_AL.Count;
                // the insertion part here...

                for (int i = 0; i < count; i++)
                {

                    string q = "insert into tbl_temp_humidity(temperature,humidity)values ('" + double.Parse(temp_AL[i].ToString()) + " ',  '" + double.Parse(hum_AL[i].ToString()) + "')";
                    insert_in_db(q);

                }

                MessageBox.Show("db insertion : success !");



            }//close of if

            //btn_insert_values.Enabled = false;

        }

        private void heatMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //lets add heat map form to the application ...
            form_heat_map fm_hm = new form_heat_map(this);//--This is done because we are making the form1_main change the values to main form ie form1_main
            fm_hm.Show();

        }

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
            timer1.Interval = 1000*60; // in miliseconds //2min * 30 = 60 min minute ie every 1 hour
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
            Form3 f3 = new Form3();
            f3.Show();


        }

        private void insertNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //upon this click the form should pop up

            Form_Input_For_Seriespoint form_point_input = new Form_Input_For_Seriespoint(this);
            form_point_input.Show();

        }
        Series series1 = new Series("My Series");

        int setItemSelectedID = 0;

        int oneTimeClick = 1;
        //int twoTimeClick = 0;
        //int incrementIndex = 0;
        int mouseClickAction = 0;

        int idSelected = 0;
        int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..

        bool arrowOn = false;


        double xAxis1;
        double yAxis1;

        int load = 0;//false
        private void chart1_MouseClick(object sender, MouseEventArgs e)
        {
            //--If mouse right click is perform this will be performed...

            //this is used to select the partciular id values..


            if (readyForMouseClick == 1)
            {

                if (oneTimeClick == 1)
                {
                    setItemSelectedID = idSelected;
                  //  MessageBox.Show("Node grabbed - id=" + setItemSelectedID);
                    Cursor = Cursors.Cross;
                    oneTimeClick = 0;
                    //MessageBox.Show("one time click");

                    mouseClickAction = 1;



                }

                else
                {
                    mouseClickAction = 0;
                    //two time click 
                    oneTimeClick = 1;//again reset to oneTimeClick
                    Cursor = Cursors.Arrow;
                    //MessageBox.Show("Node released by second click");



                    if (Control.ModifierKeys == Keys.Alt)
                    {
                        //--This alter key is for moving along constant x-axis ...
                        //MessageBox.Show(" alt is pressed for x axis constant");


                        //menuStripAllValues[idSelected].xVal = xAxis1;
                        menuStripAllValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripAllValues)
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripAllValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripAllValues[x].label;
                            }
                            else if (menuStripAllValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripAllValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripAllValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {

                            ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                            incrementIndex++;

                        }



                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..





                    }
                    else if (Control.ModifierKeys == Keys.Shift)
                    {
                        //--This ctrl key is for moving along the y-  axis...

                        //MessageBox.Show("shift  is pressed for y  axis constant");

                        menuStripAllValues[idSelected].xVal = xAxis1;
                        //menuStripAllValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripAllValues)
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripAllValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripAllValues[x].label;
                            }
                            else if (menuStripAllValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripAllValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripAllValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {

                            ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                            incrementIndex++;

                        }



                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..




                    }
                    else
                    {

                        menuStripAllValues[idSelected].xVal = xAxis1;
                        menuStripAllValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripAllValues[idSelected].xVal + " y " + menuStripAllValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripAllValues)
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {
                            string labelValue;
                            if (menuStripAllValues[x].showItemText == "Label")
                            {
                                labelValue = menuStripAllValues[x].label;
                            }
                            else if (menuStripAllValues[x].showItemText == "Name")
                            {
                                labelValue = menuStripAllValues[x].name;
                            }
                            else
                            {
                                labelValue = menuStripAllValues[x].source;
                            }


                            ReDrawPoints(series1, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue, menuStripAllValues[x].source, menuStripAllValues[x].name, menuStripAllValues[x].label, labelValue);
                            incrementIndex++;

                        }
                        //--resetting incrementIndex
                        incrementIndex = 0;
                        for (int x = 0; x < menuStripAllValues.Count; x++)
                        {

                            ReDrawLines(menuStripAllValues[x].id, menuStripAllValues[x].xVal, menuStripAllValues[x].yVal, menuStripAllValues[x].colorValue);
                            incrementIndex++;

                        }



                        chart1.Invalidate();
                        incrementIndex = 0;//reset the values again..
                    }//closing of key else part
                }//closing of second click



            }//closing of else block




        }

        
        private void humiditySensorCalibrationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application_Form4 ap_f4 = new Application_Form4(this);
            ap_f4.Show();


        }

        private void realTimePlottingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // this is the part of real time plotting where we try to plot the different previous values ...
            //Form5_real_time_plot f5 = new Form5_real_time_plot(this);
            //f5.Show();
            form_app_timer f5 = new form_app_timer(this);
            f5.Show();

        }





        int incrementIndex = 0;//--Defining the index


        //--this class is used for storing temporary the values of id xCoord,yCoord,source,name,label,color 
        //--so that a line could be plotted in it and we can do some processing as well 
        private class TempDataType
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

        List<TempDataType> menuStripAllValues = new List<TempDataType>();


        public void ReDrawPoints(Series s1, double x, double y, Color c, string source1, string name1, string label1x, string labelValueText)
        {

            //s1.ChartType = SeriesChartType.Point;
            string s = "source : " + source1 + "\n Name : " + name1 + "\nLable : " + label1x;
            s1.Points.AddXY(x, y);
            chart1.Series["My Series"].Points[incrementIndex].ToolTip = s;
            chart1.Series["My Series"].Points[incrementIndex].Label = labelValueText;
            s1.Points[incrementIndex].Color = c;
        }



        double humidityCalculated = 0;
        double enthalpyCalculated = 0;
        public void ReDrawLines(double id, double x, double y, Color c)
        {

            if (incrementIndex > 0)
            {
                double startHumidity1 = 0;
                double startEnthalpy1 = 0;
                double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
                double endEnthalpy1 = 0;
                //now lets plot lines between tow points...
                Series newSeries = new Series("LineSeries" + incrementIndex);
                newSeries.MarkerSize = 15;
                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newSeries.ChartType = SeriesChartType.Line;
                //newSeries.ToolTip = 
                newSeries.Color = menuStripAllValues[incrementIndex].colorValue;
                //--this sets the initial values of humidity and enthalpy
                CalculateHumidityEnthalpy((double)menuStripAllValues[incrementIndex - 1].xVal, (double)menuStripAllValues[incrementIndex - 1].yVal);
                startHumidity1 = Math.Round(humidityCalculated, 2);
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripAllValues[incrementIndex].xVal, (double)menuStripAllValues[incrementIndex].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripAllValues[incrementIndex - 1].name + " to " + menuStripAllValues[incrementIndex].name;

                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :" + Math.Round(menuStripAllValues[incrementIndex - 1].xVal, 2) + "               " + Math.Round(menuStripAllValues[incrementIndex].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : " + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;



                newSeries.ToolTip = tooltipString;
                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripAllValues[index - 1].xVal, menuStripAllValues[index].xVal, menuStripAllValues[index - 1].yVal, menuStripAllValues[index].yVal);
                newSeries.Points.Add(new DataPoint(menuStripAllValues[incrementIndex - 1].xVal, menuStripAllValues[incrementIndex - 1].yVal));
                newSeries.Points.Add(new DataPoint(menuStripAllValues[incrementIndex].xVal, menuStripAllValues[incrementIndex].yVal));
                chart1.Series.Add(newSeries);
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

            //this part is not correct yet we need to do this again....

            double phi = 0.00000;
            //double y_axis = yVal;
            //now for pg..
            ArrayList temperature_value = new ArrayList();
            ArrayList pg_value_from_txtfile = new ArrayList();

            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;
            string line1;

            using (StreamReader st = new StreamReader(path1))
            {

                while ((line1 = st.ReadLine()) != null)
                {

                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0]);
                        double temp2 = Double.Parse(value[1]);
                        //now lets add to temperature and pg array..                     
                        temperature_value.Add(temp1);
                        pg_value_from_txtfile.Add(temp2);


                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

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




        //--this is used by set data button
        int countNumberOfPoints = 0;
        int xCoord = 0;

        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {


            if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            {
                //we need to show context menu strip
                //contextMenuStrip1.Show(MousePosition);//--This is dissabled
                CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                //MessageBox.Show("Right pressed");    
                //this is calculated based on this location the graphics will be plotted..
                xCoord = e.Location.X;
                yCoord = e.Location.Y;

            }

        }

        int yCoord = 0;
        double humidityValue;
        double temperatureValue;

        string tbSource;
        string tbName;
        string tbLabel;
        Color colorValue;
        string comboboxItemText;

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

                    string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = dir + @"\t_pg.txt";
                    string path1 = file;
                    string line1;

                    using (StreamReader st = new StreamReader(path1))
                    {

                        while ((line1 = st.ReadLine()) != null)
                        {

                            string[] value = line1.Split(',');
                            try
                            {
                                double temp1 = Double.Parse(value[0]);
                                double temp2 = Double.Parse(value[1]);
                                //now lets add to temperature and pg array..                     
                                temperature_value.Add(temp1);
                                pg_value_from_txtfile.Add(temp2);


                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.ToString());
                            }


                        }//close of while

                    }//close of using

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

                    double patm = 101.325;//this is constant...
                                          // double w = 622*phi*corres_pg_value/(patm-phi*corres_pg_value);
                                          //double w1 = 622*phi*pg/(patm-phi*pg);
                    double w = yVal;
                    phi = w * patm / (622 * corres_pg_value + w * corres_pg_value);//this phi gives the relative humidty..
                    phi = phi * 100;//changing into percent..
                                    //now display in label...
                                    //lb_RH.Text = phi.ToString();

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


            string line1;
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;

            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {

                while ((line1 = st.ReadLine()) != null)
                {

                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[0]);
                        double temp2 = Double.Parse(value[1]);
                        //now lets add to temperature and pg array..                     
                        temperature_value.Add(temp1);
                        pg_value_from_txtfile.Add(temp2);


                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

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
            menuStripAllValues.Add(new TempDataType
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


            //the liine plot part is only done when ther is two points or more
            if (index > 0)
            {

                double startHumidity1 = 0;
                double startEnthalpy1 = 0;
                double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
                double endEnthalpy1 = 0;

                //now lets plot lines between tow points...
                Series newSeries = new Series("LineSeries" + index);

                //newSeries.MarkerStyle = MarkerStyle.Triangle;
                newSeries.ChartType = SeriesChartType.Line;

                //newSeries.MarkerStyle = MarkerStyle.Circle;
                newSeries.MarkerSize.Equals(15);
                newSeries.Color = menuStripAllValues[index].colorValue;

                //--this sets the initial values of humidity and enthalpy
                CalculateHumidityEnthalpy((double)menuStripAllValues[index - 1].xVal, (double)menuStripAllValues[index - 1].yVal);
                startHumidity1 = Math.Round(humidityCalculated, 2);//--Fro showing only up to 2 dec. eg."34.52"
                startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripAllValues[index].xVal, (double)menuStripAllValues[index].yVal);
                endHumidity1 = Math.Round(humidityCalculated, 2);
                endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripAllValues[index - 1].name + " to " + menuStripAllValues[index].name;


                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :" + Math.Round(menuStripAllValues[index - 1].xVal, 2) + "               " + Math.Round(menuStripAllValues[index].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : " + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;
                newSeries.ToolTip = tooltipString;
                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripAllValues[index - 1].xVal, menuStripAllValues[index].xVal, menuStripAllValues[index - 1].yVal, menuStripAllValues[index].yVal);
                newSeries.Points.Add(new DataPoint(menuStripAllValues[index - 1].xVal, menuStripAllValues[index - 1].yVal));
                newSeries.Points.Add(new DataPoint(menuStripAllValues[index].xVal, menuStripAllValues[index].yVal));
                chart1.Series.Add(newSeries);
            }



            index++;


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

        Series series1_heat_map = null;
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
            toolTip1.SetToolTip(label5_H_unit, "Kilo Joule per K.G");
        }

        private void label2_DBT_units_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label2_DBT_units, "Degree Celcius");

        }

        private void label3_Sp_H_unit_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label3_Sp_H_unit, "ratio of mass of water vapur to mass of dry air(K.G(w)/K.G(dry_air))");
        }

        private void label4_RH_unit_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label4_RH_unit, "Percent");
        }

        private void label6_DP_Unit_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(label6_DP_Unit, "Degree Celcius");
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

        private void helpPsychometricChartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\PsychometricHelp.chm";
            Help.ShowHelp(this, file);

        }

        int index_series_heat_map = 0;//this index is used by  plot_on_graph_values method

        int SeriesCount = 0;
        public void heat_map_button_click(int index_selected_heat_map,DateTime from1,DateTime to1)
        {

            if (SeriesCount > 0)
            { 
            chart1.Series.Remove(series1_heat_map);//--Removing the series that already exist...
            }
            SeriesCount = 1;

            //--lest reset soem values..
            hist_temp_hum_list.Clear();

            series1_heat_map = new Series("My Series_heat_map");//changed form "My Series"


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
                        //station_name = reader["station_name"].ToString();
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
                MessageBox.Show("value counted " + hist_temp_hum_list.Count);
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



                string line1;

                string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = dir + @"\t_pg.txt";
                string path1 = file;

                using (StreamReader st = new StreamReader(path1))
                {

                    while ((line1 = st.ReadLine()) != null)
                    {

                        string[] value = line1.Split(',');
                        try
                        {
                            double temp1 = Double.Parse(value[0]);
                            double temp2 = Double.Parse(value[1]);
                            //now lets add to temperature and pg array..                     
                            temperature_value.Add(temp1);
                            pg_value_from_txtfile.Add(temp2);


                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.ToString());
                        }


                    }//close of while

                }//close of using

                //this series is used to add to the 
                chart1.Series.Add(series1_heat_map);



                for (int i = 0; i < hist_temp_hum_list.Count; i++)
                {
                    plot_by_DBT_HR_heat_map(hist_temp_hum_list[i].temp, hist_temp_hum_list[i].hum / 100);

                }

                MessageBox.Show(Properties.Resources.Success_final);
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
                    
                    double start = min_value;

                    double value = start;
                    // double temp_value = (max_value - min_value);
                    //double increment = 0;
                    //increment = temp_value / 50;


                    //decimal val = (Decimal)((value - min_value) / (max_value - min_value));

                    Pen pen1 = new Pen(Color.FromArgb(0, 255, 0));
                    grp1.DrawRectangle(pen1, 958, 537, 15, 15);
                    SolidBrush drawBrushGreen = new SolidBrush(Color.FromArgb(0, 255, 0));
                    grp1.FillRectangle(drawBrushGreen, 958, 537, 15, 15);


                    String drawString = Math.Round(value,0).ToString();
                    // Create font and brush.
                    Font drawFont = new Font("Arial", 7);
                    SolidBrush drawBrush = new SolidBrush(Color.Black);
                    // Create point for upper-left corner of drawing.
                    PointF drawPoint = new PointF(958-12, 520);//--537->520
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


                    for (int i = 1; i <= 50; i++)
                    {

                        //decimal val = (Decimal)((value - min_value) / (max_value - min_value));
                        double val = (double)((value - min_value) / (max_value - min_value));
                        int r = Convert.ToByte(255 * val);
                        int g = Convert.ToByte(255 * (1 - val));
                        int b = 0;
                        Pen pen1 = new Pen(Color.FromArgb(r, g, b));
                        grp1.DrawLine(pen1, 958, 520 - i, 973, 520 - i);//--changed

                        if (i == 0)
                        {
                            String drawString = Math.Round(value, 0).ToString();
                            // Create font and brush.
                            Font drawFont = new Font("Arial", 7);
                            SolidBrush drawBrush = new SolidBrush(Color.Black);
                            // Create point for upper-left corner of drawing.
                            PointF drawPoint = new PointF(958-12, 520 - i); //--change
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
                            PointF drawPoint = new PointF(958-12, 520 - i);
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
                            PointF drawPoint = new PointF(958-12, 520 - i);
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
                            PointF drawPoint = new PointF(958-12, 520 - i);
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
                            PointF drawPoint = new PointF(958-12, 520 - i);
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

            plot_on_graph_values_heat_map(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            index_heat_map++;


            return 0;
        }


        public void plot_on_graph_values_heat_map(double dbt, double hr, double xval, double yval)
        {
            //chart1.Series.Clear();
            //Series series1 = new Series("My Series" + index);
            //chart1.Series.Add(series1);
            try
            {


                series1.ChartType = SeriesChartType.Point;
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

                series1.MarkerSize = 15;
                //string label = "DBT=" + dbt + ",HR=" + hr;
                //series1.Label = label;
                //chart1.Series["SeriesDBT_HR" + index].;
                series1.Points.AddXY(xval, yval);
                series1.Points[index_series_heat_map++].Color = Color.FromArgb(255, r, g, b);//blue
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






    }//close of btn4








}
