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
using System.Text;
using System.Linq;
using System.IO.BACnet.Serialize;
using System.Diagnostics;
using System.Net;
using System.Text.RegularExpressions;

namespace System.IO.BACnet
{
    public delegate void MessageRecievedHandler(IBacnetTransport sender, byte[] buffer, int offset, int msg_length, BacnetAddress remote_address);

    public interface IBacnetTransport : IDisposable
    {
        event MessageRecievedHandler MessageRecieved;
        int Send(byte[] buffer, int offset, int data_length, BacnetAddress address, bool wait_for_transmission, int timeout);
        BacnetAddress GetBroadcastAddress();
        BacnetAddressTypes Type { get; }
        void Start();

        int HeaderLength { get; }
        int MaxBufferLength { get; }
        BacnetMaxAdpu MaxAdpuLength { get; }

        bool WaitForAllTransmits(int timeout);
        byte MaxInfoFrames { get; set; }
    }

    /// <summary>
    /// This is the standard BACNet udp transport
    /// </summary>
    public class BacnetIpUdpProtocolTransport : IBacnetTransport, IDisposable
    {
        private System.Net.Sockets.UdpClient m_shared_conn;
        private System.Net.Sockets.UdpClient m_exclusive_conn;
        private int m_port;

        private BVLC bvlc;

        public BVLC Bvlc { get { return bvlc; } }

        private bool m_exclusive_port = false;
        private bool m_dont_fragment;
        private int m_max_payload;
        private string m_local_endpoint;

        public BacnetAddressTypes Type { get { return BacnetAddressTypes.IP; } }
        public event MessageRecievedHandler MessageRecieved;
        public int SharedPort { get { return m_port; } }
        public int ExclusivePort { get { return ((Net.IPEndPoint)m_exclusive_conn.Client.LocalEndPoint).Port; } }

        public int HeaderLength { get { return BVLC.BVLC_HEADER_LENGTH; } }
        public BacnetMaxAdpu MaxAdpuLength { get { return BVLC.BVLC_MAX_APDU; } }
        public byte MaxInfoFrames { get { return 0xff; } set { /* ignore */ } }     //the udp doesn't have max info frames
        public int MaxBufferLength { get { return m_max_payload; } }

        public BacnetIpUdpProtocolTransport(int port, bool use_exclusive_port = false, bool dont_fragment = false, int max_payload = 1472, string local_endpoint_ip = "")
        {
            m_port = port;
            m_max_payload = max_payload;
            m_exclusive_port = use_exclusive_port;
            m_dont_fragment = dont_fragment;
            m_local_endpoint = local_endpoint_ip;

        }

        public override bool Equals(object obj)
        {
            if (!(obj is BacnetIpUdpProtocolTransport)) return false;
            BacnetIpUdpProtocolTransport a = (BacnetIpUdpProtocolTransport)obj;
            return a.m_port == m_port;
        }

        public override int GetHashCode()
        {
            return m_port.GetHashCode();
        }

        public override string ToString()
        {
            return "Udp:" + m_port;
        }

        private void Open()
        {

            if (!m_exclusive_port)
            {
                /* We need a shared broadcast "listen" port. This is the 0xBAC0 port */
                /* This will enable us to have more than 1 client, on the same machine. Perhaps it's not that important though. */
                /* We (might) only recieve the broadcasts on this. Any unicasts to this might be eaten by another local client */
                if (m_shared_conn == null)
                {
                    m_shared_conn = new Net.Sockets.UdpClient();
                    m_shared_conn.ExclusiveAddressUse = false;
                    m_shared_conn.Client.SetSocketOption(Net.Sockets.SocketOptionLevel.Socket, Net.Sockets.SocketOptionName.ReuseAddress, true);
                    System.Net.EndPoint ep = new System.Net.IPEndPoint(System.Net.IPAddress.Any, m_port);
                    if (!string.IsNullOrEmpty(m_local_endpoint)) ep = new System.Net.IPEndPoint(Net.IPAddress.Parse(m_local_endpoint), m_port);
                    m_shared_conn.Client.Bind(ep);
                    m_shared_conn.DontFragment = m_dont_fragment;
                }

                /* This is our own exclusive port. We'll receive everything sent to this. */
                /* So this is how we'll present our selves to the world */
                if (m_exclusive_conn == null)
                {
                    System.Net.EndPoint ep = new Net.IPEndPoint(System.Net.IPAddress.Any, 0);
                    if (!string.IsNullOrEmpty(m_local_endpoint)) ep = new Net.IPEndPoint(Net.IPAddress.Parse(m_local_endpoint), 0);

                    // Opens the socket, udp will choose the Port number
                    m_exclusive_conn = new Net.Sockets.UdpClient((Net.IPEndPoint)ep);
                    // Gets the Endpoint : the assigned Udp port number in fact
                    ep = m_exclusive_conn.Client.LocalEndPoint;
                    // closes the socket
                    m_exclusive_conn.Close();
                    // Re-opens it with the freeed port number, to be sure it's a real active/server socket
                    // which cannot be disarmed for listen by .NET for incoming call after a few inactivity
                    // minutes ... yes it's like this at least on several systems
                    m_exclusive_conn = new Net.Sockets.UdpClient((Net.IPEndPoint)ep);

                    m_exclusive_conn.DontFragment = m_dont_fragment;
                    m_exclusive_conn.EnableBroadcast = true;
                }
            }
            else
            {
                System.Net.EndPoint ep = new Net.IPEndPoint(System.Net.IPAddress.Any, m_port);
                if (!string.IsNullOrEmpty(m_local_endpoint)) ep = new Net.IPEndPoint(Net.IPAddress.Parse(m_local_endpoint), m_port);
                m_exclusive_conn = new Net.Sockets.UdpClient();
                m_exclusive_conn.ExclusiveAddressUse = true;
                m_exclusive_conn.Client.Bind((Net.IPEndPoint)ep);
                m_exclusive_conn.DontFragment = m_dont_fragment; m_exclusive_conn.EnableBroadcast = true;
            }

            bvlc = new BVLC(this);
        }

