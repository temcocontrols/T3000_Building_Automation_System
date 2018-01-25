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

    public partial class Form1 : Form
    {
        private IList<TemcoProduct> ITemcoProduct = new List<TemcoProduct>();

        public int PageID = 1;
        //定义一个BindingList的学生链表

        private BindingList<TemcoProduct> BTProduct;

        RestAPI rest;//= new RestAPI("https://www.bravocontrols.com/wp-json/wc/v2", "ck_5635f9d0ce065a46aecf8bb2a92fcccc1d5efd5a", "cs_85135c93196d8c186a60a4e6d9a12441e595140a");
        WCObject wc; //= new WCObject(rest);
        int m_current_Row;
        int m_current_Col;
        
        public Form1()
        {
            InitializeComponent();
              rest= new RestAPI("https://www.bravocontrols.com/wp-json/wc/v2", "ck_5635f9d0ce065a46aecf8bb2a92fcccc1d5efd5a", "cs_85135c93196d8c186a60a4e6d9a12441e595140a");
              wc= new WCObject(rest);
            m_current_Row = 0;
            m_current_Col = 0;
        }

        private  void getProductsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowData();
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
             m_current_Row = e.RowIndex;
             m_current_Col = e.ColumnIndex;

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

        private void button_UP_Click(object sender, EventArgs e)
        {
            if(PageID>1)
            {
                --PageID;
            }
            else
            {
              //  button_UP.Enabled = false;
            }
            ShowData();
        }


        private async void ShowData()
        {
            

            //Get all products
            var  products = await wc.Product.GetAll(new Dictionary<string, string>() {
                { "page", PageID.ToString() } });

            Thread.Sleep(2000);
            ITemcoProduct.Clear();
            for (int i = 0; i < (int)products.Count; i++)
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
                tp.SKU = products[i].sku;
                ITemcoProduct.Add(tp);
            }

            BTProduct = new BindingList<TemcoProduct>(ITemcoProduct);

            this.dataGridView1.DataSource = BTProduct;
        }

        private void button_Down_Click(object sender, EventArgs e)
        {
            ++PageID;
            ShowData();
        }
        private async void SaveData()
        {

          //  Product pd = new Product { name = textBox_Name.Text };

            Product pd = new Product { price = 10.0M };
            await wc.Product.Update(Convert.ToInt32(ITemcoProduct[m_current_Row].ID), pd);
        }
        private void Save_Click(object sender, EventArgs e)
        {

            SaveData();
            MessageBox.Show("OK");
        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            webBrowser_Description.Width = this.Width - dataGridView1.Width;
            webBrowser_Description.Height = this.Height - pictureBox_Image.Height;
        }
    }
}
