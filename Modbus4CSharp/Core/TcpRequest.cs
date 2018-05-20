using System;
using System.Runtime.InteropServices;

namespace Modbus.Core
{
    public sealed class TcpRequest : Request
    {
        public int TransactionId { get; private set; }
        public int ProtocolId { get; private set; }
        public int Length { get; private set; }

        private TcpRequest()
        {
        }

        public sealed class Builder : BuilderBase
        {
            private ushort _transactionId;
            private ushort _protocolId;
            private ushort _length;

            public Builder SetTransactionId(int transactionId)
            {
                this._transactionId = (ushort)transactionId;
                return this;
            }

            public Builder SetProtocolId(int protocolId)
            {
                this._protocolId = (ushort)protocolId;
                return this;
            }

            public Builder SetLength(int length)
            {
                this._length = (ushort)length;
                return this;
            }

            public Builder AutoComputeLength()
            {
                if (data == null)
                    throw new InvalidOperationException("Data must be initialized");
                this._length = (ushort)(8 + data.Length);
                return this;
            }

            public override Request Build()
            {
                if (data == null)
                    throw new InvalidOperationException("Data must be initialized");

                var message = new byte[8 + data.Length];

                message[0] = (byte)(_transactionId >> 8);
                message[1] = (byte)(_transactionId);

                message[2] = (byte)(_protocolId >> 8);
                message[3] = (byte)(_protocolId);

                message[4] = (byte)(_length >> 8);
                message[5] = (byte)(_length);

                message[6] = slaveAddress;

                message[7] = functionCode;

                Array.Copy(data, 0, message, 8, data.Length);

                return new TcpRequest()
                {
                    TransactionId = _transactionId,
                    ProtocolId = _protocolId,
                    Length = _length,
                    SlaveAddress = slaveAddress,
                    FunctionCode = functionCode,
                    Data = data,
                    RequestBytes = message
                };
            }
        }
    }
}