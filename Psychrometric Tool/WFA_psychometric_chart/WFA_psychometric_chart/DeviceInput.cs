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
            try {
            //this.Focus(); //This just focuses on this item rather than textbox
            string input = textBox1.Text.Trim();
            Cursor.Current = Cursors.WaitCursor;
                if(input == "")
                {
                    //MessageBox.Show("Enter the input key");
                    return;//Enter values
                }

            //EditNodeLineForm ef = new EditNodeLineForm()
            frm.TemperatureInputPorcessForDevice(input);
            //MessageBox.Show("End");
            frm.LoadNodeAndLine();

            Cursor.Current = Cursors.Default;
            this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"{ex.Message} \n Please select input, output and variable section in T3000 and retry!");
                this.Close();
            }
        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {
          //  string textAvailable = textBox1.Text.Trim();

          //string t=   textAvailable.ToUpper();
          //  textBox1.Text = t;//textAvailable;
        }
    }
}
