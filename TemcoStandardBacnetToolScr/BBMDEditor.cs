/**************************************************************************
*                           MIT License
* 
* Copyright (C) 2017 Frederic Chaxel <fchaxel@free.fr>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO.BACnet;
using System.Threading;

namespace TemcoStandardBacnetTool
{
    public partial class BBMDEditor : Form
    {
        BacnetIpUdpProtocolTransport transport;
        IPEndPoint BBMDep;
        bool WriteInOperation = false;

        public BBMDEditor(BacnetClient comm, BacnetAddress adr)
        {
            InitializeComponent();

            DataGridViewCellStyle dgv = new DataGridViewCellStyle();
            dgv.Alignment = DataGridViewContentAlignment.MiddleCenter;

            BBMDTable.ColumnHeadersDefaultCellStyle = FDRTable.ColumnHeadersDefaultCellStyle = dgv;

            transport = (BacnetIpUdpProtocolTransport)comm.Transport;
            BBMDep = new IPEndPoint(System.Net.IPAddress.Parse(adr.ToString().Split(':')[0]),Convert.ToInt32(adr.ToString().Split(':')[1]));

            transport.Bvlc.MessageReceived += new BVLC.BVLCMessageReceiveHandler(Bvlc_MessageReceived);
        }

        void Bvlc_MessageReceived(System.Net.IPEndPoint sender, BacnetBvlcFunctions function, BacnetBvlcResults result, object data)
        {
            if (!sender.Equals(BBMDep)) return; // don't care

            if (InvokeRequired) // GUI call back
            {
                BeginInvoke(new Action<System.Net.IPEndPoint, BacnetBvlcFunctions, BacnetBvlcResults, object>(Bvlc_MessageReceived), new object[] { sender, function, result, data });
                return;
            }

            if (function == BacnetBvlcFunctions.BVLC_READ_BROADCAST_DIST_TABLE_ACK) // an table (could be empty)
            {
                List<Tuple<IPEndPoint, IPAddress>> Entries = (List<Tuple<IPEndPoint, IPAddress>>)data;
                BBMDTable.Rows.Clear();
                foreach (Tuple<IPEndPoint, IPAddress> tpl in Entries)
                {
                    BBMDTable.Rows.Add(new object[] { tpl.Item1.Address.ToString(), tpl.Item1.Port, tpl.Item2.ToString() });
                }
                lbBBMDlInfo.Visible = false;
            }

            if (function == BacnetBvlcFunctions.BVLC_READ_FOREIGN_DEVICE_TABLE_ACK)
            {
                List<Tuple<IPEndPoint, ushort, ushort>> Entries = (List<Tuple<IPEndPoint, ushort, ushort>>)data;
                FDRTable.Rows.Clear();
                foreach (Tuple<IPEndPoint, ushort, ushort> tpl in Entries)
                {
                    FDRTable.Rows.Add(new object[] { tpl.Item1.ToString(), tpl.Item2.ToString(), tpl.Item3.ToString() });
                }
                lbFDRInfo.Visible = false;
            }
            if ((function == BacnetBvlcFunctions.BVLC_RESULT) && (result == BacnetBvlcResults.BVLC_RESULT_SUCCESSFUL_COMPLETION) && (WriteInOperation))
            {
                WriteInOperation = false;
                transport.Bvlc.SendReadBroadCastTable(BBMDep);
            }

            if ((function == BacnetBvlcFunctions.BVLC_RESULT) && (result == BacnetBvlcResults.BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK))
            {
                lbBBMDlInfo.Visible = true;
                lbBBMDlInfo.Text = "Read Broadcast Table Rejected";
            }
            if ((function == BacnetBvlcFunctions.BVLC_RESULT) && (result == BacnetBvlcResults.BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK) && (WriteInOperation))
            {
                lbBBMDlInfo.Visible = true;
                WriteInOperation = false;
                lbBBMDlInfo.Text="Write Broadcast Table Rejected";
            }
            if ((function == BacnetBvlcFunctions.BVLC_RESULT) && (result == BacnetBvlcResults.BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK))
            {
                lbFDRInfo.Visible = true;
                lbFDRInfo.Text = "Read FDR Table Rejected";
            }            
        }

        private void BBMDEditor_Load(object sender, EventArgs e)
        {
            transport.Bvlc.SendReadBroadCastTable(BBMDep);
            Thread.Sleep(100); // wait a little
            transport.Bvlc.SendReadFDRTable(BBMDep);
        }

        private void bpWrite_Click(object sender, EventArgs e)
        {

            List<Tuple<IPEndPoint, IPAddress>> Entries = new List<Tuple<IPEndPoint, IPAddress>>();
            int idx = 0;

            try
            {
                foreach (DataGridViewRow dtr in BBMDTable.Rows)
                {
                    idx = dtr.Index;
                    if (!dtr.IsNewRow)
                    {
                        IPEndPoint ep = new IPEndPoint(System.Net.IPAddress.Parse(dtr.Cells["IPAddress"].Value.ToString()), Convert.ToInt32(dtr.Cells["Port"].Value.ToString()));
                        System.Net.IPAddress mask = System.Net.IPAddress.Parse(dtr.Cells["Mask"].Value.ToString());
                        Tuple<IPEndPoint, IPAddress> entry = new Tuple<IPEndPoint, IPAddress>(ep, mask);
                        Entries.Add(entry);
                    }

                }

                WriteInOperation = true;
                transport.Bvlc.SendWriteBroadCastTable(BBMDep, Entries);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error at line " + (idx + 1).ToString(), MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void BBMDEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            transport.Bvlc.MessageReceived -= new BVLC.BVLCMessageReceiveHandler(Bvlc_MessageReceived);
        }

        private void bpRefresh_Click(object sender, EventArgs e)
        {
            transport.Bvlc.SendReadFDRTable(BBMDep);
        }
    }
}
