using System;

namespace Modbus.Core
{
    public sealed class RtuRequest : Request
    {
        private RtuRequest()
        {
        }

        public sealed class Builder : BuilderBase
        {
            public override Request Build()
            {
                if (data == null)
                    throw new InvalidOperationException("Data must be initialized");

                var message = new byte[4 + data.Length];

                message[0] = slaveAddress;

                message[1] = functionCode;

                Array.Copy(data, 0, message, 2, data.Length);

                var crc16 = Checksum.ComputeCrc16(message, 0, message.Length - 2);
                Array.Copy(crc16, 0, message, message.Length - 2, crc16.Length);

                return new RtuRequest()
                {
                    SlaveAddress = slaveAddress,
                    FunctionCode = functionCode,
                    Data = data,
                    RequestBytes = message
                };
            }
        }
    }
}