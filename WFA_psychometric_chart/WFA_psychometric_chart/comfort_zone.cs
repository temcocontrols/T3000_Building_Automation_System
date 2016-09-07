using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class comfort_zone : Form
    {
        buildingChartSetting bs;
        public comfort_zone(buildingChartSetting b)
        {
            InitializeComponent();
            this.bs = b;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

            try {
                if(textBox1.Text != "") {  
                    double t =double.Parse(textBox1.Text);
                    if(t >=0 && t<= 50)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Temperature should be between 0 to 50 Deg.C ");
                        textBox1.Text = 0.ToString();
                    }
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Value should be numeric : "+ex.Message);
                textBox1.Text = 0.ToString();
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (textBox2.Text != "")
                {
                    double t = double.Parse(textBox2.Text);
                    if (t >= 0 && t <= 50)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Temperature should be between 0 to 50 Deg.C ");
                        textBox2.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                textBox2.Text = 0.ToString();
            }
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (textBox3.Text != "")
                {
                    double h = double.Parse(textBox3.Text);
                    if (h >= 0 && h <= 100)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Humidity should be between 0 to 100 % ");
                        textBox3.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                textBox3.Text = 0.ToString();
            }
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (textBox4.Text != "")
                {
                    double h = double.Parse(textBox4.Text);
                    if (h >= 0 && h <= 100)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Humidity should be between 0 to 100 % ");
                        textBox4.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                textBox4.Text = 0.ToString();
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = comboBox1.SelectedIndex;
            if(index >= 0)
            {
                //put the values in the combobox
                textBox1.Text = bs.listComfortZoneDetail[index].min_temp;
                textBox2.Text = bs.listComfortZoneDetail[index].max_temp;
                textBox3.Text = bs.listComfortZoneDetail[index].min_hum;
                textBox4.Text = bs.listComfortZoneDetail[index].max_hum;
                btn_ok.Enabled = true;
            } 

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            //On check box checked...
            if (checkBox1.Checked == true)
            {
                //it is checked 
                this.Height = 506;
                groupBox2.Enabled = true;
                btn_add.Enabled = true;
                tb_name.Text = "";
                tb_mintemp.Text = "";
                tb_maxtemp.Text = "";
                tb_minhum.Text = "";
                tb_maxhum.Text = "";
                btn_update.Enabled = false;
                btn_delete.Enabled = false;
                btn_color.BackColor = Color.LightGreen;

                //when checked we need to fulfill the combobox2 as well
                //Now lets load the values int combobox1.items.add()
                //we need to reset first
                comboBox2.Items.Clear();
                comboBox2.Refresh();
                foreach (var item in bs.listComfortZoneDetail)
                {
                    comboBox2.Items.Add(item.name);
                }


            }
            else
            {
                //It is not checked
                groupBox2.Enabled = false;
                btn_add.Enabled = false;
                this.Height = 280;
            }
        }

        private void comfort_zone_Load(object sender, EventArgs e)
        {
            //on load it need to pull the data form database and it need to reset the values as well
            groupBox2.Enabled = false;
            this.Height = 280;

            //pull data form database.. first


            bs.PullComfortZoneData();//This will load the listComfortZoneDetail
            btn_color.BackColor = Color.LightGreen;

            //Now lets load the values int combobox1.items.add()
            comboBox1.Items.Clear();
            foreach (var item in bs.listComfortZoneDetail) { 
            comboBox1.Items.Add(item.name);
            }



        }

        private void button1_Click(object sender, EventArgs e)
        {
            //When ok button is clicked the there should be plotting of the comfort zone.
            //This has basically two function...
            /*
            1.insert the comfort zone info to database 
            2. Plot the comfort zone based on the value passed.
            */
            if(textBox1.Text !="" && textBox2.Text !="" && textBox3.Text !="" &&textBox4.Text != "") { 
            //chart id
            string chartid = bs.chartDetailList[bs.indexOfChartSelected].chartID;
            string comfortzoneid = bs.listComfortZoneDetail[comboBox1.SelectedIndex].id;
            bs.InsertChartComfortzoneInfo(chartid, comfortzoneid);

                //now second part call the plot function 
                // bs.PlotComfortZone()
                double mint = double.Parse(textBox1.Text);
                double maxt = double.Parse(textBox2.Text);
                double minh = double.Parse(textBox3.Text);
                double maxh = double.Parse(textBox4.Text);
                Color c = bs.listComfortZoneDetail[comboBox1.SelectedIndex].colorValue;
                string name = bs.listComfortZoneDetail[comboBox1.SelectedIndex].name;
                if (bs.default_comfort_zone_of_chart.Count > 0)
                {
                    if (bs.listchartComfortZoneInfoSingle.Count > 0) {
                        //If default comfort zone is present then 
                        //Clear previous one 
                        //MessageBox.Show("Clear previous comfortzone");
                     bs.ClearComfortZone(double.Parse(bs.listchartComfortZoneInfoSingle[0].min_temp), double.Parse(bs.listchartComfortZoneInfoSingle[0].max_temp), double.Parse(bs.listchartComfortZoneInfoSingle[0].min_hum), double.Parse(bs.listchartComfortZoneInfoSingle[0].max_hum));
                    }
                }

                bs.PlotComfortZone(mint, maxt, minh, maxh, c, name);

                //insert of update database value 
                bs.insertOrUpdateComfortChartSetting(chartid, comfortzoneid);

            }//close of if


        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        { int index = comboBox2.SelectedIndex;
            if (index >= 0)
            {
                //We need to enable the button update.
                btn_update.Enabled = true;
                btn_delete.Enabled = true;
                btn_add.Enabled = false;
                //Next we need to fulfil the text boxes base on the combobox selected
                tb_name.Text = bs.listComfortZoneDetail[index].name;
                tb_mintemp.Text = bs.listComfortZoneDetail[index].min_temp;
                tb_maxtemp.Text = bs.listComfortZoneDetail[index].max_temp;
                tb_minhum.Text = bs.listComfortZoneDetail[index].min_hum;
                tb_maxhum.Text = bs.listComfortZoneDetail[index].max_hum;
                btn_color.BackColor = bs.listComfortZoneDetail[index].colorValue;

            }

        }

        private void btn_update_Click(object sender, EventArgs e)
        {
            //now we can update at this point the previous values.
            if (tb_name.Text != "")
            {

                if (tb_mintemp.Text != "" && tb_maxtemp.Text != "" && tb_minhum.Text != "" && tb_maxhum.Text != "")
                {
                    //we need to grab the value and insert it
                    double minT = double.Parse(tb_mintemp.Text);
                    double maxT = double.Parse(tb_maxtemp.Text);
                    double minH = double.Parse(tb_minhum.Text);
                    double maxH = double.Parse(tb_maxhum.Text);
                    string name = tb_name.Text;
                    Color c = btn_color.BackColor;
                    string id = bs.listComfortZoneDetail[comboBox2.SelectedIndex].id;
                    // bs.InsertComfortZoneValue(name, minT, maxT, minH, maxH, c);
                    bs.UpdateComfortZoneValue(id, name, minT, maxT, minH, maxH, c);
                    MessageBox.Show("Value updated succefully");
                    //NOW LEST REFRESH THE FORM
                    refreshAfterEdit();
                }
                else
                {
                    MessageBox.Show("Please fill the temperature and humidity accurately");

                }
            }
            else
            {
                MessageBox.Show("Enter the name of the comfortzone");
            }


        }

        public void refreshAfterEdit()
        {
            //Now lets do some of the refreshing task
            bs.PullComfortZoneData();//This will pull the co

            //Now lets load the values int combobox1.items.add()
            comboBox1.Items.Clear();
            foreach (var item in bs.listComfortZoneDetail)
            {
                comboBox1.Items.Add(item.name);
            }


            //Now lets load the values int combobox1.items.add()
            comboBox2.Items.Clear();
            foreach (var item in bs.listComfortZoneDetail)
            {
                comboBox2.Items.Add(item.name);
            }

            tb_name.Text = "";
            tb_mintemp.Text = "";
            tb_maxtemp.Text = "";
            tb_minhum.Text = "";
            tb_maxhum.Text = "";

            //Also first text boxes need to be refreshed
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
            textBox4.Text = "";
            btn_ok.Enabled = false;
            btn_update.Enabled = false;
            btn_delete.Enabled = false;
            btn_add.Enabled = true;

        }
        private void btn_add_Click(object sender, EventArgs e)
        {
            //We can add new value in thsi function 
            //now lets do the somethings.
            if(tb_name.Text != "") { 

                if(tb_mintemp.Text != "" && tb_maxtemp.Text != "" && tb_minhum.Text != "" && tb_maxhum.Text !="")
                {
                    //we need to grab the value and insert it
                    double minT = double.Parse(tb_mintemp.Text);
                    double maxT = double.Parse(tb_maxtemp.Text);
                    double minH = double.Parse(tb_minhum.Text);
                    double maxH = double.Parse(tb_maxhum.Text);
                    string name = tb_name.Text;
                    Color c = btn_color.BackColor;
                    bs.InsertComfortZoneValue(name, minT, maxT, minH, maxH, c);
                    MessageBox.Show("insert success");

                    //NOW LEST REFRESH THE FORM
                    refreshAfterEdit();

                }
                else
                {
                    MessageBox.Show("Please fill the temperature and humidity accurately");

                 }
            }
            else
            {
                MessageBox.Show("Enter the name of the comfortzone");
            }
            //Inserted message
            //MessageBox.Show("insert end");
        }

        private void btn_delete_Cl(object sender, EventArgs e)
        {
            //This one is for deletion operation
            int index = comboBox2.SelectedIndex;
            if (index >= 0)
            {
                //WE need to call the index value as well as send the id parameter
                //This will delete the value form both the comfortDetail and chartcomfortzonesetting table.
                 DialogResult dialogResult = MessageBox.Show("are you sure you want to delete :"+ bs.listComfortZoneDetail[index].name, "Delete", MessageBoxButtons.YesNo);
                if(dialogResult == DialogResult.Yes) { 
                    bs.DeleteComfortZoneData(bs.listComfortZoneDetail[index].id);
                    MessageBox.Show("value deleted successfully");
                    //Refresh the values
                    refreshAfterEdit();
                }


            }
        }

        private void btn_color_Click(object sender, EventArgs e)
        {
            ColorDialog colorDialog1 = new ColorDialog();
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                btn_color.BackColor = colorDialog1.Color;

            }
        }

        private void tb_mintemp_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (tb_mintemp.Text != "")
                {
                    double t = double.Parse(tb_mintemp.Text);
                    if (t >= 0 && t <= 50)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Temperature should be between 0 to 50 Deg.C ");
                        tb_mintemp.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                tb_mintemp.Text = 0.ToString();
            }

        }

        private void tb_maxtemp_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (tb_maxtemp.Text != "")
                {
                    double t = double.Parse(tb_maxtemp.Text);
                    if (t >= 0 && t <= 50)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Temperature should be between 0 to 50 Deg.C ");
                        tb_maxtemp.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                tb_maxtemp.Text = 0.ToString();
            }
        }

        private void tb_minhum_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (tb_minhum.Text != "")
                {
                    double h = double.Parse(tb_minhum.Text);
                    if (h >= 0 && h <= 100)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Humidity should be between 0 to 100 % ");
                        tb_minhum.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                tb_minhum.Text = 0.ToString();
            }
        }

        private void tb_maxhum_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (tb_maxhum.Text != "")
                {
                    double h = double.Parse(tb_maxhum.Text);
                    if (h >= 0 && h <= 100)
                    {

                    }
                    else
                    {
                        MessageBox.Show("Humidity should be between 0 to 100 % ");
                        tb_maxhum.Text = 0.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Value should be numeric : " + ex.Message);
                tb_maxhum.Text = 0.ToString();
            }
        }
    }
}
