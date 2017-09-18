using System;
using System.Configuration;
using System.Data.SQLite;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace PH_App
{
    public partial class FormFluidSelection : Form
    {
        MainController mc;
        Form_Main_PH_Application fmain;
        public FormFluidSelection(MainController mf,Form_Main_PH_Application form)
        {
            mc = mf;
            fmain= form;
            InitializeComponent();
        }
        
        private void FormFluidSelection_Load(object sender, EventArgs e)
        {
            cbFluidName.SelectedIndex = 0;
           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try { 
            //--Ok button is pressed we need to check the chart property first
            if (mc.listNodeInfoValues.Count == 0)
            {
              if (mc.chartDetailList.Count > 0) { 
                //--Now lets call the function for plotting 
                //--Pulling chart list
                int rowIndex = fmain.dataGridView1.CurrentCell.RowIndex;
                int columnIndex = fmain.dataGridView1.CurrentCell.ColumnIndex;
                if (rowIndex >= 0 && rowIndex < mc.chartDetailList.Count)
               {
               //--Now lets set the fluid name to selected fluidName
                 string tableName = "tbl_" + fmain.lb_db_name.Text + "_chart_fluid_info";
                string fluidName = cbFluidName.Text.Trim();
                string chartID = mc.chartDetailList[rowIndex].chartID;
                UpdateFluidNameForChartTable(tableName, fluidName, chartID);


                mc.DataGridView_Show_Data(fmain);
                            //--Now after showing we need to pull the chart list 

                //if (columnIndex >0 )
                // {
                                // var eventArgs = new DataGridViewCellEventArgs(columnIndex, rowIndex);
                                // //or setting the selected cells manually before executing the function
                                //fmain.dataGridView1.Rows[rowIndex].Cells[columnIndex].Selected = true;
                                //fmain.dataGridView1_CellClick(this, eventArgs);

                     var eventArgs = new DataGridViewCellEventArgs(0, 0);
                      //or setting the selected cells manually before executing the function
                     fmain.dataGridView1.Rows[0].Cells[0].Selected = true;
                    fmain.dataGridView1_CellClick(this, eventArgs);


                 //}

                 }
               else
                {
                            if (fmain.dataGridView1.RowCount > 0)
                            {
                                var eventArgs = new DataGridViewCellEventArgs(1, 0);
                                //or setting the selected cells manually before executing the function
                                fmain.dataGridView1.Rows[0].Cells[1].Selected = true;
                                fmain.dataGridView1_CellClick(this, eventArgs);
                            }
               }
            }

            }else
            {
                MessageBox.Show("Please clear the chart first");
            }
            this.Close();
            }catch(Exception ex)
            {
                MessageBox.Show(ex.Message);

            }
        }

        public void UpdateFluidNameForChartTable(string FluidTableName, string fluidName, string chartId)
        {
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                thisConnection.Open();
                string sqlQuery = "Update " + FluidTableName + " SET fluidName='" + fluidName + "' Where chartID = '" + chartId + "'";
                //MessageBox.Show(sqlQuery);
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, thisConnection);               
                cmd.ExecuteNonQuery();
            }//close of using
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
                 
    }
}
