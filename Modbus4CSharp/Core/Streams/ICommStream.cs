using System;

namespace Modbus.Core.Streams
{
    public interface ICommStream : IDisposable
    {
        int Available { get; }

        int ReadTimeout { get; set; }

        int WriteTimeout { get; set; }

        void Write(byte[] buffer, int offset, int length);

        int Read(byte[] buffer, int offset, int length);
    }
}