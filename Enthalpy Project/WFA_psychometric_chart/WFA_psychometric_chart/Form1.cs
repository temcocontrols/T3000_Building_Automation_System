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
using System.Collections;
namespace WFA_psychometric_chart
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
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
            string path1 = "C:\\Users\\nischal\\Desktop\\t_pg.txt";
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
            string s="";
            for (int i = 0; i < t.Count; i++)
            {
                s += t[i].ToString() + ","+pg[i].ToString()+"\n" ;
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

            double wg_calc=0; // = 622 * a_value / (patm - pg_value);
            //now calculation begins..
            double x=0;
            for (int i = 0; i < t.Count; i++)
            {
               double pg_value = Double.Parse(pg[i].ToString());
                wg_calc = 622 * pg_value / (patm - pg_value);
                x = double.Parse( t[i].ToString());
                double y = wg_calc;
                chart1.Series["Series1"].Points.AddXY(x, y);
                
              
            }//close of for

          /*  TextAnnotation text_wet_bulb_temp = new TextAnnotation();
            text_wet_bulb_temp.Name = " Wet bulb temp";
            text_wet_bulb_temp.X = 2;
            text_wet_bulb_temp.Y = 1;
            chart1.Annotations.Add(text_wet_bulb_temp);
            */

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
                    wg_calc = (622 *phi* pg_value / (patm - phi*pg_value));
                    //double x = Double.Parse(t[i].ToString());
                    double y = wg_calc;
                    x2 = double.Parse(t[i].ToString());
                    chart1.Series["Series"+ival].Points.AddXY(x2, y);
                    s1 += x2+","+ y +";";

                    
                    //index++;
                }//close of for
                MessageBox.Show(s1);
                ival++;
                //this is to print 10%,20,30,40% 
                int c=int.Parse((phi*10+1).ToString());
                chart1.Series["Series"+c].Points[45].Label = phi * 100 + "%";
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
                MessageBox.Show(s1);
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
            string path2 = "C:\\Users\\nischal\\Desktop\\t_pg1.txt";
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
                double tempval =double.Parse(pg1[i].ToString());
                double tempwg1 = 622 * tempval / (patm - tempval);
                wg1.Add(tempwg1);
            }

            //specific volume of dry air  (cubic m/kg dry air) (green)

            ArrayList vol = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double temppg1 = double.Parse(pg1[i].ToString());
                double tempt1 = double.Parse(t1[i].ToString());
                double temp = rair * (tempt1+273) / (patm - temppg1);
                vol.Add(temp);
            }

            //% air temperature at zero humidity

            ArrayList tv0 = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double tempvol = double.Parse(vol[i].ToString());

                double temp = patm * tempvol/ rair - 273;
                tv0.Add(temp);
            }

            //now lets plot..
           double xtemp = 0.79;
            for (int i = 0; i < 7; i++)
            {

                //for plotting different lines 
                chart1.Series.Add("Line"+i);
                chart1.Series["Line" + i].Color = Color.Green;
                chart1.Series["Line"+i].Points.Add(new DataPoint(double.Parse(t1[i].ToString()), double.Parse(wg1[i].ToString())));
                chart1.Series["Line" + i].Points.Add(new DataPoint(double.Parse(tv0[i].ToString()), 0));
                chart1.Series["Line"+i].ChartType = SeriesChartType.Line;
                chart1.Series["Line" + i].Points[1].Label = xtemp + "";
                xtemp =xtemp+ 0.02;

            }


            //now towards the plotting of next part....
            //% wet bulb temperature and enthalpy lines (red)
            //arraylist for storing h 


            ArrayList h = new ArrayList();//saturation specific humidity...
            for (int i = 0; i < pg1.Count; i++)
            {
                double tempval1 = double.Parse(t1[i].ToString());
                double tempval2 = double.Parse(wg1[i].ToString());

                double temp = tempval1 + 2.5*tempval2;
                h.Add(temp);
            }
            
            //temperature at zero humidity..
            ArrayList t0 = new ArrayList();
            t0 = h;
            int t_plot_value = 5;
            for (int i = 0; i < 6; i++)
            {
                //for plotting different lines 
                chart1.Series.Add("Line_r" + i);
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
            string hv =" ";
            string t0v ="";
            string t1v="";
            for(int i=0;i<h.Count;i++){
                hv += " " + h[i] + "; ";
                t0v += " "+ t0[i] +";";
                t1v += " " + t1[i] + ";";

            }
            MessageBox.Show("h=  " + hv + " \n t0 =" + t0v + " \n t1 = " + t1v);

            int t_plot1 = 10;
            for (int hval=10 ; hval <= 110; hval += 10)
            {
                //% temperature on enthalpy axis
                double t1_temp = ( hval - 12.5) / 3.5;//this is t1;
                double w1_temp = t1_temp + 5;//% specific humidity on enthalpy axis
                int t0val = hval;//t0
                chart1.Series.Add("Line_b" + hval);
                chart1.Series["Line_b" + hval].Color = Color.Black;
                chart1.Series["Line_b" + hval].Points.Add(new DataPoint(t0val,0));
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
            chart1.Series.Add("Line_b_straight");
            chart1.Series["Line_b_straight"].Color = Color.Black;
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(0, 5));
            chart1.Series["Line_b_straight"].Points.Add(new DataPoint(25, 30));
            chart1.Series["Line_b_straight"].ChartType = SeriesChartType.Line;
            //chart1.Series["Line_b_straight"].Points[0].Label = "Enthalpy kj/kg dry air";




        }
    }
}
