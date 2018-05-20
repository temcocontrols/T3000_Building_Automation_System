using Modbus.Core.Converters;
using Modbus.Core.Exceptions;
using System;
using System.Runtime.InteropServices;

namespace Modbus.Core
{
    public class TcpResponse<T> : Response<T> where T : struct
    {
        public int TransactionId { get; private set; }
        public int ProtocolId { get; private set; }
        public int Length { get; private set; }

        public static int ComputeResponseBytesLength()
        {
            return 8 + Marshal.SizeOf(typeof(T));// transactionId + protocolId + length slaveAddress + functionCode + data
        }

        public int ComputeMessageLength()
        {
            return ResponseBytes.Length - 6;
        }

        private TcpResponse()
        {
        }

        public class Builder : BuilderBase
        {
            public override Response<T> Build()
            {
                if (responseBytes == null)
                    throw new InvalidOperationException("ResponseBytes must be initialized");

                if (responseBytes.Length < 8)
                    throw new MissingDataException("ResponseBytes must have at least 8 bytes");

                var response = new TcpResponse<T>
                {
                    ResponseBytes = (byte[])responseBytes.Clone(),
                    TransactionId = BytesNumeric.ToUInt16(responseBytes, 0),
                    ProtocolId = BytesNumeric.ToUInt16(responseBytes, 2),
                    Length = BytesNumeric.ToUInt16(responseBytes, 4),
                    SlaveAddress = responseBytes[6],
                    FunctionCode = responseBytes[7]
                };

                var data = new byte[responseBytes.Length - 8];
                Array.Copy(responseBytes, 8, data, 0, data.Length);
                response.Data = BytesStructure.FromBytes<T>(data);

                return response;
            }
        }
    }
}