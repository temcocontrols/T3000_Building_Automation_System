using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class TrashBox : Form
    {
        Form1_main f1;
        public TrashBox(Form1_main f)
        {
            f1 = f;
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void TrashBox_Load(object sender, EventArgs e)
        {
            //--Values which have enableStatus = false should be loaded first
            LoadDataFromDB();
        }
        string buildingNameGlobal = "";

        /// <summary>
        /// Functions helps to load teh values that hare dissabled in the list
        /// </summary>
        

        public void LoadDataFromDB()
        {

            //--We need to  pass the buildingName value
            Form1_main f1 = new Form1_main();
            f1.CheckSelectedBuilding();
            string buildingName = f1.selectedBuildingList[0].BuildingName;
            // f1.CurrentSelectedBuilding;//f1.selectedBuildingList[0].BuildingName;
            buildingNameGlobal = buildingName;

           // MessageBox.Show("BuildingName = " + buildingNameGlobal);
            if(buildingNameGlobal == "")
            {
                //If no building is selected then do not go futher 
                return;
            }

            PullChartList(buildingName);
           // MessageBox.Show("number of list value items =  " + chartDetailListForDissabledValue.Count);

            //--Now lets load the values in the database

            fillDataGridView();//This will load the values form db



        }

        private void fillDataGridView()
        {


            int xCount = 0;
            if (chartDetailListForDissabledValue.Count > 0)
            {
                for (int i = 0; i < chartDetailListForDissabledValue.Count; i++)
                {
                    if (chartDetailListForDissabledValue[i].enableChartStatus == "false") //when enable then only display
                    {
                        dataGridView1.Rows.Add();
                        dataGridView1.Rows[i].Cells["Count"].Value = ++xCount;//chartDetailList[i].;
                        dataGridView1.Rows[i].Cells["chartID"].Value = chartDetailListForDissabledValue[i].chartID; //++xCount;//chartDetailList[i].;
                        dataGridView1.Rows[i].Cells["ChartName"].Value = chartDetailListForDissabledValue[i].chartName;
                       // dataGridView1.Rows[i].Cells["RestoreChartCheckBox"].Value = "";
                       // MessageBox.Show("entered " + i);
                    }
                }
            }//--if close

          //  lb_numberOfHandler.Text = chartDetailList.Count.ToString();


        }//--close fo filldatagridview


        public class chartDetailDT
        {
            public int count { get; set; }
            public string chartID { get; set; }
            public string chartName { get; set; }
            public string chart_respective_nodeID { get; set; }
            public string chart_respective_lineID { get; set; }
            public string enableChartStatus { get; set; }
        }
        public List<chartDetailDT> chartDetailListForDissabledValue = new List<chartDetailDT>();//This is used for storing the chart detail ids

        public void PullChartList(string buildingName)
        {

            chartDetailListForDissabledValue.Clear();//resetting the chart list value..
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";
            MessageBox.Show("connection string " + connString);

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "select * from " + tableForChartDetail + " where enableChartStatus ='false' ";

                SQLiteCommand command = new SQLiteCommand(sql, connection);

                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    //This is the reading part of the data...
                    chartDetailListForDissabledValue.Add(new chartDetailDT
                    {
                        count = int.Parse(reader["count"].ToString()),
                        chartID = reader["chartID"].ToString(),
                        chartName = reader["chartName"].ToString(),
                        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                        chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                        enableChartStatus = reader["enableChartStatus"].ToString() //either true or false

                    });
                }
            }

        }  //Close of the function  PullChartList


        //List to store the id and name of the chart
        ArrayList arrayList = new ArrayList();
        private void button1_Click(object sender, EventArgs e)
        {
            //--After applying ok we will restore all the data that has been 
            //--triggered as false as to be true and load the value again 
            //--Also close the window as soon as  it is set
            /*
           Steps : 
           1.Check for all the check boxes and restore
           */
           if(dataGridView1.RowCount<= 0)
            {
                //--If there is no data don't need to do futher processing
                return;
            }

          

            arrayList.Clear();//Empty the arraylist 
            for(int i =0;i<dataGridView1.RowCount;i++)
            {
                //--In rows we are looping .
                if(Convert.ToBoolean(dataGridView1.Rows[i].Cells[3].Value)) //Will be true if checkedd  
                {
                    arrayList.Add(dataGridView1.Rows[i].Cells[1].Value.ToString());//Insert the chart id values
                    //storeRestoreList.Add(chartDetailListForDissabledValue[i].chartID);
                   // MessageBox.Show("Value = " + dataGridView1.Rows[i].Cells[1].Value.ToString());
                }        
            }

            //MessageBox.Show("StoreRestoreList count =" + arrayList.Count);
            //--Now lets remove the checked values form the database sections

            //string buildingName = 
            for(int i=0;i< arrayList.Count;i++)
            {
                updateValueInDatabaseToTrue(buildingNameGlobal, arrayList[i].ToString());
            }

            //--Now after updating call the datagridview_show_Data function in main chart and close the 
            //window

           // Form1_main f1 = new Form1_main();
            //Form1_main f1 = new Form1_main();
            //f1.CheckSelectedBuilding();

            //--We are claling the dataGridView_Show_Data is called
          //  MessageBox.Show("DataGridView_show_Data Calls");

            // f1.Form1_Load(sender, e);
            // f1.LoadFunctionForTrash();
            //f1.chart1RefreshCustom();
            //f1.dataGridView1.Rows.Clear();
            //f1.DataGridView_Show_Data();

            //f1.dataGridView1.Rows.Add();
            //f1.Refresh();
            //f1.RefreshGraph();//Refresh the graph
            //--Now close this form 

            f1.RefreshingEverythingChartAndContent(sender);//Calling the function for refresh


            this.Close();

              
        }

       public void updateValueInDatabaseToTrue(string buildingName,string chartID)
        {
            //--Now lets make the check state to be true
            string tableForChartDetail = "tbl_" + buildingName + "_chart_detail";
            string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string databaseFile = databasePath + @"\db_psychrometric_project.s3db";
            string connString = @"Data Source=" + databaseFile + ";Version=3;";

            using (SQLiteConnection connection = new SQLiteConnection(connString))
            {
                connection.Open();

                //--We also need to create table for particular data added..

                string sql = "UPDATE " + tableForChartDetail + " SET  enableChartStatus ='true' where chartID = '"+chartID+"'";
                

                SQLiteCommand command = new SQLiteCommand( connection);
                command.CommandText = sql;
               // command.Parameters.AddWithValue("@enable", "true");
               // command.Parameters.AddWithValue("@chartid", chartID);
                MessageBox.Show("Sql= " + sql);
                command.ExecuteNonQuery();
                //SQLiteDataReader reader = command.ExecuteReader();
                //while (reader.Read())
                //{
                //    //This is the reading part of the data...
                //    chartDetailListForDissabledValue.Add(new chartDetailDT
                //    {
                //        count = int.Parse(reader["count"].ToString()),
                //        chartID = reader["chartID"].ToString(),
                //        chartName = reader["chartName"].ToString(),
                //        chart_respective_nodeID = reader["chart_respective_nodeID"].ToString(),
                //        chart_respective_lineID = reader["chart_respective_lineID"].ToString(),
                //        enableChartStatus = reader["enableChartStatus"].ToString() //either true or false

                //    });
                //}
            }

        }

    }
}
