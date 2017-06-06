using System;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    public partial class Form_Main_PH_Application :Form//MainController// Form
    {

        public MainController mc;
        public InitialDatabaseSetupController idsc;
        public DatabaseOperationModel dom;
        //public BuildingOperation bo;
        public ObjectCollection ObjColl;
        //public ChartCreationAndOperations chartOprn;

        public Form_Main_PH_Application()
        {
            InitializeComponent();
            //bo = new BuildingOperation();
            ObjColl = new ObjectCollection();
            //ObjColl.InitializeAllObjects();
            mc = ObjColl.mc;
            idsc = ObjColl.idsc;
            dom = ObjColl.dom;
            //bo = ObjColl.bo;
            //chartOprn = ObjColl.coprn;
        }
        public void objectInitialization()
        {
            ObjColl = new ObjectCollection();
           // ObjColl.InitializeAllObjects();
            mc = ObjColl.mc;
            idsc = ObjColl.idsc;
            dom = ObjColl.dom;
           // bo = ObjColl.bo;
        }
        Series seriesPoint = new Series("seriesNode1");
        
        private void Form_Main_PH_Application_Load(object sender, System.EventArgs e)
        {
            //--This is the main load application part
            //bo.ReadDataForBuildingSelectedFromPsychrometric();
            
        }

        private void Form_Main_PH_Application_Shown(object sender, EventArgs e)
        {
            try
            {
                this.Enabled = false;//optional, better target a panel or specific controls
                this.UseWaitCursor = true;//from the Form/Window instance
                
                string fluidName = "Water";//"1-Butene"; // "Water";//"n-Propane";//Acetone//Ammonia//Krypton//Nitrogen //Note for Air not working //Argon//CarbonDioxide// not working p-Xylene//R134a
                //PH_App.ChartCreation ch = new PH_App.ChartCreation();
                //ch.PlotPHChart(fluidName, phChart);
                // lb_fluid_name.Text = "PH-Curve for " + fluidNAME;
                //var mc = new PH_App.MainController();
                mc.LoadForPH(fluidName,this);//--
                phChart.Series.Add(seriesPoint);
                phChart.Series.Add(mc.series1);
                mc.ReadDataForBuildingSelectedFromPsychrometric();
                mc.buildingSelectedInT3000 = mc.FindPathOfBuildingInT3000();
                // MessageBox.Show("chart condn=" + phChart.Enabled);
                // phChart.Enabled = true; 
                mc.RefreshByLoadingDataWhileLoad(sender, e, this);
                }
            finally
            {
               this.Enabled = true;//optional
               this.UseWaitCursor = false;
            }
        }

        private void phChart_MouseDown(object sender, MouseEventArgs e)
        {

            //if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            //{

            //        //==This shows the contextmenustrip on right click
            //        CMSinsertNode.Enabled = true;
            //      CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
            //}
            // MessageBox.Show("VALUES ="+chartOprn.ToString());
            // MessageBox.Show("Ph chart" + phChart.Enabled);
            mc.Chart_MouseDown(sender, e,this, MousePosition);
        }
        double xCoord=0, yCoord=0;
        private void quickNodeInsertToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Now adding nodes
            try
            {
                this.Enabled = false;//optional, better target a panel or specific controls
                this.UseWaitCursor = true;//from the Form/Window instance

                mc.InsertNodeAndLine(phChart, xCoord, yCoord);

            }
            finally
            {
                this.Enabled = true;//optional
                this.UseWaitCursor = false;
            }
           

        }

        public void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //var bo = new PH_App.BuildingOperation();

           // mc.DGVCellClick(sender, e, this, phChart);
            //try
            //{
            //    this.Enabled = false;//optional, better target a panel or specific controls
            //    this.UseWaitCursor = true;//from the Form/Window instance

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
                    
                    var yVal =Math.Pow(10, (result.ChartArea.AxisY.PixelPositionToValue(pos.Y)));
                    xCoord = xVal;
                    yCoord = yVal;
                    // tooltip.Show("X=" + xVal + ", Y=" + yVal, this.phChart,
                    //  pos.X, pos.Y - 15);
                    
                }
            }

            //mc.chart_MouseMove(sender, e, phChart, this, xCoord, yCoord);
            mc.chart_MouseMove(sender, e, phChart, this);



        }

        private void phChart_MouseClick(object sender, MouseEventArgs e)
        {
            mc.chart_MouseClick(sender, e, phChart,this);
           
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
            //MessageBox.Show("Do worker");
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
            var frmsetting = new FormSetting();
            frmsetting.ShowDialog();
        }

        public void RefreshChartListForTrashBoxRestore(Form_Main_PH_Application Fmain)
        {
            mc.RefreshingEverythingChartAndContent(Fmain, Fmain, phChart);
        }
    }
}
