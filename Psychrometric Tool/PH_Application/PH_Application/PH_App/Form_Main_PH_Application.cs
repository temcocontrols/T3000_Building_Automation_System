/*
 * Project : PH application
 * Author Name : Bhoj bahadur karki
 * Date : 2017-July-4th 
 * Contact : nishantkarki2013@hotmail.com
 */
using System;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using static PH_App.DatabaseOperationModel;

namespace PH_App
{
    public partial class Form_Main_PH_Application :Form//MainController// Form
    {

        public MainController mc;
        public InitialDatabaseSetupController idsc;
        public DatabaseOperationModel dom;
        public ObjectCollection ObjColl;
       

        public Form_Main_PH_Application()
        {
            InitializeComponent();
            //bo = new BuildingOperation();
            ObjColl = new ObjectCollection();
            //ObjColl.InitializeAllObjects();
            mc = ObjColl.mc;
            idsc = ObjColl.idsc;
            dom = ObjColl.dom;
           
        }
        public void objectInitialization()
        {
            ObjColl = new ObjectCollection();
            //ObjColl.InitializeAllObjects();
            mc = ObjColl.mc;
            idsc = ObjColl.idsc;
            dom = ObjColl.dom;
            //bo = ObjColl.bo;
        }
        Series seriesPoint = new Series("seriesNode1");
        
        private void Form_Main_PH_Application_Load(object sender, System.EventArgs e)
        {
            //--This is the main load application part
            //bo.ReadDataForBuildingSelectedFromPsychrometric();
            
        }

        string fluidName = "Water";//"1-Butene"; // "Water";//"n-Propane";//Acetone//Ammonia//Krypton//Nitrogen //Note for Air not working //Argon//CarbonDioxide// not working p-Xylene//R134a

        //--Pulling the fluid info 
        double Xmin = 0.001;
        double Xmax = 4000;
        double Ymin = 0.001;
        double Ymax = 100;
        
        public void Form_Main_PH_Application_Shown(object sender, EventArgs e)
        {
            try
            {
                //this.Enabled = false;//optional, better target a panel or specific controls
                //this.UseWaitCursor = true;//from the Form/Window instance
              
                //--Now lets pull the values from database

                mc.ReadDataForBuildingSelectedFromPsychrometric();
                mc.buildingSelectedInT3000 = mc.FindPathOfBuildingInT3000();
                /*
                 Steps: 1. Load building name,
                 2. pull the chart list 
                 3. if chart is present then pull the data from that list , pull fluid name
                 4. pull fluid detail 
                 5. pass it to plot function
                 */
                if (mc.buildingSelectedInT3000.Count > 0)
                { 
                mc.PullDataOfFluidInfo(mc.buildingSelectedInT3000[0].Building_Name);
                    if (mc.selectedBuildingList.Count > 0) { 
                mc.PullChartList(mc.selectedBuildingList[0].BuildingName);

                if (mc.chartDetailList.Count > 0) {
                 string chartIdForFluidName = mc.chartDetailList[0].chartID;
                mc.PullFluidForChartList(mc.selectedBuildingList[0].BuildingName);
                fluidChartType fluidInfoForName = mc.fluidForChartsList.Find(x => x.chartID == chartIdForFluidName);

                 string fluidName1 = fluidInfoForName.fluidName;
                  fluidProperty fluidDetail = mc.fluidInfo.Find(x => x.fluidName == fluidName1);

               
                //--Now we have the data
                if (mc.fluidInfo.Count > 0)
                {
                                //--We need to update the values
                                //fluidName = mc.fluidInfo[0].fluidName;
                                //Xmin = mc.fluidInfo[0].Xmin;
                                //Xmax = mc.fluidInfo[0].Xmax; //--No prob
                                //Ymin = mc.fluidInfo[0].Ymin;
                                //Ymax = mc.fluidInfo[0].Ymax;//--No prob                    

                                fluidName = fluidDetail.fluidName;
                                Xmin = fluidDetail.Xmin;
                                Xmax = fluidDetail.Xmax; //--No prob
                                Ymin = fluidDetail.Ymin;
                                Ymax = fluidDetail.Ymax;
                   }
                }
                }
                }
                // mc.LoadForPH(fluidName,this,Xmin,Xmax,Ymin,Ymax,xDiv,yDiv,xFlag,yFlag);//--
                mc.LoadForPH(fluidName, this, Xmin, Xmax, Ymin, Ymax);
                phChart.Series.Add(seriesPoint);
                phChart.Series.Add(mc.series1);
               //MessageBox.Show("Enthaly val(temp = 60deg,pre=20mpa) ="+Math.Round((CoolProp.PropsSI("H", "P", 40 * 1000000, "T", (10 + 273.15), "water") / 1000), 2));
                //MessageBox.Show("chart condn=" + phChart.Enabled);
                //phChart.Enabled = true; 
                mc.RefreshByLoadingDataWhileLoad(sender, e, this);
                }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            //finally
            //{
            //   this.Enabled = true;//optional
            //   this.UseWaitCursor = false;
            //}
        }

