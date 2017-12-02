using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using  WpfTestApplication;

using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Net;
using System.IO;

namespace RestfulTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string loginUrl = " https://www.bravocontrols.com/wp-json/wc/v2/products";
            string userName = "ck_5635f9d0ce065a46aecf8bb2a92fcccc1d5efd5a";
            string password = "cs_85135c93196d8c186a60a4e6d9a12441e595140a";

            //IDictionary<string, string> parameters = new Dictionary<string, string>();
            //parameters.Add("Username", userName);
            //parameters.Add("Password", password);
            CookieCollection cc = new CookieCollection();
            cc.Add(new Cookie("TYPE", "Basic Auth"));
            cc.Add(new Cookie("Username", userName));
            cc.Add(new Cookie("Password", password));
            HttpWebResponse response = HttpWebResponseUtility.CreateGetHttpResponse(loginUrl, null, null, cc);
            //(loginUrl, cc, null, null, Encoding.UTF8, null);
            var stream = response.GetResponseStream();
            StreamReader reader = new StreamReader(stream);
            string content = reader.ReadToEnd();
            reader.Close();
            stream.Close();

        }
    }
}
