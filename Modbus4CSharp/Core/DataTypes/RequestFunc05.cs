using System.Runtime.InteropServices;
using Modbus.Core.Converters;

namespace Modbus.Core.DataTypes
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RequestFunc05
    {
        [Endian(Endianness.BigEndian)]
        public ushort coidAddress;

        [Endian(Endianness.BigEndian)]
        public short status;
    }
}