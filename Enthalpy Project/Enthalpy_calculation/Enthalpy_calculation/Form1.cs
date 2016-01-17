using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Enthalpy_calculation
{
    public partial class Form1 : Form
    {
        double temperature, humidity, Patm, TDewpoint, A, m, Tn, B, Pws, X, h;
        public Form1()
        {


            InitializeComponent();
        }

        private void btnCalculate_Click(object sender, EventArgs e)
        {

            try
            {
          
                tb_temp.Text = Convert.ToString(temperature);
                tb_humi.Text = Convert.ToString(humidity);
                Patm = 1013;
                A = 6.116441;
                m = 7.591386;
                Tn = 240.7263;
                B = 621.9907;

                tb_b.Text = Convert.ToString(B);
                tb_tn.Text = Convert.ToString(Tn);
                tb_m.Text = Convert.ToString(m);
                tb_a.Text = Convert.ToString(A);
                tb_atm.Text = Convert.ToString(Patm);

                TDewpoint = 243.04 * (Math.Log(humidity / 100) + ((17.625 * temperature) / (243.04 + temperature))) / (17.625 - Math.Log(humidity / 100) - ((17.625 * temperature) / (243.04 + temperature)));
                tb_td.Text = Convert.ToString(TDewpoint);
                Pws = A * Math.Pow(10, (m * TDewpoint) / (TDewpoint + Tn));
                tb_pws.Text = Convert.ToString(Pws);

                X = B * Pws / (Patm - Pws);
                tb_x.Text = Convert.ToString(X);

                h = temperature * (1.01 + (0.00189 * X)) + 2.5 * X;
                tb_h.Text = Convert.ToString(h);

            }
            catch (Exception mg)
            {
                MessageBox.Show(mg.Message);
            }


        }

        private void radio_si_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void txtCheck_TextChanged(object sender, EventArgs e)
        {
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            try
            {
                StreamReader reader = new StreamReader("input.txt");
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    string[] splitted = line.Split('#');
                    string first = splitted[0].Trim();
                    string second = splitted[1].Trim();
                    temperature = Convert.ToInt32(first);
                    humidity = Convert.ToInt32(second);

                }
                tb_temp.Text = Convert.ToString(temperature);
                tb_humi.Text = Convert.ToString(humidity);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }



        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            try
            {
                tb_temp.Text = ""; 
                tb_humi.Text = "";
                tb_h.Text = "";
                tb_td.Text = "";
                tb_pws.Text = "";
                tb_x.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DataTable dtTextFileData = new DataTable();
            dtTextFileData.Columns.AddRange(new[]
        {
            new DataColumn("First", typeof(string)), 
            new DataColumn("Second", typeof(string)) 
            });

            StreamReader file = new StreamReader("input.txt");
            string line = file.ReadLine();
            while (line != null)
            {
                string[] fields = line.Split('#');
                DataRow dr = dtTextFileData.NewRow();
                dr["First"] = fields[0].ToString();
                dr["Second"] = fields[1].ToString();
                dtTextFileData.Rows.Add(dr);
                line = file.ReadLine();
            }
        }
    }
}
