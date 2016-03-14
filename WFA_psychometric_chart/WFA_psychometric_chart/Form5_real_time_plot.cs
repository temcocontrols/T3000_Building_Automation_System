using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.Data.OleDb;
using System.Globalization;

namespace WFA_psychometric_chart
{
    public partial class Form5_real_time_plot : Form
    {
        private Form1 form1;
        public Form5_real_time_plot(Form1 form1)
        {
            this.form1 = form1;//defined in top..
            //InitializeComponent();
            InitializeComponent();
        }

        public void plot()
        {

            //we need the instance of first form to be passed into this form..

            //steps
            /*
             1.load the data from the test file 
             * 2.parse the data
             * 3.then pass the data to the function of form1.
             * 4.see plot.
             */

            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\oneday_data_test.txt";
            string path1 = file;
            string line1;


            //lets store the data into the arraylist..
            ArrayList al_temp = new ArrayList();
            ArrayList al_hum = new ArrayList();
            using (System.IO.StreamReader st = new System.IO.StreamReader(path1))
            {
                //int i = 0;
                //string s = " ";
                while ((line1 = st.ReadLine()) != null)
                {
                    //line.Skip(1);


                    string[] value = line1.Split(',');
                    try
                    {
                        double temp1 = Double.Parse(value[1]);
                        double temp2 = Double.Parse(value[3].Trim());
                        //now lets add to temperature and pg array..
                        al_temp.Add(temp1);
                        al_hum.Add(temp2);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }


                }//close of while

            }//close of using

            //now lets test the value..
            try
            {
                //lets plot the values..           
                string s = "";
                for (int i = 0; i < al_temp.Count; i++)
                {
                    s += "temp = " + al_temp[i].ToString() + ", hum = " + al_hum[i].ToString() + "\n";
                    form1.plot_by_DBT_HR(double.Parse(al_temp[i].ToString()), double.Parse(al_hum[i].ToString()) / 100);
                }


                MessageBox.Show("data = " + s);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            //now lets plot the data based on the data store in the database...
            



        }//clos of plot fxn

        string station_name = null;

        List<string> day_list = new List<string>();

        public void date_of_day_in_month(int year_val ,int month_val, DayOfWeek day_name)
        {


      
     /*   for(int i=0;i<(int)DateTime.DaysInMonth(year_val, month_val); i++)
        {
            DateTime dt = new DateTime(year_val, month_val, i);
            if (dt.DayOfWeek == day_name)
                day_list.Add(dt);
        }

            */

            var month = month_val;
            var year = year_val;

            DateTime start = new DateTime(year, month, 1);
           DateTime end = start.AddMonths(1);

            while (start < end)
            {
                if (start.DayOfWeek == day_name)
                {
                    day_list.Add(start.ToShortDateString());//this will be added in the day_list..
                }
                start = start.AddDays(1);
            }



        }


        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum{get;set;}
            public DateTime date{get;set;}
        }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>(); 

        //this function is used to pull the data form the database using the given dates as delimitars..
       


