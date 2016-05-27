using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WFA_psychometric_chart
{
    public partial class form_process_diagram : Form
    {
        //private Form1 form_porcess;
        //public form_process_diagram(Form1 form1)
        public form_process_diagram()
        {
            InitializeComponent();
            //form_porcess = form1;
            // this.Shown += new System.EventHandler(this.form_process_diagram_Load);//-- this is used to show form load event...to remove the problem of 
            //-- mouse move event calling befor the form is loaded 
          this.Disposed += new System.EventHandler ( this.form_process_diagram_Disposed );
        }
        //global variable decleration...



        //this is used by set data button
        int countNumberOfPoints = 0;
        int xCoord = 0;
        int yCoord = 0;
        double humidityValue;
        double temperatureValue;

        //string s1 = "My Series";
        Series series1 = new Series("My Series");

        //this class is used for storing temporary the values of id xCoord,yCoord,source,name,label,color 
        //so that a line could be plotted in it and we can do some processing as well 
        private class TempDataType
        {
            public int id { get; set; } //for identifying which point is selected..

            public double xVal { get; set; }//this is the values that represent the point in a chart
            public double yVal { get; set; }

            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public Color colorValue { get; set; }

            public string showItemText { get; set; }
        }

        List<TempDataType> menuStripAllValues = new List<TempDataType>();


        private void form_process_diagram_Load(object sender, EventArgs e)
        {
            //on load method loads the map from the form one...

            plot_new_chart();

            //as we load the combobox we should selecta default drop down menu..
            toolStripComboBoxShow.ComboBox.SelectedIndex = 0;

            //this is for the chart

            chart1.Series.Add(series1);

            //string s = "pox x =  " + chart1.ChartAreas["ChartArea1"].Position.X + " pos y =" + chart1.ChartAreas["ChartArea1"].Position.Y + "posx +wid = " + (chart1.ChartAreas["ChartArea1"].Position.Width + chart1.ChartAreas["ChartArea1"].Position.X) +" pox y  +  heig = "+  (chart1.ChartAreas["ChartArea1"].Position.Height + chart1.ChartAreas["ChartArea1"].Position.Y)+"chart pos="+chart1.Location.X+",y="+chart1.Location.Y;
            //MessageBox.Show(" string  = " + s);


            //--this is byTaw
            //ca_ = chart1.ChartAreas[0];
            // s_ = chart1.Series["My Series"];
            //--end of Taw 

            string text =  WFA_psychometric_chart.Properties.Resources.Right_click_to_insert_a_new_no;


            tb1_Instruction.Text = text;


        }





        private void plot_new_chart()
        {
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
         //   chart_plot();

        }
        int map_loaded = 0;
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
            chart1.Series["Series1"].Color = Color.Black;//was red initially

            chart1.Series["Series2"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series2"].Color = Color.Black;//was blue intitially

            chart1.Series["Series3"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series3"].Color = Color.Black;//was blue initially
            chart1.Series["Series4"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series4"].Color = Color.Black;//was blue initially
            chart1.Series["Series5"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series5"].Color = Color.Black;//was blue initially

            chart1.Series["Series6"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series6"].Color = Color.Black;//was blue initially
            chart1.Series["Series7"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Series7"].Color = Color.Black;//was blue initially


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
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\t_pg.txt";
            string path1 = file;

            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {
                //int i = 0;
                //string s = " ";
                while ((line1 = st.ReadLine()) != null)
                {

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
                chart1.Series["Line" + i].Color = Color.Black;//was green initially
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
                chart1.Series["Line_r" + i].Color = Color.Black;//was red initially
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

        private void mouse_pressed_event(object sender, MouseEventArgs e)
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


            //--this is Taw sug.
            //if(dp_ != null)
            //{
            //    foreach (DataPoint dp in s_.Points)
            //        if (((RectangleF)dp.Tag).Contains(e.Location))
            //        {
            //            dp.Color = Color.Orange;
            //            dp_ = dp;
            //            break;
            //        }


            //}



        }

        private void colorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog CD1 = new ColorDialog();
            //CD1.ShowDialog();

            if (CD1.ShowDialog() == DialogResult.OK)
            {
                toolStripTextBoxColor.BackColor = CD1.Color;
                //after the color selection contextmenustrip should be shown

                CMSinsertNode.Show();

            }
        }


        string tbSource;
        string tbName;
        string tbLabel;
        Color colorValue;
        string comboboxItemText;
        private void setDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //here is the magical part happens ....
            /*
            First lets get the values form the text boxes..

            */

            tbSource = toolStripTextBoxSource.Text;
            tbName = toolStripTextBoxName.Text;
            tbLabel = toolStripTextBoxLabel.Text;
            colorValue = toolStripTextBoxColor.BackColor;

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
            MessageBox.Show("Temp= " + temperatureValue + ",hum = " + humidityValue);

            //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if ((humidityValue > 0 && humidityValue <= 100) && (temperatureValue >= 0 && temperatureValue <= 50))
            {
                //now lets plot the values....

                plot_by_DBT_HR((double)(int)temperatureValue, (double)humidityValue / 100);




            }
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_select_a_proper_region_);
            }



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

                plot_by_DBT_HR((double)(int)temperatureValue, (double)humidityValue / 100);




            }
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_select_a_proper_region_);
            }





        }



        //this series is used for plotting on the graph
        //Series series1 = new Series("My series");
        int index_series = 0;
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

            plot_on_graph_values(DBT, HR, x_axis, y_axis);

            //MessageBox.Show("reached series print" +series1.ToString());

            // index++;

            return 0;
        }





        public void plot_on_graph_values(double dbt, double hr, double xval, double yval)
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
                startHumidity1 =Math.Round( humidityCalculated,2);//--Fro showing only up to 2 dec. eg."34.52"
                startEnthalpy1 = Math.Round(enthalpyCalculated,2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripAllValues[index].xVal, (double)menuStripAllValues[index].yVal);
                endHumidity1 = Math.Round(humidityCalculated,2);
                endEnthalpy1 = Math.Round(enthalpyCalculated,2);
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripAllValues[index - 1].name + " to " + menuStripAllValues[index].name;



                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :"+Math.Round( menuStripAllValues[index - 1].xVal,2) + "               " +Math.Round( menuStripAllValues[index].xVal,2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : "+startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;
                newSeries.ToolTip = tooltipString;

                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
                newSeries.Points.Add(new DataPoint(menuStripAllValues[index - 1].xVal, menuStripAllValues[index - 1].yVal));
                newSeries.Points.Add(new DataPoint(menuStripAllValues[index].xVal, menuStripAllValues[index].yVal));
                chart1.Series.Add(newSeries);
            }



            index++;


        }//close of buttons



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





        private void SetDataMouseLeaveEvent(object sender, EventArgs e)
        {
            this.Cursor = Cursors.Arrow;
        }

        private void SetDataMouseMoveEvent(object sender, MouseEventArgs e)
        {
            this.Cursor = Cursors.Hand;
        }

        int idSelected = 0;
        int readyForMouseClick = 0;//this event will be true only when the cursor is in hand mode..

        bool arrowOn = false;


        double xAxis1;
        double yAxis1;

        int load = 0;//false
        private void MouseMoveEvent(object sender, MouseEventArgs e)
        {

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
            if ((e.X > 59 && e.Y > 23) && (e.X < 592 && e.Y < 401))
            {

                try
                {

                    //Point position = e.Location;
                    double xValue = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                    double yValue = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
                    label1.Text = xValue.ToString();
                    label2.Text = yValue.ToString();

                    xAxis1 = xValue;
                    yAxis1 = yValue;
                    //Console.Write("xval = " + xValue + "yvalue = " + yValue);
                    if (menuStripAllValues.Count > 0)
                    {
                        //foreach(var values in menuStripNodeInfoValues)

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


                            //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                            menuStripAllValues[idSelected].yVal = yAxis1;

                            // label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripNodeInfoValues)
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

                            // MessageBox.Show("shift  is pressed for y  axis constant");

                            menuStripAllValues[idSelected].xVal = xAxis1;
                            //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                            //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripNodeInfoValues)
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

                            //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                            series1.Points.Clear();
                            for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                            {
                                chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                            }
                            //--this is redraw functionality
                            //foreach(var values in menuStripNodeInfoValues)
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


                }catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }//close of if chart1.ChartAreas[0]






        }



        private void chart1_DragEnter(object sender, DragEventArgs e)
        {
            //--drag enter is enabled 

            //label3.Text = "xasis1  = "+xAxis1+" yaxis = "+yAxis1;
            //Cursor = Cursors.Hand;



        }



        private void chart1_DragLeave(object sender, EventArgs e)
        {
            //-- this event is triggered if the mouse button is left .....



            //menuStripNodeInfoValues[idSelected].xVal = chart1.ChartAreas[0].AxisX.PixelPositionToValue(Location.X);
            //menuStripNodeInfoValues[idSelected].yVal = chart1.ChartAreas[0].AxisY.PixelPositionToValue(Location.Y);

            //label5.Text = "Drag leave x ="+ menuStripNodeInfoValues[idSelected].xVal+" y "+ menuStripNodeInfoValues[idSelected].yVal;

            //series1.Points.Clear();
            //chart1.Invalidate();


        }

        private void chart1_DragOver(object sender, DragEventArgs e)
        {
            //-- drag over is triggered ... i love programming bhoj bahadure karki kathmandu Nepal(NEC 011)

            //menuStripNodeInfoValues[idSelected ].xVal = xAxis1;
            //menuStripNodeInfoValues[idSelected].yVal = yAxis1;
            //label4.Text = "dragover x = " + menuStripNodeInfoValues[idSelected ].xVal + " y =" + menuStripNodeInfoValues[idSelected ].yVal;


        }

        int setItemSelectedID = 0;

        int oneTimeClick = 1;
        //int twoTimeClick = 0;
        int incrementIndex = 0;
        int mouseClickAction = 0;
        private void chart1_Click(object sender, EventArgs e)
        {
            //this is used to select the partciular id values..


            if (readyForMouseClick == 1)
            {

                if (oneTimeClick == 1)
                {
                    setItemSelectedID = idSelected;
                    MessageBox.Show("Node grabbed - id=" + setItemSelectedID);
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
                    MessageBox.Show("Node released by second click");



                    if (Control.ModifierKeys == Keys.Alt)
                    {
                        //--This alter key is for moving along constant x-axis ...
                        //MessageBox.Show(" alt is pressed for x axis constant");


                        //menuStripNodeInfoValues[idSelected].xVal = xAxis1;
                        menuStripAllValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
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
                        //menuStripNodeInfoValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
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
                    else {

                        menuStripAllValues[idSelected].xVal = xAxis1;
                        menuStripAllValues[idSelected].yVal = yAxis1;

                        //label5.Text = "click past x =" + menuStripNodeInfoValues[idSelected].xVal + " y " + menuStripNodeInfoValues[idSelected].yVal;

                        series1.Points.Clear();
                        for (int i = 1; i <= menuStripAllValues.Count - 1; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            chart1.Series.Remove(chart1.Series["LineSeries" + i]);
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
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
        }//close of main function private void


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
                startHumidity1 = Math.Round(humidityCalculated,2);
                startEnthalpy1 = Math.Round(enthalpyCalculated,2);
                //--This calculates the end humidity and the enthalpy values..
                CalculateHumidityEnthalpy((double)menuStripAllValues[incrementIndex].xVal,(double) menuStripAllValues[incrementIndex].yVal);
                endHumidity1 = Math.Round(humidityCalculated,2);
                endEnthalpy1 = Math.Round(enthalpyCalculated,2);
                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = menuStripAllValues[incrementIndex - 1].name + " to " + menuStripAllValues[incrementIndex].name;

                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + "                 start             end \n" + "Temp         :" +Math.Round(menuStripAllValues[incrementIndex - 1].xVal,2) + "               " +Math.Round( menuStripAllValues[incrementIndex].xVal,2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + "\nEnthalpy : " + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;



                newSeries.ToolTip = tooltipString;
                //newSeries.MarkerStyle = MarkerStyle.Circle;
                //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);
                newSeries.Points.Add(new DataPoint(menuStripAllValues[incrementIndex - 1].xVal, menuStripAllValues[incrementIndex - 1].yVal));
                newSeries.Points.Add(new DataPoint(menuStripAllValues[incrementIndex].xVal, menuStripAllValues[incrementIndex].yVal));
                chart1.Series.Add(newSeries);
            }




        }

        private void CalculateHumidityEnthalpy(double xVal,double yVal)
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

         double    Patm = 1013;
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

           // Form_Input_For_Seriespoint form_point_input = new Form_Input_For_Seriespoint(this);
           // form_point_input.Show();


        }

      public void form_process_diagram_Disposed ( object sender, System.EventArgs e )
      {
      }
    }


}


