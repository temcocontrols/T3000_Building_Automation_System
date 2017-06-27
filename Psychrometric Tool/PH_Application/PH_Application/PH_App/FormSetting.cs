using System;
using System.Configuration;
using System.Data.SQLite;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace PH_App
{
    public partial class FormSetting : Form
    {
        Form_Main_PH_Application f1;
        public FormSetting(Form_Main_PH_Application f)
        {
            f1 = f;
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //--Setting the form 
            if (cbFluidName.SelectedIndex >= 0)
            {

                if(cbFluidName.Text != "")
                {
                    try
                    {
                        this.Enabled = false;//optional, better target a panel or specific controls
                        this.UseWaitCursor = true;//from the Form/Window instance

                        string fname = cbFluidName.Text.Trim();
                    string tableName = "tbl_" + f1.lb_db_name.Text + "_FluidInfo";
                    SetAllFluidToZero(tableName);
                    SetSingleFluidToOne(tableName, fname);
                        // LoadingTheNewChart();
                     System.Diagnostics.Process.Start(Application.ExecutablePath); // to start new instance of application
                     f1.Close(); //to turn off current app
                    }
                    finally
                    {
                        this.Enabled = true;//optional
                        this.UseWaitCursor = false;
                    }
                }

                this.Close();
            }
        }

        //--Lets turn off the nodes and set the values

        
        public void SetAllFluidToZero(string tableName)
        {
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                string sqlQuery = "Update " + tableName+" SET selected='0'";
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, thisConnection);
                thisConnection.Open();
                cmd.ExecuteNonQuery();

            }//close of using
        }

       

        public void SetSingleFluidToOne(string tableName,string fluidName)
        {
            string databaseFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + ConfigurationManager.AppSettings["databaseName"];
            string connString1 = @"Data Source=" + databaseFile + ";Version=3;";
            using (SQLiteConnection thisConnection = new SQLiteConnection(connString1))
            {
                string sqlQuery = "Update " + tableName + " SET selected='1' WHERE fluidName='"+fluidName+"'";
                SQLiteCommand cmd = new SQLiteCommand(sqlQuery, thisConnection);
                thisConnection.Open();
                cmd.ExecuteNonQuery();

            }//close of using
        }


        public void LoadingTheNewChart()
        {
            if (f1.dataGridView1.Rows.Count > 0)  //If there is data then only do this one
            {
                if (f1.dataGridView1.CurrentCell.RowIndex >= 0 )
                {
                    var eventArgs = new DataGridViewCellEventArgs(0,0);
                    f1.Form_Main_PH_Application_Shown(f1, eventArgs);
                }
            }  
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void cbFluidName_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
