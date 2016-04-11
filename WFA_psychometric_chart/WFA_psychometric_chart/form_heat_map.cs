using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WFA_psychometric_chart
{
    public partial class form_heat_map : Form
    {
        public form_heat_map()
        {
            InitializeComponent();
        }
        int map_loaded = 0;
        
        private void form_heat_map_Load(object sender, EventArgs e)
        {

            chart_plot();
            
            //lets set the data time picker default values...
            dtp_From.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_From.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_To.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_From.Value = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.Value = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);

        }

        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
           }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();


        //chart definatioin..
        public void chart_plot()
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

            //chart1.Series["Series1"].Points[16].Label = "Wet bulb temp";
            //chart1.Series["Series1"].Points[16].LabelBackColor = Color.Red;
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
                  //  chart1.Series["Line_b" + hval].Points[1].Label = "Enthalpy kj/kg dry air";
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

        double min_value = 0;
        double max_value = 0;

        //lets create two arraylist to add those and store it in the arraylist
        ArrayList temperature_value = new ArrayList();
        ArrayList pg_value_from_txtfile = new ArrayList();

        Series series1 = null;
        int load_map_checker = 0;//checks weather to load a map or not
        int index_series = 0;//this index is used by  plot_on_graph_values method

        private void button1_Click(object sender, EventArgs e)
        {
            //initially resetting values to empty
            
            load_map_checker++;
            hist_temp_hum_list.Clear();
            temperature_value.Clear();
            pg_value_from_txtfile.Clear();
            
            if (load_map_checker>1)
            {
                index_series = 0;
                chart1.Series.Clear();
                chart1.Series.Add("Series1");
                chart1.Series.Add("Series2");
                chart1.Series.Add("Series3");
                chart1.Series.Add("Series4");
                chart1.Series.Add("Series5");
                chart1.Series.Add("Series6");
                chart1.Series.Add("Series7");
                chart1.Series.Add("Line0");
                chart1.Series.Add("Line1");
                chart1.Series.Add("Line2");
                chart1.Series.Add("Line3");
                chart1.Series.Add("Line4");
                chart1.Series.Add("Line5");
                chart1.Series.Add("Line6");
                chart1.Series.Add("Line_r0");
                chart1.Series.Add("Line_r1");
                chart1.Series.Add("Line_r2");
                chart1.Series.Add("Line_r3");
                chart1.Series.Add("Line_r4");
                chart1.Series.Add("Line_r5");
                chart1.Series.Add("Line_b10");
                chart1.Series.Add("Line_b20");
                chart1.Series.Add("Line_b30");
                chart1.Series.Add("Line_b40");
                chart1.Series.Add("Line_b50");
                chart1.Series.Add("Line_b60");
                chart1.Series.Add("Line_b70");
                chart1.Series.Add("Line_b80");
                chart1.Series.Add("Line_b90");
                chart1.Series.Add("Line_b100");
                chart1.Series.Add("Line_b110");
                chart1.Series.Add("Line_b_straight");
                chart_plot();

            }

            //resetting ends...

            series1 = new Series("My Series");


            //this  is going to plot the heat map...
            /*Steps:
            1.Get the database values..
            2.filter those values ..
            3.plot those values in the map..       
            */
            DateTime from = dtp_From.Value;
            DateTime to = dtp_To.Value;

            //2.database connection ..

            if (dtp_To.Value > dtp_From.Value)
            {

                string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //  string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                //sql connection string is this..
                string connString1 = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                using (SqlConnection connection1 = new SqlConnection(connString1))
                {
                    connection1.Open();



                    //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                    //lets pass this string to a query which does the pulling part.
                    SqlDataReader reader1 = null;
                    SqlCommand command1 = new SqlCommand("Select * from tbl_historical_data WHERE date_current BETWEEN @date_first AND @date_second", connection1);
                    command1.Parameters.AddWithValue("@date_first", dtp_From.Value);
                    command1.Parameters.AddWithValue("@date_second", dtp_To.Value);
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
                MessageBox.Show("Please select correct date format[From<To]");
            }


            //this will only be done when the data is returned

            if (hist_temp_hum_list.Count > 0) {
                MessageBox.Show("value counted " + hist_temp_hum_list.Count);
            //after we have the data we do the actual part of heat map plotting...
            //setting up maximum and minimum value to use in color value calculation..
            
            ArrayList temporary_val_temp = new ArrayList();
            max_value = hist_temp_hum_list[0].temp;
            min_value = hist_temp_hum_list[0].temp;
            for (int i = 1 ; i < hist_temp_hum_list.Count; i++)//this is done because we are counting from 1 index no error 
            {                                                  //as we are comparing the first index value with all the vlues in the index  

                if (max_value < hist_temp_hum_list[i].temp)
                {
                    max_value = hist_temp_hum_list[i].temp;
                }
                if(min_value > hist_temp_hum_list[i].temp)
                {
                    min_value = hist_temp_hum_list[i].temp;
                }                            

            }
            
            

            //min_value = hist_temp_hum_list.Min<data_type_hum_temp>().temp;

            MessageBox.Show("max = " + max_value + " ,min = " + min_value);
                //callin gthe method.
                //lets increase th performance first... this below code if from plot_by_dbt_hr



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

                //this series is used to add to the 
                chart1.Series.Add(series1);



                for (int i = 0; i < hist_temp_hum_list.Count; i++)
            {
                plot_by_DBT_HR(hist_temp_hum_list[i].temp, hist_temp_hum_list[i].hum/100);
                
            }

                MessageBox.Show("Success final !");
            }//close of if
            else
            {
                MessageBox.Show("No data found in database !");
            }

            marker();
        }//closing of the buttton...
        private void marker()
        {
            using (Graphics grp1 = this.CreateGraphics()) { 
            double start = min_value;

            double value = start;
            double temp_value = (max_value - min_value) ;
            double increment = 0;
            increment = temp_value/50;
                
                
            for(int i = 1; i <= 50; i++) {
               
            decimal val = (Decimal)((value - min_value) / (max_value - min_value));
            int r = Convert.ToByte(255 * val);
            int g = Convert.ToByte(255 * (1 - val));
            int b = 0;
            Pen pen1 = new Pen(Color.FromArgb(r,g,b));
            grp1.DrawLine(pen1,870,423-i,895,423-i);
          
                    if(i == 0)
                    {
                        String drawString = value.ToString();
                        // Create font and brush.
                        Font drawFont = new Font("Arial", 7);
                        SolidBrush drawBrush = new SolidBrush(Color.Black);
                        // Create point for upper-left corner of drawing.
                        PointF drawPoint = new PointF(842,423-i);
                        // Draw string to screen.
                        grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                    }
                    else if(i == 13)
                    {
                        String drawString = value.ToString();
                        // Create font and brush.
                        Font drawFont = new Font("Arial", 7);
                        SolidBrush drawBrush = new SolidBrush(Color.Black);
                        // Create point for upper-left corner of drawing.
                        PointF drawPoint = new PointF(842, 423 - i);
                        // Draw string to screen.
                        grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                    }
                    else if (i == 25)
                    {

                        String drawString = value.ToString();
                        // Create font and brush.
                        Font drawFont = new Font("Arial", 7);
                        SolidBrush drawBrush = new SolidBrush(Color.Black);
                        // Create point for upper-left corner of drawing.
                        PointF drawPoint = new PointF(842, 423 - i);
                        // Draw string to screen.
                        grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                    }
                   else if (i == 35)
                    {

                        String drawString = value.ToString();
                        // Create font and brush.
                        Font drawFont = new Font("Arial", 7);
                        SolidBrush drawBrush = new SolidBrush(Color.Black);
                        // Create point for upper-left corner of drawing.
                        PointF drawPoint = new PointF(842, 423 - i);
                        // Draw string to screen.
                        grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                    }
                    else if (i == 50)
                    {

                        String drawString = value.ToString();
                        // Create font and brush.
                        Font drawFont = new Font("Arial", 7);
                        SolidBrush drawBrush = new SolidBrush(Color.Black);
                        // Create point for upper-left corner of drawing.
                        PointF drawPoint = new PointF(842, 423 - i);
                        // Draw string to screen.
                        grp1.DrawString(drawString, drawFont, drawBrush, drawPoint);
                    }

                 value += increment;
                }//close of for...

            }//close of using statement..

        }

        int index = 0;
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
            
            plot_on_graph_values(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            index++;
            

            return 0;
        }
       

        public void plot_on_graph_values(double dbt, double hr, double xval, double yval)
        {
            //chart1.Series.Clear();
            //Series series1 = new Series("My Series" + index);
            //chart1.Series.Add(series1);

            

            series1.ChartType = SeriesChartType.Point;
           
            double value = dbt;
            decimal val = (Decimal)((value - min_value) / (max_value - min_value));
            int r = Convert.ToByte(255 * val);
            int g = Convert.ToByte(255 * (1 - val));
            int b = 0;

            //MessageBox.Show("dbt =" + dbt + "\n xval =" + xval + "\n yval = " + yval+"\n rgb = "+r+","+g+",0");

            

            series1.MarkerSize = 15;
            //string label = "DBT=" + dbt + ",HR=" + hr;
            //series1.Label = label;
            //chart1.Series["SeriesDBT_HR" + index].;
            series1.Points.AddXY(xval, yval);
            series1.Points[index_series++].Color = Color.FromArgb(255, r, g, b);//blue
            //series1.Enabled = true;

        }

        private void printMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try { 
            //this when click prints the chart.
            System.Drawing.Printing.PrintDocument pd = new System.Drawing.Printing.PrintDocument();
            chart1.Printing.PrintPaint(chart1.CreateGraphics(), chart1.DisplayRectangle);
            PrintDialog pdi = new PrintDialog();
            pdi.Document = pd;
            if (pdi.ShowDialog() == DialogResult.OK)
                pdi.Document.Print();

            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void saveAsImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string fileName = "";
            saveFD.InitialDirectory = "C:";
            saveFD.FileName = "ChartImage";
            saveFD.Filter = "PNG(.png) |*.png|Bitmap(.bmp) |*.bmp|JPEG |*.jpeg";
            ImageFormat format = ImageFormat.Png;
            if (saveFD.ShowDialog()==DialogResult.OK)
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
            }
            else
            {
                fileName = "ChartImage.png";                
                MessageBox.Show(" Your chart image will be saved as " +fileName);
            }
            chart1.SaveImage(fileName,format);


        }
    }
}