        private void Close()
        {
            m_exclusive_conn.Close();
        }

        public void Start()
        {
            Open();

            if (m_shared_conn != null)
                m_shared_conn.BeginReceive(OnReceiveData, m_shared_conn);

            if (m_exclusive_conn != null)
                m_exclusive_conn.BeginReceive(OnReceiveData, m_exclusive_conn);

        }

        private void OnReceiveData(IAsyncResult asyncResult)
        {
            System.Net.Sockets.UdpClient conn = (System.Net.Sockets.UdpClient)asyncResult.AsyncState;
            try
            {
                System.Net.IPEndPoint ep = new Net.IPEndPoint(System.Net.IPAddress.Any, 0);
                byte[] local_buffer;
                int rx = 0;

                try
                {
                    local_buffer = conn.EndReceive(asyncResult, ref ep);
                    rx = local_buffer.Length;
                }
                catch (Exception) // ICMP port unreachable
                {
                    //restart data receive
                    conn.BeginReceive(OnReceiveData, conn);
                    return;
                }

                if (rx == 0)    // Empty frame : port scanner maybe
                {
                    //restart data receive
                    conn.BeginReceive(OnReceiveData, conn);
                    return;
                }

                try
                {
                    // restart data reception before the job is finish ... could be dangerous
                    conn.BeginReceive(OnReceiveData, conn);

                    //verify message
                    BacnetAddress remote_address;
                    Convert((System.Net.IPEndPoint)ep, out remote_address);
                    BacnetBvlcFunctions function;
                    int msg_length;
                    if (rx < BVLC.BVLC_HEADER_LENGTH)
                    {
                        Trace.TraceWarning("Some garbage data got in");
                    }
                    else
                    {
                        // Basic Header lenght
                        int HEADER_LENGTH = bvlc.Decode(local_buffer, 0, out function, out msg_length, ep);

                        if (HEADER_LENGTH == -1)
                        {
                            Trace.WriteLine("Unknow BVLC Header");
                            return;
                        }

                        // response to BVLC_REGISTER_FOREIGN_DEVICE (could be BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK ... but we are not a BBMD, don't care)
                        if (function == BacnetBvlcFunctions.BVLC_RESULT)
                        {
                            Trace.WriteLine("Receive Register as Foreign Device Response");
                        }

                        // a BVLC_FORWARDED_NPDU frame by a BBMD, change the remote_address to the original one (stored in the BVLC header) 
                        // we don't care about the BBMD address
                        if (function == BacnetBvlcFunctions.BVLC_FORWARDED_NPDU)
                        {
                            long ip = ((long)local_buffer[7] << 24) + ((long)local_buffer[6] << 16) + ((long)local_buffer[5] << 8) + (long)local_buffer[4];
                            int port = (local_buffer[8] << 8) + local_buffer[9];    // 0xbac0 maybe
                            ep = new Net.IPEndPoint(ip, port);

                            Convert((System.Net.IPEndPoint)ep, out remote_address);

                        }

                        if ((function == BacnetBvlcFunctions.BVLC_ORIGINAL_UNICAST_NPDU) || (function == BacnetBvlcFunctions.BVLC_ORIGINAL_BROADCAST_NPDU) || (function == BacnetBvlcFunctions.BVLC_FORWARDED_NPDU))
                            //send to upper layers
                            if ((MessageRecieved != null) && (rx > HEADER_LENGTH)) MessageRecieved(this, local_buffer, HEADER_LENGTH, rx - HEADER_LENGTH, remote_address);
                    }
                }
                catch (Exception ex)
                {
                    Trace.TraceError("Exception in udp recieve: " + ex.Message);
                }
                finally
                {
                    //restart data receive
                    //conn.BeginReceive(OnReceiveData, conn);
                }
            }
            catch (Exception ex)
            {
                //restart data receive
                if (conn.Client != null)
                {
                    Trace.TraceError("Exception in Ip OnRecieveData: " + ex.Message);
                    conn.BeginReceive(OnReceiveData, conn);
                }
            }
        }

