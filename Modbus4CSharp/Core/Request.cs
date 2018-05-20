using System;
using Modbus.Core.Converters;

namespace Modbus.Core
{
    public abstract class Request
    {
        public int SlaveAddress { get; protected set; }
        public int FunctionCode { get; protected set; }
        public byte[] Data { get; protected set; }

        public byte[] RequestBytes { get; protected set; }

        public abstract class BuilderBase
        {
            protected byte slaveAddress;
            protected byte functionCode;
            protected byte[] data;

            public byte SlaveAddress => slaveAddress;
            public byte FunctionCode => functionCode;

            public BuilderBase SetSlaveAddress(int slaveAddress)
            {
                this.slaveAddress = (byte)slaveAddress;
                return this;
            }

            public BuilderBase SetFunctionCode(int functionCode)
            {
                this.functionCode = (byte)functionCode;
                return this;
            }

            public BuilderBase SetInt16(ushort int16)
            {
                data = new byte[2];
                data[0] = (byte)(int16 >> 8);
                data[1] = (byte)(int16);
                return this;
            }

            public BuilderBase SetDataBytes(byte[] dataBytes)
            {
                this.data = dataBytes;
                return this;
            }

            public BuilderBase SetObject(object structObj)
            {
                data = BytesStructure.ToBytes(structObj);
                return this;
            }

            public BuilderBase AddInt16(ushort int16)
            {
                var offset = ExtendedData(2);
                data[offset + 0] = (byte)(int16 >> 8);
                data[offset + 1] = (byte)(int16);
                return this;
            }

            private int ExtendedData(int numberOfBytes)
            {
                if (data == null)
                {
                    data = new byte[numberOfBytes];
                    return 0;
                }
                else
                {
                    var previousSegment = data;
                    data = new byte[previousSegment.Length + numberOfBytes];
                    Array.Copy(previousSegment, 0, data, 0, previousSegment.Length);
                    return previousSegment.Length;
                }
            }

            public abstract Request Build();
        }
    }
}