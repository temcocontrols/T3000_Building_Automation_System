using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Data.SqlClient;
using System.Reflection;
using System.Data.SQLite;

namespace WFA_psychometric_chart
{
    public partial class Form4_insert_data : Form
    {
        private Form3 form3;
        public Form4_insert_data(Form3 form3)
        {
            this.form3 = form3;
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.Form4_insert_data_Disposed );
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(tb_building_name.Text == "")
            {
                MessageBox.Show("Please enter building name!");       //MLHIDE
            }

            else if (tb_country.Text == "")
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Enter_country_name);

            }
            else if (tb_city.Text == "")
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Enter_city_name);
            }
            else if (tb_ZIP.Text == "")
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.enter_ZIP_number);

            }
            else
            {
                string buildingName = tb_building_name.Text;
                string country = tb_country.Text;
                string city = tb_city.Text;
                string state = tb_state.Text;
                string street = tb_street.Text;
                string zip = tb_ZIP.Text;
                string lng = tb_longitude.Text;
                string lat = tb_latitude.Text;
                string elevation = tb_elevation.Text;
                //now lets update the values in database...

                try
                {
                     //lets pull the vales offline values stored in db...
                   // string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";                
                    //string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";
                 //   string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                    //--changing all the database to the sqlite database...
                    string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                    string connString = @"Data Source=" + databaseFile + ";Version=3;";



                    using (SQLiteConnection connection = new SQLiteConnection(connString))
                    {

                        connection.Open();
                        //string sql_string = "update tbl_building_location set   country=@country_value,state=@state_value,city=@city_value,street=@street_value,ZIP=@zip_value where ID = 1;";
                        string sql_query = "INSERT INTO tbl_building_location(country,state,city,street,zip,longitude,latitude,elevation,BuildingName) VALUES(@country_value,@state_value,@city_value,@street_value,@zip_value,@long_value,@lat_value,@elevation_value,@building_name)";
                         SQLiteCommand command = new SQLiteCommand(sql_query, connection);
                        command.CommandType = CommandType.Text;
                        command.Parameters.AddWithValue("@country_value", country);
                        command.Parameters.AddWithValue("@state_value", state);
                        command.Parameters.AddWithValue("@city_value", city);
                        command.Parameters.AddWithValue("@street_value", street);
                        command.Parameters.AddWithValue("@zip_value", zip);
                        command.Parameters.AddWithValue("@long_value", lng);
                        command.Parameters.AddWithValue("@lat_value", lat);
                        command.Parameters.AddWithValue("@elevation_value", elevation);
                        command.Parameters.AddWithValue("@building_name", buildingName);
                        command.ExecuteNonQuery();
                        //MessageBox.Show("sql string = " + sql_string);
                        MessageBox.Show(WFA_psychometric_chart.Properties.Resources.value_inserted_successfully);
                        btn_insert_data.Enabled = false;
                        tb_country.Text = "";
                        tb_city.Text = "";
                        tb_state.Text = "";
                        tb_street.Text = "";
                        tb_ZIP.Text = "";
                        tb_latitude.Text = "";
                        tb_longitude.Text = "";
                        tb_elevation.Text = "";

                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }





            }//close of else..

           //lets refresh the list....
            form3.fill_combobox();


        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public double getElevation(double lat, double lng)
        {
            //https://developers.google.com/maps/documentation/elevation/intro
            var request = (HttpWebRequest)WebRequest.Create(string.Format("https://maps.googleapis.com/maps/api/elevation/json?locations={0},{1}&key=AIzaSyB27paQGkpwaVGAd7wp0N6rcFkq5JEKeiQ", lat, lng));
            var response = (HttpWebResponse)request.GetResponse();
            var sr = new StreamReader(response.GetResponseStream() ?? new MemoryStream()).ReadToEnd();

            var json = JObject.Parse(sr);
            return (double)json.SelectToken("results[0].elevation");
        }

        private void pull_data_online()
        {
            //this function pulls the data from online devices...


            /*
            1.country,state,city,street,latitude,longitude,elev,zip
            */
            string country = tb_country.Text;
            string state = tb_state.Text;
            string city = tb_city.Text;
            string street = tb_street.Text;
            string zip =tb_ZIP.Text;
            int value;
            if (int.TryParse(zip,out value))
            {

                if (country != "" && city != "")
                {
                    string join_string = "";
                    if (state != "" && street != "")
                    {
                        join_string = country + "," + state + "," + city + "," + street;
                    }
                    else
                    {
                        join_string = country + "," + city;
                    }

                    //geo location code goes here..
                    try
                    {

                        var address = join_string;
                        var requestUri = string.Format("http://maps.googleapis.com/maps/api/geocode/xml?address={0}&sensor=false", Uri.EscapeDataString(address));

                        var request = WebRequest.Create(requestUri);
                        var response = request.GetResponse();

                        var xdoc = XDocument.Load(response.GetResponseStream());

                        var result = xdoc.Element("GeocodeResponse").Element("result");
                        //MessageBox.Show("result = " + result);
                        var locationElement = result.Element("geometry").Element("location");
                        var lat = locationElement.Element("lat");
                        var lng = locationElement.Element("lng");

                        double lat2 = Double.Parse(lat.Value);
                        double lng2 = Double.Parse(lng.Value);
                        tb_latitude.Text = lat2.ToString();
                        tb_longitude.Text = lng2.ToString();

                        //now lets go for the elevation calculation 

                        //var latlng = lat2 + "," + lng2;
                        //var reqUri = string.Format("https://maps.googleapis.com/maps/api/elevation/xml?locations='"+latlng+"'&key=AIzaSyB27paQGkpwaVGAd7wp0N6rcFkq5JEKeiQ");
                        //MessageBox.Show("requrl= " + reqUri);
                        //var req = WebRequest.Create(reqUri);
                        //var resp = req.GetResponse();

                        //var xdoc1 = XDocument.Load(resp.GetResponseStream());
                        //MessageBox.Show("xdoc1= " + xdoc1);
                        //var result1 = xdoc1.Element("elevation").Value;
                        //MessageBox.Show("result1 = " + result1);
                        double elevation = getElevation(lat2, lng2);
                        //MessageBox.Show("elev= " + elevation);
                        tb_elevation.Text = elevation.ToString();
                        btn_insert_data.Enabled = true;

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                        btn_insert_data.Enabled = false;
                        tb_country.Text = "";
                        tb_city.Text = "";
                        tb_state.Text = "";
                        tb_street.Text = "";
                        tb_ZIP.Text = "";

                    }






                }//close of if...
            }//close of if int try parse.
            else
            {
                MessageBox.Show(WFA_psychometric_chart.Properties.Resources.Please_enter_a_valid_zip_numbe);
            }

        }


        private void button3_Click(object sender, EventArgs e)
        {
            //this is where online elevation and other facotrs are pulled..
            //pull_data_online();            
            

        }

        private void tb_country_text_change_event(object sender, EventArgs e)
        {
            string a = tb_country.Text;

            if(a.Length<0){
                //if the length is < 2 do nothing 

            }
            else if(a.Length>=0 && a.Length <= 2){
                  tb_state.Enabled = false;
                tb_city.Enabled = false;
            }
            else {
                //otherwise activate other textbox..

                tb_state.Enabled = true;
                tb_city.Enabled = true;


            }
            



        }

        private void tb_city_text_change_event(object sender, EventArgs e)
        {
            string a = tb_city.Text;

            if (a.Length < 0)
            {
                //if the length is < 2 do nothing 

            }
            else if (a.Length >= 0 && a.Length <= 2)
            {
                tb_street.Enabled = false;
                tb_ZIP.Enabled = false;
            }
            else
            {
                //otherwise activate other textbox..

                tb_street.Enabled = true;
                tb_ZIP.Enabled = true;


            }




        }

        private void tb_zip_textChange_event(object sender, EventArgs e)
        {
            //here we actually call the pulling function..
            pull_data_online(); 



        }

       

      public void Form4_insert_data_Disposed ( object sender, System.EventArgs e )
      {
      }

        private void Form4_insert_data_Load(object sender, EventArgs e)
        {

        }
    }
}
