using Modbus.Core.Streams;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

namespace Modbus.Core
{
    public sealed class ModbusProtocolImpl : IModbusProtocol
    {
        private readonly object _lock = new object();
        private readonly ICommStream _stream;
        private readonly Stopwatch _stopwatch = new Stopwatch();

        public ModbusProtocolImpl(ICommStream stream)
        {
            this._stream = stream;
        }

        public byte[] SendForResult(byte[] requestBytes, int responseBytesLength)
        {
            lock (_lock)
            {
                _stream.Write(requestBytes, 0, requestBytes.Length);
                var response = ReceiveResponseBytes(responseBytesLength);
                return response;
            }
        }

        public Task<byte[]> SendForResultAsync(byte[] requestBytes, int responseBytesLength)
        {
            return Task.Run(() => SendForResult(requestBytes, responseBytesLength));
        }

        private bool WaitUntilAvailable(int timeout)
        {
            _stopwatch.Restart();
            while ((_stream.Available < 1) && (timeout > _stopwatch.ElapsedMilliseconds))
            {
                Thread.Sleep(50);
            }
            _stopwatch.Stop();
            return _stream.Available > 0;
        }

        private byte[] ReceiveResponseBytes(int responseBytesLength)
        {
            int timeout = _stream.ReadTimeout;

            if (WaitUntilAvailable(timeout))
            {
                _stopwatch.Restart();
                int totalReceivedBytes = 0;
                var response = new byte[responseBytesLength];

                do
                {
                    int receivedBytes = _stream.Read(response, totalReceivedBytes, response.Length - totalReceivedBytes);
                    totalReceivedBytes += receivedBytes;
                    if (totalReceivedBytes >= response.Length)
                        break;
                    while ((_stream.Available < 1) && (timeout > _stopwatch.ElapsedMilliseconds))
                    {
                        Thread.Sleep(1);
                    }
                } while (timeout > _stopwatch.ElapsedMilliseconds);

                _stopwatch.Stop();

                return response;
            }
            return null;
        }

        public void Dispose()
        {
            _stream?.Dispose();
        }
    }
}