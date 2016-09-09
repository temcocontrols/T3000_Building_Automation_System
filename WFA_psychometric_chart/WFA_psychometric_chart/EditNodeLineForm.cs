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
        public buildingChartSetting bcs;
        public EditNodeLineForm(buildingChartSetting b)
        {
            this.bcs = b;
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void EditNodeLineForm_Load(object sender, EventArgs e)
        {
            //This on load we need to laod the data from the other form
            LoadNoadAndLine();//load values
        }
        public void LoadNoadAndLine()
        {
            //dataGridView1.Rows.Clear();
            /*
            steps:1. get the data from the list values..
                  2.display the data
                    a) display the background color of the button in the color value and when clicked show the colordialog toselect
                    b) and the text in the list
            */
            //--First lets do for the node
            if(bcs.menuStripNodeInfoValues.Count>0) { 
        for(int i = 0; i < bcs.menuStripNodeInfoValues.Count; i++)
            {
                    //Now lets display the values..


                   // DataGridViewCell cx = (DataGridViewCell)dataGridView1.Rows[i].Cells[4];
                    DataGridViewComboBoxCell cx = (DataGridViewComboBoxCell)dataGridView1.Rows[i].Cells[4];
                    int selectIndex = 0;
                    string value;
                    int value1;


                    //DataGridViewComboBoxColumn ColumnPage = dataGridView1.Rows[i].Cells[4];//(DataGridViewComboBoxColumn)dataGridView1.Rows[i].Cells[4];//new DataGridViewComboBoxColumn();
                    //ColumnPage.DefaultCellStyle.NullValue = "Medium";


                    //Now identifying the item selected.
                    if (bcs.menuStripNodeInfoValues[i].showItemText == "Name")
                    {
                       // selectIndex = 0;
                        // showText.ValueMember = "Name";
                        // showText.DisplayMember = "Name";
                        // cx.ValueMember = cx.Items[0].ToString();  // "Name";
                        //cx.DisplayMember = "Name";
                        //dataGridView1.Rows[i].Cells[4].Value = "Name";
                        // cx.DisplayMember = cx.Items[0].ToString();
                        // showText.DisplayMember = cx.Items[0].ToString();
                        //dataGridView1.Rows[i].Cells[4].n
                        //dataGridView1.Rows[i].Cells[4].Value = "0";
                  
                       // showText.DefaultCellStyle.NullValue = "Name";
                        value = "Name";
                      //  value1 = 0;
                    }
                    else if (bcs.menuStripNodeInfoValues[i].showItemText == "Source")
                    {
                       // selectIndex = 1;
                        //showText.ValueMember = "Source";
                        //showText.DisplayMember = "Source";
                        //cx.ValueMember = cx.Items[1].ToString();//"Source";
                        //cx.DisplayMember = "Source";

                        // dataGridView1.Rows[i].Cells[4].Value = "Source";
                        //cx.DisplayMember = cx.Items[1].ToString();
                        //showText.DisplayMember = cx.Items[1].ToString();
                        //dataGridView1.Rows[i].Cells[4].Value = "1";
                        //showText.DefaultCellStyle.NullValue = "Source";
                        value = "Source";
                      //  value1 = 1;
                    }
                    else
                    {
                      //  selectIndex = 2;
                        //showText.ValueMember = "Label";
                        //showText.DisplayMember = "Label";
                        // cx.ValueMember = cx.Items[2].ToString();//"Label";
                        // cx.DisplayMember = "Label";
                        // dataGridView1.Rows[i].Cells[4].Value = "Label";
                        //cx.DisplayMember = cx.Items[2].ToString();
                        // showText.DisplayMember = cx.Items[2].ToString();
                      //   dataGridView1.Rows[i].Cells[4].Value = "2";
                      //  showText.DefaultCellStyle.NullValue = "Label";
                        value = "Label";
                    //    value1 = 2;
                    }
                    // cell.DisplayIndex = selectIndex;//This index should be display

                    //showText.HeaderText = "nowthing";
                    //DataGridViewComboBoxColumn cmb = showText;
               //showText.ValueMember =  showText.Items[selectIndex].ToString();


                    bcs.enthalpyHumidityCalculatorForXYvalue(bcs.menuStripNodeInfoValues[i].xVal, bcs.menuStripNodeInfoValues[i].yVal);
                string [] row = new string[]{ bcs.menuStripNodeInfoValues[i].id,Math.Round(bcs.menuStripNodeInfoValues[i].xVal, 2).ToString(), Math.Round(bcs.humDataGridValue,2).ToString(),Math.Round(bcs.enthalpyDataGridView,2).ToString(), value, bcs.menuStripNodeInfoValues[i].name, bcs.menuStripNodeInfoValues[i].source, bcs.menuStripNodeInfoValues[i].label, " ",bcs.menuStripNodeInfoValues[i].marker_Size.ToString() };
                dataGridView1.Rows.Add(row);

                   

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[i].Cells[8];
                buttonCell.FlatStyle = FlatStyle.Popup;
                buttonCell.Style.BackColor = bcs.menuStripNodeInfoValues[i].colorValue;

            }
            }//clsoe of if value

            //--Second : This is for the line to show in the datagridview2
            if (bcs.menuStripNodeLineInfoValues.Count > 0)
            {
                for(int i = 0; i < bcs.menuStripNodeLineInfoValues.Count; i++)
                {
                    //now lets display...
                    string[] row = new string[] { bcs.menuStripNodeLineInfoValues[i].ID, bcs.menuStripNodeLineInfoValues[i].prevNodeId.ToString(), bcs.menuStripNodeLineInfoValues[i].nextNodeId.ToString(), "", bcs.menuStripNodeLineInfoValues[i].lineThickness.ToString() };
                    dataGridView2.Rows.Add(row);

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[i].Cells[3];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = bcs.menuStripNodeLineInfoValues[i].lineColorValue;

                }

            }


        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //This part contains the cell click values
            if((e.RowIndex>=0 && e.RowIndex<bcs.menuStripNodeInfoValues.Count) && (e.ColumnIndex == 8))
            {
                if (colorDialog1.ShowDialog() == DialogResult.OK)
                {
                    //DataGridViewCellStyle style = new DataGridViewCellStyle();
                    // style.ForeColor = colorDialog1.Color;
                    //style.BackColor = colorDialog1.Color; //Color.White ;
                                                  
                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView1.Rows[e.RowIndex].Cells[8];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = colorDialog1.Color;//System.Drawing.Color.Red;

                    // dataGridView1.Rows[e.RowIndex].Cells[8].Style = b.Style;
                    //.BackColor = colorDialog1.Color;

                }

            }
        }

        private void dataGridView2_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if ((e.RowIndex >= 0 && e.RowIndex < bcs.menuStripNodeLineInfoValues.Count) && (e.ColumnIndex == 3))
            {
                if (colorDialog1.ShowDialog() == DialogResult.OK)
                {   

                    DataGridViewButtonCell buttonCell = (DataGridViewButtonCell)dataGridView2.Rows[e.RowIndex].Cells[3];
                    buttonCell.FlatStyle = FlatStyle.Popup;
                    buttonCell.Style.BackColor = colorDialog1.Color;//System.Drawing.Color.Red;
                
                }

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


        private void button1_Click(object sender, EventArgs e)
        {
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
                    marker_Size = int.Parse( dataGridView1.Rows[i].Cells[9].Value.ToString())                                                                                             
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

        private void dataGridView2_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            //Thi part is for regular expression 
            if (dataGridView2.CurrentCell.ColumnIndex == 4)
            {
                //This is the value of the thickneess
                initialThickness = dataGridView2.CurrentCell.Value.ToString();

            }

        }

        string initialTemp, initialHumidity, initialName, initialLabel, initialNodeSize;

        private void dataGridView2_CellEndEdit(object sender, DataGridViewCellEventArgs e)
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
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("You can only input integer values");

                dataGridView2.CurrentCell.Value = initialThickness;
            }


        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {

            try {

                //Lets record previous data here 
                if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    try {
                        //This one is temperature
                        double finalTemp = double.Parse(dataGridView1.CurrentCell.Value.ToString());
                        // Create the regular expression
                        // string pattern = @"\w+_";
                        // Regex regex = new Regex(pattern);

                        // Compare a string against the regular expression
                        // return regex.IsMatch(finalTemp);
                    } catch (Exception ex)
                    {
                        MessageBox.Show("You can only input number values");

                        dataGridView1.CurrentCell.Value = initialTemp;
                    }

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 2)
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
                    }

                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 5)
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
                        }

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input letters,numbers and underscores values");

                        dataGridView1.CurrentCell.Value = initialName;
                    }



                }
                //else if (dataGridView1.CurrentCell.ColumnIndex == 6)
                //{
                //    //This one is source ==> non editable now
                //}
                else if (dataGridView1.CurrentCell.ColumnIndex == 7)
                {
                    //This one is lable
                    //initialLabel = dataGridView1.CurrentCell.Value.ToString();

                    try
                    {

                        string finalLabel = dataGridView1.CurrentCell.Value.ToString();

                        string pattern = @"^\w+$";// @"^[a-zA-Z0-9\_]+$"; //for string 
                        Regex rgx = new Regex(pattern);
                        //string sentence = "Who writes these notes?";
                        if (rgx.IsMatch(finalLabel) == true)
                        {
                            //matched no need to edit 
                        }
                        else
                        {
                            MessageBox.Show("You can only input letters,numbers and underscores values");
                            dataGridView1.CurrentCell.Value = initialLabel;
                        }

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input letters,numbers and underscores values");

                        dataGridView1.CurrentCell.Value = initialLabel;
                    }


                }
                else if (dataGridView1.CurrentCell.ColumnIndex == 9)
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
                        }

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("You can only input integer values");

                        dataGridView1.CurrentCell.Value = initialNodeSize;
                    }



                }
                }
            catch
            {

            }

            }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {

            try { 
            //Lets record previous data here 
            if(dataGridView1.CurrentCell.ColumnIndex == 1)
            {
                //This one is temperature
                initialTemp = dataGridView1.CurrentCell.Value.ToString();
            }
            else if (dataGridView1.CurrentCell.ColumnIndex == 2)
            {
                //This one is humidity
                initialHumidity = dataGridView1.CurrentCell.Value.ToString();

            }
            else if (dataGridView1.CurrentCell.ColumnIndex == 5)
            {
                //This one is name
                initialName = dataGridView1.CurrentCell.Value.ToString();

            }                                                          
            else if (dataGridView1.CurrentCell.ColumnIndex == 7)
            {
                //This one is lable
                initialLabel = dataGridView1.CurrentCell.Value.ToString();
            }
            else if (dataGridView1.CurrentCell.ColumnIndex == 9)
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
