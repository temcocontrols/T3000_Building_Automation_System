using Modbus.Core.Exceptions;

namespace Modbus.Core.Streams
{
    public interface IBroadcastStream : ICommStream
    {
        event StreamExceptionEventHandler StreamException;

        void Add(ICommStream streamMember);

        void Remove(ICommStream streamMember);
    }
}