using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PH_App
{
    public partial class FormEditNodeAndLine : Form
    {
        MainController bcs;
        public FormEditNodeAndLine(MainController mc1)
        {
            bcs = mc1;
            InitializeComponent();
        }

        private void FormEditNodeAndLine_Load(object sender, EventArgs e)
        {

        }


        public void LoadNodeAndLine()
        {
            //dataGridView1.Rows.Clear();
            /*
            steps:1. get the data from the list values..
                  2.display the data
                    a) display the background color of the button in the color value and when clicked show the colordialog toselect
                    b) and the text in the list
            */
            //--First lets do for the node
            //--Lets clear the rows first 
            //--This one for handling null refrence error
            //dataGridView1.Enabled = false;


            dataGridView1.Rows.Clear();
            dataGridView2.Rows.Clear();
            string temperature_value_For_Device = "Device"; //By default it will be device 
            string pressure_value_for_Device = "Device";

            //--For node
            if (bcs.listNodeInfoValues.Count > 0)
            {

                int indexForParticularRow = 0;

                for (int i = 0; i < bcs.listNodeInfoValues.Count; i++)
                {
                    //Now lets display the values..
                    string temperature_value = "";
                    int temperature_indexValue = 0;
                    //Now identifying the item selected.

                    //For temperature source 

                    if (bcs.listNodeInfoValues[i].temperature_source == "Manual")
                    {
                        temperature_value = "Manual";
                        temperature_indexValue = 0;
                    }
                    else if (bcs.listNodeInfoValues[i].temperature_source == "Web")
                    {
                        temperature_value = "Web";
                        temperature_indexValue = 1;
                    }
                    else if (bcs.listNodeInfoValues[i].temperature_source == "Mix")
                    {
                        // temperature_value = "Mix";
                        temperature_value = "Manual";//For mix we make it manual
                        //temperature_indexValue = 2;//No need for this one
                    }

                    else if (bcs.listNodeInfoValues[i].temperature_source == "Device") //For anything else //--if() changed
                    {

                        /*
                         Steps : 1. Pulling data from table for device
                                 2. setting the data value to temperature_value item

                         */
                        bcs.PullDataForTemperaturePressureSource(bcs.listNodeInfoValues[i].ID);
                        //--Now we can get the values
                        if (bcs.listTempPressureSourceInfo.Count > 0)
                        {
                            temperature_value_For_Device = bcs.listTempPressureSourceInfo[0].TemepratureSoureString; // "Device";
                        }
                        else
                        {
                            temperature_value_For_Device = "Device";
                        }
                        temperature_value = temperature_value_For_Device;//This one is for inputting
                        temperature_indexValue = 2;
                    }


                    //==For entering the datasource in each datagridview by removing previous one
                    //--This one is for tempeature source in columnIndex = 1;
                    //DataGridViewComboBoxCell dgvcbc = (DataGridViewComboBoxCell)dataGridView1.Rows[0].Cells[1];
                    //dgvcbc.Items.Clear();
                    //dgvcbc.Items.Add("Manual");
                    //dgvcbc.Items.Add("Web");
                    //if (bcs.menuStripNodeInfoValues[i].temperature_source == "Device")
                    //{
                    //    dgvcbc.Items.Add(temperature_value);

                    //}else
                    //{
                    //    dgvcbc.Items.Add("Device");
                    //}

                    //==Now for source

                    string pressure_sourceValue = "";
                    int indexForSource = 0;
                    if (bcs.listNodeInfoValues[i].pressure_source == "Manual")
                    {
                        pressure_sourceValue = "Manual";
                        indexForSource = 0;

                    }
                    else if (bcs.listNodeInfoValues[i].pressure_source == "Web")
                    {
                        pressure_sourceValue = "Web";
                        indexForSource = 1;

                    }
                    else if (bcs.listNodeInfoValues[i].pressure_source == "Mix")
                    {
                        // humidity_sourceValue = "Mix";
                        pressure_sourceValue = "Manual";
                        //  indexForSource = 2;
                    }
                    else if (bcs.listNodeInfoValues[i].pressure_source == "Device") //--changed recently because humidty need to be  changed
                    {
                        bcs.PullDataForTemperaturePressureSource(bcs.listNodeInfoValues[i].ID);
                        //--Now we can get the values
                        if (bcs.listTempPressureSourceInfo.Count > 0)
                        {
                            //temperature_value = bcs.listTempHumSourceInfo[0].TemepratureSoureString; // "Device";
                            pressure_value_for_Device = bcs.listTempPressureSourceInfo[0].PressureSourceString; // "Device";//"Device";
                        }
                        else
                        {
                            pressure_value_for_Device = "Device";

                        }
                        pressure_sourceValue = pressure_value_for_Device;
                        indexForSource = 2;
                    }


                    //--Samething for humidity
                    //var dgvcbc1 = CB_DGV_Humidity_Source;//dataGridView1.Rows[0].Cells[4];
                    //dgvcbc1.Items.Clear();
                    //dgvcbc1.Items.Add("Manual");
                    //dgvcbc1.Items.Add("Web");
                    //if (bcs.menuStripNodeInfoValues[i].humidity_source == "Device")
                    //{
                    //    dgvcbc1.Items.Add(humidity_sourceValue);

                    //}
                    //else
                    //{
                    //    dgvcbc1.Items.Add("Device");
                    //}

                    // DataGridViewComboBoxColumn cb = (DataGridViewComboBoxColumn)showText;
                    //cb.Items[0].ToString();
                    // MessageBox.Show("source VALUE MEMBER= " + CB_DGV_Source.Items[indexForSource]);
                   // bcs.enthalpyHumidityCalculatorForXYvalue(bcs.menuStripNodeInfoValues[i].xVal, bcs.menuStripNodeInfoValues[i].yVal);

                    if (bcs.listNodeInfoValues[i].pressure_source == "Mix")
                    {
                        //string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name,"Manual", bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(),"Manual", Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(), bcs.menuStripNodeInfoValues[i].airFlow.ToString() };
                        //dataGridView1.Rows.Add(row);

                        dataGridView1.Rows.Add();//After adding rows assigning the values to the Rows 2 datagridviewComboboxCell

                        //--This one if for temperature 
                        DataGridViewComboBoxCell cb = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[1];
                        cb.Items.Clear();
                        cb.Items.Add("Manual");
                        cb.Items.Add("Web");
                        if (bcs.listNodeInfoValues[i].temperature_source == "Device")
                        {
                            cb.Items.Add(temperature_value_For_Device);

                        }
                        else
                        {
                            cb.Items.Add("Device");

                        }

                        //--For humidity
                        DataGridViewComboBoxCell cb_Hum = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                        cb_Hum.Items.Clear();
                        cb_Hum.Items.Add("Manual");
                        cb_Hum.Items.Add("Web");
                        if (bcs.listNodeInfoValues[i].pressure_source == "Device")
                        {
                            cb_Hum.Items.Add(pressure_value_for_Device);

                        }
                        else
                        {
                            cb_Hum.Items.Add("Device");
                        }
                        //--Now assigning the values , for mix node
                        dataGridView1.Rows[i].Cells[0].Value = bcs.listNodeInfoValues[i].name;
                        dataGridView1.Rows[i].Cells[1].Value = "Manual"; //bcs.menuStripNodeInfoValues[i].name;
                        dataGridView1.Rows[i].Cells[2].Value = bcs.listNodeInfoValues[i].ID;
                        dataGridView1.Rows[i].Cells[3].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();
                        dataGridView1.Rows[i].Cells[4].Value = "Manual";
                        dataGridView1.Rows[i].Cells[5].Value = "";//Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[7].Value = "";
                        dataGridView1.Rows[i].Cells[8].Value = bcs.listNodeInfoValues[i].marker_Size.ToString();//"Manual";
                        //dataGridView1.Rows[i].Cells[9].Value = bcs.menuStripNodeInfoValues[i].airFlow.ToString(); //"Manual";





                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].ReadOnly = false;
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].ReadOnly = false;
                        //indexForParticularRow
                        dataGridView1.Rows[indexForParticularRow].Cells[1].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[4].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[6].ReadOnly = true;
                       // dataGridView1.Rows[indexForParticularRow].Cells[9].ReadOnly = true;

                    }
                    else
                    {

                        // string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name,  CB_DGV_Temperature_Source.Items[temperature_indexValue].ToString(), bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(),CB_DGV_Humidity_Source.Items[indexForSource].ToString() ,Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(),bcs.menuStripNodeInfoValues[i].airFlow.ToString() };
                        // string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name, temperature_value, bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(), CB_DGV_Humidity_Source.Items[indexForSource].ToString(), Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(), bcs.menuStripNodeInfoValues[i].airFlow.ToString() };
                        //--This one is for enabling or dissabling a particualr row
                        if (bcs.listNodeInfoValues[i].temperature_source == "Manual")
                        {
                            //sourceValue = "Manual";
                            //indexForSource = 0;

                            //  dataGridView1.Rows.Add(row);
                            //=================================New added way of coding ===============================//

                            dataGridView1.Rows.Add();//After adding rows assigning the values to the Rows 2 datagridviewComboboxCell

                            //--This one if for temperature 
                            DataGridViewComboBoxCell cb = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[1];
                            cb.Items.Clear();
                            cb.Items.Add("Manual");
                            cb.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].temperature_source == "Device")
                            {
                                cb.Items.Add(temperature_value_For_Device);

                            }
                            else
                            {
                                cb.Items.Add("Device");

                            }

                            //--For humidity
                            DataGridViewComboBoxCell cb_Hum = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                            cb_Hum.Items.Clear();
                            cb_Hum.Items.Add("Manual");
                            cb_Hum.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].pressure_source == "Device")
                            {
                                cb_Hum.Items.Add(pressure_value_for_Device);

                            }
                            else
                            {
                                cb_Hum.Items.Add("Device");
                            }
                            //--Now assigning the values
                            dataGridView1.Rows[i].Cells[0].Value = bcs.listNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[1].Value = temperature_value;//CB_DGV_Temperature_Source.Items[temperature_indexValue].ToString();//"Manual"; //bcs.menuStripNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[2].Value = bcs.listNodeInfoValues[i].ID;
                            dataGridView1.Rows[i].Cells[3].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();
                            dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[5].Value = "";//Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[7].Value = "";
                            dataGridView1.Rows[i].Cells[8].Value = bcs.listNodeInfoValues[i].marker_Size.ToString();//"Manual";
                           // dataGridView1.Rows[i].Cells[9].Value = bcs.menuStripNodeInfoValues[i].airFlow.ToString(); //"Manual";

                            //================================End of new added way of coding========================//

                            //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].ReadOnly = false;
                            //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].ReadOnly = false;
                            //indexForParticularRow
                            dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = false;
                            // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;

                            //--This one is for humidity
                            if (bcs.listNodeInfoValues[i].pressure_source == "Manual")
                            {
                                //sourceValue = "Manual";
                                //indexForSource = 0;                                                    
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;
                            }
                            else if (bcs.listNodeInfoValues[i].pressure_source == "Web")
                            {
                                //sourceValue = "Web";
                                //indexForSource = 1;
                                // dataGridView1.Rows.Add(row);                                             
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }
                            else
                            {
                                //sourceValue = "Device";
                                //indexForSource = 2;
                                //dataGridView1.Rows.Add(row);                                             
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }



                        }
                        else if (bcs.listNodeInfoValues[i].temperature_source == "Web")
                        {
                            //sourceValue = "Web";
                            //indexForSource = 1;
                            // dataGridView1.Rows.Add(row);
                            //=================================New added way of coding ===============================//

                            dataGridView1.Rows.Add();//After adding rows assigning the values to the Rows 2 datagridviewComboboxCell

                            //--This one if for temperature 
                            DataGridViewComboBoxCell cb = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[1];
                            cb.Items.Clear();
                            cb.Items.Add("Manual");
                            cb.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].temperature_source == "Device")
                            {
                                cb.Items.Add(temperature_value_For_Device);

                            }
                            else
                            {
                                cb.Items.Add("Device");

                            }

                            //--For humidity
                            DataGridViewComboBoxCell cb_Hum = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                            cb_Hum.Items.Clear();
                            cb_Hum.Items.Add("Manual");
                            cb_Hum.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].pressure_source == "Device")
                            {
                                cb_Hum.Items.Add(pressure_value_for_Device);

                            }
                            else
                            {
                                cb_Hum.Items.Add("Device");
                            }
                            //--Now assigning the values
                            dataGridView1.Rows[i].Cells[0].Value = bcs.listNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[1].Value = temperature_value;//CB_DGV_Temperature_Source.Items[temperature_indexValue].ToString();//"Manual"; //bcs.menuStripNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[2].Value = bcs.listNodeInfoValues[i].ID;
                            dataGridView1.Rows[i].Cells[3].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();
                            dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[5].Value = "";//Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString();// Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[7].Value = "";
                            dataGridView1.Rows[i].Cells[8].Value = bcs.listNodeInfoValues[i].marker_Size.ToString();//"Manual";
                           // dataGridView1.Rows[i].Cells[9].Value = bcs.menuStripNodeInfoValues[i].airFlow.ToString(); //"Manual";

                            //================================End of new added way of coding========================//

                            dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                            // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;

                            //--This one is for humidity
                            if (bcs.listNodeInfoValues[i].pressure_source == "Manual")
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;
                            }
                            else if (bcs.listNodeInfoValues[i].pressure_source == "Web")
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }
                            else
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }

                        }
                        else
                        {
                            //sourceValue = "Device";
                            //indexForSource = 2;
                            // dataGridView1.Rows.Add(row);
                            //=================================New added way of coding ===============================//

                            dataGridView1.Rows.Add();//After adding rows assigning the values to the Rows 2 datagridviewComboboxCell
                            //--This one if for temperature 
                            DataGridViewComboBoxCell cb = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[1];
                            cb.Items.Clear();
                            cb.Items.Add("Manual");
                            cb.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].temperature_source == "Device")
                            {
                                cb.Items.Add(temperature_value_For_Device);
                            }
                            else
                            {
                                cb.Items.Add("Device");

                            }

                            //--For humidity
                            DataGridViewComboBoxCell cb_Hum = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                            cb_Hum.Items.Clear();
                            cb_Hum.Items.Add("Manual");
                            cb_Hum.Items.Add("Web");
                            if (bcs.listNodeInfoValues[i].pressure_source == "Device")
                            {
                                cb_Hum.Items.Add(pressure_value_for_Device);

                            }
                            else
                            {
                                cb_Hum.Items.Add("Device");
                            }
                            //--Now assigning the values
                            dataGridView1.Rows[i].Cells[0].Value = bcs.listNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[1].Value = temperature_value;//CB_DGV_Temperature_Source.Items[temperature_indexValue].ToString();//"Manual"; //bcs.menuStripNodeInfoValues[i].name;
                            dataGridView1.Rows[i].Cells[2].Value = bcs.listNodeInfoValues[i].ID;
                            dataGridView1.Rows[i].Cells[3].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();
                            dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[5].Value = ""; //Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString(); ;// Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[7].Value = "";
                            dataGridView1.Rows[i].Cells[8].Value = bcs.listNodeInfoValues[i].marker_Size.ToString();//"Manual";
                           // dataGridView1.Rows[i].Cells[9].Value = bcs.menuStripNodeInfoValues[i].airFlow.ToString(); //"Manual";

                            //================================End of new added way of coding========================//

                            dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                            // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;

                            //--This one is for humidity
                            if (bcs.listNodeInfoValues[i].pressure_source == "Manual")
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;
                            }
                            else if (bcs.listNodeInfoValues[i].pressure_source == "Web")
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }
                            else
                            {
                                dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                            }


                        }

                    }//Close of else function for mix

                    indexForParticularRow++;
                    //dataGridView1.Rows.Add(row);
                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[i].Cells[7];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.listNodeInfoValues[i].colorValue;

                }
            }//clsoe of if value

            //--Second : This is for the line to show in the datagridview2
            /*
             Lets store the node1 and node2 parameters in variables.
             */
            //--variables to store info of two nodes
            string idNode1 = "", lastUpdatedDateNode1 = "", humiditySourceNode1 = "", temperatureSourceNode1 = "", nameNode1 = "";
            string idNode2 = "", lastUpdatedDateNode2 = "", humiditySourceNode2 = "", temperatureSourceNode2 = "", nameNode2 = "";
            double xValueNode1 = 0, yValueNode1 = 0;
            double xValueNode2 = 0, yValueNode2 = 0;
            int airFlowNode1 = 0, airFlowNode2 = 0;

            //--Scanning for the values
            if (bcs.listLineInfoValues.Count > 0)
            {
                for (int i = 0; i < bcs.listLineInfoValues.Count; i++)
                {

                    string startNodeName = "";
                    string endNodeName = "";
                    //Now lets calculate the startNodeName and endNodeName
                    for (int x = 0; x < bcs.listNodeInfoValues.Count; x++)
                    {
                        if (bcs.listLineInfoValues[i].prevNodeId == bcs.listNodeInfoValues[x].ID)
                        {
                            nameNode1 = startNodeName = bcs.listNodeInfoValues[x].name;
                            idNode1 = bcs.listNodeInfoValues[x].ID;
                            lastUpdatedDateNode1 = bcs.listNodeInfoValues[x].lastUpdatedDate;
                            humiditySourceNode1 = bcs.listNodeInfoValues[x].pressure_source;
                            temperatureSourceNode1 = bcs.listNodeInfoValues[x].temperature_source;
                            //nameNode1 =
                            xValueNode1 = bcs.listNodeInfoValues[x].xVal;
                            yValueNode1 = bcs.listNodeInfoValues[x].yVal;
                            // airFlowNode1 = (int)bcs.menuStripNodeInfoValues[x].airFlow;
                            break;
                        }

                    }

                    //--This one is for end nodename
                    for (int v = 0; v < bcs.listNodeInfoValues.Count; v++)
                    {
                        if (bcs.listLineInfoValues[i].nextNodeId == bcs.listNodeInfoValues[v].ID)
                        {
                            nameNode2 = endNodeName = bcs.listNodeInfoValues[v].name;
                            idNode2 = bcs.listNodeInfoValues[v].ID;
                            lastUpdatedDateNode2 = bcs.listNodeInfoValues[v].lastUpdatedDate;
                            humiditySourceNode2 = bcs.listNodeInfoValues[v].pressure_source;
                            temperatureSourceNode2 = bcs.listNodeInfoValues[v].temperature_source;
                            //nameNode1 =
                            xValueNode2 = bcs.listNodeInfoValues[v].xVal;
                            yValueNode2 = bcs.listNodeInfoValues[v].yVal;
                            // airFlowNode2 = (int)bcs.menuStripNodeInfoValues[v].airFlow;
                            break;
                        }

                    }

                    //Lets calculate some energy parameters such as DBT, Relative humidity,humidity ratio,,
                    //specific volume, mass flow rate,enthalpy, Total energy flow, and heat change.

                    //--Lets make a function which returns all the other values
                    // EnergyParameterCalculationForTwoNodes(xValueNode1, yValueNode1, airFlowNode1, xValueNode2, yValueNode2, airFlowNode2);
                    //double relativeHumidity1, relativeHumidity2, spVol1, spVol2, massFlowRate1, massFlowRate2, enthalpy1, enthalpy2, totalEnergyFlow1, totalEnergyFlow2, heatChange;
                    //now lets display...                    
                    string[] row = new string[] { bcs.listLineInfoValues[i].ID, bcs.listLineInfoValues[i].name, startNodeName, endNodeName, "", bcs.listLineInfoValues[i].lineThickness.ToString(), bcs.listLineInfoValues[i].prevNodeId.ToString(), bcs.listLineInfoValues[i].nextNodeId.ToString(), bcs.listLineInfoValues[i].lineSeriesID.ToString(), "" };//, Math.Round(xValueNode1, 2).ToString(), Math.Round(relativeHumidity1, 2).ToString(), Math.Round(yValueNode1, 2).ToString(), Math.Round(spVol1, 2).ToString(), Math.Round(massFlowRate1, 2).ToString(), Math.Round(enthalpy1, 2).ToString(), Math.Round(totalEnergyFlow1, 2).ToString(), Math.Round(xValueNode2, 2).ToString(), Math.Round(relativeHumidity2, 2).ToString(), Math.Round(yValueNode2, 2).ToString(), Math.Round(spVol2, 2).ToString(), Math.Round(massFlowRate2, 2).ToString(), Math.Round(enthalpy2, 2).ToString(), Math.Round(totalEnergyFlow2, 2).ToString(), Math.Round(heatChangeForBoth, 2).ToString() };
                    dataGridView2.Rows.Add(row);

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[i].Cells[4];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.listLineInfoValues[i].lineColorValue;

                    try
                    {
                        DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[i].Cells[9];
                        if (bcs.listLineInfoValues[i].status == 1)
                        {
                            cbCell.Value = "true";

                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }

                }

            }

            dataGridView1.EndEdit();//To end the edit call ,and to refresh
            dataGridView2.EndEdit();
            //  dataGridView1.Enabled = true;

        }

    }
}
