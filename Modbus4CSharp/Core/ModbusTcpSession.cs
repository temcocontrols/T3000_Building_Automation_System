using System;
using Modbus.Core.Exceptions;
using System.Threading.Tasks;

namespace Modbus.Core
{
    public sealed class ModbusTcpSession : IModbusSession
    {
        private static readonly int PROTOCOL_ID = 0;

        private readonly IModbusProtocol _modbusProtocol;
        private int _slaveAddress;
        private ushort _transactionId;

        public SessionState State { get; private set; }

        public bool EnableCheck { get; set; } = true;

        public ModbusTcpSession(IModbusProtocol modbusProtocol, int slaveAddress)
        {
            _modbusProtocol = modbusProtocol;
            this._slaveAddress = slaveAddress;
            _transactionId = 0;

            State = slaveAddress != Constants.UndefinedSlaveAddress
                ? SessionState.Identified
                : SessionState.Unidentified;
        }

        internal ModbusTcpSession(IModbusProtocol modbusProtocol)
        {
            _modbusProtocol = modbusProtocol;
            this._slaveAddress = Constants.UndefinedSlaveAddress;
            _transactionId = 0;
            State = SessionState.Unidentified;
        }

        internal void SetSlaveAddress(int slaveAddress)
        {
            this._slaveAddress = slaveAddress;

            State = slaveAddress != Constants.UndefinedSlaveAddress
                ? SessionState.Identified
                : SessionState.Unidentified;
        }

        public Response<T> SendRequest<T>(int functionCode, object data) where T : struct
        {
            if (State == SessionState.Unidentified)
                throw new InvalidOperationException("Slave address must be defined");

            var builder = (TcpRequest.Builder) new TcpRequest.Builder()
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

            var tcpBuilder = (TcpRequest.Builder) builder;

            tcpBuilder
                .SetTransactionId(++_transactionId)
                .SetProtocolId(PROTOCOL_ID)
                .AutoComputeLength();

            var request = tcpBuilder.Build();

            var responseBytes =
                _modbusProtocol.SendForResult(request.RequestBytes, TcpResponse<T>.ComputeResponseBytesLength());

            if (responseBytes == null)
                return null;

            var response = (TcpResponse<T>) new TcpResponse<T>.Builder()
                .SetResponseBytes(responseBytes)
                .Build();

            if (EnableCheck)
                CheckResponse(request, response);

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

        private void CheckResponse<T>(Request request, TcpResponse<T> response) where T : struct
        {
            var tcpRequest = (TcpRequest) request;

            if (tcpRequest.TransactionId != response.TransactionId)
                throw new DataCorruptedException("Response transaction id does not match " + _transactionId);
            if (tcpRequest.ProtocolId != response.ProtocolId)
                throw new DataCorruptedException("Response protocol id does not match " + PROTOCOL_ID);
            if (response.ComputeMessageLength() != response.Length)
                throw new DataCorruptedException("Actual message bytes length does not match " + response.Length);
            if (request.SlaveAddress != response.SlaveAddress)
                throw new MismatchDataException("Response slave address mismatch " + request.SlaveAddress);
            if (request.FunctionCode != response.FunctionCode)
                throw new MismatchDataException("Response function code mismatch " + request.FunctionCode);
        }

        public void Dispose()
        {
            State = SessionState.Expired;
            _modbusProtocol?.Dispose();
        }
    }
}