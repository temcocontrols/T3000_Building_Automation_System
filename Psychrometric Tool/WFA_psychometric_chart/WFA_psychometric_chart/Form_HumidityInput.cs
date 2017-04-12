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
    public partial class Form_HumidityInput : Form
    {
        EditNodeLineForm frm;
        public Form_HumidityInput(EditNodeLineForm f)
        {
            frm = f;
            InitializeComponent();
        }

        private void btn_ok_Click(object sender, EventArgs e)
        {
            try
            { 
            string input = textBox1.Text.Trim();
            Cursor.Current = Cursors.WaitCursor;
            //EditNodeLineForm ef = new EditNodeLineForm()
            frm.HumidityInputPorcessForDevice(input);
            frm.LoadNodeAndLine();
            //MessageBox.Show("End");
            Cursor.Current = Cursors.Default;
            this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"{ex.Message} \n Please select input, output and variable section in T3000 and retry!");
                this.Close();
            }
        }
    }
}
