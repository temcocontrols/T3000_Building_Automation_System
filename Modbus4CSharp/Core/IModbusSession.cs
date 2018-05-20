using System;
using System.Threading.Tasks;

namespace Modbus.Core
{
    public interface IModbusSession : IDisposable
    {
        SessionState State { get; }

        bool EnableCheck { get; set; }

        Response<T> SendRequest<T>(int functionCode, object data) where T : struct;

        Response<T> SendRequest<T>(Request.BuilderBase builder) where T : struct;

        Task<Response<T>> SendRequestAsync<T>(int functionCode, object data) where T : struct;

        Task<Response<T>> SendRequestAsync<T>(Request.BuilderBase builder) where T : struct;
    }
}