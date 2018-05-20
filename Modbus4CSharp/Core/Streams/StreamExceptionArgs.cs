using System;

namespace Modbus.Core.Streams
{
    public delegate void StreamExceptionEventHandler(object sender, StreamExceptionArgs e);

    public class StreamExceptionArgs : EventArgs
    {
        public ICommStream Stream { get; }

        public Exception Exception { get; }

        public StreamExceptionArgs(ICommStream stream, Exception exception)
        {
            this.Stream = stream;
            this.Exception = exception;
        }
    }
}