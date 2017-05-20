using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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

        //=================Variables Compartment ===============//
        string initialTemp, initialPressure, initialName, initialLabel, initialNodeSize, initialAirFlow;
        ///THis flag is for omitting of calling of the double event while clicking in the web section
        bool flagForWeb = false;
        /// <summary>
        /// This store temperature source value like VAR1, LABEL1 etc from device 
        /// or web or manual
        /// </summary>
        public string temperatureSourceName;
        /// <summary>
        /// This store humidity source value like VAR1, LABEL1 etc from device 
        /// or web or manual
        /// </summary>
        public string pressureSourceName;
        public string finalTemperatureSource;
        public string finalPressureSource;
        //==================End of variable compartment========//
        private void dataGridView1_EditingControlShowing(object sender, DataGridViewEditingControlShowingEventArgs e)
        {
            try
            {
                if (sender == null || e == null)
                {
                    return;
                }
                else if (dataGridView1.CurrentCell.Value == null)
                {
                    //MessageBox.Show("Current cell value is empty");
                    return;
                }
                if (dataGridView1.CurrentCell.RowIndex < bcs.listNodeInfoValues.Count)
                {

                    ComboBox combo = e.Control as ComboBox;

                    if (combo != null)
                    {
                     
                        combo.SelectionChangeCommitted -= new EventHandler(ComboBox_SelectedIndexChanged);
                        combo.SelectionChangeCommitted += new EventHandler(ComboBox_SelectedIndexChanged);
                    }
                }
                //}catch(Exception ex)
                //{
                //    MessageBox.Show("exception at line 45:" + ex.Message);
                //}
            }
            catch (NullReferenceException nre)
            {
                //MessageBox.Show("\nline 49.\n" + nre.Message);

            }

        }

        //--Lets make a global variable that can be accessed by edit cell mode

        //public string temporarySelectedName = "";
        //public string temporarySelectedLabel = "";
        //public string temporarySelectedShowText = "";

        public string tempNodeID = "";
        public int tempRowIndex = 0;



        //--End of making the global variable 


        private void ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender == null || e == null)
            {
                return;//If cb is null then empty
            }
            ComboBox cb = (ComboBox)sender;

            if (cb.Text == null)
            {
                return;//Handling null refrence exception
            }
            string item = Convert.ToString(cb.Text);

            if (item == "" || item == null)
            {
                //yedi item khe mildo chaina bhannea futher na janea 
                return;
            }

            //if (item != null)
            // MessageBox.Show("bbk item selected  = "+item);

            /*
            Here we can differenticate between the item are 
            from source :
            These will be  : Manual , Web ra Device

            Testai gari :  which item to show ma k k hun6 bhannea
            ==> Name
            ==>Label
            ==>Source
            yo garna if else lagayea hun6 


            */

            if (item == "Manual")
            {
                // MessageBox.Show("Manual select gariyo " +cb.SelectedIndex);

                if (item == "" || item == null)
                {
                    return;
                }
                //MessageBox.Show("Web select gariyo ");
                //=We need to select chose the web value and then update the values...

                DataGridViewComboBoxCell cbx = (DataGridViewComboBoxCell)dataGridView1.CurrentCell;

                cbx.Value = cb.Items[0];

                if (cbx.ColumnIndex == 1)
                {
                    //--TEMPERATURE SOURCE IS CHANGED TO Manual

                    string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
                    string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
                    string temperature_Source = "Manual"; //Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    string pressure_source = "";
                    string pressureSouceCheckValue = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);
                    if (pressureSouceCheckValue == "Manual" || pressureSouceCheckValue == "Web")
                    {
                        pressure_source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
                    }
                    else
                    {
                        pressure_source = "Device";
                    }
                    double xVal = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value));
                    // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    double pressure = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));
                    //  bcs.CalculateYFromXandHumidity(xVal, pressure / 100);
                    double yVal = pressure;//bcs.y_coord_value;

                    // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                    int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
                   // int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

                    // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                    updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, pressure_source, nameVal, colorVal, nodeSizeVal);



                }
                else if (cbx.ColumnIndex == 4)
                {
                    //Humidity source is chosen to be Manual

                    string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
                    string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name

                    // string temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    string temperature_Source = "";
                    string tempValueCheck = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    if (tempValueCheck == "Manual" || tempValueCheck == "Web")
                    {
                        temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    }
                    else
                    {
                        temperature_Source = "Device";
                    }

                    string pressure_souce = "Manual"; //Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
                    double xVal = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value));
                    // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    double pressure = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));
                    //bcs.CalculateYFromXandHumidity(xVal, pressure / 100);
                    double yVal = pressure;// bcs.y_coord_value;

                    // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                    int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
                    //int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

                    // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                    updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, pressure_souce, nameVal, colorVal, nodeSizeVal);

                }
            }
            //else if (item == "Web")
            //{
            //    if (item == "" || item == null)
            //    {
            //        return;
            //    }
            //    //MessageBox.Show("Web select gariyo ");
            //    //=We need to select chose the web value and then update the values...
            //    if (dataGridView1.CurrentCell.Value == null)
            //    {
            //        return;
            //    }

            //    DataGridViewComboBoxCell cbx = (DataGridViewComboBoxCell)dataGridView1.CurrentCell;
            //    cbx.Value = cb.Items[1];


            //    // WebUpdateSelection(); 
            //    if (cbx.ColumnIndex == 1)
            //    {
            //        WebUpdateSelectionForTemperature();
            //    }
            //    else if (cbx.ColumnIndex == 4)
            //    {
            //        WebUpdateSelectionForHumidity();
            //    }

            //    //RefreshDataFromDBAndChart();
            //    RefreshChartAndDGVForMixNodeFunction();
            //    //this.Refresh();

            //    //dataGridView1.Refresh();
            //    ////--Now Refresh Data...
            //    //RefreshDataFromDBAndChart();
            //    ////--After refreshing
            //    //LoadNodeAndLine();//--Loading the data 

            //    //RefreshChartAndDGVForMixNodeFunction();
            //    // dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
            //    // dataGridView1.EditMode(false);//This one is for setting form edit mode to normal mode
            //    // Put the cells in edit mode when user enters them.
            //    dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
            //    dataGridView1.EndEdit();

            //}
            else //if(item == "Device")
            {
                if (dataGridView1.CurrentCell.RowIndex < bcs.listNodeInfoValues.Count)
                {





                    //MessageBox.Show("column index = " + dataGridView1.CurrentCell.ColumnIndex + ",row index" + dataGridView1.CurrentCell.RowIndex);

                    //--This sometimes gives error why not take the id form menustripnodeinfovalue
                    tempNodeID = bcs.listNodeInfoValues[dataGridView1.CurrentCell.RowIndex].ID;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();

                    if (item == "" || item == null)
                    {
                        return;
                    }


                    if (tempNodeID == "")
                    {
                        //If empty then dont proceed forward
                        return;
                    }

                    DataGridViewComboBoxCell cbx = (DataGridViewComboBoxCell)dataGridView1.CurrentCell;
                    cbx.Value = cb.Items[2];

                    //temporarySelectedName = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();
                    //temporarySelectedLabel = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();
                    //temporarySelectedShowText = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    // MessageBox.Show("Device select gariyo "+ temporarySelectedShowText);
                    //InsertNodeFormForBuildingChartSetting insertF = new InsertNodeFormForBuildingChartSetting(this);
                    //insertF.ShowDialog();
                    try
                    {
                        // Form_Device_Information_For_EditMode fx = new Form_Device_Information_For_EditMode(this);
                        //fx.ShowDialog();
                        if (dataGridView1.CurrentCell.ColumnIndex == 1)
                        {
                            //Temperature is selected 
                            //Form_TemperatureSourceDeviceInput ftemp = new Form_TemperatureSourceDeviceInput(this);
                            //ftemp.ShowDialog();
                            try
                            {
                                var fm = new Form_TemperatureInput(this);
                                fm.ShowDialog();
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show($"{ex.Message} \n Please select input, output and variable section in T3000 and retry!");
                            }
                        }
                        else if (dataGridView1.CurrentCell.ColumnIndex == 4)
                        {
                            //Humidity is selected

                            //Form_HumiditySourceDeviceInput ftemp = new Form_HumiditySourceDeviceInput(this);
                            //ftemp.ShowDialog();
                            try
                            {
                                var fm = new Form_PressureInput(this);
                                fm.ShowDialog();
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show($"{ex.Message} \n Please select input, output and variable section in T3000 and retry!");
                            }
                        }
                    }
                    catch (ArgumentNullException ex)
                    {
                        MessageBox.Show("exception:" + ex.Message);
                    }                    
                }
            }

          

        }


        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //dataGridView1.EndEdit();
            if (dataGridView1.CurrentCell.Value == null)
            {
                return;
            }

            if (sender == null || e == null)
            {
                dataGridView1.EndEdit();
                return;
            }
            //dataGridView1.
            // MessageBox.Show("Line1280 : ");
            //--For temperature source
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.listNodeInfoValues.Count) && (e.ColumnIndex == 1))
            {
                if (sender == null || e == null)
                {
                    dataGridView1.EndEdit();
                    return;
                }
            }
            //-------For Humidity source
            else if ((e.RowIndex >= 0 && e.RowIndex < bcs.listNodeInfoValues.Count) && (e.ColumnIndex == 4))
            {
                if (sender == null || e == null)
                {
                    dataGridView1.EndEdit();
                    return;
                }
            }

            try
            {
                //--This one is new one
                //This part contains the cell click values
                if ((e.RowIndex >= 0 && e.RowIndex < bcs.listNodeInfoValues.Count) && (e.ColumnIndex == 7))
                {
                    if (colorDialog1.ShowDialog() == DialogResult.OK)
                    {
                        DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[e.RowIndex].Cells[7];
                        buttonCell.FlatStyle = FlatStyle.Popup;
                        buttonCell.Style.BackColor = colorDialog1.Color;//System.Drawing.Color.Red;
                                                                        //--We need to update to db as well

                        try
                        {
                            //--Once the name is changed it has to save the changes
                            //string finalSize = dataGridView1.CurrentCell.Value.ToString();
                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                            string temperature_Source = "";
                            string value = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            if (value == "Web" || value == "Manual")
                            {
                                temperature_Source = value;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            }
                            else
                            {
                                temperature_Source = "Device";
                            }


                            string pressure_source = "";

                            string pressureSourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            if (pressureSourceVal == "Web" || pressureSourceVal == "Manual")
                            {
                                pressure_source = pressureSourceVal;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name

                            }
                            else
                            {
                                pressure_source = "Device";
                            }

                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double pressure = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            // bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = pressure;//bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                           // int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, pressure_source, nameVal, colorVal, nodeSizeVal);
                        }
                        catch { }
                    }
                }

            }
            catch { }


        }
        public void RefreshChartAndDGVForMixNodeFunction()
        {
            //--Refreshing data
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            try
            {

                //Lets record previous data here 
                if (dataGridView1.CurrentCell.ColumnIndex == 3)
                {
                    try
                    {

                        if (dataGridView1.CurrentCell.Value == null)
                        {
                            dataGridView1.CurrentCell.Value = initialTemp;
                            return;
                        }
                        //This one is temperature
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].
                        // dataGridView1.CurrentCell.Style.BackColor = Color.Red;
                        double finalTemp = double.Parse(dataGridView1.CurrentCell.Value.ToString().Trim())+273.15;//adding will make it to Kelvin
                        // Create the regular expression
                        // string pattern = @"\w+_";
                        // Regex regex = new Regex(pattern);
                        // Compare a string against the regular expression
                        // return regex.IsMatch(finalTemp);
                        //--Now lets insert the values
                        //.                     

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input number values");

                        dataGridView1.CurrentCell.Value = initialTemp;
                        return;
                    }

                    try
                    {
                        //--Once the name is changed it has to save the changes

                        double finalTemp = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString())+273.15; 
                       
                        string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        string pressure_sourceVal;
                        if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Web")
                        {
                            pressure_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name

                        }
                        else
                        {
                            pressure_sourceVal = "Device";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        }
                        string Temperature_sourceVal;
                        if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Web")
                        {
                            Temperature_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        }
                        else
                        {
                            Temperature_sourceVal = "Device"; // dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        }

                     
                        double pressure = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        var opt = new MathOperation();
                        double enthalpyValue = opt.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", pressure * 1000000, "T", finalTemp, "water")/1000;
                       // bcs.CalculateYFromXandHumidity(xVal, pressure / 100);
                        double yVal = pressure;//bcs.y_coord_value;
                        double xVal = enthalpyValue;



                        //double yVal = //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());

                        // MessageBox.Show("column index " + dataGridView1.CurrentCell.ColumnIndex+",\ntemperature="+xVal+"\nHumidity ="+humidity+"\nyval= "+yVal);
                        //  string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        //int airFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
                        //--This one is closed 
                        //  UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, Temperature_sourceVal, pressure_sourceVal, nameVal, colorVal, nodeSizeVal);
                        // dataGridView1.CurrentCell.Style.BackColor = Color.SteelBlue;
                        //=============================For mix node refresh function===========//
                        //  MessageBox.Show("HERE WE ARE");
                        //bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

                        RefreshChartAndDGVForMixNodeFunction();
                        //=================================End of : For mix node section==============//
                    }
                    catch { }

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
                {
                    //This one is pressure
                    // initialHumidity = dataGridView1.CurrentCell.Value.ToString();
                    try
                    {

                        if (dataGridView1.CurrentCell.Value == null)
                        {
                            dataGridView1.CurrentCell.Value = initialPressure;
                            return;
                        }

                        double finalHum = double.Parse(dataGridView1.CurrentCell.Value.ToString());
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input number values");

                        dataGridView1.CurrentCell.Value = initialPressure;
                        return;
                    }

                    try
                    {
                        //--Once the name is changed it has to save the changes

                        string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                                                                                                                          //string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                                                                                                                          //string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        string pressure_SourceValue;
                        if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Web")
                        {
                            pressure_SourceValue = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name

                        }
                        else
                        {
                            pressure_SourceValue = "Device";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        }
                        string Temperature_sourceVal;
                        if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Web")
                        {
                            Temperature_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        }
                        else
                        {
                            Temperature_sourceVal = "Device"; // dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        }
                        double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value.ToString());
                        // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        double pressure = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        // bcs.CalculateYFromXandHumidity(xVal, pressure / 100);
                        double yVal = pressure;//bcs.y_coord_value;

                        // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                       // int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, Temperature_sourceVal, pressure_SourceValue, nameVal, colorVal, nodeSizeVal);

                        //=============================For mix node refresh function===========//
                       // bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

                        RefreshChartAndDGVForMixNodeFunction();
                        //=================================End of : For mix node section==============//

                    }
                    catch { }



                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 0)
                {
                    //This one is name
                    // initialName = dataGridView1.CurrentCell.Value.ToString();

                    /*
                     STEPS : Check for the temperature value using the inex value and the update based on that
                     */
                    int index = dataGridView1.CurrentCell.RowIndex;//This is the index
                    string temperature_source = bcs.listNodeInfoValues[index].temperature_source;
                    if (temperature_source != "Mix")
                    {


                        try
                        {


                            if (dataGridView1.CurrentCell.Value == null)
                            {
                                dataGridView1.CurrentCell.Value = initialName;
                                return;
                            }
                            string finalName = dataGridView1.CurrentCell.Value.ToString();

                            string pattern = @"^\w+$"; //@"\b\w+es\b";
                            Regex rgx = new Regex(pattern);
                            //string sentence = "Who writes these notes?";
                            if (rgx.IsMatch(finalName) == true)
                            {
                                //matched no need to edit 
                            }
                            else
                            {
                                MessageBox.Show("You can only input letters,numbers and underscores values");
                                dataGridView1.CurrentCell.Value = initialName;
                                return;

                            }


                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("You can only input letters,numbers and underscores values");

                            dataGridView1.CurrentCell.Value = initialName;
                            return;
                        }

                        try
                        {
                            //UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                                                                                                                              //string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                                                                                                                              //string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            string pressure_source_value;
                            if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Web")
                            {
                                pressure_source_value = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name

                            }
                            else
                            {
                                pressure_source_value = "Device";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            }
                            string Temperature_sourceVal;
                            if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Web")
                            {
                                Temperature_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            }
                            else
                            {
                                Temperature_sourceVal = "Device"; // dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            }
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double pressure = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            //bcs.CalculateYFromXandHumidity(xVal, pressure / 100);
                            double yVal = pressure;//bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            //Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                          //  int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, Temperature_sourceVal, pressure_source_value, nameVal, colorVal, nodeSizeVal);





                        }
                        catch { }


                    }
                    



                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 8)
                {
                    //This one is nodesize
                    // initialNodeSize = dataGridView1.CurrentCell.Value.ToString();

                    int index = dataGridView1.CurrentCell.RowIndex;//This is the index
                    string temperature_source = bcs.listNodeInfoValues[index].temperature_source;
                    if (temperature_source != "Mix")
                    {


                        try
                        {

                            if (dataGridView1.CurrentCell.Value == null)
                            {
                                dataGridView1.CurrentCell.Value = initialNodeSize;
                                return;
                            }
                            string finalSize = dataGridView1.CurrentCell.Value.ToString();

                            string pattern = @"^[0-9]+$"; //for integers only
                            Regex rgx = new Regex(pattern);
                            //string sentence = "Who writes these notes?";
                            if (rgx.IsMatch(finalSize) == true)
                            {
                                //matched no need to edit 
                            }
                            else
                            {
                                MessageBox.Show("You can only input integer values");
                                dataGridView1.CurrentCell.Value = initialNodeSize;
                                return;
                            }

                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("You can only input integer values");

                            dataGridView1.CurrentCell.Value = initialNodeSize;
                            return;
                        }

                        try
                        {
                            //--Once the name is changed it has to save the changes
                            // string finalSize = dataGridView1.CurrentCell.Value.ToString();

                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                                                                                                                              //string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                                                                                                                              //string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name


                            string pressureSouce;
                            if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString() == "Web")
                            {
                                pressureSouce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name

                            }
                            else
                            {
                                pressureSouce = "Device";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            }
                            string Temperature_sourceVal;
                            if (dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Manual" || dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString() == "Web")
                            {
                                Temperature_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            }
                            else
                            {
                                Temperature_sourceVal = "Device"; // dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            }
                          //  double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[6].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double pressure = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            // bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = pressure; //bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                           // int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, Temperature_sourceVal, pressureSouce, nameVal, colorVal, nodeSizeVal);

                        }
                        catch { }

                    }
                    
                } //Close of 8
                
                else if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    try
                    {
                        //--For temperature value
                        finalTemperatureSource = dataGridView1.CurrentCell.Value.ToString();//dataGridView1.CurrentCell.Value.ToString().Trim();                         
                                                                                            //MessageBox.Show("here : " + finalTemperatureSource);

                        // TemperatureInputPorcessForDevice(finalTempInput);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 4)
                {
                    try
                    {
                        //For humidity value
                        finalPressureSource = dataGridView1.CurrentCell.Value.ToString().Trim();
                        //dataGridView1_KeyPress(sender, e);
                        //HumidityInputPorcessForDevice(finalHumInput);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }

                }
            }
            catch
            {

            }



        }

        public void updateNodeInfoBasedOnPresentValue(string NodeID, double xVal, double yVal, string temperature_source, string pressure_source, string name, Color color, int nodesize)
        {
            string nodeIDVal = NodeID;
            string temperatureSource = temperature_source;
            string pressureSouce = pressure_source;
            double temperature_Val = xVal;
            double y_value = yVal;
            string nameVal = name;
            Color colorVal = color;
            int nodeSizeVal = nodesize;
           // int airFlow = airFlow1;
            string buildingName = bcs.selectedBuildingList[0].BuildingName;

            if (flagForWeb == true)
            {
                flagForWeb = false;//Dissabling theflag
                return;
            }

            // DatabaseOperationsForEditNodeClass ObjDBClass = new DatabaseOperationsForEditNodeClass();
            //  ObjDBClass.UpdateDataForNodeInfoInEditNodeSection(buildingName, nodeIDVal, temperature_Val, y_value, temperatureSource, humiditySouce, nameVal, colorVal, nodeSizeVal, airFlow);
            var db = new DatabaseOperationModel();
            db.UpdateDataForNodeInfoInEditNodeSection(buildingName, nodeIDVal, temperature_Val, y_value, temperatureSource, pressureSouce, nameVal, colorVal, nodeSizeVal);
            //--Now Refresh Data...
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 




        }

        public void RefreshDataFromDBAndChart()
        {
            bcs.LoadNodeAndLineFromDB(bcs.indexOfChartSelected);   //Lets make it passing the stirngs 

            // flagForInsertOrUpdateDataToDB = 1;
            //--This is also completed..    
            bcs.chart1RefreshCustom();
            //chart1.Invalidate();
            //bcs.ReDrawingLineAndNode();

        }

        private void FormEditNodeAndLine_Load(object sender, EventArgs e)
        {
            LoadNodeAndLine();
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
            MathOperation mth = new MathOperation();
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

                    //===============================calling coolProp api for temperature values====//

                    double temperatureValueFromCoolProp = 0;
                    
                    temperatureValueFromCoolProp = Math.Round( mth.IAPWS_IF97_TowParameterEquivalentFxn("T", "P", bcs.listNodeInfoValues[i].yVal * 1000000, "H", bcs.listNodeInfoValues[i].xVal * 1000,"water"),2)-273.15; //CoolProp.PropsSI()//Now in degreecelcius

                    //==========================END of calling coolprop============================//

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

                    //if (bcs.listNodeInfoValues[i].pressure_source == "Mix")
                    //{
                    //    //string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name,"Manual", bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(),"Manual", Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(), bcs.menuStripNodeInfoValues[i].airFlow.ToString() };
                    //    //dataGridView1.Rows.Add(row);

                    //    dataGridView1.Rows.Add();//After adding rows assigning the values to the Rows 2 datagridviewComboboxCell

                    //    //--This one if for temperature 
                    //    DataGridViewComboBoxCell cb = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[1];
                    //    cb.Items.Clear();
                    //    cb.Items.Add("Manual");
                    //    cb.Items.Add("Web");
                    //    if (bcs.listNodeInfoValues[i].temperature_source == "Device")
                    //    {
                    //        cb.Items.Add(temperature_value_For_Device);

                    //    }
                    //    else
                    //    {
                    //        cb.Items.Add("Device");

                    //    }

                    //    //--For humidity
                    //    DataGridViewComboBoxCell cb_Hum = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                    //    cb_Hum.Items.Clear();
                    //    cb_Hum.Items.Add("Manual");
                    //    cb_Hum.Items.Add("Web");
                    //    if (bcs.listNodeInfoValues[i].pressure_source == "Device")
                    //    {
                    //        cb_Hum.Items.Add(pressure_value_for_Device);

                    //    }
                    //    else
                    //    {
                    //        cb_Hum.Items.Add("Device");
                    //    }
                    //    //--Now assigning the values , for mix node
                    //    dataGridView1.Rows[i].Cells[0].Value = bcs.listNodeInfoValues[i].name;
                    //    dataGridView1.Rows[i].Cells[1].Value = "Manual"; //bcs.menuStripNodeInfoValues[i].name;
                    //    dataGridView1.Rows[i].Cells[2].Value = bcs.listNodeInfoValues[i].ID;
                    //    dataGridView1.Rows[i].Cells[3].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();
                    //    dataGridView1.Rows[i].Cells[4].Value = "Manual";
                    //    dataGridView1.Rows[i].Cells[5].Value = "";//Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                    //    dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
                    //    dataGridView1.Rows[i].Cells[7].Value = "";
                    //    dataGridView1.Rows[i].Cells[8].Value = bcs.listNodeInfoValues[i].marker_Size.ToString();//"Manual";
                    //    //dataGridView1.Rows[i].Cells[9].Value = bcs.menuStripNodeInfoValues[i].airFlow.ToString(); //"Manual";





                    //    //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].ReadOnly = false;
                    //    //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].ReadOnly = false;
                    //    //indexForParticularRow
                    //    dataGridView1.Rows[indexForParticularRow].Cells[1].ReadOnly = true;
                    //    dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                    //    dataGridView1.Rows[indexForParticularRow].Cells[4].ReadOnly = true;
                    //    dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                    //    dataGridView1.Rows[indexForParticularRow].Cells[6].ReadOnly = true;
                    //   // dataGridView1.Rows[indexForParticularRow].Cells[9].ReadOnly = true;

                    //}
                    //else
                    //{

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
                            //cb.Items.Add("Web");
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
                            //cb_Hum.Items.Add("Web");
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
                            dataGridView1.Rows[i].Cells[3].Value = temperatureValueFromCoolProp;//"";
                            dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[5].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString(); //Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                            dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
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
                           // cb.Items.Add("Web");
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
                           // cb_Hum.Items.Add("Web");
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
                        dataGridView1.Rows[i].Cells[3].Value = temperatureValueFromCoolProp;//"";
                        dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[5].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString(); //Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
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
                           // cb.Items.Add("Web");
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
                            //cb_Hum.Items.Add("Web");
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
                        dataGridView1.Rows[i].Cells[3].Value = temperatureValueFromCoolProp;//"";
                        dataGridView1.Rows[i].Cells[4].Value = pressure_sourceValue;// CB_DGV_Humidity_Source.Items[indexForSource].ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[5].Value = Math.Round(bcs.listNodeInfoValues[i].yVal, 2).ToString(); //Math.Round(bcs.humDataGridValue, 2).ToString();//"Manual";
                        dataGridView1.Rows[i].Cells[6].Value = Math.Round(bcs.listNodeInfoValues[i].xVal, 2).ToString();//Math.Round(bcs.enthalpyDataGridView, 2).ToString();//"Manual";
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

                   // }//Close of else function for mix

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

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            try
            {
                if (dataGridView1.CurrentCell.Value == null)
                {
                    //dataGridView1.CurrentCell.Value = initialAirFlow;
                    dataGridView1.EndEdit();
                    return;
                }
                //Lets record previous data here 
                if (dataGridView1.CurrentCell.ColumnIndex == 3)
                {
                    //This one is temperature
                    initialTemp = dataGridView1.CurrentCell.Value.ToString().Trim();
                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
                {
                    //This one is humidity
                    initialPressure = dataGridView1.CurrentCell.Value.ToString().Trim();

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 0)
                {
                    //This one is name
                    initialName = dataGridView1.CurrentCell.Value.ToString().Trim();
                }
                //else if (dataGridView1.CurrentCell.ColumnIndex == 1)
                //{
                //    //This one is lable
                //    initialLabel = dataGridView1.CurrentCell.Value.ToString();
                //}
                else if (dataGridView1.CurrentCell.ColumnIndex == 8)
                {
                    //This one is nodesize
                    initialNodeSize = dataGridView1.CurrentCell.Value.ToString().Trim();
                }
                //else if (dataGridView1.CurrentCell.ColumnIndex == 9)
                //{
                //    //This one is nodesize
                //    initialAirFlow = dataGridView1.CurrentCell.Value.ToString().Trim();
                //}
                else if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    //This one is nodesize
                    temperatureSourceName = dataGridView1.CurrentCell.Value.ToString();
                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 4)
                {
                    //This one is nodesize
                    pressureSourceName = dataGridView1.CurrentCell.Value.ToString().Trim();
                }
            }
            catch
            {

            }

        }


        //===========================This one is for Temperature Source Pull=========================//

        public void TemperatureInputPorcessForDevice(string DeviceIdentificationKeyWord)
        {
            /*
             *Prerequisite : the datagridviewcombobox need to be dynamic for this to work
             Steps : 1. Interpret the variable inputed
                     2. Find controller info 
                     3. Find Variable info
                     4. Pull data from yabee
                     5. Update data to db
                     6. Update the DGV  
             */
            Regex reg = new Regex("^[^<>.!@#%/]+$");//new Regex(@"[A-Za-z0-9\-\_]+");//Regular Expression for checking letters , hypens and numbers.
            Match match = reg.Match(DeviceIdentificationKeyWord);
            if (match.Success)
            {
                //Matched so its ok
            }
            else
            {
                //Match fail so fall back
                MessageBox.Show("Please enter a valid entity.");
                return;
            }


            bool condition = processInputData(DeviceIdentificationKeyWord);
            // MessageBox.Show("ProcessInputData finish");

            if (condition == true)
            {
                //Means we have input for controllers and variable infos not label
                //So we have controllers values as
                //controllers identified

                // MessageBox.Show("condition true start");
                int controllerNumberValue = controllerNumber;//This is alwyas between >0 and < 1000,ie 1,2,3,....999
                string VariableIdentifiedValue = variableIdentifier;
                int variableNumberValue = variableNumber;//Here [var]1 menas 0 input index and [var]2 means 1
                //MessageBox.Show("controllerNum=" + controllerNumberValue + ", variableiden= " + VariableIdentifiedValue + ",varnum= " + variableNumberValue);
                if (VariableIdentifiedValue == "IN")
                {
                    //We have input section.
                    //We have variable section.
                    //--We need to do filtering accordingly
                    FilterDataFromT3000 fd = new FilterDataFromT3000();
                    Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                    DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                    ft.FindPathOfBuildingDB();
                    string buildingPath = ft.BuildingSelected[0].Building_Path;
                    // fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath); //Pulls the input information
                    // fd.ReadDataFromAlexDatabaseForOutput("OUTPUTable", buildingPath);//pulls the output information
                    fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath);//Pulls variable information
                    //==We only need variable data...
                    //MessageBox.Show("Inside IN , inputFromT3000List.Count= " + fd.InputFromT3000List.Count);
                    for (int i = 0; i < fd.InputFromT3000List.Count; i++)
                    {
                        //if (controllerNumberValue == int.Parse(fd.InputFromT3000List[i].PanelID) && (variableNumberValue - 1) == int.Parse(fd.InputFromT3000List[i].InputIndex))
                        if (controllerNumberValue == int.Parse(fd.InputFromT3000List[i].PanelID) && (variableNumberValue - 1) == int.Parse(fd.InputFromT3000List[i].InputIndex))//the input index starts from 0 so but in yabee starts from 1
                        {
                            //==We are ok value is present now we need to check for unit
                            if (fd.InputFromT3000List[i].InputUnit == "Deg.C")
                            {
                                //We can accept this value 

                                //    MessageBox.Show("Deg.C matched  and value of label = " + fd.InputFromT3000List[i].InputLabel);

                                //--Now steps 
                                /*
                                 * 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                 1. Retrive data from device
                                 2. Update data in database
                                 3. Refresh Controller
                                 */
                                // double returnValue = dpp.InputDeviceInfoAndValue()

                                fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controllerNumberValue);//We have data now.

                                if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                {
                                    //==We have the values now we need to retrive the data

                                    //double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue-1);
                                    double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue);
                                    if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                    {
                                        MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                        return;//we can break form loop as well
                                    }
                                    //==Now updating data in database
                                    //     MessageBox.Show("Temperature returned value = " + returnValueTemperature+", Object instance = "+ fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance);
                                    //------------------------------------Working here -------------------------------------------------------//
                                    //UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue-1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_INPUT");
                                    UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_INPUT");
                                    //------------------------------------End of working here------------------------------------------------//
                                    CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);
                                }
                            }
                            else
                            {
                                //Invalid input
                                MessageBox.Show("This is not a temperature parameter!");
                                return;
                            }
                            break; //We found the value so break from the loop
                        }

                    }

                }
                else if (VariableIdentifiedValue == "OUT")
                {
                    //--we have output section

                    //We have input section.
                    //We have variable section.
                    //--We need to do filtering accordingly
                    FilterDataFromT3000 fd = new FilterDataFromT3000();
                    Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                    DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                    ft.FindPathOfBuildingDB();
                    string buildingPath = ft.BuildingSelected[0].Building_Path;
                    // fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath); //Pulls the input information
                    // fd.ReadDataFromAlexDatabaseForOutput("OUTPUTable", buildingPath);//pulls the output information
                    fd.ReadDataFromAlexDatabaseForOutput("OUTPUTable", buildingPath);//Pulls variable information
                    //==We only need variable data...
                    for (int i = 0; i < fd.outputFromT3000List.Count; i++)
                    {
                        //if (controllerNumberValue == int.Parse(fd.outputFromT3000List[i].out_panel) && (variableNumberValue - 1) == int.Parse(fd.outputFromT3000List[i].out_index))
                        if (controllerNumberValue == int.Parse(fd.outputFromT3000List[i].out_panel) && (variableNumberValue - 1) == int.Parse(fd.outputFromT3000List[i].out_index))
                        {
                            //==We are ok value is present now we need to check for unit
                            if (fd.outputFromT3000List[i].out_units == "Deg.C")
                            {
                                //We can accept this value 


                                //--Now steps 
                                /*
                                 * 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                 1. Retrive data from device
                                 2. Update data in database
                                 3. Refresh Controller
                                 */
                                // double returnValue = dpp.InputDeviceInfoAndValue()

                                fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controllerNumberValue);//We have data now.

                                if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                {
                                    //==We have the values now we need to retrive the data

                                    //double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue-1);
                                    double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue);
                                    if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                    {
                                        MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                        return;//we can break form loop as well
                                    }
                                    //==Now updating data in database
                                    //------------------------------------Working here -------------------------------------------------------//
                                    //UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue-1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_OUTPUT");
                                    UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_OUTPUT");
                                    //------------------------------------End of working here------------------------------------------------//
                                    CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);
                                }
                            }
                            else
                            {
                                //Invalid input
                                MessageBox.Show("This is not a temperature parameter!");
                                return;
                            }
                            break; //We found the value so break from the loop
                        }

                    }

                }
                else if (VariableIdentifiedValue == "VAR") //We are strict regarding typing "var" small case will not work 
                {
                    //We have variable section.
                    //--We need to do filtering accordingly
                    FilterDataFromT3000 fd = new FilterDataFromT3000();
                    Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                    DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                    ft.FindPathOfBuildingDB();
                    string buildingPath = ft.BuildingSelected[0].Building_Path;
                    // fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath); //Pulls the input information
                    // fd.ReadDataFromAlexDatabaseForOutput("OUTPUTable", buildingPath);//pulls the output information
                    fd.ReadDataFromAlexDatabaseForVariable("VariablesTable", buildingPath);//Pulls variable information
                    //==We only need variable data...
                    for (int i = 0; i < fd.variableFromT3000List.Count; i++)
                    {
                        //if(controllerNumberValue ==int.Parse(fd.variableFromT3000List[i].var_panel) && (variableNumberValue -1)== int.Parse(fd.variableFromT3000List[i].var_index))
                        if (controllerNumberValue == int.Parse(fd.variableFromT3000List[i].var_panel) && (variableNumberValue - 1) == int.Parse(fd.variableFromT3000List[i].var_index))
                        {
                            //==We are ok value is present now we need to check for unit
                            if (fd.variableFromT3000List[i].var_units == "Deg.C")
                            {
                                //We can accept this value 

                                //--Now steps 
                                /*
                                 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                 1. Retrive data from device
                                 2. Update data in database
                                 3. Refresh Controller
                                 */
                                // double returnValue = dpp.InputDeviceInfoAndValue()

                                fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controllerNumberValue);//We have data now.

                                if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                {
                                    //==We have the values now we need to retrive the data

                                    //double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue-1);
                                    double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), VariableIdentifiedValue, variableNumberValue);
                                    if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                    {
                                        MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                        return;//we can break form loop as well
                                    }

                                    //==Now updating data in database
                                    //------------------------------------Working here -------------------------------------------------------//
                                    //UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue-1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_VALUE");
                                    UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableNumberValue).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_VALUE");
                                    //------------------------------------End of working here------------------------------------------------//
                                    CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);

                                }

                                // break;

                            }
                            else
                            {
                                //Invalid input
                                MessageBox.Show("This is not a temperature parameter!");
                                return;
                            }
                            break; //We found the value so break from the loop
                        }

                    }



                }
                else
                {
                    //We have invalid input
                    return;
                }



            }
            if (condition != true)
            {
                //We need to check if the input is for labels
                //--If condition is true we do not need to check for labels 
                /*
               steps : 1.  Compare for data extracted from database for all level values
               2.Find controller information.
               3. Retrive data from hardware
               4. Update data
               5. Refresh the controll this will be done finally               
               */

                //Retrive data for comparision
                // MessageBox.Show("condition false start");
                // PullDataForAllPossibleInputsFromT3000();
                FilterDataFromT3000 fd = new FilterDataFromT3000();
                Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                int compareValue = CompareLabelInformation(DeviceIdentificationKeyWord, fd, ft);

                // MessageBox.Show("Compare lebel return value = "+ compareValue);


                if (compareValue == 0)
                {
                    return;//No futher processing no data present

                }
                else if (compareValue == 1)// Input value is matched
                {

                    if (listInputValueAfterFilter.Count > 0)
                    {

                        int controller_PanelNumber = int.Parse(listInputValueAfterFilter[0].PanelID);
                        int variableIndex = int.Parse(listInputValueAfterFilter[0].InputIndex);

                        //We have input section.
                        //--We need to do filtering accordingly
                        //FilterDataFromT3000 fd = new FilterDataFromT3000();
                        //Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                        //DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                        ft.FindPathOfBuildingDB();
                        string buildingPath = ft.BuildingSelected[0].Building_Path;
                        //==We only need variable data...
                        for (int i = 0; i < fd.InputFromT3000List.Count; i++)
                        {
                            //--We dont need to do varibleIndex -1 because this is being called from the table itself
                            //if (controller_PanelNumber == int.Parse(fd.InputFromT3000List[i].PanelID) && (variableIndex-1) == int.Parse(fd.InputFromT3000List[i].InputIndex))
                            if (controller_PanelNumber == int.Parse(fd.InputFromT3000List[i].PanelID) && (variableIndex) == int.Parse(fd.InputFromT3000List[i].InputIndex))
                            {
                                //==We are ok value is present now we need to check for unit
                                if (fd.InputFromT3000List[i].InputUnit == "Deg.C")
                                {
                                    //We can accept this value 

                                    //--Now steps 
                                    /*
                                     * 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                     1. Retrive data from device
                                     2. Update data in database
                                     3. Refresh Controller
                                     */
                                    // double returnValue = dpp.InputDeviceInfoAndValue()

                                    fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controller_PanelNumber);//We have data now.

                                    if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                    {
                                        //==We have the values now we need to retrive the data

                                        double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), "IN", variableIndex + 1);//this variableIndex+1 is done because yabee starts form 1 and t3000 from 0
                                        if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                        {
                                            MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                            return;//we can break form loop as well
                                        }
                                        //==Now updating data in database
                                        //------------------------------------Working here -------------------------------------------------------//
                                        UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableIndex + 1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_INPUT");
                                        //------------------------------------End of working here------------------------------------------------//
                                        CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);
                                    }
                                }
                                else
                                {
                                    //Invalid input
                                    MessageBox.Show("This is not a temperature parameter!");
                                    return;
                                }
                                break; //We found the value so break from the loop
                            }

                        }

                    }//Close of input.Count


                    // MessageBox.Show("compate 1 finish");
                }
                else if (compareValue == 2)// Output value is matched
                {
                    if (listOutputValueAfterFilter.Count > 0)
                    {

                        int controller_PanelNumber = int.Parse(listOutputValueAfterFilter[0].out_panel);
                        int variableIndex = int.Parse(listOutputValueAfterFilter[0].out_index);

                        //We have input section.
                        //We have variable section.
                        //--We need to do filtering accordingly
                        //FilterDataFromT3000 fd = new FilterDataFromT3000();
                        //Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                        //DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                        ft.FindPathOfBuildingDB();
                        string buildingPath = ft.BuildingSelected[0].Building_Path;
                        // fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath); //Pulls the input information
                        // fd.ReadDataFromAlexDatabaseForOutput("OUTPUTable", buildingPath);//pulls the output information
                        // fd.ReadDataFromAlexDatabaseForInput("INPUTable", buildingPath);//Pulls variable information
                        //==We only need variable data...
                        for (int i = 0; i < fd.outputFromT3000List.Count; i++)
                        {

                            //if (controller_PanelNumber == int.Parse(fd.outputFromT3000List[i].out_panel) && (variableIndex-1) == int.Parse(fd.outputFromT3000List[i].out_index))
                            if (controller_PanelNumber == int.Parse(fd.outputFromT3000List[i].out_panel) && (variableIndex) == int.Parse(fd.outputFromT3000List[i].out_index))
                            {
                                //==We are ok value is present now we need to check for unit
                                if (fd.outputFromT3000List[i].out_units == "Deg.C")
                                {
                                    //We can accept this value 


                                    //--Now steps 
                                    /*
                                     * 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                     1. Retrive data from device
                                     2. Update data in database
                                     3. Refresh Controller
                                     */
                                    // double returnValue = dpp.InputDeviceInfoAndValue()

                                    fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controller_PanelNumber);//We have data now.

                                    if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                    {
                                        //==We have the values now we need to retrive the data

                                        double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), "OUT", variableIndex + 1);
                                        if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                        {
                                            MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                            return;//we can break form loop as well
                                        }
                                        //==Now updating data in database
                                        //------------------------------------Working here -------------------------------------------------------//
                                        UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableIndex + 1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_INPUT");
                                        //------------------------------------End of working here------------------------------------------------//
                                        CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);
                                    }
                                }
                                else
                                {
                                    //Invalid input
                                    MessageBox.Show("This is not a temperature parameter!");
                                    return;
                                }
                                break; //We found the value so break from the loop
                            }

                        }

                    }//Close of input.Count
                    //MessageBox.Show("compate 2 finish");
                }
                else if (compareValue == 3)// Variable value is matched
                {
                    if (listVariableValueAfterFilter.Count > 0)
                    {

                        int controller_PanelNumber = int.Parse(listVariableValueAfterFilter[0].var_panel);
                        int variableIndex = int.Parse(listVariableValueAfterFilter[0].var_index);

                        //We have input section.
                        //We have variable section.
                        //--We need to do filtering accordingly
                        //FilterDataFromT3000 fd = new FilterDataFromT3000();
                        //Form_TemperatureSourceDeviceInput ft = new Form_TemperatureSourceDeviceInput(this);
                        //DeviceAndParameterProcess dpp = new DeviceAndParameterProcess();
                        ft.FindPathOfBuildingDB();
                        string buildingPath = ft.BuildingSelected[0].Building_Path;
                        //==We only need variable data...
                        for (int i = 0; i < fd.variableFromT3000List.Count; i++)
                        {
                            //if (controller_PanelNumber == int.Parse(fd.variableFromT3000List[i].var_panel) && (variableIndex-1) == int.Parse(fd.variableFromT3000List[i].var_index))
                            if (controller_PanelNumber == int.Parse(fd.variableFromT3000List[i].var_panel) && (variableIndex) == int.Parse(fd.variableFromT3000List[i].var_index))
                            {
                                //==We are ok value is present now we need to check for unit
                                if (fd.variableFromT3000List[i].var_units == "Deg.C")
                                {
                                    //We can accept this value 


                                    //--Now steps 
                                    /*
                                     * 0. Find controller instance id from alex db //we currently have panel_number which is not instance id
                                     1. Retrive data from device
                                     2. Update data in database
                                     3. Refresh Controller
                                     */

                                    fd.ReadALL_NODE_Information_For_Given_Panel("ALL_NODE", buildingPath, controller_PanelNumber);//We have data now.

                                    if (fd.listForControllerInfoFromALL_NODE_Table.Count > 0)
                                    {
                                        //==We have the values now we need to retrive the data
                                        double returnValueTemperature = dpp.InputDeviceInfoAndValue(int.Parse(fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance), "VAR", variableIndex + 1);
                                        if (returnValueTemperature <= 0.0000001 && returnValueTemperature > 50)
                                        {
                                            MessageBox.Show("Temperature value can be less than 0 and greater than 50 Degree Celsius");
                                            return;//we can break form loop as well
                                        }
                                        //==Now updating data in database
                                        //------------------------------------Working here -------------------------------------------------------//
                                        UpdateNodeInfoForTemperatureDeviceSelection_ForTextInput_ForDevice(tempNodeID, fd.listForControllerInfoFromALL_NODE_Table[0].Object_Instance, "", (variableIndex + 1).ToString(), "temp", returnValueTemperature.ToString(), "OBJECT_ANALOG_INPUT");
                                        //------------------------------------End of working here------------------------------------------------//
                                        CallFromTemperatureAndHumidtyFormForMixNodeFxn(tempNodeID);//This updates the DGV as well no problem
                                    }
                                }
                                else
                                {
                                    //Invalid input
                                    MessageBox.Show("This is not a temperature parameter!");
                                    return;
                                }
                                break; //We found the value so break from the loop
                            }

                        }

                    }//Close of input.Count
                     // MessageBox.Show("compate3 finish");
                } //Close ofcomp ==3



            }

            //--Here we will update the node info values in the database
            // MessageBox.Show("updating start");
            bcs.UpdateOrInsertTemperatureSourceInfo(tempNodeID, DeviceIdentificationKeyWord);

            //LoadNodeAndLine();//--This is called after completion of this function so 

        }//Close of temperatureInputPorcess... fxn

        int controllerNumber;
        string variableIdentifier; int variableNumber;
        bool outputCondtion = false;
        /// <summary>
        ///function helps to filter the input and check if it contains the controller information. 
        /// eg. 1-VAR1 , 2-VAR2 etc
        /// /// </summary>
        /// <param name="input"></param>
        /// <returns>True if formate is correct</returns>
        public bool processInputData(string input)
        {
            string captureString = input;//DeviceIdentificationKeyWord;
            if (captureString.Contains("-"))
            {
                // var count =;
                if (captureString.Count(x => x == '-') != 1)
                {
                    //--Contains more then one - so discard
                    outputCondtion = false;
                    return outputCondtion;
                }
                else
                {

                    string[] parts = captureString.Split('-');
                    if (parts != null)
                    {
                        int idpart = Convert.ToInt32(parts[0]);
                        string secondPart = parts[1];

                        //--Testing first part , first part should be greater than 0 so...
                        if (idpart > 0 && idpart <= 1000)
                        {
                            //--Then okie 
                        }
                        else
                        {
                            outputCondtion = false;
                            return outputCondtion;
                        }

                        //--Now testing second part... for var name and input name
                        //--Seprating number and letters
                        Regex re = new Regex(@"([a-zA-Z]+)(\d+)");
                        Match result = re.Match(captureString);
                        string alphaPart = result.Groups[1].Value;
                        int numberPart = int.Parse(result.Groups[2].Value);

                        if (numberPart <= 0 || numberPart >= 1000)
                        {
                            outputCondtion = false;
                            return outputCondtion;//False input
                        }
                        if (alphaPart == "VAR" || alphaPart == "IN" || alphaPart == "OUT")
                        {
                            //--OKIE
                        }
                        else
                        {
                            outputCondtion = false;
                            return outputCondtion;
                        }
                        controllerNumber = idpart;
                        variableIdentifier = alphaPart;
                        variableNumber = numberPart;
                    }
                    else
                    {
                        //We can not desipher
                        outputCondtion = false;
                        return outputCondtion;
                    }

                }//close of else

                outputCondtion = true;
                return outputCondtion;
            }
            else
            {
                /*
                //--Seprating number and letters
                Regex re = new Regex(@"([a-zA-Z]+)(\d+)");
                Match result = re.Match(captureString);
                string alphaPart = result.Groups[1].Value;
                int numberPart = int.Parse(result.Groups[2].Value);

                if (numberPart <= 0 || numberPart >= 1000)
                {
                    outputCondtion = false;
                    return outputCondtion ;//False input
                }
                if (alphaPart == "VAR" || alphaPart == "IN" || alphaPart == "OUT")
                {
                    //--OKIE
                }
                else
                {
                    outputCondtion = false;
                    return outputCondtion;
                }

                controllerNumber = 1;
                variableIdentifier = alphaPart;
                variableNumber = numberPart;
                */
            }


            outputCondtion = false;
            return outputCondtion;
        }

        //===========================End of temperature souce pull====================//

    }
}