        private void phChart_MouseDown(object sender, MouseEventArgs e)
        {
            // This gives the corresponding X and Y coordinates of the mouse point.
            var pos = e.Location;
            if (prevPosition.HasValue && pos == prevPosition.Value)
                return;
            tooltip.RemoveAll();
            prevPosition = pos;
            var results = phChart.HitTest(pos.X, pos.Y, false,
                                         ChartElementType.PlottingArea);
            foreach (var result in results)
            {
                if (result.ChartElementType == ChartElementType.PlottingArea)
                {
                    var xVal = result.ChartArea.AxisX.PixelPositionToValue(pos.X);

                    var yVal = Math.Pow(10, (result.ChartArea.AxisY.PixelPositionToValue(pos.Y)));
                    xCoord = xVal;
                    yCoord = yVal;
                    //tooltip.Show("X=" + xVal + ", Y=" + yVal, this.phChart,
                    //pos.X, pos.Y - 15);
                }
            }
            mc.Chart_MouseDown(sender, e,this, MousePosition);            
        }

        double xCoord=0, yCoord=0;
        private void quickNodeInsertToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Now adding nodes
            try
            {
                //this.Enabled = false;//optional, better target a panel or specific controls
                //this.UseWaitCursor = true;//from the Form/Window instance
                mc.InsertNodeAndLine(phChart, xCoord, yCoord);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            //finally
            //{
            //    this.Enabled = true;//optional
            //    this.UseWaitCursor = false;
            //}
           

        }

        public void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //var bo = new PH_App.BuildingOperation();
           // mc.DGVCellClick(sender, e, this, phChart);
            //try
            //{
            //this.Enabled = false;//optional, better target a panel or specific controls
            //this.UseWaitCursor = true;//from the Form/Window instance
                mc.DGVCellClick(sender, e, this, phChart);
            //}
            //finally
            //{
            //    this.Enabled = true;//optional
            //    this.UseWaitCursor = false;
            //}
        }

        public void dataGridView1_SelectionChanged(object sender, EventArgs e)
        {
            mc.DGVSelectionChanged(sender, e, this);
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            
            mc.DGVCellBeginEdit(sender, e, this);
        }

