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
    public partial class Form_PressureInput : Form
    {
        FormEditNodeAndLine frm;
        public Form_PressureInput(FormEditNodeAndLine f)
        {
            frm = f;
            InitializeComponent();
        }

        private void btn_ok_Click(object sender, EventArgs e)
        {
            //--This is a button click event...

            try
            {
                string input = textBox1.Text.Trim();
                Cursor.Current = Cursors.WaitCursor;
                //EditNodeLineForm ef = new EditNodeLineForm()
                frm.PressureInputPorcessForDevice(input);
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
