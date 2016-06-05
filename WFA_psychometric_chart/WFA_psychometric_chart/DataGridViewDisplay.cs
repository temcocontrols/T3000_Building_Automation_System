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
    public partial class DataGridViewDisplay : Form
    {
        Form1_main form1;
        public DataGridViewDisplay(Form1_main formx)
        {
            this.form1 = formx;
            InitializeComponent();
        }

        private void DataGridViewDisplay_Load(object sender, EventArgs e)
        {
            //--On load function...
            DataGridView();
        }

        public void DataGridView()
        {
            //   DataGridView dtv = new DataGridView();
            //  DataTable dt = new DataTable();


            //   try {

            //dtv.ColumnCount = 5;
            //dtv.Columns.Add("nodeid", "node ID");
            //dtv.Columns.Add("temp", "temp");
            //dtv.Columns.Add("humidity", "hum");
            //dtv.Columns.Add("enthalpy", "enthalpy");
            //dtv.Columns.Add("label", "label");

            dataGridView1.ColumnCount = 5;
            //dataGridView1.Columns.Add("nodeid", "node ID");
            //dataGridView1.Columns.Add("temp", "temp");
            //dataGridView1.Columns.Add("humidity", "hum");
            //dataGridView1.Columns.Add("enthalpy", "enthalpy");
            //dataGridView1.Columns.Add("label", "label");


            //dataGridView1.Columns.Add();
            dataGridView1.Columns[0].Name = "node Id";

            dataGridView1.Columns[1].Name = "Temperature";
            dataGridView1.Columns[2].Name = "Humidity";
            dataGridView1.Columns[3].Name = "Enthalpy";
            dataGridView1.Columns[4].Name = "Label";


            //    dt.Columns.Equals(5);
            //dt.Columns.Add("node id");
            //dt.Columns.Add("tmep");

            //dt.Columns.Add("hum");
            //dt.Columns.Add("enth");
            //dt.Columns.Add("label");

            //dt.Columns[0].ColumnName = "nodeID";
            //    dt.Columns[1].ColumnName = "Temperature";
            //    dt.Columns[2].ColumnName = "Humidity";
            //    dt.Columns[3].ColumnName = "Enthalpy";
            //    dt.Columns[4].ColumnName = "Label";
            if (form1.menuStripNodeInfoValues.Count > 0)
            {

                for (int i = 0; i < form1.menuStripNodeInfoValues.Count; i++)
                {
                    // MessageBox.Show("ameo" + i);
                    form1.enthalpyHumidityCalculatorForXYvalue(form1.menuStripNodeInfoValues[i].xVal, form1.menuStripNodeInfoValues[i].yVal);
                    string[] row = new string[5] { form1.menuStripNodeInfoValues[i].id.ToString(), form1.menuStripNodeInfoValues[i].xVal.ToString(), form1.humDataGridValue.ToString(), form1.enthalpyDataGridView.ToString(), form1.menuStripNodeInfoValues[i].label };
                    //  dtv.Rows.Add(row);
                    //dt.NewRow().Equals(row);
                    //  dtv.Rows.Add(row);
                    dataGridView1.Rows.Add(row);
                    this.Controls.Add(dataGridView1);
                }
            }
            // dtv.DataSource = dt;

            //dtv.AutoGenerateColumns = false;
            //dtv.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dataGridView1.Show();
            // MessageBox.Show("dOING FINE");
            //}catch(Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}

        }

    }

}