        public bool SendRegisterAsForeignDevice(System.Net.IPEndPoint BBMD, short TTL)
        {
            if (BBMD.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
            {
                bvlc.SendRegisterAsForeignDevice(BBMD, TTL);
                return true;
            }
            return false;
        }
        public bool SendRemoteWhois(byte[] buffer, System.Net.IPEndPoint BBMD, int msg_length)
        {
            if (BBMD.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
            {
                bvlc.SendRemoteWhois(buffer, BBMD, msg_length);
                return true;
            }
            return false;
        }

        public bool WaitForAllTransmits(int timeout)
        {
            //we got no sending queue in udp, so just return true
            return true;
        }

        public static string ConvertToHex(byte[] buffer, int length)
        {
            string ret = "";

            for (int i = 0; i < length; i++)
                ret += buffer[i].ToString("X2");

            return ret;
        }

        public int Send(byte[] buffer, int data_length, System.Net.IPEndPoint ep)
        {

            // return m_exclusive_conn.Send(buffer, data_length, ep);
            System.Threading.ThreadPool.QueueUserWorkItem((o) => { try { m_exclusive_conn.Send(buffer, data_length, ep); } catch { } }, null);
            return data_length;

        }

        public int Send(byte[] buffer, int offset, int data_length, BacnetAddress address, bool wait_for_transmission, int timeout)
        {
            if (m_exclusive_conn == null) return 0;

            //add header
            int full_length = data_length + HeaderLength;
            bvlc.Encode(buffer, offset - BVLC.BVLC_HEADER_LENGTH, address.net == 0xFFFF ? BacnetBvlcFunctions.BVLC_ORIGINAL_BROADCAST_NPDU : BacnetBvlcFunctions.BVLC_ORIGINAL_UNICAST_NPDU, full_length);

            //create end point
            System.Net.IPEndPoint ep;
            Convert(address, out ep);

            try
            {
                //send
                return m_exclusive_conn.Send(buffer, full_length, ep);    //broadcasts are transported from our local unicast socket also
            }
            catch
            {
                return 0;
            }
        }

        public static void Convert(System.Net.IPEndPoint ep, out BacnetAddress addr)
        {
            byte[] tmp1 = ep.Address.GetAddressBytes();
            byte[] tmp2 = BitConverter.GetBytes((ushort)ep.Port);
            Array.Reverse(tmp2);
            Array.Resize<byte>(ref tmp1, tmp1.Length + tmp2.Length);
            Array.Copy(tmp2, 0, tmp1, tmp1.Length - tmp2.Length, tmp2.Length);
            addr = new BacnetAddress(BacnetAddressTypes.IP, 0, tmp1);
        }

        public static void Convert(BacnetAddress addr, out System.Net.IPEndPoint ep)
        {
            long ip_address = BitConverter.ToUInt32(addr.adr, 0);
            ushort port = (ushort)((addr.adr[4] << 8) | (addr.adr[5] << 0));
            ep = new System.Net.IPEndPoint(ip_address, (int)port);
        }


        // Get the IPAddress only if one is present
        // this could be usefull to find the broadcast address even if the socket is open on the default interface
        // removes somes virtual interfaces (certainly not all)
        private Net.NetworkInformation.UnicastIPAddressInformation GettAddress_DefaultInterface()
        {
            Net.NetworkInformation.UnicastIPAddressInformation UniIP = null;
            int NbAdd = 0;

            foreach (System.Net.NetworkInformation.NetworkInterface adapter in System.Net.NetworkInformation.NetworkInterface.GetAllNetworkInterfaces())
                if (adapter.OperationalStatus == System.Net.NetworkInformation.OperationalStatus.Up)
                    if (adapter.NetworkInterfaceType != System.Net.NetworkInformation.NetworkInterfaceType.Loopback)
                        if (!(adapter.Description.Contains("VirtualBox") || adapter.Description.Contains("VMware")))
                            foreach (System.Net.NetworkInformation.UnicastIPAddressInformation ip in adapter.GetIPProperties().UnicastAddresses)
                            {
                                if (ip.Address.AddressFamily == Net.Sockets.AddressFamily.InterNetwork)
                                {
                                    UniIP = ip;
                                    NbAdd++;
                                }
                            }
            if (NbAdd == 1)
                return UniIP;
            else
                return null;

        }
        // A lot of problems on Mono (Raspberry) to get the correct broadcast @
        // so this method is overridable (this allows the implementation of operating system specific code)
        // Marc solution http://stackoverflow.com/questions/8119414/how-to-query-the-subnet-masks-using-mono-on-linux for instance
        //
        protected virtual BacnetAddress _GetBroadcastAddress()
        {
            // general broadcast by default if nothing better is found
            System.Net.IPEndPoint ep = new Net.IPEndPoint(System.Net.IPAddress.Parse("255.255.255.255"), m_port);

            Net.NetworkInformation.UnicastIPAddressInformation ipAddr = null;

            if (LocalEndPoint.Address.ToString() == "0.0.0.0")
                ipAddr = GettAddress_DefaultInterface();
            else
            {
                // restricted local broadcast (directed ... routable)
                foreach (System.Net.NetworkInformation.NetworkInterface adapter in System.Net.NetworkInformation.NetworkInterface.GetAllNetworkInterfaces())
                    foreach (System.Net.NetworkInformation.UnicastIPAddressInformation ip in adapter.GetIPProperties().UnicastAddresses)
                        if (LocalEndPoint.Address.Equals(ip.Address))
                        {
                            ipAddr = ip;
                            break;
                        }
            }

            if (ipAddr != null)
            {
                try
                {
                    string[] strCurrentIP = ipAddr.Address.ToString().Split('.');
                    string[] strIPNetMask = ipAddr.IPv4Mask.ToString().Split('.');
                    StringBuilder BroadcastStr = new StringBuilder();
                    for (int i = 0; i < 4; i++)
                    {
                        BroadcastStr.Append(((byte)(int.Parse(strCurrentIP[i]) | ~int.Parse(strIPNetMask[i]))).ToString());
                        if (i != 3) BroadcastStr.Append('.');
                    }
                    ep = new Net.IPEndPoint(System.Net.IPAddress.Parse(BroadcastStr.ToString()), m_port);
                }
                catch { }  //On mono IPv4Mask feature not implemented
            }

            BacnetAddress broadcast;
            Convert(ep, out broadcast);
            broadcast.net = 0xFFFF;
            return broadcast;
        }


        BacnetAddress BroadcastAddress = null;
        public BacnetAddress GetBroadcastAddress()
        {
            if (BroadcastAddress == null) BroadcastAddress = _GetBroadcastAddress();
            return BroadcastAddress;
        }

        // Give 0.0.0.0:xxxx if the socket is open with System.Net.IPAddress.Any
        // Today only used by _GetBroadcastAddress method & the bvlc layer class in BBMD mode
        // Some more complex solutions could avoid this, that's why this property is virtual
        public virtual System.Net.IPEndPoint LocalEndPoint
        {
            get
            {
                return (System.Net.IPEndPoint)m_exclusive_conn.Client.LocalEndPoint;
            }
        }

        public void Dispose()
        {
            try
            {
                m_exclusive_conn.Close();
                m_exclusive_conn = null;
                m_shared_conn.Close(); // maybe an exception if null
                m_shared_conn = null;
            }
            catch { }
        }
    }

    public enum BacnetBvlcFunctions : byte
    {
        BVLC_RESULT = 0,
        BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE = 1,
        BVLC_READ_BROADCAST_DIST_TABLE = 2,
        BVLC_READ_BROADCAST_DIST_TABLE_ACK = 3,
        BVLC_FORWARDED_NPDU = 4,
        BVLC_REGISTER_FOREIGN_DEVICE = 5,
        BVLC_READ_FOREIGN_DEVICE_TABLE = 6,
        BVLC_READ_FOREIGN_DEVICE_TABLE_ACK = 7,
        BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY = 8,
        BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK = 9,
        BVLC_ORIGINAL_UNICAST_NPDU = 10,
        BVLC_ORIGINAL_BROADCAST_NPDU = 11,
        MAX_BVLC_FUNCTION = 12
    };

    public enum BacnetBvlcResults : ushort
    {
        BVLC_RESULT_SUCCESSFUL_COMPLETION = 0x0000,
        BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK = 0x0010,
        BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK = 0x0020,
        BVLC_RESULT_REGISTER_FOREIGN_DEVICE_NAK = 0X0030,
        BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK = 0x0040,
        BVLC_RESULT_DELETE_FOREIGN_DEVICE_TABLE_ENTRY_NAK = 0x0050,
        BVLC_RESULT_DISTRIBUTE_BROADCAST_TO_NETWORK_NAK = 0x0060
    };

    // Special thanks to VTS tool (BBMD services not activated but programmed !) and Steve Karg stack
    public class BVLC
    {
        public delegate void BVLCMessageReceiveHandler(System.Net.IPEndPoint sender, BacnetBvlcFunctions function, BacnetBvlcResults result, object data);
        public event BVLCMessageReceiveHandler MessageReceived;

        BacnetIpUdpProtocolTransport MyBBMDTransport;
        String BroadcastAdd;

        bool BBMD_FD_ServiceActivated = false;

        public const byte BVLL_TYPE_BACNET_IP = 0x81;
        public const byte BVLC_HEADER_LENGTH = 4;
        public const BacnetMaxAdpu BVLC_MAX_APDU = BacnetMaxAdpu.MAX_APDU1476;

        // Two lists for optional BBMD activity
        List<KeyValuePair<System.Net.IPEndPoint, DateTime>> ForeignDevices = new List<KeyValuePair<System.Net.IPEndPoint, DateTime>>();
        List<KeyValuePair<System.Net.IPEndPoint, System.Net.IPAddress>> BBMDs = new List<KeyValuePair<System.Net.IPEndPoint, System.Net.IPAddress>>();

        // Contains the rules to accept FRD based on the IP adress
        // If empty it's equal to *.*.*.*, everyone allows
        List<Regex> AutorizedFDR = new List<Regex>();

        public BVLC(BacnetIpUdpProtocolTransport Transport)
        {
            MyBBMDTransport = Transport;
            BroadcastAdd = MyBBMDTransport.GetBroadcastAddress().ToString().Split(':')[0];
        }

        public string FDList()
        {
            StringBuilder sb = new StringBuilder();
            lock (ForeignDevices)
            {
                // remove oldest Device entries (Time expiration > TTL + 30s delay)
                ForeignDevices.Remove(ForeignDevices.Find(item => DateTime.Now > item.Value));

                foreach (KeyValuePair<System.Net.IPEndPoint, DateTime> client in ForeignDevices)
                {
                    sb.Append(client.Key.Address);
                    sb.Append(":");
                    sb.Append(client.Key.Port);
                    sb.Append(";");
                }
            }
            return sb.ToString();
        }

        public void AddFDRAutorisationRule(Regex IpRule)
        {
            AutorizedFDR.Add(IpRule);
        }

        // Used to initiate the BBMD & FD behaviour, if BBMD is null it start the FD activity only
        public void AddBBMDPeer(Net.IPEndPoint BBMD, Net.IPAddress Mask)
        {
            BBMD_FD_ServiceActivated = true;

            if (BBMD != null)
                lock (BBMDs)
                    BBMDs.Add(new KeyValuePair<System.Net.IPEndPoint, System.Net.IPAddress>(BBMD, Mask));
        }

        // Add a FD to the table or renew it
        private void RegisterForeignDevice(System.Net.IPEndPoint sender, int TTL)
        {
            lock (ForeignDevices)
            {
                // remove it, if any
                ForeignDevices.Remove(ForeignDevices.Find(item => item.Key.Equals(sender)));
                // TTL + 30s grace period
                DateTime Expiration = DateTime.Now.AddSeconds(TTL + 30);
                // add it
                if (AutorizedFDR.Count == 0) // No rules, accept all
                {
                    ForeignDevices.Add(new KeyValuePair<System.Net.IPEndPoint, DateTime>(sender, Expiration));
                    return;
                }
                else
                    foreach (Regex r in AutorizedFDR)
                    {
                        if (r.Match(sender.Address.ToString()).Success)
                        {
                            ForeignDevices.Add(new KeyValuePair<System.Net.IPEndPoint, DateTime>(sender, Expiration));
                            return;
                        }
                    }
                System.Diagnostics.Trace.TraceInformation("Rejected FDR registration, IP : " + sender.Address.ToString());
            }
        }

        // Send a Frame to each registered foreign devices, except the original sender
        private void SendToFDs(byte[] buffer, int msg_length, Net.IPEndPoint EPsender = null)
        {
            lock (ForeignDevices)
            {
                // remove oldest Device entries (Time expiration > TTL + 30s delay)
                ForeignDevices.Remove(ForeignDevices.Find(item => DateTime.Now > item.Value));
                // Send to all others, except the original sender
                foreach (KeyValuePair<System.Net.IPEndPoint, DateTime> client in ForeignDevices)
                {
                    if (!(client.Key.Equals(EPsender)))
                        MyBBMDTransport.Send(buffer, msg_length, client.Key);
                }
            }
        }

        private System.Net.IPEndPoint BBMDSentAdd(Net.IPEndPoint BBMD, Net.IPAddress Mask)
        {
            byte[] bm = Mask.GetAddressBytes();
            byte[] bip = BBMD.Address.GetAddressBytes();

            /* annotation in Steve Karg bacnet stack :
         
            The B/IP address to which the Forwarded-NPDU message is
            sent is formed by inverting the broadcast distribution
            mask in the BDT entry and logically ORing it with the
            BBMD address of the same entry. This process
            produces either the directed broadcast address of the remote
            subnet or the unicast address of the BBMD on that subnet
            depending on the contents of the broadcast distribution
            mask. 
             
            remark from me :
               for instance remote BBMD 192.168.0.1 - mask 255.255.255.255
                    messages are forward directly to 192.168.0.1
               remote BBMD 192.168.0.1 - mask 255.255.255.0
                    messages are forward to 192.168.0.255, ie certainly the local broadcast
                    address, but these datagrams are generaly destroy by the final IP router
             */

            for (int i = 0; i < bm.Length; i++)
                bip[i] = (byte)(bip[i] | (~bm[i]));

            return new System.Net.IPEndPoint(new System.Net.IPAddress(bip), BBMD.Port);
        }

        // Send a Frame to each registered BBMD except the original sender
        private void SendToBBMDs(byte[] buffer, int msg_length)
        {
            lock (BBMDs)
            {
                foreach (KeyValuePair<System.Net.IPEndPoint, System.Net.IPAddress> e in BBMDs)
                {
                    System.Net.IPEndPoint endpoint = BBMDSentAdd(e.Key, e.Value);
                    MyBBMDTransport.Send(buffer, msg_length, endpoint);
                }
            }
        }

        private void First4BytesHeaderEncode(byte[] b, BacnetBvlcFunctions function, int msg_length)
        {
            b[0] = BVLL_TYPE_BACNET_IP;
            b[1] = (byte)function;
            b[2] = (byte)(((msg_length) & 0xFF00) >> 8);
            b[3] = (byte)(((msg_length) & 0x00FF) >> 0);
        }

        private void Forward_NPDU(byte[] buffer, int msg_length, bool ToGlobalBroadcast, Net.IPEndPoint EPsender)
        {
            // Forms the forwarded NPDU from the original one, and send it to all
            // orignal     - 4 bytes BVLC -  NPDU  - APDU
            // change to   -  10 bytes BVLC  -  NPDU  - APDU

            // copy, 6 bytes shifted
            byte[] b = new byte[msg_length + 6];    // normaly only 'small' frames are present here, so no need to check if it's to big for Udp
            Array.Copy(buffer, 0, b, 6, msg_length);

            // 10 bytes for the BVLC Header, with the embedded 6 bytes IP:Port of the original sender
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_FORWARDED_NPDU, msg_length + 6);
            BacnetAddress BacSender;
            BacnetIpUdpProtocolTransport.Convert(EPsender, out BacSender); // to embbed in the forward BVLC header
            for (int i = 0; i < BacSender.adr.Length; i++)
                b[4 + i] = BacSender.adr[i];

            // To BBMD
            SendToBBMDs(b, msg_length + 6);
            // To FD, except the sender
            SendToFDs(b, msg_length + 6, EPsender);
            // Broadcast if required
            if (ToGlobalBroadcast == true)
                MyBBMDTransport.Send(b, msg_length + 6, new Net.IPEndPoint(Net.IPAddress.Parse(BroadcastAdd), MyBBMDTransport.SharedPort));
        }

        // Send ack or nack
        private void SendResult(System.Net.IPEndPoint sender, BacnetBvlcResults ResultCode)
        {
            byte[] b = new byte[6];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_RESULT, 6);
            b[4] = (byte)(((ushort)ResultCode & 0xFF00) >> 8);
            b[5] = (byte)((ushort)ResultCode & 0xFF);

            MyBBMDTransport.Send(b, 6, sender);
        }

