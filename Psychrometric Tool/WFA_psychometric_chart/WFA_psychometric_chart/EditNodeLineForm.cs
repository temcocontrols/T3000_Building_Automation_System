using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using Excel = Microsoft.Office.Interop.Excel;

namespace WFA_psychometric_chart
{
    public partial class EditNodeLineForm : Form
    {
        public Form1_main bcs;
        public EditNodeLineForm(Form1_main b)
        {
            this.bcs = b;
            InitializeComponent();
            dataGridView1.EditingControlShowing += new DataGridViewEditingControlShowingEventHandler(dataGridView1_EditingControlShowing);
        }


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
                if (dataGridView1.CurrentCell.RowIndex < bcs.menuStripNodeInfoValues.Count)
                {

                    ComboBox combo = e.Control as ComboBox;

                    if (combo != null)
                    {
                        //combo.SelectedIndexChanged -= new EventHandler(ComboBox_SelectedIndexChanged);
                        //combo.SelectedIndexChanged += new EventHandler(ComboBox_SelectedIndexChanged);
                        //SelectionChangeCommitted
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
            if (sender == null || e==null)
            {
                return;//If cb is null then empty
            }
            ComboBox cb = (ComboBox)sender;
           
            if(cb.Text == null)
            {
                return;//Handling null refrence exception
            }
            string item =Convert.ToString(cb.Text);

            if(item == "" || item == null)
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

            if(item == "Manual")
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

                if(cbx.ColumnIndex == 1)
                {
                    //--TEMPERATURE SOURCE IS CHANGED TO Manual
                    // string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                    // string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                    // string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                    // string sourceVal = "Manual";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                    // double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                    // //double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    // double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    // bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                    // double yVal = bcs.y_coord_value;
                    // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    ////  MessageBox.Show("source  = " + sourceVal);
                    // Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                    // int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
                    // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

                    string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
                    string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
                    string temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    string humidity_Souce = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
                    double xVal = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value));
                    // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    double humidity = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));
                    bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                    double yVal = bcs.y_coord_value;

                    // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                    int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
                    int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

                    // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                    updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);



                }
                else if(cbx.ColumnIndex == 4)
                {
                    //Humidity source is chosen to be Manual

                    string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
                    string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
                    string temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                    string humidity_Souce = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
                    double xVal = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value));
                    // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                    double humidity = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));
                    bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                    double yVal = bcs.y_coord_value;

                    // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    Color colorVal =dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                    int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
                    int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

