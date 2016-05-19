using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Data.SQLite;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WFA_psychometric_chart
{
    public partial class form_heat_map : Form
    {
        private  Form1_main form1;
        public form_heat_map(Form1_main formx)
        {
            this.form1 = formx;
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.form_heat_map_Disposed );
        }
        int map_loaded = 0;
        int index_selected;//this is used for location services..
        private void form_heat_map_Load(object sender, EventArgs e)
        {

            //chart_plot();
            form1.plot_new_graph();


            //lets set the data time picker default values...
            dtp_From.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_From.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_To.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_From.Value = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.Value = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);

            PullLocationInformation();//this is for loading location information
        }

        public class DataTypeTempBuildingValue
        {
            public int ID { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
        }

        //ArrayList temp_building_values = new ArrayList();
        List<DataTypeTempBuildingValue> temp_building_values = new List<DataTypeTempBuildingValue>();


        private void PullLocationInformation()
        {
            try
            {
                //cb1_select_data.Items.Clear();
                ArrayList stored_location = new ArrayList();
                temp_building_values.Clear();//we need to clear the values for new items
                                             //while loading it should populate the field...
                                             //lets pull the vales offline values stored in db...
                                             //string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

                //string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";


                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";


                // MessageBox.Show("connection string = " + connString);


                SQLiteConnection connection = new SQLiteConnection(connString);
                connection.Open();
                SQLiteDataReader reader = null;
                SQLiteCommand comm = new SQLiteCommand("SELECT * from tbl_building_location where selection = 1", connection);
                //command.Parameters.AddWithValue("@1", userName)
                reader = comm.ExecuteReader();
                while (reader.Read())
                {

                    //string selecte_location = reader["id"].ToString()+","+reader["country"].ToString() + "," + reader["state"].ToString() + "," + reader["city"].ToString();
                    //stored_location.Add(selecte_location);

                    temp_building_values.Add(new DataTypeTempBuildingValue
                    {
                        ID = int.Parse(reader["id"].ToString()),
                        country = reader["country"].ToString(),
                        state = reader["state"].ToString(),
                        city = reader["city"].ToString()
                    });

                }
                ////string s = "";
                //for (int i = 0; i < temp_building_values.Count; i++)
                //{

                //    string tempValue = temp_building_values[i].ID + "," + temp_building_values[i].country + "," + temp_building_values[i].state + "," + temp_building_values[i].city;
                //  //  cb1_select_data.Items.Add(tempValue);
                //    //s += stored_location[i] + " , \n";
                //}

                index_selected = temp_building_values[0].ID;
                // MessageBox.Show("stored place = " + s);
                comm.Dispose();
                reader.Dispose();
                connection.Close();



            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
           }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();


        //chart definatioin..
        public void chart_plot()
        {
            /*
         steps:
         * 1.set x and y axis in graph
         * 2.plot red lines
         * 3.plot green lines
         * 4.plot wet bult curve red line.
         * 5.
         * 
         */

          
        }

        double min_value = 0;
        double max_value = 0;

        //lets create two arraylist to add those and store it in the arraylist
        ArrayList temperature_value = new ArrayList();
        ArrayList pg_value_from_txtfile = new ArrayList();

        Series series1 = null;
        int load_map_checker = 0;//checks weather to load a map or not
        int index_series = 0;//this index is used by  plot_on_graph_values method

        private void button1_Click(object sender, EventArgs e)
        {
            //initially resetting values to empty
            
            //resetting ends...
            DateTime fromDate = dtp_From.Value;
            DateTime toDate = dtp_To.Value;

            
            //--Calling part here.....
            form1.heat_map_button_click(index_selected, fromDate, toDate);





        }//closing of the buttton...

        
        int index = 0;
        

        //THi will be done later 

        private void printMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form1.printHeatMap();
        }

        private void saveAsImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form1.saveAsImageHeatMap();

        }

        
      public void form_heat_map_Disposed ( object sender, System.EventArgs e )
      {
      }

    
    }
}
