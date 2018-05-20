using System.Runtime.InteropServices;
using Modbus.Core.Converters;

namespace Modbus.Core.DataTypes
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RequestFunc01
    {
        [Endian(Endianness.BigEndian)]
        public ushort firstCoilAddress;

        [Endian(Endianness.BigEndian)]
        public ushort numberOfCoils;
    }
}