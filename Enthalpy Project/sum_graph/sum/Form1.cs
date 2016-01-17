using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows.Forms.DataVisualization.Charting;
namespace sum
{
    public partial class Form1 : Form
    {
        public Form1()
        {

            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            /*float patm,rair,t,pg,wg; 
            tpg = dlmread('t_pg.txt','\t'); //saturation temp/pressure
            t = tpg(:,1); //% temperature (C)
            pg = tpg(:,2); //% saturation vapor pressure (kPa)
            patm = 101.325; //% standard atmosphere (kPa)
            rair = 0.287; //% gas constant of air (kJ/kg.K)
            wg = 622*pg./(patm-pg);// % saturation specific humidity           
            for phi = 0.1:0.1:0.4, % phi = relative humidity 10% - 40%
             w = 622*phi*pg./(patm-phi*pg);
                plot(t,w)
            end*/
            int i, j = 0, k = 0;
            try
            {
                // StreamReader sr = new StreamReader("input.txt");
                //string line = "";
                //while ((line = sr.ReadLine()) != null)
                //{
                // StreamReader objreader = new StreamReader(@"c:\input.txt");

                string reader = System.IO.File.ReadAllText(@"C:\input.txt");

                string reader1 = "";
                string pattern = @"\s+";
                string pattern2 = " ";
                string replacement = " ";
                Regex rgx = new Regex(pattern);
                reader1 = rgx.Replace(reader, replacement);
                //txtShow.Text = reader1;
                string[] tmp = Regex.Split(reader1, pattern2);
                // txtShow.Text = tmp[0];
                //txtShow.Text = tmp[1];
                //txtShow.Text = tmp[2];
                //txtShow.Text = tmp[12];
                //string home = Regex.Replace(reader1,pattern);
                string[] temperature = new string[100];
                string[] humidity = new string[100];
                //List<string> word=new List<string>();
                //List<string> temperature = new List<string>();
                //List<string> humidity = new List<string>();
                //  string str="\t";
                //string str2 = string.Empty;

                for (i = 0; i < tmp.Length; i++)
                {
                    if (i % 2 == 0) { temperature[j] = tmp[i]; j++; }
                    if (i % 2 != 0) { humidity[k] = tmp[i]; k++; }

                }



                /* if (word.Contains(tmp[i]))
                 {
                     if (tmp[i] != str || tmp[i] != str2)
                     {
                         humidity[word.IndexOf(tmp[i])] += 1;
                         break;

                     }
                              
                         temperature[word.IndexOf(tmp[i])] += 1;
                         continue;

                              
                 }*/
                //for (i = 0; i < 2; i++)
                //{
                //  double sum = humidity[i] + temperature[i];
                //txtShow.Text = Convert.ToString(sum);
                //}








                /*                  if (word.Contains(tmp[i]))
                                  {
                                      if(tmp[i]==str)
                                      temperature[word.IndexOf(tmp[i])] += 1;
                                      continue;
                                  }
                                  else
                                  {
                                      word.Add(tmp[i]);
                                      //freq.Add(1);

                                  }*/

                //              } MessageBox.Show("File read", "Info");

                /*string[] splitted = line.Split('\t');
                string first = splitted[0].Trim();
                string[] splitted1 = line.Split('\n');
                string second = splitted1[0].Trim();
                x = Convert.ToInt32(first);
                y = Convert.ToInt32(second);
                 int i=0;
                int[] temperature = new int[55];
                int [] pressure =new int[55];
                for (i=0;i<2;i++ )
                {
                    temperature[i] = Convert.ToInt32(first);
                    pressure[i] = Convert.ToInt32(second);
                }
                for (i = 0; i < 2; i++)
                {
                    txtShow.AppendText(temperature[i].ToString() + Environment.NewLine);
                    txtShow.AppendText(pressure[i].ToString() + Environment.NewLine);                        
                }
                    

            }
            sr.Close();
                */
                /*  var result = File
          .ReadLines("input.txt")
          .Select(line => line.Split('#'))
                      //.Where(items => items.Length == 2)  // you may want to filter out the lines
          .Sum(items => int.Parse(items[0]) + int.Parse(items[1]));

                  txtShow.Text = result.ToString();
              }*/
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        private void btnLoad_Graph_Click(object sender, EventArgs e)
        {
            int i, j = 0, k = 0, l = 0, m = 0;
            int counter2 = 0;
            double phi = 0;
            double phi2 = 0;
            double patm = 101.325;
            double rair = 0.287;
            int counter = 0, counter1 = 0;

            try
            {

                //reading the file

                string reader = System.IO.File.ReadAllText(@"C:\input1.txt");
                string reader1 = "";
                string pattern = @"\s+";
                string pattern2 = " ";
                string replacement = " ";
                Regex rgx = new Regex(pattern);
                reader1 = rgx.Replace(reader, replacement);
                string[] tmp = Regex.Split(reader1, pattern2);
                string[] temperature = new string[100];
                string[] vaporPressure = new string[100];

                //initializing the temperature and vaporpressure array

                for (i = 0; i < tmp.Length; i++)
                {
                    if (i % 2 == 0) { temperature[j] = tmp[i]; counter++; j++; }
                    if (i % 2 != 0) { vaporPressure[k] = tmp[i]; k++; }

                }

                //plotting (t,w) i.e first curve

                for (i = 0; i < counter; i++)
                {

                    chart1.ChartAreas[0].AxisX.Minimum = 0;
                    chart1.ChartAreas[0].AxisX.Maximum = 60;


                    double wg = 622 * Convert.ToDouble(vaporPressure[i]) / (patm - Convert.ToDouble(vaporPressure[i]));
                    this.chart1.Series["first"].Points.AddXY(Convert.ToDouble((temperature[i])), wg);
                }



                // phi = 0.1:0.1:0.4, % phi = relative humidity 10% - 40%	             //w = 622*phi*pg./(patm-phi*pg);                //%     pause(1);	            //plot(t,w);


                //plotting 6 curve lines 

                for (double loop = 0.1; loop <= 0.5; loop += 0.1)
                {
                    counter2++;
                    switch (counter2)
                    {
                        case 1: phi = 0.1;
                            for (int a = 0; a < counter; a++)
                            {
                                // txtCheck.Text = "first";
                                double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                chart1.Series["second_1"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                            }
                            break;


                        case 2: phi = 0.2;
                            for (int a = 0; a < counter; a++)
                            {

                                double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                chart1.Series["second_2"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                            }
                            break;

                        case 3: phi = 0.3;
                            for (int a = 0; a < counter; a++)
                            {

                                double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                chart1.Series["second_3"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                            }
                            break;
                        case 4: phi = 0.4;
                            for (int a = 0; a < counter; a++)
                            {

                                double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                chart1.Series["second_4"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                            }
                            break;
                        default: break;
                    }

                    /* if (loop == 0.1) 
                     {
                         phi = loop;
                         for ( i = 0; i < counter; i++)
                         {
                             double w1 = 622 * phi * Convert.ToDouble(vaporPressure[i]) / (patm - phi * Convert.ToDouble(vaporPressure[i]));
                             chart1.Series["second_1"].Points.AddXY(Convert.ToDouble(temperature[i]), w1);
                         }
                        
                     }
                     else if (loop == 0.2)
                     {
                         txtCheck.Text = "Hello world!";
                         phi = loop;
                         for (i = 0; i < counter; i++)
                         {
                             double w1 = 622 * phi * Convert.ToDouble(vaporPressure[i]) / (patm - phi * Convert.ToDouble(vaporPressure[i]));
                             chart1.Series["second_2"].Points.AddXY(Convert.ToDouble(temperature[i]), w1);
                         }
                        
                     }
                        
                     else if (loop == 0.4)
                     {
                         //txtCheck.Text = "Hello world!";
                         phi = 0.4;
                         for (i = 0; i < counter; i++)
                         {
                             double w1 = 622 * phi * Convert.ToDouble(vaporPressure[i]) / (patm - phi * Convert.ToDouble(vaporPressure[i]));
                             chart1.Series["second_3"].Points.AddXY(Convert.ToDouble(temperature[i]), w1);
                         }

                     }
                     else { break; }
                 }*/
                    /*  switch (q)
                     {
                         case 1: phi = 0.1;
                             for (int a = 0; a < counter; a++)
                             {
                                 // txtCheck.Text = "first";
                                 double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                 chart1.Series["second_1"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                             }
                             break;


                         case 2: phi = 0.2;
                             for (int a = 0; a < counter; a++)
                             {

                                 double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                 chart1.Series["second_2"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                             }
                             break;

                         case 3: phi = 0.3;
                             for (int a = 0; a < counter; a++)
                             {

                                 double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                 chart1.Series["second_3"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                             }
                             break;
                         case 4: phi = 0.4;
                             for (int a = 0; a < counter; a++)
                             {

                                 double w1 = 622 * phi * Convert.ToDouble(vaporPressure[a]) / (patm - phi * Convert.ToDouble(vaporPressure[a]));
                                 chart1.Series["second_4"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                             }
                             break;
                         default: break;
                     }*/


                }
                    //second plot (t,w)                //for phi = 0.6:0.2:0.8, % phi = 60%, 80%	            //w = 622*phi*pg./(patm-phi*pg);	            //plot(t,w)                //%end
                    for (i = 1; i <= 3; i++)
                    {
                        switch (i)
                        {
                            case 1: phi2 = 0.6;
                                for (int a = 0; a < counter; a++)
                                {
                                    // txtCheck.Text = "first";
                                    double w1 = 622 * phi2 * Convert.ToDouble(vaporPressure[a]) / (patm - phi2 * Convert.ToDouble(vaporPressure[a]));
                                    chart1.Series["third_1"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                                }
                                break;


                            case 2: phi2 = 0.8;
                                for (int a = 0; a < counter; a++)
                                {

                                    double w1 = 622 * phi2 * Convert.ToDouble(vaporPressure[a]) / (patm - phi2 * Convert.ToDouble(vaporPressure[a]));
                                    chart1.Series["third_2"].Points.AddXY(Convert.ToDouble(temperature[a]), w1);
                                }
                                break;

                            default: break;
                        }
                    }
                }
            

                //end of try 


            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            try
            {
                string rreader = System.IO.File.ReadAllText(@"C:\input2.txt");
                string rreader1 = "";
                string ppattern = @"\s+";
                string ppattern2 = " ";
                string rreplacement = " ";
                Regex rrgx = new Regex(ppattern);
                rreader1 = rrgx.Replace(rreader, rreplacement);
                string[] ttmp = Regex.Split(rreader1, ppattern2);
                string[] temperature2 = new string[100];
                string[] vaporPressure2 = new string[100];



                for (int a = 0; a < ttmp.Length; a++)
                {
                    if (a % 2 == 0) { temperature2[l] = ttmp[a]; l++; }
                    if (a % 2 != 0) { vaporPressure2[m] = ttmp[a]; counter1++; m++; }

                }
                // txtCheck.Text = vaporPressure2[6];
                //second file reading
                // % specific volume and enthalpy/wet-bulb-temp
                //tpg1 = dlmread('t_pg1.txt','\t');


                //t1 = tpg1(:,1); % saturation temperature (C)
                ///pg1 = tpg1(:,2); % saturation pressure (kPa)
                ///

                // wg1 = 622*pg1./(patm-pg1); % saturation specific humidity

                //% specific volume of dry air (cubic m/kg dry air) (green)
                //vol = rair.*(t1+273)./(patm-pg1) % specific vol at saturation
                //tv0 = patm*vol/rair-273; % air temperature at zero humidity
                //for i = 1:7,
                //%     pause(1);
                //%plot([t1(i),tv0(i)],[wg1(i),0],'g-')
                //end


                double[] vol = new double[(counter1) + 2];
                double[] tv0 = new double[(counter1) + 2];
                double[] wg1 = new double[(counter1) + 2];

                for (i = 0; i < counter1; i++)
                {
                    wg1[i] = 622 * Convert.ToDouble(vaporPressure2[i]) / (patm - Convert.ToDouble(vaporPressure2[i]));
                }

                for (i = 0; i < counter1; i++)
                {
                    vol[i] = rair * (Convert.ToDouble(temperature2[i]) + 273) / (patm - Convert.ToDouble(vaporPressure2[i]));
                }

                for (i = 0; i < counter1; i++)
                {
                    tv0[i] = patm * vol[i] / rair - 273;

                }



                for (int p = 0; p <= 6; p++)
                {
                    switch (p)
                    {
                        //%plot([t1(i),tv0(i)],[wg1(i),0],'g-')
                        case 0:

                          //  chart1.Series["verti_1"].Points.AddXY(new DataPoint((Convert.ToDouble(temperature2[0])), (Convert.ToDouble(wg1[0]))));
                           // txtCheck.Text = Convert.ToString(tv0[0]);
                            //chart1.Series["verti_1"].Points.AddXY(new DataPoint((Convert.ToDouble(tv0[i])), 0));
                            //  chart1.Series["verti_1"].Points.Add((Convert.ToDouble(tv0[0])),(Convert.ToDouble(wg1[0])));

                            // chart1.Series.Add("Line");
                            //chart1.Series["verti_1"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                            //chart1.Series["verti_1"].Points.Add(new DataPoint(wg1[p], 0));
                            //   chart1.Series["Line"].ChartType = SeriesChartType.Line;
                            break;
                        // chart1.Series["verti_1"].Points.AddXY(wg1[i], 0);
                        //chart1.Series["verti_1"].Points.AddXY(Convert.ToDouble(temperature2[i]), tv0[i]);
                        //case 1:
                        //    chart1.Series["verti_2"].Points.Add((Convert.ToDouble(tv0[1])), (Convert.ToDouble(wg1[2])));
                        //chart1.Series["verti_2"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                        //chart1.Series["verti_2"].Points.Add(new DataPoint(wg1[p], 0));
                        //  break;
                        /*
                     case 2: chart1.Series["verti_3"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                               chart1.Series["verti_3"].Points.Add(new DataPoint(wg1[p], 0));
                         break;
                     case 3: chart1.Series["verti_4"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                         chart1.Series["verti_4"].Points.Add(new DataPoint(wg1[p], 0));
                         break;
                     case 4: chart1.Series["verti_5"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                         chart1.Series["verti_5"].Points.Add(new DataPoint(wg1[p], 0));
                         break;
                     case 5: chart1.Series["verti_6"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                         chart1.Series["verti_6"].Points.Add(new DataPoint(wg1[p], 0));
                         break;
                     case 6: chart1.Series["verti_7"].Points.Add(new DataPoint(Convert.ToDouble(temperature2[p]), tv0[p]));
                         chart1.Series["verti_7"].Points.Add(new DataPoint(wg1[p], 0));
                         break;*/
                        default: break;
                    }
                }
                //%plot([0,25],[5,30],'k-')

                // chart1.Series["line"].Points.Add(new DataPoint(0, 25));
                //chart1.Series["line"].Points.Add(new DataPoint(5,30));

                /*
                  for ( i = 1; i <= 7; i++)
                  {
                      double wg1 = 622 * Convert.ToDouble(vaporPressure2[i]) / (patm - Convert.ToDouble(vaporPressure2[i]));
                    // this.chart1.Series["curve2"].Points.AddXY();
                      this.chart1.Series["curve1"].Points.Add((Convert.ToDouble(temperature2[i])),(tv0[i]));  //Convert.ToDouble(temperature2[o])),(tv0[o]) ,            ((Convert.ToDouble (vaporPressure2[o])),0
                  }
                */


            }
            catch (Exception mes)
            {
                MessageBox.Show(mes.Message);
            }

        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.ShowDialog();



            if (dlg.ShowDialog() == DialogResult.OK)
            {

                string fileName;

                fileName = dlg.FileName;

                MessageBox.Show(fileName);
            }

        }
    }
}
