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
using System.Windows.Forms.DataVisualization.Charting;

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

            if (dataGridView1.CurrentCell.RowIndex < bcs.menuStripNodeInfoValues.Count)
            {
                ComboBox combo = e.Control as ComboBox;

                if (combo != null)
                {
                    combo.SelectedIndexChanged -= new EventHandler(ComboBox_SelectedIndexChanged);
                    combo.SelectedIndexChanged += new EventHandler(ComboBox_SelectedIndexChanged);
                }
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
            ComboBox cb = (ComboBox)sender;
            string item =Convert.ToString( cb.Text);

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
                else if(cbx.ColumnIndex == 4)
                {
                    //Humidity source is chosen to be Manual

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
            }
            else if(item == "Web")
            {
                if(item == "" || item == null)
                {
                    return;
                }
                //MessageBox.Show("Web select gariyo ");
                //=We need to select chose the web value and then update the values...

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

                // dataGridView1.EditMode(false);//This one is for setting form edit mode to normal mode
                // Put the cells in edit mode when user enters them.
                //dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
                dataGridView1.Refresh();

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
                        MessageBox.Show("exception :" + ex.Message);
                    }
                }

            }

            else if (item == "Name")
            {
               // MessageBox.Show("name select gariyo ");

                

                //--Once the name is changed it has to save the changes
                string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                double xVal =double.Parse( dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                //double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
                double yVal = bcs.y_coord_value;
                string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
               // MessageBox.Show("show text = " + showTextVal);
                Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

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
            //dataGridView1.Rows[bcs.menuStripNodeInfoValues.Count].Visible = false;
            dataGridView1.AllowUserToAddRows = false;
            dataGridView2.AllowUserToAddRows = false;
        }



        //==THis flag is for omitting of calling of the double event while clicking in the web section

        bool flagForWeb = false;


        public void WebUpdateSelection()
        {
            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
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

            double temperature_Val = bcs.temp_pulled_from_web;
            double humidity_Val = double.Parse(bcs.hum_pulled);
            double y_value = bcs.CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);

           // xVal = temperature_Val;
            yVal = y_value;

            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            // Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
           // double yVal = y_value; //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;

            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);




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
            double humidity_Val = double.Parse(bcs.hum_pulled);
            double y_value = bcs.CalculateYFromXandHumidity(temperature_Val, humidity_Val / 100);

             xVal = temperature_Val;
            //yVal = y_value;

            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();
            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            string temperature_Source = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            string humidity_Souce = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString();//--This contains the name
                                                                                                                     // double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString());
            double yVal = y_value; //double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Style.BackColor;
            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
            int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

            updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);


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
            MessageBox.Show("Temperature Error print = " + TemperatureValue);

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
                    else
                    {
                        temperature_value = "Device";
                        temperature_indexValue = 2;
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
                    else
                    {
                        humidity_sourceValue = "Device";
                        indexForSource = 2;
                    }

                    // DataGridViewComboBoxColumn cb = (DataGridViewComboBoxColumn)showText;
                    //cb.Items[0].ToString();
                    // MessageBox.Show("source VALUE MEMBER= " + CB_DGV_Source.Items[indexForSource]);
                    bcs.enthalpyHumidityCalculatorForXYvalue(bcs.menuStripNodeInfoValues[i].xVal, bcs.menuStripNodeInfoValues[i].yVal);
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

                                                                                                             

                    indexForParticularRow++;

                    //  dataGridView1.Rows.Add(row);

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[i].Cells[7];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.menuStripNodeInfoValues[i].colorValue;

                }
            }//clsoe of if value

            //--Second : This is for the line to show in the datagridview2
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
                            startNodeName = bcs.menuStripNodeInfoValues[x].name;
                            break;
                        }

                    }

                    //--This one is for end nodename
                    for (int v = 0; v < bcs.menuStripNodeInfoValues.Count; v++)
                    {
                        if (bcs.menuStripNodeLineInfoValues[i].nextNodeId == bcs.menuStripNodeInfoValues[v].id)
                        {
                            endNodeName = bcs.menuStripNodeInfoValues[v].name;
                            break;
                        }

                    }



                    //now lets display...                    
                    string[] row = new string[] { bcs.menuStripNodeLineInfoValues[i].ID, bcs.menuStripNodeLineInfoValues[i].name, startNodeName, endNodeName, "", bcs.menuStripNodeLineInfoValues[i].lineThickness.ToString(), bcs.menuStripNodeLineInfoValues[i].prevNodeId.ToString(), bcs.menuStripNodeLineInfoValues[i].nextNodeId.ToString(), bcs.menuStripNodeLineInfoValues[i].lineSeriesID.ToString(), };
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
            ////This part contains the cell click values
            //if((e.RowIndex>=0 && e.RowIndex<bcs.menuStripNodeInfoValues.Count) && (e.ColumnIndex == 8))
            //{
            //    if (colorDialog1.ShowDialog() == DialogResult.OK)
            //    {
            //        //DataGridViewCellStyle style = new DataGridViewCellStyle();
            //        // style.ForeColor = colorDialog1.Color;
            //        //style.BackColor = colorDialog1.Color; //Color.White ;                                                  
            //        DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[e.RowIndex].Cells[8];
            //        buttonCell.FlatStyle = FlatStyle.Popup;
            //        buttonCell.Style.BackColor = colorDialog1.Color;//System.Drawing.Color.Red;
            //        // dataGridView1.Rows[e.RowIndex].Cells[8].Style = b.Style;
            //        //.BackColor = colorDialog1.Color;


            //        //--We need to update to db as well

            //        try
            //        {
            //            //--Once the name is changed it has to save the changes
            //            string finalSize = dataGridView1.CurrentCell.Value.ToString();
            //            string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
            //            string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
            //            string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
            //            string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
            //            double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
            //            // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            //            double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
            //            bcs.CalculateYFromXandHumidity(xVal, humidity / 100);
            //            double yVal = bcs.y_coord_value;

            //            string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
            //            Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
            //            int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

            //            UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

            //        }
            //        catch { }



            //    }

            //}

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
              
        }

        private void dataGridView2_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && (e.ColumnIndex == 4))
            {
                if (colorDialog1.ShowDialog() == DialogResult.OK)
                {   

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[e.RowIndex].Cells[4];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = colorDialog1.Color;//System.Drawing.Color.Red;



                    //We need to update 
                    try

                    {
                       // string finalThickness = dataGridView2.CurrentCell.Value.ToString();

                        //Convert.ToString is for handling null event
                        string lineid = Convert.ToString(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[0].Value.ToString());
                        string prevNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[6].Value.ToString();
                        string nextNodeID = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        Color color = colorDialog1.Color;  //dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[3].Style.BackColor;
                        int thickness = int.Parse(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[5].Value.ToString());//finalThickness
                        Series s = new Series(dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        string lineName = dataGridView2.Rows[dataGridView2.CurrentCell.RowIndex].Cells[1].Value.ToString();

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
                    catch
                    { }
                 
                }
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

        }

        /*
        Lets define the datatype first for the same thing
        */

        public class TempDataType
        {
            public string id { get; set; } //--for identifying which point is selected..
            public double xVal { get; set; }//--this is the values that represent the point in a chart
            //public double yVal { get; set; }
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


        private void button1_Click(object sender, EventArgs e)
        {

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

        string initialTemp, initialHumidity, initialName, initialLabel, initialNodeSize;

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
            if (e.ColumnIndex == 9)
            {
              // dataGridView2.CommitEdit(DataGridViewDataErrorContexts.Commit);
            }

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
            RefreshDataFromDBAndChart();
            //--After refreshing
            LoadNodeAndLine();//--Loading the data 



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
                       // .                     

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


                    }
                    catch { }

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
                {
                    //This one is humidity
                    // initialHumidity = dataGridView1.CurrentCell.Value.ToString();
                    try
                    {

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
                        Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);



                    }
                    catch { }



                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 0)
                {
                    //This one is name
                    // initialName = dataGridView1.CurrentCell.Value.ToString();
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

                    try
                    {
                        //--Once the name is changed it has to save the changes
                        //string finalName = dataGridView1.CurrentCell.Value.ToString();
                        //string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                        //string nameVal = finalName; //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        //string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        //string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                        //double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                        //// double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        //double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        //bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                        //double yVal = bcs.y_coord_value;

                        //string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
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
                        Color colorVal = colorDialog1.Color;   //dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        int nodeSizeVal = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Value.ToString());
                        int AirFlow = int.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[9].Value.ToString());

                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);
                        updateNodeInfoBasedOnPresentValue(nodeIDVal, xVal, yVal, temperature_Source, humidity_Souce, nameVal, colorVal, nodeSizeVal, AirFlow);


                    }
                    catch { }





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

                        // string nodeIDVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[3].Value.ToString();
                        // string nameVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value.ToString();//--This contains the name
                        // string labelVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[1].Value.ToString();//--This contains the name
                        // string sourceVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[2].Value.ToString();//--This contains the name
                        // double xVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[4].Value.ToString());
                        // // double yVal = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        // double humidity = double.Parse(dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[5].Value.ToString());
                        // bcs.CalculateYFromXandHumidity(xVal, humidity / 100);

                        // double yVal = bcs.y_coord_value;

                        // string showTextVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[7].Value.ToString();
                        // Color colorVal = dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[8].Style.BackColor;
                        // int nodeSizeVal = int.Parse(finalSize);

                        //// MessageBox.Show("Node SIZE = " + nodeSizeVal);
                        // UpdateDataValueAndRefreshDGV(nodeIDVal, xVal, yVal, sourceVal, nameVal, labelVal, colorVal, showTextVal, nodeSizeVal);

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
            }
            catch
            {

            }


        }
    }
}
