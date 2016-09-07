using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;
using System.IO;
using System.Reflection;
using System.Data.SQLite;

namespace WFA_psychometric_chart
{
    public partial class form_app_timer : Form
    {
        private Form1_main form1;
        public form_app_timer(Form1_main form1)
        {
            this.form1 = form1;
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.form_app_timer_Disposed );
        }

        int first_enable = 0;
        int second_enable = 0;
        ArrayList new_checked_item_index = new ArrayList();

        int index_selected;//this index is used for pulling location information 

        private void form_app_timer_Load(object sender, EventArgs e)
        {
            //btn_from_month_down.Text = char.ConvertFromUtf32(8595);
            
            //dtp1.Value = new DateTime(DateTime.Now.Year, 1, 1);
            //dtp2.Value = new DateTime(DateTime.Now.Year,DateTime.Now.Month,DateTime.Now.Day);
            //dateTimePicker.MinDate = DateTime.Now;
           // dateTimePicker.MaxDate = DateTime.Now.AddDays(15);
            dtp1.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp2.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp1.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp2.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);

            dtp1.Value = new DateTime(DateTime.Now.Year, 1, 1);
            dtp2.Value = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);


            //this method selects location pulls the location address 
            PullLocationInformation();
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

        private void button1_Click(object sender, EventArgs e)
        {
            //when this one is clicked 
            //mc1.Visible = true;

            
        }

        private void monthCalendar1_DateChanged(object sender, DateRangeEventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {

           // mc2.Visible = true;
        }

        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
            public string  date { get; set; }
            public int hour { get; set; }
        }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();
        List<data_type_hum_temp> temp_hist_temp_hum_list = new List<data_type_hum_temp>(); 

        //this is the function that plotes the values form the graph..
        public void plot_on_first_graph()
        {
             


            if (first_enable == 1 && second_enable == 1)
            {

                if (checkedListBox1.CheckedItems.Count > 0)
                {

                //string t = null;

                
                //lets check if the hour was changed or not...
                foreach(object item in checkedListBox1.CheckedItems)//this is done like this because the items checked has discarded in 0.                
                {
                    int index = checkedListBox1.Items.IndexOf(item);
                    if (index != 0) { 
                    new_checked_item_index.Add(index);
                    //t += index + ",\t";
                    }
                   


                }


                    //then only do the changes else we dont have to do it.
                    //MessageBox.Show("it works\n "+t);
                   
                //lets do the plotting part here..

                  if (dtp2.Value > dtp1.Value) {

                  //-----------resetting to newly plotted graph---------//
                  
                   form1.plot_new_graph();//this is doen because we need to plot on new graph every time the values are pulled.
                  //---------------resetting ends here-----------------//

                //  string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //  string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                //sql connection string is this..
                //      string connString1 = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                        //--changing all the database to the sqlite database...
                        string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                        string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                        string connString1 = @"Data Source=" + databaseFile + ";Version=3;";

                        using (SQLiteConnection connection1 = new SQLiteConnection(connString1))
                         {
                      connection1.Open();

                      //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                      //lets pass this string to a query which does the pulling part.
                      SQLiteDataReader reader1 = null;
                            SQLiteCommand command1 = new SQLiteCommand("Select * from tbl_historical_data WHERE date_current BETWEEN @date_first AND @date_second AND ID=@id_value", connection1);
                            command1.Parameters.AddWithValue("@date_first", dtp1.Value);
                            command1.Parameters.AddWithValue("@date_second", dtp2.Value);
                            command1.Parameters.AddWithValue("@id_value", index_selected);
                            //command1.Parameters.AddWithValue("@station_name", station_name);
                            reader1 = command1.ExecuteReader();
                      while (reader1.Read())
                      {
                          //station_name = reader["station_name"].ToString();
                          hist_temp_hum_list.Add(
                              new data_type_hum_temp
                              {
                                  temp = double.Parse(reader1["temperature"].ToString()),
                                  hum = double.Parse(reader1["humidity"].ToString()),
                                  date = reader1["date_current"].ToString(),
                                  hour = int.Parse(reader1["hour_current"].ToString())
                              });
                      }//close of while loop       
                      // connection1.Close();
                  }//close of database using statement 

                      //lets display the data 
                  //string test = null;
                  //for (int i = 0; i < hist_temp_hum_list.Count; i++)
                  //{
                  //    test += hist_temp_hum_list[i].temp+" , hum=  "+ hist_temp_hum_list[i].hum +",date="+hist_temp_hum_list[i].date+",hour="+hist_temp_hum_list[i].hour+"\n";
                    
                  //}
                  //MessageBox.Show("value = \n"+test);
                //since we have the data now we can start plotting it in the form..
                      for(int i=0;i < new_checked_item_index.Count ; i++)
                      {
                          for (int y = 0; y < hist_temp_hum_list.Count;y++ )
                          {
                             //here we compare the desired outcome...
                              if (new_checked_item_index[i].ToString() ==hist_temp_hum_list[y].hour.ToString())
                              {
                                  //then push it into the new list..
                                  temp_hist_temp_hum_list.Add(hist_temp_hum_list[y]);
                                  hist_temp_hum_list.RemoveAt(y);
                              }
                          }
                      }//close of for..

                      //--Testing...
                      //string test = null;
                      //for (int i = 0; i < temp_hist_temp_hum_list.Count; i++)
                      //{
                      //    test += "Temp = "+temp_hist_temp_hum_list[i].temp + " , hum=  " + temp_hist_temp_hum_list[i].hum + ",date=" + temp_hist_temp_hum_list[i].date + ",hour=" + temp_hist_temp_hum_list[i].hour + "\n";

                      //}
                     // MessageBox.Show(WFA_psychometric_chart.Properties.Resources.after_filtering_the_values_of_ + test);
                    //now lets plot it in the graph
                      for (int x = 0; x < temp_hist_temp_hum_list.Count; x++)
                      {
                          //this plots the value
                          form1.plot_by_DBT_HR((double)temp_hist_temp_hum_list[x].temp, (double)(temp_hist_temp_hum_list[x].hum/100) );
                        
                      }
                      //now lets reset all the values..
                      temp_hist_temp_hum_list.Clear();
                      hist_temp_hum_list.Clear();
                      new_checked_item_index.Clear();

                        form1.lb_title_display.Text = "Historical Plot";

                        //if plotted then we need to dissable the insert node fxn
                        form1.insertNodeToolStripMenuItem.Enabled = false;



                  }//close of comparision if
                  else
                  {
                      MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_dates_Dat);
                  }
                
                  

                }//close of if checkedItem>0
                else
                {
                    MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_Select_one_or_more_hour);
                }

            }//close of if
            
        }//close of the main function .

        private void dtp1_event_onValue_Change(object sender, EventArgs e)
        {
            //lets enable the event and see what happens ..
            first_enable = 1;//event enabler , this is done to see if the value is changed or not if both the value of To and From
                             //is changed then we make the calculation..

            plot_on_first_graph();


        }

        private void dtp2_event_onValue_Change(object sender, EventArgs e)
        {
            //this value for second one ...
            second_enable = 1;//enable the event 
           // dtp1.MaxDate = dtp2.Value;//this is done so that the maximun value changed of the second is max for 1 compulasary.
            plot_on_first_graph();
        }
       // int item_selected = 0;
        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (checkedListBox1.SelectedItem == checkedListBox1.Items[0])
            {
                //if(checkedListBox1.SelectedIndex==0)
                if (checkedListBox1.GetItemChecked(0) == true)
                {

                    //then checked the items..
                    //MessageBox.Show("item checked is" + checkedListBox1.SelectedIndex);
                    for (int i = 1; i <= 24; i++)
                    {
                        checkedListBox1.SetItemChecked(i, true);

                    }


                }//close of ir
                else if (checkedListBox1.GetItemChecked(0) == false)
                {
                    for (int i = 1; i <= 24; i++)
                    {
                        checkedListBox1.SetItemChecked(i, false);

                    }

                }




            }
        }//close of flb.selecteditem

        private void cb1_select_data_SelectedIndexChanged(object sender, EventArgs e)
        {
            //on change index it will select the index value or better known as selected index.
            try
            {
                //index_selected = cb1_select_data.SelectedIndex + 1; //is used to identify the location and data associated with it.
               // int cb_index_selected = cb1_select_data.SelectedIndex;
                //index_selected = temp_building_values[cb_index_selected].ID;

                gb_select_time_and_date.Enabled = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

      public void form_app_timer_Disposed ( object sender, System.EventArgs e )
      {
      }
    }
}
