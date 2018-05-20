using System;

namespace Modbus.Core.Exceptions
{
    public class DataCorruptedException : Exception
    {
        public DataCorruptedException()
        {
        }

        public DataCorruptedException(string message) : base(message)
        {
        }
    }
}