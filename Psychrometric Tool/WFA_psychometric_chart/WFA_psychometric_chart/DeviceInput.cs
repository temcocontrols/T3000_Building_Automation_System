using System;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    public partial class Form_DeviceInput : Form
    {
        EditNodeLineForm frm;
        public Form_DeviceInput(EditNodeLineForm fx)
        {
            frm = fx;
            InitializeComponent();
        }

        private void btn_ok_Click(object sender, EventArgs e)
        {
            string input = textBox1.Text.Trim();
            Cursor.Current = Cursors.WaitCursor;

            //EditNodeLineForm ef = new EditNodeLineForm()
            frm.TemperatureInputPorcessForDevice(input);
            //MessageBox.Show("End");

            Cursor.Current = Cursors.Default;
            this.Close();

        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {
          //  string textAvailable = textBox1.Text.Trim();

          //string t=   textAvailable.ToUpper();
          //  textBox1.Text = t;//textAvailable;
        }
    }
}
