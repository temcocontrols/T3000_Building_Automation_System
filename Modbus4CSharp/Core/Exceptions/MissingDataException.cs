using System;

namespace Modbus.Core.Exceptions
{
    public class MissingDataException : Exception
    {
        public MissingDataException()
        {
        }

        public MissingDataException(string message) : base(message)
        {
        }
    }
}