        private void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            mc.DGVCellDoubleClick(sender, e, this);
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            mc.DGVCellEndEdit(sender, e, this);
        }

        private void dataGridView1_CellEnter(object sender, DataGridViewCellEventArgs e)
        {
            
            // dataGridView1_CellClick(sender, e);
            //--this calls the cell click methods
            mc.DGVCellClick(sender, e, this, this.phChart);

        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            mc.DGVCellMouseClick(sender, e, this,contextMenuStrip1);
        }

        private void editNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mc.LoadEditNodeDialog(this);
        }
        Point? prevPosition = null;
        ToolTip tooltip = new ToolTip();

        private void phChart_MouseMove(object sender, MouseEventArgs e)
        {
            try
            {            
            //mc.chart_MouseMove(sender, e, phChart, this, xCoord, yCoord);
            mc.chart_MouseMove(sender, e, phChart, this);
            }catch(Exception ex)
            {
                
            }
        }
        int countClicks = 0;
        private void phChart_MouseClick(object sender, MouseEventArgs e)
        {
            mc.chart_MouseClick(sender, e, phChart,this);

            if (mc.FlagForLockUnlock == 1) //Flag is disenabled
            {
                countClicks++;
                if(countClicks == 3)
                {
                    MessageBox.Show("Please unlock the lock icon and proceed forward!");
                    countClicks = 0;
                }
            }

        }

        private void clearChartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Clear chart option
            mc.ClearChart(sender, e, this);
           
        }

        private void deleteNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This one is for deleting node
            try
            {
                this.Enabled = false;//optional, better target a panel or specific controls
                this.UseWaitCursor = true;//from the Form/Window instance
                mc.DeleteNode(sender, e, this, phChart);
            }
            finally
            {
                this.Enabled = true;//optional
                this.UseWaitCursor = false;
            }
           

        }

        private void nodeAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--we will call here
            //Disconnect form A
            mc.DisconnectLineFromNodeA(sender, e, this, phChart);
        }

        private void nodeBToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Disconnect from B
            mc.DisconnectLineFromNodeB(sender, e, this, phChart);

        }

        private void saveConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This one is for save configuration
            mc.SaveConfiguration(this);
        }

        private void loadConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--This one is for load configuration
            mc.LoadConfiguration(this, phChart);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Closing application
            Close();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Deleting chart 
            mc.DeleteIndividualChart(sender, e, this, phChart);
        }

        private void saveAsTemplateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Save as template===
            mc.SaveAsTemplate(sender,e,this);
        }

        private void loadTemplateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Load as template file
            mc.LoadTemplate(sender, e, this, phChart);
        }

        private void excelExportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Contains the excell export
           mc.ExcelExportFunction(sender, e, this);
        }

        private void gridViewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mc.LoadEditNodeDialog(this);
        }

        private void pb_lock_unlock_Click(object sender, EventArgs e)
        {
            mc.LockAndUnlock(this);
        }

        private void backgroundWorker1_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            //--Background worker
            //--MessageBox.Show("Do worker");
            mc.RefreshDataFromDeviceAndWeb(this);
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, System.ComponentModel.RunWorkerCompletedEventArgs e)
        {
            //--BG Complete
            if(mc.flagForCompletingDataPullForBG == 1)
            { 
            mc.BackgroundWorkerRunWorkerCompleted(sender, e, this, phChart);
            }
        }

        //--This one is for refreshing the data
        /// <summary>
        /// Function should be called inside the boundary of the chart list
        /// </summary>
        public void RefreshChart(int index)
        {            
            int rowIndex = index;//dataGridView1.CurrentCell.RowIndex;
            var eventArgs = new DataGridViewCellEventArgs(1, rowIndex);           
            // or setting the selected cells manually before executing the function
            dataGridView1.Rows[rowIndex].Cells[1].Selected = true;
            dataGridView1_CellClick(this, eventArgs);            
        }

        private void trashBoxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Lets open the trash box
            try
            {
                TrashBox tb = new TrashBox(this);
                tb.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void simulationMode_Click(object sender, EventArgs e)
        {
            var v = new PHCalculator();
            v.ShowDialog();
        }

        private void helpPsychometricChartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Help file section
            //var path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //MessageBox.Show("PATH = " + path);
            try
            {
                string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string file = dir + @"\manual_ph_chart.chm";
                if (File.Exists(file))
                { 
                Help.ShowHelp(this, file);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var frmsetting = new FormSetting(this);
            frmsetting.ShowDialog();
        }

        private void lbFluidName_Click(object sender, EventArgs e)
        {
            if (mc.FlagForLockUnlock == 0) //Flag is enabled
            {
                //--This is function for on click for fluid info
                FormFluidSelection f = new FormFluidSelection(mc, this);
                f.ShowDialog();
            }
        }

        private void lbFluidName_MouseHover(object sender, EventArgs e)
        {            
            lbFluidName.BorderStyle = BorderStyle.FixedSingle;
            lbFluidName.ForeColor = Color.Blue;            
        }

        private void Form_Main_PH_Application_MouseHover(object sender, EventArgs e)
        {
            lbFluidName.ForeColor = Color.Black;
            lbFluidName.BorderStyle = BorderStyle.None;
        }

        private void phChart_MouseHover(object sender, EventArgs e)
        {
            //lbFluidName.ForeColor = Color.Black;
            //lbFluidName.BorderStyle = BorderStyle.None;
        }

        private void lbFluidName_MouseLeave(object sender, EventArgs e)
        {
            lbFluidName.ForeColor = Color.Black;
            lbFluidName.BorderStyle = BorderStyle.None;
        }

        private void Form_Main_PH_Application_KeyDown(object sender, KeyEventArgs e)
        {
            if(Keys.F1 == e.KeyCode)
            {
                try
                {
                    string dir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string file = dir + @"\manual_ph_chart.chm";
                    if (File.Exists(file))
                    {
                        Help.ShowHelp(this, file);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Print 
            mc.printHeatMap(this.phChart);
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mc.saveAsImageHeatMap(this.phChart,this);
        }

        public void RefreshChartListForTrashBoxRestore(Form_Main_PH_Application Fmain)
        {
            mc.RefreshingEverythingChartAndContent(Fmain, Fmain, phChart);
        }
    }
}
