using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    class ChartOperation :NodeAndLineClass
    {
        public Series series1 = new Series("My Series");
        int indexForSeriesNodePoint = 0;
        public Chart chart1 { get; set; }
        Series newLineSeries;//--This is temporary for storing series name

        public void ReDrawingLineAndNode()
        {

            try
            {

                lock (this)
                {
                    if (listNodeInfoValues.Count > 0)
                    {
                        //--This is for replotting all the things again...


                        if (chart1.InvokeRequired)
                        {
                            chart1.Invoke(new Action(() => series1.Points.Clear()));
                        }
                        else
                        {
                            //lb_device_status.Text = "connected";
                            series1.Points.Clear();
                        }

                        //series1.Points.Clear();
                        for (int i = 0; i < listLineInfoValues.Count; i++)//-- this -1 is done because for three points we have two line series..
                        {
                            //chart1.Series.Remove(menuStripNodeLineInfoValues[i].lineSeriesID);
                            listLineInfoValues[i].lineSeriesID.Points.Clear();
                        }
                        //--this is redraw functionality
                        //foreach(var values in menuStripNodeInfoValues)
                        //--Resetting the index value
                        indexForSeriesNodePoint = 0;
                        for (int x = 0; x < listNodeInfoValues.Count; x++)
                        {
                            string labelValue;
                           
                            labelValue = listNodeInfoValues[x].name;
                           
                            //--Redefined code bbk305
                            ReDrawPoints(series1, listNodeInfoValues[x].xVal, listNodeInfoValues[x].yVal, listNodeInfoValues[x].colorValue,listNodeInfoValues[x].temperature_source, listNodeInfoValues[x].pressure_source, listNodeInfoValues[x].name, labelValue, listNodeInfoValues[x].marker_Size);

                            //CODE : BBK305A
                            //--incrementIndex++;
                            indexForSeriesNodePoint++;

                        }
                        //--resetting incrementIndex
                        //CODE:BBK305A  
                        // incrementIndex = 0;

                        if (listLineInfoValues.Count > 0)
                        {

                            for (int x = 0; x < listLineInfoValues.Count; x++)
                            {
                                //  incrementIndex++;


                                //--tHIS IS REDEFINED code bbk305
                                ReDrawLines(listLineInfoValues[x].ID, listLineInfoValues[x].prevNodeId, listLineInfoValues[x].nextNodeId, listLineInfoValues[x].lineSeriesID, listLineInfoValues[x].lineColorValue, listLineInfoValues[x].lineThickness, listLineInfoValues[x].name, listLineInfoValues[x].status);

                            }

                        }

                        if (chart1.InvokeRequired)
                        {
                            chart1.Invoke(new Action(() => chart1.Invalidate()));//--Invalidates the entire surface of the Chart and causes the Chart control to be redrawn.
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

            }
            catch (Exception ex)
            {
               // MessageBox.Show(ex.Message);
            }

        }//--Close of the actual function....


        public void ReDrawPoints(Series s1, double x, double y, Color c, string source_temperature, string pressure_source, string name1, string labelValueText, int marker_size_value)
        {
            // lock (this) { 

            //s1.ChartType = SeriesChartType.Point;

            //string s = "source => temp:" + source_temperature+",hum : "+source_humidity + "\n Name : " + name1 + "\nLable : " + label1x;// + "\nindex=" + indexForSeriesNodePoint;
            string s = "source \t\n temp: " + source_temperature + ",\t\n Pressure : " + pressure_source + "\n Name : " + name1;// + "\nindex=" + indexForSeriesNodePoint;
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

        } //Close of redraw point 
        List<NodeAndLineClass.DataTypeForNode> temporaryNodeValueStoreForRedrawLine = new List<NodeAndLineClass.DataTypeForNode>();
        //--Redraw line function
        public void ReDrawLines(string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color c, int thickness_value, string name, int status)
        {
            // lock (this) { 

            // if (incrementIndex > 0)
            //  if(indexForSeriesNodePoint>0) //This index is resetted later

            //   {
            temporaryNodeValueStoreForRedrawLine.Clear();//Clearing the values of the list
                                                         // MessageBox.Show("ReDrawLines FRIST LINE");


            //double startHumidity1 = 0;
            //double startEnthalpy1 = 0;
            //double endHumidity1 = 0;//--this is for the start and end humidity print in the tooltip
            //double endEnthalpy1 = 0;
            //double startSpecificVolume1 = 0;//--specific volume
            //double endSpecificVolume1 = 0;


            if (chart1.InvokeRequired)
            {
                //now lets plot lines between tow points...
                chart1.Invoke(new Action(() => newLineSeries = lineSeriesID));//new Series("LineSeries" + incrementIndex); //lineSeriesID; 


                if (chart1.Series.IndexOf(newLineSeries.Name) != -1)
                {
                    //--This  means the series is present....
                    chart1.Invoke(new Action(() => chart1.Series.RemoveAt(chart1.Series.IndexOf(newLineSeries.Name))));
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
            //var nc = new NodeAndLineClass();
            //First for previous node id
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                if (prevNodeID == listNodeInfoValues[i].ID)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore =  menuStripNodeInfoValues.GetRange(i,1);  //This is for copying the value.
                    //Copying the values on index 0 assumption 
                    temporaryNodeValueStoreForRedrawLine.Add(new NodeAndLineClass.DataTypeForNode
                    {
                        ID = listNodeInfoValues[i].ID,
                        xVal = listNodeInfoValues[i].xVal,
                        yVal = listNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = listNodeInfoValues[i].temperature_source,
                        pressure_source = listNodeInfoValues[i].pressure_source,
                        name = listNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = listNodeInfoValues[i].colorValue,
                        marker_Size = listNodeInfoValues[i].marker_Size,
                       // airFlow = menuStripNodeInfoValues[i].airFlow
                       lastUpdatedDate = listNodeInfoValues[i].lastUpdatedDate

                    });

                    break;//Break form loop
                }
            }

            //Second for the next node id
            for (int i = 0; i < listNodeInfoValues.Count; i++)
            {
                if (nextNodeID == listNodeInfoValues[i].ID)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore[1].Equals(menuStripNodeInfoValues[i]);

                    //temporaryNodeValueStore = menuStripNodeInfoValues.GetRange(i, 1);
                    //The index of this values will be temporaryNodeValueStore[1] ==> 1
                    temporaryNodeValueStoreForRedrawLine.Add(new NodeAndLineClass.DataTypeForNode
                    {
                        ID =  listNodeInfoValues[i].ID,
                        xVal =  listNodeInfoValues[i].xVal,
                        yVal =  listNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = listNodeInfoValues[i].temperature_source,
                        pressure_source = listNodeInfoValues[i].pressure_source,
                        name =  listNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = listNodeInfoValues[i].colorValue,
                        marker_Size = listNodeInfoValues[i].marker_Size,
                        //airFlow = menuStripNodeInfoValues[i].airFlow
                        lastUpdatedDate = listNodeInfoValues[i].lastUpdatedDate

                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStoreForRedrawLine.Count > 0)
            {

                //--this sets the initial values of humidity and enthalpy
                string tooltipString = "";
                ///*
                //CalculateHumidityEnthalpy(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal);
                //startHumidity1 = Math.Round(humidityCalculated, 2);
               // startEnthalpy1 = Math.Round(enthalpyCalculated, 2);
                //startSpecificVolume1 = SpecificVolumeReturn;
                //--This calculates the end humidity and the enthalpy values..
               // CalculateHumidityEnthalpy((double)temporaryNodeValueStoreForRedrawLine[1].xVal, (double)temporaryNodeValueStoreForRedrawLine[1].yVal);
                //endHumidity1 = Math.Round(humidityCalculated, 2);
                //endEnthalpy1 = Math.Round(enthalpyCalculated, 2);
               // endSpecificVolume1 = SpecificVolumeReturn;

                // MessageBox.Show("Start hum" + startHumidity1 + " end enth" + endEnthalpy1);
                //MessageBox.Show("menustripinfovalues[prevNodeID].xVal=" + menuStripNodeInfoValues[prevNodeID].xVal + "menuStripNodeInfoValues[nextNodeID].yVal=" + menuStripNodeInfoValues[nextNodeID].yVal + "menuStripNodeInfoValues[nextNodeID].xVal = "+ menuStripNodeInfoValues[nextNodeID].xVal + " menuStripNodeInfoValues[nextNodeID].yVal" + menuStripNodeInfoValues[nextNodeID].yVal);

               // double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

                //                string tooltipString = "Sequence :  " + sequenceDetected + " \n" + @"                 start             end 
                //" + "Temp         :" + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "               " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;

                
                //"            " + sequenceDetected + " \n" + @"                 "+ temporaryNodeValueStoreForRedrawLine[0].name + "             end 
                //" + "Temp         :" + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "               " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2) + "\nHumidity :" + startHumidity1 + "           " + endHumidity1 + WFA_psychometric_chart.Properties.Resources._Enthalpy + startEnthalpy1 + "           " + endEnthalpy1 + "\nEnthalpy Change:" + enthalpyChange;


                string ZeroLine = "Process:  " + name + " ";
                string FirstLine = @"Parameters                      " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
                string SecondLine = @"enthalpy                                   " + "KJ/KG                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                           " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
                //string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                string FourthLine = @"Pressure               " + "MPa  " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                       " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);
                //string FifthLine = "Volume Flow Rate           " + "m\xB3/s                " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2) + "                      " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2);

                //string SixthLine = "Specific Volume              " + "m\xB3/Kg             " + startSpecificVolume1 + "                    " + endSpecificVolume1;
                //double massFlowRate1 = temporaryNodeValueStoreForRedrawLine[0].airFlow / startSpecificVolume1;
                //double massFlowRate2 = temporaryNodeValueStoreForRedrawLine[1].airFlow / endSpecificVolume1;

                //string SeventhLine = @"Mass flow rate(dry air)   " + "Kg(dry air)/s   " + Math.Round(massFlowRate1, 2) + "                        " + Math.Round(massFlowRate2, 2);
                //string EighthLine = @"Enthalpy                           " + "KJ/Kg              " + startEnthalpy1 + "                       " + endEnthalpy1;
                //double totalEnthalpyFlow1 = massFlowRate1 * startEnthalpy1;
                //double totalEnthalpyFlow2 = massFlowRate2 * endEnthalpy1;
                //string NinthLine = @"Total Enthalpy Flow         " + "KJ/s                " + Math.Round(totalEnthalpyFlow1, 2) + "                      " + Math.Round(totalEnthalpyFlow2, 2);
                //double heatChange = totalEnthalpyFlow2 - totalEnthalpyFlow1;
                //string TenthLine = @"Heat Change                    " + "KW                  " + Math.Round(heatChange, 2) + "                     ";
                //tooltipString = ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + ThirdLine + "\n" + FourthLine + "\n" + FifthLine + "\n" + SixthLine + "\n" + SeventhLine + "\n" + EighthLine + "\n" + NinthLine + "\n" + TenthLine;
                tooltipString = ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + FourthLine;


                //*/

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
                if (chart1.InvokeRequired)
                {

                    chart1.Invoke(new Action(() => newLineSeries.ToolTip = tooltipString));
                    //newSeries.MarkerStyle = MarkerStyle.Circle;
                    //newSeries.Points.AddXY(menuStripNodeInfoValues[index - 1].xVal, menuStripNodeInfoValues[index].xVal, menuStripNodeInfoValues[index - 1].yVal, menuStripNodeInfoValues[index].yVal);

                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal))));   //for prevnodeid
                    double mid_point_XValue = (temporaryNodeValueStoreForRedrawLine[0].xVal + temporaryNodeValueStoreForRedrawLine[1].xVal) / 2;
                    double mid_point_YValue = (temporaryNodeValueStoreForRedrawLine[0].yVal + temporaryNodeValueStoreForRedrawLine[1].yVal) / 2;
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(mid_point_XValue, mid_point_YValue))));   //Middle point for plotting the Label
                    chart1.Invoke(new Action(() => newLineSeries.Points.Add(new DataPoint(temporaryNodeValueStoreForRedrawLine[1].xVal, temporaryNodeValueStoreForRedrawLine[1].yVal))));   //for nextnodeid

                    if (status == 1)
                    {
                        //Then show the label in the chart
                        chart1.Invoke(new Action(() => newLineSeries.Points[1].Label = name));
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


    }
}
