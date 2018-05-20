using System.Runtime.InteropServices;

namespace Modbus.Core
{
    public abstract class Response<T> where T : struct
    {
        public int SlaveAddress { get; protected set; }
        public int FunctionCode { get; protected set; }
        public T Data { get; protected set; }
        public byte[] ResponseBytes { get; protected set; }

        public abstract class BuilderBase
        {
            protected byte[] responseBytes;

            public BuilderBase SetResponseBytes(byte[] responseBytes)
            {
                this.responseBytes = responseBytes;
                return this;
            }

            public abstract Response<T> Build();
        }
    }
}