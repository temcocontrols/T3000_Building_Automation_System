using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Windows.Forms.DataVisualization;
using System.Collections;
using System.Net;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Data.OleDb;
using System.Xml.Linq;
using System.Timers;
using System.Data.SqlClient;



namespace WFA_psychometric_chart
{
    public partial class Form1 : Form
    {
        //OleDbCommand cmd = new OleDbCommand();
        //OleDbConnection con = new OleDbConnection();
        SqlConnection con = new SqlConnection();
        SqlCommand cmd = new SqlCommand();
        public Form1()
        {
            InitializeComponent();
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
        ArrayList hum2_AL = 
            new ArrayList();


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
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;

            // string path;
            // path = System.IO.Path.GetDirectoryName(
            //    System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);

            // MessageBox.Show(path);
            //string  path1 = path + "\\t_pg.txt";
            //MessageBox.Show(path1);
            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
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

            //this is other part.
            radioButton1.Checked = true;

            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            //string file = dir + @"\TestDir\TestFile.txt";
            //con.ConnectionString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
           //change here..
            
            //con.ConnectionString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+dir+@"\T3000.mdb;Persist Security Info=True";
            //this is sql connnection string..
            con.ConnectionString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";
            cmd.Connection = con;

            
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked == true)
            {
                //MessageBox.Show("You have pressed 1");
                // if buttton one is clicked the units has to change...
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //this is where the actual calculation begins ..
            //lets defin the constants..

            try
            {




                //lets grab from the textbox input.
                temperature = Double.Parse(textBox1.Text.Trim());
                humidity = Double.Parse(textBox2.Text.Trim());



                Patm = 1013;
                A = 6.116441;
                m = 7.591386;
                Tn = 240.7263;
                B = 621.9907;

                //lets set the values..
                textBox3.Text = Patm.ToString();
                textBox5.Text = A.ToString();
                textBox6.Text = m.ToString();
                textBox7.Text = Tn.ToString();
                textBox9.Text = B.ToString();

                //calculating 1.dew point..
                TDewpoint = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature) / (243.04 + temperature))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature) / (243.04 + temperature)));
                //setting to textbox4 its a dew point.
                textBox4.Text = Convert.ToString(TDewpoint);
                //2.Partial pressure of saturation..
                Pws = A * Math.Pow(10, (m * TDewpoint) / (TDewpoint + Tn));
                textBox8.Text = Convert.ToString(Pws);

                X = B * Pws / (Patm - Pws);
                textBox10.Text = Convert.ToString(X);

                h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
                textBox11.Text = Convert.ToString(h);


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                textBox1.Text = "";
                textBox2.Text = "";
                //lets set the values..
                textBox3.Text = "";
                textBox5.Text = "";
                textBox6.Text = "";
                textBox7.Text = "";
                textBox9.Text = "";


            }

            //for the EU unit just check if EU unit was selected of not if selected then convert
            //this values here...


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
            Series series1 = new Series("My Series" + index);
            chart1.Series.Add(series1);

            series1.ChartType = SeriesChartType.Point;
            series1.Color = Color.FromArgb(0, 0, 255);//blue
            series1.MarkerSize = 7;
            string label = "DBT=" + dbt + ",HR=" + hr;
            series1.Label = label;
            //chart1.Series["SeriesDBT_HR" + index].;
            series1.Points.AddXY(xval, yval);
            
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
            var results = chart1.HitTest(pos.X, pos.Y, false,
                                         ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(pos.X);
                    var yVal = result.ChartArea.AxisY.PixelPositionToValue(pos.Y);

                 //   tooltip.Show("X=" + xVal + ", Y=" + yVal, this.chart1,
                   //              pos.X, pos.Y - 15);

                    //now printing the x and y coordinate to label..
                    lb_x_cord.Text = xVal.ToString();
                    lb_y_cord.Text = yVal.ToString();


                    //now lets move on to making other part 
                    /*1.find dbt value => this is x axis value 
                     * 2.find sp.ratio value => this is yaxis value
                     */
                    lb_dbt.Text = xVal.ToString();
                    lb_humidity_ratio.Text = yVal.ToString();


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
                    string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    string file = dir + @"\t_pg.txt";
                    string path1 = file;
                    string line1;

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
                    lb_RH.Text = phi.ToString();

                    //now lets calculate the dew point...
                    double humidity = phi;
                    double temperature1 = xVal;
                   double TD = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature1) / (243.04 + temperature1))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature1) / (243.04 + temperature1)));
                    //now lets print this value..
                   lb_DP.Text = TD.ToString();


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
                   lb_enthalpy.Text = h.ToString();

                }
            }

        }

        //private void button7_Click(object sender, EventArgs e)
        //{


        //    //this method helps to pull the names fo country and city in the combobox
        //    cb_country.Items.Add("USA");
        //    cb_country.Items.Add("Russia");
        //    cb_country.Items.Add("Nepal");


        //    button7.Enabled = false;
        //}

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
            form_heat_map fm_hm = new form_heat_map();
            fm_hm.Show();

        }

        private void exportDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_export_data formExportData = new form_export_data();
            formExportData.Show();

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

        //private void button4_Click_1(object sender, EventArgs e)
        //{
        //    double DBT = 0.000000;
        //    double enthalpy = 0.000000;
        //    try
        //    {
        //        DBT = Double.Parse(txt_DBT.Text.Trim());
        //        enthalpy = Double.Parse(txt_enthalpy.Text.Trim());
        //    }
        //    catch (Exception ex)
        //    {
        //        MessageBox.Show(ex.Message);
        //    }

        //    //calling the function here...
        //    plot_by_DBT_Enthalpy(DBT, enthalpy);
        //}

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





        }//close of btn4

       



       

    
}
