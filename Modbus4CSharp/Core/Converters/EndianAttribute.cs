using System;

namespace Modbus.Core.Converters
{
    [AttributeUsage(AttributeTargets.Field)]
    public class EndianAttribute : Attribute
    {
        public Endianness Endianness { get; private set; }

        public EndianAttribute(Endianness endianness)
        {
            this.Endianness = endianness;
        }
    }

    public enum Endianness
    {
        BigEndian,
        LittleEndian
    }
}