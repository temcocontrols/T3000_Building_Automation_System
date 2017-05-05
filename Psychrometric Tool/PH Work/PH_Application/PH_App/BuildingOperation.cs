using System;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Reflection;

namespace PH_App
{
    public class  BuildingOperation : MainController
    {
        
        int flagResistingForDGVChangeSelection = 0;//disENABLE;
        string beginEditText = "";
        int flagSinglCellClick = 0;
        int FlagForCntdBG_Update = 1;//On currently
        public System.Timers.Timer atimer;
        int flagForTimer = 0;
        int FlagForLockUnlock = 0;//0 means lock and 1 means unlock
        int FlagForDissableLeftAndRightClicksInChart = 0;//==do not dissable /OFF
      public  int indexRun = 0;
      public  int FlagForStopingRunningProcessInstantly = 0;//Currently OFF
      public  int flagForInsertOrUpdateDataToDB = 0;

        public Form_Main_PH_Application frm1;//This one is for backgroundworker 

        int incrementIndex = 0;
        public int indexForWhichChartIsSelected = 0;//--This index value stores the index value in a list of which chart is selected
        public int indexOfChartSelected = 0;
        //int index=0;

        public void DGVSelectionChanged(object sender, EventArgs e, Form_Main_PH_Application f)
        {
            //--This need to fire an event when the selection is done 
          
            if (flagResistingForDGVChangeSelection == 1)
            {
                // MessageBox.Show("Test");
                return;
            }
          
            if (chartDetailList.Count > 0)
            {
                //try { 
                var cellCopy = f.dataGridView1.CurrentCell;

                if (cellCopy != null)
                {
                    if (f.dataGridView1.CurrentCell.ColumnIndex.ToString() != "" && f.dataGridView1.CurrentCell.RowIndex.ToString() != "")
                    {
                       
                        if (f.dataGridView1.CurrentCell.RowIndex >= 0 && f.dataGridView1.CurrentCell.RowIndex < chartDetailList.Count)
                        {
                            var eventArgs = new DataGridViewCellEventArgs(f.dataGridView1.CurrentCell.ColumnIndex, f.dataGridView1.CurrentCell.RowIndex);
                            //or setting the selected cells manually before executing the function
                            f.dataGridView1.Rows[f.dataGridView1.CurrentCell.RowIndex].Cells[f.dataGridView1.CurrentCell.ColumnIndex].Selected = true;
                            f.dataGridView1_CellClick(sender, eventArgs);
                            //MessageBox.Show("selecteion change clicked");
                        }
                    }

                }
                //}catch(Exception ex)
                //{
                //    MessageBox.Show(ex.Message);
                //}
            }
        }//Close of SelectionChange
        public void DGVCellBeginEdit(object sender, DataGridViewCellCancelEventArgs e, Form_Main_PH_Application f)
        {

            //--This start the editing of the cells
             if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                f.dataGridView1.Tag = f.dataGridView1.CurrentCell.Value;
                if (f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
                {
                    beginEditText = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                   
                }
            }
        }

