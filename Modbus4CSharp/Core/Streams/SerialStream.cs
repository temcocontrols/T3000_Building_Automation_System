using System.IO.Ports;

namespace Modbus.Core.Streams
{
    public class SerialStream : ICommStream
    {
        private readonly SerialPort _serialPort;

        public int Available => _serialPort.BytesToRead;

        public int ReadTimeout
        {
            get { return _serialPort.ReadTimeout; }

            set { _serialPort.ReadTimeout = value; }
        }

        public int WriteTimeout
        {
            get { return _serialPort.WriteTimeout; }
            set { _serialPort.WriteTimeout = value; }
        }

        public void Dispose()
        {
            _serialPort.Close();
        }

        public int Read(byte[] buffer, int offset, int length)
        {
            return _serialPort.Read(buffer, offset, length);
        }

        public void Write(byte[] buffer, int offset, int length)
        {
            lock (this)
            {
                _serialPort.DiscardInBuffer();
                _serialPort.Write(buffer, offset, length);
                _serialPort.BaseStream.Flush();
            }
        }

        public SerialStream(SerialPort serialPort)
        {
            this._serialPort = serialPort;
            ReadTimeout = WriteTimeout = 5000;
        }

        public override string ToString()
        {
            return _serialPort.ToString();
        }
    }
}