                    // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                    updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);
      
                }
            }
            else if(item == "Web")
            {
                if(item == "" || item == null)
                {
                    return;
                }
                //MessageBox.Show("Web select gariyo ");
                //=We need to select chose the web value and then update the values...
                if(dataGridView1.CurrentCell.Value== null)
                {
                    return;
                }

                DataGridViewComboBoxCell cbx = (DataGridViewComboBoxCell)dataGridView1.CurrentCell;
                cbx.Value = cb.Items[1];
               

               // WebUpdateSelection(); 
             if(cbx.ColumnIndex == 1)
                {
                    WebUpdateSelectionForTemperature();
                }    
             else if(cbx.ColumnIndex == 4)
                {
                    WebUpdateSelectionForHumidity();
                }

                //RefreshDataFromDBAndChart();
                RefreshChartAndDGVForMixNodeFunction();
                //this.Refresh();

                //dataGridView1.Refresh();
                ////--Now Refresh Data...
                //RefreshDataFromDBAndChart();
                ////--After refreshing
                //LoadNodeAndLine();//--Loading the data 

                //RefreshChartAndDGVForMixNodeFunction();
                // dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
                // dataGridView1.EditMode(false);//This one is for setting form edit mode to normal mode
                // Put the cells in edit mode when user enters them.
                dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
                dataGridView1.EndEdit();

            }
            else if(item == "Device")
            {
                if (dataGridView1.CurrentCell.RowIndex < bcs.menuStripNodeInfoValues.Count)
                {
                    //MessageBox.Show("column index = " + dataGridView1.CurrentCell.ColumnIndex + ",row index" + dataGridView1.CurrentCell.RowIndex);

                    //--This sometimes gives error why not take the id form menustripnodeinfovalue
                    tempNodeID = bcs.menuStripNodeInfoValues[dataGridView1.CurrentCell.RowIndex].id;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();

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
                      if(dataGridView1.CurrentCell.ColumnIndex == 1)
                        {
                            //Temperature is selected 
                            Form_TemperatureSourceDeviceInput ftemp = new Form_TemperatureSourceDeviceInput(this);
                            ftemp.ShowDialog();
                        }  
                      else if(dataGridView1.CurrentCell.ColumnIndex == 4)
                        {
                            //Humidity is selected

                            Form_HumiditySourceDeviceInput ftemp = new Form_HumiditySourceDeviceInput(this);
                            ftemp.ShowDialog();


                        }


                    }
                    catch(ArgumentNullException ex)
                    {
                        MessageBox.Show("exception:"+ ex.Message);
                    }
                }

            }

            else if (item == "Name")
            {
               // MessageBox.Show("name select gariyo ");

                

                //--Once the name is changed it has to save the changes
                string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value);
                string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
                string labelVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
                string sourceVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);//--This contains the name
                double xVal =double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value));
                //double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                double humidity = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));
                bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                double yVal = bcs.y_coord_value;
                string showTextVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value);
               // MessageBox.Show("show text = " + showTextVal);
                Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

                UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

            }

            else if (item == "Label")
            {
               // MessageBox.Show("Label select gariyo ");
                string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                //double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                double yVal = bcs.y_coord_value;

                string showTextVal = cb.Text; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
               // MessageBox.Show("show text = " + showTextVal + ",item = " + cb.Text);
                Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
                

                UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);


            }

            else if (item == "Source")
            {
               // MessageBox.Show("Source select gariyo ");
                string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                double yVal = bcs.y_coord_value;

                string showTextVal = cb.Text;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
               // MessageBox.Show("show text = " + showTextVal);
                Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

            }


        }


        private void button2_Click(object sender, EventArgs e)
        {
            //this.Close();
        }

        private void EditNodeLineForm_Load(object sender, EventArgs e)
        {
            //This on load we need to laod the data from the other form
           LoadNodeAndLine();//load values
            if(bcs.flagShow == 1)
            {
                //Means the data is on so
                checkBox1.Checked = true;
            }
            else
            {
                checkBox1.Checked = false;
            }
            //dataGridView1.Rows[bcs.menuStripNodeInfoValues.Count].Visible = false;
            dataGridView1.AllowUserToAddRows = false;
            dataGridView2.AllowUserToAddRows = false;
            //--This one is for speical characters print in process energy calculation
            lb_unit_degCelcius.Text = "\x00B0 C";
            lb_unit_vol_flow_rate.Text = "m\xB3/s";
            lb_unit_sp_vol.Text = "m\xB3/Kg";

            if (dataGridView2.Rows.Count > 0)  //If there is data then only do this one
            {
                //set parameters of your event args
                var eventArgs = new DataGridViewCellEventArgs(1, 0);
                // or setting the selected cells manually before executing the function
                dataGridView2.Rows[0].Cells[1].Selected = true;
                dataGridView2_CellClick(sender, eventArgs);
            }

            //--This setting is for comfortzone 
            //==========================HERE COMFORTZONE LOAD=============================//
            
            //on load it need to pull the data form database and it need to reset the values as well
            groupBox2.Enabled = false;
            groupBox2.Visible = false;
            //this.Height = 280;
            //pull data form database.. first            
            bcs.PullComfortZoneData();//This will load the listComfortZoneDetail
            btn_color.BackColor = Color.LightGreen;
            //Now lets load the values int combobox1.items.add()
            comboBox1.Items.Clear();
            foreach (var item in bcs.listComfortZoneDetail)
            {
                comboBox1.Items.Add(item.name);
            }
                                                   
         //================================END OF COMFORT ZONE SETTING=========================//


        }





        //==THis flag is for omitting of calling of the double event while clicking in the web section

        bool flagForWeb = false;


        public void WebUpdateSelection()
        {
            string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value);
            string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
            string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            string sourceVal = "Web";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
            double xVal = 0.00;
            double yVal = 0.00;
            string showTextVal =dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

            //Now we need to identify the web values
            //==Web value calculation
            double latitudeVal = bcs.selectedBuildingList[0].latitude;
            double longitudeVal = bcs.selectedBuildingList[0].longitude;
            //Pulling data form web it will always be in temp and humidity form
            bcs.GetDataFromWeb(latitudeVal, longitudeVal);

            double temperature_Val = bcs.temp_pulled_from_web;
            double humidity_Val = double.Parse(bcs.hum_pulled);
            double y_value = bcs.CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);

            xVal = temperature_Val;
            yVal = y_value;
          
            //MessageBox.Show("sourcevalue  =  "+sourceVal+ "\ntemp / temp_pulled_form_web = "+temperature_Val+"\nhum = "+humidity_Val+"\ny value web= " + y_value+"\nshowtext"+showTextVal);
            //Now lets update the values in db
            //bcs.UpdateNodeInfoToDB(nodeIDVal, temperature_Val, y_value, sourceVal, nameVal, labelVal, colorVal, showTextVal,nodeSizeVal);
            // RefreshDataFromDBAndChart();
            // //--After refreshing
            // LoadNodeAndLine();//--Loading the data 
            UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

           
            flagForWeb = true;

        }


        public void WebUpdateSelectionForHumidity()
        {


            /*
            
              string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                        // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                        double yVal = bcs.y_coord_value;

                       // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);
            
            */



            //double xVal = 0.00;
            double yVal = 0.00;
            //Now we need to identify the web values
            //==Web value calculation
            double latitudeVal = bcs.selectedBuildingList[0].latitude;
            double longitudeVal = bcs.selectedBuildingList[0].longitude;
            //Pulling data form web it will always be in temp and humidity form
            bcs.GetDataFromWeb(latitudeVal, longitudeVal);

            double temperature_Val = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value)); // bcs.temp_pulled_from_web;
            double humidity_Val = double.Parse(bcs.hum_pulled);
            double y_value = bcs.CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);

           // xVal = temperature_Val;
            yVal = y_value;

            string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
            string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
            string temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
            string humidity_Souce = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
            double xVal = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value));
            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
           // double yVal = y_value; //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;

            int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
            int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);

            //==For mix node update
            bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

           // RefreshChartAndDGVForMixNodeFunction();



            flagForWeb = true;

        }


        /// <summary>
        /// This one update the node info value to db
        /// </summary>
        public void WebUpdateSelectionForTemperature()
        {
            double xVal = 0.00;
           // double yVal = 0.00;
            //Now we need to identify the web values
            //==Web value calculation
            double latitudeVal = bcs.selectedBuildingList[0].latitude;
            double longitudeVal = bcs.selectedBuildingList[0].longitude;
            //Pulling data form web it will always be in temp and humidity form
            bcs.GetDataFromWeb(latitudeVal, longitudeVal);

            double temperature_Val = bcs.temp_pulled_from_web;
            double humidity_Val = double.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value));//double.Parse(bcs.hum_pulled);
            double y_value = bcs.CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);

             xVal = temperature_Val;
            //yVal = y_value;

            string nodeIDVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value);
            string nameVal = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value);//--This contains the name
            string temperature_Source = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value);//--This contains the name
            string humidity_Souce = Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value);//--This contains the name
                                                                                                                     // double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
            double yVal = y_value; //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
            int nodeSizeVal = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value));
            int AirFlow = int.Parse(Convert.ToString(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value));

            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);


            //==For mix node update
            bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

            //RefreshChartAndDGVForMixNodeFunction();

            flagForWeb = true;

        }









        public void UpdateDataValueAndRefreshDGV(string NodeID,double xVal,double yVal,string source,string name,string label,Color color,string showtext,int nodesize)
        {
            string nodeIDVal = NodeID;
            double temperature_Val = xVal;
            double y_value = yVal;
            string sourceVal = source;
            string nameVal = name;
            string labelVal = label;
            Color colorVal = color;
            string showTextVal = showtext;
            int nodeSizeVal = nodesize;

            if (flagForWeb == true)
            {
                flagForWeb = false;//Dissabling theflag
                return;
            }
            

           // MessageBox.Show("Bbk before update to db source = " + sourceVal+", yvalue = "+y_value+"xvalue = "+temperature_Val +",name = "+nameVal);

            bcs.UpdateNodeInfoToDB(nodeIDVal, temperature_Val, y_value, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 
            
        }




        public void updateNodeInfoBasedOnPresentValue(string NodeID, double xVal, double yVal, string temperature_source, string humidity_source, string name, Color color,  int nodesize,int airFlow1)
        {
            string nodeIDVal = NodeID;
            string temperatureSource = temperature_source;
            string humiditySouce = humidity_source;
            double temperature_Val = xVal;
            double y_value = yVal;            
            string nameVal = name;            
            Color colorVal = color;           
            int nodeSizeVal = nodesize;
            int airFlow = airFlow1;
            string buildingName =bcs.selectedBuildingList[0].BuildingName;

            if (flagForWeb == true)
            {
                flagForWeb = false;//Dissabling theflag
                return;
            }

            DatabaseOperationsForEditNodeClass ObjDBClass = new DatabaseOperationsForEditNodeClass();
            ObjDBClass.UpdateDataForNodeInfoInEditNodeSection(buildingName, nodeIDVal, temperature_Val, y_value, temperatureSource, humiditySouce, nameVal, colorVal, nodeSizeVal,airFlow);

            //--Now Refresh Data...
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 




        }



        /// <summary>
        /// This function helps to update the device information of a node and also update the node value 
        /// </summary>
        /// <param name="nodeID">node id</param>
        /// <param name="deviceInstancID">device instance id </param>
        /// <param name="IP">IP value</param>
        /// <param name="param1id">param1 id</param>
        /// <param name="param2id">param2 id</param>
        /// <param name="param1info">param1info eg. temp</param>
        /// <param name="param2info">param2 info eg. hum/enthalpy</param>
        /// <param name="param1_id_type">eg. OBJECT_ANALOG_INPUT</param>
        /// <param name="param2_id_type">eg. OBJECT_ANALOG_INPUT,</param>
        public void UpdateNodeInfoForDeviceSelection(string nodeID,string deviceInstancID,string IP,string param1id,string param2id,string param1info,string param2info,string param1Value,string param2Value,string param1_id_type,string param2_id_type)
        {

            //==We require following things : 
            /*
            1. name ,label,source,xValue,yValue,showText,color,nodeSize== for updating node
            2. nodeID,deviceInstanceID,deviceIP, param1[id,value,type],param2[id,value,type] ==>insert if first time else update the values
            */
            string nodeIDVal = nodeID; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();            
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
            double xVal = 0.00;
            double yVal = 0.00;
            string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
            int nodeSizeVal =int.Parse( dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());


            //This is where the actual action is going to happen
            //First lets find the xValue and the yValue of the selected index value
            if (param1info == "temp" && param2info == "hum")
            {
                //We got temperature and humidity

                bcs.CalculateYFromXandHumidity(double.Parse(param1Value),double.Parse(param2Value) / 100);//This will get the y value

                xVal = int.Parse(param1Value);
                yVal = bcs.y_coord_value;//This value will be updated
            }
            else
            {
                //We get temp and enthaly

                xVal = double.Parse(param1Value);
                yVal = bcs.CalculateYFromXandEnthalpy(xVal, double.Parse(param2Value));



            }

           // MessageBox.Show("bbk test , nodeIDValue= " + nodeIDVal);
            //tHIS FXN will update to db
            bcs.UpdateNodeInfoToDB(nodeIDVal,xVal, yVal, sourceVal, nameVal, labelVal, colorVal,showTextVal, nodeSizeVal);
            //--Now since the value has been updated now we again want to reload the values to both the tables.
            //insert or update the node information if already present
            bcs.InsertIntoDeviceInfoDB(nodeIDVal, deviceInstancID, IP, param1id, param2id, param1info, param2info, param1_id_type, param2_id_type);


            //--Refreshing data
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 

        }

       /// <summary>
       /// fUNCTION WILL HELP IN Updating node info value for temperature form
       /// and inserts to db
       /// </summary>
       /// <param name="nodeID"></param>
       /// <param name="deviceInstancID"></param>
       /// <param name="IP"></param>
       /// <param name="param1id"></param>
       /// <param name="param2id"></param>
       /// <param name="param1info"></param>
       /// <param name="param2info"></param>
       /// <param name="param1Value"></param>
       /// <param name="param2Value"></param>
       /// <param name="param1_id_type"></param>
       /// <param name="param2_id_type"></param>
        public void UpdateNodeInfoForTemperatureDeviceSelection(string nodeID, string Temp_deviceInstancID, string Temp_IP,string Temp_param1id, string Temp_param1info, string Temp_param1Value,  string Temp_param1_id_type)
        {

            //==We require following things : 
            /*
            1. name ,label,source,xValue,yValue,showText,color,nodeSize== for updating node
            2. nodeID,deviceInstanceID,deviceIP, param1[id,value,type],param2[id,value,type] ==>insert if first time else update the values
            */
            string nodeIDVal = nodeID; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();            
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            string TemperatureSource = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            //----Temperature Value will be received for parameter value from hardware
            string TemperatureValue = Temp_param1Value;  //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
            string HumiditySource = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
            string HumidityValue = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString();//--This contains the name
            //string TemperatureSource = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            
            double xVal = 0.00;
            double yVal = 0.00;

            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());

            double humValue = double.Parse(HumidityValue);//Now we have humidity value as well 

            //This is where the actual action is going to happen
            //First lets find the xValue and the yValue of the selected index value
           // if (param1info == "temp" && param2info == "hum")
           // {
           //--Now lets calculate the x and y value
                //We got temperature and humidity

                bcs.CalculateYFromXandHumidity(double.Parse(TemperatureValue), humValue / 100);//This will get the y value

                xVal = (int)double.Parse(TemperatureValue); //int.Parse(TemperatureValue);
            yVal = bcs.y_coord_value;//This value will be updated
            //}
            //else
            //{
            //    //We get temp and enthaly

            //    xVal = double.Parse(param1Value);
            //    yVal = bcs.CalculateYFromXandEnthalpy(xVal, double.Parse(param2Value));

            //}

            // MessageBox.Show("bbk test , nodeIDValue= " + nodeIDVal);
            //tHIS FXN will update to db
            //--------------------This one is also commented latere--------------------------------//
            //bcs.UpdateNodeInfoToDB(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
            bcs.UpdateNodeInfoToDBFromTemperatureDeviceSource(nodeID, xVal, yVal, TemperatureSource, HumiditySource, nameVal, colorVal, AirFlow, nodeSizeVal);
            //----------------------lets rememeber this--------------------------------------------//
            //--Now since the value has been updated now we again want to reload the values to both the tables.
            //insert or update the node information if already present

            //----------------------------This one is comented now ---------------------------------------//
            //bcs.InsertIntoDeviceInfoDB(nodeIDVal, deviceInstancID, IP, param1id, param2id, param1info, param2info, param1_id_type, param2_id_type);
            bcs.InsertUpdateFromTemperatureDeviceSelectionToDBOnlyTempertureUpdate(nodeID, Temp_deviceInstancID, Temp_IP, Temp_param1id, Temp_param1info, Temp_param1_id_type);
           //------------------------------end this------------------------------------------------------//

           //--Refreshing data
           RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 

        }




        public void UpdateNodeInfoForHumidityDeviceSelection(string nodeID, string Humidity_deviceInstancID, string Humidity_IP, string Humidity_param1id, string Humidity_param1info, string Humidity_param1Value, string Humidity_param1_id_type)
        {

            //==We require following things : 
            /*
            1. name ,label,source,xValue,yValue,showText,color,nodeSize== for updating node
            2. nodeID,deviceInstanceID,deviceIP, param1[id,value,type],param2[id,value,type] ==>insert if first time else update the values
            */
            string nodeIDVal = nodeID; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();            
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            string TemperatureSource = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            //----Temperature Value will be received for parameter value from hardware
            string TemperatureValue = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();//--This contains the name
            string HumiditySource = "Device";//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
            string HumidityValue = Humidity_param1Value; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString();//--This contains the name
                                                                                                                    //string TemperatureSource = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name

            double xVal = 0.00;
            double yVal = 0.00;

            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());

            double humValue = double.Parse(HumidityValue);//Now we have humidity value as well 

            //This is where the actual action is going to happen
            //First lets find the xValue and the yValue of the selected index value
            // if (param1info == "temp" && param2info == "hum")
            // {
            //--Now lets calculate the x and y value
            //We got temperature and humidity

            bcs.CalculateYFromXandHumidity(double.Parse(TemperatureValue), humValue / 100);//This will get the y value
           // MessageBox.Show("Temperature Error print = " + TemperatureValue);

            xVal = (int)double.Parse(TemperatureValue);
            yVal = bcs.y_coord_value;//This value will be updated
            //}
            //else
            //{
            //    //We get temp and enthaly

            //    xVal = double.Parse(param1Value);
            //    yVal = bcs.CalculateYFromXandEnthalpy(xVal, double.Parse(param2Value));

            //}

            // MessageBox.Show("bbk test , nodeIDValue= " + nodeIDVal);
            //tHIS FXN will update to db
            //--------------------This one is also commented latere--------------------------------//
            //bcs.UpdateNodeInfoToDB(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
            bcs.UpdateNodeInfoToDBFromTemperatureDeviceSource(nodeID, xVal, yVal, TemperatureSource, HumiditySource, nameVal, colorVal, AirFlow, nodeSizeVal);
            //----------------------lets rememeber this--------------------------------------------//
            //--Now since the value has been updated now we again want to reload the values to both the tables.
            //insert or update the node information if already present

            //----------------------------This one is comented now ---------------------------------------//
            //bcs.InsertIntoDeviceInfoDB(nodeIDVal, deviceInstancID, IP, param1id, param2id, param1info, param2info, param1_id_type, param2_id_type);
           // bcs.InsertUpdateFromTemperatureDeviceSelectionToDBOnlyTempertureUpdate(nodeID, Temp_deviceInstancID, Temp_IP, Temp_param1id, Temp_param1info, Temp_param1_id_type);
            
            bcs.InsertUpdateFromHumidityDeviceSelectionToDBOnlyTempertureUpdate(nodeID, Humidity_deviceInstancID, Humidity_IP, Humidity_param1id, Humidity_param1info, Humidity_param1_id_type);
            //------------------------------end this------------------------------------------------------//

            //--Refreshing data
            //  RefreshDataFromDBAndChart();
            //--After refreshing
            // LoadNodeAndLine();//--Loading the data 
            RefreshChartAndDGVForMixNodeFunction();
        }



         public void RefreshChartAndDGVForMixNodeFunction()
        {
            //--Refreshing data
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 
        }









        public void RefreshDataFromDBAndChart()
        {
           bcs.LoadNodeAndLineFromDB(bcs.indexOfChartSelected);   //Lets make it passing the stirngs 

            // flagForInsertOrUpdateDataToDB = 1;
            //--This is also completed..
            // bcs.RefreshGraph();
            // bcs.Invalidate();
            bcs.chart1RefreshCustom();
            //chart1.Invalidate();
            bcs.ReDrawingLineAndNode();

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

            if (bcs.menuStripNodeInfoValues.Count > 0)
            {

                int indexForParticularRow = 0;

                for (int i = 0; i < bcs.menuStripNodeInfoValues.Count; i++)
                {
                    //Now lets display the values..
                    string temperature_value;
                    int temperature_indexValue = 0;
                    //Now identifying the item selected.
                    
                    //For temperature source 

                    if (bcs.menuStripNodeInfoValues[i].temperature_source == "Manual")
                    {
                        temperature_value = "Manual";
                        temperature_indexValue = 0;
                    }
                    else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Web")
                    {
                        temperature_value = "Web";
                        temperature_indexValue = 1;
                    }
                    else  if(bcs.menuStripNodeInfoValues[i].temperature_source == "Device")
                    {
                        temperature_value = "Device";
                        temperature_indexValue = 2;
                    }
                    else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Mix")
                    {
                        temperature_value = "Mix";
                        //temperature_indexValue = 2;//No need for this one
                    }





                    //==Now for source

                    string humidity_sourceValue = "";
                    int indexForSource = 0;
                    if (bcs.menuStripNodeInfoValues[i].humidity_source == "Manual")
                    {
                        humidity_sourceValue = "Manual";
                        indexForSource = 0;

                    }
                    else if (bcs.menuStripNodeInfoValues[i].humidity_source == "Web")
                    {
                        humidity_sourceValue = "Web";
                        indexForSource = 1;

                    }
                    else if (bcs.menuStripNodeInfoValues[i].humidity_source == "Device")
                    {
                        humidity_sourceValue = "Device";
                        indexForSource = 2;
                    }
                    else if (bcs.menuStripNodeInfoValues[i].humidity_source == "Mix")
                    {
                        humidity_sourceValue = "Mix";
                      //  indexForSource = 2;
                    }

                    // DataGridViewComboBoxColumn cb = (DataGridViewComboBoxColumn)showText;
                    //cb.Items[0].ToString();
                    // MessageBox.Show("source VALUE MEMBER= " + CB_DGV_Source.Items[indexForSource]);
                    bcs.enthalpyHumidityCalculatorForXYvalue(bcs.menuStripNodeInfoValues[i].xVal, bcs.menuStripNodeInfoValues[i].yVal);
                    if (bcs.menuStripNodeInfoValues[i].humidity_source == "Mix")
                    {
                        string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name,"Manual", bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(),"Manual", Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(), bcs.menuStripNodeInfoValues[i].airFlow.ToString() };
                        dataGridView1.Rows.Add(row);
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].ReadOnly = false;
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].ReadOnly = false;
                        //indexForParticularRow
                        dataGridView1.Rows[indexForParticularRow].Cells[1].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;                   
                        dataGridView1.Rows[indexForParticularRow].Cells[4].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[6].ReadOnly = true;
                        dataGridView1.Rows[indexForParticularRow].Cells[9].ReadOnly = true;

                    }
                    else { 

                        string[] row = new string[] { bcs.menuStripNodeInfoValues[i].name,  CB_DGV_Temperature_Source.Items[temperature_indexValue].ToString(), bcs.menuStripNodeInfoValues[i].id, Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(),CB_DGV_Humidity_Source.Items[indexForSource].ToString() ,Math.Round(bcs.humDataGridValue, 2).ToString(), Math.Round(bcs.enthalpyDataGridView, 2).ToString(), " ", bcs.menuStripNodeInfoValues[i].marker_Size.ToString(),bcs.menuStripNodeInfoValues[i].airFlow.ToString() };

                    //--This one is for enabling or dissabling a particualr row
                    if (bcs.menuStripNodeInfoValues[i].temperature_source == "Manual")
                    {
                        //sourceValue = "Manual";
                        //indexForSource = 0;

                        dataGridView1.Rows.Add(row);
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].ReadOnly = false;
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].ReadOnly = false;
                        //indexForParticularRow
                        dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = false;
                        // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;

                        //--This one is for humidity
                        if (bcs.menuStripNodeInfoValues[i].humidity_source == "Manual")
                        {
                            //sourceValue = "Manual";
                            //indexForSource = 0;                                                    
                            dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;


                        }
                        else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Web")
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
                    else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Web")
                    {
                        //sourceValue = "Web";
                        //indexForSource = 1;
                        dataGridView1.Rows.Add(row);
                        dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                        // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;

                        //--This one is for humidity
                        if (bcs.menuStripNodeInfoValues[i].humidity_source == "Manual")
                        {
                            dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;




                        }
                        else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Web")
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
                        dataGridView1.Rows.Add(row);
                        dataGridView1.Rows[indexForParticularRow].Cells[3].ReadOnly = true;
                        // dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = true;

                        //--This one is for humidity
                        if (bcs.menuStripNodeInfoValues[i].humidity_source == "Manual")
                        {                    
                            dataGridView1.Rows[indexForParticularRow].Cells[5].ReadOnly = false;


                        }
                        else if (bcs.menuStripNodeInfoValues[i].temperature_source == "Web")
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

                    //  dataGridView1.Rows.Add(row);

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[i].Cells[7];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.menuStripNodeInfoValues[i].colorValue;

                }
            }//clsoe of if value

            //--Second : This is for the line to show in the datagridview2
            /*
             Lets store the node1 and node2 parameters in variables.
             */
             //--variables to store info of two nodes
            string idNode1="", lastUpdatedDateNode1 = "", humiditySourceNode1 = "", temperatureSourceNode1 = "", nameNode1 = "";
            string idNode2 = "", lastUpdatedDateNode2 = "", humiditySourceNode2 = "", temperatureSourceNode2 = "", nameNode2 = "";
            double xValueNode1=0, yValueNode1=0;
            double xValueNode2=0, yValueNode2=0;
            int airFlowNode1=0, airFlowNode2=0;

            //--Scanning for the values
            if (bcs.menuStripNodeLineInfoValues.Count > 0)
            {
                for (int i = 0; i < bcs.menuStripNodeLineInfoValues.Count; i++)
                {

                    string startNodeName = "";
                    string endNodeName = "";
                    //Now lets calculate the startNodeName and endNodeName
                    for (int x = 0; x < bcs.menuStripNodeInfoValues.Count; x++)
                    {
                        if (bcs.menuStripNodeLineInfoValues[i].prevNodeId == bcs.menuStripNodeInfoValues[x].id)
                        {
                            nameNode1 = startNodeName = bcs.menuStripNodeInfoValues[x].name;
                            idNode1 = bcs.menuStripNodeInfoValues[x].id;
                            lastUpdatedDateNode1 = bcs.menuStripNodeInfoValues[x].lastUpdatedDate;
                            humiditySourceNode1 = bcs.menuStripNodeInfoValues[x].humidity_source;
                            temperatureSourceNode1 = bcs.menuStripNodeInfoValues[x].temperature_source;
                            //nameNode1 =
                            xValueNode1 = bcs.menuStripNodeInfoValues[x].xVal;
                            yValueNode1 = bcs.menuStripNodeInfoValues[x].yVal;
                            airFlowNode1 = (int)bcs.menuStripNodeInfoValues[x].airFlow;
                            break;
                        }

                    }

                    //--This one is for end nodename
                    for (int v = 0; v < bcs.menuStripNodeInfoValues.Count; v++)
                    {
                        if (bcs.menuStripNodeLineInfoValues[i].nextNodeId == bcs.menuStripNodeInfoValues[v].id)
                        {
                            nameNode2 = endNodeName = bcs.menuStripNodeInfoValues[v].name;
                            idNode2 = bcs.menuStripNodeInfoValues[v].id;
                            lastUpdatedDateNode2 = bcs.menuStripNodeInfoValues[v].lastUpdatedDate;
                            humiditySourceNode2 = bcs.menuStripNodeInfoValues[v].humidity_source;
                            temperatureSourceNode2 = bcs.menuStripNodeInfoValues[v].temperature_source;
                            //nameNode1 =
                            xValueNode2 = bcs.menuStripNodeInfoValues[v].xVal;
                            yValueNode2 = bcs.menuStripNodeInfoValues[v].yVal;
                            airFlowNode2 = (int)bcs.menuStripNodeInfoValues[v].airFlow;
                            break;
                        }

                    }

                    //Lets calculate some energy parameters such as DBT, Relative humidity,humidity ratio,,
                    //specific volume, mass flow rate,enthalpy, Total energy flow, and heat change.

                    //--Lets make a function which returns all the other values
                    EnergyParameterCalculationForTwoNodes(xValueNode1,yValueNode1,airFlowNode1,xValueNode2,yValueNode2,airFlowNode2);
                    //double relativeHumidity1, relativeHumidity2, spVol1, spVol2, massFlowRate1, massFlowRate2, enthalpy1, enthalpy2, totalEnergyFlow1, totalEnergyFlow2, heatChange;
                    //now lets display...                    
                    string[] row = new string[] { bcs.menuStripNodeLineInfoValues[i].ID, bcs.menuStripNodeLineInfoValues[i].name, startNodeName, endNodeName, "", bcs.menuStripNodeLineInfoValues[i].lineThickness.ToString(), bcs.menuStripNodeLineInfoValues[i].prevNodeId.ToString(), bcs.menuStripNodeLineInfoValues[i].nextNodeId.ToString(), bcs.menuStripNodeLineInfoValues[i].lineSeriesID.ToString(),"",Math.Round(xValueNode1,2).ToString(),Math.Round(relativeHumidity1,2).ToString(),Math.Round(yValueNode1,2).ToString(),Math.Round(spVol1,2).ToString(),Math.Round(massFlowRate1,2).ToString(),Math.Round(enthalpy1,2).ToString(),Math.Round(totalEnergyFlow1,2).ToString(), Math.Round(xValueNode2, 2).ToString(), Math.Round(relativeHumidity2, 2).ToString(), Math.Round(yValueNode2, 2).ToString(), Math.Round(spVol2, 2).ToString(), Math.Round(massFlowRate2, 2).ToString(), Math.Round(enthalpy2, 2).ToString(), Math.Round(totalEnergyFlow2, 2).ToString(),Math.Round(heatChangeForBoth, 2).ToString() };
                    dataGridView2.Rows.Add(row);

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[i].Cells[4];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.menuStripNodeLineInfoValues[i].lineColorValue;

                    try
                    {
                        DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[i].Cells[9];
                        if (bcs.menuStripNodeLineInfoValues[i].status == 1)
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


          //  dataGridView1.Enabled = true;

        }


        double relativeHumidity1, relativeHumidity2, spVol1, spVol2, massFlowRate1, massFlowRate2, enthalpy1, enthalpy2, totalEnergyFlow1, totalEnergyFlow2, heatChangeForBoth;

             
        public void EnergyParameterCalculationForTwoNodes(double node1_X_Value,double node1_Y_Value,int node1_air_flow, double node2_X_Value, double node2_Y_Value, int node2_air_flow)
        {

            bcs.CalculateHumidityEnthalpy(node1_X_Value, node1_Y_Value);
            relativeHumidity1 = Math.Round(bcs.humidityCalculated, 2);
            enthalpy1 = Math.Round(bcs.enthalpyCalculated, 2);
            spVol1 = bcs.SpecificVolumeReturn;
            //--This calculates the end humidity and the enthalpy values..
            bcs.CalculateHumidityEnthalpy(node2_X_Value, node2_Y_Value);
            relativeHumidity2 = Math.Round(bcs.humidityCalculated, 2);
            enthalpy2 = Math.Round(bcs.enthalpyCalculated, 2);
            spVol2 = bcs.SpecificVolumeReturn;

            double enthalpyChange = enthalpy2 - enthalpy1;
           
            //string SixthLine = "Specific Volume              " + "m\xB3/Kg             " + startSpecificVolume1 + "                    " + endSpecificVolume1;
            massFlowRate1 = node1_air_flow / spVol1;
            massFlowRate2 = node2_air_flow / spVol2;

            totalEnergyFlow1 = massFlowRate1 *enthalpy1;
            totalEnergyFlow2 = massFlowRate2 * enthalpy2;
            //string NinthLine = @"Total Enthalpy Flow         " + "KJ/s                " + Math.Round(totalEnthalpyFlow1, 2) + "                      " + Math.Round(totalEnthalpyFlow2, 2);
            heatChangeForBoth = totalEnergyFlow2 - totalEnergyFlow1;
           

        }




        public void SetNodeForWeb(string source, string name, string label, Color c1, string comboboxItemText1, int marker_size)
        {
            bcs.markerSize = marker_size; //This is for changing the marker size 
            bcs.tbSource = source;
            bcs.tbName = name;
            bcs.tbLabel = label;
            bcs.colorValue = c1;
            bcs.comboboxItemText = comboboxItemText1;

            //Temp and humidity value 
            double temp1Val = 0.000;
            double hum1Val = 0.00;
            //web choice is selected.
            string db_name = "tbl_building_location";
            bcs.PullLongitudeAndLatitude(db_name);

            //Now lets pass the value to weather api
            bcs.GetDataFromWeb(bcs.latValueFromDB, bcs.longValueFromDB);//This gets the temperature and humidity

            temp1Val = bcs.temp_pulled_from_web;
            hum1Val = double.Parse(bcs.hum_pulled);
             
            //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
            if ((hum1Val > 0 && hum1Val <= 100) && (temp1Val >= 0 && temp1Val <= 50))
            {
                //now lets plot the values....
               bcs.plot_by_DBT_HR_process_diagram((double)(int)temp1Val, (double)hum1Val / 100);
            }
            else
            {
                MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
            }                   
        }



        public void SetNodeForDeviceUsingTempAndHum(string source, string name, string label, Color c1, string comboboxItemText1, int marker_size, string deviceInstance, string IP, string param1ID, string param2ID, string param1_info, string param2_info, string param1_value, string param2_value, string object_identifier_type_param1, string object_identifier_type_param2)
        {
            bcs.markerSize = marker_size; //This is for changing the marker size 
            bcs.tbSource = source;
            bcs.tbName = name;
            bcs.tbLabel = label;
            bcs.colorValue = c1;
            bcs.comboboxItemText = comboboxItemText1;
            bcs.deviceInstanceValue = deviceInstance;
            bcs.deviceIP = IP;
            bcs.deviceParam1ID = param1ID;
            bcs.deviceParam2ID = param2ID;
            bcs.device_param1_info_for_node = param1_info;

            bcs.device_param2_info_for_node = param2_info;
            //MessageBox.Show("param2 infor=="+param2_info);

            //--These parameter identifiy which section the node belongs to ie analog_input, analog_output,analog_value 
            bcs.object_param1_identifier_type = object_identifier_type_param1;
            bcs.object_param2_identifier_type = object_identifier_type_param2;

            // medium_device_or_web_value = device_or_web;
            //Temp and humidity value 

            if (source == "Device")
            {

                if (param1_info == "temp" && param2_info == "hum")
                {
                    double temp1Val = 0.0000;
                    double hum1Val = 0.0000;
                    //Device choice is selected
                    //  ReadDataFromDevice();
                    temp1Val = double.Parse(param1_value);//we have tmep here//hardwareTemperatureRead;
                    hum1Val = double.Parse(param2_value);//we have humidity here//hardwareHumidityRead;

                    // MessageBox.Show("Temp = " + temp1Val + ",hum=" + hum1Val);
                    if (temp1Val == 0.0000 && hum1Val == 0.0000)
                    {
                        //if hardware is damaged or not pulling the temp and hum then we need to pull form the web
                        //We need to pull form the web..
                        //web choice is selected.
                        MessageBox.Show("Cound not get data from device. Pulling data from web");
                        string db_name = "tbl_building_location";
                        bcs.PullLongitudeAndLatitude(db_name);

                        //Now lets pass the value to weather api
                        bcs.GetDataFromWeb(bcs.latValueFromDB, bcs.longValueFromDB);//This gets the temperature and humidity
                        temp1Val = bcs.temp_pulled_from_web;
                        hum1Val = double.Parse(bcs.hum_pulled);

                    }

                    //Now lets plot the values
                    //now lets plot the values only when the humidity is <= 100 and temp >0  and < 50
                    if ((hum1Val > 0 && hum1Val <= 100) && (temp1Val >= 0 && temp1Val <= 50))
                    {
                        //now lets plot the values....
                        bcs.plot_by_DBT_HR_process_diagram((double)(int)temp1Val, (double)hum1Val / 100);

                    }
                    else
                    {
                        MessageBox.Show(Properties.Resources.Please_select_a_proper_region_);
                    }


                }//close of if value
                else if (param1_info == "temp" && param2_info == "enthalpy")
                {
                    //temperature and enthalpy
                    /*
                    Steps :1.get temp and enthalpy =>done
                           2. convert to x and y values => done
                           3. convert to x                      
                    */
                    // Tuple<int,int> x = plot_by_DBT_Enthalpy(double.Parse(param1_value),double.Parse(param2_value));
                    double x_value = double.Parse(param1_value);
                    double y_value = bcs.plot_by_DBT_Enthalpy(x_value, double.Parse(param2_value));
                    bcs.plot_on_graph_values_process_diagram(x_value, y_value);
                }                        
            }

        } //Close of setnode_forweb function


        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //dataGridView1.EndEdit();
            if(dataGridView1.CurrentCell.Value == null)
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
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeInfoValues.Count) && (e.ColumnIndex == 1))
            {
                if(sender == null || e== null)
                {
                    dataGridView1.EndEdit();
                    return;
                }
            }
            //-------For Humidity source
            else if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeInfoValues.Count) && (e.ColumnIndex == 4))
            {
                if (sender == null || e== null)
                {
                    dataGridView1.EndEdit();
                    return;
                }
            }
           
            try
            { 
            //--This one is new one
            //This part contains the cell click values
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeInfoValues.Count) && (e.ColumnIndex == 7))
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
                        string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                        // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                        double yVal = bcs.y_coord_value;

                       // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);
                    }
                    catch { }
                  }
            }

            }catch { }


        }

        private void dataGridView2_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && (e.ColumnIndex == 4))
            {
                if (colorDialog2.ShowDialog() == DialogResult.OK)
                {   

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[e.RowIndex].Cells[4];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = colorDialog2.Color;//System.Drawing.Color.Red;



                    //We need to update 
                    try

                    {
                       // string finalThickness = dataGridView2.CurrentCell.Value.ToString();

                        //Convert.ToString is for handling null event
                        string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value);
                        string prevNodeID = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value);
                        string nextNodeID = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value);
                        Color color = colorDialog2.Color;  //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[3].Style.BackColor;
                        int thickness = int.Parse(Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value));//finalThickness
                        Series s = new Series(Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value));
                        string lineName = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value);

                        int status = 0;//0 means dissable 1 means enabled

                        DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];

                        if (cbCell.Value.ToString() == "true")
                        {
                            status = 1;
                        }
                        else
                        {
                            status = 0;
                        }
                        LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s, lineName, status);

                    }
                    catch(Exception ex)
                    {
                       // MessageBox.Show("exception  at line :1391"+ex.Message);
                    }
                 
                }//Close of if
            }

           else if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && e.ColumnIndex == 9)
            {
                string finalThickness = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString();

                //Convert.ToString is for handling null event
                string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
                string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
                string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
                Color color = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[4].Style.BackColor;
                int thickness = int.Parse(finalThickness);//finalThickness
                Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
                string lineName = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();
                int status = 0;//0 means dissable 1 means enabled

                DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];
                if (cbCell.Value == cbCell.TrueValue) // .ToString() == "true"
                {
                    status = 1;
                }
                else
                {
                    status = 0;
                }
                LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s, lineName, status);


            }


            ////==Cell clicked function is triggered
            
            if(e.RowIndex >= 0)
            { 
            DataGridViewRow row = dataGridView2.Rows[e.RowIndex];
            EnergyCalculationForProcess(row);//SELECTED ROW
            }
        }

        public void EnergyCalculationForProcess(DataGridViewRow dgv_row)
        {
            //now lets calculate every parameters...
            string id = dgv_row.Cells[0].Value.ToString();
            string LineName= dgv_row.Cells[1].Value.ToString();
            Color LineColorValue = dgv_row.Cells[4].Style.BackColor;//dgv_row.Cells[1].Value.ToString();
            string prevNodeID = dgv_row.Cells[6].Value.ToString();
            string nextNodeID = dgv_row.Cells[7].Value.ToString();
            Series lineseriesID = new Series(dgv_row.Cells[8].Value.ToString());
            int thickness =int.Parse(dgv_row.Cells[5].Value.ToString());


            CalculateProcessParameterForEnergy(id, prevNodeID, nextNodeID, lineseriesID, LineColorValue, thickness, LineName);

        }

        public class TempDataType1
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
        List<TempDataType1> temporaryNodeValueStoreForRedrawLine = new List<TempDataType1>();
        //--Redraw line function
        public void CalculateProcessParameterForEnergy(string id, string prevNodeID, string nextNodeID, Series lineSeriesID, Color colorVal, int thickness_value, string name)
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


            /*
            We need to calculate the previous node id values and the next node id values.
            */
            //First for previous node id
            for (int i = 0; i <bcs.menuStripNodeInfoValues.Count; i++)
            {
                if (prevNodeID == bcs.menuStripNodeInfoValues[i].id)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore =  menuStripNodeInfoValues.GetRange(i,1);  //This is for copying the value.
                    //Copying the values on index 0 assumption 
                    temporaryNodeValueStoreForRedrawLine.Add(new TempDataType1
                    {
                        id = bcs.menuStripNodeInfoValues[i].id,
                        xVal = bcs.menuStripNodeInfoValues[i].xVal,
                        yVal = bcs.menuStripNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = bcs.menuStripNodeInfoValues[i].temperature_source,
                        humidity_source = bcs.menuStripNodeInfoValues[i].humidity_source,
                        name = bcs.menuStripNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = bcs.menuStripNodeInfoValues[i].colorValue,
                        marker_Size = bcs.menuStripNodeInfoValues[i].marker_Size,
                        airFlow = bcs.menuStripNodeInfoValues[i].airFlow

                    });

                    break;//Break form loop
                }
            }

            //Second for the next node id
            for (int i = 0; i < bcs.menuStripNodeInfoValues.Count; i++)
            {
                if (nextNodeID == bcs.menuStripNodeInfoValues[i].id)
                {
                    //This is a node : i.e start end of the node
                    //We need to store the node every information in 0 index.

                    // temporaryNodeValueStore[1].Equals(menuStripNodeInfoValues[i]);

                    //temporaryNodeValueStore = menuStripNodeInfoValues.GetRange(i, 1);
                    //The index of this values will be temporaryNodeValueStore[1] ==> 1
                    temporaryNodeValueStoreForRedrawLine.Add(new TempDataType1
                    {
                        id = bcs.menuStripNodeInfoValues[i].id,
                        xVal = bcs.menuStripNodeInfoValues[i].xVal,
                        yVal = bcs.menuStripNodeInfoValues[i].yVal,
                        // source = menuStripNodeInfoValues[i].source,
                        temperature_source = bcs.menuStripNodeInfoValues[i].temperature_source,
                        humidity_source = bcs.menuStripNodeInfoValues[i].humidity_source,
                        name = bcs.menuStripNodeInfoValues[i].name,
                        // label = menuStripNodeInfoValues[i].label,
                        // showItemText = menuStripNodeInfoValues[i].showItemText,
                        colorValue = bcs.menuStripNodeInfoValues[i].colorValue,
                        marker_Size = bcs.menuStripNodeInfoValues[i].marker_Size,
                        airFlow = bcs.menuStripNodeInfoValues[i].airFlow

                    });

                    break;//Break form loop
                }
            }

            if (temporaryNodeValueStoreForRedrawLine.Count > 0)
            {

                //--this sets the initial values of humidity and enthalpy
                bcs.CalculateHumidityEnthalpy(temporaryNodeValueStoreForRedrawLine[0].xVal, temporaryNodeValueStoreForRedrawLine[0].yVal);
                startHumidity1 = Math.Round(bcs.humidityCalculated, 2);
                startEnthalpy1 = Math.Round(bcs.enthalpyCalculated, 2);
                startSpecificVolume1 = bcs.SpecificVolumeReturn;
                //--This calculates the end humidity and the enthalpy values..
                bcs.CalculateHumidityEnthalpy((double)temporaryNodeValueStoreForRedrawLine[1].xVal, (double)temporaryNodeValueStoreForRedrawLine[1].yVal);
                endHumidity1 = Math.Round(bcs.humidityCalculated, 2);
                endEnthalpy1 = Math.Round(bcs.enthalpyCalculated, 2);
                endSpecificVolume1 = bcs.SpecificVolumeReturn;

                double enthalpyChange = endEnthalpy1 - startEnthalpy1;

                string sequenceDetected = temporaryNodeValueStoreForRedrawLine[0].name + " to " + temporaryNodeValueStoreForRedrawLine[1].name;

              
               // string tooltipString = "";
                
               // string ZeroLine = "Process:  " + name + " ";
               // string FirstLine = @"Parameters                      " + "Units               " + temporaryNodeValueStoreForRedrawLine[0].name + "                  " + temporaryNodeValueStoreForRedrawLine[1].name;
               // string SecondLine = @"DBT                                   " + "\x00B0 C                   " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2) + "                           " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2);
                //string ThirdLine = @"Relative Humidity           " + "%                     " + startHumidity1 + "                     " + endHumidity1;
                //string FourthLine = @"Humidity Ratio                " + "Kg/Kg dryair  " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2) + "                       " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2);
                //string FifthLine = "Volume Flow Rate           " + "m\xB3/s                " + Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2) + "                      " + Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2);

                //string SixthLine = "Specific Volume              " + "m\xB3/Kg             " + startSpecificVolume1 + "                    " + endSpecificVolume1;
                double massFlowRate1 = temporaryNodeValueStoreForRedrawLine[0].airFlow / startSpecificVolume1;
                double massFlowRate2 = temporaryNodeValueStoreForRedrawLine[1].airFlow / endSpecificVolume1;

                //string SeventhLine = @"Mass flow rate(dry air)   " + "Kg(dry air)/s   " + Math.Round(massFlowRate1, 2) + "                        " + Math.Round(massFlowRate2, 2);
                //string EighthLine = @"Enthalpy                           " + "KJ/Kg              " + startEnthalpy1 + "                       " + endEnthalpy1;
                double totalEnthalpyFlow1 = massFlowRate1 * startEnthalpy1;
                double totalEnthalpyFlow2 = massFlowRate2 * endEnthalpy1;
                //string NinthLine = @"Total Enthalpy Flow         " + "KJ/s                " + Math.Round(totalEnthalpyFlow1, 2) + "                      " + Math.Round(totalEnthalpyFlow2, 2);
                double heatChange = totalEnthalpyFlow2 - totalEnthalpyFlow1;
                //string TenthLine = @"Heat Change                    " + "KW                  " + Math.Round(heatChange, 2) + "                     ";
                //tooltipString = ZeroLine + "\n" + FirstLine + "\n" + SecondLine + "\n" + ThirdLine + "\n" + FourthLine + "\n" + FifthLine + "\n" + SixthLine + "\n" + SeventhLine + "\n" + EighthLine + "\n" + NinthLine + "\n" + TenthLine;


                lb_process.Text = name;
                lb_process.ForeColor = colorVal;
                lb_process.BackColor = Color.White;
                lb_node_one_name.Text = temporaryNodeValueStoreForRedrawLine[0].name.ToString();
                lb_node_one_name.ForeColor = temporaryNodeValueStoreForRedrawLine[0].colorValue;
                lb_node_one_name.BackColor = Color.White;

                lb_node_two_name.Text = temporaryNodeValueStoreForRedrawLine[1].name.ToString();
                lb_node_two_name.ForeColor = temporaryNodeValueStoreForRedrawLine[1].colorValue;
                lb_node_two_name.BackColor = Color.White;

                lb_dbt_node1_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[0].xVal, 2).ToString();
                lb_dbt_node2_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[1].xVal, 2).ToString();
                lb_RH_node1_value.Text = startHumidity1.ToString();
                lb_RH_node2_value.Text = endHumidity1.ToString();
                lb_HR_node1_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[0].yVal, 2).ToString();
                lb_HR_node2_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[1].yVal, 2).ToString();
                lb_VFR_node1_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[0].airFlow, 2).ToString();
                lb_VFR_node2_value.Text = Math.Round(temporaryNodeValueStoreForRedrawLine[1].airFlow, 2).ToString();
                lb_SV_node1_value.Text = startSpecificVolume1.ToString();
                lb_SV_node2_value.Text = endSpecificVolume1.ToString();
                lb_MFR_node1_value.Text = Math.Round(massFlowRate1, 2).ToString();
                lb_MFR_node2_value.Text = Math.Round(massFlowRate2, 2).ToString();
                lb_enthalpy_node1_value.Text = startEnthalpy1.ToString();
                lb_enthalpy_node2_value.Text = endEnthalpy1.ToString();
                lb_TEF_node1_value.Text = Math.Round(totalEnthalpyFlow1, 2).ToString();
                lb_TEF_node2_value.Text = Math.Round(totalEnthalpyFlow2, 2).ToString();
                lb_HC_value.Text = Math.Round(heatChange, 2).ToString();






            }//close of temporary node value

            // }//--Close of LOCK
        }



        /*
        Lets define the datatype first for the same thing
        */

        public class TempDataType
        {
            public string id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
           // public double yVal { get; set; }
            public double humidity { get; set; } //This humidity need to be converted to yValue...
            public string source { get; set; }
            public string name { get; set; }
            public string label { get; set; }
            public Color colorValue { get; set; }
            public string showItemText { get; set; }
            public int marker_Size { get; set; }
        }
        public List<TempDataType> nodeInfo = new List<TempDataType>();
        //--This one right here is for editing the lines...

        /// <summary>
        /// This also need to be changed we need individual unique id of the line 
        /// </summary>
        public class lineDataType
        {
            //--Line ID
            public string ID { get; set; }
            public string prevNodeId { get; set; }
            public string nextNodeId { get; set; }
            public Color lineColorValue { get; set; }
          // public Series lineSeriesID { get; set; }
            public int lineThickness { get; set; }

        }

        public List<lineDataType> lineInfo = new List<lineDataType>();



        public void previousApplyButtonCode() {



            return;
            /*
            This will help in saving the edited values            
            Steps : 1.find which value is changed 
            2.Save those changes to the database
            3.Refresh the graph and the values.
            4.Close this dialog
            */
            //Step1 : lets get the values first.
            nodeInfo.Clear();//Resetting the values first
            lineInfo.Clear();
            //   MessageBox.Show("Row count value 1 = "+ bcs.menuStripNodeInfoValues.Count);
            //   MessageBox.Show("Row count value 2 = " + bcs.menuStripNodeLineInfoValues.Count);
            for (int i = 0; i < bcs.menuStripNodeInfoValues.Count; i++)
            {
                nodeInfo.Add(new TempDataType
                {
                    id = dataGridView1.Rows[i].Cells[0].Value.ToString(),
                    xVal = double.Parse(dataGridView1.Rows[i].Cells[1].Value.ToString()),
                    humidity = double.Parse(dataGridView1.Rows[i].Cells[2].Value.ToString()),
                    showItemText = dataGridView1.Rows[i].Cells[4].Value.ToString(),
                    name = dataGridView1.Rows[i].Cells[5].Value.ToString(),
                    source = dataGridView1.Rows[i].Cells[6].Value.ToString(),
                    label = dataGridView1.Rows[i].Cells[7].Value.ToString(),
                    colorValue = dataGridView1.Rows[i].Cells[8].Style.BackColor,
                    marker_Size = int.Parse(dataGridView1.Rows[i].Cells[9].Value.ToString())
                });

            }   //Close of the for loop


            for (int i = 0; i < bcs.menuStripNodeLineInfoValues.Count; i++)
            {
                lineInfo.Add(new lineDataType
                {
                    ID = dataGridView2.Rows[i].Cells[0].Value.ToString(),
                    prevNodeId = dataGridView2.Rows[i].Cells[1].Value.ToString(),
                    nextNodeId = dataGridView2.Rows[i].Cells[2].Value.ToString(),
                    lineColorValue = dataGridView2.Rows[i].Cells[3].Style.BackColor,
                    lineThickness = int.Parse(dataGridView2.Rows[i].Cells[4].Value.ToString())
                });

            }

            //==Now lets check we get the real value or not
            //Test...
            //string s1="";
            //for (int i = 0; i < nodeInfo.Count; i++)
            //{
            //    s1 += "node value = " + nodeInfo[i].id + ",xval = " + nodeInfo[i].xVal + "," + ",hum = " + nodeInfo[i].humidity + "," + ",show text value = " + nodeInfo[i].showItemText + "," + ",color  = " + nodeInfo[i].colorValue+"\n";
            //}

            //MessageBox.Show("node val =\n " + s1);
            //string s2 = "";
            //for (int i = 0; i < lineInfo.Count; i++)
            //{
            //    s2 += "line value = " + lineInfo[i].ID + ",prev = " + lineInfo[i].prevNodeId + "," + ",next nodev = " + lineInfo[i].nextNodeId + "," + ",color = " + lineInfo[i].lineColorValue;
            //}
            //MessageBox.Show("line = " + s2);


            //Now step 2nd save changes to database.
            for (int i = 0; i < nodeInfo.Count; i++)
            {

                bcs.CalculateYFromXandHumidity(nodeInfo[i].xVal, nodeInfo[i].humidity / 100);//This will get the y value
                                                                                             // MessageBox.Show("Yalue" + bcs.y_coord_value);
                bcs.UpdateNodeInfoToDB(nodeInfo[i].id, nodeInfo[i].xVal, bcs.y_coord_value, nodeInfo[i].source, nodeInfo[i].name, nodeInfo[i].label, nodeInfo[i].colorValue, nodeInfo[i].showItemText, nodeInfo[i].marker_Size);
            }

            //For line value we need new function withou lineseries id
            for (int i = 0; i < lineInfo.Count; i++)
            {
                bcs.UpdateLineInfoToDBWithouSeriesID(lineInfo[i].ID, lineInfo[i].prevNodeId, lineInfo[i].nextNodeId, lineInfo[i].lineColorValue, lineInfo[i].lineThickness);
            }

            //Now lets refresh the graph and replot again
            //bcs.Refresh();
            //bcs.ReDrawingLineAndNode();


            bcs.RefreshGraph();
            if (bcs.chartDetailList.Count > 0)
            {
                int id = bcs.indexOfChartSelected;    //This value is changed 
                bcs.LoadNodeAndLineFromDB(id);   //Lets make it passing the stirngs 

                // flagForInsertOrUpdateDataToDB = 1;
                //--This is also completed..
                bcs.ReDrawingLineAndNode();

            }

            //loading comfrot zone
            bcs.LoadComfortZone(bcs.chartDetailList[bcs.indexForWhichChartIsSelected].chartID);


            this.Close();
            // double y =  bcs.CalculateYFromXandHumidity(30.0005, 30.0000/100.00);
            //  MessageBox.Show("Yval = " + y);

        }

        string initialThickness;
        string initialLineName;

        private void dataGridView2_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            //Thi part is for regular expression 
            if (dataGridView2.CurrentCell.ColumnIndex == 5)
            {
                //This is the value of the thickneess
                initialThickness = dataGridView2.CurrentCell.Value.ToString();

            }
            else if(e.ColumnIndex == 1)
            {
                //Name is being edited
                initialLineName = dataGridView2.CurrentCell.Value.ToString();
            }

        }

        string initialTemp, initialHumidity, initialName, initialLabel, initialNodeSize, initialAirFlow;

        private void button2_Click_1(object sender, EventArgs e)
        {
            //--Export data to excel
            CreatingExcelFileWithData();
        }

        private void btn_ok_Click(object sender, EventArgs e)
        {

            //--F this little code
            //--Lets program..

            //When ok button is clicked the there should be plotting of the comfort zone.
            //This has basically two function...
            /*
            1.insert the comfort zone info to database 
            2. Plot the comfort zone based on the value passed.
            */
            if (textBox1.Text != "" && textBox2.Text != "" && textBox3.Text != "" && textBox4.Text != "")
            {
                //chart id
                string chartid = bcs.chartDetailList[bcs.indexOfChartSelected].chartID;
                string comfortzoneid = bcs.listComfortZoneDetail[comboBox1.SelectedIndex].id;
                bcs.InsertChartComfortzoneInfo(chartid, comfortzoneid);

                //now second part call the plot function 
                // bs.PlotComfortZone()
                double mint = double.Parse(textBox1.Text);
                double maxt = double.Parse(textBox2.Text);
                double minh = double.Parse(textBox3.Text);
                double maxh = double.Parse(textBox4.Text);
                Color c = bcs.listComfortZoneDetail[comboBox1.SelectedIndex].colorValue;
                string name = bcs.listComfortZoneDetail[comboBox1.SelectedIndex].name;
                if (bcs.default_comfort_zone_of_chart.Count > 0)
                {
                    if (bcs.listchartComfortZoneInfoSingle.Count > 0)
                    {
                        //If default comfort zone is present then 
                        //Clear previous one 
                        //MessageBox.Show("Clear previous comfortzone");
                        bcs.ClearComfortZone(double.Parse(bcs.listchartComfortZoneInfoSingle[0].min_temp), double.Parse(bcs.listchartComfortZoneInfoSingle[0].max_temp), double.Parse(bcs.listchartComfortZoneInfoSingle[0].min_hum), double.Parse(bcs.listchartComfortZoneInfoSingle[0].max_hum));
                    }
                }

                bcs.PlotComfortZone(mint, maxt, minh, maxh, c, name);

                //insert of update database value 
                bcs.insertOrUpdateComfortChartSetting(chartid, comfortzoneid);

                if (bcs.dataGridView1.Rows.Count > 0)  //If there is data then only do this one
                {
                    //set parameters of your event args
                    var eventArgs = new DataGridViewCellEventArgs(1, bcs.dataGridView1.CurrentCell.RowIndex);
                    // or setting the selected cells manually before executing the function
                    bcs.dataGridView1.Rows[bcs.dataGridView1.CurrentCell.RowIndex].Cells[1].Selected = true;
                    bcs.dataGridView1_CellClick(sender, eventArgs);
                }

               // this.Close();

            }//close of if


        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //--When a chart is selected then 
            int index = comboBox1.SelectedIndex;
            if (index >= 0)
            {
                //put the values in the combobox
                textBox1.Text = bcs.listComfortZoneDetail[index].min_temp;
                textBox2.Text = bcs.listComfortZoneDetail[index].max_temp;
                textBox3.Text = bcs.listComfortZoneDetail[index].min_hum;
                textBox4.Text = bcs.listComfortZoneDetail[index].max_hum;
                btn_ok.Enabled = true;
            }

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            //--For updating things
            int index = comboBox2.SelectedIndex;
            if (index >= 0)
            {
                //We need to enable the button update.
                btn_update.Enabled = true;
                btn_delete.Enabled = true;
                btn_add.Enabled = false;
                //Next we need to fulfil the text boxes base on the combobox selected
                tb_name.Text = bcs.listComfortZoneDetail[index].name;
                tb_mintemp.Text = bcs.listComfortZoneDetail[index].min_temp;
                tb_maxtemp.Text = bcs.listComfortZoneDetail[index].max_temp;
                tb_minhum.Text = bcs.listComfortZoneDetail[index].min_hum;
                tb_maxhum.Text = bcs.listComfortZoneDetail[index].max_hum;
                btn_color.BackColor = bcs.listComfortZoneDetail[index].colorValue;

            }

        }

        private void btn_color_Click(object sender, EventArgs e)
        {
            //--This one is for color change
            ColorDialog colorDialog1 = new ColorDialog();
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                btn_color.BackColor = colorDialog1.Color;

            }
        }

        private void btn_add_Click(object sender, EventArgs e)
        {
            //We can add new value in thsi function 
            //now lets do the somethings.
            if (tb_name.Text != "")
            {

                if (tb_mintemp.Text != "" && tb_maxtemp.Text != "" && tb_minhum.Text != "" && tb_maxhum.Text != "")
                {
                    //we need to grab the value and insert it
                    double minT = double.Parse(tb_mintemp.Text);
                    double maxT = double.Parse(tb_maxtemp.Text);
                    double minH = double.Parse(tb_minhum.Text);
                    double maxH = double.Parse(tb_maxhum.Text);
                    string name = tb_name.Text;
                    Color c = btn_color.BackColor;
                    bcs.InsertComfortZoneValue(name, minT, maxT, minH, maxH, c);
                    MessageBox.Show("Add success");

                    //NOW LEST REFRESH THE FORM
                    refreshAfterEdit();

                }
                else
                {
                    MessageBox.Show("Please fill the temperature and humidity accurately");

                }
            }
            else
            {
                MessageBox.Show("Enter the name of the comfortzone");
            }
            //Inserted message
            //MessageBox.Show("insert end");
        }


        public void refreshAfterEdit()
        {
            //Now lets do some of the refreshing task
            bcs.PullComfortZoneData();//This will pull the co

            //Now lets load the values int combobox1.items.add()
            comboBox1.Items.Clear();
            foreach (var item in bcs.listComfortZoneDetail)
            {
                comboBox1.Items.Add(item.name);
            }


            //Now lets load the values int combobox1.items.add()
            comboBox2.Items.Clear();
            foreach (var item in bcs.listComfortZoneDetail)
            {
                comboBox2.Items.Add(item.name);
            }

            tb_name.Text = "";
            tb_mintemp.Text = "";
            tb_maxtemp.Text = "";
            tb_minhum.Text = "";
            tb_maxhum.Text = "";

            //Also first text boxes need to be refreshed
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
            textBox4.Text = "";
            btn_ok.Enabled = false;
            btn_update.Enabled = false;
            btn_delete.Enabled = false;
            btn_add.Enabled = true;

        }

        private void btn_update_Click(object sender, EventArgs e)
        {
            //now we can update at this point the previous values.
            if (tb_name.Text != "")
            {

                if (tb_mintemp.Text != "" && tb_maxtemp.Text != "" && tb_minhum.Text != "" && tb_maxhum.Text != "")
                {
                    //we need to grab the value and insert it
                    double minT = double.Parse(tb_mintemp.Text);
                    double maxT = double.Parse(tb_maxtemp.Text);
                    double minH = double.Parse(tb_minhum.Text);
                    double maxH = double.Parse(tb_maxhum.Text);
                    string name = tb_name.Text;
                    Color c = btn_color.BackColor;
                    string id = bcs.listComfortZoneDetail[comboBox2.SelectedIndex].id;
                    //bs.InsertComfortZoneValue(name, minT, maxT, minH, maxH, c);
                    bcs.UpdateComfortZoneValue(id, name, minT, maxT, minH, maxH, c);
                    MessageBox.Show("Value updated succefully");
                    //NOW LEST REFRESH THE FORM
                    refreshAfterEdit();
                }
                else
                {
                    MessageBox.Show("Please fill the temperature and humidity accurately");

                }
            }
            else
            {
                MessageBox.Show("Enter the name of the comfortzone");
            }

        }

        private void btn_delete_Click(object sender, EventArgs e)
        {
            //--This one is for deleting purpose
            //This one is for deletion operation
            int index = comboBox2.SelectedIndex;
            if (index >= 0)
            {
                //WE need to call the index value as well as send the id parameter
                //This will delete the value form both the comfortDetail and chartcomfortzonesetting table.
                DialogResult dialogResult = MessageBox.Show("are you sure you want to delete :" + bcs.listComfortZoneDetail[index].name, "Delete", MessageBoxButtons.YesNo);
                if (dialogResult == DialogResult.Yes)
                {
                    bcs.DeleteComfortZoneData(bcs.listComfortZoneDetail[index].id);
                    MessageBox.Show("value deleted successfully");
                    //Refresh the values
                    refreshAfterEdit();
                }


            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            //--Edit option is clicked
            //On check box checked...
            if (checkBox2.Checked == true)
            {
                //it is checked 
                //this.Height = 506;
                groupBox2.Enabled = true;
                btn_add.Enabled = true;
                tb_name.Text = "";
                tb_mintemp.Text = "";
                tb_maxtemp.Text = "";
                tb_minhum.Text = "";
                tb_maxhum.Text = "";
                btn_update.Enabled = false;
                btn_delete.Enabled = false;
                btn_color.BackColor = Color.LightGreen;

                //when checked we need to fulfill the combobox2 as well
                //Now lets load the values int combobox1.items.add()
                //we need to reset first
                comboBox2.Items.Clear();
                comboBox2.Refresh();
                foreach (var item in bcs.listComfortZoneDetail)
                {
                    comboBox2.Items.Add(item.name);
                }

                groupBox2.Enabled = true;
                groupBox2.Visible = true;
            }
            else
            {
                //It is not checked
                tb_name.Text = "";
                tb_mintemp.Text = "";
                tb_maxtemp.Text = "";
                tb_minhum.Text = "";
                tb_maxhum.Text = "";
                //groupBox2.Enabled = false;
                btn_add.Enabled = false;
                groupBox2.Enabled = false;
                groupBox2.Visible = false;
                //this.Height = 280;
            }
        }

        public void CreatingExcelFileWithData()
        {

            try
            {

                if(dataGridView1.RowCount <= 0 && dataGridView2.RowCount <= 0)
                {
                    //Do not proceed futher 
                    MessageBox.Show("No data to save");
                    return;
                }

                //excel is checked..
                Excel.Application oApp;
                Excel.Workbook oBook;
                Excel.Worksheet oSheet;

                oApp = new Excel.Application();
                oBook = oApp.Workbooks.Add();
                oSheet = (Excel.Worksheet)oBook.Worksheets.get_Item(1);
                Cursor = Cursors.WaitCursor;

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

                for(int i =0;i<dataGridView1.RowCount;i++) {
                    //for station 
                    //oSheet.Cells[count+i, 1] = "Node Information";//WFA_psychometric_chart.Properties.Resources.Building_Information;
                    oSheet.Cells[count + i, 1] = dataGridView1.Rows[i].Cells[0].Value.ToString();
                    oSheet.Cells[count + i, 2] = dataGridView1.Rows[i].Cells[1].Value.ToString();//"Temperature Source";
                    oSheet.Cells[count + i, 3] = dataGridView1.Rows[i].Cells[3].Value.ToString();//"Temperature";
                    oSheet.Cells[count + i, 4] = dataGridView1.Rows[i].Cells[4].Value.ToString();//"Humidity Source";
                    oSheet.Cells[count + i, 5] = dataGridView1.Rows[i].Cells[5].Value.ToString(); //"Humidity";
                    oSheet.Cells[count + i, 6] = dataGridView1.Rows[i].Cells[6].Value.ToString(); //"Equivalent Enthalpy";
                    //--For color value
                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[i].Cells[7];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    Color c = buttonCell.Style.BackColor;//= colorDialog1.Color;//System.Drawing.Color.Red;
                   
                    oSheet.Cells[count + i, 7] = c.ToString();
                    oSheet.Cells[count + i, 8] = dataGridView1.Rows[i].Cells[8].Value.ToString(); // building_info[0].zip.ToString();  //zip doesnot exist now
                    oSheet.Cells[count + i, 9] = dataGridView1.Rows[i].Cells[9].Value.ToString(); ;

                } //Close of for loop

                int NewLineCount =count+ dataGridView1.RowCount +2 ;


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
                for (int i = 0; i < dataGridView2.RowCount; i++)
                {
                    oSheet.Cells[NewLineCount + 2 + i, 1] = dataGridView2.Rows[i].Cells[1].Value.ToString();
                    oSheet.Cells[NewLineCount + 2+i, 2] = dataGridView2.Rows[i].Cells[2].Value.ToString();//"Start Node Name";

                    oSheet.Cells[NewLineCount + 2+i, 3] = dataGridView2.Rows[i].Cells[3].Value.ToString();//"End Node Name";
                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[i].Cells[4];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    Color col = buttonCell.Style.BackColor;//= colorDialog1.Color;//System.Drawing.Color.Red;

                    oSheet.Cells[NewLineCount + 2+i, 4] = col.ToString();
                    oSheet.Cells[NewLineCount + 2+i, 5] = dataGridView2.Rows[i].Cells[5].Value.ToString(); //"Thickness";
                    DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[i].Cells[9];

                    string status = "false";
                    if (cbCell.Value.ToString() == "true")
                    {
                        status = "true";
                    }
                    else
                    {
                        status = "false";
                    }

                    oSheet.Cells[NewLineCount + 2+i, 6] = status; //"Show Name";
                    oSheet.Cells[NewLineCount + 2+i, 7] = dataGridView2.Rows[i].Cells[10].Value.ToString(); //"DBT1";
                    oSheet.Cells[NewLineCount + 2+i, 8] = dataGridView2.Rows[i].Cells[11].Value.ToString();//"RH1";
                    oSheet.Cells[NewLineCount + 2+i, 9] = dataGridView2.Rows[i].Cells[12].Value.ToString(); //"HR1";
                    oSheet.Cells[NewLineCount + 2+i, 10] = dataGridView2.Rows[i].Cells[13].Value.ToString(); //"SV1";


                    oSheet.Cells[NewLineCount + 2+i, 11] = dataGridView2.Rows[i].Cells[14].Value.ToString(); //"MFR1";
                    oSheet.Cells[NewLineCount + 2+i, 12] = dataGridView2.Rows[i].Cells[15].Value.ToString(); //"enthalpy1";

                    oSheet.Cells[NewLineCount + 2+i, 13] = dataGridView2.Rows[i].Cells[16].Value.ToString(); //"TEF1";
                    oSheet.Cells[NewLineCount + 2+i, 14] = dataGridView2.Rows[i].Cells[17].Value.ToString(); //"DBT2";
                    oSheet.Cells[NewLineCount + 2+i, 15] = dataGridView2.Rows[i].Cells[18].Value.ToString();// "RH2";
                    oSheet.Cells[NewLineCount + 2+i, 16] = dataGridView2.Rows[i].Cells[19].Value.ToString(); //"HR2";
                    oSheet.Cells[NewLineCount + 2+i, 17] = dataGridView2.Rows[i].Cells[20].Value.ToString(); //"SV2";
                    oSheet.Cells[NewLineCount + 2+i, 18] = dataGridView2.Rows[i].Cells[21].Value.ToString(); //"MFR2";
                    oSheet.Cells[NewLineCount + 2+i, 19] = dataGridView2.Rows[i].Cells[22].Value.ToString(); //"Enthalpy2";
                    oSheet.Cells[NewLineCount + 2+i, 20] = dataGridView2.Rows[i].Cells[23].Value.ToString();// "TEF2";
                    oSheet.Cells[NewLineCount + 2+i, 21] = dataGridView2.Rows[i].Cells[24].Value.ToString(); //"heat change";


                }

                //now lets open the save dialog box and the save it there..

                Cursor = Cursors.Default;
                String fileName = "";

                saveFD.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);// "C:";
                saveFD.FileName = "Excelfile";
                saveFD.Title = WFA_psychometric_chart.Properties.Resources.Save_Excel_file_to;
                saveFD.Filter = "Excel file|*.xls";
                if (saveFD.ShowDialog() == DialogResult.OK)
                {
                    //save the file..
                    fileName = saveFD.FileName;
                    oBook.SaveAs(fileName);

                }
                oBook.Close();
                oApp.Quit();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }



        /// <summary>
        /// Exports two data grid value to excell
        /// </summary>
        public void ExportDataToExcelFile()
        {
            //--These functions do the export work
        }


        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            //--click or unclick 
            if(checkBox1.Checked == true)
            {
                if (bcs.default_comfort_zone_of_chart.Count > 0)
                {
                    bcs.ShowOrHideForComfortZoneFromEditNode(1);
                }
                else
                {

                    // MessageBox.Show("Please set a comfort zone for this chart first !");
                    // checkBox1.Checked = false;


                    //--We need to write some function which will set the comfortzone by itself
                    //--steps: 1. if data present selecte default comfort zone else throw message
                    if (comboBox1.Items.Count > 0)
                    {
                        comboBox1.SelectedIndex = 0;
                        btn_ok_Click(sender, e);
                        //bcs.ShowOrHideForComfortZoneFromEditNode(1);
                        bcs.PullComfortZoneData();//This will load the listComfortZoneDetail
                        //btn_color.BackColor = Color.LightGreen;
                        //Now lets load the values int combobox1.items.add()
                        comboBox1.Items.Clear();
                        foreach (var item in bcs.listComfortZoneDetail)
                        {
                            comboBox1.Items.Add(item.name);
                        }

                    }
                    else
                    {
                        //--No data present
                        MessageBox.Show("Comfortzone data not present. Make some comfortzone first!");
                        checkBox1.Checked = false;

                    }

                }
            }
            else
            {
              //  bcs.ShowOrHideForComfortZoneFromEditNode(0);
                if (bcs.default_comfort_zone_of_chart.Count > 0)
                {
                    //MessageBox.Show("false clicked");
                    bcs.ShowOrHideForComfortZoneFromEditNode(0);
                }
                //else
                //{
                //    MessageBox.Show("Please set a comfort zone for this chart first !");
                //    checkBox1.Checked = false;
                //}
            }

        }


        private void button1_Click_1(object sender, EventArgs e)
        {
            //--This click should trigger the other click in the form
            //Should show the comfortzone formate
           // bcs.SettingChartOpen();
        }

        private void dataGridView1_CellValueChanged_1(object sender, DataGridViewCellEventArgs e)
        {
            //--This is where we are going to do all our formating 

            //if(dataGridView1.CurrentCell.ColumnIndex.ToString() == "" || dataGridView1.CurrentCell.ColumnIndex.ToString() == null)
            //{
            //    return;
            //}
            //if (dataGridView1.CurrentCell.ColumnIndex == 4)
            //{
            //    try
            //    {
            //        if (Convert.ToString(dataGridView1.CurrentCell.Value) != "")
            //        {
            //            double t = double.Parse(Convert.ToString(dataGridView1.CurrentCell.Value));
            //            if (t >= 0 && t <= 50)
            //            {

            //            }
            //            else
            //            {
            //                MessageBox.Show("Temperature should be between 0 to 50 Deg Celcius");
            //                dataGridView1.CurrentCell.Value = 0.ToString();
            //            }
            //        }
            //    }
            //    catch (Exception ex)
            //    {
            //        MessageBox.Show("Value should be numeric : " + ex.Message);
            //        dataGridView1.CurrentCell.Value = 0.ToString();
            //    }
            //}



            ////--This represents humidity

            //if (dataGridView1.CurrentCell.ColumnIndex == 5)
            //{
            //    try
            //    {
            //        if (Convert.ToString(dataGridView1.CurrentCell.Value)!= "")
            //        {
            //            double h = double.Parse(Convert.ToString(dataGridView1.CurrentCell.Value));
            //            if (h >= 0 && h <= 100)
            //            {

            //            }
            //            else
            //            {
            //                MessageBox.Show("Humidity should be between 0 to 100 % ");
            //                dataGridView1.CurrentCell.Value = 0.ToString();
            //            }
            //        }
            //    }
            //    catch (Exception ex)
            //    {
            //        MessageBox.Show("Value should be numeric : " + ex.Message);
            //        dataGridView1.CurrentCell.Value = 0.ToString();
            //    }
            //}





        }

        private void dataGridView2_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            //This is for doing soem work
            //if (e.ColumnIndex == 9)
            //{
            //  // dataGridView2.CommitEdit(DataGridViewDataErrorContexts.Commit);
            //}

        }

        private void dataGridView2_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            //if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && e.ColumnIndex == 9)
            //{
            //    string finalThickness = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString();

            //    //Convert.ToString is for handling null event
            //    string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
            //    string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
            //    string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
            //    Color color = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[4].Style.BackColor;
            //    int thickness = int.Parse(finalThickness);//finalThickness
            //    Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
            //    string lineName = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();
            //    int status = 0;//0 means dissable 1 means enabled

            //    DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];
            //    if (cbCell.Value == cbCell.TrueValue)
            //    {
            //        status = 1;
            //    }
            //    else
            //    {
            //        status = 0;
            //    }
            //    LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s, lineName, status);


            //}
        }

        public void LineUpdateAndReload(string lineid,string prevNodeID,string nextNodeID,Color color,int thickness,Series lineseries,string name,int status)
        {

            bcs.UpdateLineInfoToDB(lineid, prevNodeID, nextNodeID, color, lineseries, thickness,name,status);

            //==Now lets refresh the data and 
            //RefreshDataFromDBAndChart();
            //--After refreshing
            //LoadNodeAndLine();//--Loading the data 
            RefreshChartAndDGVForMixNodeFunction();


        }

        private void dataGridView2_SelectionChanged(object sender, EventArgs e)
        {
            var cellCopy = dataGridView2.CurrentCell;

            if(dataGridView2.Rows.Count <= 0)
            {
                return;
            }

            if (cellCopy != null)
            {
                if ( dataGridView2.CurrentCell.RowIndex.ToString() != "")
                {
                    //set parameters of your event args
                    var eventArgs = new DataGridViewCellEventArgs(0, dataGridView2.CurrentCell.RowIndex);
                    //or setting the selected cells manually before executing the function
                    //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[dataGridView2.CurrentCell.ColumnIndex].Selected = true;
                    dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Selected = true;
                    dataGridView2_CellClick(sender, eventArgs);
                }

            }
        }

        private void dataGridView1_MouseClick(object sender, MouseEventArgs e)
        {
            //--This is done for removing the editmode error null refrence error
            //DataGridView dgv = (DataGridView)sender;
            //if (dgv.HitTest(e.X, e.Y).Type == DataGridViewHitTestType.RowHeader)
            //{
            //    dgv.EditMode = DataGridViewEditMode.EditOnKeystrokeOrF2;
            //    dgv.EndEdit();
            //}
            //else {
            //    dgv.EditMode = DataGridViewEditMode.EditOnEnter;
            //}
            //try { 
            //if(sender == null || e == null)
            //{
            //    return;
            //}
            //else if(dataGridView1.CurrentCell.Value == null)
            //{
            //    return;
            //}
            //}catch(NullReferenceException ex)
            //{
            //   //MessageBox.Show("Line 2011 : " + ex.Message);
            //}


        }

        private void dataGridView2_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if((e.RowIndex >=0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) &&  e.ColumnIndex == 5)
            { 
            //now lets check for the number value
            try
            {

                string finalThickness = dataGridView2.CurrentCell.Value.ToString();

                string pattern = @"^[0-9]+$"; //for integers only
                Regex rgx = new Regex(pattern);
                //string sentence = "Who writes these notes?";
                if (rgx.IsMatch(finalThickness) == true)
                {
                    //matched no need to edit 
                }
                else
                {
                    MessageBox.Show("You can only input integer values");
                    dataGridView2.CurrentCell.Value = initialThickness;
                    return;
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("You can only input integer values");

                dataGridView2.CurrentCell.Value = initialThickness;
                return;
            }

            try

            {
                string finalThickness = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString();

                //Convert.ToString is for handling null event
                string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
                string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
                string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
                Color color = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[4].Style.BackColor;
                int thickness = int.Parse(finalThickness);//finalThickness
                Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
                 string lineName = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();
                    int status = 0;//0 means dissable 1 means enabled

         DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];
                    if (cbCell.Value.ToString() == "true")//cbCell.TrueValue
                    {                        
                        status = 1;
                    }
                    else
                    {
                        status = 0;
                    }

                    LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s,lineName,status);

            }
            catch
            { }


            } else if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && e.ColumnIndex == 9)
            {
                //string finalThickness = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString();

                ////Convert.ToString is for handling null event
                //string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
                //string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
                //string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
                //Color color = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[4].Style.BackColor;
                //int thickness = int.Parse(finalThickness);//finalThickness
                //Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
                //string lineName = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();
                //int status = 0;//0 means dissable 1 means enabled

                //DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];
                //if (cbCell.Value == cbCell.TrueValue)
                //{
                //    status = 1;
                //}
                //else
                //{
                //    status = 0;
                //}
                //LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s, lineName, status);


            }
            else if((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && e.ColumnIndex == 1)
            {

                //Name changed 
                try
                {

                    string finalLineName = dataGridView2.CurrentCell.Value.ToString();

                    string pattern = @"^\w+$"; //for integers only
                    Regex rgx = new Regex(pattern);
                    //string sentence = "Who writes these notes?";
                    if (rgx.IsMatch(finalLineName) == true)
                    {
                        //matched no need to edit 
                    }
                    else
                    {
                        MessageBox.Show("You can only input letters,integer and underscore values");
                        dataGridView2.CurrentCell.Value = initialLineName;
                        return;
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show("You can only input integer values");

                    dataGridView2.CurrentCell.Value = initialLineName;
                    return;
                }

                try

                {
                    string finalLineName = dataGridView2.CurrentCell.Value.ToString();

                    //Convert.ToString is for handling null event
                    string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
                    string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
                    string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
                    Color color = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[4].Style.BackColor;
                    int thickness = int.Parse(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString());//finalThickness
                    Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
                    string lineName = finalLineName; //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();
                    int status = 0;//0 means dissable 1 means enabled

                    DataGridViewCheckBoxCell cbCell = (DataGridViewCheckBoxCell)dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[9];
                    if (cbCell.Value.ToString() == "true")//cbCell.TrueValue
                    {
                        status = 1;
                    }
                    else
                    {
                        status = 0;
                    }
                   // MessageBox.Show("Status 1means true , 0 means false = " + status);
                    LineUpdateAndReload(lineid, prevNodeID, nextNodeID, color, thickness, s, lineName, status);

                }
                catch
                { }


            }


        }

        //--cALLING END EDIT FUNCTION 
        public void EndEditCall()
        {

        }



        public void CallFromTemperatureAndHumidtyFormForMixNodeFxn(string nodeID)
        {

            bcs.DBUpdateMixPointOnNodeValueChange(nodeID);

            RefreshChartAndDGVForMixNodeFunction();
        }

        /// <summary>
        /// This is triggered when the editing is completed
        /// 
        /// </summary>
        /// <param name="sender"> an object</param>
        /// <param name="e"> event value</param>
        public void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {


            
             
            try {



                //Lets record previous data here 
                if (dataGridView1.CurrentCell.ColumnIndex == 3)
                {
                    try {

                        if (dataGridView1.CurrentCell.Value == null)
                        {
                            dataGridView1.CurrentCell.Value = initialTemp;
                            return;
                        }
                        //This one is temperature
                        //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].
                        // dataGridView1.CurrentCell.Style.BackColor = Color.Red;
                        double finalTemp = double.Parse(dataGridView1.CurrentCell.Value.ToString());
                        // Create the regular expression
                        // string pattern = @"\w+_";
                        // Regex regex = new Regex(pattern);
                        // Compare a string against the regular expression
                        // return regex.IsMatch(finalTemp);
                        //--Now lets insert the values
                       //.                     

                    } catch (Exception ex)
                    {
                        MessageBox.Show("You can only input number values");

                        dataGridView1.CurrentCell.Value = initialTemp;
                        return;
                    }

                    try {
                        //--Once the name is changed it has to save the changes
                        
                        double finalTemp =double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString()); //double.Parse(dataGridView1.CurrentCell.Value.ToString());

                        string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        string Humidity_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        string  Temperature_sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        double xVal = finalTemp; //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());

                         double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                        double yVal = bcs.y_coord_value;



                      //  double yVal = //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());

                       // MessageBox.Show("column index " + dataGridView1.CurrentCell.ColumnIndex+",\ntemperature="+xVal+"\nHumidity ="+humidity+"\nyval= "+yVal);
                      //  string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int airFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
                        //--This one is closed 
                        //  UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, Temperature_sourceVal, Humidity_sourceVal, nameVal, colorVal, nodeSizeVal, airFlow);
                        // dataGridView1.CurrentCell.Style.BackColor = Color.SteelBlue;
                        //=============================For mix node refresh function===========//
                      //  MessageBox.Show("HERE WE ARE");
                        bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

                        RefreshChartAndDGVForMixNodeFunction();
                         //=================================End of : For mix node section==============//
                    }
                    catch { }

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
                {
                    //This one is humidity
                    // initialHumidity = dataGridView1.CurrentCell.Value.ToString();
                    try
                    {

                        if (dataGridView1.CurrentCell.Value == null)
                        {
                            dataGridView1.CurrentCell.Value = initialHumidity;
                            return;
                        }

                        double finalHum = double.Parse(dataGridView1.CurrentCell.Value.ToString());
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input number values");

                        dataGridView1.CurrentCell.Value = initialHumidity;
                        return;
                    }

                    try
                    {
                        //--Once the name is changed it has to save the changes
                       // double finalHum = double.Parse(dataGridView1.CurrentCell.Value.ToString());

                        //string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                        //string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        //string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        //string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                        //double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                        ////double yVal = finalHum;//double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        ////--Now we need to get y value form humidity and xVal(ie temperature)
                        //bcs.CalculateYFromXandHumidity(xVal, finalHum / 100);

                        //double yVal = bcs.y_coord_value;
                        //string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        //// MessageBox.Show("show text = " + showTextVal);
                        //Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        //int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());
                        //UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);


                        string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                        double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                        // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                        double yVal = bcs.y_coord_value;

                        // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);

                        //=============================For mix node refresh function===========//
                        bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

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
                    string temperature_source = bcs.menuStripNodeInfoValues[index].temperature_source;
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
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            //Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);





                        }
                        catch { }


                    }
                    else
                    {
                        //Do it for the mix section

                        try
                        {




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

                        //try
                        //{
                            //UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            //Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, "Mix", "Mix", nameVal, colorVal, nodeSizeVal, AirFlow);




                        }



            }
                //else if (dataGridView1.CurrentCell.ColumnIndex == 6)
                //{
                //    //This one is source ==> non editable now
                //}
                //else if (dataGridView1.CurrentCell.ColumnIndex == 1)
                //{
                //    //This one is lable
                //    //initialLabel = dataGridView1.CurrentCell.Value.ToString();

                //    try
                //    {
                        
                //        string finalLabel = dataGridView1.CurrentCell.Value.ToString();

                //        string pattern = @"^\w+$";// @"^[a-zA-Z0-9\_]+$"; //for string 
                //        Regex rgx = new Regex(pattern);
                //        //string sentence = "Who writes these notes?";
                //        if (rgx.IsMatch(finalLabel) == true)
                //        {
                //            //matched no need to edit 
                //        }
                //        else
                //        {
                //            MessageBox.Show("You can only input letters,numbers and underscores values");
                //            dataGridView1.CurrentCell.Value = initialLabel;
                //            return;
                //        }

                //    }
                //    catch (Exception ex)
                //    {
                //        MessageBox.Show("You can only input letters,numbers and underscores values");

                //        dataGridView1.CurrentCell.Value = initialLabel;
                //        return;
                //    }

                //    //Else we need to input value and update
                //    try
                //    {
                //        //--Once the name is changed it has to save the changes
                //        string finalLabel = dataGridView1.CurrentCell.Value.ToString();
                //        string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                //        string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                //        string labelVal = finalLabel;//dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                //        string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                //        double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                //        //double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                //        double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                //        bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                //        double yVal = bcs.y_coord_value;

                //        string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                //        Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                //        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                //        UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

                //    }
                //    catch { }




                //}
                else if (dataGridView1.CurrentCell.ColumnIndex == 8)
                {
                    //This one is nodesize
                    // initialNodeSize = dataGridView1.CurrentCell.Value.ToString();

                    int index = dataGridView1.CurrentCell.RowIndex;//This is the index
                    string temperature_source = bcs.menuStripNodeInfoValues[index].temperature_source;
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
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);

                        }
                        catch { }

                    }
                    else
                    {
                        //========================mixing section ==============================//

                        try
                        {

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
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, "Mix", "Mix", nameVal, colorVal, nodeSizeVal, AirFlow);

                        }
                        catch { }


                        //========================end of mixing =================================//
                    }
                } //Close of 8
                else if (dataGridView1.CurrentCell.ColumnIndex == 9)
                {
                    //This one is nodesize
                    // initialNodeSize = dataGridView1.CurrentCell.Value.ToString();


                    int index = dataGridView1.CurrentCell.RowIndex;//This is the index
                    string temperature_source = bcs.menuStripNodeInfoValues[index].temperature_source;
                    if (temperature_source != "Mix")
                    {


                        try
                        {


                            if (dataGridView1.CurrentCell.Value == null)
                            {
                                dataGridView1.CurrentCell.Value = initialAirFlow;
                                return;
                            }
                            string finalAirflow = dataGridView1.CurrentCell.Value.ToString();

                            string pattern = @"^[0-9]+$"; //for integers only
                            Regex rgx = new Regex(pattern);
                            //string sentence = "Who writes these notes?";
                            if (rgx.IsMatch(finalAirflow) == true)
                            {
                                //matched no need to edit 
                            }
                            else
                            {
                                MessageBox.Show("You can only input integer values");
                                dataGridView1.CurrentCell.Value = initialAirFlow;
                                return;
                            }

                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("You can only input integer values");

                            dataGridView1.CurrentCell.Value = initialAirFlow;
                            return;
                        }

                        try
                        {
                            //--Once the name is changed it has to save the changes
                            // string finalSize = dataGridView1.CurrentCell.Value.ToString();

                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);
                            //=============================For mix node refresh function===========//
                            bcs.DBUpdateMixPointOnNodeValueChange(nodeIDVal);

                            RefreshChartAndDGVForMixNodeFunction();
                            //=================================End of : For mix node section==============//

                        }
                        catch { }

                    }
                    else
                    {
                        //--MIXING SECTION

                        try
                        {

                            string finalAirflow = dataGridView1.CurrentCell.Value.ToString();

                            string pattern = @"^[0-9]+$"; //for integers only
                            Regex rgx = new Regex(pattern);
                            //string sentence = "Who writes these notes?";
                            if (rgx.IsMatch(finalAirflow) == true)
                            {
                                //matched no need to edit 
                            }
                            else
                            {
                                MessageBox.Show("You can only input integer values");
                                dataGridView1.CurrentCell.Value = initialAirFlow;
                                return;
                            }

                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("You can only input integer values");

                            dataGridView1.CurrentCell.Value = initialAirFlow;
                            return;
                        }

                        try
                        {
                            //--Once the name is changed it has to save the changes
                            // string finalSize = dataGridView1.CurrentCell.Value.ToString();

                            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
                            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
                            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                            double yVal = bcs.y_coord_value;

                            // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
                            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                            // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, "Mix", "Mix", nameVal, colorVal, nodeSizeVal, AirFlow);

                        }
                        catch { }


                    }

                }//Close of 9
            }
            catch
            {

            }

            }