        public void DGVCellEndEdit(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {


            // if(Control.ModifierKeys == Keys.Tab) { 
            //--Here we need to add datato the database...
            
            //try
            //{

                //MessageBox.Show("end edit");
                //--This section helps in validation..
                if (f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
                {
                    var regex = new Regex(@"^[a-zA-Z0-9_]+$");
                    string name = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                    // MessageBox.Show("name= " + name);
                    if (regex.IsMatch(name))
                    {
                        int length = name.Length;
                        if (length > 30)
                        {
                            //}
                            //else
                            //{

                            MessageBox.Show("You can only have name upto 30 characters");
                            f.dataGridView1.Refresh();
                            if (beginEditText == "")
                            {
                                f.dataGridView1.CurrentCell.Value = "";
                            }
                            else
                            {
                                f.dataGridView1.CurrentCell.Value = beginEditText;
                            }
                            return;
                            // }

                        }
                        //--Testing ..
                        //  MessageBox.Show(" name = " + name);
                        //--we need to check that the table name enter doesnot matches previous values

                        if (name != beginEditText)
                        {
                            for (int i = 0; i < chartDetailList.Count; i++)
                            {
                                if (name == chartDetailList[i].chartName)
                                {
                                    MessageBox.Show("chart can not have same name");
                                    f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = beginEditText;

                                    return;
                                }
                            }

                            //--The value is ok 
                            //--Now lets do the insert opeation for the data...



                            AddOrUpdateNewRecord(name, e,f);

                            //  MessageBox.Show("regx name");
                            //--now lets add new rows..
                            f.dataGridView1.Refresh();
                            //dataGridView1.Rows.Clear();//Remove the rows
                            // BeginInvoke(new MethodInvoker(PopulateControl));
                            //This flag is raised so that first coloumn is not selected everytime we do update
                            flagResistingForDGVChangeSelection = 1;//enable cell edit
                            f.dataGridView1.Rows.Clear();

                            //--Lets get the data and refill the data in datagridview.
                            // PullDataFromDB();


                            //LETS FILL THE DATA
                            // fillDataGridView();
                           // MainController mc = new MainController();
                            DataGridView_Show_Data(f);//This will do both pulling data and filing the data...

                            f.dataGridView1.Rows.Add();
                            f.dataGridView1.CurrentCell.Selected = false;

                            //===============This one for refreshing the chart and selecting new created chart===//

                            //if (chartDetailList.Count > 0)
                            //{

                            if (f.dataGridView1.Rows.Count > 0)
                            {
                                //We need to select the particular index 
                                //--I hope this will do the replotting thing as well
                                if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                                {
                                    f.dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;//The row is selected 
                                }

                            }


                            if (f.dataGridView1.Rows.Count > 0) //If there is data in datagridview then only
                            {
                                //MessageBox.Show("Clicked here row index: " + e.RowIndex);
                                var eventArgs = new DataGridViewCellEventArgs(1, e.RowIndex);
                                // or setting the selected cells manually before executing the function
                                // MessageBox.Show("END edit = " + e.RowIndex);
                                f.dataGridView1.Rows[e.RowIndex].Cells[1].Selected = true;
                                DGVCellClick(sender, eventArgs,f,f.phChart);
                                // }
                            }

                            flagResistingForDGVChangeSelection = 0;//close enable of dgv_selectionChange event 

                            //===================end of selection of new created chart =======================//
                        }
                        else
                        {
                            //  MessageBox.Show("No change in name");
                        }

                     
                    }
                    else
                    {
                        MessageBox.Show("You can only have letters, numbers and underscores");
                        f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = string.Empty;

                    }


                }


            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show("Edit and then press tab to commit changes " + ex.Message);

            //    f.dataGridView1.CurrentCell.Value = beginEditText;
            //    // }
            //}
        }

        public void AddOrUpdateNewRecord(string name, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {
            /*
            //--We need to count the index of the current datagridview if the row index 
                is greater then the data in the list of data             
            */
            if (e.RowIndex + 1 > chartDetailList.Count)
            {
                //insert the data...
                string nameValue = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                insertNewDataFromDGV(nameValue);

            }
            else
            {
                //--Update the data
                string chartIDValue = chartDetailList[e.RowIndex].chartID;
                string nameValue = f.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
                UpdateNewDataFromDGV(nameValue, chartIDValue);

            }
        }
        public void DGVCellDoubleClick(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f)
        {
            if (f.dataGridView1.CurrentCell.ColumnIndex == 1)
            {
                f.dataGridView1.CurrentCell.ReadOnly = false;
                f.dataGridView1.BeginEdit(true);

            }

        }

        int indexSelectedForDeletion = 0;

        public void DGVCellMouseClick(object sender, DataGridViewCellMouseEventArgs e, Form_Main_PH_Application f,ContextMenuStrip cms)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.RowIndex >= 0 && e.ColumnIndex == 0)
                {
                    if (f.dataGridView1.Rows[e.RowIndex].Cells[0].Value != null)
                    {
                        Point p = new Point(e.X, e.Y);
                        indexSelectedForDeletion = e.RowIndex;
                 
                        int xAxis = (int)e.X;
                        //--now alsow count the row index so that yavis = e.y+ (rowIndex*constantMultiple)
                        int rowIndex = f.dataGridView1.CurrentCell.RowIndex;
                        int yAxis = (int)e.Y + (rowIndex * 22);

                        //new Point((int)e.X, (int)e.Y)
                        //contextMenuStrip1.Show(dataGridView1, new Point(xAxis, yAxis));
                        cms.Show(f.dataGridView1, new Point(xAxis, yAxis));
                        //contextMenuStrip1.Show(dataGridView1.PointToScreen(e.Location));
                    }
                }


            }
        }
        public void DGVCellClick(object sender, DataGridViewCellEventArgs e, Form_Main_PH_Application f,System.Windows.Forms.DataVisualization.Charting.Chart chart1)
        {

            if (f.dataGridView1.Rows.Count <= 0)
            {
                return;
            }

            if (flagForTimer == 1)
            {
                if (atimer.Enabled) // Check if the timer is running
                {
                    //Return error.........
                    atimer.Enabled = false;
                    atimer.Dispose();
                    flagForTimer = 0;

                }
            }//close of flagfortimer


            //--Lets check if the datagridview1 is empty or chartDetailList is empty
            if (chartDetailList.Count <= 0)
            {
                //--This flag is for OFFLINE mode Or ONLINE mode
                //--Now you are going to turn OFF the offline mode and go into realtime mode
                FlagForCntdBG_Update = 0;//currently in realtime mode so 1


                //--also make the radio button to be OFF and other to be ON
                // rb_ON.Checked = true;

                //--IFno chart is present thMMen make edit mode on
                FlagForLockUnlock = 1;//   This means edit mode ON 
                LockAndUnlock(f);//This method will make things lock

                return;
            }

            //--Showing the data on cell selected...
            // MessageBox.Show("CELL SELECT " );
            //When dgv is click it clicks twice this if is written to stop those twice click.

            if (flagSinglCellClick == 1)
            {
                flagSinglCellClick = 0;
                return;
            }
            if ((e.RowIndex >= 0) && (e.ColumnIndex >= 0))
            {

                if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                {
                    // MessageBox.Show("CELL SELECT ROW= " + e.RowIndex+",columns = "+e.ColumnIndex);
                    //The row is selected ..
                    for (int i = 0; i < chartDetailList.Count; i++)
                    {
                        //--Checking for evey item
                        if (f.dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString() == chartDetailList[i].chartName)
                        {
                            //if match found load
                            //data_load();
                            //lb_title_display.Text = "";//--Historical plot needs to be cleared
                            frm1 = f;//Copying the object for further use

                            indexRun = 0;//For inserting nodeName and lineName like node1,line1 etc
                            chart1.Enabled = true;

                            //--This flag is for OFFLINE mode Or ONLINE mode
                            //--Now you are going to turn OFF the offline mode and go into realtime mode
                            FlagForCntdBG_Update = 1;//currently in realtime mode so 1

                            //--also make the radio button to be OFF and other to be ON
                            // rb_OFF.Checked = true;

                            FlagForLockUnlock = 0;//   This means edit mode OFF 
                            LockAndUnlock(f);//This method will make things lock

                            //This one(the signal) is for stoping the background running process instantly 
                            FlagForStopingRunningProcessInstantly = 1;


                            RefreshGraph(chart1, f);
                            /*
                            We need to identify which item in the list was clicked .
                              //Before we load line we need to identify the id of the chart to load the data..
                              */
                            //chartSelectedIDValue = chartDetailList[i].chartID;//--tHIS CONTAINS THE ID OF THE CHART
                            indexForWhichChartIsSelected = i;
                            //This has been changed
                            indexOfChartSelected = e.RowIndex;    //This value is changed 
                            LoadNodeAndLineFromDB(indexOfChartSelected);   //Lets make it passing the stirngs 

                            flagForInsertOrUpdateDataToDB = 1;
                            //--This is also completed..
                            //var ch = new ChartOperation();
                             ReDrawingLineAndNode();  //Done checking bbk modif: changing to alex db 


                            flagSinglCellClick = 1;

                           
                            //==============This part is for comfort zone=====================
                           
                            //====================end of comfort zone=========================

                            //---Here we are going to call scan hardware function ----------------//

                            //StartHardwareScan();
                            //---End of call for scan hardware-----------------------------------//

                            //--Lets make the status of the device and web to be dissabled 
                             //lb_web_status.Text = "inactive";
                            f.lb_device_status.Text = "disconnected";

                            break;
                        }
                        else
                        {
                            flagForInsertOrUpdateDataToDB = 0;
                            RefreshGraph(chart1,f);
                        }
                    }


                }  //close of != null if
                else
                {
                    flagForInsertOrUpdateDataToDB = 0; //==insert or update OFF
                }


                //This is for cell formating and editing...
                //  if (dataGridView1.CurrentCell.ColumnIndex >= 0) { 
                try
                {
                    if (f.dataGridView1.CurrentCell.ColumnIndex == 1)
                    {
                        // this.dataGridView1.BeginEdit(false);
                        f.dataGridView1.CurrentCell.ReadOnly = true;
                    }
                }
                catch { }
              
                if (f.backgroundWorker1.IsBusy)
                {
                    // MessageBox.Show("background asyc dissabled");
                    f.backgroundWorker1.CancelAsync(); //==First dissabling background worker

                }
               
                if (FlagForCntdBG_Update == 1)
                {
                    // MessageBox.Show("Entered");//--test
                    // chart1.Enabled = false;//should be dissabled 
                    //If the chart is in online mode then only do updating task
                    //--***********************Uncomment later********************//
                    InitTimerForDevice();
                   
                    //--*************************end*****************************//
                }
                else
                {
                    // chart1.Enabled = true;
                }
            }//close of columnindex if
            else if (e.RowIndex >= 0 && (e.ColumnIndex >= 0 && e.ColumnIndex <= 1))
            {
                //we need to return and does not proceed forwared
                //select all 
                return;
            }


        }
        public void LockAndUnlock(Form_Main_PH_Application f)
        {
            //--We need to load and unload the image 
            if (FlagForLockUnlock == 0)
            {
                //Edit mode is dissabled //Edit OFF section
                //--Load image lock image
                // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);


                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = imagePath + @"\..\Database\image\lock.png";

                Bitmap SOME = new Bitmap(Image.FromFile(file));

                Bitmap bp = new Bitmap(SOME, f.pb_lock_unlock.Width, f.pb_lock_unlock.Height);
                f.pb_lock_unlock.Image = bp;//Image.FromFile(file);

                //==Code form edit OFF
                FlagForCntdBG_Update = 1;//On currently
                FlagForDissableLeftAndRightClicksInChart = 1;//turn on 
                                                             //==End of edit OFF
                FlagForLockUnlock = 1;
                f.lb_realtimeshow.Text = "Real Time Mode";
            }
            else
            {
                //Edit mode is enable
                //--Load image lock image
                // string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                string imagePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = imagePath + @"\..\Database\image\unlock.png";

                Bitmap SOME = new Bitmap(Image.FromFile(file));
                Bitmap bp = new Bitmap(SOME, f.pb_lock_unlock.Width, f.pb_lock_unlock.Height);

                f.pb_lock_unlock.Image = bp;//Image.FromFile(file);
                //==============Edit ON section these flags make the Edit on=====//
                FlagForCntdBG_Update = 0;//OFF currently
                FlagForDissableLeftAndRightClicksInChart = 0;//==TURN OFF
                //==============End of edit on flag decleration =========//

                FlagForLockUnlock = 0;
                f.lb_realtimeshow.Text = "Edit Mode";
            }
        }

