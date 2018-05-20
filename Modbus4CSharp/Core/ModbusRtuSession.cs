using System;
using Modbus.Core.Exceptions;
using System.Threading.Tasks;

namespace Modbus.Core
{
    public sealed class ModbusRtuSession : IModbusSession
    {
        private readonly IModbusProtocol _modbusProtocol;
        private readonly int _slaveAddress;

        public SessionState State { get; private set; }

        public bool EnableCheck { get; set; } = true;

        public ModbusRtuSession(IModbusProtocol modbusProtocol, int slaveAddress)
        {
            _modbusProtocol = modbusProtocol;
            this._slaveAddress = slaveAddress;

            State = slaveAddress != Constants.UndefinedSlaveAddress
                ? SessionState.Identified
                : SessionState.Unidentified;
        }

        public Response<T> SendRequest<T>(int functionCode, object data) where T : struct
        {
            if (State == SessionState.Unidentified)
                throw new InvalidOperationException("Slave address must be defined");

            var builder = new RtuRequest.Builder()
                .SetFunctionCode(functionCode)
                .SetObject(data);

            return SendRequest<T>(builder);
        }

        public Response<T> SendRequest<T>(Request.BuilderBase builder) where T : struct
        {
            if (State == SessionState.Expired)
                throw new InvalidOperationException("This session already expired");

            if (State == SessionState.Identified)
                builder.SetSlaveAddress(_slaveAddress);

            var request = builder.Build();

            var responseBytes =
                _modbusProtocol.SendForResult(request.RequestBytes, RtuResponse<T>.ComputeResponseBytesLength());

            if (responseBytes == null)
                return null;

            var response = new RtuResponse<T>.Builder()
                .SetResponseBytes(responseBytes)
                .Build();

            if (EnableCheck)
                CheckResponse(request, responseBytes, response);

            return response;
        }

        public Task<Response<T>> SendRequestAsync<T>(int functionCode, object data) where T : struct
        {
            return Task.Run(() => SendRequest<T>(functionCode, data));
        }

        public Task<Response<T>> SendRequestAsync<T>(Request.BuilderBase builder) where T : struct
        {
            return Task.Run(() => SendRequest<T>(builder));
        }

        public static void CheckResponse<T>(int slaveAddress, int functionCode, byte[] responseBytes, Response<T> response) where T : struct
        {
            Checksum<T>(responseBytes);

            if (slaveAddress != response.SlaveAddress)
                throw new MismatchDataException("Response slave address mismatch with " + slaveAddress);
            if (functionCode != response.FunctionCode)
                throw new MismatchDataException("Response function code mismatch with " + functionCode);
        }

        private static void CheckResponse<T>(Request request, byte[] responseBytes, Response<T> response)
            where T : struct
        {
            CheckResponse(request.SlaveAddress, request.FunctionCode, responseBytes, response);
        }

        private static void Checksum<T>(byte[] responseBytes) where T : struct
        {
            var crc16 = Core.Checksum.ComputeCrc16(responseBytes, 0, responseBytes.Length - 2);
            if (crc16[0] != responseBytes[responseBytes.Length - 2] || crc16[1] != responseBytes[responseBytes.Length - 1])
                throw new DataCorruptedException("Checksum fail");
        }

        public void Dispose()
        {
            State = SessionState.Expired;
            _modbusProtocol?.Dispose();
        }
    }
}