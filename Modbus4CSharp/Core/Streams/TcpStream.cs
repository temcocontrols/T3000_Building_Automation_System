using System.Net.Sockets;

namespace Modbus.Core.Streams
{
    public class TcpStream : ICommStream
    {
        private readonly TcpClient _tcpClient;
        private readonly NetworkStream _networkStream;

        public TcpClient TcpClient => _tcpClient;

        public int Available => _tcpClient.Available;

        public int ReadTimeout
        {
            get { return _networkStream.ReadTimeout; }
            set { _networkStream.ReadTimeout = value; }
        }

        public int WriteTimeout
        {
            get { return _networkStream.WriteTimeout; }
            set { _networkStream.WriteTimeout = value; }
        }

        public void Dispose()
        {
            _networkStream.Close();
            _tcpClient.Close();
        }

        public int Read(byte[] buffer, int offset, int length)
        {
            return _networkStream.Read(buffer, offset, length);
        }

        public void Write(byte[] buffer, int offset, int length)
        {
            while (_networkStream.DataAvailable)
                _networkStream.ReadByte();
            _networkStream.Write(buffer, offset, length);
            _networkStream.Flush();
        }

        public TcpStream(TcpClient tcpClient)
        {
            this._tcpClient = tcpClient;
            this._networkStream = tcpClient.GetStream();
            ReadTimeout = WriteTimeout = 5000;
        }

        public override string ToString()
        {
            return _tcpClient.ToString();
        }
    }
}