        public void InitTimerForDevice()
        {

            // MessageBox.Show("inside timer");
            flagForTimer = 1;
            atimer = new System.Timers.Timer();
            atimer.Enabled = true;
            atimer.Elapsed += timer1_Tick_For_Device;
            atimer.Interval = 1000 * 5; //x seconds[ 1000 ms * x  =  x seconds]

        }

        //public void timer1_Tick_For_Device(object sender, EventArgs e)
        public void timer1_Tick_For_Device(object sender, EventArgs e)
        {

            try
            {
                if (FlagForCntdBG_Update == 0)
                {
                    return;
                }
                else
                {
                    //var f = new Form_Main_PH_Application();
                    if (!frm1.backgroundWorker1.IsBusy)
                    {

                        frm1.backgroundWorker1.RunWorkerAsync();//--Running the worker async

                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        public void RefreshGraph(System.Windows.Forms.DataVisualization.Charting.Chart chart1,Form_Main_PH_Application f)
        {
           
            f.Invalidate();
            chart1.Invalidate();
            //plot_new_graph(); //--This one needs to be reinvented

           // NodeAndLineClass nc = new NodeAndLineClass();
            //--Reseting the menustrip values for new plotting....
           listLineInfoValues.Clear();
            listNodeInfoValues.Clear();
            index = 0;  //This is resetting the index values
            incrementIndex = 0;
           // ReloadComfortZoneForBackGroundWorker();
           
        }
        public void dataGridView1_DataBindingComplete(object sender, DataGridViewBindingCompleteEventArgs e, Form_Main_PH_Application f)
        {
            //MessageBox.Show("ok");
            f.dataGridView1.ClearSelection();
            //MessageBox.Show("Databinding");
            f.dataGridView1.SelectionChanged -= f.dataGridView1_SelectionChanged;

            // flagDataBindingCompleteForSelection = 1;
        }
    }
}
