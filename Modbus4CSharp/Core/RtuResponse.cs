using Modbus.Core.Converters;
using Modbus.Core.Exceptions;
using System;
using System.Runtime.InteropServices;

namespace Modbus.Core
{
    public class RtuResponse<T> : Response<T> where T : struct
    {
        public static int ComputeResponseBytesLength()
        {
            return 4 + Marshal.SizeOf(typeof(T));// slaveAddress + functionCode + data + crc16
        }

        private RtuResponse()
        {
        }

        public class Builder : BuilderBase
        {
            public override Response<T> Build()
            {
                if (responseBytes == null)
                    throw new InvalidOperationException("ResponseBytes must be initialized");

                if (responseBytes.Length < 4)
                    throw new MissingDataException("ResponseBytes must have at least 4 bytes");

                RtuResponse<T> response = new RtuResponse<T>
                {
                    ResponseBytes = (byte[])responseBytes.Clone(),
                    SlaveAddress = responseBytes[0],
                    FunctionCode = responseBytes[1]
                };

                var data = new byte[responseBytes.Length - 4];
                Array.Copy(responseBytes, 2, data, 0, data.Length);
                response.Data = BytesStructure.FromBytes<T>(data);

                return response;
            }
        }
    }
}