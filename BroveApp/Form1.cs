using System;
using System.Collections.Generic;
 
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data;
using System.Threading;

using System.IO;

using System.Net;
using System.Reflection;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.ComponentModel;
using System.Drawing;
using WooCommerceNET.WooCommerce.v2;
using WooCommerceNET.WooCommerce.v2.Extension;
using WooCommerceNET;
using WooCommerceNET.Base;


namespace BroveApp
{

    public partial class MainFrm : Form
    {
        private IList<TemcoProduct> ITemcoProduct = new List<TemcoProduct>();

        //定义一个BindingList的学生链表

        private BindingList<TemcoProduct> BTProduct;

        public MainFrm()
        {
            InitializeComponent();
        }

        private async void getProductsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RestAPI rest = new RestAPI("", "", "");
            WCObject wc = new WCObject(rest);

            //Get all products
            var products = await wc.Product.GetAll();

            Thread.Sleep(2000);
            
            for (int i = 0; i < products.Count; i++)
            {
                // textBox3.Text += products[i].name + "\n";
                //listBox1.Items.Insert(i, products[i].name);
                TemcoProduct tp = new TemcoProduct();
                tp.ID = products[i].id.ToString();
                tp.Name = products[i].name.ToString();
                tp.Description = products[i].description.ToString();
                tp.Price = products[i].price.ToString();
                tp.Weight = products[i].weight.ToString();
                tp.ImagesLink = products[i].images[0].src.ToString();
                ITemcoProduct.Add(tp);
            }

            BTProduct = new BindingList<TemcoProduct>(ITemcoProduct);

            this.dataGridView1.DataSource = BTProduct;
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //string rowIndex = e.RowIndex.ToString();
            //string colIndex = e.ColumnIndex.ToString();

            TemcoProduct tp = new TemcoProduct();
            tp = ITemcoProduct[e.RowIndex];
            textBox_ID.Text = tp.ID;
            textBox_Name.Text = tp.Name;
            textBox_Price.Text = tp.Price;
            textBox_Weight.Text = tp.Weight;

           // string url = @"http://web.sr.gehua.net.cn/r/_u/239/295239/avatar/29523920150327184235.jpg";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(tp.ImagesLink);
            using (WebResponse response = request.GetResponse())
            {
                pictureBox_Image.Image  = Image.FromStream(response.GetResponseStream()); 
            }
            webBrowser_Description.DocumentText = tp.Description;


        }
    }
}