        public void plot_historical_data()
        {
            //steps
            /*
             1.store the date in the array list and then futher process..
             */
            ArrayList month_al = new ArrayList();
            ArrayList day_al = new ArrayList();
            ArrayList hour_al = new ArrayList();

            //lets check if the check boxes are empty or not...
            if (cb1.Checked == true || cb2.Checked == true || cb3.Checked == true || cb4.Checked == true || cb5.Checked == true || cb6.Checked == true || cb7.Checked == true || cb8.Checked == true || cb9.Checked == true || cb10.Checked == true || cb11.Checked == true || cb12.Checked == true)
            {

                //lets insert the values in the array list..
                if (cb1.Checked == true)
                {
                    //insert  it in arraylist..
                    month_al.Add(01);//january.
                }
                 if (cb2.Checked == true)
                {
                    month_al.Add(02);//february.
                }
                if (cb3.Checked == true)
                {
                    month_al.Add(03);//march.
                }
                 if (cb4.Checked == true)
                {
                    month_al.Add(04);//april.
                }
                 if (cb5.Checked == true)
                {
                    month_al.Add(05);//may.
                }
                if (cb6.Checked == true)
                {
                    month_al.Add(06);//june.
                }
                if (cb7.Checked == true)
                {
                    month_al.Add(07);//july.
                }
                if (cb8.Checked == true)
                {
                    month_al.Add(08);//august.
                }
                 if (cb9.Checked == true)
                {
                    month_al.Add(09);//september.
                }
                if (cb10.Checked == true)
                {
                    month_al.Add(10);//october.
                }
                if (cb11.Checked == true)
                {
                    month_al.Add(11);//november.
                }
                if (cb12.Checked == true)
                {
                    month_al.Add(12);//december.
                }

                //MessageBox.Show("month count = " + month_al.Count);


                if (cb13.Checked == true || cb14.Checked == true || cb15.Checked == true || cb16.Checked == true || cb17.Checked == true || cb18.Checked == true || cb19.Checked == true)
                {
                    //now lets count the days..
                    if (cb13.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Sunday);

                    }
                    if (cb14.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Monday);
                    }
                     if (cb15.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Tuesday);
                    }
                     if (cb16.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Wednesday);
                    }
                    if (cb17.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Thursday);
                    }
                     if (cb18.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Friday);
                    }
                     if (cb19.Checked == true)
                    {
                        day_al.Add(DayOfWeek.Saturday);
                    }
                  //   MessageBox.Show("days count = " + day_al.Count);

                    //lets check for the hours..
                    if (cb20.Checked == true || cb21.Checked == true || cb22.Checked == true || cb23.Checked == true || cb24.Checked == true || cb25.Checked == true || cb26.Checked == true || cb27.Checked == true || cb28.Checked == true || cb29.Checked == true || cb30.Checked == true || cb31.Checked == true || cb32.Checked == true || cb33.Checked == true || cb34.Checked == true || cb35.Checked == true || cb36.Checked == true || cb37.Checked == true || cb38.Checked == true || cb39.Checked == true || cb40.Checked == true || cb41.Checked == true || cb42.Checked == true || cb43.Checked == true)
                    {
                        //then lets store the values in the arraylist..
                        if (cb20.Checked == true)
                        {
                            hour_al.Add(01);//1 hour
                        }
                         if (cb21.Checked == true)
                        {
                            hour_al.Add(02);//2 hour
                        }
                         if (cb22.Checked == true)
                        {
                            hour_al.Add(03);//3 hour
                        }
                         if (cb23.Checked == true)
                        {
                            hour_al.Add(04);//4 hour
                        }
                         if (cb24.Checked == true)
                        {
                            hour_al.Add(05);//5 hour
                        }
                         if (cb25.Checked == true)
                        {
                            hour_al.Add(06);//6 hour
                        }
                         if (cb26.Checked == true)
                        {
                            hour_al.Add(07);//7 hour
                        }
                        if (cb27.Checked == true)
                        {
                            hour_al.Add(08);//8 hour
                        }
                        if (cb28.Checked == true)
                        {
                            hour_al.Add(09);//9 hour
                        }
                        if (cb29.Checked == true)
                        {
                            hour_al.Add(10);//10 hour
                        }
                         if (cb30.Checked == true)
                        {
                            hour_al.Add(11);//11 hour
                        }
                         if (cb31.Checked == true)
                        {
                            hour_al.Add(12);//12 hour
                        }
                         if (cb32.Checked == true)
                        {
                            hour_al.Add(13);//13 hour
                        }
                        if (cb33.Checked == true)
                        {
                            hour_al.Add(14);//14 hour
                        }
                         if (cb34.Checked == true)
                        {
                            hour_al.Add(15);//15 hour
                        }
                        if (cb35.Checked == true)
                        {
                            hour_al.Add(16);//16 hour
                        }
                        if (cb36.Checked == true)
                        {
                            hour_al.Add(17);//17 hour
                        }
                        if (cb37.Checked == true)
                        {
                            hour_al.Add(18);//18 hour
                        }
                        if (cb38.Checked == true)
                        {
                            hour_al.Add(19);//19 hour
                        }
                        if (cb39.Checked == true)
                        {
                            hour_al.Add(20);//20 hour
                        }
                         if (cb40.Checked == true)
                        {
                            hour_al.Add(21);//21 hour
                        }
                        if (cb41.Checked == true)
                        {
                            hour_al.Add(22);//22 hour
                        }
                         if (cb42.Checked == true)
                        {
                            hour_al.Add(23);//23 hour
                        }
                         if (cb43.Checked == true)
                        {
                            hour_al.Add(24);//24 hour
                        }

                    //     MessageBox.Show("hour count = " + hour_al.Count);

                        //now here we are going to do our calculation...
                        //ArrayList date_of_days = new ArrayList();
                        string test = null;
                        for (int m = 0; m < month_al.Count; m++)
                        {
                            //month loop..
                            int month = int.Parse(month_al[m].ToString());

                            for (int d = 0; d < day_al.Count; d++)
                            {
                                //this is the day section...
                                DayOfWeek day_name = (DayOfWeek)day_al[d];
                                //now  we need to calculate the time in that day..
                                date_of_day_in_month(2016, month, day_name);//this function will fullfill the day_list value.


                                
                            }//close of for days
                            
                        }//close of for month...

                        //lets open the database connection here and do the following task later.

                       //lets test the date 

                        for (int a = 0; a < day_list.Count; a++)
                        {
                            test += day_list[a] + " , \t ";

                        }
                        MessageBox.Show("test = \n"+test);

                        string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                        string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                               

                        //this is different part

                        for (int i1 = 0; i1 < day_list.Count; i1++)
                        {
                            //the day_list store the data in the date time format..like 13/03/2016
                            //lets check if thsi works or not..
                            //test += day_list[i] + " , \t ";
                            //here we need to do the date calculation 
                            for (int h = 0; h < hour_al.Count; h++)
                            {

                                using (OleDbConnection connection1 = new OleDbConnection(connString1))
                                {
                                    connection1.Open();
                        


                                //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                                //lets pass this string to a query which does the pulling part.
                                OleDbDataReader reader1 = null;
                                OleDbCommand command1 = new OleDbCommand("Select * from tbl_historical_data WHERE date_current = @date AND hour_current = @hour ", connection1);
                                command1.Parameters.AddWithValue("@date",day_list[i1]);
                                command1.Parameters.AddWithValue("@hour", int.Parse(hour_al[h].ToString()));
                                //command1.Parameters.AddWithValue("@station_name", station_name);
                                reader1 = command1.ExecuteReader();
                                while (reader1.Read())
                                {
                                    //station_name = reader["station_name"].ToString();
                                    hist_temp_hum_list.Add(
                                        new data_type_hum_temp
                                        {
                                            temp= double.Parse(reader1["temperature"].ToString()),
                                            hum = double.Parse(reader1["humidity"].ToString()),
                                            date = (DateTime)reader1["date_current"]
                                        });
                                }//close of while loop       
                               // connection1.Close();
                                }//close of database using statement 
                            }//close of for hour
                        }//close of for day list
                         
                       // MessageBox.Show("testvalu= \n" + test);

                        //now lets try to plot the values in the first form...
                    string test1 = null;
                        for (int x = 0; x < hist_temp_hum_list.Count; x++)
                    {
                        //this plots the value
                        form1.plot_by_DBT_HR(hist_temp_hum_list[x].temp,hist_temp_hum_list[x].hum/100);
                        test1 += "temp= " + hist_temp_hum_list[x].temp + "hum = " + hist_temp_hum_list[x].hum + "date =" + hist_temp_hum_list[x].date + "\n";
                    }
                        MessageBox.Show("the data outputed final test1 " + test1);
                        //after ploting lets reset the values..
                    hist_temp_hum_list.Clear();
                    day_list.Clear();
                    }
                    else
                    {
                        MessageBox.Show("Please chose at least 1 hour");
                    }



                }//closing of days if
                else
                {
                    MessageBox.Show("Please chose at least 1 day");
                }




            }//close of if..
            else
            {
                MessageBox.Show("Please select at least 1 month");
            }
        }

        
        private void btn_plot_Click(object sender, EventArgs e)
        {
          //  plot();

            //this function does the actual calculation.
            //lets pull the station name form the values..

            if (cb1_select_data.SelectedIndex > -1)
            {
                //then perform the operation 
                //MessageBox.Show("selected index" + cb1_select_data.SelectedIndex);
                int id = cb1_select_data.SelectedIndex + 1;

                //lets calculate the station name form the given data...

                try {

                    string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                    using (OleDbConnection connection = new OleDbConnection(connString))
                    {
                        connection.Open();
                        OleDbDataReader reader = null;
                        OleDbCommand command = new OleDbCommand("SELECT * from tbl_weather_related_values where ID=@id", connection);
                        command.Parameters.AddWithValue("@id", id);
                        reader = command.ExecuteReader();
                        while (reader.Read())
                        {
                            station_name = reader["station_name"].ToString();
                        }
                    }


                    
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

                MessageBox.Show("station name = " + station_name);
                plot_historical_data();


            }//close of if
            else
            {
                MessageBox.Show("Please select a location first");
            }

            
            

        }//close of private void...

        private void cb_month_check_all_CheckedChanged(object sender, EventArgs e)
        {

            //if the value is checked the we check all the items of months else none
            if (cb_month_check_all.Checked == true)
            {
                //we gonna check the checkboxes..
                cb1.Checked = true;
                cb2.Checked = true;
                cb3.Checked = true;
                cb4.Checked = true;
                cb5.Checked = true;
                cb6.Checked = true;
                cb7.Checked = true;
                cb8.Checked = true;
                cb9.Checked = true;
                cb10.Checked = true;
                cb11.Checked = true;
                cb12.Checked = true;
                check_enable_for_days();
            }
            else
            {
                //we gonna uncheck..
                cb1.Checked = false;
                cb2.Checked = false;
                cb3.Checked = false;
                cb4.Checked = false;
                cb5.Checked = false;
                cb6.Checked = false;
                cb7.Checked = false;
                cb8.Checked = false;
                cb9.Checked = false;
                cb10.Checked = false;
                cb11.Checked = false;
                cb12.Checked = false;
                check_enable_for_days();

            }



        }

        private void cb_hour_check_all_CheckedChanged(object sender, EventArgs e)
        {

            //if the value is checked the we check all the items of months else none
            if (cb_hour_check_all.Checked == true)
            {
                //we gonna check the checkboxes..
                cb20.Checked = true;
                cb21.Checked = true;
                cb22.Checked = true;
                cb23.Checked = true;
                cb24.Checked = true;
                cb25.Checked = true;
                cb26.Checked = true;
                cb27.Checked = true;
                cb28.Checked = true;
                cb29.Checked = true;
                cb30.Checked = true;
                cb31.Checked = true;
                cb32.Checked = true;
                cb33.Checked = true;
                cb34.Checked = true;
                cb35.Checked = true;
                cb36.Checked = true;
                cb37.Checked = true;
                cb38.Checked = true;
                cb39.Checked = true;
                cb40.Checked = true;
                cb41.Checked = true; 
                cb42.Checked = true;
                cb43.Checked = true;

            }
            else
            {
                //we gonna uncheck..
                cb20.Checked = false;
                cb21.Checked = false;
                cb22.Checked = false;
                cb23.Checked = false;
                cb24.Checked = false;
                cb25.Checked = false;
                cb26.Checked = false;
                cb27.Checked = false;
                cb28.Checked = false;
                cb29.Checked = false;
                cb30.Checked = false;
                cb31.Checked = false;
                cb32.Checked = false;
                cb33.Checked = false;
                cb34.Checked = false;
                cb35.Checked = false;
                cb36.Checked = false;
                cb37.Checked = false;
                cb38.Checked = false;
                cb39.Checked = false;
                cb40.Checked = false;
                cb41.Checked = false;
                cb42.Checked = false;
                cb43.Checked = false;


            }



        }

        public class new_data
        {
            public double id { get; set; }
            public string country { get; set; }
            public string state { get; set; }
            public string city { get; set; }
            //public string station_name { get; set; }
        }

        List<new_data> new_list_data =new List<new_data>();
        public void fill_combobox()
        {
            new_list_data.Clear();
            cb1_select_data.Items.Clear();
           // ArrayList stored_location = new ArrayList();
            //while loading it should populate the field...
            //lets pull the vales offline values stored in db...
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
            string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
            // MessageBox.Show("connection string = " + connString);


            OleDbConnection connection = new OleDbConnection(connString);
            connection.Open();
            OleDbDataReader reader = null;
            OleDbCommand comm = new OleDbCommand("SELECT * from tbl_building_location", connection);
            //command.Parameters.AddWithValue("@1", userName)
            reader = comm.ExecuteReader();
            while (reader.Read())
            {

                //string selecte_location = reader["id"].ToString() + "," + reader["country"].ToString() + "," + reader["state"].ToString() + "," + reader["city"].ToString();
                new_list_data.Add(new new_data
                {
                    id = double.Parse(reader["id"].ToString()),
                    country = reader["country"].ToString(),
                    state = reader["state"].ToString(),
                    city = reader["city"].ToString(),
                    //station_name = reader["station_name"].ToString()
                });

                //stored_location.Add(selecte_location);
            }
            //string s = "";
            for (int i = 0; i < new_list_data.Count; i++)
            {
                string s1 = new_list_data[i].id + ". " + new_list_data[i].country + "," + new_list_data[i].state + "," + new_list_data[i].city;
                cb1_select_data.Items.Add(s1);
                
            }
            // MessageBox.Show("stored place = " + s);
            comm.Dispose();
            reader.Dispose();
            connection.Close();





        }


        private void Form5_real_time_plot_Load(object sender, EventArgs e)
        {
            fill_combobox();
        }

        public void check_enable_for_days(){

            if (cb1.Checked == true || cb2.Checked == true || cb3.Checked == true || cb4.Checked == true || cb5.Checked == true || cb6.Checked == true || cb7.Checked == true || cb8.Checked == true || cb9.Checked == true || cb10.Checked == true || cb11.Checked == true || cb12.Checked == true)
            {
                //it is checked enable the buttons..                
                groupBox3.Enabled = true;


            }
            else
            {
                //disable them ...
                groupBox3.Enabled = false;


                cb13.Checked = false;
                cb14.Checked = false;
                cb15.Checked = false;
                cb16.Checked = false;
                cb17.Checked = false;
                cb18.Checked = false;
                cb19.Checked = false;
                cb_check_all_days.Checked = false;

            }


        }
        private void check1_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb2_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb3_check_event(object sender, EventArgs e)
        {
            check_enable_for_days(); 
        }

        private void cb4_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb5_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb6_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb7_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb8_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb9_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb10_check_event(object sender, EventArgs e)
        {
            check_enable_for_days(); 
        }

        private void cb11_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        private void cb12_check_event(object sender, EventArgs e)
        {
            check_enable_for_days();
        }

        //lets make functions for the check events 
        public void check_enable_for_hour()
        {
            if (cb13.Checked == true || cb14.Checked == true || cb15.Checked == true || cb16.Checked == true || cb17.Checked == true || cb18.Checked == true || cb19.Checked == true)
            {
                //we gonna check the checkboxes..
                groupBox4.Enabled = true;


            }//close of if..
            else
            {
                
                groupBox4.Enabled = false;

                cb20.Checked = false;
                cb21.Checked = false;
                cb22.Checked = false;
                cb23.Checked = false;
                cb24.Checked = false;
                cb25.Checked = false;
                cb26.Checked = false;
                cb27.Checked = false;
                cb28.Checked = false;
                cb29.Checked = false;
                cb30.Checked = false;
                cb31.Checked = false;
                cb32.Checked = false;
                cb33.Checked = false;
                cb34.Checked = false;
                cb35.Checked = false;
                cb36.Checked = false;
                cb37.Checked = false;
                cb38.Checked = false;
                cb39.Checked = false;
                cb40.Checked = false;
                cb41.Checked = false;
                cb42.Checked = false;
                cb43.Checked = false;
                cb20.Checked = false;
                cb_hour_check_all.Checked = false;

            }//close of else..
        }

        private void cb13_check_event(object sender, EventArgs e)
        {
                //then do this ...
                check_enable_for_hour();
            
           
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

            //if checked then enable all the check..
            if (cb_check_all_days.Checked == true)
            {
                //check all check boxes 
                cb13.Checked = true;
                cb14.Checked = true;
                cb15.Checked = true;
                cb16.Checked = true;
                cb17.Checked = true;
                cb18.Checked = true;
                cb19.Checked = true;
                check_enable_for_hour();

            }
            else
            {
                //uncheck all
                cb13.Checked = false;
                cb14.Checked = false;
                cb15.Checked = false;
                cb16.Checked = false;
                cb17.Checked = false;
                cb18.Checked = false;
                cb19.Checked = false;
                check_enable_for_hour();
            }


        }

        private void cb14_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

        private void cb15_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

        private void cb16_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

        private void cb17_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

        private void cb18_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

        private void cb19_CheckedChanged(object sender, EventArgs e)
        {
            check_enable_for_hour();
        }

       


        
    }
}
