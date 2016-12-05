using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WFA_psychometric_chart
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //Show the language select dialog
            MultiLang.SelectLanguage frmLang = new MultiLang.SelectLanguage();
            //MessageBox.Show("frm lang" + frmLang);
            frmLang.LoadSettingsAndShow();
            frmLang.Dispose();
            frmLang = null;

            Application.Run(new Form1_main());
        }
    }
}
