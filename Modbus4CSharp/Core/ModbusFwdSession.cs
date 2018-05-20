using System;
using System.Threading.Tasks;

namespace Modbus.Core
{
    /// <summary>
    /// Master ---request1-> parent slave ---request2--> child slave.
    /// Master +--response-- parent slave +--response--- child slave.
    ///<para/>
    /// We have two requests are request1(request to parent slave) and
    /// request2(which we want to request to child slave). Request1 will carry
    /// request2 bytes in data segment.
    /// <para/>
    /// Parent slave should forward whole data segment
    /// to child slave, after child slave responsed, parent slave should forward whole
    /// data response bytes that received from child slave to master via response's data segment.
    /// </summary>
    public sealed class ModbusFwdSession : IModbusSession
    {
        private readonly IModbusSession _modbusSession;
        private readonly Settings _settings;

        public Settings CurrentSettings => _settings;

        public SessionState State { get; private set; }

        public bool EnableCheck
        {
            get { return _modbusSession.EnableCheck; }
            set { _modbusSession.EnableCheck = value; }
        }

        public ModbusFwdSession(IModbusProtocol modbusProtocol, Settings settings)
        {
            _modbusSession = new ModbusTcpSession(modbusProtocol, settings.ParentSlaveAddress);
            _settings = settings;
            ApplySettings();
        }

        public void ApplySettings()
        {
            State = _settings.ChildSlaveAddress != Constants.UndefinedSlaveAddress
                ? SessionState.Identified
                : SessionState.Unidentified;
        }

        public Response<T> SendRequest<T>(int functionCode, object data) where T : struct
        {
            if (_settings.ParentSlaveAddress == Constants.UndefinedSlaveAddress)
                throw new InvalidOperationException("ParentSlaveAddress must be defined");

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
                builder.SetSlaveAddress(_settings.ChildSlaveAddress);

            var requestBytes = builder.Build().RequestBytes;

            var parentBuilder = new TcpRequest.Builder()
                .SetFunctionCode(_settings.ParentFunctionCode)
                .SetDataBytes(requestBytes);

            var response = _modbusSession.SendRequest<T>(parentBuilder);

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

        public void Dispose()
        {
            State = SessionState.Expired;
            _modbusSession?.Dispose();
        }

        public class Settings
        {
            public int ParentSlaveAddress { get; set; } = Constants.UndefinedSlaveAddress;
            public int ChildSlaveAddress { get; set; } = Constants.UndefinedSlaveAddress;
            public int ParentFunctionCode { get; set; } = Constants.DefaultForwardFuncCode;
        }
    }
}