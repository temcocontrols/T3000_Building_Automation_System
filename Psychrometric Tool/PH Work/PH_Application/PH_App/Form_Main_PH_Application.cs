using System;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace PH_App
{
    public partial class Form_Main_PH_Application : Form
    {
          BuildingOperation bo;
        public Form_Main_PH_Application(BuildingOperation b)
        {
            InitializeComponent();
            bo = b;//new BuildingOperation();
            bo.ReadDataForBuildingSelectedFromPsychrometric();
            
        }
        Series seriesPoint = new Series("seriesNode1");
        
        private void Form_Main_PH_Application_Load(object sender, System.EventArgs e)
        {
            //--This is the main load application part
            

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
                var mc = new PH_App.MainController();
                mc.LoadForPH(fluidName,this);//--
                phChart.Series.Add(seriesPoint);

            }
            finally
            {
                this.Enabled = true;//optional
                this.UseWaitCursor = false;
            }
        }

        private void phChart_MouseDown(object sender, MouseEventArgs e)
        {

            if (e.Button == MouseButtons.Right)//on right mouse button is clicked.
            {

               
               
                    //==This shows the contextmenustrip on right click
                    CMSinsertNode.Enabled = true;
                    CMSinsertNode.Show(MousePosition);//-- this mouse position is used to show the menustrip in mouse pointer
                
            }

        }

        private void quickNodeInsertToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //--Now adding nodes 


        }

        private void phChart_Click(object sender, EventArgs e)
        {

        }

        public void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //var bo = new PH_App.BuildingOperation();
            bo.DGVCellClick(sender, e, this, this.phChart);
        }

        public void dataGridView1_SelectionChanged(object sender, EventArgs e)
        {
            bo.DGVSelectionChanged(sender, e, this);
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            
            bo.DGVCellBeginEdit(sender, e, this);
        }

        private void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            bo.DGVCellDoubleClick(sender, e, this);
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            bo.DGVCellEndEdit(sender, e, this);
        }

        private void dataGridView1_CellEnter(object sender, DataGridViewCellEventArgs e)
        {
            
            // dataGridView1_CellClick(sender, e);
            //--this calls the cell click methods
            bo.DGVCellClick(sender, e, this, this.phChart);

        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            bo.DGVCellMouseClick(sender, e, this,this.CMSinsertNode);
        }
    }
}
