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

namespace WFA_psychometric_chart
{
    public partial class Form4_insert_data : Form
    {
        public Form4_insert_data()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (tb_country.Text == "")
            {
                MessageBox.Show("Enter country name!");

            }
            else if (tb_city.Text == "")
            {
                MessageBox.Show("Enter city name!");
            }
            else if (tb_ZIP.Text == "")
            {
                MessageBox.Show("enter ZIP number");

            }
            else
            {
                string country = tb_country.Text;
                string city = tb_city.Text;
                string state = tb_state.Text;
                string street = tb_street.Text;
                string zip = tb_ZIP.Text;
                //now lets update the values in database...

                try
                {
                     //lets pull the vales offline values stored in db...
                    string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";                
                    string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                    using (OleDbConnection connection = new OleDbConnection(connString))
                    {
                        connection.Open();
                        //string sql_string = "update tbl_building_location set   country=@country_value,state=@state_value,city=@city_value,street=@street_value,ZIP=@zip_value where ID = 1;";
                        string sql_query = "INSERT INTO tbl_building_location(country,state,city,street,zip) VALUES(@country_value,@state_value,@city_value,@street_value,@zip_value)";
                        OleDbCommand command = new OleDbCommand(sql_query, connection);
                        command.CommandType = CommandType.Text;
                        command.Parameters.AddWithValue("@country_value", country);
                        command.Parameters.AddWithValue("@state_value", state);
                        command.Parameters.AddWithValue("@city_value", city);
                        command.Parameters.AddWithValue("@street_value", street);
                        command.Parameters.AddWithValue("@zip_value", zip);
                        
                        command.ExecuteNonQuery();
                        //MessageBox.Show("sql string = " + sql_string);
                        MessageBox.Show("value inserted successfully!");

                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }





            }//close of else..

           


        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }//close of private method

  

        

     
    }
}
