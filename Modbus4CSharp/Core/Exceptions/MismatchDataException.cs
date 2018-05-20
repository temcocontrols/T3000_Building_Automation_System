using System;

namespace Modbus.Core.Exceptions
{
    public class MismatchDataException : Exception
    {
        public MismatchDataException()
        {
        }

        public MismatchDataException(string message) : base(message)
        {
        }
    }
}