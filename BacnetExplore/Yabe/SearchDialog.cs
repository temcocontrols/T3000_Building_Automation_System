/**************************************************************************
*                           MIT License
* 
* Copyright (C) 2014 Morten Kvistgaard <mk@pch-engineering.dk>
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
using System.Text;
using System.Linq;
using System.Windows.Forms;
using System.IO.BACnet;
using SharpPcap.LibPcap;
using System.Runtime.InteropServices;
namespace Yabe
{
    public class BACnetSearchBackend
    {
        #region Search_Constants
        const int m_PortValue = 47808;
        const int m_Udp_MaxPayload = 1472;
        const int m_YabeDeviceId = -1;
        const int m_TimeoutValue = 1000;
        const int m_RetriesValue = 3;
        const bool m_Udp_ExclusiveUseOfSocket = false;
        const bool m_Udp_DontFragment = false;
        #endregion
        private string m_SearchNetwork;
        private BacnetClient m_Result;
        public BacnetClient Result
        {
            get
            {
                if (m_Result == null)
                {
                    m_Result = m_SearchNetwork.Contains(':')
                        ? new BacnetClient(new BacnetIpV6UdpProtocolTransport(m_PortValue, m_YabeDeviceId, m_Udp_ExclusiveUseOfSocket, m_Udp_DontFragment, m_Udp_MaxPayload, m_SearchNetwork), m_TimeoutValue, m_RetriesValue)
                        : new BacnetClient(new BacnetIpUdpProtocolTransport(m_PortValue, m_Udp_ExclusiveUseOfSocket, m_Udp_DontFragment, m_Udp_MaxPayload, m_SearchNetwork), m_TimeoutValue, m_RetriesValue);
                }

                return m_Result;
            }
        }

        public BACnetSearchBackend(string networktosearch)
        {
            m_SearchNetwork = networktosearch;
        }
    }

    public partial class SearchDialog : Form
    {
        public static int autocrun = 1;
        private BacnetClient m_result;

        public BacnetClient Result { get { return m_result; } }

        public void theout(object source, System.Timers.ElapsedEventArgs e)
        {
            if (autocrun == 1)
            {
                autocrun = 0;
                m_SearchIpButton_Click(this, null);//Fandu
            }
        }

        public SearchDialog(bool bIsVisible = true)
        {
            //System.Timers.Timer t = new System.Timers.Timer(1000);//实例化Timer类，设置间隔时间为10000毫秒；
            //t.Elapsed += new System.Timers.ElapsedEventHandler(theout);//到达时间的时候执行事件；
            //t.AutoReset = false;//设置是执行一次（false）还是一直执行(true)；
            //t.Enabled = true;//是否执行System.Timers.Timer.Elapsed事件；
            InitializeComponent();
            this.Visible = bIsVisible;

            //find all serial ports
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            m_SerialPortCombo.Items.AddRange(ports);
            m_SerialPtpPortCombo.Items.AddRange(ports);

            if (Environment.OSVersion.Platform == System.PlatformID.Win32Windows) 
            {
                //find all pipe transports that's pretending to be com ports  : fail on Linux
                ports = BacnetPipeTransport.AvailablePorts;
                foreach (string str in ports)
                    if (str.StartsWith("com", StringComparison.InvariantCultureIgnoreCase))
                    {
                        m_SerialPortCombo.Items.Add(str);
                        m_SerialPtpPortCombo.Items.Add(str);
                    }

                //select first
                if (m_SerialPortCombo.Items.Count > 0) m_SerialPortCombo.SelectedItem = m_SerialPortCombo.Items[0];
                if (m_SerialPtpPortCombo.Items.Count > 0) m_SerialPtpPortCombo.SelectedItem = m_SerialPtpPortCombo.Items[0];
            }
            //this.m_AddUdpButton.Click += new System.EventHandler(this.m_AddSerialButton_Click);
            //m_AddUdpButton.PerformClick();
            //m_SearchIpButton_Click(this,);
            //m_SearchIpButton_Click(this, null);//Fandu
        }

        private void m_SearchIpButton_Click(object sender, EventArgs e)
        {
            String adr = Properties.Settings.Default.DefaultUdpIp;
            if (adr.Contains(':'))
                m_result = new BacnetClient(new BacnetIpV6UdpProtocolTransport((int)m_PortValue.Value, Properties.Settings.Default.YabeDeviceId, Properties.Settings.Default.Udp_ExclusiveUseOfSocket, Properties.Settings.Default.Udp_DontFragment, Properties.Settings.Default.Udp_MaxPayload, adr), (int)m_TimeoutValue.Value, (int)m_RetriesValue.Value);
            else
                m_result = new BacnetClient(new BacnetIpUdpProtocolTransport((int)m_PortValue.Value, Properties.Settings.Default.Udp_ExclusiveUseOfSocket, Properties.Settings.Default.Udp_DontFragment, Properties.Settings.Default.Udp_MaxPayload, adr), (int)m_TimeoutValue.Value, (int)m_RetriesValue.Value);

            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void m_AddEthernetButton_Click(object sender, EventArgs e)
        {
            try
            {
                String[] s = m_EthernetInterfaceCombo.Text.Split(':');

                m_result = new BacnetClient(new BacnetEthernetProtocolTransport(s[0]), (int)m_TimeoutValue.Value, (int)m_RetriesValue.Value);
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
            }
            catch{}

            this.Close();
        }

        private void m_AddSerialButton_Click(object sender, EventArgs e)
        {
            try
            {
                int com_number = 0;
                if (m_SerialPortCombo.Text.Length >= 3) int.TryParse(m_SerialPortCombo.Text.Substring(3), out com_number);
                BacnetMstpProtocolTransport transport;
                if (com_number >= 1000)      //these are my special "pipe" com ports 
                    transport = new BacnetMstpProtocolTransport(new BacnetPipeTransport(m_SerialPortCombo.Text), (short)m_SourceAddressValue.Value, (byte)m_MaxMasterValue.Value, (byte)m_MaxInfoFramesValue.Value);
                else
                    transport = new BacnetMstpProtocolTransport(m_SerialPortCombo.Text, (int)m_BaudValue.Value, (short)m_SourceAddressValue.Value, (byte)m_MaxMasterValue.Value, (byte)m_MaxInfoFramesValue.Value);
                transport.StateLogging = Properties.Settings.Default.MSTP_LogStateMachine;
                m_result = new BacnetClient(transport, (int)m_TimeoutValue.Value, (int)m_RetriesValue.Value);
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            }
            catch { }
        }

        private void m_AddPtpSerialButton_Click(object sender, EventArgs e)
        {
            try
            {
                int com_number = 0;
                if (m_SerialPtpPortCombo.Text.Length >= 3) int.TryParse(m_SerialPtpPortCombo.Text.Substring(3), out com_number);
                BacnetPtpProtocolTransport transport;
                if (com_number >= 1000)      //these are my special "pipe" com ports 
                    transport = new BacnetPtpProtocolTransport(new BacnetPipeTransport(m_SerialPtpPortCombo.Text), false);
                else
                    transport = new BacnetPtpProtocolTransport(m_SerialPtpPortCombo.Text, (int)m_BaudValue.Value, false);
                transport.Password = m_PasswordText.Text;
                transport.StateLogging = Properties.Settings.Default.MSTP_LogStateMachine;
                m_result = new BacnetClient(transport, (int)m_TimeoutValue.Value, (int)m_RetriesValue.Value);
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            }
            catch { }
        }


        private void FillEthernetInterface()
        {
            var devices = LibPcapLiveDeviceList.Instance.Where(dev => dev.Interface != null);

            foreach (var device in devices)
            {
                if (device.Opened == false) // Don't re-open and lists an already open interface
                {
                    try
                    {
                        device.Open();
                        if (device.LinkType == PacketDotNet.LinkLayers.Ethernet
                            && device.Interface.MacAddress != null)
                            m_EthernetInterfaceCombo.Items.Add(device.Interface.FriendlyName + ": " + device.Interface.Description);
                        device.Close();
                    }
                    catch { }
                }
            }
        }

        private void SearchDialog_Load(object sender, EventArgs e)
        {
            //get all local endpoints for udp
            string[] local_endpoints = GetAvailableIps();
            m_localUdpEndpointsCombo.Items.Clear();
            m_localUdpEndpointsCombo.Items.AddRange(local_endpoints);

            string str3 = System.IO.Directory.GetCurrentDirectory();// System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            str3 = str3 + "\\T3000_config.ini";
            StringBuilder AutoLocalIP = new StringBuilder();
            GetPrivateProfileString("Yabe", "LocalPCIPaddress", "", AutoLocalIP, 255, str3);
            //WritePrivateProfileString("Yabe", "LocalPCIPaddress", "", str3);

            this.m_localUdpEndpointsCombo.Text = AutoLocalIP.ToString();// "192.168.0.38"; //fandu
            // try to get Ethernet interfaces
            try
            {
                FillEthernetInterface();
            }
            catch { }
        }

        [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern bool WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFileName);

        [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);


        public static string[] GetAvailableIps()
        {
            List<string> ips = new List<string>();
            System.Net.NetworkInformation.NetworkInterface[] interfaces = System.Net.NetworkInformation.NetworkInterface.GetAllNetworkInterfaces();
            foreach (System.Net.NetworkInformation.NetworkInterface inf in interfaces)
            {
                if (!inf.IsReceiveOnly && inf.OperationalStatus == System.Net.NetworkInformation.OperationalStatus.Up && inf.SupportsMulticast && inf.NetworkInterfaceType != System.Net.NetworkInformation.NetworkInterfaceType.Loopback)
                {
                    System.Net.NetworkInformation.IPInterfaceProperties ipinfo = inf.GetIPProperties();
                    //if (ipinfo.GatewayAddresses == null || ipinfo.GatewayAddresses.Count == 0 || (ipinfo.GatewayAddresses.Count == 1 && ipinfo.GatewayAddresses[0].Address.ToString() == "0.0.0.0")) continue;
                    foreach (System.Net.NetworkInformation.UnicastIPAddressInformation addr in ipinfo.UnicastAddresses)
                    {
                        if ( (addr.Address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork) || 
                            ((addr.Address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetworkV6)&&Properties.Settings.Default.IPv6_Support))
                        {
                            ips.Add(addr.Address.ToString());
                        }
                    }
                }
            }
            return ips.ToArray();
        }

    }
}
