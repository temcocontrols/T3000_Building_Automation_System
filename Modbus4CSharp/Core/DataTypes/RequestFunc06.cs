using System.Runtime.InteropServices;
using Modbus.Core.Converters;

namespace Modbus.Core.DataTypes
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RequestFunc06
    {
        [Endian(Endianness.BigEndian)]
        public ushort registerAddress;

        [Endian(Endianness.BigEndian)]
        public short value;
    }
}