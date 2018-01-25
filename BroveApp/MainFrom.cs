using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BroveApp
{
    public partial class MainFrom : Form
    {
        public MainFrom()
        {
            InitializeComponent();
        }

        private void productsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form1 msf = new Form1();
            msf.MdiParent = this;//设置窗体的父子关系
            msf.Parent = panelFrm;//设置子窗体的容器为父窗体中的Panel
            msf.Show();//显示子窗体，此句很重要，否则子窗体不会显示
        }

        private void MainFrom_Resize(object sender, EventArgs e)
        {
           
        }

        private void MainFrom_SizeChanged(object sender, EventArgs e)
        {
            panelFrm.Width = this.Width;
            panelFrm.Height = this.Height;
        }
    }
}
