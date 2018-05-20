using System.Runtime.InteropServices;
using Modbus.Core.Converters;

namespace Modbus.Core.DataTypes
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RequestFunc02
    {
        [Endian(Endianness.BigEndian)]
        public ushort firstInputAddress;

        [Endian(Endianness.BigEndian)]
        public ushort numberOfCoils;
    }
}