        public void SendRegisterAsForeignDevice(System.Net.IPEndPoint BBMD, short TTL)
        {
            byte[] b = new byte[6];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_REGISTER_FOREIGN_DEVICE, 6);
            b[4] = (byte)((TTL & 0xFF00) >> 8);
            b[5] = (byte)(TTL & 0xFF);
            MyBBMDTransport.Send(b, 6, BBMD);
        }

        public void SendReadBroadCastTable(System.Net.IPEndPoint BBMD)
        {
            byte[] b = new byte[4];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_READ_BROADCAST_DIST_TABLE, 4);
            MyBBMDTransport.Send(b, 4, BBMD);
        }

        public void SendReadFDRTable(System.Net.IPEndPoint BBMD)
        {
            byte[] b = new byte[4];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_READ_FOREIGN_DEVICE_TABLE, 4);
            MyBBMDTransport.Send(b, 4, BBMD);
        }

        public void SendWriteBroadCastTable(System.Net.IPEndPoint BBMD, List<Tuple<IPEndPoint, IPAddress>> Entries)
        {
            byte[] b = new byte[4 + 10 * Entries.Count];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE, 4 + 10 * Entries.Count);

            for (int i = 0; i < Entries.Count; i++)
            {
                Array.Copy(Entries[i].Item1.Address.GetAddressBytes(), 0, b, 4 + i * 10, 4);
                b[8 + i * 10] = (byte)(Entries[i].Item1.Port >> 8);
                b[9 + i * 10] = (byte)(Entries[i].Item1.Port & 0xFF);
                Array.Copy(Entries[i].Item2.GetAddressBytes(), 0, b, 10 + i * 10, 4);
            }

            MyBBMDTransport.Send(b, 4 + 10 * Entries.Count, BBMD);
        }

        public void SendDeleteForeignDeviceEntry(System.Net.IPEndPoint BBMD, IPEndPoint Fdevice)
        {
            byte[] b = new byte[4 + 6];
            First4BytesHeaderEncode(b, BacnetBvlcFunctions.BVLC_READ_FOREIGN_DEVICE_TABLE, 4 + 6);
            Array.Copy(Fdevice.Address.GetAddressBytes(), 0, b, 4, 4);
            b[8] = (byte)(Fdevice.Port >> 8);
            b[9] = (byte)(Fdevice.Port & 0xFF);
            MyBBMDTransport.Send(b, 4 + 6, BBMD);
        }

        public void SendRemoteWhois(byte[] buffer, System.Net.IPEndPoint BBMD, int msg_length)
        {
            Encode(buffer, 0, BacnetBvlcFunctions.BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK, msg_length);
            MyBBMDTransport.Send(buffer, msg_length, BBMD);

        }
        // Encode is called by internal services if the BBMD is also an active device
        public int Encode(byte[] buffer, int offset, BacnetBvlcFunctions function, int msg_length)
        {
            // offset always 0, we are the first after udp

            // do the job
            First4BytesHeaderEncode(buffer, function, msg_length);

            // optional BBMD service
            if ((BBMD_FD_ServiceActivated == true) && (function == BacnetBvlcFunctions.BVLC_ORIGINAL_BROADCAST_NPDU))
            {
                Net.IPEndPoint me = MyBBMDTransport.LocalEndPoint;
                // just sometime working, enable to get the local ep, always 0.0.0.0 if the socket is open with
                // System.Net.IPAddress.Any
                // So in this case don't send a bad message
                if ((me.Address.ToString() != "0.0.0.0"))
                    Forward_NPDU(buffer, msg_length, false, me);   // send to all BBMDs and FDs
            }
            return 4; // ready to send
        }

        // Decode is called each time an Udp Frame is received
        public int Decode(byte[] buffer, int offset, out BacnetBvlcFunctions function, out int msg_length, System.Net.IPEndPoint sender)
        {

            // offset always 0, we are the first after udp
            // and a previous test by the caller guaranteed at least 4 bytes into the buffer

            function = (BacnetBvlcFunctions)buffer[1];
            msg_length = (buffer[2] << 8) | (buffer[3] << 0);
            if ((buffer[0] != BVLL_TYPE_BACNET_IP) || (buffer.Length != msg_length)) return -1;

            switch (function)
            {
                case BacnetBvlcFunctions.BVLC_RESULT:
                    int ResultCode = (buffer[4] << 8) + buffer[5];
                    if (MessageReceived != null)
                        MessageReceived(sender, function, (BacnetBvlcResults)ResultCode, null);
                    return 0;   // not for the upper layers

                case BacnetBvlcFunctions.BVLC_ORIGINAL_UNICAST_NPDU:
                    return 4;   // only for the upper layers

                case BacnetBvlcFunctions.BVLC_ORIGINAL_BROADCAST_NPDU: // Normaly received in an IP local or global broadcast packet
                    // Send to FDs & BBMDs, not broadcast or it will be made twice !
                    if (BBMD_FD_ServiceActivated == true)
                        Forward_NPDU(buffer, msg_length, false, sender);
                    return 4;   // also for the upper layers

                case BacnetBvlcFunctions.BVLC_FORWARDED_NPDU:   // Sent only by a BBMD, broadcast on it network, or broadcast demand by one of it's FDs
                    if ((BBMD_FD_ServiceActivated == true) && (msg_length >= 10))
                    {
                        bool ret;
                        lock (BBMDs)
                            ret = BBMDs.Exists(items => items.Key.Address.Equals(sender.Address));    // verify sender (@ not Port!) presence in the table

                        if (ret)    // message from a know BBMD address, sent to all FDs and broadcast
                        {
                            SendToFDs(buffer, msg_length);  // send without modification

                            // Assume all BVLC_FORWARDED_NPDU are directly sent to me in the 
                            // unicast mode and not by the way of the local broadcast address
                            // ie my mask must be 255.255.255.255 in the others BBMD tables
                            // If not, it's not really a big problem, devices on the local net will 
                            // receive two times the message (after all it's just WhoIs, Iam, ...)
                            MyBBMDTransport.Send(buffer, msg_length, new Net.IPEndPoint(Net.IPAddress.Parse(BroadcastAdd), MyBBMDTransport.SharedPort));
                        }
                    }

                    return 10;  // also for the upper layers

                case BacnetBvlcFunctions.BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK:  // Sent by a Foreign Device, not a BBMD
                    if (BBMD_FD_ServiceActivated == true)
                    {
                        // Send to FDs except the sender, BBMDs and broadcast
                        lock (ForeignDevices)
                        {
                            if (ForeignDevices.Exists(item => item.Key.Equals(sender))) // verify previous registration
                                Forward_NPDU(buffer, msg_length, true, sender);
                            else
                                SendResult(sender, BacnetBvlcResults.BVLC_RESULT_DISTRIBUTE_BROADCAST_TO_NETWORK_NAK);
                        }
                    }
                    return 0;   // anot for the upper layers

                case BacnetBvlcFunctions.BVLC_REGISTER_FOREIGN_DEVICE:
                    if ((BBMD_FD_ServiceActivated == true) && (msg_length == 6))
                    {
                        int TTL = (buffer[4] << 8) + buffer[5]; // unit is second
                        RegisterForeignDevice(sender, TTL);
                        SendResult(sender, BacnetBvlcResults.BVLC_RESULT_SUCCESSFUL_COMPLETION);  // ack
                    }
                    return 0;  // not for the upper layers

                // We don't care about Read/Write operation in the BBMD/FDR tables (who realy use it ?)
                case BacnetBvlcFunctions.BVLC_READ_FOREIGN_DEVICE_TABLE:
                    SendResult(sender, BacnetBvlcResults.BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK);
                    return 0;
                case BacnetBvlcFunctions.BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY:
                    SendResult(sender, BacnetBvlcResults.BVLC_RESULT_DELETE_FOREIGN_DEVICE_TABLE_ENTRY_NAK);
                    return 0;
                case BacnetBvlcFunctions.BVLC_READ_BROADCAST_DIST_TABLE:
                    SendResult(sender, BacnetBvlcResults.BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK);
                    return 0;

                case BacnetBvlcFunctions.BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE:
                case BacnetBvlcFunctions.BVLC_READ_BROADCAST_DIST_TABLE_ACK:
                    {
                        int NbEntries = (msg_length - 4) / 10;
                        List<Tuple<IPEndPoint, IPAddress>> Entries = new List<Tuple<IPEndPoint, IPAddress>>();

                        for (int i = 0; i < NbEntries; i++)
                        {
                            long add = BitConverter.ToInt32(buffer, 4 + i * 10);

                            Array.Reverse(buffer, 8 + i * 10, 2);
                            ushort port = BitConverter.ToUInt16(buffer, 8 + i * 10);

                            // new IPAddress(long) with 255.255.255.255 (ie -1) not OK
                            byte[] Mask = new byte[4];
                            Array.Copy(buffer, 10 + i * 10, Mask, 0, 4);

                            Tuple<IPEndPoint, IPAddress> entry = new Tuple<IPEndPoint, IPAddress>(new IPEndPoint(new IPAddress(add), port), new IPAddress(Mask));
                            Entries.Add(entry);
                        }

                        if ((MessageReceived != null) && (function == BacnetBvlcFunctions.BVLC_READ_BROADCAST_DIST_TABLE_ACK))
                            MessageReceived(sender, function, BacnetBvlcResults.BVLC_RESULT_SUCCESSFUL_COMPLETION, Entries);

                        // Today we don't accept it
                        if (function == BacnetBvlcFunctions.BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE)
                            SendResult(sender, BacnetBvlcResults.BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK);

                        return 0;
                    }

                case BacnetBvlcFunctions.BVLC_READ_FOREIGN_DEVICE_TABLE_ACK:
                    {
                        int NbEntries = (msg_length - 4) / 10;
                        List<Tuple<IPEndPoint, ushort, ushort>> Entries = new List<Tuple<IPEndPoint, ushort, ushort>>();

                        for (int i = 0; i < NbEntries; i++)
                        {
                            long add = BitConverter.ToInt32(buffer, 4 + i * 10);

                            Array.Reverse(buffer, 8 + i * 10, 2);
                            ushort port = BitConverter.ToUInt16(buffer, 8 + i * 10);

                            Array.Reverse(buffer, 10 + i * 10, 2);
                            ushort TTL = BitConverter.ToUInt16(buffer, 10 + i * 10);
                            Array.Reverse(buffer, 12 + i * 10, 2);
                            ushort RemainTTL = BitConverter.ToUInt16(buffer, 12 + i * 10);

                            Tuple<IPEndPoint, ushort, ushort> entry = new Tuple<IPEndPoint, ushort, ushort>(new IPEndPoint(new IPAddress(add), port), TTL, RemainTTL);
                            Entries.Add(entry);
                        }

                        if (MessageReceived != null)
                            MessageReceived(sender, function, BacnetBvlcResults.BVLC_RESULT_SUCCESSFUL_COMPLETION, Entries);

                        return 0;
                    }
                // error encoding function or experimental one
                default:
                    return -1;
            }
        }
    }
}