//==========================================End of dataGridView1_CellEdit...===========================================///

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {

            //try { 
            ////Lets record previous data here 
            //if(dataGridView1.CurrentCell.ColumnIndex == 4)
            //{
            //    //This one is temperature
            //    initialTemp = dataGridView1.CurrentCell.Value.ToString();
            //}
            //else if (dataGridView1.CurrentCell.ColumnIndex == 5)
            //{
            //    //This one is humidity
            //    initialHumidity = dataGridView1.CurrentCell.Value.ToString();

            //}
            //else if (dataGridView1.CurrentCell.ColumnIndex == 0)
            //{
            //    //This one is name
            //    initialName = dataGridView1.CurrentCell.Value.ToString();

            //}                                                          
            //else if (dataGridView1.CurrentCell.ColumnIndex == 1)
            //{
            //    //This one is lable
            //    initialLabel = dataGridView1.CurrentCell.Value.ToString();
            //}
            //else if (dataGridView1.CurrentCell.ColumnIndex == 9)
            //{
            //    //This one is nodesize
            //    initialNodeSize = dataGridView1.CurrentCell.Value.ToString();
            //}
            //}
            //catch
            //{

            //}

            //--This one is edited section 
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
                    initialTemp = dataGridView1.CurrentCell.Value.ToString();
                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
                {
                    //This one is humidity
                    initialHumidity = dataGridView1.CurrentCell.Value.ToString();

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 0)
                {
                    //This one is name
                    initialName = dataGridView1.CurrentCell.Value.ToString();

                }
                //else if (dataGridView1.CurrentCell.ColumnIndex == 1)
                //{
                //    //This one is lable
                //    initialLabel = dataGridView1.CurrentCell.Value.ToString();
                //}
                else if (dataGridView1.CurrentCell.ColumnIndex == 8)
                {
                    //This one is nodesize
                    initialNodeSize = dataGridView1.CurrentCell.Value.ToString();
                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 9)
                {
                    //This one is nodesize
                    initialAirFlow = dataGridView1.CurrentCell.Value.ToString();
                }
            }
            catch
            {

            }


        }
    }
}
