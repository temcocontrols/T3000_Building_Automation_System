using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PH_App
{
    public partial class Form_TemperatureInput : Form
    {
        FormEditNodeAndLine frm;
        public Form_TemperatureInput(FormEditNodeAndLine f)
        {
            frm = f;
            InitializeComponent();
        }

        private void btn_ok_Click(object sender, EventArgs e)
        {
            //--Okie clicked

            try
            {
                //this.Focus(); //This just focuses on this item rather than textbox
                string input = textBox1.Text.Trim();
                Cursor.Current = Cursors.WaitCursor;
                if (input == "")
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

    }
}
